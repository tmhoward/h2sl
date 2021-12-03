/**
 * @file object.cc
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
#include <stdlib.h>
#include <time.h>

#include "h2sl/object.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

using namespace std;

namespace h2sl{

//
// Default Object class constructor.
//
Object::Object( const std::string& uidArg,
  const ObjectState::vectorObjectState& stateHistoryArg,
  const mapProperties& propertiesArg ) : uid( uidArg ),
	state_history( stateHistoryArg ), properties( propertiesArg ){}

//
// Object class constructor from a static state.
//
Object::Object( const std::string& uidArg, const ObjectState& stateArg,
  const mapProperties& propertiesArg ) : uid( uidArg ), state_history(),
	properties( propertiesArg ){
  state_history.push_back( stateArg );
}

//
// Object class constructor from an ObjectProbabilistic.
//
Object::Object( const ObjectProbabilistic& objectArg ) : uid( objectArg.uid ),
    state_history( objectArg.state_history ), properties() {

  // Sample object properties based on objectArg's property distributions.
  double r, sum;
  for( const auto& prop_dist : objectArg.property_distributions ){
    r = ( rand() % 10000 ) / 10000.0; // Sample random number between 0 and 1
    sum = 0.0;

    // Find the first property which makes the sum pass the random sample.
    for( const auto& prop : prop_dist.second ){
      sum += prop.second; // Add this property's probability
      if( sum > r ){
	// Add this property to the Object's properties
        properties.emplace( prop_dist.first, prop.first );
	break; // Skip the rest of this distribution's properties
      }
    } // property for
  } // property_distributions for
}

//
// Object XML constructor
//
Object::Object( const tinyxml2::XMLElement *root ){
  if( !from_xml( root ) ){
    std::cout << "XML constructor failed. Line: " << __LINE__ << " in " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }
}

//
// Class constructor for Object from ObjectMessage message.
//
Object::Object( const ObjectMessage& msg ) : uid( msg.uid ), state_history(), properties(){
  // Import all ObjectStateMessages into state history
  for( const auto& state : msg.state_history ){
    state_history.emplace_back( state );
  }

  // Import all PropertyMessages into properties
  for( const auto& property : msg.properties ){
    properties.emplace( property.name, property.value );
  }
}

//
// Imports the Object class from an XML file
//
bool Object::from_xml( const string& filename ){

  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename.c_str() );

  // Check for loading errors
  if( doc.Error() )
    return false;

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr )
    return false;

  // Check for any Object child element of the root
  const tinyxml2::XMLElement* object_elem = root->FirstChildElement("object");
  if( object_elem == nullptr )
    return false;

  return from_xml( object_elem );
}

//
// Imports the Object class from an tinyxml2::XMLElement pointer
//
bool Object::from_xml(const tinyxml2::XMLElement * object_elem) {

  uid = "";
  state_history.clear();
  properties.clear();

  // Check that the element is a object element
  if( object_elem == nullptr )
    return false;

  if( std::strcmp(object_elem->Name(), "object") != 0 )
    return false;

  // Read the attributes
  const auto* uid_attribute = object_elem->FindAttribute("uid");

  // Signal false if uid attribute is missing
  if(uid_attribute == nullptr){
    std::cout << "failed to find uid" << std::endl;
    return false;
  }
  uid = uid_attribute->Value();

  // Loop over attributes of the xml_element to fill in properties
  auto object_property_attribute = object_elem->FirstAttribute();
  while( object_property_attribute != nullptr ){

    // Handle special "xyz" attribute which gives a static position
    if( strcmp(object_property_attribute->Name(), "xyz") == 0 ){
      // Convert attribute into a position
      if( state_history.empty() ){ // Create new ObjectState
        h2sl::ObjectState object_state = ObjectState();
        object_state.pose = std::make_shared<h2sl::Transform3D>();
        object_state.pose->position.from_cs_string( object_property_attribute->Value() );

        // Save the position-only object state to our state history
        state_history.push_back( object_state );
      } else{ // Add this position to the back of the state history
        state_history.back().pose->position.from_cs_string( object_property_attribute->Value() );
      }
    } else if( strcmp(object_property_attribute->Name(), "rpy") == 0 ){
      // Convert attribute into an orientation
      std::vector<double> rpy = double_vector_from_std_string( object_property_attribute->Value() );
      if( rpy.size() == 3 ){
        if( state_history.empty() ){ // Create new ObjectState
          h2sl::ObjectState object_state = ObjectState();
          object_state.pose = std::make_shared<h2sl::Transform3D>();
          object_state.pose->orientation = h2sl::UnitQuaternion( rpy[0], rpy[1], rpy[2] );

          // Save the orientation-only object state to our state history
          state_history.push_back( object_state );
        } else{ // Add this orientation to the back of the state history
          state_history.back().pose->orientation = h2sl::UnitQuaternion( rpy[0], rpy[1], rpy[2] );
        }
      }
    } else if( strcmp(object_property_attribute->Name(), "uid") != 0 ){
      properties.emplace( object_property_attribute->Name(), object_property_attribute->Value() );
    }
    object_property_attribute = object_property_attribute->Next();
  }

  // Read state_history
  // The state history should have one or more entries based on the task requirements.
  auto state_history_element = object_elem->FirstChildElement("state_history");
  if( state_history_element != nullptr ){

    auto object_state_element = state_history_element->FirstChildElement("object_state");
    while( object_state_element != nullptr ){

      h2sl::ObjectState object_state = h2sl::ObjectState( object_state_element );
      state_history.push_back( object_state );

      object_state_element = object_state_element->NextSiblingElement("object_state");
    }
  }

  return true;
}

//
// Method to load from a json formatted string
//
bool Object::from_json( const std::string& json_string ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_string, root );
  return from_json( root );
}

//
// Method to load from a json value
//
bool Object::from_json( const Json::Value& root ){

  if(root["class"].asString() != "object")
    return false;

  uid = "";
  state_history.clear();
  properties.clear();

  if( root.isMember("uid") ) {
    uid = root["uid"].asString();
  }

  // Read state_history
  if(root.isMember("state_history")){
    Json::Value state_history_array = root["state_history"];
    for( const auto& state_history_element : state_history_array ){
      h2sl::ObjectState object_state = h2sl::ObjectState();
      object_state.from_json( state_history_element );
      state_history.push_back(object_state);
    }
  }

  // Read properties
  for( auto it = root["properties"].begin(); it != root["properties"].end(); ++it ){
    properties.emplace( it.key().asString(), it->asString() );
  }

  return true;
}

//
// Exports the Object class to an XML file
//
bool Object::to_xml(const string& filename,
                    const int object_states_count) const {

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
// Exports the Object class to an XML node pointer
//
void Object::to_xml(tinyxml2::XMLDocument& document,
                    tinyxml2::XMLElement *root,
                    const int object_states_count) const {

  auto object_elem = document.NewElement("object");

  // Write uid
  object_elem->SetAttribute("uid", uid.c_str());

  // Write the state_history
  auto state_history_elem = document.NewElement("state_history");
  // Write ObjectState from state_history as a child to the state_history node
  // object_states_count indicates how many latest object states to be exported
  int counter = 0;
  for( auto object_state = state_history.rbegin(); object_state != state_history.rend(); ++object_state){
    if(counter >= object_states_count)
      break;
    object_state->to_xml( document, state_history_elem );
    counter++;
  }
  object_elem->InsertEndChild(state_history_elem);

  // Write semantic properties
  for( const auto& property : properties ){
    object_elem->SetAttribute( property.first.c_str(), property.second.c_str() );
  }

  root->InsertEndChild( object_elem );
  return;
}

//
// Method to write to a json formatted string
//
std::string Object::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

//
// Method to write to a json value
//
void Object::to_json( Json::Value& root )const{

  root["class"] = "object";
  root["uid"] = uid;

  // Write the state_history
  root["state_history"] = Json::Value( Json::arrayValue );
  for( const auto& object_state : state_history ){
    Json::Value state_history_root;
    object_state.to_json( state_history_root );
    root["state_history"].append( state_history_root );
  }

  // Write the semantic properties
  for( const auto& property : properties ){
    root["properties"][property.first] = property.second;
  }

  return;
}

//
// This method exports an Object to an ObjectMessage.
//
ObjectMessage Object::to_msg( void )const{
  ObjectMessage msg;
  msg.uid = uid;

  // Export each ObjectState in the state history
  for( const auto& object_state : state_history ){
    msg.state_history.push_back( object_state.to_msg() );
  }

  // Export each property in the properties map
  for( const auto& property : properties ){
    PropertyMessage p;
    p.name = property.first;
    p.value = property.second;
    msg.properties.push_back( p );
  }

  return msg;
}

//
// Object class ostream operator
//
ostream& operator<<(ostream& out, const Object& other) {
  out << "\nObject(uid=\"" << other.uid << "\",";
  for (auto it_property = other.properties.begin();
       it_property != other.properties.end(); it_property++) {
    out << it_property->first << "=\"" << it_property->second << "\"";
    if (next(it_property) != other.properties.end()) {
      out << ",";
    }
  }

  out << ",state_history{";
  for( auto it_object_state = other.state_history.begin();
       it_object_state != other.state_history.end(); it_object_state++ ){
    out << *it_object_state;
    if( next(it_object_state) != other.state_history.end() ){
      out << ",";
    }
  }
  out << "})";
  return out;
}

} // namespace h2sl
