/**
 * @file symbolspace-demo.cc
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

#include "h2sl/world.h"
#include "h2sl/symboldictionary.h"
#include "h2sl/symbolspace.h"

int main( int argc, char* argv[] ){
  std::cout << "Start of the h2sl::SymbolSpace class demo program" << std::endl;

  boost::program_options::options_description desc("Options");
  desc.add_options()("help.h", "Help Screen")(
    "input,i", boost::program_options::value<std::string>(), "input_file")(
    "world,i", boost::program_options::value<std::string>(), "world_file")(
    "symboldictionary,i", boost::program_options::value<std::string>(), "symboldictionary_file")(
    "output,o", boost::program_options::value<std::string>(), "output file");
  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if( vm.count("help") ){
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  // Create a SymbolSpace using the default constructor
  h2sl::SymbolSpace default_symSpace = h2sl::SymbolSpace();
  std::cout << "Default SymbolSpace constructed: " << default_symSpace
            << std::endl << std::endl;

  // Create multiple Symbols of type "object"
  h2sl::Symbol::propertiesMap symbol_props({{"name", "ball_0"},
                                            {"object_type", "ball"},
                                            {"position","0.0,0.0,0.0"}});
  h2sl::Symbol first_objSymbol = h2sl::Symbol( "object", symbol_props );

  symbol_props.clear();
  symbol_props = h2sl::Symbol::propertiesMap({{"name", "ball_1"},
                                              {"object_type", "ball"},
                                              {"position","1.0,0.0,0.0"}});
  h2sl::Symbol second_objSymbol = h2sl::Symbol( "object", symbol_props );

  h2sl::Symbol copy_second_objSymbol = h2sl::Symbol( second_objSymbol );

  // Create multiple Symbols of type "spatial_relation"
  symbol_props.clear();
  symbol_props = h2sl::Symbol::propertiesMap({{"spatial_relation_type", "near"}});
  h2sl::Symbol first_spatialRelationSymbol = h2sl::Symbol("spatial_relation", 
                                                          symbol_props );
  symbol_props.clear();
  symbol_props = h2sl::Symbol::propertiesMap({{"spatial_relation_type", "far"}});
  h2sl::Symbol second_spatialRelationSymbol = h2sl::Symbol("spatial_relation", 
                                                            symbol_props );

  // Insert the Symbols
  default_symSpace.insert( first_objSymbol );
  default_symSpace.insert( second_objSymbol );
  default_symSpace.insert( first_spatialRelationSymbol );
  default_symSpace.insert( second_spatialRelationSymbol );

  std::cout << "SymbolSpace with inserted Symbols: " << default_symSpace
            << std::endl << std::endl;

  // Insert a copy of one of the Symbols
  auto ptr_second_objSymbol = default_symSpace.insert( copy_second_objSymbol );
  std::cout << "SymbolSpace after inserting duplicate Symbol: " << default_symSpace
            << std::endl << std::endl;

  // Search for the copy
  auto ptr_found_symbol = default_symSpace.find_symbol( copy_second_objSymbol );
  if( ptr_found_symbol != nullptr ){
    std::cout << "Searched for copy_second_objSymbol, returned: " 
              << *ptr_found_symbol << std::endl << std::endl;
  } else{
    std::cout << "Searched for copy_second_objSymbol, returned nullptr"
              << std::endl << std::endl;
  }

  // Search for a symbol NOT in default_symSpace
  ptr_found_symbol = default_symSpace.find_symbol( h2sl::Symbol() );
  if( ptr_found_symbol != nullptr ){
    std::cout << "Searched for an empty Symbol, returned: " << *ptr_found_symbol
              << std::endl << std::endl;
  } else{
    std::cout << "Searched for an empty Symbol, returned nullptr"
              << std::endl << std::endl;
  }
  

  // Check if ptr_second_objSymbol points to the same Symbol as in default_symSpace
  std::cout << "*(default_symSpace[second_objSymbol.type].back()): "
            << *(default_symSpace.symbolsMap[second_objSymbol.type].back()) << std::endl;
  std::cout << "default_symSpace[second_objSymbol.type].back(): "
            << default_symSpace.symbolsMap[second_objSymbol.type].back() << std::endl;
  std::cout << "default_symSpace[second_objSymbol.type].back().use_count(): "
            << default_symSpace.symbolsMap[second_objSymbol.type].back().use_count()
            << std::endl;
  std::cout << "ptr_second_objSymbol: " << ptr_second_objSymbol << std::endl;
  std::cout << "ptr_second_objSymbol.use_count(): " 
            << ptr_second_objSymbol.use_count() << std::endl;
 
  // Create a SymbolSpace using the copy constructor
  h2sl::SymbolSpace copied_symSpace = h2sl::SymbolSpace( default_symSpace );
  std::cout << "Copied SymbolSpace constructed: " << copied_symSpace
            << std::endl << std::endl;

  // Import a user-provided SymbolSpace from XML
  if( vm.count( "input" ) ){
    std::cout << "Importing a SymbolSpace from \"" << vm["input"].as<std::string>()
              << "\"" << std::endl;
    default_symSpace.from_xml( vm["input"].as<std::string>().c_str() );
    std::cout << "Imported SymbolSpace: " << default_symSpace << std::endl << std::endl;
  } else{
    std::cout << "No input XML file provided." << std::endl << std::endl;
  }

  // Import a user-provided SymbolSpace from a SymbolDictionary and World
  if( vm.count( "symboldictionary" ) && vm.count( "world" ) ){
    std::shared_ptr<h2sl::SymbolDictionary> symboldictionary = std::make_shared<h2sl::SymbolDictionary>( vm["symboldictionary"].as<std::string>().c_str() );
    auto world = std::make_shared<h2sl::World>();
    world->from_xml(vm["world"].as<std::string>());
    default_symSpace.fill( symboldictionary, world );
  } else {
    std::cout << "No symboldictionary or world file provided." << std::endl << std::endl;
  }

  // Export the user-provided SymbolSpace to XML
  if( vm.count( "output" ) ){
    std::cout << "Exporting a SymbolSpace to \"" << vm["output"].as<std::string>()
              << "\"" << std::endl;
    std::cout << "The SymbolSpace is: " << default_symSpace << std::endl;
    default_symSpace.to_xml( vm["output"].as<std::string>().c_str() );
  }

  // Create a SymbolSpace using the move constructor
  h2sl::SymbolSpace moved_symSpace = h2sl::SymbolSpace(std::move( default_symSpace ));
  std::cout << "Moved SymbolSpace constructed: " << moved_symSpace
            << std::endl << std::endl;

  // Export XML then Import XML and compare
  const char* filename = "/tmp/h2sl-symbolspace-demo-exported-symbolspace.xml";
  default_symSpace.to_xml(filename);
  h2sl::SymbolSpace i_symSpace = h2sl::SymbolSpace();
  bool e_result = i_symSpace.from_xml(filename);
  std::cout << "Exported XML SymbolSpace: " << default_symSpace << std::endl;
  std::cout << "Imported XML SymbolSpace: " << i_symSpace << std::endl;
  std::cout << "h2sl::SymbolSpace::from_xml() return: " << e_result << std::endl;

  std::cout << "End of the h2sl::SymbolSpace class demo program" << std::endl;
  return EXIT_SUCCESS;
}
