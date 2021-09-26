/**
 * @file world-probabilistic.cc
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

#include "h2sl/common.h"
#include "h2sl/world-probabilistic.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

namespace h2sl {

//
// WorldProbabilistic class constructor
//
WorldProbabilistic::WorldProbabilistic( const mapProbObjects& objectsArg ) : objects( objectsArg ){}

//
// WorldProbabilistic XML constructor
//

WorldProbabilistic::WorldProbabilistic( const tinyxml2::XMLElement* root ) : objects(){
  if( not from_xml( root ) ){
    std::cout << "Class XML constructor failed. Line:  " << __LINE__ << " File:  " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }
}

//
// Imports the WorldProbabilistic class from an XML file
//
bool WorldProbabilistic::from_xml( const std::string& filename ){

  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename.c_str() );

  // Check for loading errors
  if( doc.Error() ){
    std::cout << "xml document had loading errors in WorldProbabilistic::from_xml( " << filename << ")" << std::endl;
    return false;
  }

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr ){
    std::cout << "No root element found in WorldProbabilistic::from_xml( " << filename << ")" << std::endl;
    return false;
  }

  // Check for any world child element of the root
  const tinyxml2::XMLElement* world_elem = root->FirstChildElement("world_probabilistic");
  if( world_elem == nullptr ){
    std::cout << "No world element found in WorldProbabilistic::from_xml( " << filename << ")" << std::endl;
    return false;
  }

  return from_xml( world_elem );
}

//
// Imports the WorldProbabilistic class from an tinyxml2::XMLElement pointer
//
bool WorldProbabilistic::from_xml( const tinyxml2::XMLElement* world_elem ){

  // Check that the element is a world element
  if( world_elem == nullptr )
    return false;
  if( std::strcmp(world_elem->Name(), "world_probabilistic") != 0 )
    return false;

  objects.clear();

  // Read objects
  auto object_elem = world_elem->FirstChildElement( "object_probabilistic" );
  while( object_elem != nullptr ){
    ObjectProbabilistic object( object_elem );
    objects.emplace( object.uid, object );
    object_elem = object_elem->NextSiblingElement( "object_probabilistic" );
  }

  return true;
}

//
// Method to load from a json formatted std::string
//
bool WorldProbabilistic::from_json( const std::string& json_string ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_string, root );
  return from_json( root );
}

//
// Method to load from a json value
//
bool WorldProbabilistic::from_json( const Json::Value& root ){

  if(root["class"].asString() != "world_probabilistic")
    return false;

  objects.clear();

  if(root.isMember("objects")){
    Json::Value objects_array = root["objects"];
    for( const auto& object_element : objects_array ){
      h2sl::ObjectProbabilistic object = h2sl::ObjectProbabilistic( "na", h2sl::ObjectState::vectorObjectState() );
      object.from_json( object_element );
      objects.emplace( object.uid, object );
    }
  }

  return true;
}

//
// Exports the WorldProbabilistic class to an XML file
//
bool WorldProbabilistic::to_xml(const std::string& filename, const int object_states_count) const {

  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertEndChild( root );
  to_xml( doc, root, object_states_count );

  // Save the file and store the error status
  tinyxml2::XMLError error_result = doc.SaveFile( filename.c_str() );
  if( error_result != tinyxml2::XML_SUCCESS )
    return false;

  return true;
}

//
// Exports the WorldProbabilistic class to an XML node pointer
//
void WorldProbabilistic::to_xml( tinyxml2::XMLDocument& document,
                    tinyxml2::XMLElement* root,
                    const int object_states_count ) const {

  auto* world = document.NewElement( "world_probabilistic" );

  // write objects
  for( const auto & object : objects ){
    object.second.to_xml( document, world, object_states_count );
  }

  root->InsertEndChild( world );
  return;
}

//
// Method to write to a json formatted std::string
//
std::string WorldProbabilistic::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

//
// Method to write to a json value
//
void WorldProbabilistic::to_json( Json::Value& root )const{

  root["class"] = "world_probabilistic";

  root["objects"] = Json::Value( Json::arrayValue ); 

  for( const auto& object : objects ){
    Json::Value object_root;
    object.second.to_json( object_root );
    root["objects"].append( object_root );
  }

  return;
}


//
// WorldProbabilistic class ostream operator
//
std::ostream& operator<<(std::ostream& out, const WorldProbabilistic& other) {
  out << "[" << other.objects.size() << "]:{";
  for (auto it_object = other.objects.begin();
       it_object != other.objects.end(); it_object++) {
    out << it_object->second;
    if( next( it_object ) != other.objects.end() ){
      out << ",";
    }
  }
  out << "}";
  return out;
}

}  // namespace h2sl
