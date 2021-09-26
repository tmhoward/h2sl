/**
 * @file pcfg-generator.cc
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
#include "h2sl/pcfg.h"


int main( int argc, char* argv[] ){
  try{
    std::cout << "Start of the pcfg-generator program.\n" << std::endl;

    boost::program_options::options_description desc("Options");
    desc.add_options()("help,h", "Help Screen")(
      "sentences,s", boost::program_options::value<std::vector<std::string>>()->multitoken(), "sentence files from which to generate a pcfg")(
      "output,o", boost::program_options::value<std::string>(), "output file");
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
    if( vm.count("help") ){
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }

    if( !vm.count( "sentences" ) ){
      throw std::runtime_error( "No sentence files provided." );
    }
    auto sentence_filenames = vm["sentences"].as<std::vector<std::string>>();

    // Initialize a PCFG object that will extract a pcfg from the input sentences
    auto pcfg = h2sl::PCFG();
    // Initialize a container to store the input sentences as h2sl::Sentence 
    std::vector< std::shared_ptr<h2sl::Sentence> > sentences;
    // Construct a symbolspace to appease the Sentence from_xml API
    auto symbolspace = std::make_shared<h2sl::SymbolSpace>();
    
    for( const auto& sentence_filename : sentence_filenames ){
      std::cout << "processing_filename:\"" << sentence_filename << "\"" << std::endl;

      // Import the current sentence
      auto sentence = std::make_shared<h2sl::Sentence>();
      if( !sentence->from_xml( sentence_filename.c_str(), symbolspace ) ){
        std::stringstream error_msg;
        error_msg << "\nFailed to load sentence file \"" << sentence_filename <<"\"\n";
        throw std::runtime_error( error_msg.str() );
      }
      sentences.push_back( sentence );
    }

    // Use the pcfg object to extract a pcfg for the sentences
    std::cout << "scraping sentences" << std::endl;
    pcfg.scrape_sentences( sentences );
    std::cout << "done scraping sentences" << std::endl;

    if( vm.count( "output" ) ){
      std::cout << "Exporting the generated pcfg to \""
                << vm["output"].as<std::string>() << "\"\n";
      pcfg.to_xml( vm["output"].as<std::string>().c_str() );
    } else{
      std::cout << "No output location provided, exporting to \"/tmp/pcfg.xml\"\n";
      pcfg.to_xml( "/tmp/pcfg.xml" );
    }

    std::cout << "End of the pcfg generation program" << std::endl;
    return EXIT_SUCCESS;
  }
  catch( const std::runtime_error& error ){
    std::cerr << "terminate called after throwing an instance of 'std::runtime_error'"
              << std::endl;
    std::cerr << error.what() << std::endl;
    return EXIT_FAILURE;
  }
}
