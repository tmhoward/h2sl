/**
 * @file symbolcompositionalinfo.cc
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

#include "h2sl/common.h"
#include "h2sl/symbolcompositionalinfo.h"

namespace h2sl{

//
// SymbolCompositionalInfo class default constructor
//
SymbolCompositionalInfo::SymbolCompositionalInfo( 
    const std::string& symbol_type,
    const std::unordered_set<std::string>& language_variable_types,
    const mapSetSymbolProperties& properties,
    const mapSymbolCompositionalInfo& children ) : symbol_type( symbol_type ),
                                                  language_variable_types( language_variable_types ),
                                                  properties( properties ),
                                                  children( children ){}

//
// SymbolCompositionalInfo class constructor from XMLElement*
//
SymbolCompositionalInfo::SymbolCompositionalInfo( 
                const tinyxml2::XMLElement* root ) : symbol_type(),
                                                    language_variable_types(),
                                                    properties(),
                                                    children()
{
  from_xml( root );
}

//
// Method to scrape compositional information from a Symbol
//
bool SymbolCompositionalInfo::scrape_symbol( const std::string& language_variable_type,
                                             Symbol& symbol )
{
  // Check that symbol's type matches this symbol_type
  if( symbol.type != symbol_type )
    return false;

  // Check if the language_variable_type is in language_variable_types, insert if no
  if( language_variable_types.find( language_variable_type ) == language_variable_types.end() )
    language_variable_types.insert( language_variable_type );

  // Check each of symbol's properties to see if it is in properties
  // If not, insert a new pair for that property type
  for( auto const& symbol_property : symbol.properties ){
    auto it_properties = properties.find( symbol_property.first );
    if( it_properties == properties.end() ){
      std::unordered_set<std::string> symbol_property_value = { symbol_property.second };
      properties.emplace( symbol_property.first, symbol_property_value );
    } else{
      // Check if the property value is in properties, insert it if not
      if( it_properties->second.find( symbol_property.second ) == 
                                      it_properties->second.end() )
      {
        it_properties->second.insert( symbol_property.second );
      }
    }
  }

  // Scrape symbol's children
  for( auto& symbol_child : symbol.children ){
    // Check if any SymbolCompositionalInfo children have a matching symbol type
    auto it_children = children.find( symbol_child.type );
    if( it_children == children.end() ){
      auto result = children.emplace( symbol_child.type,
              std::make_shared<SymbolCompositionalInfo>( symbol_child.type ) );
      it_children = result.first;
    }
    if( !it_children->second->scrape_symbol( language_variable_type, symbol_child ) )
      return false;
  }
  return true;
}

//
// Method to import a SymbolCompositionalInfo from an XML file
//
bool SymbolCompositionalInfo::from_xml( const char* filename ){
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

  // Check for any SymbolCompositionalInfo child element of the root
  const tinyxml2::XMLElement* symcompinfo_elem = 
                      root->FirstChildElement( XMLSymbolCompositionalInfoName );
  if( symcompinfo_elem == nullptr )
    return false;

  return from_xml( symcompinfo_elem );
}

//
// Method to import a SymbolCompositionalInfo from an XMLElement*
//
bool SymbolCompositionalInfo::from_xml( const tinyxml2::XMLElement* symcompinfo_elem ){
  symbol_type = "";
  language_variable_types.clear();
  properties.clear();
  children.clear();

  // Check that the element is a symbolcompositionalinfo element
  if( symcompinfo_elem == nullptr )
    return false;
  if( std::strcmp(symcompinfo_elem->Name(), XMLSymbolCompositionalInfoName) != 0 )
    return false;

  // Import the symbol_type from the XMLAttribute
  const tinyxml2::XMLAttribute* symbol_type_attr = 
                                symcompinfo_elem->FindAttribute(XMLSymbolTypeName);
  if( symbol_type_attr == nullptr )
    return false;
  symbol_type = symbol_type_attr->Value();

  // Import the language_variable_types from the XMLAttribute & parse the comma-separated values
  const tinyxml2::XMLAttribute* language_variable_types_attr = 
                                symcompinfo_elem->FindAttribute(XMLLanguageVariableTypesName);
  if( language_variable_types_attr == nullptr )
    return false;
  // Function call from h2sl::common to parse CSV string into std::unordered_set
  language_variable_types = std_string_u_set_from_std_string( language_variable_types_attr->Value() );

  // Import the properties from a child XMLElement whose attributes are the
  // property name (attribute name) and property values (attribute value)
  const tinyxml2::XMLElement* symbol_properties_elem = 
                              symcompinfo_elem->FirstChildElement(XMLPropertiesName);
  if( symbol_properties_elem == nullptr )
    return false;

  // Each property name and associated values is an XMLAttribute
  const tinyxml2::XMLAttribute* symbol_property_attr = 
                                        symbol_properties_elem->FirstAttribute();
  // Reject if there are zero properties
  if( symbol_property_attr == nullptr )
    return false;
  while( symbol_property_attr != nullptr ){
    std::string symbol_property_name = symbol_property_attr->Name();
    // Function call from h2sl::common to parse CSV string into std::unordered_set
    auto symbol_property_values = 
              std_string_u_set_from_std_string( symbol_property_attr->Value() );
    properties.insert(std::make_pair( symbol_property_name, symbol_property_values ));
    symbol_property_attr = symbol_property_attr->Next();
  }

  // Import all child SymbolCompositionalInfo via recursion
  const tinyxml2::XMLElement* symcompinfo_child_elem = 
            symcompinfo_elem->FirstChildElement(XMLSymbolCompositionalInfoName);
  while( symcompinfo_child_elem != nullptr ){
    std::shared_ptr<SymbolCompositionalInfo> child = 
                                  std::make_shared<SymbolCompositionalInfo>();
    if( !child->from_xml( symcompinfo_child_elem ) )
      return false; 
    children.insert(std::make_pair( child->symbol_type, child ));
    symcompinfo_child_elem->NextSiblingElement(XMLSymbolCompositionalInfoName);
  }
  return true;
}

//
// Method to export a SymbolCompositionalInfo to an XML file
//
bool SymbolCompositionalInfo::to_xml( const char* filename ) const{
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
// Method to export a SymbolCompositionalInfo to an XMLDocument object
//
void SymbolCompositionalInfo::to_xml( tinyxml2::XMLDocument& doc, 
                                      tinyxml2::XMLElement* root) const
{
  tinyxml2::XMLElement* symcompinfo_elem =
                              doc.NewElement( XMLSymbolCompositionalInfoName );
  // Export symbol_type as an XMLAttribute
  symcompinfo_elem->SetAttribute( XMLSymbolTypeName, symbol_type.c_str() );

  // Export language_variable_types as an XMLAttribute
  std::string lv_types_string = std_string_u_set_to_std_string( language_variable_types );
  symcompinfo_elem->SetAttribute( XMLLanguageVariableTypesName, lv_types_string.c_str() );

  // Add the element as a child to root
  root->InsertEndChild( symcompinfo_elem );

  // Export properties as a child XMLElement with XMLAttributes for each property
  // with the property name as the attribute name and the property values as the 
  // attribute value
  tinyxml2::XMLElement* symbol_properties_elem = doc.NewElement(XMLPropertiesName);
  for( auto const& property : properties ){
    std::string symbol_property_name = property.first;
    // Function call to h2sl::common to create a CSV string from an unordered_set
    std::string symbol_property_values = std_string_u_set_to_std_string( property.second );
    symbol_properties_elem->SetAttribute( symbol_property_name.c_str(),
                                          symbol_property_values.c_str() );
  }
  
  // Add the element as a child to symcompinfo_elem
  symcompinfo_elem->InsertEndChild( symbol_properties_elem );

  // Add children via recursive call
  for( auto const& child : children ){
    child.second->to_xml( doc, symcompinfo_elem );
  }
  return;
}

//
// SymbolCompositionalInfo class ostream operator
//
std::ostream& operator<<( std::ostream& out, const SymbolCompositionalInfo& other ){
  out << "SymbolCompositionalInfo( symbol_type: \"" << other.symbol_type << "\",";
  out << "language_variable_types(" << std_string_u_set_to_std_string( other.language_variable_types ) << "),";
  out << "properties(";
  for( auto it = other.properties.cbegin(); it != other.properties.cend(); ++it ){
    out << "\"" << it->first << "\":(" << std_string_u_set_to_std_string( it->second )
        <<")";
    if( std::next(it) != other.properties.cend() )
      out << ", ";
  }
  if( !other.children.empty() ){
    out << ",children(";
    for( auto it = other.children.cbegin(); it != other.children.cend(); it++ ){
      out << *(it->second);
      if( std::next(it) != other.children.cend() )
        out << ",";
    }
    out << ")";
  }
  out << ")";
  return out;
}

//
// Constant used for XMLElement Name for SymbolCompositionalInfo element
//
const char* SymbolCompositionalInfo::
XMLSymbolCompositionalInfoName = "symbolcompositionalinfo";

//
// Constant used for XMLAttribute Name for symbol_type
//
const char* SymbolCompositionalInfo::
XMLSymbolTypeName = "symbol_type";

//
// Constant used for XMLAttribute Name for language_variable_types
//
const char* SymbolCompositionalInfo::
XMLLanguageVariableTypesName = "language_variable_types";

//
// Constant used for XMLElement Name for properties
//
const char* SymbolCompositionalInfo::
XMLPropertiesName = "properties";

} // namespace h2sl
