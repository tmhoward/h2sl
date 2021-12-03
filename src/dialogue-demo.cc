/**
 * @file dialogue-demo.cc
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
#include "h2sl/dialogue.h"

int main( int argc, char* argv[] ){

  boost::program_options::options_description desc{"Options"};
  desc.add_options()("help,h", "Help screen")(
      "input,i", boost::program_options::value<std::string>(), "Input file")(
      "output,o", boost::program_options::value<std::string>(), "Output file");

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  auto symbolspace = std::make_shared<h2sl::SymbolSpace>();

  h2sl::Dialogue dialogue;

  // create the first sentence
  auto np0 = std::make_shared<h2sl::LanguageVariable>("NP", "np1", h2sl::LanguageVariable::symbolsVector(),
                                                      std::vector<h2sl::LanguageVariable::language_variable_connection_t>());
  np0->words() = std::vector<h2sl::Word>();
  np0->words()->push_back( h2sl::Word( "DT", "the", 2 ) );
  np0->words()->push_back( h2sl::Word( "NN", "ball", 3 ) );
  np0->symbols().push_back( symbolspace->insert(h2sl::Symbol( "object_type", {{"object_type","ball"}})) );

  auto pp0 = std::make_shared<h2sl::LanguageVariable>("PP", "pp1", h2sl::LanguageVariable::symbolsVector(),
                                                      std::vector<h2sl::LanguageVariable::language_variable_connection_t>());
  pp0->words() = std::vector<h2sl::Word>();
  pp0->words()->push_back( h2sl::Word("IN", "near", 1) );
  pp0->symbols().push_back( symbolspace->insert(h2sl::Symbol( "region", {{"spatial_relation","near"},{"object_type","ball"}})) );
  pp0->children().emplace_back( std::nullopt, np0 );

  auto vp0 = std::make_shared<h2sl::LanguageVariable>("VP", "vp1", h2sl::LanguageVariable::symbolsVector(),
                                                      std::vector<h2sl::LanguageVariable::language_variable_connection_t>());
  vp0->words() = std::vector<h2sl::Word>();
  vp0->words()->push_back( h2sl::Word("VB", "go", 0) );
  vp0->symbols().push_back( symbolspace->insert(h2sl::Symbol( "action", {{"action_type","navigation"},{"spatial_relation","near"},{"payload_type","ball"}})) );
  vp0->children().emplace_back( std::nullopt, pp0 );

  h2sl::Sentence sentence0( "operator", vp0, vp0->symbols() );

  dialogue.sentences().push_back( sentence0 );

  // create the second sentence
  auto whnp1 = std::make_shared<h2sl::LanguageVariable>("WHNP", "whnp1", h2sl::LanguageVariable::symbolsVector(),
                                                      std::vector<h2sl::LanguageVariable::language_variable_connection_t>());
  whnp1->words() = std::vector<h2sl::Word>();
  whnp1->words()->push_back( h2sl::Word( "WDT", "which", 4 ) );
  whnp1->words()->push_back( h2sl::Word( "NN", "ball", 5 ) );
  whnp1->symbols().push_back( symbolspace->insert(h2sl::Symbol("query",{{"object","ball"}})));

  h2sl::Sentence sentence1( "robot", whnp1, whnp1->symbols() );

  dialogue.sentences().push_back( sentence1 );

  // create the third sentence
  auto np2 = std::make_shared<h2sl::LanguageVariable>("NP", "np2", h2sl::LanguageVariable::symbolsVector(),
                                                      std::vector<h2sl::LanguageVariable::language_variable_connection_t>());
  np2->words() = std::vector<h2sl::Word>();
  np2->words()->push_back( h2sl::Word( "DT", "the", 6 ) );
  np2->words()->push_back( h2sl::Word( "JJS", "leftmost", 7 ) );
  np2->words()->push_back( h2sl::Word( "NN", "ball", 8 ) );
  np2->symbols().push_back( symbolspace->insert(h2sl::Symbol("object", {{"uid","ball0"}})));

  h2sl::Sentence sentence2( "operator", np2 );
  sentence2.symbols().push_back( symbolspace->insert(h2sl::Symbol( "action", {{"action_type","navigation"},{"spatial_relation","near"},{"payload_uid","ball0"}})));

  dialogue.sentences().push_back( sentence2 );

  if (vm.count("input")) {
    std::cout << "reading dialogue from \"" << vm["input"].as<std::string>() << "\"" << std::endl;
    dialogue.from_file(vm["input"].as<std::string>(), symbolspace.get() );
  }

  std::cout << dialogue << std::endl;

  if (vm.count("output")) {
    std::cout << "writing dialogue to \"" << vm["output"].as<std::string>() << "\"" << std::endl;
    dialogue.to_file(vm["output"].as<std::string>());
  }

  return EXIT_SUCCESS;
}
