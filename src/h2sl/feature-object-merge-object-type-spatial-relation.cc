/**
 * @file feature-object-merge-object-type-spatial-relation.cc
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

#include "h2sl/feature-object-merge-object-type-spatial-relation.h"

namespace h2sl {

///
/// FeatureObjectMergeObjectTypeSpatialRelation default class constructor
///
FeatureObjectMergeObjectTypeSpatialRelation::FeatureObjectMergeObjectTypeSpatialRelation( const std::string& spatialRelationTypeArg )
    : Feature( FeatureType::DynamicSymbol, FeatureValue::Unknown, Feature::dependent_parameters_t( false, false, true, true, true ) ),
      spatial_relation_type( spatialRelationTypeArg ) {}

///
/// FeatureObjectMergeObjectTypeSpatialRelation class constructor from an XMLElement
///
FeatureObjectMergeObjectTypeSpatialRelation::FeatureObjectMergeObjectTypeSpatialRelation( const tinyxml2::XMLElement* root ) : Feature(), spatial_relation_type() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureObjectMergeObjectTypeSpatialRelation::dup( void )const{
  return std::make_shared<FeatureObjectMergeObjectTypeSpatialRelation>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureObjectMergeObjectTypeSpatialRelation::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::DynamicSymbol;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, false, true, true, true );
  spatial_relation_type = "";

  // Check that the element is a feature-object-merge-object-type-spatial-relation element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectMergeObjectTypeSpatialRelation Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-object-merge-object-type-spatial-relation
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectMergeObjectTypeSpatialRelation Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-object-merge-object-type-spatial-relation") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectMergeObjectTypeSpatialRelation Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Read the spatial_relation_type attribute and set it to the spatial_relation_type
  const tinyxml2::XMLAttribute* spatial_relation_type_attr = root->FindAttribute("spatial_relation_type");
  if( spatial_relation_type_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectMergeObjectTypeSpatialRelation Class Error] Missing \"spatial_relation_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  spatial_relation_type = spatial_relation_type_attr->Value();

  return true;
}

FeatureValue
FeatureObjectMergeObjectTypeSpatialRelation::evaluate( const std::shared_ptr< std::string >& cv,
                                                        const std::shared_ptr<LanguageVariable>& lv,
                                                        const std::shared_ptr<WorldDCG>& world,
                                                        const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;

  // Enforce that the passed symbol is an object; early return false otherwise
  if( symbol->type != "object" )
    return value;

  // Extract the object from the world via the UID property
  auto it_object_uid = symbol->properties.find( "uid" );
  // No "uid" property found, return false; TODO consider throwing an exception here
  if( it_object_uid == symbol->properties.end() )
    return value;
  auto it_object = world->objects.find( it_object_uid->second );
  // Enforce that the object is in the world && that the object has a state with pose
  if( it_object == world->objects.end() )
    return value;
  if( it_object->second.state_history.back().pose == nullptr )
    return value;

  // Extract the child symbols; Enforce that there is an object_type and a spatial_relation
  // Further enforce that the symbols come from different language variables via break statements
  std::shared_ptr<h2sl::Symbol> child_object_type_symbol = nullptr;
  std::shared_ptr<h2sl::Symbol> child_spatial_relation_symbol = nullptr;
  for( const auto& connection : lv->children() ){
    for( const auto& child_symbol : connection.child->symbols() ){
      if( child_symbol->type == "object_type" ){
        child_object_type_symbol = child_symbol;
        break;
      }
      if( child_symbol->type == "spatial_relation" ){
        child_spatial_relation_symbol = child_symbol;
        break;
      }
    }
  }
  if( (child_object_type_symbol == nullptr) || (child_spatial_relation_symbol == nullptr) )
    return value;

  // Enforce that the feature's spatial_relation_type matches the
  //  child_spatial_relation_symbol's spatial_relation_type property
  auto it_child_spatial_relation_spatial_relation_type = child_spatial_relation_symbol->properties.find("spatial_relation_type");
  if( spatial_relation_type != it_child_spatial_relation_spatial_relation_type->second )
    return value;

  // Enforce that the object's type and the child_object_type_symbol match
  auto it_child_object_type_type = child_object_type_symbol->properties.find( "object_type" );
  if( it_child_object_type_type == child_object_type_symbol->properties.end() )
    return value;
  auto it_object_type = it_object->second.properties.find( "object_type" );
  if( it_object_type == it_object->second.properties.end() )
    return value;
  if( it_object_type->second != it_child_object_type_type->second )
    return value;

  // Check the object's position order according to the spatial_relation_type
  // Copy the object to an object for readability
  auto object_symbol = it_object->second;
  const std::string& object_type = object_symbol.properties["object_type"];

  // Evaluate the feature using the World::SortKey associated with the spatial_relation_type
  // Get the sorted world objects according to the spatial_relation_type
  // WorldDCG::mapSortKeyMapObjects::const_iterator it_sorted_objects;
  std::vector< std::shared_ptr<Object> > sorted_objects;
  if( spatial_relation_type == "left" )
    sorted_objects = world->sorted_objects().find( World::SortKey::MAX_Y_AXIS )->second.find(object_type)->second;
  else if( spatial_relation_type == "right" )
    sorted_objects = world->sorted_objects().find( World::SortKey::MIN_Y_AXIS )->second.find(object_type)->second;
  else if( spatial_relation_type == "middle" )
    sorted_objects = world->sorted_objects().find( World::SortKey::MIN_CENTER_DISTANCE )->second.find(object_type)->second;

  if( object_symbol.uid == sorted_objects.front()->uid )
    value = FeatureValue::True;
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureObjectMergeObjectTypeSpatialRelation::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-object-merge-object-type-spatial-relation(type=\"" << type << "\",value=\"" << value << "\",spatial_relation_type=\"" << spatial_relation_type << "\")";
  } else {
    out << "feature-object-merge-object-type-spatial-relation(type=\"" << type << "\",spatial_relation_type=\"" << spatial_relation_type << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureObjectMergeObjectTypeSpatialRelation::key( void ) const{
  std::stringstream out;
  out << "feature-object-merge-object-type-spatial-relation(type=\"" << type << "\",spatial_relation_type=\"" << spatial_relation_type << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureObjectMergeObjectTypeSpatialRelation::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureObjectMergeObjectTypeSpatialRelation::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureObjectMergeObjectTypeSpatialRelation::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_object_merge_object_type_spatial_relation_elem = document.NewElement( "feature" );
  feature_object_merge_object_type_spatial_relation_elem->SetAttribute( "class", "feature-object-merge-object-type-spatial-relation" );
  feature_object_merge_object_type_spatial_relation_elem->SetAttribute( "spatial_relation_type", spatial_relation_type.c_str() );
  root->InsertEndChild( feature_object_merge_object_type_spatial_relation_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureObjectMergeObjectTypeSpatialRelation::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureObjectMergeObjectTypeSpatialRelation::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureObjectMergeObjectTypeSpatialRelation class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureObjectMergeObjectTypeSpatialRelation& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
