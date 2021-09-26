/**
 * @file dialogue.cc
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

#include "h2sl/dialogue.h"
#include "h2sl/common.h"

namespace h2sl {

///
/// Dialogue parameter constructor from a vector of sentences
///
Dialogue::Dialogue( const std::vector< Sentence >& sentencesArg ) : sentences( sentencesArg ) {

}

///
/// Dialogue parameter constructor from a single sentence
///
Dialogue::Dialogue( const Sentence& sentence ) : sentences() {
  sentences.push_back( sentence );
}

///
/// Dialogue parameter constructor to load from a file
///
Dialogue::Dialogue( const std::string& filename ) : sentences() {
  from_file( filename );
}

//
// Method to recursively convert the words member to a string, including children
//
std::string Dialogue::words_to_std_string( void ) const{
  std::string all_words_str;
  for( auto it_sentence = sentences.cbegin();
            it_sentence != sentences.cend();
            ++it_sentence )
  {
    all_words_str = all_words_str + "[" + it_sentence->uid + "] " + it_sentence->words_to_std_string();
    if( std::next( it_sentence ) != sentences.cend() ){
      all_words_str = all_words_str + " ";
    }
  }
  return all_words_str;
}

///
/// Method to load from a file without symbols
///
bool Dialogue::from_file( const std::string& filename ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return from_xml( filename );
  } else {
    std::cerr << "[Dialogue Class Error] Erroneous file suffix (\"" << filename << "\") in from_file";
    return false;
  }
}

///
/// Method to load from a file with symbols
///
bool Dialogue::from_file( const std::string& filename, std::shared_ptr<SymbolSpace>& symbolSpace ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return from_xml( filename, symbolSpace );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return from_json( filename );
  } else {
    std::cerr << "[Dialogue Class Error] Erroneous file suffix (\"" << filename << "\") in from_file";
    return false;
  }
}

///
/// Method to load from an XML file without symbols
///
bool Dialogue::from_xml( const std::string& filename ) {
  tinyxml2::XMLDocument doc;
  doc.LoadFile(filename.c_str());
  const tinyxml2::XMLElement *root = doc.FirstChildElement("root");
  if (root != nullptr) {
    const tinyxml2::XMLElement *child = root->FirstChildElement();
    while( child != nullptr ){
      from_xml(child);
      child = child->NextSiblingElement();
    }
  }
  return doc.ErrorID();
}

///
/// Method to load from an XML file with symbols
///
bool Dialogue::from_xml( const std::string& filename, std::shared_ptr<SymbolSpace>& symbolSpace ) {
  tinyxml2::XMLDocument doc;
  doc.LoadFile(filename.c_str());
  const tinyxml2::XMLElement *root = doc.FirstChildElement("root");
  if (root != nullptr) {
    const tinyxml2::XMLElement *child = root->FirstChildElement();
    while( child != nullptr ){
      from_xml(child, symbolSpace);
      child = child->NextSiblingElement();
    }
  }
  return doc.ErrorID();
}

///
/// Method to load from a tinyxml2 element without symbols
///
bool Dialogue::from_xml( const tinyxml2::XMLElement* dialogueElem ){
  if( dialogueElem == nullptr )
    return false;

  if( std::strcmp( dialogueElem->Name(), "dialogue" ) == 0 ){
    sentences.clear();
    // Iterate through all of the children and load each sentence into the sentences vector
    const tinyxml2::XMLElement* sentence_elem = dialogueElem->FirstChildElement("sentence");
    while( sentence_elem != nullptr ){
      Sentence sentence = Sentence();
      if( !(sentence.from_xml( sentence_elem ) ) )
        return false;
      sentences.push_back( sentence );
    }
  } else if ( std::strcmp( dialogueElem->Name(), "sentence" ) == 0 ){
    sentences.clear();
    Sentence sentence = Sentence();
    if( !(sentence.from_xml( dialogueElem ) ) )
      return false;
    sentences.push_back( sentence );
  }

  return true;
}

///
/// Method to load from a tinyxml2 element with symbols
///
bool Dialogue::from_xml( const tinyxml2::XMLElement* dialogueElem,
                          std::shared_ptr<SymbolSpace>& symbolSpace ){
  if( dialogueElem == nullptr )
    return false;

  if( std::strcmp( dialogueElem->Name(), "dialogue" ) == 0 ){
    sentences.clear();
    // Iterate through all of the children and load each sentence into the sentences vector
    const tinyxml2::XMLElement* sentence_elem = dialogueElem->FirstChildElement("sentence");
    while( sentence_elem != nullptr ){
      Sentence sentence = Sentence();
      if( !(sentence.from_xml( sentence_elem, symbolSpace ) ) )
        return false;
      sentences.push_back( sentence );
    }
  } else if ( std::strcmp( dialogueElem->Name(), "sentence" ) == 0 ){
    sentences.clear();
    Sentence sentence = Sentence();
    if( !(sentence.from_xml( dialogueElem, symbolSpace ) ) )
      return false;
    sentences.push_back( sentence );
  }
  
  return true;
}

///
/// Method to load from a json formatted string
///
bool Dialogue::from_json( const std::string& json_string ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_string, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool Dialogue::from_json( const Json::Value& root ){
  
  sentences.clear();

  for( const auto& sentence_root : root["sentences"] ){
    h2sl::Sentence sentence = h2sl::Sentence();
    sentence.from_json( sentence_root );
    sentences.push_back(sentence);
  }

  return true;
}

///
/// Method to write to a file
///
void Dialogue::to_file( const std::string& filename )const {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return to_xml( filename );
  } else {
    try {
      std::stringstream error_msg;
      error_msg << "[Dialogue Class Error] Erroneous file suffix (\"" << filename << "\") in to_file";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
  }
  return;
}

///
/// Method to write to an XML file
///
void Dialogue::to_xml( const std::string& filename )const {
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement * root = doc.NewElement( "root" );
  doc.InsertEndChild( root );
  to_xml( doc, root );
  doc.SaveFile( filename.c_str() );
  return;
}

///
/// Method to write to a tinyxml2 document
///
void Dialogue::to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* dialogue_elem = doc.NewElement( "dialogue" );
  dialogue_elem->SetAttribute( "text", words_to_std_string().c_str() );
  for( auto it_sentence = sentences.cbegin();
            it_sentence != sentences.cend();
            ++it_sentence )
  {
    it_sentence->to_xml( doc, dialogue_elem );
  }
  root->InsertEndChild( dialogue_elem );
}

///
/// Method to write to a json formatted string
///
std::string Dialogue::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void Dialogue::to_json( Json::Value& root )const{

  root["sentences"] = Json::Value( Json::arrayValue ); 

  for( const auto& sentence : sentences ){
    Json::Value sentence_root;
    sentence.to_json( sentence_root );
    root["sentences"].append( sentence_root );
  }

  return;
}

///
/// Dialogue class ostream operator
///
std::ostream& operator<<(std::ostream& out, const Dialogue& other) {
  for( auto it_sentence = other.sentences.cbegin();
            it_sentence != other.sentences.cend();
            ++it_sentence )
  {
    out << "(" << *it_sentence << ")";
    if( std::next( it_sentence ) != other.sentences.cend() ){
      out << ",";
    }
  }
  return out;
}

} // namespace h2sl
