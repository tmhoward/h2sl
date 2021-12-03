/**
 * @file feature-symbol-attribute-value.cc
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

#include "h2sl/feature-symbol-attribute-value.h"

namespace h2sl {

///
/// FeatureSymbolAttributeValue default class constructor
///
FeatureSymbolAttributeValue::FeatureSymbolAttributeValue( const std::string& symbolTypeArg,
                                                          const std::string& attributeTypeArg,
                                                          const std::string& attributeValueArg,
                                                          const bool invertArg ) :
                                                            Feature( FeatureType::StaticSymbol,
                                                                FeatureValue::Unknown,
                                                                Feature::dependent_parameters_t( false, false, true, false, false )
                                                            ),
                                                            symbol_type( symbolTypeArg ),
                                                            attribute_type( attributeTypeArg ),
                                                            attribute_value( attributeValueArg ),
                                                            invert( invertArg ){}

///
/// FeatureSymbolAttributeValue class constructor from an XMLElement
///
FeatureSymbolAttributeValue::FeatureSymbolAttributeValue( const tinyxml2::XMLElement* root ) : Feature(), symbol_type(), attribute_type(), attribute_value(), invert() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureSymbolAttributeValue::dup( void )const{
  return std::make_shared<FeatureSymbolAttributeValue>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureSymbolAttributeValue::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::StaticSymbol;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, false, true, false, false );
  symbol_type = "";
  attribute_type = "";
  attribute_value = "";
  invert = false;

  // Check that the element is a feature-symbol-attribute-value element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolAttributeValue Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-symbol-attribute-value
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolAttributeValue Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-symbol-attribute-value") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolAttributeValue Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
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
      error_msg << "[FeatureSymbolAttributeValue Class Error] Missing \"symbol_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  symbol_type = symbol_type_attr->Value();

  // Read the attribute_type attribute and set it to the attribute_type
  const tinyxml2::XMLAttribute* attribute_type_attr = root->FindAttribute("attribute_type");
  if( attribute_type_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolAttributeValue Class Error] Missing \"attribute_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  attribute_type = attribute_type_attr->Value();

  // Read the attribute_value attribute and set it to the attribute_value
  const tinyxml2::XMLAttribute* attribute_value_attr = root->FindAttribute("attribute_value");
  if( attribute_value_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolAttributeValue Class Error] Missing \"attribute_value\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  attribute_value = attribute_value_attr->Value();

  // Read the invert attribute and set it to invert
  const auto* invert_attr = root->FindAttribute("invert");
  if( invert_attr == nullptr ){
    throw std::runtime_error("[FeatureSymbolAttributeValue Class Error] Missing \"invert\" attribute in from_xml()");
  }
  invert = invert_attr->BoolValue();

  return true;
}

FeatureValue FeatureSymbolAttributeValue::evaluate( const std::shared_ptr< std::string >& cv,
                                                    const std::shared_ptr<LanguageVariable>& lv,
                                                    const std::shared_ptr<WorldDCG>& world,
                                                    const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;
  if( symbol->type != symbol_type ){
    return value; // Reject when symbol does not match symbol type
  }


  // lookup the object if the symbol_type is object and the attribute is not the uid
  if( ( symbol_type == "object" ) && ( attribute_type != "uid" ) ){
    std::unordered_map< std::string, std::string >::const_iterator it_object_uid = symbol->properties.find( "uid" );
    if( it_object_uid != symbol->properties.end() ){
      std::map< std::string, Object >::const_iterator it_object = world->objects.find( it_object_uid->second );
      if( it_object != world->objects.end() ){
        // Prepare value for an invert case
        if( invert ) value = FeatureValue::True;
        // check the object for a property that matches the attribute
	std::map< std::string, std::string >::const_iterator it_object_attribute = it_object->second.properties.find( attribute_type );
        if( it_object_attribute != it_object->second.properties.end() ){
          if( it_object_attribute->second == attribute_value ){
            if( !invert ) value = FeatureValue::True;
            else value = FeatureValue::False;
            return value;
          }
        }
      } // Otherwise object is not in the world model
    }
  } else {
    // Prepare value for an invert case
    if( invert ) value = FeatureValue::True;
    // look for the attribute type in the symbol
    std::unordered_map< std::string, std::string >::const_iterator it_symbol_attribute = symbol->properties.find( attribute_type );
    if( it_symbol_attribute != symbol->properties.end() ){
      if( it_symbol_attribute->second == attribute_value ){
        if( !invert ) value = FeatureValue::True;
        else value = FeatureValue::False;
        return value;
      } // Otherwise symbol attribute does not match attribute value, set to false
    } // Otherwise symbol does not have attribute type, set to false
  }
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureSymbolAttributeValue::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-symbol-attribute-value(type=\"" << type << "\",value=\"" << value << "\",symbol_type=\"" << symbol_type << "\",attribute_type=\"" << attribute_type << "\",attribute_value=\"" << attribute_value << "\",invert=" << invert << ")";
  } else {
    out << "feature-symbol-attribute-value(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\",attribute_type=\"" << attribute_type << "\",attribute_value=\"" << attribute_value << "\",invert=" << invert << ")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureSymbolAttributeValue::key( void ) const{
  std::stringstream out;
  out << "feature-symbol-attribute-value(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\",attribute_type=\"" << attribute_type << "\",attribute_value=\"" << attribute_value << "\",invert=" << invert << ")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureSymbolAttributeValue::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureSymbolAttributeValue::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureSymbolAttributeValue::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_symbol_attribute_value_elem = document.NewElement( "feature" );
  feature_symbol_attribute_value_elem->SetAttribute( "class", "feature-symbol-attribute-value" );
  feature_symbol_attribute_value_elem->SetAttribute( "symbol_type", symbol_type.c_str() );
  feature_symbol_attribute_value_elem->SetAttribute( "attribute_type", attribute_type.c_str() );
  feature_symbol_attribute_value_elem->SetAttribute( "attribute_value", attribute_value.c_str() );
  feature_symbol_attribute_value_elem->SetAttribute( "invert", invert );
  root->InsertEndChild( feature_symbol_attribute_value_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureSymbolAttributeValue::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureSymbolAttributeValue::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureSymbolAttributeValue class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureSymbolAttributeValue& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
