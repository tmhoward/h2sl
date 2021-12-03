/**
 * @file trained-dcg-demo.cc
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

#include <chrono>
#include <boost/program_options.hpp>
#include "h2sl/dcg.h"
#include "h2sl/feature.h"
#include "h2sl/feature-pool.h"
#include "h2sl/language-variable.h"

int main( int argc, char* argv[] ){
  /************************************************************************************
  * Load the command-line arguments, construct the variables, generate the symbolspace
  *************************************************************************************/
  boost::program_options::options_description desc{"Options"};
  desc.add_options()("help,h", "Help screen")(
      "world,w", boost::program_options::value<std::string>(), "World file")(
      "symbol_dictionary,s", boost::program_options::value<std::string>(), "Symbol dictionary file")(
      "llm,l", boost::program_options::value<std::string>(), "LLM file")(
      "example,e", boost::program_options::value<std::string>(), "Example file")(
      "output,o", boost::program_options::value<std::string>()->default_value("/tmp/dcg_search_output.xml"), "Output file");
  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  // Ensure that either a world or an example is provided
  auto p_world = std::make_shared<h2sl::World>();
  if( !( vm.count( "world" ) || vm.count( "example" ) ) ){
    std::cout << "No world file provided, exiting." << std::endl;
    return EXIT_FAILURE;
  } else if ( vm.count( "world" ) && vm.count( "example" ) ){
    std::cout << "Both world and example files provided, using the world provided in \""
              << vm["world"].as<std::string>() << "\"." << std::endl;
    p_world->from_xml( vm["world"].as<std::string>().c_str() );
  } else{
    std::cout << "Using the world provided in the example file \"" << vm["example"].as<std::string>() << "\"." << std::endl;
    p_world->from_xml( vm["example"].as<std::string>().c_str() );
  }

  // Ensure a symbol dictionary is provided
  auto p_symbol_dictionary = std::make_shared<h2sl::SymbolDictionary>();
  if( !vm.count( "symbol_dictionary" ) ){
    std::cout << "No symbol dictionary file provided, exiting." << std::endl;
    return EXIT_FAILURE;
  } else{
    std::cout << "Using the symbol dictionary provided in \"" << vm["symbol_dictionary"].as<std::string>() << "\"." << std::endl;
    p_symbol_dictionary->from_file( vm["symbol_dictionary"].as<std::string>().c_str() );
  }

  // Ensure an example file is provided for the language
  auto p_sentence = std::make_shared< h2sl::Sentence >();
  if( !vm.count( "example" ) ){
    std::cout << "No example file provided, exiting." << std::endl;
    return EXIT_FAILURE;
  } else{
    auto example_filepath = vm["example"].as<std::string>();
    p_sentence->from_xml( example_filepath.c_str() );
    // Remove any annotated symbols
    p_sentence->symbols().clear();
    p_sentence->child()->clear_symbols();
  }

  // Ensure a log linear model file is provided
  auto p_llm = std::make_shared<h2sl::LLM>();
  if( !vm.count("llm") ){
    std::cout << "No log-linear model file provided, exiting." << std::endl;
    return EXIT_FAILURE;
  } else{
    std::cout << "Using the log-linear model provided in \"" << vm["llm"].as<std::string>() << "\"." << std::endl;
    p_llm->from_xml( vm["llm"].as<std::string>().c_str() );
  }

  // Generate the symbolspace from the symbol_dictionary and world
  auto p_symbolspace = std::make_shared<h2sl::SymbolSpace>();
  if( !p_symbolspace->fill( p_symbol_dictionary, p_world ) ){
    std::cout << "Failed to fill the symbolspace, exiting." << std::endl;
    return EXIT_FAILURE;
  }

  /************************************************************************************************
  * Create the DCG, solve the example (search), record the runtime, export the top solution to XML
  *************************************************************************************************/
  std::cout << "Constructing the DCG, solving the example." << std::endl;

  // Create the correspondence variables
  auto cvs = std::make_shared< std::vector< std::shared_ptr< std::string > > >();
  cvs->push_back( std::make_shared< std::string >( "false" ) );
  cvs->push_back( std::make_shared< std::string >( "true" ) );
  // Construct the DCG
  h2sl::DCG dcg { p_symbolspace, p_sentence, p_world, p_llm, cvs, true };

  // Solve the problem & record the runtime
  auto start_time = std::chrono::steady_clock::now();
  auto dcg_search_results = dcg.search( 1, true );
  auto stop_time = std::chrono::steady_clock::now();
  if( !dcg_search_results ){
    std::cout << "DCG::search() failed, exiting." << std::endl;
    return EXIT_FAILURE;
  }
  auto sec_runtime = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count()/1000.0;
  std::cout << "DCG::search() completed in " << sec_runtime << " seconds. " << std::endl;

  std::cin.get();

  if( dcg_search_results->empty() ){
    std::cout << "DCG::Search() did not provide any solutions, exiting." << std::endl;
    return EXIT_SUCCESS;
  }

  // Export the top solution to XML
  std::cout << "Exporting the top solution to \"" << vm["output"].as<std::string>() << "\"" << std::endl;
  tinyxml2::XMLDocument document;
  auto p_root = document.NewElement("root");
  document.InsertEndChild( p_root );
  p_world->to_xml( document, p_root );
  dcg_search_results->front().sentence.to_xml( document, p_root );
  document.SaveFile( vm["output"].as<std::string>().c_str() );

  return EXIT_SUCCESS;
}
