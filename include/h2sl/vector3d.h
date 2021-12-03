/**
 * @file vector3d.h
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

#pragma once

#include <iostream>
#include <vector>
#include <math.h>

#include "h2sl/Vector3DMessage.h"

#include <tinyxml2.h>

namespace h2sl {
/**
 * Vector3D class definition
 */
class Vector3D {
public:

  /**
    Default constructor for h2sl::Vector3D class.
    @brief Default Vector3D class constructor.
    @returns                  none
    @throws                   no expected throws
  */
  Vector3D() = default;

  /**
    Arg constructor for h2sl::Vector3D class.

    @brief Arg Vector3D class constructor.
    @param[in]    x        x element
    @param[in]    y        y element
    @param[in]    z        z element
    @returns                  none
    @throws                   no expected throws
  */
  Vector3D(const double& x, const double& y, const double& z);

  /**
    Class constructor for Vector3D from xml root element

    @brief Class constructor from xml root element.
    @param[in] root   xml root element
    @returns          none
    @throws           no expected throws
  */
  Vector3D( const tinyxml2::XMLElement *root );

  /**
    Class constructor for Vector3D from Vector3DMessage message.

    @brief Class constructor from Vector3DMessage.
    @param[in]  msg   h2sl::Vector3DMessage message
    @returns          none
    @throws           no expected throws
  */
  Vector3D( const Vector3DMessage& msg );

  /**
    Default copy constructor for h2sl::Vector3D class
  **/
  Vector3D( const Vector3D & ) = default;

  /**
    Default move constructor for h2sl::Vector3D class
  **/

  Vector3D( Vector3D&& ) = default;

  /**
    Vector3D class default destructor.

    @brief Vector3D class default destructor.
    @returns                  none
    @throws                   no expected throws
  */
  virtual ~Vector3D() = default;

  /**
    Default copy assignment operator for h2sl::Vector3D class
  **/
  Vector3D &operator=( const Vector3D & ) = default;

  /**
    Default move assignment operator for h2sl::Vector3D class
  **/
  Vector3D& operator=( Vector3D&& ) = default;

  /**
    Vector3D class element accessor operator.

    @brief Element accessor operator.
    @param[in]    index       index in the vector to be accessed
    @returns                  reference to the value at the provided index
    @throws                   no expected throws
  */
  double& operator[]( const unsigned int& index );

  /**
    Vector3D class element accessor operator (const version).

    @brief Element accessor operator (const version).
    @param[in]    index       index in the vector to be accessed
    @returns                  reference to the value at the provided index
    @throws                   no expected throws
  */
  const double& operator[]( const unsigned int& index )const;

  /**
    Vector3D class element wise addition operator.

    @brief Element element wise addition operator.
    @param[in]    rhs         vector to be added
    @returns                  vector after performing the element wise addition
    @throws                   no expected throws
  */
  Vector3D operator+( const Vector3D& rhs )const;

  /**
    Vector3D class element wise subtraction operator.

    @brief Element element wise subtraction operator.
    @param[in]    rhs         vector to be subtracted
    @returns                  vector after performing the element wise subtraction
    @throws                   no expected throws
  */
  Vector3D operator-( const Vector3D& rhs )const;

  /**
    Vector3D class element wise multiplication operator.

    @brief Element element wise multiplication operator.
    @param[in]    rhs         vector to multiply with
    @returns                  vector after performing the element wise multiplication
    @throws                   no expected throws
  */
  Vector3D operator*( const Vector3D& rhs )const;

  /**
    Vector3D class element wise division operator.

    @brief Element element wise division operator.
    @param[in]    rhs         vector to divide with
    @returns                  vector after performing the element wise division
    @throws                   no expected throws
  */
  Vector3D operator/( const Vector3D& rhs )const;

  /**
    Vector3D class element wise addition operator.

    @brief Element element wise addition operator.
    @param[in]    value       value to be added
    @returns                  vector after performing the element wise addition
    @throws                   no expected throws
  */
  Vector3D operator+( const double& value )const;

  /**
    Vector3D class element wise subtraction operator.

    @brief Element element wise subtraction operator.
    @param[in]    value       value to be subtracted
    @returns                  vector after performing the element wise subtraction
    @throws                   no expected throws
  */
  Vector3D operator-( const double& value )const;

