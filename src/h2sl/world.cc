/**
 * @file world.cc
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
#include "h2sl/world.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

namespace h2sl {

/************************************************************************************/
/******************************** World class source ********************************/
/************************************************************************************/
//
// World class constructor
//
World::World( const mapObjects& objectsArg ) : objects( objectsArg ){}

//
// World XML constructor
//
World::World( const tinyxml2::XMLElement* root ) : objects(){
  if( not from_xml( root ) ){
    std::cout << "Class XML constructor failed. Line:  " << __LINE__ << " File:  " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }
}

//
// Class constructor for World from WorldProbabilistic.
//
World::World(const WorldProbabilistic& world) : objects(){
  // Sample an Object from each ObjectProbabilistic
  for( const auto & prob_object : world.objects ){
    Object sampled_object = Object( prob_object.second );
    objects.emplace( sampled_object.uid, sampled_object );
  }
}

//
// Class constructor for World from WorldMessage message.
//
World::World( const WorldMessage& msg ) : objects(){
  // Import each ObjectMessage from the message
  for( const auto& object_msg : msg.objects ){
    objects.emplace( object_msg.uid, Object( object_msg ) );
  }
}

//
// Imports the World class from an XML file
//
bool World::from_xml( const std::string& filename ){

  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename.c_str() );

  // Check for loading errors
  if( doc.Error() ){
    std::cout << "xml document had loading errors in World::from_xml( " << filename << ")" << std::endl;
    return false;
  }

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr ){
    std::cout << "No root element found in World::from_xml( " << filename << ")" << std::endl;
    return false;
  }

  // Check for any world child element of the root
  const tinyxml2::XMLElement* world_elem = root->FirstChildElement("world");
  if( world_elem == nullptr ){
    std::cout << "No world element found in World::from_xml( " << filename << ")" << std::endl;
    return false;
  }

  return from_xml( world_elem );
}

//
// Imports the World class from an tinyxml2::XMLElement pointer
//
bool World::from_xml( const tinyxml2::XMLElement* world_elem ){

  // Check that the element is a world element
  if( world_elem == nullptr )
    return false;
  if( std::strcmp(world_elem->Name(), "world") != 0 )
    return false;

  objects.clear();

  // read objects
  auto object_elem = world_elem->FirstChildElement( "object" );
  while( object_elem != nullptr ){
    Object object( object_elem );
    objects.insert( std::make_pair( object.uid, object ) );
    object_elem = object_elem->NextSiblingElement( "object" );
  }

  return true;
}

//
// Method to load from a json formatted std::string
//
bool World::from_json( const std::string& json_string ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_string, root );
  return from_json( root );
}

//
// Method to load from a json value
//
bool World::from_json( const Json::Value& root ){

  if(root["class"].asString() != "world")
    return false;

  objects.clear();

  if(root.isMember("objects")){
    Json::Value objects_array = root["objects"];
    for( const auto& object_element : objects_array ){
      h2sl::Object object = h2sl::Object( "na", h2sl::ObjectState::vectorObjectState() );
      object.from_json( object_element );
      objects.insert( std::make_pair( object.uid, object ) );
    }
  }

  return true;
}

