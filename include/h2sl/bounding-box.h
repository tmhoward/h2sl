/**
 * @file bounding-box.h
 * @author H2SL Developers
 *
 * @version 2.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * @section DESCRIPTION
 */

#pragma once

#include <iostream>

#include "transform3d.h"
#include "h2sl/BoundingBoxMessage.h"

#include <tinyxml2.h>

namespace h2sl {
/**
 * BoundingBox class definition
 */
class BoundingBox {
public:

  /**
    Default constructor for h2sl::BoundingBox class.
    @brief Default BoundingBox class constructor.
    @returns                  none
    @throws                   no expected throws
  */
  BoundingBox() = default;

  /**
    Parameterized constructor for h2sl::BoundingBox class.
    @brief Parameterized BoundingBox class constructor.
    @param[in]    dimensionsArg        dimensions of the box along x, y and z axis
    @param[in]    offsetArg            a transformation from the object pose to box center
    @returns                           none
    @throws                            no expected throws
  */
  BoundingBox( const h2sl::Vector3D& dimensionsArg, const h2sl::Transform3D& offsetArg );

  /**
    Class constructor for BoundingBox from xml root element

    @brief Class constructor from xml root element.
    @param[in] root   xml root element
    @returns          none
    @throws           no expected throws
  */
  BoundingBox( const tinyxml2::XMLElement *root );

  /**
    Class constructor for BoundingBox from BoundingBoxMessage message.

    @brief Class constructor from BoundingBoxMessage.
    @param[in]  msg   BoundingBoxMessage message
    @returns          none
    @throws           no expected throws
  */
  BoundingBox( const BoundingBoxMessage& msg );

  /**
    Default copy constructor for h2sl::BoundingBox class
  **/
  BoundingBox( const BoundingBox& ) = default;

  /**
    Default move constructor for h2sl::BoundingBox class
  **/

  BoundingBox( BoundingBox&& ) = default;

  /**
    BoundingBox class default destructor.
  */
  virtual ~BoundingBox() = default;

  /**
    Default copy assignment operator for h2sl::BoundingBox class
  **/
  BoundingBox &operator=( const BoundingBox & ) = default;

  /**
    Default move assignment operator for h2sl::BoundingBox class
  **/
  BoundingBox& operator=( BoundingBox&& ) = default;

  /**
    This method imports a BoundingBox class from an XML file.

    @brief Import a BoundingBox class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const std::string& filename );

  /**
    This method imports a BoundingBox class from an XMLElement pointer.

    @brief Import a BoundingBox class from an XMLElement pointer.
    @overload
    @param[in]    root      XMLElement from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement *root );

  /**
    This method export a BoundingBox class to an XML file.

    @brief Export a BoundingBox to an XML file.
    @overload
    @param[in]    filename    XML filename to which BoundingBox class is exported
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool to_xml( const std::string& filename ) const;

  /**
    This method exports a BoundingBox class to an XMLDocument object. The root
    XMLElement* takes the current BoundingBox as a child XMLElement.

    @brief Export a BoundingBox class to an XMLDocument object.
    @overload
    @param[in]    document       XMLDocument object for the current document
    @param[in]    root      XMLElement* to accept a BoundingBox as a child element
    @returns                none
    @throws                 no expected throws
  */
  void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement *root ) const;

  /**
      This method exports a BoundingBox to a BoundingBoxMessage.

      @brief This method exports a BoundingBox to a BoundingBoxMessage.
      @returns                    message version of the BoundingBox
      @throws                     no expected throws
  */
  BoundingBoxMessage to_msg( void )const;

  /**
  * BoundingBox class ostream operator
  */
  friend std::ostream& operator<<( std::ostream& out, const BoundingBox& other );

  /**
  * A vector representing the dimensions of the bbox
  */
  h2sl::Vector3D dimensions = h2sl::Vector3D();

  /**
  * A transform3d representing the offset of the bbox
  */
  h2sl::Transform3D offset = h2sl::Transform3D();

protected:
private:
}; // class BoundingBox


} // h2sl namespace
