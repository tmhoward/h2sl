/**
 * @file symboldictionary-demo.cc
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

#include "h2sl/symboldictionary.h"

int main( int argc, char* argv[] ){
  std::cout << "Start of the h2sl::SymbolDictionary class demo program" << std::endl;

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

  // Create a SymbolDictionary using the default constructor
  h2sl::SymbolDictionary default_symdict = h2sl::SymbolDictionary();
  std::cout << "Default SymbolDictionary constructed: " << default_symdict
      <<"\n" << std::endl;

  // ************************************************************************* //
  // Create 2 SymbolCompositionalInfo objects to be used by a SymbolDictionary
  // ************************************************************************* //

  // Create a SymbolCompositionalInfo of multiple Symbols of type "spatial_relation"
  auto sr_symcompinfo =
            std::make_shared<h2sl::SymbolCompositionalInfo>("spatial_relation");
  auto symbol_props = h2sl::Symbol::propertiesMap({{"spatial_relation_type", "near"}});
  h2sl::Symbol first_spatial_relation_symbol = h2sl::Symbol("spatial_relation",
                                                                symbol_props );
  symbol_props.clear();
  symbol_props = h2sl::Symbol::propertiesMap({{"spatial_relation_type", "far"}});
  h2sl::Symbol second_spatial_relation_symbol = h2sl::Symbol("spatial_relation",
                                                                  symbol_props );
  // Scrape the created Symbols; Pass in an empty world since the symbols are not Objects
  sr_symcompinfo->scrape_symbol( "PP", first_spatial_relation_symbol );
  sr_symcompinfo->scrape_symbol( "PP", second_spatial_relation_symbol );

  // Create a SymbolCompositionalInfo of multiple Symbols of type "object_type"
  auto ot_symcompinfo = std::make_shared<h2sl::SymbolCompositionalInfo>("object_type");
  symbol_props.clear();
  symbol_props = h2sl::Symbol::propertiesMap({{"object_type", "ball"}});
  h2sl::Symbol first_object_type_symbol = h2sl::Symbol("object_type", symbol_props );

  symbol_props.clear();
  symbol_props = h2sl::Symbol::propertiesMap({{"object_type", "block"}});
  h2sl::Symbol second_object_type_symbol = h2sl::Symbol("object_type", symbol_props );

  // Scrape the created symbols; Pass in an empty world since the symbols are not Objects
  ot_symcompinfo->scrape_symbol( "NP", first_object_type_symbol );
  ot_symcompinfo->scrape_symbol( "PP", second_object_type_symbol );

  // ************************************************************************* //
  // Insert the two SymbolCompositionalInfo objects onto default_symdict
  // ************************************************************************* //
  default_symdict.dictionary.insert(std::make_pair(
                                  sr_symcompinfo->symbol_type, sr_symcompinfo ));
  default_symdict.dictionary.insert(std::make_pair(
                                  ot_symcompinfo->symbol_type, ot_symcompinfo ));

  std::cout << "SymbolDictionary with scraped Symbols: " << default_symdict
            << "\n" << std::endl;

  // Create a SymbolDictionary using the copy constructor
  h2sl::SymbolDictionary copied_symdict = h2sl::SymbolDictionary( default_symdict );
  std::cout << "Copied SymbolDictionary constructed: " << copied_symdict
            << "\n" << std::endl;

  // Import a user-provided SymbolSpace from XML
  if( vm.count( "input" ) ){
    std::cout << "Importing a SymbolDictionary from \""
              << vm["input"].as<std::string>() << "\"" << std::endl;
    default_symdict.from_xml( vm["input"].as<std::string>().c_str() );
    std::cout << "Imported SymbolDictionary: " << default_symdict
              << "\n" << std::endl;
  } else{
    std::cout << "No input XML file provided.\n" << std::endl;
  }

  // Export the user-provided SymbolDictionary to XML
  if( vm.count( "output" ) ){
    std::cout << "Exporting a SymbolDictionary to \""
              << vm["output"].as<std::string>() << "\"" << std::endl;
    std::cout << "The SymbolDictionary is: " << default_symdict << std::endl;
    default_symdict.to_xml( vm["output"].as<std::string>().c_str() );
  }

  // Create a SymbolDictionary using the move constructor
  h2sl::SymbolDictionary moved_symdict =
                          h2sl::SymbolDictionary(std::move( default_symdict ));
  std::cout << "Moved SymbolDictionary constructed: " << moved_symdict
            << "\n" << std::endl;

  // Export XML then Import XML and compare
  const char* filename = "/tmp/h2sl-symboldictionary-demo-exported-symboldictionary.xml";

  moved_symdict.to_xml(filename);
  std::cout << "Export finished" << std::endl;
  h2sl::SymbolDictionary i_symdict = h2sl::SymbolDictionary();
  bool e_result = i_symdict.from_xml(filename);
  std::cout << "Exported XML SymbolDictionary: " << moved_symdict << std::endl;
  std::cout << "Imported XML SymbolDictionary: " << i_symdict << std::endl;
  std::cout << "h2sl::SymbolDictionary::from_xml() return: " << e_result << std::endl;

  std::cout << "End of the h2sl::SymbolDictionary class demo program" << std::endl;
  return EXIT_SUCCESS;
}
