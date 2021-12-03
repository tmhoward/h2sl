/**
 * @file unit-quaternion.cc
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
#include "h2sl/unit-quaternion.h"

using namespace std;

namespace h2sl{

//
// UnitQuaternion class default constructor
//
UnitQuaternion::UnitQuaternion( const h2sl::Vector3D& qvArg,
                                const double& qsArg) : qv( qvArg ),
                                                       qs( qsArg ){
  _normalize();
}

//
// UnitQuaternion class roll, pitch, yaw constructor
//
UnitQuaternion::UnitQuaternion( const double& roll, const double& pitch, const double& yaw ){

  qs = cos( roll / 2.0 ) * cos( pitch / 2.0 ) * cos( yaw / 2.0 ) + sin( roll / 2.0 ) * sin ( pitch / 2.0 ) * sin( yaw / 2.0 );
  qv[0] = sin( roll / 2.0 ) * cos( pitch / 2.0 ) * cos( yaw / 2.0 ) - cos( roll / 2.0 ) * sin( pitch / 2.0 ) * sin( yaw / 2.0 );
  qv[1] = cos( roll / 2.0 ) * sin( pitch / 2.0 ) * cos( yaw / 2.0 ) + sin( roll / 2.0 ) * cos( pitch / 2.0 ) * sin( yaw / 2.0 );
  qv[2] = cos( roll / 2.0 ) * cos( pitch / 2.0 ) * sin( yaw / 2.0 ) - sin( roll / 2.0 ) * sin( pitch / 2.0 ) * cos( yaw / 2.0 );

  _normalize();

}

//
// UnitQuaternion XML constructor
//
UnitQuaternion::UnitQuaternion( const tinyxml2::XMLElement *root ){
  from_xml(root);
}

//
// Class constructor for UnitQuaternion from UnitQuaternionMessage message.
//
UnitQuaternion::UnitQuaternion( const UnitQuaternionMessage& msg ){
  qs = msg.s;
  qv[0] = msg.x;
  qv[1] = msg.y;
  qv[2] = msg.z;
  _normalize();
}

//
// UnitQuaternion class scalar multiplication operator
//
UnitQuaternion UnitQuaternion::operator*( const double& rhs )const{
  return UnitQuaternion( qv * rhs, qs * rhs );
}

//
// UnitQuaternion class vector multiplication operator
//
Vector3D
UnitQuaternion::
operator*( const Vector3D& rhs )const{
  Vector3D vec1 = qv * 2.0;
  Vector3D vec2 = qv.cross(rhs) + (rhs * qs);
  return rhs + vec1.cross(vec2);
}

//
// UnitQuaternion class unitquaternion multiplication operator
//
UnitQuaternion UnitQuaternion::operator*( const UnitQuaternion& rhs )const{
  double tmp_scalar = (qs * rhs.qs) - qv.dot(rhs.qv);
  h2sl::Vector3D tmp_vector = (rhs.qv * qs) + (qv * rhs.qs) + qv.cross(rhs.qv);
  return UnitQuaternion( tmp_vector, tmp_scalar );
}

//
// UnitQuaternion class unitquaternion self multiplication operator
//
void UnitQuaternion::operator*=( const UnitQuaternion& rhs ){
  (*this) = (*this) * rhs;
  return;
}

//
// UnitQuaternion class scalar self multiplication operator
//
void UnitQuaternion::operator*=( const double& rhs ){
  qv = qv * rhs;
  qs = qs * rhs;
  _normalize();
  return;
}

//
// Normalizes the quaternion
//
void UnitQuaternion::_normalize( void ){

  double norm = sqrt( qv[0] * qv[0] + qv[1] * qv[1] + qv[2] * qv[2] + qs * qs );

  qv = qv / norm;
  qs = qs / norm;
  if( qs < 0.0 ){
    qs = qs * -1.0;
    qv = qv * -1.0;
  }
  return;
}

//
// Method to return the roll of the UnitQuaternion
//
double UnitQuaternion::roll( void ) const {
  const double y = 2.0 * (qs * qv[0] + qv[1] * qv[2]);
  const double x = 1.0 - 2.0 * (qv[0] * qv[0] + qv[1] * qv[1]);
  return std::atan2(y, x);
}

//
// Method to return the pitch of the UnitQuaternion
//
double UnitQuaternion::pitch( void ) const {
  return std::min(std::max( 2.0 * (qs * qv[1] - qv[2] * qv[0]) , -1.0), 1.0);
}

//
// Method to return the yaw of the UnitQuaternion
//
double UnitQuaternion::yaw( void ) const {
  const double y = 2.0 * (qs * qv[2] + qv[0] * qv[1]);
  const double x = 1.0 - 2.0 * (qv[1] * qv[1] + qv[2] * qv[2]);
  return std::atan2(y, x);
}

//
// Returns a conjugate of this quaternion
//
UnitQuaternion UnitQuaternion::conjugate( void )const{
  return UnitQuaternion( qv * -1.0, qs );
}

//
// Import the UnitQuaternion class from a comma separated value string
//
bool UnitQuaternion::from_cs_string( const std::string &csv_string ){
  std::vector< std::string > tmp_strings;
  boost::split( tmp_strings, csv_string, boost::is_any_of( "," ) );

  if( tmp_strings.size() != 4 )
    return false;

  // make sure the order is correct
  qv[0] = std::stod( tmp_strings[ 0 ] );
  qv[1] = std::stod( tmp_strings[ 1 ] );
  qv[2] = std::stod( tmp_strings[ 2 ] );
  qs = std::stod( tmp_strings[ 3 ] );

  _normalize();

  return true;

}

//
// Imports the UnitQuaternion class from an XML file
//
bool UnitQuaternion::from_xml(const string &filename) {

  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename.c_str() );

  // Check for loading errors
  if( doc.Error() ){
    std::cout << "Document error in UnitQuaternion::from_xml()" << std::endl;
    return false;
  }

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr ){
    std::cout << "Root element was nullptr" << std::endl;
    return false;
  }

  // Check for any unit_quaternion child element of the root
  const tinyxml2::XMLElement* unit_quaternion_element = root->FirstChildElement("unit_quaternion");
  if( unit_quaternion_element == nullptr ){
    std::cout << "No unit_quaternion element found in from_xml" << std::endl;
    return false;
  }

  return from_xml( unit_quaternion_element );

}

//
// Imports the UnitQuaternion class from an tinyxml2::XMLElement pointer
//
bool UnitQuaternion::from_xml(const tinyxml2::XMLElement *unit_quaternion_element) {
  if (unit_quaternion_element == nullptr)
    return false;

  qv = h2sl::Vector3D();
  qs = 1.0;

  // Read the attributes
  const auto* x_attr = unit_quaternion_element->FindAttribute("x");
  const auto* y_attr = unit_quaternion_element->FindAttribute("y");
  const auto* z_attr = unit_quaternion_element->FindAttribute("z");
  const auto* w_attr = unit_quaternion_element->FindAttribute("w");

  // Signal false if x attribute is missing
  if(x_attr == nullptr){
    std::cout << "failed to find the x attribute" << std::endl;
    return false;
  }

  // Signal false if y attribute is missing
  if(y_attr == nullptr){
    std::cout << "failed to find the y attribute" << std::endl;
    return false;
  }

  // Signal false if z attribute is missing
  if(z_attr == nullptr){
    std::cout << "failed to find the z attribute" << std::endl;
    return false;
  }

  // Signal false if z attribute is missing
  if(w_attr == nullptr){
    std::cout << "failed to find the w attribute" << std::endl;
    return false;
  }

  qv.data[0] = std::stod(x_attr->Value());
  qv.data[1] = std::stod(y_attr->Value());
  qv.data[2] = std::stod(z_attr->Value());
  qs = std::stod(w_attr->Value());

  _normalize();

  return true;
}

//
// Exports the UnitQuaternion class to an XML file
//
bool UnitQuaternion::to_xml(const string &filename) const {

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
// exports the UnitQuaternion class to an XML node pointer
//
void UnitQuaternion::to_xml(tinyxml2::XMLDocument &document,
                         tinyxml2::XMLElement *root) const {

  tinyxml2::XMLElement* unit_quaternion_element = document.NewElement("unit_quaternion");

  unit_quaternion_element->SetAttribute("x", std::to_string(qv.data[0]).c_str() );
  unit_quaternion_element->SetAttribute("y", std::to_string(qv.data[1]).c_str() );
  unit_quaternion_element->SetAttribute("z", std::to_string(qv.data[2]).c_str() );
  unit_quaternion_element->SetAttribute("w", std::to_string(qs).c_str() );

  // Add the constructed element to the root
  root->InsertEndChild(unit_quaternion_element);

  return;
}

//
// Exports the UnitQuaternion class to a comma-separated string
//
std::string UnitQuaternion::to_csv_string( const unsigned int& precision )const{
  std::stringstream tmp;
  tmp << std::setprecision( precision ) << qv[0] << "," << qv[1] << "," << qv[2] << "," << qs;
  return tmp.str();
}

//
// This method exports a UnitQuaternion to a UnitQuaternionMessage.
//
UnitQuaternionMessage UnitQuaternion::to_msg( void )const{
  UnitQuaternionMessage msg;
  msg.s = qs;
  msg.x = qv[0];
  msg.y = qv[1];
  msg.z = qv[2];
  return msg;
}

//
// This method prints one equivalent RPY representation of the Unit Quaternion.
//
void UnitQuaternion::print_rpy( void )const{
  std::cout << "Quaternion RPY: (" << roll() << "," << pitch() << "," << yaw() << ")" << std::endl;
}

//
// UnitQuaternion class ostream operator
//
ostream &operator<<(ostream &out, const UnitQuaternion &other) {

  // out << "UnitQuaternion(";
  out << "qv=" << other.qv.data[0] << " " << other.qv.data[1] << " " << other.qv.data[2] << " ";
  out << " qs=" << other.qs;
  // out << ")";

  return out;
}

} // namespace h2sl
