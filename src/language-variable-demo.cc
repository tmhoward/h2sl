/**
 * @file language-variable-demo.cc
 * @author h2sl developers
 *
 * @version 2.0
 *
 * @section license
 *
 * this file is part of h2sl.
 *
 * this program is free software; you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation; either version 2 of the license, or (at
 * your option) any later version.
 * 
 * this program is distributed in the hope that it will be useful, but
 * without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the gnu
 * general public license for more details.
 * 
 * you should have received a copy of the gnu general public license
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the free
 * software foundation, inc., 51 franklin street, fifth floor, boston, ma
 * 02110-1301, usa.
 *
 * @section description
 */

#include <boost/program_options.hpp>

#include "h2sl/language-variable.h"

int main( int argc, char* argv[] ){
  std::cout << "Start of h2sl::LanguageVariable class demo program" << std::endl;

  boost::program_options::options_description desc{"Options"};
  desc.add_options()("help,h", "Help screen")(
      "input", boost::program_options::value<std::string>(), "input file")(
      "output", boost::program_options::value<std::string>(), "output file");

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  // Create an SymbolSpace for containing the Symbols
  auto symbolspace = std::make_shared<h2sl::SymbolSpace>();

  // Create a LanguageVariable using the default constructor
  h2sl::LanguageVariable default_language_variable = h2sl::LanguageVariable();
  std::cout << "Default LanguageVariable constructed: " << default_language_variable
            << std::endl << std::endl;

  // Create a child LanguageVariable
  auto child_language_variable = std::make_shared<h2sl::LanguageVariable>("NP");
  child_language_variable->name = "child";

  // Add Words to the child language variable
  child_language_variable->words = std::make_optional<std::vector<h2sl::Word>>();
  child_language_variable->words->push_back( h2sl::Word( "DT", "the", 1 ) ); 
  child_language_variable->words->push_back( h2sl::Word( "NN", "ball", 2 ) ); 
  
  // Create an object Symbol to add to the child language variable
  h2sl::Symbol::propertiesMap symbol_props({{"name", "ball_0"},
                                            {"object_type", "ball"},
                                            {"position", "0.0,0.0,0.0"}});
  h2sl::Symbol child_language_variable_symbol = h2sl::Symbol( "object", symbol_props );
  
  // Add the Symbol to the child language variable via the symbolspace
  child_language_variable->symbols.push_back( symbolspace->insert(child_language_variable_symbol) );
  std::cout << "Child LanguageVariable constructed: " << *child_language_variable << std::endl;
  std::cout << "Current symbolspace: " << symbolspace << std::endl;
  std::cout << "Current use_count for child_language_variable's Symbol: "
            << child_language_variable->symbols.back().use_count() << std::endl << std::endl;

  // Update default_language_variable to be "near the ball" with child_language_variable as a child
  default_language_variable.type = "PP";
  default_language_variable.name = "parent";
  default_language_variable.words = std::make_optional<std::vector<h2sl::Word>>(); 
  default_language_variable.words->push_back( h2sl::Word("IN", "near", 0) );
  default_language_variable.children.emplace_back( std::nullopt, child_language_variable );
  
  // Give it two roles to test their I/O
  default_language_variable.roles = std::make_optional<h2sl::LanguageVariable::rolesMap>();
  default_language_variable.roles->emplace( "mode", "declarative" );
  default_language_variable.roles->emplace( "tense", "present" );

  // Assign it a Region symbol via the symbolspace
  symbol_props.clear();
  symbol_props = h2sl::Symbol::propertiesMap({{"spatial_relation_type","near"},
                                              {"object","ball_0"}});
  h2sl::Symbol default_language_variable_symbol = h2sl::Symbol( "region", symbol_props );
  default_language_variable.symbols.push_back( symbolspace->insert(default_language_variable_symbol) );
  std::cout << "Default LanguageVariable updated: " << default_language_variable << std::endl;
  std::cout << "Current symbolspace: " << *symbolspace << std::endl;
  std::cout << "Current use_count for child_language_variable's Symbol: "
            << child_language_variable->symbols.back().use_count() << std::endl << std::endl;

  // Create a LanguageVariable using the copy constructor
  h2sl::LanguageVariable copied_language_variable = h2sl::LanguageVariable( default_language_variable );
  std::cout << "Copied LanguageVariable constructed: " << copied_language_variable << std::endl;
  std::cout << "Current symbolspace: " << *symbolspace << std::endl;
  std::cout << "Current use_count for child_language_variable's Symbol: "
            << child_language_variable->symbols.back().use_count() << std::endl << std::endl;

  // Create a LanguageVariable using the move constructor
  h2sl::LanguageVariable moved_language_variable = h2sl::LanguageVariable( default_language_variable );
  std::cout << "Moved LanguageVariable constructed: " << moved_language_variable << std::endl;
  std::cout << "Current symbolspace: " << *symbolspace << std::endl;
  std::cout << "Current use_count for child_language_variable's Symbol: "
            << child_language_variable->symbols.back().use_count() << std::endl << std::endl;
  
  // Export XML then Import XML and compare
  const char* filename = "/tmp/h2sl-language-variable-demo-exported-language-variable.xml";
  std::cout << "Exporting Default LanguageVariable to xml..." << std::endl;
  default_language_variable.to_xml(filename);

  std::cout << "Importing Default LanguageVariable back from xml..." << std::endl;
  h2sl::LanguageVariable i_language_variable = h2sl::LanguageVariable();
  bool e_result = i_language_variable.from_xml( filename, symbolspace );
  std::cout << "Exported XML LanguageVariable: " << default_language_variable << std::endl;
  std::cout << "Imported XML LanguageVariable: " << i_language_variable << std::endl;
  std::cout << "h2sl::LanguageVariable::from_xml() return: " << e_result
            << std::endl << std::endl;

  // Test to/from message methods
  std::cout << "Converting Default LanguageVariable to message..." << std::endl;
  h2sl::LanguageVariableMessage msg = default_language_variable.to_msg();
  std::cout << "LV exported to message:\n" << default_language_variable << std::endl;

  std::optional<h2sl::LanguageVariable> lv_from_msg = h2sl::LanguageVariable::from_msg( msg );
  if( !lv_from_msg ){
    std::cout << "Unable to convert back from message" << std::endl;
  } else{
    std::cout << "LV imported from message:\n" << *lv_from_msg << std::endl;
  }

  std::cout << "Ready to print message..." << std::endl;
  std::cin.get();
  std::cout << msg << std::endl;

  // Import a user-provided LanguageVariable from XML
  if( vm.count("input") ){
    std::cout << "Importing a LanguageVariable from \"" << vm["input"].as<std::string>()
              << "\"" << std::endl;
    bool import_result = default_language_variable.from_xml( vm["input"].as<std::string>().c_str(), symbolspace );
    std::cout << "Imported LanguageVariable: " << default_language_variable << "\n\n";
    std::cout << "h2sl::LanguageVariable::from_xml() return: " << import_result << "\n" << std::endl;
  } else{
    std::cout << "No input XML file provided." << std::endl << std::endl;
  }

  // Export the user-provided LanguageVariable to XML
  if( vm.count("output") ){
    std::cout << "Exporting a LanguageVariable to \"" << vm["output"].as<std::string>().c_str()
              << "\"" << std::endl;
    std::cout << "The LanguageVariable is: " << default_language_variable << std::endl;
    default_language_variable.to_xml( vm["output"].as<std::string>().c_str() );
  }

  std::cout << "End of the h2sl::LanguageVariable class demo program" << std::endl;
  return EXIT_SUCCESS;
}
