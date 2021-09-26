/**
 * @file transform3d.h
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
#include "h2sl/unit-quaternion.h"
#include "h2sl/Transform3DMessage.h"

#include <tinyxml2.h>

namespace h2sl {
/**
 * Transform3D class definition
 */
class Transform3D {
public:

  /**
    Default constructor for h2sl::Transform3D class.
    @brief Default Transform3D class constructor.
    @returns                  none
    @throws                   no expected throws
  */
  Transform3D() = default;

  /**
    Parameterized constructor for h2sl::Transform3D class.

    @brief Default Transform3D class constructor.
    @param[in] positionArg      position represented by h2sl::Vector3D
    @param[in] orientationArg   orientation represented by h2sl::UnitQuaternion
    @returns                    none
    @throws                     no expected throws
  */
  Transform3D( const Vector3D& positionArg ,
               const UnitQuaternion& orientationArg );

  /**
    Class constructor for Transform3D from xml root element

    @brief Class constructor from xml root element.
    @param[in] root   xml root element
    @returns          none
    @throws           no expected throws
  */
  Transform3D( const tinyxml2::XMLElement *root );
  
  /**
    Class constructor for Transform3D from Transform3DMessage message.

    @brief Class constructor from Transform3DMessage.
    @param[in]  msg   Transform3DMessage message
    @returns          none
    @throws           no expected throws
  */
  Transform3D( const Transform3DMessage& msg );

  /**
    Default copy constructor for h2sl::Transform3D class
  **/
  Transform3D( const Transform3D & ) = default;

  /**
    Default move constructor for h2sl::Transform3D class
  **/

  Transform3D( Transform3D&& ) = default;

  /**
    Transform3D class default destructor.
  */
  virtual ~Transform3D() = default;

  /**
    Default copy assignment operator for h2sl::Transform3D class
  **/
  Transform3D &operator=( const Transform3D & ) = default;

  /**
    Default move assignment operator for h2sl::Transform3D class
  **/
  Transform3D& operator=( Transform3D&& ) = default;

  /**
    Transform3D class multiplication operator.

    @brief Multiply this transform with another and return the product.
    @param[in]    rhs         a Transform3D with which to multiply
    @returns                  Transform3D representing the product
    @throws                   no expected throws
  */
  Transform3D operator*( const Transform3D& rhs )const;

  /**
    Transform3D class self multiplication operator.

    @brief Edit this transform by multiplying it with another.
    @param[in]    rhs         a Transform3D with which to multiply
    @throws                   no expected throws
  */
  void operator*=( const Transform3D& rhs );

  /**
    This method returns the inverse of this transformation.

    @brief Returns the inverse of this transformation.
    @overload
    @returns                    an inverse transformation of this transform3D
    @throws                     no expected throws
  */
  Transform3D inverse( void )const;

  /**
    This method returns the x value of the position element.

    @brief Returns the the x value of the position element.
    @overload
    @returns                    the x value of the position element inside this transofrm
    @throws                     no expected throws
  */
  double x( void )const;

  /**
    This method returns the y value of the position element.

    @brief Returns the the y value of the position element.
    @overload
    @returns                    the y value of the position element inside this transofrm
    @throws                     no expected throws
  */
  double y( void )const;

  /**
    This method returns the z value of the position element.

    @brief Returns the the z value of the position element.
    @overload
    @returns                    the z value of the position element inside this transofrm
    @throws                     no expected throws
  */
  double z( void )const;

  /**
    This method imports a Transform3D class from an XML file.

    @brief Import a Transform3D class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const std::string &filename );
  
  /**
    This method imports a Transform3D class from an XMLElement pointer.

    @brief Import a Transform3D class from an XMLElement pointer.
    @overload
    @param[in]    root     XMLElement pointer from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement *root );

  /**
    This method export a Transform3D class to an XML file.

    @brief Export a Transform3D to an XML file.
    @overload
    @param[in]    filename    XML filename to which Transform3D class is exported
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool to_xml( const std::string &filename ) const;

  /**
    This method exports a Transform3D class to an XMLDocument object. The root
    XMLElement* takes the current Transform3D as a child XMLElement.

    @brief Export a Transform3D class to an XMLDocument object.
    @overload
    @param[in]    doc       XMLDocument object for the current document
    @param[in]    root      XMLElement* to accept a Transform3D as a child element
    @returns                none
    @throws                 no expected throws
  */
  void to_xml( tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *root ) const;
  
  /**
      This method exports a Transform3D to a Transform3DMessage.

      @brief This method exports a Transform3D to a Transform3DMessage.
      @returns                    message version of the Transform3D
      @throws                     no expected throws
  */
  Transform3DMessage to_msg( void )const;

  /**
  * Transform3D class ostream operator
  */
  friend std::ostream& operator<<( std::ostream &out, const Transform3D &other );

  /**
  * A unit quaternion representing the orientation of the transform
  */
  h2sl::UnitQuaternion orientation = h2sl::UnitQuaternion( h2sl::Vector3D(0.0,0.0,0.0), 1.0 );
  
  /**
  * A 3d vector representing the position transform
  */
  h2sl::Vector3D position = h2sl::Vector3D( 0.0, 0.0, 0.0 );


protected:
private:
};


}
