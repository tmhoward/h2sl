/**
 * @file language-client.cc
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
#include "h2sl/language-variable.h"

#include "ros/ros.h"
#include "h2sl/GroundLanguage.h"

int main( int argc, char* argv[] ){
  std::cout << "\nStart of the language client program" << std::endl << std::endl;

  boost::program_options::options_description desc{"Options"};
  desc.add_options()("help,h", "Help screen")(
    "example,e", boost::program_options::value<std::string>(), "language example");
  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  // Get language example
  if( !vm.count( "example" ) ){
    std::cout << "No language example provided, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }

  // Load language content into a LanguageVariable
  std::string example_filename = vm["example"].as<std::string>();
  auto lv = h2sl::LanguageVariable();
  lv.from_xml( example_filename.c_str() );

  // Clear any symbols the LV might have
  lv.clear_symbols();

  /******************************************************************
  * Initialize the ROS node
  *******************************************************************/
  ros::init( argc, argv, "language_client" );
  ros::NodeHandle n;

  ros::ServiceClient client = n.serviceClient<h2sl::GroundLanguage>( "ground_language" );

  // Populate the request part of the service
  h2sl::GroundLanguage srv;
  srv.request.language = lv.to_msg();

  if( client.call( srv ) ){
    // We can now access the srv.response member
    std::cout << "Service succeeded!" << std::endl;
    std::cout << "Search returned the symbols:" << std::endl;

    std::vector<h2sl::Symbol> result_symbols;
    for( const auto& symbol_msg : srv.response.symbols ){
      result_symbols.emplace_back( symbol_msg ); // Convert messages to h2sl::Symbols
      std::cout << result_symbols.back() << std::endl;
    }
  } else{
    ROS_ERROR( "Failed to call service ground_language" );
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
