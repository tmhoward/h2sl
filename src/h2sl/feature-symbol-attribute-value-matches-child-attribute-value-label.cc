/**
 * @file feature-symbol-attribute-value-matches-child-attribute-value-label.cc
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

#include "h2sl/feature-symbol-attribute-value-matches-child-attribute-value-label.h"

namespace h2sl {

///
/// FeatureSymbolAttributeValueMatchesChildAttributeValueLabel default class constructor
///
FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::
FeatureSymbolAttributeValueMatchesChildAttributeValueLabel(
                                  const std::string& symbolTypeArg,
                                  const std::string& attributeTypeArg,
                                  const std::string& childSymbolTypeArg,
                                  const std::string& childAttributeTypeArg,
                                  const std::string& edgeLabelArg ) :
    Feature( FeatureType::DynamicSymbol,
             FeatureValue::Unknown,
	     Feature::dependent_parameters_t( false, true, true, true, false ) ),
    symbol_type( symbolTypeArg ),
    attribute_type( attributeTypeArg ),
    child_symbol_type( childSymbolTypeArg ),
    child_attribute_type( childAttributeTypeArg ),
    edge_label( edgeLabelArg ){}

///
/// FeatureSymbolAttributeValueMatchesChildAttributeValueLabel class constructor from an XMLElement
///
FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::FeatureSymbolAttributeValueMatchesChildAttributeValueLabel( const tinyxml2::XMLElement* root ) : Feature(), symbol_type(), attribute_type(), child_symbol_type(), child_attribute_type(), edge_label() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::dup( void )const{
  return std::make_shared<FeatureSymbolAttributeValueMatchesChildAttributeValueLabel>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::DynamicSymbol;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, true, true, true, false );
  symbol_type = "";
  attribute_type = "";
  child_symbol_type = "";
  child_attribute_type = "";
  edge_label = "";

  // Check that the element is a feature-symbol-attribute-value-matches-child-attribute-value-label element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolAttributeValueMatchesChildAttributeValueLabel Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-symbol-attribute-value-matches-child-attribute-value-label
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolAttributeValueMatchesChildAttributeValueLabel Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-symbol-attribute-value-matches-child-attribute-value-label") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolAttributeValueMatchesChildAttributeValueLabel Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
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
      error_msg << "[FeatureSymbolAttributeValueMatchesChildAttributeValueLabel Class Error] Missing \"symbol_type\" attribute in from_xml";
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
      error_msg << "[FeatureSymbolAttributeValueMatchesChildAttributeValueLabel Class Error] Missing \"attribute_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  attribute_type = attribute_type_attr->Value();

  // Read the child_symbol_type attribute and set it to the child_symbol_type
  const tinyxml2::XMLAttribute* child_symbol_type_attr = root->FindAttribute("child_symbol_type");
  if( child_symbol_type_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolAttributeValueMatchesChildAttributeValueLabel Class Error] Missing \"child_symbol_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  child_symbol_type = child_symbol_type_attr->Value();

  // Read the child_attribute_type attribute and set it to the child_attribute_type
  const tinyxml2::XMLAttribute* child_attribute_type_attr = root->FindAttribute("child_attribute_type");
  if( child_attribute_type_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolAttributeValueMatchesChildAttributeValueLabel Class Error] Missing \"child_attribute_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  child_attribute_type = child_attribute_type_attr->Value();

  // Read the edge_label attribute and assign it to edge_label
  const tinyxml2::XMLAttribute* edge_label_attr = root->FindAttribute("edge_label");
  if( edge_label_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolAttributeValueMatchesChildAttributeValueLabel Class Error] Missing \"edge_label\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  edge_label = edge_label_attr->Value();

  return true;
}

FeatureValue
FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::evaluate( const std::shared_ptr< std::string >& cv,
                                                                  const std::shared_ptr<LanguageVariable>& lv,
                                                                  const std::shared_ptr<WorldDCG>& world,
                                                                  const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;

  // check if the symbol's type matches the feature's symbol_type
  if( symbol->type != symbol_type )
    return value;

  // check for the feature's attribute_type in the symbol's properties
  std::string extracted_symbol_attribute;
  if( symbol_type == "object" ){
    auto it_object_uid = symbol->properties.find("uid");
    if( it_object_uid == symbol->properties.end() ) return value;
    auto it_object = world->objects.find( it_object_uid->second );
    if( it_object == world->objects.end() ) return value;
    auto it_object_attribute = it_object->second.properties.find( attribute_type );
    if( it_object_attribute == it_object->second.properties.end() ) return value;
    extracted_symbol_attribute = it_object_attribute->second;
  } else{
    // auto explained: std::unordered_map< std::string, std::string>::iterator
    const auto it_symbol_attribute = symbol->properties.find( attribute_type );
    if( it_symbol_attribute == symbol->properties.cend() ) return value;
    extracted_symbol_attribute = it_symbol_attribute->second;
  }

  // check if the language variable has a child with a matching edge label which
  // contains a symbol with a type that matches the feature's child_symbol_type
  for( auto& lv_connection : lv->children() ){
    if( !lv_connection.label )
      continue; // Reject children LV with no edge label
    if( lv_connection.label.value() != edge_label )
      continue; // Reject children LV with non-matching edge label

    for( auto& child_symbol : lv_connection.child->symbols() ){
      if( child_symbol->type != child_symbol_type )
        continue;

      // check for the feature's child_attribute_type in the child_symbol's properties
      // auto explained: std::unordered_map< std::string, std::string>::iterator
      const auto it_child_symbol_attribute = child_symbol->properties.find( child_attribute_type );
      if( it_child_symbol_attribute == child_symbol->properties.cend() )
        continue;

      // check if the symbol's attribute matches the child_symbol's attribute
      if( it_child_symbol_attribute->second == extracted_symbol_attribute ){
        value = FeatureValue::True;
      }
    }
  }
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-symbol-attribute-value-matches-child-attribute-value-label(type=\"" << type << "\",value=\"" << value << "\",symbol_type=\"" << symbol_type << "\",attribute_type=\"" << attribute_type << "\",child_symbol_type=\"" << child_symbol_type << "\",child_attribute_type=\"" << child_attribute_type << "\",edge_label=\"" << edge_label << "\")";
  } else {
    out << "feature-symbol-attribute-value-matches-child-attribute-value-label(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\",attribute_type=\"" << attribute_type << "\",child_symbol_type=\"" << child_symbol_type << "\",child_attribute_type=\"" << child_attribute_type << "\",edge_label=\"" << edge_label << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::key( void ) const{
  std::stringstream out;
  out << "feature-symbol-attribute-value-matches-child-attribute-value-label(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\",attribute_type=\"" << attribute_type << "\",child_symbol_type=\"" << child_symbol_type << "\",child_attribute_type=\"" << child_attribute_type << "\",edge_label=\"" << edge_label << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_elem = document.NewElement( "feature" );
  feature_elem->SetAttribute( "class", "feature-symbol-attribute-value-matches-child-attribute-value-label" );
  feature_elem->SetAttribute( "symbol_type", symbol_type.c_str() );
  feature_elem->SetAttribute( "attribute_type", attribute_type.c_str() );
  feature_elem->SetAttribute( "child_symbol_type", child_symbol_type.c_str() );
  feature_elem->SetAttribute( "child_attribute_type", child_attribute_type.c_str() );
  feature_elem->SetAttribute( "edge_label", edge_label.c_str() );
  root->InsertEndChild( feature_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureSymbolAttributeValueMatchesChildAttributeValueLabel::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureSymbolAttributeValueMatchesChildAttributeValueLabel class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureSymbolAttributeValueMatchesChildAttributeValueLabel& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
