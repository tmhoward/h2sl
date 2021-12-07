/**
 * @file feature-symbol-has-child.cc
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

#include "h2sl/feature-symbol-has-child.h"

namespace h2sl {

///
/// FeatureSymbolHasChild default class constructor
///
FeatureSymbolHasChild::FeatureSymbolHasChild( const std::string& symbolTypeArg, const bool invertArg ) :
    Feature( FeatureType::DynamicSymbol, FeatureValue::Unknown, Feature::dependent_parameters_t( false, false, true, true, false ) ),
    symbol_type( symbolTypeArg ), invert( invertArg ) {}

///
/// FeatureSymbolHasChild class constructor from an XMLElement
///
FeatureSymbolHasChild::FeatureSymbolHasChild( const tinyxml2::XMLElement* root ) : Feature(), symbol_type(), invert() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureSymbolHasChild::dup( void )const{
  return std::make_shared<FeatureSymbolHasChild>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureSymbolHasChild::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::DynamicSymbol;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, false, true, true, false );
  symbol_type = "";
  invert = false;

  // Check that the element is a feature-symbol-has-child element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolHasChild Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-symbol-has-child
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolHasChild Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-symbol-has-child") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolHasChild Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Read the symbol_type attribute and set it to the symbol_type
  const tinyxml2::XMLAttribute* symbol_type_attr = root->FindAttribute("symbol_type");
  if( symbol_type_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolHasChild Class Error] Missing \"symbol_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  symbol_type = symbol_type_attr->Value();

  // Read the invert attribute and set it to invert
  const auto* invert_attr = root->FindAttribute("invert");
  if( invert_attr == nullptr ){
    throw std::runtime_error("[FeatureSymbolAttributeValue Class Error] Missing \"invert\" attribute in from_xml()");
  }
  invert = invert_attr->BoolValue();

  return true;
}

FeatureValue FeatureSymbolHasChild::evaluate( const std::shared_ptr< std::string >& cv,
                                                  const std::shared_ptr<LanguageVariable>& lv,
                                                  const std::shared_ptr<WorldDCG>& world,
                                                  const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;
  if( symbol->type == symbol_type ){
    if( !lv->children().empty() ){
      value = (invert) ? FeatureValue::False : FeatureValue::True;
    } else{
      value = (invert) ? FeatureValue::True : FeatureValue::False;
    }
  }
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureSymbolHasChild::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-symbol-has-child(type=\"" << type << "\",value=\"" << value << "\",symbol_type=\"" << symbol_type << "\",invert= " << invert << "\")";
  } else {
    out << "feature-symbol-has-child(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\",invert= " << invert << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureSymbolHasChild::key( void ) const{
  std::stringstream out;
  out << "feature-symbol-has-child(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\",invert= " << invert << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureSymbolHasChild::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureSymbolHasChild::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureSymbolHasChild::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_symbol_has_child_elem = document.NewElement( "feature" );
  feature_symbol_has_child_elem->SetAttribute( "class", "feature-symbol-has-child" );
  feature_symbol_has_child_elem->SetAttribute( "symbol_type", symbol_type.c_str() );
  feature_symbol_has_child_elem->SetAttribute( "invert", invert );
  root->InsertEndChild( feature_symbol_has_child_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureSymbolHasChild::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureSymbolHasChild::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureSymbolHasChild class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureSymbolHasChild& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
