/**
 * @file objectstate.h
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
#include <vector>
#include <memory>

#include "h2sl/transform3d.h"
#include "h2sl/bounding-box.h"
#include "h2sl/ObjectStateMessage.h"

#include <tinyxml2.h>
#include <jsoncpp/json/value.h>

namespace h2sl {
/**
 * ObjectState class definition
 */
class ObjectState {
public:
  /**
    Type alias to represent the state history of an object. It is a vector of
    ObjectStates, where ObjectState is a class that stores the position, orientation,
    bounding box, angular velocity, linear velocity, and the associated timestamp.
  */
  using vectorObjectState = std::vector<h2sl::ObjectState>;

  /**
    Default constructor for h2sl::ObjectState class.

    @brief Default ObjectState class constructor.
    @returns                  none
    @throws                   no expected throws
  */
  ObjectState() = default;

  /**
    Default constructor for h2sl::ObjectState class.

    @brief Default ObjectState class constructor.
    @param[in] poseArg object pose
    @param[in] boundingBoxArg collision geometry of the object
    @param[in] vArg linear velocity vector
    @param[in] wArg angular velocity vector
    @param[in] timeArg timestamp in nano seconds
    @returns                  none
    @throws                   no expected throws
  */
  ObjectState( std::shared_ptr<h2sl::Transform3D> poseArg,
               std::shared_ptr<h2sl::BoundingBox> boundingBoxArg,
               std::shared_ptr<h2sl::Vector3D> vArg,
               std::shared_ptr<h2sl::Vector3D> wArg,
               std::shared_ptr<double> timeArg );

  /**
    Class constructor for ObjectState from xml root element

    @brief Class constructor from xml root element.
    @param[in] root   xml root element
    @returns          none
    @throws           no expected throws
  */
  ObjectState( const tinyxml2::XMLElement *root );

  /**
    Class constructor for ObjectState from ObjectStateMessage message.

    @brief Class constructor from ObjectStateMessage.
    @param[in]  msg   ObjectStateMessage message
    @returns          none
    @throws           no expected throws
  */
  ObjectState( const ObjectStateMessage& msg );

  /**
    Default copy constructor for h2sl::ObjectState class
  **/
  ObjectState( const ObjectState & ) = default;

  /**
    Default move constructor for h2sl::ObjectState class
  **/

  ObjectState( ObjectState&& ) = default;

  /**
    ObjectState class default destructor.
  */
  virtual ~ObjectState() = default;

  /**
    Default copy assignment operator for h2sl::ObjectState class
  **/
  ObjectState &operator=( const ObjectState & ) = default;

  /**
    Default move assignment operator for h2sl::ObjectState class
  **/
  ObjectState& operator=( ObjectState&& ) = default;

  /**
    This method imports a ObjectState class from an XML file.

    @brief Import a ObjectState class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml(const std::string& filename);

  /**
    This method imports a ObjectState class from an XMLElement pointer.

    @brief Import a ObjectState class from an XMLElement pointer.
    @overload
    @param[in]    filename      filename of the XMLElement pointer from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml(const tinyxml2::XMLElement *root);

  /**
      This method imports a ObjectState from a json formatted string.

      @brief This method imports a ObjectState from a json formatted string.
      @param[in]    json_string   json formatting string to read from
      @returns                    success/failure
      @throws                     no expected throws
  */
  bool from_json( const std::string& json_string );

  /**
      This method imports a ObjectState from a json value.

      @brief This method imports a ObjectState from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  bool from_json( const Json::Value& root );

  /**
    This method export a ObjectState class to an XML file.

    @brief Export a ObjectState to an XML file.
    @overload
    @param[in]    filename    XML filename to which ObjectState class is exported
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool to_xml(const std::string& filename) const;

  /**
    This method exports a ObjectState class to an XMLDocument object. The root
    XMLElement* takes the current ObjectState as a child XMLElement.

    @brief Export a ObjectState class to an XMLDocument object.
    @overload
    @param[in]    doc       XMLDocument object for the current document
    @param[in]    root      XMLElement* to accept a ObjectState as a child element
    @returns                none
    @throws                 no expected throws
  */
  void to_xml(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement *root) const;

  /**
      This method exports a ObjectState to a json formatted string.

      @brief This method exports a ObjectState to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  std::string to_json( void )const;

  /**
      This method exports a ObjectState to a json value.

      @brief This method exports a ObjectState to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  void to_json( Json::Value& root )const;

  /**
      This method exports an ObjectState to an ObjectStateMessage.

      @brief This method exports an ObjectState to an ObjectStateMessage.
      @returns                    message version of the ObjectState
      @throws                     no expected throws
  */
  ObjectStateMessage to_msg( void )const;

  /**
  * ObjectState class ostream operator
  */
  friend std::ostream& operator<<(std::ostream& out, const ObjectState& other);

  /** Shared pointer to an instance of Transform3D to represent the pose of the object.
  * This pointer points to NULL when this metric measurment is not available.
  */
  std::shared_ptr<h2sl::Transform3D> pose = nullptr;

  /** Shared pointer to an instance of BoundingBox to represent the collision geometry of the object.
  * This pointer points to NULL when this metric measurment is not available.
  */
  std::shared_ptr<h2sl::BoundingBox> bbox = nullptr;

  /** Shared pointer to a h2sl::Vector3 of doubles to represent the linear velocity of the object.
  * This pointer points to NULL when this metric measurment is not available.
  */
  std::shared_ptr<h2sl::Vector3D> v = nullptr;

  /** Shared pointer to a h2sl::Vector3 of doubles to represent the angular velocity of the object.
  * This pointer points to NULL when this metric measurment is not available.
  */
  std::shared_ptr<h2sl::Vector3D> w = nullptr;

  /** Shared pointer to a double to represent the timestamp in nano seconds (Unix epoch time).
  * This pointer points to NULL when this metric measurment is not available.
  */
  std::shared_ptr<double> t = nullptr;

protected:
private:
}; // class ObjectState

} // namespace h2sl
