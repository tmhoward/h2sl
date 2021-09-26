/**
 * @file feature-symbol-matches-child.cc
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

#include "h2sl/feature-symbol-matches-child.h"

namespace h2sl {

///
/// FeatureSymbolMatchesChild default class constructor
///
FeatureSymbolMatchesChild::FeatureSymbolMatchesChild( const std::string& symbolTypeArg ) :
    Feature( FeatureType::DynamicSymbol, FeatureValue::Unknown, Feature::dependent_parameters_t( false, false, true, true, false ) ),
    symbol_type( symbolTypeArg ) {}

///
/// FeatureSymbolMatchesChild class constructor from an XMLElement
///
FeatureSymbolMatchesChild::FeatureSymbolMatchesChild( const tinyxml2::XMLElement* root ) : Feature(), symbol_type() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureSymbolMatchesChild::dup( void )const{
  return std::make_shared<FeatureSymbolMatchesChild>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureSymbolMatchesChild::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::DynamicSymbol;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, false, true, true, false );
  symbol_type = "";

  // Check that the element is a feature-symbol-matches-child element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolMatchesChild Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
 
  // check to see if the class name is feature-symbol-matches-child
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolMatchesChild Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-symbol-matches-child") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolMatchesChild Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
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
      error_msg << "[FeatureSymbolMatchesChild Class Error] Missing \"symbol_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  symbol_type = symbol_type_attr->Value();

  return true;
}

FeatureValue FeatureSymbolMatchesChild::evaluate( const std::shared_ptr< std::string >& cv,
                                                  const std::shared_ptr<LanguageVariable>& lv,
                                                  const std::shared_ptr<WorldDCG>& world,
                                                  const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;
  if( symbol->type == symbol_type ){
    // look for the matching symbol in the language variable child groundings  
    for( auto & child_lv_connection : lv->children ){ 
      for( auto & child_symbol : child_lv_connection.child->symbols ){
        if( child_symbol->type == symbol_type ){
          if( *symbol == *child_symbol ){
            value = FeatureValue::True;
            return value;
          }
        }
      }
    } 
  }

  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureSymbolMatchesChild::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-symbol-matches-child(type=\"" << type << "\",value=\"" << value << "\",symbol_type=\"" << symbol_type << "\")";
  } else {
    out << "feature-symbol-matches-child(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureSymbolMatchesChild::key( void ) const{
  std::stringstream out;
  out << "feature-symbol-matches-child(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureSymbolMatchesChild::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureSymbolMatchesChild::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureSymbolMatchesChild::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_symbol_matches_child_elem = document.NewElement( "feature" );
  feature_symbol_matches_child_elem->SetAttribute( "class", "feature-symbol-matches-child" );
  feature_symbol_matches_child_elem->SetAttribute( "symbol_type", symbol_type.c_str() );
  root->InsertEndChild( feature_symbol_matches_child_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureSymbolMatchesChild::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureSymbolMatchesChild::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureSymbolMatchesChild class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureSymbolMatchesChild& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
