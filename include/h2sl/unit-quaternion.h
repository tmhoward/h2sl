/**
 * @file unit-quaternion.h
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

#include "h2sl/vector3d.h"
#include "h2sl/UnitQuaternionMessage.h"

#include <tinyxml2.h>

namespace h2sl {
/**
 * UnitQuaternion class definition
 */
class UnitQuaternion {
public:

  /**
    Default constructor for h2sl::UnitQuaternion class.
    @brief Default UnitQuaternion class constructor.
    @returns                  none
    @throws                   no expected throws
  */
  UnitQuaternion() = default;

  /**
    Arg constructor for h2sl::UnitQuaternion class.

    @brief Arg UnitQuaternion class constructor.
    @param[in]    qvArg       First three elements of the quaternion
    @param[in]    qsArg       The last element of the quaternion
    @returns                  none
    @throws                   no expected throws
  */
  UnitQuaternion( const h2sl::Vector3D& qvArg, const double& qsArg );

  /**
    Arg constructor for h2sl::UnitQuaternion class.

    @brief Arg UnitQuaternion class constructor.
    @param[in]    roll        Roll for the quaternion
    @param[in]    pitch       Pitch for the quaternion
    @param[in]    yaw         Yaw for the quaternion
    @returns                  none
    @throws                   no expected throws
  */
  UnitQuaternion( const double& roll, const double& pitch, const double& yaw );

  /**
    Class constructor for UnitQuaternion from xml root element

    @brief Class constructor from xml root element.
    @param[in] root   xml root element
    @returns          none
    @throws           no expected throws
  */
  UnitQuaternion( const tinyxml2::XMLElement *root );
  
  /**
    Class constructor for UnitQuaternion from UnitQuaternionMessage message.

    @brief Class constructor from UnitQuaternionMessage.
    @param[in]  msg   UnitQuaternionMessage message
    @returns          none
    @throws           no expected throws
  */
  UnitQuaternion( const UnitQuaternionMessage& msg );

  /**
    Default copy constructor for h2sl::UnitQuaternion class
  **/
  UnitQuaternion( const UnitQuaternion & ) = default;

  /**
    Default move constructor for h2sl::UnitQuaternion class
  **/

  UnitQuaternion( UnitQuaternion&& ) = default;

  /**
    UnitQuaternion class default destructor.
  */
  virtual ~UnitQuaternion() = default;

  /**
    Default copy assignment operator for h2sl::UnitQuaternion class
  **/
  UnitQuaternion &operator=( const UnitQuaternion & ) = default;

  /**
    Default move assignment operator for h2sl::UnitQuaternion class
  **/
  UnitQuaternion& operator=( UnitQuaternion&& ) = default;

  /**
    UnitQuaternion class scalar multiplication operator.

    @brief Multiply this quaternion with another and return the product.
    @param[in]    rhs         A scalar value with which to multiply
    @returns                  UnitQuaternion representing the product
    @throws                   no expected throws
  */
  UnitQuaternion operator*( const double& rhs )const;

  /**
    UnitQuaternion class vector multiplication operator.

    @brief Multiply this vector with another and return the product.
    @param[in]    rhs         a Vector with which to multiply
    @returns                  a Vector representing the product
    @throws                   no expected throws
  */
  Vector3D operator*( const Vector3D& rhs )const;

  /**
    UnitQuaternion class quaternion multiplication operator.

    @brief Multiply this quaternion with another and return the product.
    @param[in]    rhs         a UnitQuaternion with which to multiply
    @returns                  UnitQuaternion representing the product
    @throws                   no expected throws
  */
  UnitQuaternion operator*( const UnitQuaternion& rhs )const;

  /**
    UnitQuaternion class quaternion self multiplication operator.

    @brief Edit this quaternion by multiplying it with another.
    @param[in]    rhs         a UnitQuaternion with which to multiply
    @throws                   no expected throws
  */
  void operator*=( const UnitQuaternion& rhs );

