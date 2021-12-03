/**
 * @file word.cc
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

#include "h2sl/word.h"


#include <fstream>
#include <sstream>
#include <math.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

namespace h2sl{

//
// Word class default constructor
//
Word::Word( const std::string& pos,
            const std::string& text,
            const double time ) : pos( pos ), text( text ), time( time ){}

//
// Word class constructor from XMLElement*
//
Word::Word( const tinyxml2::XMLElement* root ) : pos(), text(), time(){
  from_xml( root );
}

//
// Class constructor for Word from WordMessage message.
//
Word::Word( const WordMessage& msg ) : pos( msg.pos ), text( msg.text ), time( msg.time ){}

//
// Word class equality operator
//
bool Word::operator==( const Word& other ) const{
  // Compare the pos
  if( pos != other.pos )
    return false;
  // Compare the text
  if( text != other.text )
    return false;
  // Compare the time; Difference must be less than a millisecond
  if( !(fabs(time - other.time) < 0.001) ) return false;
  return true;
}

//
// Word class inequality operator
//
bool Word::operator!=( const Word& other ) const{
  return !( *this == other );
}

//
// Method to import a Word from an XML file
//
bool Word::from_xml( const char* filename ){
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

  // Check for any Word child elements of the root
  const tinyxml2::XMLElement* word_elem = root->FirstChildElement("word");
  if( word_elem == nullptr )
    return false;

  return from_xml( word_elem );
}

//
// Method to import a Word from an XMLElement*
//
bool Word::from_xml( const tinyxml2::XMLElement* word_elem ){
  pos = "";
  text = "";
  time = 0;

  // Check that the element is a word element
  if( word_elem == nullptr )
    return false;
  if( std::strcmp(word_elem->Name(), "word") != 0 )
    return false;

  // Read the attributes
  const tinyxml2::XMLAttribute* pos_attr = word_elem->FindAttribute("pos");
  const tinyxml2::XMLAttribute* text_attr = word_elem->FindAttribute("text");
  const tinyxml2::XMLAttribute* time_attr = word_elem->FindAttribute("time");

  // Signal false if part-of-speech attributes is missing
  if(pos_attr == nullptr){
    std::cout << "failed to find part-of-speech" << std::endl;
    return false;
  } else {
    pos = pos_attr->Value();
  }

  // Signal false if text attributes is missing
  if(text_attr == nullptr){
    std::cout << "failed to find text attribute" << std::endl;
    return false;
  } else {
    text = text_attr->Value();
  }

  // Signal false if time attributes is missing
  if(time_attr == nullptr){
    std::cout << "failed to find time attribute (text = \"" << text << "\")" << std::endl;
    return false;
  } else {
    time = time_attr->UnsignedValue();
  }

  return true;
}

///
/// Method to load from a json formatted string
///
bool Word::from_json( const std::string& json_string ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_string, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool Word::from_json( const Json::Value& root ){

  pos = root["pos"].asString();
  text = root["text"].asString();
  time = root["time"].asDouble();

  return true;
}

//
// Method to export a Word to an XML file
//
bool Word::to_xml( const char* filename ) const{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertEndChild( root );
  to_xml( doc, root );

  // Save the file and store the error status;
  tinyxml2::XMLError error_result = doc.SaveFile( filename );
  if( error_result != tinyxml2::XML_SUCCESS )
    return false;
  return true;
}

//
// Method to export a Word to an XMLDocument object
//
void Word::to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root) const{
  tinyxml2::XMLElement* word_elem = doc.NewElement( "word" );
  word_elem->SetAttribute("pos", pos.c_str() );
  word_elem->SetAttribute("text", text.c_str() );
  word_elem->SetAttribute("time", time );
  root->InsertEndChild( word_elem );
  return;
}

///
/// Method to write to a json formatted string
///
std::string Word::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void Word::to_json( Json::Value& root )const{

  root["pos"] = pos;
  root["text"] = text;
  root["time"] = time;

  return;
}

//
// This method exports a Word to a WordMessage.
//
WordMessage Word::to_msg( void )const{
  WordMessage msg;
  msg.pos = pos;
  msg.text = text;
  msg.time = time;
  return msg;
}

//
// Word class ostream operator
//
std::ostream& operator<<( std::ostream& out, const Word& other ){
  out << "word(" << other.pos << ",\"" << other.text << "\"," << other.time
      << ")";
  return out;
}

} // namespace h2sl
