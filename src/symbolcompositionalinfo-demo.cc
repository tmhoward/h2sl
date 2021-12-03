/**
 * @file symbolcompositionalinfo-demo.cc
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

#include "h2sl/symbolcompositionalinfo.h"

int main( int argc, char* argv[] ){
  std::cout << "Start of the h2sl::SymbolCompositionalInfo class demo program"
            << std::endl;

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

  // Create a SymbolCompositionalInfo using the default constructor
  h2sl::SymbolCompositionalInfo default_symcompinfo = h2sl::SymbolCompositionalInfo();
  std::cout << "Default SymbolCompositionalInfo constructed: " << default_symcompinfo
      <<"\n" << std::endl;

  // Create multiple Symbols of type "spatial_relation"
  auto symbol_props = h2sl::Symbol::propertiesMap({{"spatial_relation_type", "near"}});
  h2sl::Symbol first_spatial_relation_symbol = h2sl::Symbol("spatial_relation",
                                                                symbol_props );
  symbol_props.clear();
  symbol_props = h2sl::Symbol::propertiesMap({{"spatial_relation_type", "far"}});
  h2sl::Symbol second_spatial_relation_symbol = h2sl::Symbol("spatial_relation",
                                                                  symbol_props );

  // Scrape the created Symbols; Pass in an empty world since the symbols are not Objects
  default_symcompinfo.symbol_type = "spatial_relation";
  default_symcompinfo.scrape_symbol( "PP", first_spatial_relation_symbol );
  default_symcompinfo.scrape_symbol( "PP", second_spatial_relation_symbol );


  std::cout << "SymbolCompositionalInfo with scraped Symbols: "
            << default_symcompinfo << "\n" << std::endl;

  // Create a SymbolCompositionalInfo using the copy constructor
  h2sl::SymbolCompositionalInfo copied_symcompinfo =
                            h2sl::SymbolCompositionalInfo( default_symcompinfo );
  std::cout << "Copied SymbolCompositionalInfo constructed: " << copied_symcompinfo
            << "\n" << std::endl;

  // Import a user-provided SymbolCompositionalInfo from XML
  if( vm.count( "input" ) ){
    std::cout << "Importing a SymbolCompositionalInfo from \""
              << vm["input"].as<std::string>() << "\"" << std::endl;
    default_symcompinfo.from_xml( vm["input"].as<std::string>().c_str() );
    std::cout << "Imported SymbolCompositionalInfo: " << default_symcompinfo
              << "\n" << std::endl;
  } else{
    std::cout << "No input XML file provided.\n" << std::endl;
  }

  // Export the user-provided SymbolCompositionalInfo to XML
  if( vm.count( "output" ) ){
    std::cout << "Exporting a SymbolCompositionalInfo to \""
              << vm["output"].as<std::string>() << "\"" << std::endl;
    std::cout << "The SymbolCompositionalInfo is: " << default_symcompinfo << std::endl;
    default_symcompinfo.to_xml( vm["output"].as<std::string>().c_str() );
  }

  // Create a SymbolCompositionalInfo using the move constructor
  h2sl::SymbolCompositionalInfo moved_symcompinfo =
                h2sl::SymbolCompositionalInfo(std::move( default_symcompinfo ));
  std::cout << "Moved SymbolCompositionalInfo constructed: " << moved_symcompinfo
            << "\n" << std::endl;

  // Export XML then Import XML and compare
  const char* filename =
  "/tmp/h2sl-symbolcompositionalinfo-demo-exported-symbolcompositionalinfo.xml";

  moved_symcompinfo.to_xml(filename);
  h2sl::SymbolCompositionalInfo i_symcompinfo = h2sl::SymbolCompositionalInfo();
  bool e_result = i_symcompinfo.from_xml(filename);
  std::cout << "Exported XML SymbolCompositionalInfo: " << moved_symcompinfo
            << std::endl;
  std::cout << "Imported XML SymbolCompositionalInfo: " << i_symcompinfo << std::endl;
  std::cout << "h2sl::SymbolCompositionalInfo::from_xml() return: "
            << e_result << std::endl;

  std::cout << "End of the h2sl::SymbolCompositionalInfo class demo program"
            << std::endl;
  return EXIT_SUCCESS;
}
