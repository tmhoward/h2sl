/**
 * @file symbolspace.cc
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
#include "h2sl/symbolspace.h"

namespace h2sl{

//
// SymbolSpace class default constructor
//
SymbolSpace::SymbolSpace( const symbolsMapVectorType& symbolsMap ) :
                                                      symbolsMap( symbolsMap ){}

//
// SymbolSpace class constructor from XMLElement*
//
SymbolSpace::SymbolSpace( const tinyxml2::XMLElement* root ) : symbolsMap(){
  from_xml( root );
}

///
/// SymbolSpace class constructor to instantiate a SymbolSpace from a SymbolDictionary and a World
///
SymbolSpace::SymbolSpace( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ) : symbolsMap(){
  fill( symbolDictionary, world );
}

///
/// Method to fill a SymbolSpace from a SymbolDictionary and a World
///
bool SymbolSpace::fill( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  symbolsMap.clear();

  fill_objects( symbolDictionary, world );
  fill_object_types( symbolDictionary, world );
  fill_object_colors( symbolDictionary, world );
  fill_object_quantifiers( symbolDictionary, world );
  fill_object_semantic_property_types( symbolDictionary, world );
  fill_spatial_relation_extrema( symbolDictionary, world );
  fill_spatial_relation_axis( symbolDictionary, world );
  fill_spatial_relations( symbolDictionary, world );
  fill_distances( symbolDictionary, world );
  fill_distance_units( symbolDictionary, world );
  fill_regions( symbolDictionary, world );
  fill_relations( symbolDictionary, world );
  fill_actions( symbolDictionary, world );

  return true;
}

bool SymbolSpace::fill_objects( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // construct symbols for each of the world model objects
  for( auto& object : world->objects ){
    h2sl::Symbol symobject = h2sl::Symbol( "object", {{"uid",object.second.uid}} );
    insert( symobject );
  }
  return true;
}

bool SymbolSpace::fill_object_types( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // construct symbols for each object_type

  // look for object_type in the symbol dictionary, return false if it is not found
  auto it_object_type = symbolDictionary->dictionary.find( "object_type" );
  if( it_object_type == symbolDictionary->dictionary.end() ){
    std::cout << "could not find object_type in symbol dictionary" << std::endl;
    return false;
  }

  // look for object_type in the object_type entry in the symbol dictionary, return false if it is not found
  auto it_object_type_type = it_object_type->second->properties.find( "object_type" );
  if( it_object_type_type == it_object_type->second->properties.end() ){
    std::cout << "could not find object_type in symbol dictionary" << std::endl;
    return false;
  }

  // iterate over all object types
  for( auto& object_type_type_value : it_object_type_type->second ){
    h2sl::Symbol symobject_type = h2sl::Symbol( "object_type", {{"object_type",object_type_type_value}} );
    insert( symobject_type );
  }

  return true;
}

///
/// Method to construct symbols for each object_color
///
bool SymbolSpace::fill_object_colors( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // look for object_color in the symbol dictionary, return false if it is not found
  auto const cit_object_color = symbolDictionary->dictionary.find( "object_color" );
  if( cit_object_color == symbolDictionary->dictionary.end() ){
    std::cout << "could not find object_color in symbol dictionary" << std::endl;
    return false;
  }

  // look for object_color in the object_color entry in the symbol dictionary, return false if it is not found
  auto const cit_object_color_color = cit_object_color->second->properties.find( "object_color" );
  if( cit_object_color_color == cit_object_color->second->properties.end() ){
    std::cout << "could not find \"object_color\" property for the \"object_color\" symbol" << std::endl;
    return false;
  }

  // iterate over all object colors
  for( const auto& object_color_color_value : cit_object_color_color->second ){
    h2sl::Symbol symobject_color = h2sl::Symbol( "object_color", {{"object_color",object_color_color_value}} );
    insert( symobject_color );
  }
  return true;
}

// Method to construct symbols for each object_quantifier
bool SymbolSpace::fill_object_quantifiers( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // look for object_quantifier in the symbol dictionary, return false if it is not found
  auto const cit_object_quantifier = symbolDictionary->dictionary.find( "object_quantifier" );
  if( cit_object_quantifier == symbolDictionary->dictionary.end() ){
    std::cout << "could not find object_quantifier in symbol dictionary" << std::endl;
    return false;
  }

  // look for "quantifier" in the object_quantifier entry in the symbol dictionary, return false if it is not found
  auto const cit_quant = cit_object_quantifier->second->properties.find( "quantifier" );
  if( cit_quant == cit_object_quantifier->second->properties.end() ){
    std::cout << "could not find \"quantifier\" property for the \"object_quantifier\" symbol" << std::endl;
    return false;
  }

  // iterate over all object quantifiers
  for( const auto& quantifier : cit_quant->second ){
    h2sl::Symbol sym = h2sl::Symbol( "object_quantifier", {{"quantifier",quantifier}} );
    insert( sym );
  }

  return true;
}

// Construct symbols for each object_semantic_property_type
bool SymbolSpace::fill_object_semantic_property_types( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // Find object_semantic_property_type in the symbol dictionary, return false if it is not found
  auto const cit_object_semantic_property_type = symbolDictionary->dictionary.find("object_semantic_property_type");
  if( cit_object_semantic_property_type == symbolDictionary->dictionary.cend() ){
    std::cout << "could not find object_semantic_property_type in symbol dictionary" << std::endl;
    return false;
  }
  // Find the "property" property in the symbol dictionary for object_semantic_property_type
  auto const cit_property_type = cit_object_semantic_property_type->second->properties.find("property");
  if( cit_property_type == cit_object_semantic_property_type->second->properties.cend() ){
    std::cout << "could not find the \"property\" property for object_semantic_property_type in the symbol dictionary" << std::endl;
    return false;
  }
  // Create a symbol for each property type
  for( const auto& property_type : cit_property_type->second ){
    h2sl::Symbol symbol_object_semantic_property_type = h2sl::Symbol("object_semantic_property_type", {{"property", property_type}} );
    insert( symbol_object_semantic_property_type );
  }
  return true;
}

// Construct symbols for each spatial_relation_extrema
bool SymbolSpace::fill_spatial_relation_extrema( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // Find spatial_relation_extrema in the symbol dictionary, return false if it is not found
  auto const cit_spatial_relation_extrema = symbolDictionary->dictionary.find("spatial_relation_extrema");
  if( cit_spatial_relation_extrema == symbolDictionary->dictionary.cend() ){
    std::cout << "could not find spatial_relation_extrema in symbol dictionary" << std::endl;
    return false;
  }

  // Find the "extrema" property in the symbol dictionary for spatial_relation_extrema
  auto const cit_extrema = cit_spatial_relation_extrema->second->properties.find("extrema");
  if( cit_extrema == cit_spatial_relation_extrema->second->properties.cend() ){
    std::cout << "could not find the \"extrema\" property for spatial_relation_extrema in the symbol dictionary" << std::endl;
    return false;
  }

  // Create a symbol for each extrema type
  for( const auto& extrema : cit_extrema->second ){
    h2sl::Symbol symbol_extrema = h2sl::Symbol("spatial_relation_extrema", {{"extrema", extrema}} );
    insert( symbol_extrema );
  }

  return true;
}

// Construct symbols for each spatial_relation_axis
bool SymbolSpace::fill_spatial_relation_axis( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // Find spatial_relation_axis in the symbol dictionary, return false if it is not found
  auto const cit_spatial_relation_axis = symbolDictionary->dictionary.find("spatial_relation_axis");
  if( cit_spatial_relation_axis == symbolDictionary->dictionary.cend() ){
    std::cout << "could not find spatial_relation_axis in symbol dictionary" << std::endl;
    return false;
  }

  // Find the "axis" property in the symbol dictionary for spatial_relation_axis
  auto const cit_axis = cit_spatial_relation_axis->second->properties.find("axis");
  if( cit_axis == cit_spatial_relation_axis->second->properties.cend() ){
    std::cout << "could not find the \"axis\" property for spatial_relation_axis in the symbol dictionary" << std::endl;
    return false;
  }

  // Create a symbol for each axis type
  for( const auto& axis : cit_axis->second ){
    h2sl::Symbol symbol_axis = h2sl::Symbol("spatial_relation_axis", {{"axis", axis}} );
    insert( symbol_axis );
  }

  return true;
}

bool SymbolSpace::fill_spatial_relations( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // construct symbols for each spatial_relation

  // look for spatial_relation in the symbol dictionary, return false if it is not found
  auto it_spatial_relation = symbolDictionary->dictionary.find( "spatial_relation" );
  if( it_spatial_relation == symbolDictionary->dictionary.end() ){
    std::cout << "could not find spatial_relation in symbol dictionary" << std::endl;
    return false;
  }

  // look for spatial_relation_type in the spatial_relation entry in the symbol dictionary, return false if it is not found
  auto it_spatial_relation_type = it_spatial_relation->second->properties.find( "spatial_relation_type" );
  if( it_spatial_relation_type == it_spatial_relation->second->properties.end() ){
    std::cout << "could not find spatial_relation_type in symbol dictionary, trying a different representation" << std::endl;
  } else{
    // iterate over all object types
    for( auto& spatial_relation_type_value : it_spatial_relation_type->second ){
      h2sl::Symbol symspatial_relation = h2sl::Symbol( "spatial_relation", {{"spatial_relation_type",spatial_relation_type_value}} );
      insert( symspatial_relation );
    }
    // Return early and avoid looking for other spatial_relation symbol representations
    return true;
  }

  // look for axis in the spatial_relation entry in the symbol dictionary, return false if not found
  // This will be used for filtering which axes the model so far understands
  auto it_axis = it_spatial_relation->second->properties.find( "axis" );
  if( it_axis == it_spatial_relation->second->properties.end() ){
    std::cout << "could not find spatial_relation axis property in symbol dictionary" << std::endl;
    return false;
  }


  // Construct maximal set of axes included in our spatial relation representation
  std::vector<std::string> directional_axes = {"x+", "x-", "y+", "y-", "z+", "z-" };
  std::vector<std::string> proximity_axes = { "d+", "d-" };

  // Create symbols for intrinsic directional relations
  for( auto& object : world->objects ){
    for( std::string& d_axis : directional_axes ){
      // Skip axes not included in the symbol dictionary
      if( it_axis->second.find( d_axis ) == it_axis->second.end() ) continue;
      h2sl::Symbol sym = h2sl::Symbol( "spatial_relation",
        {{"landmark",object.second.uid},{"axis",d_axis}} );
      insert( sym );
    }
  }

  // Create symbols for intrinsic proximity relations
  for( auto& object : world->objects ){
    for( std::string& p_axis : proximity_axes ){
      // Skip axes not included in the symbol dictionary
      if( it_axis->second.find( p_axis ) == it_axis->second.end() ) continue;
      h2sl::Symbol sym = h2sl::Symbol( "spatial_relation",
        {{"landmark",object.second.uid},{"axis",p_axis}} );
      insert( sym );
    }
  }

  // All relative relations require a viewpoint; make sure the model has seen such relations
  // We assume that the symbol dictionary includes all possible viewpoints
  auto it_viewpoint = it_spatial_relation->second->properties.find( "viewpoint" );
  if( it_viewpoint == it_spatial_relation->second->properties.end() ){
    std::cout << "could not find spatial_relation viewpoint property in symbol dictionary" << std::endl;
    return true;
  }

  // Look for extrema property in the symbol dictionary
  auto it_extrema = it_spatial_relation->second->properties.find( "extrema" );

  // Create symbols for relative directional binary relations
  for( auto& viewpoint : it_viewpoint->second ){
    for( std::string& d_axis : directional_axes ){
      // Skip axes not included in the symbol dictionary
      if( it_axis->second.find( d_axis ) == it_axis->second.end() ) continue;
      h2sl::Symbol sym = h2sl::Symbol( "spatial_relation",
        {{"viewpoint",viewpoint},{"axis",d_axis}} );
      insert( sym );

      // Also include optional extrema on these relations
      // Skip all extrema if none are in the symbol dictionary
      if( it_extrema == it_spatial_relation->second->properties.end() ) continue;
      for( auto& extrema : it_extrema->second ){
        h2sl::Symbol extrema_sym = h2sl::Symbol( "spatial_relation",
          {{"viewpoint",viewpoint},{"axis",d_axis},{"extrema",extrema}} );
        insert( extrema_sym );
      }
    }
  }

  // Create symbols for relative directional ternary relations
  for( auto& object : world->objects ){
    for( auto& viewpoint : it_viewpoint->second ){
      for( std::string& d_axis : directional_axes ){
        // Skip axes not included in the symbol dictionary
        if( it_axis->second.find( d_axis ) == it_axis->second.end() ) continue;
        h2sl::Symbol sym = h2sl::Symbol( "spatial_relation",
          {{"viewpoint",viewpoint},{"landmark",object.second.uid},{"axis",d_axis}} );
        insert( sym );

        // Also include optional extrema on these relations
        // Skip all extrema if none are in the symbol dictionary
        if( it_extrema == it_spatial_relation->second->properties.end() ) continue;
        for( auto& extrema : it_extrema->second ){
          h2sl::Symbol extrema_sym = h2sl::Symbol( "spatial_relation", {{"viewpoint",viewpoint},
            {"landmark",object.second.uid},{"axis",d_axis},{"extrema",extrema}} );
          insert( extrema_sym );
        }
      }
    }
  }

  // TODO - Add absolute spatial relations if ever needed

  return true;
}

// Construct symbols for distances
bool SymbolSpace::fill_distances( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // Find distance in the symbol dictionary, return false if it is not found
  auto const cit_distance = symbolDictionary->dictionary.find("distance");
  if( cit_distance == symbolDictionary->dictionary.cend() ){
    std::cout << "could not find spatial_relation_axis in symbol dictionary" << std::endl;
    return false;
  }

  // Find the "unit" property in the symbol dictionary for distance
  auto const cit_unit = cit_distance->second->properties.find("unit");
  if( cit_unit == cit_distance->second->properties.cend() ){
    std::cout << "could not find the \"unit\" property for distance in the symbol dictionary" << std::endl;
    return false;
  }

  // Find the "quantity" property in the symbol dictionary for distance
  auto const cit_quantity = cit_distance->second->properties.find("quantity");
  if( cit_quantity == cit_distance->second->properties.cend() ){
    std::cout << "could not find the \"quantity\" property for distance in the symbol dictionary" << std::endl;
    return false;
  }

  // Create a symbol for each possible combination
  for( const auto& unit : cit_unit->second ){
    for( const auto& quantity : cit_quantity->second ){
      h2sl::Symbol symbol_distance = h2sl::Symbol( "distance", {{"unit",unit},{"quantity",quantity}} );
      insert( symbol_distance );
    }
  }
  return true;
}

// Construct symbols for distance units
bool SymbolSpace::fill_distance_units( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // Find distance_unit in the symbol dictionary, return false if it is not found
  auto const cit_distance_unit = symbolDictionary->dictionary.find("distance_unit");
  if( cit_distance_unit == symbolDictionary->dictionary.cend() ){
    std::cout << "could not find distance_unit in symbol dictionary" << std::endl;
    return false;
  }

  // Find the "unit" property in the symbol dictionary
  auto const cit_unit = cit_distance_unit->second->properties.find("unit");
  if( cit_unit == cit_distance_unit->second->properties.cend() ){
    std::cout << "could not find the \"unit\" property for distance_unit in the symbol dictionary" << std::endl;
    return false;
  }

  // Create a symbol for each distance_unit
  for( const auto& unit : cit_unit->second ){
    h2sl::Symbol symbol_distance_unit = h2sl::Symbol( "distance_unit", {{"unit",unit}} );
    insert( symbol_distance_unit );
  }
  return true;
}

bool SymbolSpace::fill_regions( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // construct symbols for each region

  // look for region in the symbol dictionary, return false if it is not found
  auto it_region = symbolDictionary->dictionary.find( "region" );
  if( it_region == symbolDictionary->dictionary.end() ){
    std::cout << "could not find region in symbol dictionary" << std::endl;
    return false;
  }

  // look for spatial_relation_type in the region entry in the symbol dictionary, return false if it is not found
  auto it_spatial_relation_type = it_region->second->properties.find( "spatial_relation_type" );
  if( it_spatial_relation_type == it_region->second->properties.end() ){
    std::cout << "could not find spatial_relation_type in symbol dictionary" << std::endl;
    return false;
  }

  // iterate over all object types
  for( auto& object: world->objects ){
    // iterate over all world model objects as single-sided relations
    for( auto& spatial_relation_type_value : it_spatial_relation_type->second ){
      h2sl::Symbol symregion = h2sl::Symbol( "region", {{"object",object.second.uid},{"spatial_relation_type",spatial_relation_type_value}} );
      insert( symregion );
    }
  }

  return true;
}

bool SymbolSpace::fill_relations( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // construct symbols for each relation

  // look for relation in the symbol dictionary, return false if it is not found
  auto it_relation = symbolDictionary->dictionary.find( "relation" );
  if( it_relation == symbolDictionary->dictionary.end() ){
    std::cout << "could not find relation in symbol dictionary" << std::endl;
    return false;
  }

  // look for relation_type in the relation entry in the symbol dictionary, return false if it is not found
  auto it_relation_type = it_relation->second->properties.find( "relation_type" );
  if( it_relation_type == it_relation->second->properties.end() ){
    std::cout << "could not find relation_type in symbol dictionary" << std::endl;
    return false;
  }

  // iterate over all world model objects as single-sided relations
  for( auto& object1 : world->objects ){
    // iterate over all relation types
    for( auto& relation_type_value : it_relation_type->second ){
      h2sl::Symbol symrelation = h2sl::Symbol( "relation", {{"relation_type",relation_type_value},{"regions",object1.second.uid}} );
      insert( symrelation );
    }
    // iterate over all world model objects again for two-sided relations
    for( auto& object2: world->objects ){
      if( object1.second.uid != object2.second.uid ){
        // iterate over all relation types
        for( auto& relation_type_value : it_relation_type->second ){
          // sort regions using alphabetical order
          if( h2sl::std_string_alphabetical_sort( object1.second.uid, object2.second.uid ) ){
            h2sl::Symbol symrelation = h2sl::Symbol( "relation", {{"relation_type",relation_type_value},{"regions",object1.second.uid + "," + object2.second.uid}} );
            insert( symrelation );
          } else {
            h2sl::Symbol symrelation = h2sl::Symbol( "relation", {{"relation_type",relation_type_value},{"regions",object2.second.uid + "," + object1.second.uid}} );
            insert( symrelation );
          }
        }
      }
    }
  }

  return true;
}

bool SymbolSpace::fill_actions( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world ){
  // construct symbols for each of the actions

  // look for action in the symbol dictionary, return false if it is not found
  auto it_action = symbolDictionary->dictionary.find( "action" );
  if( it_action == symbolDictionary->dictionary.end() ){
    std::cout << "could not find action in symbol dictionary" << std::endl;
    return false;
  }

  // look for action_type in the action entry in the symbol dictionary, return false if it is not found
  auto it_action_type = it_action->second->properties.find( "action_type" );
  if( it_action_type == it_action->second->properties.end() ){
    std::cout << "could not find action_type in symbol dictionary" << std::endl;
    return false;
  }

  // look for payload in the action entry in the symbol dictionary
  auto it_action_payload = it_action->second->properties.find( "payload" );
  // look for goal in the action entry in the symbol dictionary
  auto it_action_goal = it_action->second->properties.find( "goal" );
  if( it_action_goal == it_action->second->properties.end() && it_action_payload == it_action->second->properties.end() ){
    std::cout << "No \"payload\" or \"goal\" properties found associated with the action symbol, not adding actions." << std::endl;
    return false;
  }


  // iterate over all world model objects as goals
  for( auto& object : world->objects ){
    if( it_action_goal != it_action->second->properties.end() ){
      // iterate over all action types
      for( auto& action_type_value : it_action_type->second ){
        h2sl::Symbol symaction = h2sl::Symbol( "action",
                                { {"action_type",action_type_value},
                                  {"goal",object.second.uid}} );
        insert( symaction );
      }
    } else if( it_action_payload != it_action->second->properties.end() ){
      // iterate over all action types
      for( auto& action_type_value : it_action_type->second ){
        h2sl::Symbol symaction = h2sl::Symbol( "action",
                                { {"action_type",action_type_value},
                                  {"agent", "robot"},
                                  {"payload",object.second.uid}} );
        insert( symaction );
      }
    }
  }

  return true;
}

//
// Method to insert a new Symbol into the symbolsMap
//
std::shared_ptr<Symbol> SymbolSpace::insert( const Symbol& symbol ){
  // Check whether symbolsMap has Symbols of this type
  // If no, insert new symbolsVectorType into symbolsMap and add symbol
  // If yes, check if symbol already exists and add it if not
  symbolsMapVectorType::iterator it_symbolsMap = symbolsMap.find( symbol.type );
  if( it_symbolsMap == symbolsMap.end() ){
    // Insert new symbolsVectorType into symbolsMap
    symbolsMap.emplace( symbol.type, symbolsVectorType() );

    // Create a shared_ptr from the symbol
    std::shared_ptr<Symbol> symbol_ptr = std::make_shared<Symbol>( symbol );

    // Move ownership of the new share_ptr to the symbolsMap
    symbolsMap[ symbol.type ].push_back(std::move( symbol_ptr ));

    // Return the shared_ptr by value to correctly increment its reference count
    return symbolsMap[ symbol.type ].back();
  } else{
    // Check if symbol already exists
    // If yes, return it; if no, add and return it
    for( const auto& symbol_ptr : it_symbolsMap->second ){
      if( symbol == *symbol_ptr )
        return symbol_ptr;
    }
    std::shared_ptr<Symbol> symbol_ptr = std::make_shared<Symbol>( symbol );
    it_symbolsMap->second.push_back(std::move( symbol_ptr ));
    return it_symbolsMap->second.back();
  }
}

//
// Method to find a provided Symbol in symbolsMap
//
std::shared_ptr<Symbol> SymbolSpace::find_symbol( const Symbol& symbol ){
  // Check whether symbolsMap has Symbols of this type
  // If no, return a nullptr value
  // If yes, check for this symbol; return its shared_ptr if found, nullptr otherwise
  symbolsMapVectorType::iterator it_symbolsMap = symbolsMap.find( symbol.type );
  if( it_symbolsMap == symbolsMap.end() )
    return nullptr;

  for( const auto& symbol_ptr : it_symbolsMap->second ){
    if( symbol == *symbol_ptr )
      return symbol_ptr;
  }
  return nullptr;
}

//
// Method to import a SymbolSpace from an XML file
//
bool SymbolSpace::from_xml( const char* filename ){
  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename );

  // Check for loading errors
  if( doc.Error() )
    return false;

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr )
    return false;

  // Check for any SymbolSpace child element of the root
  const tinyxml2::XMLElement* symSpace_elem = root->FirstChildElement("symbolspace");
  if( symSpace_elem == nullptr )
    return false;

  return from_xml( symSpace_elem );
}

//
// Method to import a SymbolSpace from an XMLElement*
//
bool SymbolSpace::from_xml( const tinyxml2::XMLElement* symSpace_elem ){
  symbolsMap.clear();

  // Check that the element is a symbolspace element
  if( symSpace_elem == nullptr )
    return false;
  if( std::strcmp(symSpace_elem->Name(), "symbolspace") != 0 )
    return false;

  // Import any child Symbols of this symSpace_elem into symbolsMap via insert()
  const tinyxml2::XMLElement* child_symbol_elem =
                                  symSpace_elem->FirstChildElement("symbol");
  while( child_symbol_elem != nullptr ){
    Symbol child_symbol = Symbol();

    // Check that the child Symbol is parsed correctly
    if( !(child_symbol.from_xml( child_symbol_elem )) )
      return false;

    // Check the child Symbol's type against the existing symbolsMap keys
    insert( child_symbol );
    child_symbol_elem = child_symbol_elem->NextSiblingElement("symbol");
  }
  return true;
}

//
// Method to export a SymbolSpace to an XML file
//
bool SymbolSpace::to_xml( const char* filename ) const{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertEndChild( root );
  to_xml( doc, root );

  // Save the file and store the error status
  tinyxml2::XMLError error_result = doc.SaveFile( filename );
  if( error_result != tinyxml2::XML_SUCCESS )
    return false;
  return true;
}

//
// Method to export a SymbolSpace to an XMLDocument object
//
void SymbolSpace::to_xml( tinyxml2::XMLDocument& doc,
                          tinyxml2::XMLElement* root) const
{
  // Create the symbolspace element to contain all of the symbols in symbolsMap
  tinyxml2::XMLElement* symSpace_elem = doc.NewElement("symbolspace");
  root->InsertEndChild( symSpace_elem );

  // Export each symbol in symbolsMap
  for( auto const& symbolsType : symbolsMap ){
    for( auto const& symbol : symbolsType.second ){
      symbol->to_xml( doc, symSpace_elem );
    }
  }
  return;
}

//
// SymbolSpace class ostream operator
//
std::ostream& operator<<( std::ostream& out, const SymbolSpace& other ){
  out << "SymbolSpace(";
  for( auto it_map = other.symbolsMap.cbegin();
            it_map != other.symbolsMap.cend(); ++it_map )
  {
    out << it_map->first << "s[" << it_map->second.size() << "]:(";
    for( auto it_vec = it_map->second.cbegin();
              it_vec != it_map->second.cend(); ++it_vec )
    {
      out << **it_vec;
      if( std::next(it_vec) != it_map->second.cend() )
        out << ", ";
    }
    if( std::next(it_map) != other.symbolsMap.cend() )
      out << ", ";
  }
  out << ")";
  return out;
}

} // namespace h2sl
