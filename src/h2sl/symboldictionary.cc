/**
 * @file symboldictionary.cc
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
#include "h2sl/symboldictionary.h"

namespace h2sl{

//
// SymbolDictionary class constructor from XMLElement*
//
SymbolDictionary::SymbolDictionary( const tinyxml2::XMLElement* root ) :
                                                            dictionary(){
  from_xml( root );
}

//
// SymbolDictionary class constructor from filename
//
SymbolDictionary::SymbolDictionary( const char* filename ) : dictionary(){
  from_file( filename );
}

///
/// Method to load from a file
///
bool SymbolDictionary::from_file( const char* filename ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return from_xml( filename );
  } else {
    std::cerr << "[SymbolDictionary Class Error] Erroneous file suffix (\"" << filename << "\") in from_file";
    return false;
  }
}

//
// Method to import a SymbolDictionary from an XML file
//
bool SymbolDictionary::from_xml( const char* filename ){
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

  // Check for any SymbolDictionary child element of the root
  const tinyxml2::XMLElement* symdict_elem = 
                              root->FirstChildElement(XMLSymbolDictionaryName);
  if( symdict_elem == nullptr )
    return false;

  return from_xml( symdict_elem );
}

//
// Method to import a SymbolDictionary from an XMLElement*
//
bool SymbolDictionary::from_xml( const tinyxml2::XMLElement* symdict_elem ){
  dictionary.clear();

  // Check that the element is a symboldictionary element
  if( symdict_elem == nullptr )
    return false;
  if( std::strcmp(symdict_elem->Name(), XMLSymbolDictionaryName) != 0 )
    return false;

  // Check for any child compositional_info XMLElement*
  const tinyxml2::XMLElement* symcompinfo_elem = symdict_elem->FirstChildElement(
                        SymbolCompositionalInfo::XMLSymbolCompositionalInfoName);
  if( symcompinfo_elem == nullptr )
    return false; // Enforce at least one child SymbolCompositionalInfo element
  while( symcompinfo_elem != nullptr ){
    auto symcompinfo = std::make_shared<SymbolCompositionalInfo>();
    if( !symcompinfo->from_xml( symcompinfo_elem ) )
      return false; // SymbolCompositionalInfo from_xml failed
    dictionary.insert(std::make_pair( symcompinfo->symbol_type, symcompinfo ));
    symcompinfo_elem = symcompinfo_elem->NextSiblingElement(
                        SymbolCompositionalInfo::XMLSymbolCompositionalInfoName);
  }
  return true;
}

//
// Method to export a SymbolDictionary to an XML file
//
bool SymbolDictionary::to_xml( const char* filename ) const{
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
// Method to export a SymbolDictionary to an XMLDocument object
//
void SymbolDictionary::to_xml( tinyxml2::XMLDocument& doc,
                               tinyxml2::XMLElement* root ) const
{
  // Create the symboldictionary element to contain the class information
  tinyxml2::XMLElement* symdict_elem = doc.NewElement(XMLSymbolDictionaryName);
  root->InsertEndChild( symdict_elem );

  // Export the dictionary
  for( auto const& symcompinfo : dictionary ){
    // Export the SymbolCompositionalInfo for this Symbol type
    symcompinfo.second->to_xml( doc, symdict_elem );
  }
  return;
}

//
// SymbolDictionary class ostream operator
//
std::ostream& operator<<( std::ostream& out, const SymbolDictionary& other ){
  out << "SymbolDictionary(";
  if( !other.dictionary.empty() ){
    out << "dictionary:(";
    for( auto it = other.dictionary.cbegin(); it != other.dictionary.cend(); ++it ){
      out << *(it->second);
      if( std::next(it) != other.dictionary.cend() )
        out << ",";
    }
    out << ")";
  }
  out << ")";
  return out;
}

const char* SymbolDictionary::XMLSymbolDictionaryName = "symboldictionary";

} // namespace h2sl
