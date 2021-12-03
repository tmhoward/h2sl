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
/// Sentence class parameter constructor without symbols or a UID
///
Sentence::Sentence( const std::shared_ptr<LanguageVariable>& p_child ) : p_child_( p_child ){}


///
/// Sentence class parameter constructor without symbols
///
Sentence::Sentence( const std::string& uid, const std::shared_ptr<LanguageVariable>& p_child ) : uid_( uid ), p_child_( p_child ){
}

//
// Sentence class parameter constructor with symbols
//
Sentence::Sentence( const std::string& uid,
                    const std::shared_ptr<LanguageVariable>& p_child,
                    const LanguageVariable::symbolsVector& symbols ) : uid_( uid ), p_child_( p_child ), symbols_( symbols ){}

//
// Sentence class constructor from an XMLElement* and a symbolspace
//
Sentence::Sentence( tinyxml2::XMLElement const * p_root, SymbolSpace *const p_symbolspace ) : uid_(), p_child_(), symbols_() {
  from_xml( p_root, p_symbolspace );
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
  return p_child_->all_words();
}

//
// Method to import a Sentence from an XML file without symbols
//
void Sentence::from_xml( const char* filename ){
  from_xml( filename, nullptr );
  return;
}

//
// Method to import a Sentence from an XML file with symbols
//
void Sentence::from_xml( const char* filename, SymbolSpace *const p_symbolspace ){
  // Initialize an error_msg for exception handling
  std::stringstream error_msg;
  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename );
  if( doc.Error() ){
    error_msg << log_error(__FILE__, __LINE__) << "Document error during LoadFile for \""
              << filename << "\" with doc.Error(): " << doc.Error();
    throw std::runtime_error( error_msg.str() );
  }

  // Extract the root element from the document object
  auto const* p_root = doc.RootElement();
  if( p_root == nullptr ){
    error_msg << log_error(__FILE__, __LINE__) << "Document error during RootElement for \"" << filename << "\"";
    throw std::runtime_error( error_msg.str() );
  }

  // Check for any Sentence child element of the root
  auto const* p_sentence_element = p_root->FirstChildElement("sentence");
  if( p_sentence_element == nullptr ){
    error_msg << log_error(__FILE__, __LINE__) << "No sentence element found as a child of the root element for \""
              << filename << "\"";
    throw std::runtime_error( error_msg.str() );
  }
  from_xml( p_sentence_element, p_symbolspace );
  return;
}

//
// Method to import a Sentence from an XMLElement* with symbols
//
void Sentence::from_xml( tinyxml2::XMLElement const * p_sentence_element , SymbolSpace *const p_symbolspace ){
  try{
    // Initialize an error_msg for exception handling
    std::stringstream error_msg;
    // Clear the member data
    reset_();
    // Check that the element is a language variable element
    if( p_sentence_element == nullptr ){
      error_msg << log_error(__FILE__, __LINE__) << "The XMLElement* was nullptr";
      throw std::runtime_error( error_msg.str() );
    }
    if( strcmp( p_sentence_element->Name(), "sentence" ) != 0 ){
      error_msg << log_error(__FILE__, __LINE__) << "The XMLElement* name \"" << p_sentence_element->Name() << "\" did not match"
                << " \"sentence\"";
      throw std::runtime_error( error_msg.str() );
    }
    // Import the uid attribute
    auto const *const p_uid_attribute = p_sentence_element->FindAttribute("uid");
    if( p_uid_attribute == nullptr ){
      error_msg << log_error(__FILE__, __LINE__) << "No uid attribute found";
      throw std::runtime_error( error_msg.str() );
    }
    uid_ = p_uid_attribute->Value();
    // Import child XMLElement Symbols as this LV's symbols; Insert it onto the symbolspace, if not nullptr
    auto const * p_symbol_element = p_sentence_element->FirstChildElement("symbol");
    while( p_symbol_element != nullptr ){
      Symbol symbol;
      // Check that the Symbol is imported correctly
      if( !symbol.from_xml( p_symbol_element ) ){
        error_msg << log_error(__FILE__, __LINE__) << "Failed to load child symbol from xml";
        throw std::runtime_error( error_msg.str() );
      }
      if( p_symbolspace != nullptr ) symbols_.push_back( p_symbolspace->insert( symbol ) );
      else symbols_.push_back( std::make_shared<Symbol>( symbol ) );
      p_symbol_element = p_symbol_element->NextSiblingElement("symbol");
    }
    // Import child XMLElement LanguageVariables as this Sentence's children
    auto const *const p_child_lv_element = p_sentence_element->FirstChildElement("language-variable");
    if( p_child_lv_element == nullptr ){
      error_msg << log_error(__FILE__, __LINE__) << "No child language variable XML elements";
      throw std::runtime_error( error_msg.str() );
    }
    p_child_->from_xml( p_child_lv_element, p_symbolspace, uid_ );
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
  symbols_.clear();
  uid_ = root["uid"].asString();
  p_child_->from_json( root["child"] );
  for( const auto& symbol_root : root["symbols"] ){
    auto symbol = std::make_shared<h2sl::Symbol>();
    symbol->from_json( symbol_root );
    symbols_.push_back(symbol);
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
  sentence_elem->SetAttribute( "uid", uid_.c_str() );

  if( p_child_ != nullptr ){
    p_child_->to_xml( doc, sentence_elem );
  }

  for( auto const& symbol : symbols_ ){
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
  root["uid"] = uid_;
  Json::Value lv_root;
  p_child_->to_json(lv_root);
  root["child"] = lv_root;
  root["symbols"] = Json::Value( Json::arrayValue );
  for( const auto& symbol : symbols_ ){
    Json::Value symbol_root;
    symbol->to_json( symbol_root );
    root["symbols"].append( symbol_root );
  }
  return;
}

//
// Reset the member data to initialization values
//
void Sentence::reset_( void ){
  uid_ = std::string();
  *p_child_ = LanguageVariable();
  symbols_.clear();
  return;
}


//
// Sentence class ostream operator
//
std::ostream& operator<<( std::ostream& out, const Sentence& other ){
  // Output uid
  out << "uid(" << other.uid() << "),";
  // Output child
  if( other.child() != nullptr ) out << "child(" << *other.child() << "),";
  // Output symbols for this Sentence
  if( !other.symbols().empty() ){
    out << "Symbols(";
    for( auto it_symbol = other.symbols().cbegin(); it_symbol != other.symbols().cend(); ++it_symbol ){
      if( *it_symbol != nullptr ) out << **it_symbol;
      if( std::next(it_symbol) != other.symbols().cend() ) out << ", ";
    }
    out << ")";
  }
  return out;
}

} // namespace h2sl
