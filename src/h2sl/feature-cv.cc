/**
 * @file feature-cv.cc
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

#include "h2sl/feature-cv.h"

namespace h2sl {

///
/// FeatureCV default class constructor
///
FeatureCV::FeatureCV( const std::string& cvArg ) : Feature( FeatureType::CorrespondenceVariable,
                                                            FeatureValue::Unknown,
                                                            Feature::dependent_parameters_t( true, false, false, false, false ) ),
                                                    cv( cvArg ){}

///
/// FeatureCV class constructor from an XMLElement
///
FeatureCV::FeatureCV( const tinyxml2::XMLElement* root ) : Feature(), cv() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureCV::dup( void )const{
  return std::make_shared<FeatureCV>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureCV::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::CorrespondenceVariable;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( true, false, false, false, false );
  cv = "";

  // Check that the element is a feature-cv element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureCV Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  
  // check to see if the class name is feature-cv
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureCV Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-cv") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureCV Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Read the type attribute and set it to this Symbol's type
  const tinyxml2::XMLAttribute* cv_attr = root->FindAttribute("cv");
  if( cv_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureCV Class Error] Missing \"cv\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  cv = cv_attr->Value();
  return true;
}

FeatureValue FeatureCV::evaluate( const std::shared_ptr< std::string >& cvArg,
                                  const std::shared_ptr<LanguageVariable>& lvArg,
                                  const std::shared_ptr<WorldDCG>& worldArg,
                                  const std::shared_ptr<Symbol>& symbolArg )
{
  if( *cvArg == cv ){
    value = FeatureValue::True;
  } else {
    value = FeatureValue::False;
  }
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureCV::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-cv(type=\"" << type << "\",value=\"" << value << "\",cv=\"" << cv << "\")";
  } else {
    out << "feature-cv(type=\"" << type << "\",cv=\"" << cv << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureCV::key( void ) const{
  std::stringstream out;
  out << "feature-cv(type=\"" << type << "\",cv=\"" << cv << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureCV::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureCV::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureCV::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_cv_elem = document.NewElement( "feature" );
  feature_cv_elem->SetAttribute( "class", "feature-cv" );
  feature_cv_elem->SetAttribute( "cv", cv.c_str() );
  root->InsertEndChild( feature_cv_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureCV::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureCV::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureCV class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureCV& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
