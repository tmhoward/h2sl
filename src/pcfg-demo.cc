/**
 * @file pcfg-demo.cc
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

#include "h2sl/pcfg.h"

int main( int argc, char* argv[] ){
  try{
    std::cout << "Start of h2sl::PCFG class demo program" << std::endl;

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


    // Create a PCFG object to load the input from xml
    auto pcfg = h2sl::PCFG();
    
    // Import from the provided input
    if( !vm.count("input") ){
      throw std::runtime_error("No input string provided.");
    } else{
      std::cout << "Importing a pcfg from \"" << vm["input"].as<std::string>()
                << "\"\n";
      pcfg.from_xml( vm["input"].as<std::string>().c_str() );
    }

    // Export the user-provided PCFG to XML
    if( vm.count("output") ){
      std::cout << "Exporting the pcfg to \""
                << vm["output"].as<std::string>().c_str() << "\"\n";
      pcfg.to_xml( vm["output"].as<std::string>().c_str() );
    } else{
      std::cout << "No output location provided, exporting to "
                << "\"/tmp/pcfg_demo_output.xml\"\n";
      pcfg.to_xml( "/tmp/pcfg_demo_output.xml" );
    }

    std::cout << "End of the h2sl::PCFG class demo program" << std::endl;
    return EXIT_SUCCESS;
  }
  catch( const std::runtime_error& error ){
    std::cerr << "terminate called after throwing an instance of 'std::runtime_error'"
              << std::endl;
    std::cerr << error.what() << std::endl;
    EXIT_FAILURE;
  }
}
