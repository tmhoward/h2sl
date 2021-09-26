/**
 * @file symbol.cc
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

#include <cmath>
#include <fstream>
#include <sstream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

#include "h2sl/common.h"
#include "h2sl/symbol.h"
#include "h2sl/feature.h"

namespace h2sl{

//
// Symbol class default constructor
//
Symbol::Symbol( const std::string& typeArg,
                const propertiesMap& propertiesArg,
                const std::vector< Symbol >& childrenArg,
                const std::map< std::string, std::vector< ExpressedFeature > >& expressedFeaturesArg ): type( typeArg ),
                                                                                      properties( propertiesArg ),
                                                                                      children( childrenArg ),  
                                                                                      expressed_features( expressedFeaturesArg ){}

//
// Symbol class constructor from XMLElement*
//
Symbol::Symbol( const tinyxml2::XMLElement* root ) : type(),
                                                     properties(),
                                                     children(),
                                                      expressed_features()
{
  from_xml( root );
}

//
// Class constructor for Symbol from SymbolMessage message.
//
Symbol::Symbol( const SymbolMessage& msg ) : type( msg.type ),
                                             properties(),
                                             children(),
                                             expressed_features(){
  // Import all properties from the message
  for( const auto& property_msg : msg.properties ){
    properties.emplace( property_msg.name, property_msg.value );
  }
}

//
// Symbol class equality operator
//
bool Symbol::operator==( const Symbol& other ) const{
  // Compare the type
  if( type != other.type )
    return false;
  // Compare the properties
  if( properties != other.properties )
    return false;
  // Compare the children
  // Note: this expects the two vectors to be in the same order
  if( children != other.children )
    return false;

  return true;
}

//
// Symbol class inequality operator
//
bool Symbol::operator!=( const Symbol& other ) const{
  return !( *this == other );
}

//
// Method to generate a unique key for the symbol
//
std::string Symbol::key( void ) const{
  std::stringstream out;
  out << type << "{";

  if( properties.size() > 0 ){
    out << "properties(";
    for( auto it_property = properties.cbegin(); it_property != properties.cend(); ++it_property ){
      out << it_property->first << ":" << it_property->second;
      if( std::next( it_property ) != properties.cend() ){
        out << ",";
      }
    }   
    out << ")";
  }
  // Output children
  if( children.size() > 0 ){
    out << ",children(";
    for( auto it_child = children.cbegin();
              it_child != children.cend();
              ++it_child )
    {
      out << (*it_child).key();
      if( std::next(it_child) != children.cend() )
        out << ",";
    }
    out << ")";
  }
  out << "}";
  return out.str();
}

//
// Method to import a Symbol from an XML file
//
bool Symbol::from_xml( const char* filename ){
  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename );

  // Check for loading errors
  if( doc.Error() )
    return false;

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr )
    return false;

  // Check for any Symbol child element of the root
  const tinyxml2::XMLElement* symbol_elem = root->FirstChildElement("symbol");
  if( symbol_elem == nullptr )
    return false;

  return from_xml( symbol_elem );
}

//
// Method to import a Symbol from an XMLElement*
//
bool Symbol::from_xml( const tinyxml2::XMLElement* symbol_elem ){
  type = "";
  properties.clear();
  children.clear(); 

  // Check that the element is a symbol element
  if( symbol_elem == nullptr )
    return false;
  if( std::strcmp(symbol_elem->Name(), "symbol") != 0 )
    return false;

  // Read the type attribute and set it to this Symbol's type
  const tinyxml2::XMLAttribute* type_attr = symbol_elem->FindAttribute("type"); 
  if( type_attr == nullptr )
    return false;
  type = type_attr->Value();

  // Assign the rest of the attributes to this Symbol's properties
  const tinyxml2::XMLAttribute* attribute = symbol_elem->FirstAttribute();
  while( attribute != nullptr ){
    // Skip the "type" attribute; this is the Symbol's type and not a property
    if( std::strcmp(attribute->Name(), "type" ) == 0 ){
      attribute = attribute->Next();
      continue;
    }
    
    // Insert the property
    properties.emplace( attribute->Name(), attribute->Value() );
    attribute = attribute->Next();
  }

  // Import any child Symbols of this Symbol
  const tinyxml2::XMLElement* child_symbol_elem = symbol_elem->FirstChildElement("symbol");
  while( child_symbol_elem != nullptr ){
    Symbol child_symbol = Symbol();

    // Check that the child symbol is imported correctly
    if( !(child_symbol.from_xml( child_symbol_elem )) )
      return false;
    children.push_back( child_symbol );
    child_symbol_elem = child_symbol_elem->NextSiblingElement("symbol");
  } 
  return true;
}

///
/// Method to load from a json formatted file
///
bool Symbol::from_json( const std::string& filename ){
  std::ifstream json_file( filename );
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_file, root );
  return from_json( root );
}

///
/// Method to load from a json formatted string
///
bool Symbol::from_json_string( const std::string& value ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( value, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool Symbol::from_json( const Json::Value& root ){
  type = "";
  properties.clear();
  children.clear();
  for( Json::Value::const_iterator it = root.begin(); it != root.end(); it++ ){
    if( strcmp( it.key().asString().c_str(), "type" ) == 0 ){
      type = root["type"].asString();
    } else {
      properties.emplace( it.key().asString(), root[ it.key().asString() ].asString() );
    }
  }
  return true;
}

//
// Method to export a Symbol to an XML file
//
bool Symbol::to_xml( const char* filename ) const{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertEndChild( root );
  to_xml( doc, root );

  // Save the file and store the error status
  tinyxml2::XMLError error_result = doc.SaveFile( filename );
  if( error_result != tinyxml2::XML_SUCCESS )
    return false;
  return true;
}

//
// Method to export a Symbol to an XMLDocument object
//
void Symbol::to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root) const{
  tinyxml2::XMLElement* symbol_elem = doc.NewElement( "symbol" );
  symbol_elem->SetAttribute( "type", type.c_str() );

  // Add all of the properties
  for( auto const& property : properties ){
    symbol_elem->SetAttribute( property.first.c_str(), 
                               property.second.c_str() );
  }

  // Add the constructed element to the root
  root->InsertEndChild( symbol_elem ); 

  // Export children
  for( auto const& child : children ){
    child.to_xml( doc, symbol_elem );
  }

  if( !expressed_features.empty() ){
    // calculate expressed feature stats
    double denominator = 0.0;
    for( auto & it_expressed_features : expressed_features ){
      double expressed_feature_weights = 0.0;
      for( auto & it_expressed_feature : it_expressed_features.second ){
        expressed_feature_weights += it_expressed_feature.weight;
      }
      denominator += exp( expressed_feature_weights );
    }

    // Export expressed features
    for( auto & it_expressed_features : expressed_features ){
      double expressed_feature_weights = 0.0;
      tinyxml2::XMLElement* expressed_features_elem = doc.NewElement( "expressed_features" );
      for( auto & it_expressed_feature : it_expressed_features.second ){
        expressed_feature_weights += it_expressed_feature.weight;  
        it_expressed_feature.to_xml( doc, expressed_features_elem );
      }
      expressed_features_elem->SetAttribute( "numerator", to_std_string( exp( expressed_feature_weights ) ).c_str() );
      expressed_features_elem->SetAttribute( "denominator", to_std_string( denominator ).c_str() );
      expressed_features_elem->SetAttribute( "expressed_feature_weights", to_std_string( expressed_feature_weights ).c_str() );
      symbol_elem->InsertEndChild( expressed_features_elem );
    }
  }
  return;
}

///
/// Method to write to a json file
///
void Symbol::to_json( const std::string& filename )const{
  std::ofstream outfile;
  outfile.open( filename );
  outfile << to_json();
  outfile.close();
  return;
}

///
/// Method to write to a json formatted string
///
std::string Symbol::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void Symbol::to_json( Json::Value& root )const{
  root["type"] = type;
  for( auto & property : properties ){
    root[property.first] = property.second;
  }
  return;
}

//
// This method exports a Symbol to a SymbolMessage.
//
SymbolMessage Symbol::to_msg( void )const{
  SymbolMessage msg;
  msg.type = type;

  // Export all properties of the Symbol
  for( const auto& property : properties ){
    PropertyMessage p;
    p.name = property.first;
    p.value = property.second;
    msg.properties.push_back( p );
  }

  return msg;
}

//
// Symbol class ostream operator
//
std::ostream& operator<<( std::ostream& out, const Symbol& other ){
  out << other.type << "{(";
  for( auto it = other.properties.cbegin(); it != other.properties.cend(); ++it ){
    out << it->first << "=\"" << it->second << "\"";
    if( std::next(it) != other.properties.cend() )
      out << ", ";
  }
  out << ")";
  if( other.children.size() > 0 ){
    out << ",children(";
    for( auto it = other.children.cbegin(); it != other.children.cend(); ++it ){
      out << *it;
      if( std::next(it) != other.children.cend() )
        out << ",";
    } 
    out << ")";
  }  
  out << "}";
  return out;
}

} // namespace h2sl