  /**
    UnitQuaternion class scalar self multiplication operator.

    @brief Edit this quaternion by multiplying it with the given scalar.
    @param[in]    rhs         a scalar with which to multiply
    @throws                   no expected throws
  */
  void operator*=( const double& rhs );

  /**
      Method to return the roll of the UnitQuaternion in radians.

      @brief Method to return the roll of the UnitQuaternion.
      @returns              UnitQuaternion roll in radians.
      @throws               no expected throws
  */
  double roll( void ) const;

  /**
      Method to return the pitch of the UnitQuaternion in radians.

      @brief Method to return the pitch of the UnitQuaternion.
      @returns              UnitQuaternion pitch in radians.
      @throws               no expected throws
  */
  double pitch( void ) const;

  /**
      Method to return the yaw of the UnitQuaternion in radians.

      @brief Method to return the yaw of the UnitQuaternion.
      @returns              UnitQuaternion yaw in radians.
      @throws               no expected throws
  */
  double yaw( void ) const;

  /**
    This method returns the conjugate of this quaternion

    @brief Return a conjugate of this quaternion.
    @overload
    @returns                h2sl::UnitQuaternion representing the conjugate of this quaternion 
    @throws                 no expected throws
  */
  UnitQuaternion conjugate( void )const;

  /**
    This method imports a UnitQuaternion class from a CSV string.

    @brief Import a UnitQuaternion class from a CSV string.
    @overload
    @param[in]    csv_string    csv string from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_cs_string( const std::string &csv_string );

  /**
    This method imports a UnitQuaternion class from an XML file.

    @brief Import a UnitQuaternion class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const std::string &filename );
  
  /**
    This method imports a UnitQuaternion class from an XMLElement pointer.

    @brief Import a UnitQuaternion class from an XMLElement pointer.
    @overload
    @param[in]    root     XMLElement pointer from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement *root );

  /**
    This method export a UnitQuaternion class to an XML file.

    @brief Export a UnitQuaternion to an XML file.
    @overload
    @param[in]    filename    XML filename to which UnitQuaternion class is exported
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool to_xml( const std::string &filename ) const;

  /**
    This method exports a UnitQuaternion class to an XMLDocument object. The root
    XMLElement* takes the current UnitQuaternion as a child XMLElement.

    @brief Export a UnitQuaternion class to an XMLDocument object.
    @overload
    @param[in]    doc       XMLDocument object for the current document
    @param[in]    root      XMLElement* to accept a UnitQuaternion as a child element
    @returns                none
    @throws                 no expected throws
  */
  void to_xml( tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *root ) const;

  /**
      This method exports a UnitQuaternion to a comma-separated string.

      @brief This method exports a UnitQuaternion to a comma-separated string (qv followed by qs)
      @param[in]    precision     number of digits after the decimal point
      @returns                    comma-separated string
      @throws                     no expected throws
  */
  std::string to_csv_string( const unsigned int& precision = 4 )const;
  
  /**
      This method exports a UnitQuaternion to a UnitQuaternionMessage.

      @brief This method exports a UnitQuaternion to a UnitQuaternionMessage.
      @returns                    message version of the UnitQuaternion
      @throws                     no expected throws
  */
  UnitQuaternionMessage to_msg( void )const;

  /**
      This method prints one equivalent RPY representation of the Unit Quaternion.

      @brief This method prints one equivalent RPY representation of the Unit Quaternion.
      @returns          none
      @throws           no expected throws
  */
  void print_rpy( void )const;

  /**
  * UnitQuaternion class ostream operator
  */
  friend std::ostream& operator<<( std::ostream &out, const UnitQuaternion &other );


protected:
private:

  /**
  * A vector representing the vector element of the quaternion
  */
  h2sl::Vector3D qv = h2sl::Vector3D();

  /**
  * A double representing the scalar element of the quaternion
  */
  double qs = 1.0;

  /**
    This method normalizes the quaternion.

    @brief Normalize this quaternion.
    @overload
    @throws                     no expected throws
  */
  void _normalize( void );

};

}
