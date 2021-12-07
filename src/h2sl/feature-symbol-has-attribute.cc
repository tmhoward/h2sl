/**
 * @file feature-symbol-has-attribute.cc
 * @author H2SL Developers
 *
 * @version 2.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * @section DESCRIPTION
 */

#include <fstream>
#include <sstream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <boost/algorithm/string.hpp>

#include "h2sl/feature-symbol-has-attribute.h"

namespace h2sl {

///
/// FeatureSymbolHasAttribute default class constructor
///
FeatureSymbolHasAttribute::FeatureSymbolHasAttribute( const std::string& symbolTypeArg,
                                                          const std::string& attributeTypeArg,
                                                          const bool invertArg ) :
                                                            Feature( FeatureType::StaticSymbol,
                                                                FeatureValue::Unknown,
                                                                Feature::dependent_parameters_t( false, false, true, false, false )
                                                            ),
                                                            symbol_type( symbolTypeArg ),
                                                            attribute_type( attributeTypeArg ),
                                                            invert( invertArg ){}

///
/// FeatureSymbolHasAttribute class constructor from an XMLElement
///
FeatureSymbolHasAttribute::FeatureSymbolHasAttribute( const tinyxml2::XMLElement* root ) : Feature(), symbol_type(), attribute_type(), invert() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureSymbolHasAttribute::dup( void )const{
  return std::make_shared<FeatureSymbolHasAttribute>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureSymbolHasAttribute::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::StaticSymbol;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, false, true, false, false );
  symbol_type = "";
  attribute_type = "";
  invert = false;

  // Check that the element is a feature-symbol-has-attribute element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolHasAttribute Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-symbol-has-attribute
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolHasAttribute Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-symbol-has-attribute") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSymbolHasAttribute Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
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
      error_msg << "[FeatureSymbolHasAttribute Class Error] Missing \"symbol_type\" attribute in from_xml";
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
      error_msg << "[FeatureSymbolHasAttribute Class Error] Missing \"attribute_type\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  attribute_type = attribute_type_attr->Value();

  // Read the invert attribute and set it to invert
  const auto* invert_attr = root->FindAttribute("invert");
  if( invert_attr == nullptr ){
    throw std::runtime_error("[FeatureSymbolHasAttribute Class Error] Missing \"invert\" attribute in from_xml()");
  }
  invert = invert_attr->BoolValue();

  return true;
}

FeatureValue FeatureSymbolHasAttribute::evaluate( const std::shared_ptr< std::string >& cv,
                                                    const std::shared_ptr<LanguageVariable>& lv,
                                                    const std::shared_ptr<WorldDCG>& world,
                                                    const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;
  if( symbol->type != symbol_type ){
    return value; // Reject when symbol does not match symbol type
  }

  // Check if the symbol has the attribute
  std::unordered_map< std::string, std::string >::const_iterator it_symbol_attribute = symbol->properties.find( attribute_type );
  if( it_symbol_attribute != symbol->properties.end() ){
    value = (!invert) ? FeatureValue::True : FeatureValue::False;
  } else value = (!invert) ? FeatureValue::False : FeatureValue::True;
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureSymbolHasAttribute::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-symbol-has-attribute(type=\"" << type << "\",value=\"" << value << "\",symbol_type=\"" << symbol_type << "\",attribute_type=\"" << attribute_type << "\",invert=" << invert << ")";
  } else {
    out << "feature-symbol-has-attribute(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\",attribute_type=\"" << attribute_type << "\",invert=" << invert << ")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureSymbolHasAttribute::key( void ) const{
  std::stringstream out;
  out << "feature-symbol-has-attribute(type=\"" << type << "\",symbol_type=\"" << symbol_type << "\",attribute_type=\"" << attribute_type << "\",invert=" << invert << ")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureSymbolHasAttribute::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureSymbolHasAttribute::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureSymbolHasAttribute::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_symbol_has_attribute_elem = document.NewElement( "feature" );
  feature_symbol_has_attribute_elem->SetAttribute( "class", "feature-symbol-has-attribute" );
  feature_symbol_has_attribute_elem->SetAttribute( "symbol_type", symbol_type.c_str() );
  feature_symbol_has_attribute_elem->SetAttribute( "attribute_type", attribute_type.c_str() );
  feature_symbol_has_attribute_elem->SetAttribute( "invert", invert );
  root->InsertEndChild( feature_symbol_has_attribute_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureSymbolHasAttribute::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureSymbolHasAttribute::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureSymbolHasAttribute class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureSymbolHasAttribute& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
