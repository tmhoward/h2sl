/**
 * @file vector3d.cc
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
#include "h2sl/vector3d.h"

using namespace std;

namespace h2sl{

//
// Vector3D class default constructor
//
Vector3D::Vector3D( const double& x, const double& y, const double& z ){
  data[0] = x;
  data[1] = y;
  data[2] = z;
}

//
// Vector3D XML constructor
//
Vector3D::Vector3D( const tinyxml2::XMLElement *root ){
  from_xml(root);
}

//
// Class constructor for Vector3D from Vector3DMessage message.
//
Vector3D::Vector3D( const Vector3DMessage& msg ){
  data[0] = msg.x;
  data[1] = msg.y;
  data[2] = msg.z;
}

//
// Vector3D class element accessor operator
//
double& Vector3D::operator[]( const unsigned int& index ){
  if( index < data.size() )
    return data[ index ];
  else{
    std::cout << "index out of bounds" << std::endl;
    std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }
}

//
// Vector3D class element accessor operator
//
const double& Vector3D::operator[]( const unsigned int& index )const{
  if( index < data.size() )
    return data[ index ];
  else{
    std::cout << "index out of bounds" << std::endl;
    std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }
}

//
// Vector3D class element wise addition operator
//
Vector3D Vector3D::operator+( const Vector3D& rhs )const{
  if( data.size() != rhs.data.size() ){
    std::cout << "vector sizes dont match" << std::endl;
    std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }

  h2sl::Vector3D result;
  for( unsigned int i = 0; i < data.size(); i++ ){
    result.data[i] = data[i] + rhs[i];
  }

  return (result);
}


//
// Vector3D class element wise subtraction operator
//
Vector3D Vector3D::operator-( const Vector3D& rhs )const{

  if( data.size() != rhs.data.size() ){
    std::cout << "vector sizes dont match" << std::endl;
    std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }

  h2sl::Vector3D result;
  for( unsigned int i = 0; i < data.size(); i++ ){
    result.data[i] = data[i] - rhs[i];
  }

  return (result);
}

//
// Vector3D class element wise multiplication operator
//
Vector3D Vector3D::operator*( const Vector3D& rhs )const{

  if( data.size() != rhs.data.size() ){
    std::cout << "vector sizes dont match" << std::endl;
    std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }


  h2sl::Vector3D result;
  for( unsigned int i = 0; i < data.size(); i++ ){
    result.data[i] = data[i] * rhs[i];
  }

  return (result);
}

//
// Vector3D class element wise division operator
//
Vector3D Vector3D::operator/( const Vector3D& rhs )const{
  if( data.size() != rhs.data.size() ){
    std::cout << "vector sizes dont match" << std::endl;
    std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }

  h2sl::Vector3D result;
  for( unsigned int i = 0; i < data.size(); i++ ){

    if( rhs[ i ] == 0.0 ){
      std::cout << "cannot divide by 0" << std::endl;
      std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
      exit(EXIT_FAILURE);
    }

    result.data[i] = data[i] / rhs[i];
  }

  return (result);
}

//
// Vector3D class element wise addition operator
//
Vector3D Vector3D::operator+( const double& value )const{

  h2sl::Vector3D result = (*this);
  for( unsigned int i = 0; i < result.data.size(); i++ ){
    result[ i ] += value;
  }

  return result;
}


//
// Vector3D class element wise subtraction operator
//
Vector3D Vector3D::operator-( const double& value )const{

  h2sl::Vector3D result = (*this);
  for( unsigned int i = 0; i < result.data.size(); i++ ){
    result[ i ] -= value;
  }

  return result;
}

//
// Vector3D class element wise multiplication operator
//
Vector3D Vector3D::operator*( const double& value )const{

  h2sl::Vector3D result = (*this);
  for( unsigned int i = 0; i < result.data.size(); i++ ){
    result[ i ] *= value;
  }

  return result;
}

//
// Vector3D class element wise division operator
//
Vector3D Vector3D::operator/( const double& value )const{

  if( value == 0.0 ){
    std::cout << "cannot divide by 0" << std::endl;
    std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }

  h2sl::Vector3D result = (*this);
  for( unsigned int i = 0; i < result.data.size(); i++ ){
    result[ i ] /= value;
  }

  return result;

}

//
// Vector3D class L2 norm method
//
double Vector3D::norm( void )const{

  double sum = 0.0;
  for( const auto& element : data ){
    sum += element*element;
  }
  return sqrt( sum );
}

//
// Vector3D class L2 distance method
//
double Vector3D::distance( const Vector3D& rhs ){

  h2sl::Vector3D difference_vector = (*this) - rhs;
  return difference_vector.norm();

}

//
// Vector3D class dot product method
// x1x2 + y1y2 + z1z2
//
double Vector3D::dot( const Vector3D& rhs )const{

  h2sl::Vector3D product_vector = (*this) * rhs;

  double sum = 0.0;
  for( const auto& element : product_vector.data ){
    sum += element;
  }

  return sum;
}

//
// Vector3D class cross product method
//
Vector3D Vector3D::cross( const Vector3D& rhs )const{
  return Vector3D( data[1] * rhs.data[2] - data[2] * rhs.data[1],
                   data[2] * rhs.data[0] - data[0] * rhs.data[2],
                   data[0] * rhs.data[1] - data[1] * rhs.data[0] );
}

// Returns the x element
//
double Vector3D::x( void )const{
  return data[0];
}

//
// Returns the y element
//
double Vector3D::y( void )const{
  return data[1];
}

//
// Returns the z element
//
double Vector3D::z( void )const{
  return data[2];
}

//
// Returns this vector normalized to unit length
//
Vector3D Vector3D::unit( void )const{
  double length = norm();
  if( length == 0.0 ){
    return Vector3D( 0.0, 0.0, 0.0 );
  }
  return Vector3D( data[0] / length, data[1] / length, data[2] / length );
}

//
// Import the Vector3D class from a comma-separated string
//
bool Vector3D::from_cs_string( const std::string &cs_string ){
  std::vector< std::string > tmp_strings;
  boost::split( tmp_strings, cs_string, boost::is_any_of( "," ) );
  if( tmp_strings.size() == 3 ){
    data[0] = std::stod( tmp_strings[ 0 ] );
    data[1] = std::stod( tmp_strings[ 1 ] );
    data[2] = std::stod( tmp_strings[ 2 ] );
    return true;
  } else {
    return false;
  }
}

//
// Imports the Vector3D class from an XML file
//
bool Vector3D::from_xml(const string &filename) {

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

  // Check for any vector child element of the root
  const tinyxml2::XMLElement* vector_element = root->FirstChildElement("vector");
  if( vector_element == nullptr )
    return false;

  return from_xml( vector_element );

}

//
// Imports the Vector3D class from an tinyxml2::XMLElement pointer
//
bool Vector3D::from_xml(const tinyxml2::XMLElement *vector_element) {

  if (vector_element == nullptr)
    return false;

  data = std::vector<double>( 3, 0.0 );

  // Read the attributes
  const auto* x_attr = vector_element->FindAttribute("x");
  const auto* y_attr = vector_element->FindAttribute("y");
  const auto* z_attr = vector_element->FindAttribute("z");


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

  data[0] = std::stod(x_attr->Value());
  data[1] = std::stod(y_attr->Value());
  data[2] = std::stod(z_attr->Value());

  return true;
}

//
// Exports the Vector3D class to an XML file
//
bool Vector3D::to_xml(const string &filename) const {

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
// Exports the Vector3D class to an XML node pointer
//
void Vector3D::to_xml(tinyxml2::XMLDocument &document,
                         tinyxml2::XMLElement *root) const {

  auto* vector_element = document.NewElement("vector");

  vector_element->SetAttribute("x", std::to_string(data[0]).c_str() );
  vector_element->SetAttribute("y", std::to_string(data[1]).c_str() );
  vector_element->SetAttribute("z", std::to_string(data[2]).c_str() );

  // Add the constructed element to the root
  root->InsertEndChild(vector_element);

  return;
}

//
// Exports the Vector3D class to a comma-separated string
//
std::string Vector3D::to_csv_string( const unsigned int& precision )const{
  std::stringstream tmp;
  tmp << std::setprecision( precision ) << data[0] << "," << data[1] << "," << data[2];
  return tmp.str();
}

//
// This method exports a Vector3D to a Vector3DMessage.
//
Vector3DMessage Vector3D::to_msg( void )const{
  Vector3DMessage msg;
  msg.x = data[0];
  msg.y = data[1];
  msg.z = data[2];
  return msg;
}

//
// Vector3D class ostream operator
//
ostream &operator<<(ostream &out, const Vector3D &other) {

  // out << "(Vector3D:";

  out << "x=" << other.data[0];
  out << " y=" << other.data[1];
  out << " z=" << other.data[2];

  // out << ")";
  return out;
}

} // namespace h2sl