  /**
    Vector3D class element wise multiplication operator.

    @brief Element element wise multiplication operator.
    @param[in]    value       value to multiply with
    @returns                  vector after performing the element wise multiplication
    @throws                   no expected throws
  */
  Vector3D operator*( const double& value )const;

  /**
    Vector3D class element wise division operator.

    @brief Element element wise division operator.
    @param[in]    value       value to divide with
    @returns                  vector after performing the element wise division
    @throws                   no expected throws
  */
  Vector3D operator/( const double& value )const;

  /**
    This method returns the L2 norm of this vector.

    @brief Returns L2 norm of this vector.
    @returns                  double representing the L2 norm of this vector
    @throws                   no expected throws
  */
  double norm( void )const;

  /**
    This method returns the Euclidean distance between this vector and the input.

    @brief Returns Euclidean distance between this vector and the input.
    @param[in]    rhs         vector to which the distance has to be calculated
    @returns                  double representing the Euclidean distance between this vector and the input
    @throws                   no expected throws
  */
  double distance( const Vector3D& rhs );

  /**
    This method returns the dot product between this vector and the input.

    @brief Returns dot product between this vector and the input.
    @param[in]    rhs         vector with which to compute the dot product
    @returns                  double representing the dot product between this vector and the input
    @throws                   no expected throws
  */
  double dot( const Vector3D& rhs )const;

  /**
    This method returns the cross product between this vector and the input.

    @brief Returns cross product between this vector and the input.
    @param[in]    rhs         vector with which to compute the cross product
    @returns                  a vector representing the cross product between this vector and the input
    @throws                   no expected throws
  */
  Vector3D cross( const Vector3D& rhs )const;

  /**
    This method returns the x value.

    @brief Returns the x value.
    @overload
    @returns                    the x value in this vector
    @throws                     no expected throws
  */
  double x( void )const;

  /**
    This method returns the y value.

    @brief Returns the y value.
    @overload
    @returns                    the y value in this vector
    @throws                     no expected throws
  */
  double y( void )const;

  /**
    This method returns the z value.

    @brief Returns the z value.
    @overload
    @returns                    the z value in this vector
    @throws                     no expected throws
  */
  double z( void )const;

  /**
    This method returns this vector normalized to unit length.

    @brief Returns this vector normalized to unit length.
    @returns                    normalized vector with unit length
    @throws                     no expected throws
  */
  Vector3D unit( void )const;

  /**
    This method imports a Vector3D class from a comma-separated string.

    @brief Import a Vector3D class from a comma-separated string.
    @overload
    @param[in]    cs_string    csv string from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_cs_string( const std::string &cs_string );

  /**
    This method imports a Vector3D class from an XML file.

    @brief Import a Vector3D class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const std::string &filename );

  /**
    This method imports a Vector3D class from an XMLElement pointer.

    @brief Import a Vector3D class from an XMLElement pointer.
    @overload
    @param[in]    root     XMLElement pointer from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement *root );

  /**
    This method export a Vector3D class to an XML file.

    @brief Export a Vector3D to an XML file.
    @overload
    @param[in]    filename    XML filename to which Vector3D class is exported
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool to_xml( const std::string &filename ) const;

  /**
    This method exports a Vector3D class to an XMLDocument object. The root
    XMLElement* takes the current Vector3D as a child XMLElement.

    @brief Export a Vector3D class to an XMLDocument object.
    @overload
    @param[in]    doc       XMLDocument object for the current document
    @param[in]    root      XMLElement* to accept a Vector3D as a child element
    @returns                none
    @throws                 no expected throws
  */
  void to_xml( tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *root ) const;

  /**
      This method exports a Vector3D to a comma-separated string.

      @brief This method exports a Vector3D to a comma-separated string
      @param[in]    precision     number of digits after the decimal point
      @returns                    comma-separated string
      @throws                     no expected throws
  */
  std::string to_csv_string( const unsigned int& precision = 4 )const;

  /**
      This method exports a Vector3D to a Vector3DMessage.

      @brief This method exports a Vector3D to a Vector3DMessage.
      @returns                    message version of the Vector3D
      @throws                     no expected throws
  */
  Vector3DMessage to_msg( void )const;

  /**
  * Vector3D class ostream operator
  */
  friend std::ostream& operator<<( std::ostream &out, const Vector3D &other );

  /**
  * A standard vector of doubles
  */
  std::vector<double> data = std::vector<double>( 3, 0.0 );

protected:
private:
};


}
