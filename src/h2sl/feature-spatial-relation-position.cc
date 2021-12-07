/**
 * @file feature-spatial-relation-position.cc
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

#include <fstream>
#include <sstream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <boost/algorithm/string.hpp>

#include "h2sl/common.h"
#include "h2sl/feature-spatial-relation-position.h"

namespace h2sl {

///
/// FeatureSpatialRelationPosition default class constructor
///
FeatureSpatialRelationPosition::FeatureSpatialRelationPosition( const unsigned int positionIndexArg ) :
  Feature( FeatureType::DynamicSymbol,
           FeatureValue::Unknown,
           Feature::dependent_parameters_t( false, false, true, true, true ) ),
  position_index( positionIndexArg ) {

}

///
/// FeatureSpatialRelationPosition class constructor from an XMLElement
///
FeatureSpatialRelationPosition::FeatureSpatialRelationPosition( const tinyxml2::XMLElement* root ) : Feature(), position_index() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureSpatialRelationPosition::dup( void )const{
  return std::make_shared<FeatureSpatialRelationPosition>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureSpatialRelationPosition::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::DynamicSymbol;
  value = FeatureValue::Unknown;
  position_index = 0;

  // Check that the element is a valid root element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSpatialRelationPosition Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-spatial-relation-position
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSpatialRelationPosition Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-spatial-relation-position") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSpatialRelationPosition Class Error] Erroneous class name (\"" << class_attr->Value() << "\") in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Read the position_index attribute
  const tinyxml2::XMLAttribute* position_attr = root->FindAttribute("position_index");
  if( position_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSpatialRelationPosition Class Error] Missing \"position_index\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  position_index = unsigned_int_from_std_string( position_attr->Value() );

  return true;
}

FeatureValue FeatureSpatialRelationPosition::evaluate( const std::shared_ptr< std::string >& cv,
                                        const std::shared_ptr<LanguageVariable>& lv,
                                        const std::shared_ptr<WorldDCG>& world,
                                        const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;

  // Check that our symbol is an object
  if( symbol->type != "object" ) return value;

  // Extract object from the world via the UID property
  auto it_object_uid = symbol->properties.find( "uid" );
  if( it_object_uid == symbol->properties.end() ){
    std::stringstream error_msg;
    error_msg << "[FeatureSpatialRelationPosition Class Error] object symbol had no \"uid\" property";
    throw std::runtime_error( error_msg.str() );
  }

  // Check that the object is in the world and has a state with pose
  auto it_object = world->objects.find( it_object_uid->second );
  if( it_object == world->objects.end() ) return value;
  if( it_object->second.state_history.back().pose == nullptr ) return value;

  auto it_object_type = it_object->second.properties.find( "object_type" );
  if( it_object_type == it_object->second.properties.end() ) return value;

  // Check for an object_color child symbol to filter the sorted world
  std::optional< std::string > color_filter = std::nullopt;
  for( const auto& connection : lv->children() ){
    if( connection.label != "color" ) continue; // Only consider :color child LVs
    for( const auto& child_symbol : connection.child->symbols() ){
      if( child_symbol->type != "object_color" ) continue;
      auto it_color = child_symbol->properties.find( "object_color" );
      if( it_color != child_symbol->properties.end() ){
        color_filter = std::make_optional< std::string >( it_color->second );
        break;
      }
    }
    if( color_filter ) break;
  }

  // Check child LanguageVariables for ordered spatial_relation symbols
  for( const auto& connection : lv->children() ){
    for( const auto& child_symbol : connection.child->symbols() ){
      if( child_symbol->type != "spatial_relation" ) continue;

      // Assume "max" if the spatial relation doesn't have an extrema
      std::string extrema = "max";
      auto it_extrema = child_symbol->properties.find( "extrema" );
      if( it_extrema != child_symbol->properties.end() ){
        extrema = it_extrema->second;
      }

      // Check that the spatial relation has an axis
      // This is the one attribute always required in a spatial relation!
      auto it_axis = child_symbol->properties.find( "axis" );
      if( it_axis == child_symbol->properties.end() ){
        std::stringstream error_msg;
        error_msg << "[FeatureSpatialRelationPosition Class Error] spatial_relation symbol had no \"axis\" property";
        throw std::runtime_error( error_msg.str() );
      } else if( it_axis->second.length() != 2 ){
        std::stringstream error_msg;
        error_msg << "[FeatureSpatialRelationPosition Class Error] spatial_relation symbol had invalid \"axis\" property value: "<< it_axis->second;
        throw std::runtime_error( error_msg.str() );
      }

      // Convert axis into 2 characters
      char axis_dimension = it_axis->second[0];
      char axis_direction = it_axis->second[1];

      // Check if the spatial relation has a landmark
      std::optional<std::string> landmark_uid = std::nullopt;
      auto it_landmark_uid = child_symbol->properties.find( "landmark" );
      if( it_landmark_uid != child_symbol->properties.end() ){
        landmark_uid = std::make_optional<std::string>( it_landmark_uid->second );
      }

      // Check if the spatial relation has a viewpoint
      std::optional<std::string> viewpoint_uid = std::nullopt;
      auto it_viewpoint_uid = child_symbol->properties.find( "viewpoint" );
      if( it_viewpoint_uid != child_symbol->properties.end() ){
        viewpoint_uid = std::make_optional<std::string>( it_viewpoint_uid->second );
      }

      // Find object states for the landmark and viewpoint
      std::optional<ObjectState> landmark = std::nullopt;
      if( landmark_uid ){
        // Check that the landmark exists in the world
        auto it_landmark = world->objects.find( *landmark_uid );
        if( it_landmark == world->objects.end() ){
          continue; // Check for more spatial_relation child symbols
        }
        // Check that the landmark has a pose
        if( it_landmark->second.state_history.back().pose == nullptr ){
          continue; // Check for more spatial_relation child symbols
        }
        landmark = std::make_optional<ObjectState>( it_landmark->second.state_history.back() );
      }

      std::optional<ObjectState> viewpoint = std::nullopt;
      if( viewpoint_uid ){
        // Check that the viewpoint exists in the world
        auto it_viewpoint = world->objects.find( *viewpoint_uid );
        if( it_viewpoint == world->objects.end() ){
          continue; // Check for more spatial_relation child symbols
        }
        // Check that the viewpoint has a pose
        if( it_viewpoint->second.state_history.back().pose == nullptr ){
          continue; // Check for more spatial_relation child symbols
        }
        viewpoint = std::make_optional<ObjectState>( it_viewpoint->second.state_history.back() );
      }

      Transform3D relation_frame = h2sl::SpatialRelation::get_relation_frame( landmark, viewpoint );

      // Now check if our object is the Xth object when the world is sorted w.r.t. the spatial relation
      // auto explained: std::vector< std::pair< std::string, double > >
      auto sorted_uids = h2sl::SpatialRelation::get_sorted_uids( world, relation_frame,
        axis_dimension, axis_direction, extrema, color_filter, it_object_type->second );

      if( position_index >= sorted_uids.size() ){
      	return value; // Index out-of-bounds => Will be for all spatial relations
      }

      // Check if the uid matches at the feature's index in the vector
      std::string indexed_uid = sorted_uids.at( position_index ).first;
      if( it_object_uid->second == indexed_uid ){
        value = FeatureValue::True;
	      return value;
      }
    }
  }

  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureSpatialRelationPosition::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-spatial-relation-position(type=\"" << type << "\",value=\"" << value << "\",position_index=\"" << position_index << "\")";
  } else {
    out << "feature-spatial-relation-position(type=\"" << type << "\",position_index=\"" << position_index << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureSpatialRelationPosition::key( void ) const{
  std::stringstream out;
  out << "feature-spatial-relation-position(type=\"" << type << "\",position_index=\"" << position_index << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureSpatialRelationPosition::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureSpatialRelationPosition::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureSpatialRelationPosition::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_elem = document.NewElement( "feature" );
  feature_elem->SetAttribute( "class", "feature-spatial-relation-position" );
  feature_elem->SetAttribute( "position_index", to_std_string( position_index ).c_str() );
  root->InsertEndChild( feature_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureSpatialRelationPosition::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureSpatialRelationPosition::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureSpatialRelationPosition class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureSpatialRelationPosition& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
