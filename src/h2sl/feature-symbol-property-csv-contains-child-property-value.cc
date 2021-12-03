/**
 * @file feature-symbol-property-csv-contains-child-property-value.cc
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

#include "h2sl/feature-symbol-property-csv-contains-child-property-value.h"
#include "h2sl/common.h"

namespace h2sl {

///
/// FeatureSymbolPropertyCSVContainsChildPropertyValue default class constructor
///
FeatureSymbolPropertyCSVContainsChildPropertyValue::
FeatureSymbolPropertyCSVContainsChildPropertyValue(
                                  const std::string& symbolTypeArg,
                                  const std::string& propertyTypeArg,
                                  const std::string& childSymbolTypeArg,
                                  const std::string& childPropertyTypeArg ) :
                                    Feature( FeatureType::DynamicSymbol, FeatureValue::Unknown,
                                              Feature::dependent_parameters_t( false, false, true, true, false ) ),
                                    symbol_type( symbolTypeArg ),
                                    property_type( propertyTypeArg ),
                                    child_symbol_type( childSymbolTypeArg ),
                                    child_property_type( childPropertyTypeArg ){}

///
/// FeatureSymbolPropertyCSVContainsChildPropertyValue class constructor from an XMLElement
///
FeatureSymbolPropertyCSVContainsChildPropertyValue::FeatureSymbolPropertyCSVContainsChildPropertyValue( const tinyxml2::XMLElement* root ) : Feature(), symbol_type(), property_type(), child_symbol_type(), child_property_type() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureSymbolPropertyCSVContainsChildPropertyValue::dup( void )const{
  return std::make_shared<FeatureSymbolPropertyCSVContainsChildPropertyValue>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureSymbolPropertyCSVContainsChildPropertyValue::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::DynamicSymbol;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, false, true, true, false );
  symbol_type = "";
  property_type = "";
  child_symbol_type = "";
  child_property_type = "";

  // Check that the element is a feature-symbol-property-csv-contains-child-property-value element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolPropertyCSVContainsChildPropertyValue Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-symbol-property-csv-contains-child-property-value
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolPropertyCSVContainsChildPropertyValue Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-symbol-property-csv-contains-child-property-value") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolPropertyCSVContainsChildPropertyValue Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
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
      error_msg << "[FeatureSymbolPropertyCSVContainsChildPropertyValue Class Error] Missing \"symbol_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  symbol_type = symbol_type_attr->Value();

  // Read the property_type attribute and set it to the property_type
  const tinyxml2::XMLAttribute* property_type_attr = root->FindAttribute("property_type");
  if( property_type_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolPropertyCSVContainsChildPropertyValue Class Error] Missing \"property_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  property_type = property_type_attr->Value();

  // Read the child_symbol_type attribute and set it to the child_symbol_type
  const tinyxml2::XMLAttribute* child_symbol_type_attr = root->FindAttribute("child_symbol_type");
  if( child_symbol_type_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolPropertyCSVContainsChildPropertyValue Class Error] Missing \"child_symbol_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  child_symbol_type = child_symbol_type_attr->Value();

  // Read the child_property_type attribute and set it to the child_property_type
  const tinyxml2::XMLAttribute* child_property_type_attr = root->FindAttribute("child_property_type");
  if( child_property_type_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolPropertyCSVContainsChildPropertyValue Class Error] Missing \"child_property_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  child_property_type = child_property_type_attr->Value();

  return true;
}

FeatureValue
FeatureSymbolPropertyCSVContainsChildPropertyValue::evaluate( const std::shared_ptr< std::string >& cv,
                                                              const std::shared_ptr<LanguageVariable>& lv,
                                                              const std::shared_ptr<WorldDCG>& world,
                                                              const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;

  // check if the symbol's type matches the feature's symbol_type
  if( symbol->type != symbol_type )
    return value;

  // check for the feature's property_type in the symbol's properties
  // auto explained: std::unordered_map< std::string, std::string>::iterator
  const auto it_symbol_property = symbol->properties.find( property_type );
  if( it_symbol_property == symbol->properties.cend() )
    return value;

  // check that the symbol's property has a csv
  const auto symbol_property_values = h2sl::std_string_vector_from_std_string( it_symbol_property->second );
  if( symbol_property_values.size() <= 1 ){
    //std::cout << "rejecting because csv had one value: " << it_symbol_property->second << std::endl;
    //std::cin.get();
    return value;
  }

  // check if the language variable's children contain a symbol with a type that matches
  // the feature's child_symbol_type
  for( auto& lv_connection : lv->children() ){
    for( auto& child_symbol : lv_connection.child->symbols() ){
      if( child_symbol->type != child_symbol_type )
        continue;

      // check for the feature's child_property_type in the child_symbol's properties
      // auto explained: std::unordered_map< std::string, std::string>::iterator
      const auto it_child_symbol_property = child_symbol->properties.find( child_property_type );
      if( it_child_symbol_property == child_symbol->properties.cend() )
        continue;

/*
      std::cout << "child_symbol_type: " << child_symbol_type << std::endl;
      std::cout << "found the symbol_property_values: ";
      for( auto& symbol_property_value : symbol_property_values ){
        std::cout << "\n" << symbol_property_value;
      }
      std::cout << std::endl;
      std::cin.get();
      auto child_symbol_property_values = h2sl::std_string_vector_from_std_string( it_child_symbol_property->second );
      std::cout << "found the child_symbol_property_values: ";
      for( auto& child_symbol_property_value : child_symbol_property_values ){
        std::cout << "\n" << child_symbol_property_value;
      }
      std::cout << std::endl;
*/
      // check whether the child_symbol's property value matches any of the extracted values
      for( const auto& symbol_property_value : symbol_property_values ){
        if( symbol_property_value == it_child_symbol_property->second ){
/*
          std::cout << "found a match: \n";
          std::cout << "symbol_property_value: " << symbol_property_value << "\n";
          std::cout << "it_child_symbol_property->second: " << it_child_symbol_property->second << std::endl;
          std::cin.get();
*/
          value = FeatureValue::True;
        }
      }
    }
  }

  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureSymbolPropertyCSVContainsChildPropertyValue::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-symbol-property-csv-contains-child-property-value(type=\"" << type << "\",value=\"" << value << "\",symbol_type=\"" << symbol_type << "\",property_type=\"" << property_type << "\",child_symbol_type=\"" << child_symbol_type << "\",child_property_type=\"" << child_property_type << "\")";
  } else {
    out << "feature-symbol-property-csv-contains-child-property-value(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\",property_type=\"" << property_type << "\",child_symbol_type=\"" << child_symbol_type << "\",child_property_type=\"" << child_property_type << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureSymbolPropertyCSVContainsChildPropertyValue::key( void ) const{
  std::stringstream out;
  out << "feature-symbol-property-csv-contains-child-property-value(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\",property_type=\"" << property_type << "\",child_symbol_type=\"" << child_symbol_type << "\",child_property_type=\"" << child_property_type << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureSymbolPropertyCSVContainsChildPropertyValue::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureSymbolPropertyCSVContainsChildPropertyValue::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureSymbolPropertyCSVContainsChildPropertyValue::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_symbol_property_value_matches_child_property_value_elem = document.NewElement( "feature" );
  feature_symbol_property_value_matches_child_property_value_elem->SetAttribute( "class", "feature-symbol-property-csv-contains-child-property-value" );
  feature_symbol_property_value_matches_child_property_value_elem->SetAttribute( "symbol_type", symbol_type.c_str() );
  feature_symbol_property_value_matches_child_property_value_elem->SetAttribute( "property_type", property_type.c_str() );
  feature_symbol_property_value_matches_child_property_value_elem->SetAttribute( "child_symbol_type", child_symbol_type.c_str() );
  feature_symbol_property_value_matches_child_property_value_elem->SetAttribute( "child_property_type", child_property_type.c_str() );
  root->InsertEndChild( feature_symbol_property_value_matches_child_property_value_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureSymbolPropertyCSVContainsChildPropertyValue::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureSymbolPropertyCSVContainsChildPropertyValue::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureSymbolPropertyCSVContainsChildPropertyValue class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureSymbolPropertyCSVContainsChildPropertyValue& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
