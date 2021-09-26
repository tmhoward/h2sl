/**
 * @file feature-lv-role-value.cc
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

#include "h2sl/feature-lv-role-value.h"

namespace h2sl {

///
/// FeatureLVRoleValue default class constructor
///
FeatureLVRoleValue::FeatureLVRoleValue( const std::string& lvTypeArg,
                                        const std::string& roleTypeArg,
	                                const std::string& roleValueArg ) :
    Feature( FeatureType::Language,
             FeatureValue::Unknown,
	     Feature::dependent_parameters_t( false, true, false, false, false ) ),
    lv_type( lvTypeArg ), role_type( roleTypeArg ), role_value( roleValueArg ){}

///
/// FeatureLVRoleValue class constructor from an XMLElement
///
FeatureLVRoleValue::FeatureLVRoleValue( const tinyxml2::XMLElement* root ) : Feature(), lv_type(), role_type(), role_value() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureLVRoleValue::dup( void )const{
  return std::make_shared<FeatureLVRoleValue>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureLVRoleValue::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::Language;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, true, false, false, false );
  lv_type = "";
  role_type = "";
  role_value = "";

  // Check that the element is a feature-lv-role-value element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureLVRoleValue Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
 
  // check to see if the class name is feature-lv-role-value
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureLVRoleValue Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-lv-role-value") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureLVRoleValue Class Error] Erroneous class name (\"" << class_attr->Value() << "\") in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
 
  // Read the lv_type attribute and set it to the lv_type
  const tinyxml2::XMLAttribute* lv_type_attr = root->FindAttribute("lv_type");
  if( lv_type_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureLVRoleValue Class Error] Missing \"lv_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  lv_type = lv_type_attr->Value();

  // Read the role_type attribute and set it to the role_type
  const tinyxml2::XMLAttribute* role_type_attr = root->FindAttribute("role_type");
  if( role_type_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureLVRoleValue Class Error] Missing \"role_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  role_type = role_type_attr->Value();
  
  // Read the role_value attribute and set it to the role_value
  const tinyxml2::XMLAttribute* role_value_attr = root->FindAttribute("role_value");
  if( role_value_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureLVRoleValue Class Error] Missing \"role_value\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  role_value = role_value_attr->Value();

  return true;
}

FeatureValue FeatureLVRoleValue::evaluate( const std::shared_ptr<std::string>& cv,
                                          const std::shared_ptr<LanguageVariable>& lv,
                                          const std::shared_ptr<WorldDCG>& world,
                                          const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;
  if( lv_type != lv->type ) return value;
  if( !lv->roles ) return value;

  // Otherwise we have roles, check for the feature's
  auto it_roles = lv->roles->find( role_type );
  if( it_roles == lv->roles->end() ) return value;

  // The LV has the role, but does it match?
  if( role_value == it_roles->second ){
    value = FeatureValue::True;
  }

  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureLVRoleValue::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-lv-role-value(type=\"" << type << "\",value=\"" << value << "\",lv_type=\"" << lv_type << "\",role_type=\"" << role_type <<"\",role_value=\"" << role_value << "\")";
  } else {
    out << "feature-lv-role-value(type=\"" << type << "\",lv_type=\"" << lv_type << "\",role_type=\"" << role_type <<"\",role_value=\"" << role_value << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureLVRoleValue::key( void ) const{
  std::stringstream out;
  out << "feature-lv-role-value(type=\"" << type << "\",lv_type=\"" << lv_type << "\",role_type=\"" << role_type <<"\",role_value=\"" << role_value << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureLVRoleValue::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureLVRoleValue::from_json( const Json::Value& root ){
  // TODO
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureLVRoleValue::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_elem = document.NewElement( "feature" );
  feature_elem->SetAttribute( "class", "feature-lv-role-value" );
  feature_elem->SetAttribute( "lv_type", lv_type.c_str() );
  feature_elem->SetAttribute( "role_type", role_type.c_str() );
  feature_elem->SetAttribute( "role_value", role_value.c_str() );
  root->InsertEndChild( feature_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureLVRoleValue::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureLVRoleValue::to_json( Json::Value& root )const{
  // TODO
  return;
}

///
/// FeatureLVRoleValue class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureLVRoleValue& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
