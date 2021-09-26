/**
 * @file sentence.cc
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

#include <algorithm>

#include <fstream>
#include <sstream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

#include "h2sl/sentence.h"
#include "h2sl/common.h"

namespace h2sl{
///
/// Sentence class parameter constructor without symbols
///
Sentence::Sentence( const std::string& uidArg, const std::shared_ptr<LanguageVariable>& childArg ) : uid( uidArg ), child( childArg ){}

//
// Sentence class parameter constructor with symbols
//
Sentence::Sentence( const std::string& uidArg,
                    const std::shared_ptr<LanguageVariable>& childArg,
                    const LanguageVariable::symbolsVector& symbolsArg ) : uid( uidArg ),
                                                                child( childArg ),
                                                                symbols( symbolsArg ){}

Sentence::Sentence( const tinyxml2::XMLElement* root,
                      std::shared_ptr<SymbolSpace>& symbolSpace ) : uid(),
                                                    child(),
                                                    symbols() {
  from_xml( root, symbolSpace );
}

//
// Method to recursively convert the words member to a string, including children
//
std::string Sentence::words_to_std_string( void ) const{
  std::string all_words_str;
  auto all_words_vec = all_words();
  std::sort( all_words_vec.begin(), all_words_vec.end(), word_time_sort );
  for( auto it_word = all_words_vec.cbegin(); 
      it_word != all_words_vec.cend(); ++it_word )
  {
    all_words_str = all_words_str + it_word->text;
    if( std::next(it_word) != all_words_vec.cend() )
      all_words_str = all_words_str + " ";
  }
  return all_words_str;
}

//
// Method to recursively return all Words in a vector, including from children
//
std::vector<Word> Sentence::all_words( void ) const{
  return child->all_words();
}

//
// Method to import a Sentence from an XML file without symbols
//
bool Sentence::from_xml( const char* filename ){
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

  // Check for any "sentence" child element of the root
  const tinyxml2::XMLElement* sentence_elem = root->FirstChildElement("sentence");
  if( sentence_elem == nullptr )
    return false;

  return from_xml( sentence_elem );
}

//
// Method to import a Sentence from an XML file with symbols
//
bool Sentence::from_xml( const char* filename, std::shared_ptr<SymbolSpace>& symbolspace ){
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

  // Check for any LanguageVariable child element of the root
  const tinyxml2::XMLElement* sentence_elem = root->FirstChildElement("sentence");
  if( sentence_elem == nullptr )
    return false;

  return from_xml( sentence_elem, symbolspace );
}

//
// Method to import a Sentence from an XMLElement* without symbols
//
bool Sentence::from_xml( const tinyxml2::XMLElement* sentence_elem ){
  uid = "";
  *child = LanguageVariable();
  symbols.clear();

  // Check that the element is a sentence element
  if( sentence_elem == nullptr )
    return false;
  if( std::strcmp(sentence_elem->Name(), "sentence") != 0 )
    return false;

  // Read the uid attribute and set it to this Sentence's uid
  const tinyxml2::XMLAttribute* uid_attr = sentence_elem->FindAttribute("uid");
  if( uid_attr == nullptr )
    return false;
  uid = uid_attr->Value();
  
  // Import child XMLElement LanguageVariables as this Sentence's children
  const tinyxml2::XMLElement* child_language_variable_elem =
                              sentence_elem->FirstChildElement("language-variable");
  while( child_language_variable_elem != nullptr ){
    // Check that the child LanguageVariable is imported correctly
    if( !(child->from_xml( child_language_variable_elem )) )
      return false;
    child_language_variable_elem = child_language_variable_elem->NextSiblingElement("language-variable");
  }

  return true;
}

//
// Method to import a Sentence from an XMLElement* with symbols
//
bool Sentence::from_xml( const tinyxml2::XMLElement* sentence_elem,
                                      std::shared_ptr<SymbolSpace>& symbolspace ){
  uid = "";
  *child = LanguageVariable();
  symbols.clear();

  // Check that the element is a sentence element
  if( sentence_elem == nullptr )
    return false;
  if( std::strcmp(sentence_elem->Name(), "sentence") != 0 )
    return false;

  // Read the uid attribute and set it to this Sentence's uid
  const tinyxml2::XMLAttribute* uid_attr = sentence_elem->FindAttribute("uid");
  if( uid_attr == nullptr )
    return false;
  uid = uid_attr->Value();

  // Import child XMLElement Symbols as this Sentence's symbols
  const tinyxml2::XMLElement* symbol_elem = sentence_elem->FirstChildElement("symbol");
  while( symbol_elem != nullptr ){
    Symbol symbol = Symbol();

    // Check that the Symbol is imported correctly
    if( !(symbol.from_xml( symbol_elem )) )
      return false;
    symbols.push_back( symbolspace->insert(symbol) );
    symbol_elem = symbol_elem->NextSiblingElement("symbol");
  }

  // Import child XMLElement LanguageVariables as this Sentence's children
  const tinyxml2::XMLElement* child_language_variable_elem = 
                              sentence_elem->FirstChildElement("language-variable");
  while( child_language_variable_elem != nullptr ){
    // Check that the child LanguageVariable is imported correctly
    if( !(child->from_xml( child_language_variable_elem, symbolspace )) )
      return false;
    child_language_variable_elem = child_language_variable_elem->NextSiblingElement("language-variable");
  }
  return true;
}

///
/// Method to load from a json formatted string
///
bool Sentence::from_json( const std::string& json_string ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_string, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool Sentence::from_json( const Json::Value& root ){

  symbols.clear();

  uid = root["uid"].asString();
  
  child->from_json( root["child"] );

  for( const auto& symbol_root : root["symbols"] ){
    auto symbol = std::make_shared<h2sl::Symbol>();
    symbol->from_json( symbol_root );
    symbols.push_back(symbol);
  }

  return true;
}

//
// Method to export a Sentence to an XML file
//
bool Sentence::to_xml( const char* filename ) const{
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
// Method to export a Sentence to an XMLDocument object
//
void Sentence::to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root) const{
  tinyxml2::XMLElement* sentence_elem = doc.NewElement("sentence");
  sentence_elem->SetAttribute( "text", words_to_std_string().c_str() );

  // Add the constructed element to the root
  root->InsertEndChild( sentence_elem );
  
  sentence_elem->SetAttribute( "uid", uid.c_str() );

  if( child != nullptr ){
    child->to_xml( doc, sentence_elem );
  }

  for( auto const& symbol : symbols ){
    symbol->to_xml( doc, sentence_elem );
  }

  return;
}

///
/// Method to write to a json formatted string
///
std::string Sentence::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void Sentence::to_json( Json::Value& root )const{

  root["uid"] = uid;

  Json::Value lv_root;
  child->to_json(lv_root);
  root["child"] = lv_root;
  
  root["symbols"] = Json::Value( Json::arrayValue ); 

  for( const auto& symbol : symbols ){
    Json::Value symbol_root;
    symbol->to_json( symbol_root );
    root["symbols"].append( symbol_root );
  }

  return;
}

//
// Sentence class ostream operator
//
std::ostream& operator<<( std::ostream& out, const Sentence& other ){
  // Output uid
  out << "uid(" << other.uid << "),";

  // Output child
  out << "child(" << *other.child << "),";

  // Output symbols for this Sentence
  if( other.symbols.size() > 0 ){
    out << "Symbols(";
    for( auto it_symbol = other.symbols.cbegin();
                it_symbol != other.symbols.cend();
                ++it_symbol )
    {
      if( *it_symbol != nullptr )
        out << **it_symbol;
      if( std::next(it_symbol) != other.symbols.cend() )
        out << ", ";
    }
    out << ")";
  }
  return out;
}

} // namespace h2sl
