/**
 * @file sentence-demo.cc
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

#include "h2sl/sentence.h"

int main( int argc, char* argv[] ){
  std::cout << "Start of h2sl::Sentence class demo program" << std::endl;

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

  // Create a child LanguageVariable
  auto np = std::make_shared<h2sl::LanguageVariable>("NP", "np1", h2sl::LanguageVariable::symbolsVector(),
                                                      std::vector<h2sl::LanguageVariable::language_variable_connection_t>() );
  np->words() = std::vector<h2sl::Word>();
  np->words()->push_back( h2sl::Word( "DT", "the", 2 ) );
  np->words()->push_back( h2sl::Word( "NN", "ball", 3 ) );
  np->symbols().push_back( symbolspace->insert(h2sl::Symbol( "object", {{"uid","ball0"}})) );

  // Create a child PP phrase
  auto pp = std::make_shared<h2sl::LanguageVariable>("PP", "pp1", h2sl::LanguageVariable::symbolsVector(),
                                                      std::vector<h2sl::LanguageVariable::language_variable_connection_t>() );
  pp->words() = std::vector<h2sl::Word>();
  pp->words()->push_back( h2sl::Word("IN", "near", 1) );
  pp->symbols().push_back( symbolspace->insert(h2sl::Symbol( "region", {{"spatial_relation","near"},{"uid","ball0"}})) );
  pp->children().emplace_back( std::nullopt, np );

  // Create a Sentence using the default constructor
  auto vp = std::make_shared<h2sl::LanguageVariable>("VP", "vp1", h2sl::LanguageVariable::symbolsVector(),
                                                      std::vector<h2sl::LanguageVariable::language_variable_connection_t>() );
  vp->words() = std::vector<h2sl::Word>();
  vp->words()->push_back( h2sl::Word("VB", "go", 0) );
  vp->symbols().push_back( symbolspace->insert(h2sl::Symbol( "action", {{"action_type","navigation"},{"spatial_relation","near"},{"payload_uid","ball0"}})) );
  vp->children().emplace_back( std::nullopt, pp );

  h2sl::Sentence sentence = h2sl::Sentence( "operator", vp, vp->symbols() );

  // Import a user-provided Sentence from XML
  if( vm.count("input") ){
    std::cout << "Importing a Sentence from \"" << vm["input"].as<std::string>()
              << "\"" << std::endl;
    sentence.from_xml( vm["default"].as<std::string>().c_str(), symbolspace.get() );
  } else{
    std::cout << "No input XML file provided." << std::endl << std::endl;
  }

  std::cout << "sentence(" << sentence << ")" << std::endl << std::endl;

  // Export the user-provided Sentence to XML
  if( vm.count("output") ){
    std::cout << "Exporting a Sentence to \"" << vm["output"].as<std::string>().c_str()
              << "\"" << std::endl;
    std::cout << "The Sentence is: " << sentence << std::endl;
    sentence.to_xml( vm["output"].as<std::string>().c_str() );
  }

  std::cout << "End of the h2sl::Sentence class demo program" << std::endl;
  return EXIT_SUCCESS;
}
