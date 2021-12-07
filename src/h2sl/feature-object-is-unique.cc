/**
 * @file feature-object-is-unique.cc
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

#include "h2sl/feature-object-is-unique.h"

namespace h2sl {

///
/// FeatureObjectIsUnique default class constructor
///
FeatureObjectIsUnique::FeatureObjectIsUnique( const bool invertArg ) :
    Feature( FeatureType::DynamicSymbol, FeatureValue::Unknown, Feature::dependent_parameters_t( false, false, true, false, true ) ),
    invert( invertArg ) {}

///
/// FeatureObjectIsUnique class constructor from an XMLElement
///
FeatureObjectIsUnique::FeatureObjectIsUnique( const tinyxml2::XMLElement* root ) : Feature(), invert() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureObjectIsUnique::dup( void )const{
  return std::make_shared<FeatureObjectIsUnique>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureObjectIsUnique::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::DynamicSymbol;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, false, true, false, true );
  invert = false;

  // Check that the element is a feature-object-is-unique element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectIsUnique Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
 
  // check to see if the class name is feature-object-is-unique
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectIsUnique Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-object-is-unique") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureObjectIsUnique Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
 
    // Read the invert attribute and set it to invert
  const auto* invert_attr = root->FindAttribute("invert");
  if( invert_attr == nullptr ){
    throw std::runtime_error("[FeatureSymbolAttributeValue Class Error] Missing \"invert\" attribute in from_xml()");
  }
  invert = invert_attr->BoolValue();

  return true;
}

FeatureValue FeatureObjectIsUnique::evaluate( const std::shared_ptr< std::string >& cv,
                                                  const std::shared_ptr<LanguageVariable>& lv,
                                                  const std::shared_ptr<WorldDCG>& world,
                                                  const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;

  // Enforce that the passed symbol is an object; early return false otherwise
  if( symbol->type != "object" ) return value;
  // Extract the object from the world via the UID property
  auto it_object_uid = symbol->properties.find( "uid" );
  // No "uid" property found, return false;
  if( it_object_uid == symbol->properties.end() ) return value;
  auto it_object = world->objects.find( it_object_uid->second );                                                                       
  // Enforce that the object is in the world && that the object has a state with pose
  if( it_object == world->objects.end() ) return value;

  // Copy the object to an object for readability
  auto object_symbol = it_object->second;
  // Get object_type
  const auto& object_type = object_symbol.properties["object_type"];

  // Look for other objects of the same type in the world
  for( auto & [uid, object] : world->objects ){
    if( uid == it_object_uid->second ) continue;
    if( object_type == object.properties["object_type"] ){
      value = (!invert) ? FeatureValue::False : FeatureValue::True;
      return value;
    }
  }
  value = (!invert) ? FeatureValue::True : FeatureValue::False;
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureObjectIsUnique::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-object-is-unique(type=\"" << type << "\",value=\"" << value << "\",invert= " << invert << "\")";
  } else {
    out << "feature-object-is-unique(type=\"" << type << "\",invert= " << invert << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureObjectIsUnique::key( void ) const{
  std::stringstream out;
  out << "feature-object-is-unique(type=\"" << type << "\",invert= " << invert << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureObjectIsUnique::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureObjectIsUnique::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureObjectIsUnique::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_symbol_has_child_elem = document.NewElement( "feature" );
  feature_symbol_has_child_elem->SetAttribute( "class", "feature-object-is-unique" );
  feature_symbol_has_child_elem->SetAttribute( "invert", invert );
  root->InsertEndChild( feature_symbol_has_child_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureObjectIsUnique::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureObjectIsUnique::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureObjectIsUnique class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureObjectIsUnique& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
