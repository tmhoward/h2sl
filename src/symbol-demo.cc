/**
 * @file symbol-demo.cc
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

#include "h2sl/symbol.h"

int main( int argc, char* argv[] ){
  std::cout << "Start of the h2sl::Symbol class demo program" << std::endl;

  boost::program_options::options_description desc("Options");
  desc.add_options()("help.h", "Help Screen")(
    "input,i", boost::program_options::value<std::string>(), "input_file")(
    "output,o", boost::program_options::value<std::string>(), "output file");
  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if( vm.count("help") ){
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  // Create a Symbol using the default constructor and default values
  h2sl::Symbol default_symbol = h2sl::Symbol();
  std::cout << "Default Symbol constructed: " << default_symbol 
            << std::endl << std::endl;

  // Create a Symbol using the default constructor and specified values
  // There are no children included
  h2sl::Symbol::propertiesMap symbol_props({{"name", "ball_0"},
                                            {"object_type", "ball"},
                                            {"position","0.0,0.0,0.0"}});
  h2sl::Symbol specific_symbol = h2sl::Symbol( "object", symbol_props );
  std::cout << "Specific Symbol constructed: " << specific_symbol 
            << std::endl << std::endl;


  // Make two more "object" Symbols to be used as a sibling of "specific_symbol"
  // Copy specific_symbol to "first_child_symbol" for naming clarity
  h2sl::Symbol first_child_symbol = h2sl::Symbol( specific_symbol );

  symbol_props.clear();
  symbol_props = h2sl::Symbol::propertiesMap({{"name", "ball_1"},
                                              {"object_type", "ball"},
                                              {"position","1.0,0.0,0.0"}});
  h2sl::Symbol second_child_symbol = h2sl::Symbol( "object", symbol_props );

  symbol_props.clear();
  symbol_props = h2sl::Symbol::propertiesMap({{"name", "ball_2"},
                                              {"object_type", "ball"},
                                              {"position","2.0,0.0,0.0"}});
  h2sl::Symbol third_child_symbol = h2sl::Symbol( "object", symbol_props );
  
  // Make a parent Container Symbol and add the above Symbols as children
  symbol_props.clear();
  symbol_props = h2sl::Symbol::propertiesMap({{"container_type", "row"}});
  std::vector<h2sl::Symbol> children{ first_child_symbol,
                                      second_child_symbol,
                                      third_child_symbol };

  h2sl::Symbol parent_symbol = h2sl::Symbol("container", symbol_props, children);
  std::cout << "Nested Symbol constructed: " << parent_symbol
            << std::endl << std::endl;

  // Create a Symbol using the copy constructor
  h2sl::Symbol copied_symbol = h2sl::Symbol( specific_symbol );
  std::cout << "Copied Symbol constructed: " << copied_symbol
            << std::endl << std::endl;
  
  // Create a Symbol using the move constructor
  h2sl::Symbol moved_symbol = h2sl::Symbol(std::move( specific_symbol ));
  std::cout << "Moved Symbol constructed: " << moved_symbol << std::endl;
  std::cout << "Original Symbol: " << copied_symbol << std::endl << std::endl;

  // Export XML then Import XML and compare using parent_symbol
  const char* filename = "/tmp/h2sl-symbol-demo-exported-symbol.xml";
  parent_symbol.to_xml(filename);
  h2sl::Symbol i_symbol = h2sl::Symbol();
  bool e_result = i_symbol.from_xml(filename);

  std::cout << "Exported XML Symbol: " << parent_symbol << std::endl << std::endl;
  std::cout << "Imported XML Symbol: " << i_symbol << std::endl << std::endl;
  std::cout << "h2sl::Symbol::from_xml() return: " << e_result << std::endl;
  if( parent_symbol == i_symbol )
    std::cout << "Export then Import XML success!" << std::endl << std::endl;
  else
    std::cout << "Export then Import XML failure!" << std::endl << std::endl;

  // Import a user-provided Symbol from XML
  h2sl::Symbol input_symbol = h2sl::Symbol();
  if( vm.count( "input" ) ){
    std::cout << "Importing a Symbol from \"" << vm["input"].as<std::string>()
              << "\"" << std::endl;
    input_symbol.from_xml( vm["input"].as<std::string>().c_str() );
    std::cout << "Imported Symbol: " << input_symbol << std::endl << std::endl;
  } else{
    std::cout << "No input XML file provided." << std::endl << std::endl;
  }
  
  // Export the user-provided Symbol to XML
  if( vm.count( "output" ) ){
    std::cout << "Exporting a Symbol to \"" << vm["output"].as<std::string>()
              << "\"" << std::endl;
    std::cout << "The Symbol is: " << input_symbol << std::endl;
    input_symbol.to_xml( vm["output"].as<std::string>().c_str() );
  }

  std::cout << "End of the h2sl::Symbol class demo program" << std::endl;
  return EXIT_SUCCESS;
}
