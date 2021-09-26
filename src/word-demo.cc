/**
 * @file word-demo.cc
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

#include "h2sl/word.h"

int main( int argc, char* argv[] ){
  std::cout << "Start of the h2sl::Word class demo program" << std::endl;

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

  // Create a Word using the default constructor and default values
  h2sl::Word default_word = h2sl::Word();
  std::cout << "Default Word constructed: " << default_word << std::endl;
  std::cout << "Default values: pos( " << default_word.pos << " ), text(\""
            << default_word.text << "\"), time( " << default_word.time << " )"
            << std::endl << std::endl;

  // Create a Word using the default constructor and specified values
  h2sl::Word specific_word = h2sl::Word( "NN", "ball", 1 );
  std::cout << "Specific Word constructed: " << specific_word << std::endl << std::endl;

  // Create a Word using the copy constructor
  h2sl::Word copied_word = h2sl::Word( specific_word );
  std::cout << "Copied Word constructed: " << copied_word << std::endl << std::endl;

  // Create a Word using the move constructor
  h2sl::Word moved_word = h2sl::Word(std::move( specific_word ));
  std::cout << "Moved word constructed: " << moved_word << std::endl;
  std::cout << "Original word: " << copied_word << std::endl << std::endl;

  // Export XML then Import XML and compare
  const char* filename = "/tmp/h2sl-word-demo-exported-word.xml";
  h2sl::Word e_word = h2sl::Word( "JJ", "heavy", 3 );
  e_word.to_xml(filename);
  h2sl::Word i_word = h2sl::Word();
  bool e_result = i_word.from_xml(filename);
  std::cout << "Exported XML Word: " << e_word << std::endl;
  std::cout << "Imported XML Word: " << i_word << std::endl;
  std::cout << "h2sl::Word::from_xml() return: " << e_result << std::endl;
  if( (e_word.pos == i_word.pos) && (e_word.text == i_word.text) &&
      (e_word.time == i_word.time ) )
  {
    std::cout << "Export then Import XML success!" << std::endl << std::endl;
  } else{
    std::cout << "Export then Import XML failure!" << std::endl << std::endl;
  }
  

  // Import a user-provided Word from XML
  h2sl::Word input_word = h2sl::Word();
  if( vm.count( "input" ) ){
    std::cout << "Importing a Word from \"" << vm["input"].as<std::string>()
              << "\"" << std::endl;
    input_word.from_xml( vm["input"].as<std::string>().c_str() );
    std::cout << "Imported Word: " << input_word << std::endl << std::endl;
  } else{
    std::cout << "No input XML file provided." << std::endl << std::endl;
  }

  // Export the user-provided Word to XML
  if( vm.count( "output" ) ){
    std::cout << "Exporting a Word to \"" << vm["output"].as<std::string>()
              << "\"" << std::endl;
    std::cout << "The Word is: " << input_word << std::endl;
    input_word.to_xml( vm["output"].as<std::string>().c_str() );
  }

  std::cout << "End of the h2sl::Word class demo program" << std::endl;
  return EXIT_SUCCESS;
}
