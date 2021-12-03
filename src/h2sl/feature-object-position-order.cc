/**
 * @file feature-object-position-order.cc
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

#include "h2sl/feature-object-position-order.h"

namespace h2sl {

///
/// FeatureObjectPositionOrder argument constructor
///
FeatureObjectPositionOrder::
FeatureObjectPositionOrder( const World::SortKey& sortKeyArg, const int offsetArg ) :
  Feature( FeatureType::DynamicSymbol, FeatureValue::Unknown, Feature::dependent_parameters_t( false, false, true, false, true ) ),
  sort_key( sortKeyArg ), offset( offsetArg ) {}

///
/// FeatureObjectPositionOrder class constructor from an XMLElement
///
FeatureObjectPositionOrder::
FeatureObjectPositionOrder( const tinyxml2::XMLElement* root ) :
  Feature(),
  sort_key(),
  offset()
{
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureObjectPositionOrder::dup( void )const{
  return std::make_shared<FeatureObjectPositionOrder>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureObjectPositionOrder::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::DynamicSymbol;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, false, true, false, true );
  sort_key = World::SortKey::UNKNOWN;
  offset = 0;

  // Check that the element is a feature-object-position-order element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectPositionOrder Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-object-position-order
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectPositionOrder Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-object-position-order") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectPositionOrder Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Read the sort_key attribute and set it to the sort_key
  const tinyxml2::XMLAttribute* sort_key_attr = root->FindAttribute("sort_key");
  if( sort_key_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectPositionOrder Class Error] Missing \"sort_key\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  sort_key = World::string_to_sortkey( sort_key_attr->Value() );

  // Read the offset attribute and set it to the offset
  const tinyxml2::XMLAttribute* offset_attr = root->FindAttribute("offset");
  if( offset_attr == nullptr ){
    std::stringstream error_msg;
    error_msg << "[FeatureObjectPositionOrder Class Error] Missing \"offset\" attribute in from_xml";
    throw std::runtime_error( error_msg.str() );
  }
  offset = offset_attr->IntValue();
  return true;
}

///
/// This method evaluates the feature
///
FeatureValue
FeatureObjectPositionOrder::evaluate( const std::shared_ptr< std::string >& cv,
                                      const std::shared_ptr<LanguageVariable>& lv,
                                      const std::shared_ptr<WorldDCG>& world,
                                      const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;

  // Enforce that the passed symbol is an object; early return false otherwise
  if( symbol->type != "object" ){
    return value;
  }
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

  // Copy the object to an object for readability
  auto object_symbol = it_object->second;

  // Get the sorted world objects according to the sort_key
  auto const it_map_sorted_objects_sort_key = world->sorted_objects().find( sort_key );
  if( it_map_sorted_objects_sort_key == world->sorted_objects().cend() ){
    std::stringstream error_msg;
    error_msg << "Did not find a sorted version of the world for \"" << World::sortkey_to_string( sort_key ) << "\"";
    throw std::runtime_error( error_msg.str() );
  }

  // Get object_type
  const auto& object_type = object_symbol.properties["object_type"];

  // Get the sorted world objects according to the object_tye
  auto const it_map_sorted_objects_object_type = it_map_sorted_objects_sort_key->second.find( object_type );
  if( it_map_sorted_objects_object_type == it_map_sorted_objects_sort_key->second.cend() ){
    std::stringstream error_msg;
    error_msg << "Did not find a sorted version of the world for object_type \"" << object_type << "\"";
    throw std::runtime_error( error_msg.str() );
  }

  // Enforce that the offset is not greater than the ( size of the sorted objects list - 1 )
  if( offset > ( it_map_sorted_objects_object_type->second.size() - 1 ) )
    return value;

  // Check if the object matches the element in the sorted objects at the feature's specified offset
  auto cit_indexed_object = it_map_sorted_objects_object_type->second.cbegin() + offset;
  if( object_symbol.uid == (*cit_indexed_object)->uid )
    value = FeatureValue::True;
  return value;
}

///
/// This method generate an EnvironmentConstraint for a given object symbol and world
///
EnvironmentConstraintMap
FeatureObjectPositionOrder::
generate_environment_constraints( const std::shared_ptr<Symbol>& object_symbol,
                                 const std::shared_ptr<World>& world)const
{

  // stringstream for exception handling messages
  std::stringstream error_msg;
  error_msg << "[FeatureObjectPositionOrder Class Error] ";

  // Initialize the return object
  auto e_constraints = EnvironmentConstraintMap();

  // Enforce that the passed symbol is an object
  if( object_symbol->type != "object" ){
    error_msg << "The provided symbol (" << *object_symbol << ") for generating an "
              << "EnvironmentConstraint was not an object.";
    throw std::runtime_error( error_msg.str() );
  }

  // Enforce that the object is present in the provided world
  // Extract the object from the world via the UID property
  auto it_object_uid = object_symbol->properties.find( "uid" );
  if( it_object_uid == object_symbol->properties.end() ){
    error_msg << "The provided object (" << *object_symbol << ") for generating an "
              << "EnvironmentConstraint did not have a \"uid\" property.";
    throw std::runtime_error( error_msg.str() );
  }
  if( world->objects.find( it_object_uid->second ) == world->objects.end() ){
    error_msg << "The provided object with uid \"" << it_object_uid->second
              << "\" for generating an EnvironmentConstraint was not present "
              << "in the provided world:\n" << *world;
    throw std::runtime_error( error_msg.str() );
  }

  // Initialize the constraint operator per the sort_key
  EnvironmentConstraint::ConstraintOperator constraint_operator;
  if( sort_key == World::SortKey::MIN_X_AXIS ||
      sort_key == World::SortKey::MIN_ABS_X_AXIS ||
      sort_key == World::SortKey::MIN_Y_AXIS ||
      sort_key == World::SortKey::MIN_ABS_Y_AXIS ||
      sort_key == World::SortKey::MIN_Z_AXIS ||
      sort_key == World::SortKey::MIN_ABS_Z_AXIS ||
      sort_key == World::SortKey::MIN_EUCLIDEAN_DISTANCE ||
      sort_key == World::SortKey::MIN_CENTER_DISTANCE )
  {
    constraint_operator = EnvironmentConstraint::ConstraintOperator::LESS_THAN;
  }

  if( sort_key == World::SortKey::MAX_X_AXIS ||
      sort_key == World::SortKey::MAX_ABS_X_AXIS ||
      sort_key == World::SortKey::MAX_Y_AXIS ||
      sort_key == World::SortKey::MAX_ABS_Y_AXIS ||
      sort_key == World::SortKey::MAX_Z_AXIS ||
      sort_key == World::SortKey::MAX_ABS_Z_AXIS ||
      sort_key == World::SortKey::MAX_EUCLIDEAN_DISTANCE ||
      sort_key == World::SortKey::MAX_CENTER_DISTANCE )
  {
    constraint_operator = EnvironmentConstraint::ConstraintOperator::GREATER_THAN;
  }

  // Initialize the object property per the sort_key
  EnvironmentConstraint::ObjectProperty object_property;
  // Axis properties
  if(sort_key == World::SortKey::MIN_X_AXIS || sort_key == World::SortKey::MAX_X_AXIS)
    object_property = EnvironmentConstraint::ObjectProperty::POSITION_X;
  if(sort_key == World::SortKey::MIN_Y_AXIS || sort_key == World::SortKey::MAX_Y_AXIS)
    object_property = EnvironmentConstraint::ObjectProperty::POSITION_Y;
  if(sort_key == World::SortKey::MIN_Z_AXIS || sort_key == World::SortKey::MAX_Z_AXIS)
    object_property = EnvironmentConstraint::ObjectProperty::POSITION_Z;
  // Absolute value axis properties
  if( sort_key == World::SortKey::MIN_ABS_X_AXIS ||
      sort_key == World::SortKey::MAX_ABS_X_AXIS)
  {
    object_property = EnvironmentConstraint::ObjectProperty::ABS_POSITION_X;
  }
  if( sort_key == World::SortKey::MIN_ABS_Y_AXIS ||
      sort_key == World::SortKey::MAX_ABS_Y_AXIS)
  {
    object_property = EnvironmentConstraint::ObjectProperty::ABS_POSITION_Y;
  }
  if( sort_key == World::SortKey::MIN_ABS_Z_AXIS ||
      sort_key == World::SortKey::MAX_ABS_Z_AXIS)
  {
    object_property = EnvironmentConstraint::ObjectProperty::ABS_POSITION_Z;
  }
  // Euclidean distance property
  if( sort_key == World::SortKey::MIN_EUCLIDEAN_DISTANCE ||
      sort_key == World::SortKey::MAX_EUCLIDEAN_DISTANCE )
  {
    object_property = EnvironmentConstraint::ObjectProperty::EUCLIDEAN;
  }
  // Center distance property
  if( sort_key == World::SortKey::MIN_CENTER_DISTANCE ||
      sort_key == World::SortKey::MAX_CENTER_DISTANCE )
  {
    object_property = EnvironmentConstraint::ObjectProperty::CENTER;
  }

  // Add each comparison for the world objects
  for( const auto& kv_object : world->objects ){
    // Do not compare the object to itself
    if( it_object_uid->second == kv_object.first )
      continue;
    auto e_constraint = EnvironmentConstraint(
            EnvironmentConstraint::operand_t{ it_object_uid->second, object_property },
            EnvironmentConstraint::operand_t{ kv_object.first, object_property },
            constraint_operator );
    e_constraints.environment_constraints.emplace( e_constraint.to_string(), e_constraint );
  }
  return e_constraints;
}

///
/// This method prints out the values of a feature
///
std::string FeatureObjectPositionOrder::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-object-position-order(type=\"" << type << "\",value=\"" << value << "\",sort_key=\"" << World::sortkey_to_string( sort_key ) << "\",offset=\"" << offset << "\")";
  } else {
    out << "feature-object-position-order(type=\"" << type << "\",sort_key=\"" << World::sortkey_to_string( sort_key ) << "\",offset=\"" << offset << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureObjectPositionOrder::key( void ) const{
  std::stringstream out;
  out << "feature-object-position-order(type=\"" << type << "\",sort_key=\"" << World::sortkey_to_string( sort_key ) << "\",offset=\"" << offset << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureObjectPositionOrder::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureObjectPositionOrder::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureObjectPositionOrder::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_object_merge_object_type_region_elem = document.NewElement( "feature" );
  feature_object_merge_object_type_region_elem->SetAttribute( "class", "feature-object-position-order" );
  feature_object_merge_object_type_region_elem->SetAttribute( "sort_key", World::sortkey_to_string( sort_key ).c_str() );
  feature_object_merge_object_type_region_elem->SetAttribute( "offset", std::to_string( offset ).c_str() );
  root->InsertEndChild( feature_object_merge_object_type_region_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureObjectPositionOrder::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureObjectPositionOrder::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureObjectPositionOrder class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureObjectPositionOrder& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
