/**
 * @file dcg-demo.cc
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
#include "h2sl/dcg.h"

int main( int argc, char* argv[] ){

  boost::program_options::options_description desc{"Options"};
  desc.add_options()("help,h", "Help screen")(
      "input,i", boost::program_options::value<std::string>(), "Input file")(
      "llm,llm", boost::program_options::value<std::string>(), "LLM file")(
      "example,e", boost::program_options::value<std::string>(), "Example file")(
      "output,o", boost::program_options::value<std::string>(), "Output file");

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  if (vm.count("example")) {
    std::cout << "running example" << std::endl;
    h2sl::Dialogue dialogue( vm["example"].as<std::string>() );
    auto world = std::make_shared<h2sl::World>();
    world->from_xml(vm["example"].as<std::string>());
    auto symspace = std::make_shared<h2sl::SymbolSpace>();
    auto llm = std::make_shared<h2sl::LLM>();
    if( vm.count( "llm" ) ){
      llm->from_file( vm["llm"].as<std::string>() );
    } 
    std::cout << "world" << world << std::endl;
    std::cout << "dialogue:(" << dialogue << ")" << std::endl;
    // Construct the factor graph
    auto cvs = std::make_shared< std::vector< std::shared_ptr< std::string > > >();
    cvs->push_back( std::make_shared< std::string >( "false" ) );
    cvs->push_back( std::make_shared< std::string >( "true" ) );
    h2sl::DCG dcg { symspace, std::make_shared<h2sl::Sentence>(dialogue.sentences.back()), world, llm, cvs, true }; 
    // Search the graph
    dcg.search( 1, false );
  }

  return EXIT_SUCCESS;
}
