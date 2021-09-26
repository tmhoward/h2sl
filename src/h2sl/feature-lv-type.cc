/**
 * @file feature-lv-type.cc
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
#include "h2sl/feature-lv-type.h"

namespace h2sl {

///
/// FeatureLVType default class constructor
///
FeatureLVType::FeatureLVType( const std::string& typeArg ) : Feature( FeatureType::Language, FeatureValue::Unknown, Feature::dependent_parameters_t( false, true, false, false, false )), lv_type( typeArg ) {

}

///
/// FeatureLVType class constructor from an XMLElement
///
FeatureLVType::FeatureLVType( const tinyxml2::XMLElement* root ) : Feature(), lv_type() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureLVType::dup( void )const{
  return std::make_shared<FeatureLVType>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureLVType::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::Language;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, true, false, false, false );
  lv_type = "";

  // Check that the element is a feature-lv-type element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureLVType Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Check to see if the class name is feature-lv-type
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureLVType Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-lv-type") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureLVType Class Error] Erroneous class name (\"" << class_attr->Value() << "\") in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Read the type attribute and set it to this Feature's type
  const tinyxml2::XMLAttribute* lv_type_attr = root->FindAttribute("lv_type");
  if( lv_type_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureLVType Class Error] Missing \"lv_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  lv_type = lv_type_attr->Value();

  return true;
}

FeatureValue FeatureLVType::evaluate( const std::shared_ptr< std::string >& cv,
                                        const std::shared_ptr<LanguageVariable>& lv,
                                        const std::shared_ptr<WorldDCG>& world,
                                        const std::shared_ptr<Symbol>& symbol )
{
  if( lv_type == lv->type ){
    value = FeatureValue::True;
  } else {
    value = FeatureValue::False;
  }
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureLVType::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-lv-type(type=\"" << type << "\",value=\"" << value << "\",lv_type=\"" << lv_type << "\")";
  } else {
    out << "feature-lv-type(type=\"" << type << "\",lv_type=\"" << lv_type << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureLVType::key( void ) const{
  std::stringstream out;
  out << "feature-lv-type(type=\"" << type << "\",lv_type=\"" << lv_type << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureLVType::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureLVType::from_json( const Json::Value& root ){
  // TODO - Features don't seem to have these
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureLVType::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_elem = document.NewElement( "feature" );
  feature_elem->SetAttribute( "class", "feature-lv-type" );
  feature_elem->SetAttribute( "lv_type", lv_type.c_str() );
  root->InsertEndChild( feature_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureLVType::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureLVType::to_json( Json::Value& root )const{
  // TODO
  return;
}

///
/// FeatureLVType class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureLVType& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
