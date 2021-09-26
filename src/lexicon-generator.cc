/**
 * @file lexicon-generator.cc
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
#include "h2sl/lexicon.h"

int main( int argc, char* argv[] ){
  std::cout << "\nStart of the lexicon generation program" << std::endl << std::endl;

  boost::program_options::options_description desc("Options");
  desc.add_options()("help,h", "Help Screen")(
    "examples,e", boost::program_options::value<std::vector<std::string>>()->multitoken(), "example files")(
    "output,o", boost::program_options::value<std::string>(), "output file");
  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if( vm.count("help") ){
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  // get vector of training examples 
  if( !vm.count( "examples" ) ){
    std::cout << "No example files provided, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }
  std::vector< std::string > example_filenames = vm["examples"].as<std::vector<std::string>>();

  // Declare containers for lexicon and sentences
  h2sl::Lexicon lexicon = h2sl::Lexicon();
  std::vector< std::shared_ptr<h2sl::Sentence> > sentences;

  for( auto & example_filename : example_filenames ){
    std::cout << "Processing filename:\"" << example_filename << "\"" << std::endl;

    // Construct a sentence with an empty language variable child
    auto p_sentence = std::make_shared<h2sl::Sentence>();

    // Load language content into the sentence's child member
    std::optional<h2sl::LanguageVariable> lv = h2sl::LanguageVariable::flattened_from_xml( example_filename.c_str() );
    if( !lv ){
      std::cout << "Failed to load language variable via LanguageVariable::flattened_from_xml()."
                << "\nAttempting to load via h2sl::Sentence::from_xml()." << std::endl;
      if( !p_sentence->from_xml( example_filename.c_str() ) ){
        std::stringstream error_msg;
        error_msg << "Failed to load a sentence OR a flattened LV from " << example_filename << std::endl;
        throw std::runtime_error( error_msg.str() );
      }
    } else{
      p_sentence->child = std::make_shared<h2sl::LanguageVariable>( lv.value() );
    }

    // Make Sentence and push onto a vector
    sentences.push_back( p_sentence );
  }

  // call function to scrape sentences into a lexicon
  lexicon.scrape_sentences( sentences );
  std::cout << "\n" << lexicon << std::endl;

  if( vm.count( "output" ) ){
    lexicon.to_xml( vm["output"].as<std::string>().c_str() );
  }

  std::cout << "End of the lexicon generation program\n" << std::endl;
  return EXIT_SUCCESS;
}
