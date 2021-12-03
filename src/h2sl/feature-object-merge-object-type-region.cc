/**
 * @file feature-object-merge-object-type-region.cc
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

#include "h2sl/feature-object-merge-object-type-region.h"

namespace h2sl {

///
/// FeatureObjectMergeObjectTypeRegion default class constructor
///
FeatureObjectMergeObjectTypeRegion::FeatureObjectMergeObjectTypeRegion( const std::string& spatialRelationTypeArg )
    : Feature( FeatureType::DynamicSymbol, FeatureValue::Unknown, Feature::dependent_parameters_t( false, false, true, true, true ) ),
      spatial_relation_type( spatialRelationTypeArg ) {}

///
/// FeatureObjectMergeObjectTypeRegion class constructor from an XMLElement
///
FeatureObjectMergeObjectTypeRegion::FeatureObjectMergeObjectTypeRegion( const tinyxml2::XMLElement* root ) : Feature(), spatial_relation_type() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureObjectMergeObjectTypeRegion::dup( void )const{
  return std::make_shared<FeatureObjectMergeObjectTypeRegion>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureObjectMergeObjectTypeRegion::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::DynamicSymbol;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, false, true, true, true );
  spatial_relation_type = "";

  // Check that the element is a feature-object-merge-object-type-region element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectMergeObjectTypeRegion Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-object-merge-object-type-region
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectMergeObjectTypeRegion Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-object-merge-object-type-region") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectMergeObjectTypeRegion Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
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
      error_msg << "[FeatureObjectMergeObjectTypeRegion Class Error] Missing \"spatial_relation_type\" attribute in from_xml";
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
FeatureObjectMergeObjectTypeRegion::evaluate( const std::shared_ptr< std::string >& cv,
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

  // Extract the child symbols; Enforce that there is an object_type and a region
  // Further enforce that the symbols come from different language variables via break statements
  std::shared_ptr<h2sl::Symbol> child_object_type_symbol = nullptr;
  std::shared_ptr<h2sl::Symbol> child_region_symbol = nullptr;
  for( const auto& connection : lv->children() ){
    for( const auto& child_symbol : connection.child->symbols() ){
      if( child_symbol->type == "object_type" ){
        child_object_type_symbol = child_symbol;
        break;
      }
      if( child_symbol->type == "region" ){
        child_region_symbol = child_symbol;
        break;
      }
    }
  }
  if( (child_object_type_symbol == nullptr) || (child_region_symbol == nullptr) )
    return value;

  // Enforce that the feature's spatial_relation_type matches the
  //  child_region_symbol's spatial_relation_type property
  auto it_child_region_spatial_relation_type = child_region_symbol->properties.find("spatial_relation_type");
  if( spatial_relation_type != it_child_region_spatial_relation_type->second )
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

  // Extract the child_region_symbol's object from the world via the uid
  auto it_child_region_object_uid = child_region_symbol->properties.find("object");
  if( it_child_region_object_uid == child_region_symbol->properties.end() )
    return value;
  auto it_child_region_object = world->objects.find( it_child_region_object_uid->second );
  if( it_child_region_object == world->objects.end() )
    return value;

  // Compare the object and child_region_object pose's according to the spatial_relation_type
  if( spatial_relation_type == "left" ){
    if( it_object->second.state_history.back().pose->y() > it_child_region_object->second.state_history.back().pose->y() )
      value = FeatureValue::True;
  } else if( spatial_relation_type == "right" ){
    if( it_object->second.state_history.back().pose->y() < it_child_region_object->second.state_history.back().pose->y() )
      value = FeatureValue::True;
  }
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureObjectMergeObjectTypeRegion::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-object-merge-object-type-region(type=\"" << type << "\",value=\"" << value << "\",spatial_relation_type=\"" << spatial_relation_type << "\")";
  } else {
    out << "feature-object-merge-object-type-region(type=\"" << type << "\",spatial_relation_type=\"" << spatial_relation_type << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureObjectMergeObjectTypeRegion::key( void ) const{
  std::stringstream out;
  out << "feature-object-merge-object-type-region(type=\"" << type << "\",spatial_relation_type=\"" << spatial_relation_type << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureObjectMergeObjectTypeRegion::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureObjectMergeObjectTypeRegion::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureObjectMergeObjectTypeRegion::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_object_merge_object_type_region_elem = document.NewElement( "feature" );
  feature_object_merge_object_type_region_elem->SetAttribute( "class", "feature-object-merge-object-type-region" );
  feature_object_merge_object_type_region_elem->SetAttribute( "spatial_relation_type", spatial_relation_type.c_str() );
  root->InsertEndChild( feature_object_merge_object_type_region_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureObjectMergeObjectTypeRegion::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureObjectMergeObjectTypeRegion::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureObjectMergeObjectTypeRegion class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureObjectMergeObjectTypeRegion& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
