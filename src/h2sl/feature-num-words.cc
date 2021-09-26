/**
 * @file feature-num-words.cc
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

#include "h2sl/common.h"
#include "h2sl/feature-num-words.h"

namespace h2sl {

///
/// FeatureNumWords default class constructor
///
FeatureNumWords::FeatureNumWords( const unsigned int numWordsArg ) :
      Feature( FeatureType::Language, FeatureValue::Unknown, Feature::dependent_parameters_t( false, true, false, false, false ) ),
      num_words( numWordsArg ) {}

///
/// FeatureNumWords class constructor from an XMLElement
///
FeatureNumWords::FeatureNumWords( const tinyxml2::XMLElement* root ) : Feature(), num_words() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureNumWords::dup( void )const{
  return std::make_shared<FeatureNumWords>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureNumWords::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::Language;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, true, false, false, false );
  num_words = 0;

  // Check that the element is a feature-num-words element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureNumWords Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-num-words
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureNumWords Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-num-words") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureNumWords Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Read the type attribute and set it to this Symbol's type
  const tinyxml2::XMLAttribute* num_words_attr = root->FindAttribute("num_words");
  if( num_words_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureNumWords Class Error] Missing \"num_words\" attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }
  num_words = unsigned_int_from_std_string( num_words_attr->Value() );

  return true;
}

FeatureValue FeatureNumWords::evaluate( const std::shared_ptr< std::string >& cv,
                                        const std::shared_ptr<LanguageVariable>& lv,
                                        const std::shared_ptr<WorldDCG>& world,
                                        const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;
  if( lv->words ){
    if( lv->words->size() == num_words ){
      value = FeatureValue::True;
    }
  }
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureNumWords::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-num-words(type=\"" << type << "\",value=\"" << value << "\",num_words=\"" << num_words << "\")";
  } else {
    out << "feature-num-words(type=\"" << type << "\",num_words=\"" << num_words << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureNumWords::key( void ) const{
  std::stringstream out;
  out << "feature-num-words(type=\"" << type << "\",num_words=\"" << num_words << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureNumWords::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureNumWords::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureNumWords::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_num_words_elem = document.NewElement( "feature" );
  feature_num_words_elem->SetAttribute( "class", "feature-num-words" );
  feature_num_words_elem->SetAttribute( "num_words", to_std_string( num_words ).c_str() );
  root->InsertEndChild( feature_num_words_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureNumWords::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureNumWords::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureNumWords class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureNumWords& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
