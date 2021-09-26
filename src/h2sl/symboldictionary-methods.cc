/**
 * @file symboldictionary-methods.cc
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

#include "h2sl/common.h"
#include "h2sl/symboldictionary-methods.h"

namespace h2sl{

//
// Method to scrape compositional info about Objects from a World
//
bool symboldictionary_scrape_world( SymbolDictionary& symbolDictionary,
                                    const std::shared_ptr<World>& world ){
  // Iterate through all objects in the world and add their semantic properties to symbolDictionary
  for( const auto& object : world->objects ){

    // Iterate through all of the properties of the object
    for( const auto& property : object.second.properties ){
  
      // To improve readability
      std::string property_key = property.first;
      std::string property_value = property.second;

      // Try inserting an entry of type property_key into the dictionary if it does not exist
      auto result = symbolDictionary.dictionary.emplace( property_key,
          std::make_shared<SymbolCompositionalInfo>( property_key ) );      
      
      // Get iterator to the inserted or existing entry (if insertion failed)
      auto it_symbol_compositional_info = result.first;

      // Try inserting the property key and property value to the properties map in symbolCompositionalInfo.   
      std::unordered_set<std::string> object_property_value = { property_value };
      auto it_properties = it_symbol_compositional_info->second->properties.emplace( property_key,
          object_property_value );
      // If such an entry already exists, just insert the property value
      if( !it_properties.second ){
        // The emplace method will not add the value if it already exists.
        it_properties.first->second.emplace( property_value );
      }

    } // End of loop over the properties of the object

    // TODO - Move the below code to a version of this function for probabilistic worlds
/*
    // Iterate through all of the property distributions of the object
    for( const auto& property_dist : object.second.property_distributions ){

      // To improve readability
      std::string property_key = property_dist.first;

      // Try inserting an entry of type property_key into the dictionary if it does not exist
      auto result = symbolDictionary.dictionary.emplace( property_key,
          std::make_shared<SymbolCompositionalInfo>( property_key ) );
      
      // Get iterator to the inserted or existing entry (if insertion failed)
      auto it_symbol_compositional_info = result.first;

      // Iterate through all possible property values for this distribution
      for( const auto& property : property_dist.second ){
        // Try inserting this property value into the properties map
	std::unordered_set<std::string> object_property_value = { property.first };
        auto it_properties = it_symbol_compositional_info->second->properties.emplace( property_key,
          object_property_value );

	      // If such an entry already exists, just insert the property value
	      if( !it_properties.second ){
	        it_properties.first->second.emplace( property.first );
	      }
      } // End of loop over property values in distribution
    } // End of loop over property distributions of the object
  */
  } // End of loop over the objects in the world

  return true;
}

//
// Method to scrape compositional info from a LanguageVariable and its Symbols
//
bool symboldictionary_scrape_language_variable( SymbolDictionary& symbolDictionary,
                                     const std::shared_ptr<LanguageVariable>& language_variable ){
  // Make a recursive call for each child language variable
  for( const auto& connection : language_variable->children ){
    symboldictionary_scrape_language_variable( symbolDictionary, connection.child );
  }

  // Iterate over all of the symbols in the language variable
  for( const auto& symbol : language_variable->symbols ){
    // skip if the symbol type is object
    if( symbol->type == "object" )
      continue;

    // Insert a new entry to the dictionary if one does not exist
    auto result = symbolDictionary.dictionary.emplace( symbol->type,
        std::make_shared<SymbolCompositionalInfo>( symbol->type ) );
	    
    // Get iterator to the inserted or existing entry
    auto it_symbol_compositional_info = result.first;

    // Scrape the symbol into the corresponding SymbolCompositionalInfo
    if( !it_symbol_compositional_info->second->scrape_symbol( language_variable->type, *symbol ) ){
      std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
      return false; // the scrape function failed
    }
  } // End of the loop over the symbols in the language variable

  return true;
}

//
// Method to scrape compositional info from multiple Sentence/World pairs by
// calling scrape_language_variable for each.
//
bool symboldictionary_scrape_world_sentence_pairs( SymbolDictionary& symbolDictionary, 
                                        vectorPairWorldSentence& wsPairs ){
  for( const auto& ws_pair : wsPairs ){

    if( !symboldictionary_scrape_world( symbolDictionary, ws_pair.first ) ){      
      std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
      return false; // the scrape_world function failed
    }

    if( !symboldictionary_scrape_language_variable( symbolDictionary, ws_pair.second->child ) ){      
      std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
      return false; // the scrape_language_variable function failed
    }
  }
  return true;
}

} // namespace h2sl
