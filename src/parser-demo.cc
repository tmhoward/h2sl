/**
 * @file parser-demo.cc
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

#include <iomanip>
#include <boost/program_options.hpp>

#include "h2sl/parser.h"


int main( int argc, char* argv[] ){
  try{
    std::cout << "Start of the parser demo program.\n" << std::endl;

    boost::program_options::options_description desc("Options");
    desc.add_options()("help,h", "Help Screen")(
      "input,i", boost::program_options::value<std::string>(), "input string to parse")(
      "pcfg,p", boost::program_options::value<std::string>(), "pcfg file location for use by the parser")(
      "output,o", boost::program_options::value<std::string>(), "output directory for parse solutions");
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
    if( vm.count("help") ){
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }

    if( !vm.count( "input" ) ){
      throw std::runtime_error("No input string provided.");
    }
    auto input_string = vm["input"].as<std::string>();

    if( !vm.count( "pcfg" ) ){
      throw std::runtime_error("No pcfg file provided.");
    }
    // Load the PCFG from file
    auto pcfg = h2sl::PCFG();
    pcfg.from_file( vm["pcfg"].as<std::string>().c_str() );
    std::cout << "Loaded the PCFG from file." << std::endl;

    // Create the parser from the PCFG
    h2sl::Parser parser (pcfg);

    // Display the binarized grammar rules
    std::cout << "Binary production rules" << std::endl;
    for( const auto& v_binary_production_rule :
                    parser.get_bpcfg().get_binary_production_rules() )
    {
      std::cout << "lhs: " << v_binary_production_rule.first << std::endl;
      for( const auto& binary_production_rule : v_binary_production_rule.second){
      std::cout << "  rhs: " << binary_production_rule.rhs.first << ", "
                << binary_production_rule.rhs.second << std::endl;
      }
    }
    std::cout << "\nX production rules" << std::endl;
    for( const auto& x_production_rule :
                    parser.get_bpcfg().get_x_production_rules() )
    {
      std::cout << "lhs: " << x_production_rule.first << ", "
                << "rhs: " << x_production_rule.second.rhs.first
                << ", " << x_production_rule.second.rhs.second << std::endl;
    }
    std::cout << std::endl;

    // Parse the input
    std::cout << "Parsing the input string \"" << input_string << "\"" << std::endl;
    auto ret_parse = parser.parse( input_string, 10 );

    // Export the parse solutions
    std::string filepath;
    if( vm.count( "output" ) )
      filepath = vm["output"].as<std::string>();
    else{
      std::cout << "No output location provided, exporting to \"/tmp/\"\n";
      filepath = "/tmp/";
    }
    for( size_t i = 0; i < ret_parse.size(); i++ ){
      std::stringstream filename;
      filename << filepath;
      filename << "parse_" << std::setfill('0') << std::setw(4) << i << ".xml";
      ret_parse[i].first.to_xml( filename.str().c_str() );
      std::cout << "The parse in file \"" << filename.str() << "\" had probability: "
                << ret_parse[i].second << std::endl;
    }
    std::cout << "End of the parser demo program" << std::endl;
    return EXIT_SUCCESS;
  }
  catch( const std::runtime_error& error ){
    std::cerr << "terminate called after throwing an instance of 'std::runtime_error'"
              << std::endl;
    std::cerr << error.what() << std::endl;
    return EXIT_FAILURE;
  }
}
