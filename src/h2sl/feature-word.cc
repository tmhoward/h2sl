/**
 * @file feature-word.cc
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

#include "h2sl/feature-word.h"

namespace h2sl {

///
/// FeatureWord default class constructor
///
FeatureWord::FeatureWord( const std::string& posArg, const std::string& textArg ) :
      Feature( FeatureType::Language, FeatureValue::Unknown, Feature::dependent_parameters_t( false, true, false, false, false ) ),
      pos( posArg ),
      text( textArg ){}

///
/// FeatureWord class constructor from an XMLElement
///
FeatureWord::FeatureWord( const tinyxml2::XMLElement* root ) : Feature(), pos(), text() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureWord::dup( void )const{
  return std::make_shared<FeatureWord>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureWord::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::Language;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, true, false, false, false );
  pos = "";
  text = "";

  // Check that the element is a feature-word element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureWord Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-word
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureWord Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-word") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureWord Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Read the type attribute and set it to this Symbol's type
  const tinyxml2::XMLAttribute* pos_attr = root->FindAttribute("pos");
  if( pos_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureWord Class Error] Missing \"pos\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  pos = pos_attr->Value();

  // Read the type attribute and set it to this Symbol's type
  const tinyxml2::XMLAttribute* text_attr = root->FindAttribute("text");
  if( text_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureWord Class Error] Missing \"text\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  text = text_attr->Value();

  return true;
}

FeatureValue FeatureWord::evaluate( const std::shared_ptr< std::string >& cv,
                                    const std::shared_ptr<LanguageVariable>& lv,
                                    const std::shared_ptr<WorldDCG>& world,
                                    const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;
  if( lv->words() == std::nullopt ){
    return value;
  }
  for( auto & word : lv->words().value() ){
    if( word.pos == pos && word.text == text ){
      value = FeatureValue::True;
      return value;
    }
  }
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureWord::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-word(type=\"" << type << "\",value=\"" << value << "\",pos=\"" << pos << "\",text=\"" << text << "\")";
  } else {
    out << "feature-word(type=\"" << type << "\",pos=\"" << pos << "\",text=\"" << text << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureWord::key( void ) const{
  std::stringstream out;
  out << "feature-word(type=\"" << type << "\",pos=\"" << pos << "\",text=\"" << text << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureWord::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureWord::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureWord::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_word_elem = document.NewElement( "feature" );
  feature_word_elem->SetAttribute( "class", "feature-word" );
  feature_word_elem->SetAttribute( "pos", pos.c_str() );
  feature_word_elem->SetAttribute( "text", text.c_str() );
  root->InsertEndChild( feature_word_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureWord::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureWord::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureWord class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureWord& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
