/**
 * @file transform3d.cc
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

#include "h2sl/common.h"
#include "h2sl/transform3d.h"

using namespace std;

namespace h2sl{

//
// Transform3D class parameterized constructor
//
Transform3D::Transform3D( const Vector3D& positionArg,
                          const UnitQuaternion& orientationArg ) : position(positionArg),
                                                                   orientation(orientationArg){}
//
// Transform3D XML constructor
//
Transform3D::Transform3D( const tinyxml2::XMLElement *root ){
  from_xml(root);
}

//
// Class constructor for Transform3D from Transform3DMessage message.
//
Transform3D::Transform3D( const Transform3DMessage& msg ) : position( msg.position ),
                                                            orientation( msg.orientation ){}

//
// Transform3D class multiplication operator
//
Transform3D Transform3D::operator*( const Transform3D& rhs )const{
  return Transform3D( position + orientation * rhs.position, orientation * rhs.orientation );
}

//
// Transform3D class self multiplication operator
//
void Transform3D::operator*=( const Transform3D& rhs ){
  position = position + orientation * rhs.position;
  orientation = orientation * rhs.orientation;
  return;
}

//
// Returns the inverse of this transform
//
Transform3D Transform3D::inverse( void )const{
  return Transform3D( orientation.conjugate() * position * -1.0, orientation.conjugate() );
}

//
// Returns the x element of the position vector
//
double Transform3D::x( void )const{
  return position.x();
}

//
// Returns the y element of the position vector
//
double Transform3D::y( void )const{
  return position.y();
}

//
// Returns the z element of the position vector
//
double Transform3D::z( void )const{
  return position.z();
}

//
// Imports the Transform3D class from an XML file
//
bool Transform3D::from_xml(const string &filename) {

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

  // Check for any transform3d child element of the root
  const tinyxml2::XMLElement* transform3d_element = root->FirstChildElement("transform3d");
  if( transform3d_element == nullptr )
    return false;

  return from_xml( transform3d_element );

}

//
// Imports the Transform3D class from an tinyxml2::XMLElement pointer
//
bool Transform3D::from_xml(const tinyxml2::XMLElement *transform3d_element) {
  if( transform3d_element == nullptr )
    return false;
  position = h2sl::Vector3D();
  orientation = h2sl::UnitQuaternion();
  return true;
}

//
// Exports the Transform3D class to an XML file
//
bool Transform3D::to_xml(const string &filename) const {

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
// exports the Transform3D class to an XML node pointer
//
void Transform3D::to_xml(tinyxml2::XMLDocument &document,
                         tinyxml2::XMLElement *root) const {

  tinyxml2::XMLElement* vector_element = document.NewElement("transform3d");

  // Add the constructed element to the root
  root->InsertEndChild(vector_element);

  return;
}

//
// This method exports a Transform3D to a Transform3DMessage.
//
Transform3DMessage Transform3D::to_msg( void )const{
  Transform3DMessage msg;
  msg.position = position.to_msg();
  msg.orientation = orientation.to_msg();
  return msg;
}

//
// Transform3D class ostream operator
//
ostream &operator<<(ostream &out, const Transform3D &other) {
  // out << "(Transform3D: ";
  out << "position= " << other.position << ",";
  out << " orientation= " << other.orientation;
  // out << ")";
  return out;
}

} // namespace h2sl