//
// exports the World class to an XML file
//
bool World::to_xml(const std::string& filename, const int object_states_count) const {


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
// exports the World class to an XML node pointer
//
void World::to_xml( tinyxml2::XMLDocument& document,
                    tinyxml2::XMLElement* root,
                    const int object_states_count ) const {

  auto* world = document.NewElement( "world" );

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
std::string World::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

//
// Method to write to a json value
//
void World::to_json( Json::Value& root )const{

  root["class"] = "world";

  root["objects"] = Json::Value( Json::arrayValue );

  for( const auto& object : objects ){
    Json::Value object_root;
    object.second.to_json( object_root );
    root["objects"].append( object_root );
  }

  return;
}

//
// This method exports a World to a WorldMessage.
//
WorldMessage World::to_msg( void )const{
  WorldMessage msg;

  // Export each object stored in the World
  for( const auto& uid_object_pair : objects ){
    msg.objects.push_back( uid_object_pair.second.to_msg() );
  }

  return msg;
}

//
// World class ostream operator
//
std::ostream& operator<<(std::ostream& out, const World& other) {
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

///
/// Method to return a vector of sorted objects per a SortKey
///
const std::map< std::string, std::vector< std::shared_ptr<Object> > >
World::sort_objects( const SortKey sort_key )const{
  // Initialize the return object
  std::vector< std::shared_ptr<Object> > sorted_objects;
  // Populate sorted_objects with the objects from the world
  for( const auto& kv_object : objects ){
    sorted_objects.push_back( std::make_shared<Object>( kv_object.second ) );
  }
  // Sort the objects according to the key
  switch( sort_key ){
    case SortKey::MIN_X_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_min_x_axis);
      break;
    case SortKey::MAX_X_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_max_x_axis);
      break;
    case SortKey::MIN_ABS_X_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_min_abs_x_axis);
      break;
    case SortKey::MAX_ABS_X_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_max_abs_x_axis);
      break;
    case SortKey::MIN_Y_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_min_y_axis);
      break;
    case SortKey::MAX_Y_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_max_y_axis);
      break;
    case SortKey::MIN_ABS_Y_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_min_abs_y_axis);
      break;
    case SortKey::MAX_ABS_Y_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_max_abs_y_axis);
      break;
    case SortKey::MIN_Z_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_min_z_axis);
      break;
    case SortKey::MAX_Z_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_max_z_axis);
      break;
    case SortKey::MIN_ABS_Z_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_min_abs_z_axis);
      break;
    case SortKey::MAX_ABS_Z_AXIS:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_max_abs_z_axis);
      break;
    case SortKey::MIN_EUCLIDEAN_DISTANCE:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_min_euclidean_distance);
      break;
    case SortKey::MAX_EUCLIDEAN_DISTANCE:
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_max_euclidean_distance);
      break;
    case SortKey::MIN_CENTER_DISTANCE:
    {
      // Create a new origin position at the center of the objects
      h2sl::Transform3D object_center;
      for( const auto& ptr_object : sorted_objects ){
        object_center.position = object_center.position + ptr_object->state_history.back().pose->position;
      }
      object_center.position = object_center.position / (double) sorted_objects.size();
      // Transform the objects to have the center_position at the origin
      for( auto& ptr_object : sorted_objects ){
        *ptr_object->state_history.back().pose =
                      object_center.inverse() * (*ptr_object->state_history.back().pose);
      }
      // Sort the objects via min euclidean distance given the new origin
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_min_euclidean_distance);
      // Transform the objects back to the initial origin
      for( auto& ptr_object : sorted_objects ){
        *ptr_object->state_history.back().pose =
                      object_center * (*ptr_object->state_history.back().pose);
      }
      break;
    }
    case SortKey::MAX_CENTER_DISTANCE:
    {
      // Create a new origin position at the center of the objects
      h2sl::Transform3D object_center;
      for( const auto& ptr_object : sorted_objects ){
        object_center.position = object_center.position + ptr_object->state_history.back().pose->position;
      }
      object_center.position = object_center.position / (double) sorted_objects.size();
      // Transform the objects to have the center_position at the origin
      for( auto& ptr_object : sorted_objects ){
        *ptr_object->state_history.back().pose =
                      object_center.inverse() * (*ptr_object->state_history.back().pose);
      }
      // Sort the objects via min euclidean distance given the new origin
      std::sort(sorted_objects.begin(), sorted_objects.end(), sort_max_euclidean_distance);
      // Transform the objects back to the initial origin
      for( auto& ptr_object : sorted_objects ){
        *ptr_object->state_history.back().pose =
                      object_center * (*ptr_object->state_history.back().pose);
      }
      break;
    }
    default:
      std::stringstream error_msg;
      error_msg << "Passed sort_key did not match a SortKey type, not sorting.";
      throw std::runtime_error( error_msg.str() );
      break;
  }

  // Convert the sorted vector into a map of sorted vectors categorized based on the object types
  std::map< std::string, std::vector< std::shared_ptr<Object> > > sorted_objects_classified_by_object_type;
  for( const auto& object : sorted_objects ){
    // Retrieve the object_type of the object
    std::string object_type = object->properties["object_type"];
    // Add an entry to the map based on the object_type
    if( sorted_objects_classified_by_object_type.find(object_type) == sorted_objects_classified_by_object_type.end() ){
      sorted_objects_classified_by_object_type.insert( std::make_pair(object_type, std::vector<std::shared_ptr<Object>>()) );
    }
    // Insert the sorted object according to the object_type
    sorted_objects_classified_by_object_type[object_type].push_back(object);
  }
  // Insert the original sorted_objects vector with a key named "all"
  sorted_objects_classified_by_object_type["na"] = sorted_objects;

  return sorted_objects_classified_by_object_type;
}


/***************************************************************************************/
/******************************** WorldDCG class source ********************************/
/***************************************************************************************/
///
/// WorldDCG constructor from a base World - Copies the base members then fills the sorted_objects members
///
WorldDCG::WorldDCG( const World& base_world ) : World( base_world ){

  if( !base_world.objects.empty() ){
    // Sort the objects
    _fill_sorted_objects();
  }

  return;
}

///
/// Method to fill the _sorted_world_objects member
///
void WorldDCG::_fill_sorted_objects( void ){
  // Clear any stored sorted world information
  _sorted_objects.clear();

  // Emplace a new sorted objects vector for each World::SortKey
  _sorted_objects.emplace( World::SortKey::MIN_X_AXIS, sort_objects( World::SortKey::MIN_X_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MAX_X_AXIS, sort_objects( World::SortKey::MAX_X_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MIN_ABS_X_AXIS, sort_objects( World::SortKey::MIN_ABS_X_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MAX_ABS_X_AXIS, sort_objects( World::SortKey::MAX_ABS_X_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MIN_Y_AXIS, sort_objects( World::SortKey::MIN_Y_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MAX_Y_AXIS, sort_objects( World::SortKey::MAX_Y_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MIN_ABS_Y_AXIS, sort_objects( World::SortKey::MIN_ABS_Y_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MAX_ABS_Y_AXIS, sort_objects( World::SortKey::MAX_ABS_Y_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MIN_Z_AXIS, sort_objects( World::SortKey::MIN_Z_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MAX_Z_AXIS, sort_objects( World::SortKey::MAX_Z_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MIN_ABS_Z_AXIS, sort_objects( World::SortKey::MIN_ABS_Z_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MAX_ABS_Z_AXIS, sort_objects( World::SortKey::MAX_ABS_Z_AXIS ) );
  _sorted_objects.emplace( World::SortKey::MIN_EUCLIDEAN_DISTANCE, sort_objects( World::SortKey::MIN_EUCLIDEAN_DISTANCE ) );
  _sorted_objects.emplace( World::SortKey::MAX_EUCLIDEAN_DISTANCE, sort_objects( World::SortKey::MAX_EUCLIDEAN_DISTANCE ) );
  _sorted_objects.emplace( World::SortKey::MIN_CENTER_DISTANCE, sort_objects( World::SortKey::MIN_CENTER_DISTANCE ) );
  _sorted_objects.emplace( World::SortKey::MAX_CENTER_DISTANCE, sort_objects( World::SortKey::MAX_CENTER_DISTANCE ) );
  return;
}

}  // namespace h2sl
