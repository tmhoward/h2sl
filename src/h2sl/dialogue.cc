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
Dialogue::Dialogue( const std::vector< Sentence >& sentences ) : sentences_( sentences ) {}

///
/// Dialogue parameter constructor from a single sentence
///
Dialogue::Dialogue( const Sentence& sentence ) : sentences_() {
  sentences_.push_back( sentence );
}

///
/// Dialogue parameter constructor to load from a file
///
Dialogue::Dialogue( const std::string& filename ) : sentences_() {
  from_file( filename );
}

//
// Method to recursively convert the words member to a string, including children
//
std::string Dialogue::words_to_std_string( void ) const{
  std::string all_words_str;
  for( auto it_sentence = sentences_.cbegin(); it_sentence != sentences_.cend(); ++it_sentence ){
    all_words_str = all_words_str + "[" + it_sentence->uid() + "] " + it_sentence->words_to_std_string();
    if( std::next( it_sentence ) != sentences_.cend() ){
      all_words_str = all_words_str + " ";
    }
  }
  return all_words_str;
}

///
/// Method to load from a file without symbols
///
void Dialogue::from_file( const std::string& filename ) {
  from_file( filename, nullptr );
  return;
}

///
/// Method to load from a file with symbols
///
void Dialogue::from_file( const std::string& filename, SymbolSpace *const p_symbolspace ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    from_xml( filename.c_str(), p_symbolspace );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    from_json( filename );
  } else {
    std::stringstream error_msg;
    error_msg << log_error( __FILE__, __LINE__) << "Erroneous file suffix (\"" << filename << "\")";
    throw std::runtime_error( error_msg.str() );
  }
  return;
}

///
/// Method to load from an XML file without symbols
///
void Dialogue::from_xml( const char* filename ) {
  from_xml( filename, nullptr );
  return;
}

///
/// Method to load from an XML file with symbols
///
void Dialogue::from_xml( const char* filename, SymbolSpace *const p_symbolspace ){
  // Initialize an error_msg for exception handling
  std::stringstream error_msg;
  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename );
  if( doc.Error() ){
    error_msg << log_error(__FILE__, __LINE__) << "Document error during LoadFile for \"" << filename << "\" with doc.Error(): "
              << doc.Error();
    throw std::runtime_error( error_msg.str() );
  }

  // Extract the root element from the document object
  auto const* p_root = doc.RootElement();
  if( p_root == nullptr ){
    error_msg << log_error(__FILE__, __LINE__) << "Document error during RootElement for \"" << filename << "\"";
    throw std::runtime_error( error_msg.str() );
  }

  // Check for any Dialogue child elements of the root
  auto const* p_dialogue_element = p_root->FirstChildElement("dialogue");
  if( p_dialogue_element == nullptr ){
    error_msg << log_error(__FILE__, __LINE__) << "No dialogue element found as a child of the root element for \""
              << filename << "\"";
    throw std::runtime_error( error_msg.str() );
  }
  from_xml( p_dialogue_element, p_symbolspace );
  return;
}

///
/// Method to load from an XMLElement*
///
void Dialogue::from_xml( tinyxml2::XMLElement const * p_dialogue_element, SymbolSpace *const p_symbolspace ){
  try{
    // Initialize an error_msg for exception handling
    std::stringstream error_msg;
    // Clear the member data
    reset_();
    // Check that the element is a dialogue element
    if( p_dialogue_element == nullptr ){
      error_msg << log_error(__FILE__, __LINE__) << "The XMLElement* was nullptr";
      throw std::runtime_error( error_msg.str() );
    }
    if( strcmp( p_dialogue_element->Name(), "dialogue") != 0 ){
      error_msg << log_error(__FILE__, __LINE__) << "The XMLElement* name \"" << p_dialogue_element->Name() << "\" did not match"
                << " \"dialogue\"";
      throw std::runtime_error( error_msg.str() );
    }
    // Import child XMLElement sentences as this Dialogue's sentences
    auto const * p_sentence_element = p_dialogue_element->FirstChildElement("sentence");
    while( p_sentence_element != nullptr ){
      h2sl::Sentence sentence;
      sentence.from_xml( p_sentence_element, p_symbolspace );
      sentences_.push_back( sentence );
      p_sentence_element = p_sentence_element->NextSiblingElement("sentence");
    }
  }
  catch( std::runtime_error& error ){
    reset_();
    throw error;
  }
  return;
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
  sentences_.clear();
  for( const auto& sentence_root : root["sentences"] ){
    h2sl::Sentence sentence = h2sl::Sentence();
    sentence.from_json( sentence_root );
    sentences_.push_back(sentence);
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
  for( auto it_sentence = sentences_.cbegin(); it_sentence != sentences_.cend(); ++it_sentence ){
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
  for( const auto& sentence : sentences_ ){
    Json::Value sentence_root;
    sentence.to_json( sentence_root );
    root["sentences"].append( sentence_root );
  }
  return;
}

///
/// Method to reset the member data
///
void Dialogue::reset_( void ){
  sentences_.clear();
  return;
}

///
/// Dialogue class ostream operator
///
std::ostream& operator<<(std::ostream& out, const Dialogue& other) {
  for( auto it_sentence = other.sentences().cbegin(); it_sentence != other.sentences().cend(); ++it_sentence ){
    out << "(" << *it_sentence << ")";
    if( std::next( it_sentence ) != other.sentences().cend() ) out << ",";
  }
  return out;
}

} // namespace h2sl
