/**
 * @file symbol-dictionary-generator.cc
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

#include "h2sl/common.h"
#include "h2sl/language-variable.h"
#include "h2sl/sentence.h"
#include "h2sl/symbolspace.h"
#include "h2sl/symboldictionary-methods.h"
#include "h2sl/symboldictionary.h"

int main( int argc, char* argv[] ){
  std::cout << "\nStart of the symbol dictionary generation program" << std::endl << std::endl;

  boost::program_options::options_description desc("Options");
  desc.add_options()("help,h", "Help Screen")
    ("examples,e", boost::program_options::value<std::vector<std::string>>()->multitoken(), "example files")
    ("world-folder,w", boost::program_options::value<std::string>(), "world folder")
    ("output,o", boost::program_options::value<std::string>(), "output file");
  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if( vm.count("help") ){
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  // Get vector of training examples
  if( !vm.count( "examples" ) ){
    std::cout << "No example files provided, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }
  auto example_filepaths = vm["examples"].as<std::vector<std::string>>();

  // Declare containers for symbol dictionary and symbol space
  auto sd = h2sl::SymbolDictionary();
  auto symbolspace = std::make_shared<h2sl::SymbolSpace>();

  h2sl::vectorPairWorldSentence world_sentence_pairs;

  for( auto & example_filepath : example_filepaths ){
    std::cout << "\nProcessing filepath:\"" << example_filepath << "\"" << std::endl;
    // Construct a sentence from the XML file
    auto p_sentence = std::make_shared< h2sl::Sentence >( );
    p_sentence->from_xml( example_filepath.c_str(), symbolspace.get() );

    // Next, load world
    std::shared_ptr<h2sl::World> world  = std::make_shared<h2sl::World>();
    bool world_import_success = false;
    if( vm.count( "world-folder" ) ){ // Access world from elsewhere based on example filename
      // Extract world name from the filepath
      std::string example_name = example_filepath.substr( example_filepath.find_last_of( "/" ) + 1 );
      std::string world_name = example_name.substr( 0, example_name.find_first_of( "_" ) );
      std::cout << "World name found was " << world_name << std::endl;

      std::string world_filepath = vm["world-folder"].as<std::string>() + "/" + world_name + ".xml";

      world_import_success = world->from_xml( world_filepath );
    } else{ // Import world from the example
      world_import_success = world->from_xml( example_filepath );
    }

    if( world_import_success ){
      std::cout << "world:" << *world << std::endl;
    } else {
      std::stringstream error_msg;
      error_msg << "Failed to load world for " << example_filepath.c_str() << std::endl;
      throw std::runtime_error( error_msg.str() );
    }

    // Make a pair of the World and Sentence, push onto a vector
    auto world_sentence_pair = std::make_pair( world, p_sentence );
    world_sentence_pairs.push_back( world_sentence_pair );
  }

  // call function to scrape sentences into a symbol dictionary
  h2sl::symboldictionary_scrape_world_sentence_pairs(sd, world_sentence_pairs);
  // std::cout << "\nsd: " << sd << std::endl;

  if( vm.count( "output" ) ){
    sd.to_xml( vm["output"].as<std::string>().c_str() );
  }

  std::cout << "End of the symbol dictionary generation program\n" << std::endl;
  return EXIT_SUCCESS;
}
