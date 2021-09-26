/**
 * @file objectstate.cc
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

#include "h2sl/common.h"
#include "h2sl/objectstate.h"

using namespace std;

namespace h2sl{

//
// ObjectState class default constructor
//
ObjectState::ObjectState( std::shared_ptr<h2sl::Transform3D> poseArg,
                          std::shared_ptr<h2sl::BoundingBox> boundingBoxArg,
                          std::shared_ptr<h2sl::Vector3D> vArg,
                          std::shared_ptr<h2sl::Vector3D> wArg,
                          std::shared_ptr<double> timeArg ) : pose( poseArg ),
                                                                   bbox( boundingBoxArg ),
                                                                   v( vArg ),
                                                                   w( wArg ),
                                                                   t( timeArg ){}


//
// ObjectState XML constructor
//
ObjectState::ObjectState( const tinyxml2::XMLElement *root ){
  if( not from_xml( root ) ){
    std::cout << "Class XML constructor failed. Line:  " << __LINE__ << " File:  " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }
}

//
// Class constructor for ObjectState from ObjectStateMessage message.
//
ObjectState::ObjectState( const ObjectStateMessage& msg ){
  if( msg.pose_exists ){
    pose = std::make_shared<h2sl::Transform3D>( msg.pose );
  }

  if( msg.bbox_exists ){
    bbox = std::make_shared<h2sl::BoundingBox>( msg.bbox );
  }

  if( msg.v_exists ){
    v = std::make_shared<h2sl::Vector3D>( msg.v );
  }

  if( msg.w_exists ){
    w = std::make_shared<h2sl::Vector3D>( msg.w );
  }

  if( msg.t_exists ){
    t = std::make_shared<double>( msg.t );
  }
}

//
// Imports the ObjectState class from an XML file
//
bool ObjectState::from_xml(const string& filename) {

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

  // Check for any object_state child element of the root
  const tinyxml2::XMLElement* object_state_element = root->FirstChildElement("object_state");
  if( object_state_element == nullptr )
    return false;

  return from_xml( object_state_element );
}

//
// Imports the ObjectState class from an tinyxml2::XMLElement pointer
//
bool ObjectState::from_xml(const tinyxml2::XMLElement *object_state_element){

  if (object_state_element != nullptr) {

    if( std::strcmp(object_state_element->Name(), "object_state") != 0 )
      return false;

    // reset() method sets these pointers to NULL
    pose.reset();
    bbox.reset();
    v.reset();
    w.reset();
    t.reset();

    // read attributes
    auto attribute = object_state_element->FirstAttribute();
    while ( attribute != nullptr ){
      if (strcmp(attribute->Name(), "xyz") == 0) {
        if( pose == nullptr )
          pose = std::make_shared<h2sl::Transform3D>();
        pose->position.from_cs_string( attribute->Value() );
      } else if (strcmp(attribute->Name(), "q") == 0) {
        if( pose == nullptr )
          pose = std::make_shared<h2sl::Transform3D>();
        pose->orientation.from_cs_string( attribute->Value() );
      } else if (strcmp(attribute->Name(), "bdim") == 0) {
        if( bbox == nullptr )
          bbox = std::make_shared<h2sl::BoundingBox>();
        bbox->dimensions.from_cs_string( attribute->Value() );
      } else if (strcmp(attribute->Name(), "bxyz") == 0) {
        if( bbox == nullptr )
          bbox = std::make_shared<h2sl::BoundingBox>();
        bbox->offset.position.from_cs_string( attribute->Value() );
      } else if (strcmp(attribute->Name(), "bq") == 0) {
        if( bbox == nullptr )
          bbox = std::make_shared<h2sl::BoundingBox>();
        bbox->offset.orientation.from_cs_string( attribute->Value() );
      } else if (strcmp(attribute->Name(), "v") == 0) {
        v = std::make_shared<h2sl::Vector3D>();
        v->from_cs_string( attribute->Value() );
      } else if (strcmp(attribute->Name(), "w") == 0) {
        w = std::make_shared<h2sl::Vector3D>();
        w->from_cs_string( attribute->Value() );
      } else if (strcmp(attribute->Name(), "t") == 0) {
        t = std::make_shared<double>( std::stod(attribute->Value()) );
      }
      attribute = attribute->Next(); 
    }
    return true;
  } else {
    return false;
  }
}

//
// Method to load from a json formatted string
//
bool ObjectState::from_json( const std::string& json_string ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_string, root );
  return from_json( root );
}

//
// Method to load from a json value
//
bool ObjectState::from_json( const Json::Value& root ){

  if(root["class"].asString() != "object_state")
    return false;

  // reset() method sets these pointers to NULL
  pose.reset();
  bbox.reset();
  v.reset();
  w.reset();
  t.reset();

  // Fill in the available properties
  // If a property is unavailable then its set to a null pointer
  if (root.isMember("xyz") ) {
    if( pose == nullptr )
      pose = std::make_shared<h2sl::Transform3D>();
    pose->position.from_cs_string( root["xyz"].asString() );
  } 
  
  if (root.isMember("q")) {
    if( pose == nullptr )
      pose = std::make_shared<h2sl::Transform3D>();
    pose->orientation.from_cs_string( root["q"].asString() );
  } 
  
  if (root.isMember("bdim")) {
    if( bbox == nullptr )
      bbox = std::make_shared<h2sl::BoundingBox>();
    bbox->dimensions.from_cs_string( root["bdim"].asString() );
  } 
  
  if (root.isMember("bxyz")) {
    if( bbox == nullptr )
      bbox = std::make_shared<h2sl::BoundingBox>();
    bbox->offset.position.from_cs_string( root["bxyz"].asString() );
  } 
  
  if (root.isMember("bq")) {
    if( bbox == nullptr )
      bbox = std::make_shared<h2sl::BoundingBox>();
    bbox->offset.orientation.from_cs_string( root["bq"].asString() );
  } 
  
  if (root.isMember("v") ) {
    v = std::make_shared<h2sl::Vector3D>();
    v->from_cs_string( root["v"].asString() );
  } 
  
  if (root.isMember("w")) {
    w = std::make_shared<h2sl::Vector3D>();
    w->from_cs_string( root["w"].asString() );
  } 
  
  if (root.isMember("t")) {
    t = std::make_shared<double>( std::stod(root["t"].asString()) );
  }

  return true;
}

//
// Exports the ObjectState class to an XML file
//
bool ObjectState::to_xml(const string& filename) const {

  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertEndChild( root );
  to_xml( doc, root );

  // Save the file and store the error status
  tinyxml2::XMLError error_result = doc.SaveFile( filename.c_str() );
  if( error_result != tinyxml2::XML_SUCCESS )
    return false;

  return true;

}

//
// Exports the ObjectState class to an XML node pointer
//
void ObjectState::to_xml(tinyxml2::XMLDocument& document,
                         tinyxml2::XMLElement *root) const {

  tinyxml2::XMLElement* object_state_element = document.NewElement("object_state");

  // write pose available
  if(pose != nullptr){
    object_state_element->SetAttribute( "xyz", pose->position.to_csv_string().c_str() );
    object_state_element->SetAttribute( "q", pose->orientation.to_csv_string().c_str() );
  }

  // write bounding box
  if(bbox != nullptr){
    object_state_element->SetAttribute( "bdim", bbox->dimensions.to_csv_string().c_str() );
    object_state_element->SetAttribute( "bxyz", bbox->offset.position.to_csv_string().c_str() );
    object_state_element->SetAttribute( "bq", bbox->offset.orientation.to_csv_string().c_str() );
  }

  // write v available
  if(v != nullptr)
    object_state_element->SetAttribute( "v", v->to_csv_string().c_str() );
  // write w available
  if(w != nullptr)
    object_state_element->SetAttribute( "w", w->to_csv_string().c_str() );
  // write time available
  if(t != nullptr)
    object_state_element->SetAttribute( "t", std::to_string(*t).c_str() );

  // Add the constructed element to the root
  root->InsertEndChild(object_state_element);

  return;
}

//
// Method to write to a json formatted string
//
std::string ObjectState::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

//
// Method to write to a json value
//
void ObjectState::to_json( Json::Value& root )const{

  root["class"] = "object_state";

  // write pose available
  if(pose != nullptr){
    root["xyz"] = pose->position.to_csv_string();
    root["q"] = pose->orientation.to_csv_string();
  }

  // write bounding box
  if(bbox != nullptr){
    root["bdim"] = bbox->dimensions.to_csv_string();
    root["bxyz"] = bbox->offset.position.to_csv_string();
    root["bq"] = bbox->offset.orientation.to_csv_string();
  }

  // write v available
  if(v != nullptr)
    root["v"] = v->to_csv_string();
  // write w available
  if(w != nullptr)
    root["w"] = w->to_csv_string();
  // write time available
  if(t != nullptr)
    root["t"] = std::to_string(*t);

  return;
}

//
// This method exports an ObjectState to an ObjectStateMessage.
//
ObjectStateMessage ObjectState::to_msg( void )const{
  ObjectStateMessage msg;
  
  // Export pose if it exists
  if( pose != nullptr ){
    msg.pose = pose->to_msg();
    msg.pose_exists = 1;
  } else{
    msg.pose_exists = 0;
  }

  // Export bounding box if it exists
  if( bbox != nullptr ){
    msg.bbox = bbox->to_msg();
    msg.bbox_exists = 1;
  } else{
    msg.bbox_exists = 0;
  }

  // Export linear velocity if it exists
  if( v != nullptr ){
    msg.v = v->to_msg();
    msg.v_exists = 1;
  } else{
    msg.v_exists = 0;
  }

  // Export angular velocity if it exists
  if( w != nullptr ){
    msg.w = w->to_msg();
    msg.w_exists = 1;
  } else{
    msg.w_exists = 0;
  }

  // Export timestamp if it exists
  if( t != nullptr ){
    msg.t = *t;
    msg.t_exists = 1;
  } else{
    msg.t_exists = 0;
  }

  return msg;
}

//
// ObjectState class ostream operator
//
ostream& operator<<(ostream& out, const ObjectState& other) {
  
  out << "(";

  // print time if available
  if(other.t != nullptr){
    // out << std::setprecision(3) << "t=" << *other.t << ", ";  
    out << std::fixed;
    out << "t=" << *other.t << ", ";  
  }

  // print pose if available
  if(other.pose != nullptr){
    out << "position(xyz)={" << other.pose->position.x() << ", "
                             << other.pose->position.y() << ", "
                             << other.pose->position.z() << "}, ";

    out << "orientation(rpy)={" << other.pose->orientation.roll() * 180.0 / M_PI << ", "
                                << other.pose->orientation.pitch() * 180.0 / M_PI << ", "
                                << other.pose->orientation.yaw() * 180.0 / M_PI << "}, ";                       
  }

  // print collision geometry if available
  if(other.bbox != nullptr){
    out << "bbox_dim(xyz)={" << other.bbox->dimensions.x() << ", "
                             << other.bbox->dimensions.y() << ", "
                             << other.bbox->dimensions.z() << "}, ";
  }

  // print v if available
  if(other.v != nullptr){
    out << "v={" << other.v->x() << " " << other.v->y() << " " << other.v->z()
        << "}";
  }

  // print w if available
  if(other.w != nullptr){
    out << "w={" << other.w->x() << " " << other.w->y() << " " << other.w->z()
        << "},";
  }

  out << ")";
  return out;
}

} // namespace h2sl
