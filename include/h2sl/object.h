/**
 * @file object.h
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
#include <map>
#include <vector>

#include <tinyxml2.h>
#include <jsoncpp/json/value.h>

#include "h2sl/common.h"
#include "h2sl/objectstate.h"
#include "h2sl/object-probabilistic.h"
#include "h2sl/ObjectMessage.h"

namespace h2sl {
/**
  A class to represent a deterministic Object in h2sl. This class samples each metric and
  semantic property distribution from an ObjectProbabilistic to find deterministic values.
*/
class Object {
  public:

  /**
    Type alias to represent the sampled semantic properties of the Object.
  */
  using mapProperties = std::map<std::string, std::string>;

  /**
    Object empty class constructor.
  **/
  Object( void ) = default;

  /**
    Object default class constructor.

    @brief Default Object class constructor.
    @param[in]   uidArg            unique identifier
    @param[in]   stateHistoryArg   state history for the Object
    @param[in]   propertiesArg     deterministic semantic properties
    @returns                       none
    @throws                        no expected throws
  */
  Object( const std::string& uidArg,
          const ObjectState::vectorObjectState& stateHistoryArg,
	  const mapProperties& propertiesArg = mapProperties() );
  
  /**
    Object class constructor from a static state.

    @brief Object class constructor from a static state.
    @param[in]   uidArg            unique identifier
    @param[in]   stateArg          single ObjectState to fill Object state history
    @param[in]   propertiesArg     deterministic semantic properties
    @returns                       none
    @throws                        no expected throws
  */
  Object( const std::string& uidArg,
          const ObjectState& stateArg,
          const mapProperties& propertiesArg = mapProperties() ); 
  /**
    Object class constructor from an ObjectProbabilistic.

    @brief Object class constructor from an ObjectProbabilistic.
    @param[in]    objectArg     ObjectProbabilistic from which to sample values
    @returns                    none
    @throws                     no expected throws
  */
  Object( const ObjectProbabilistic& objectArg );

  /**
    Class constructor for Object from xml root element

    @brief Class constructor from xml root element.
    @param[in] root   xml root element
    @returns          none
    @throws           no expected throws
  */
  Object( const tinyxml2::XMLElement *root );
  
  /**
    Class constructor for Object from ObjectMessage message.

    @brief Class constructor from ObjectMessage.
    @param[in]  msg   ObjectMessage message
    @returns          none
    @throws           no expected throws
  */
  Object( const ObjectMessage& msg );


  /**
    Default copy constructor for h2sl::Object class
  **/
  Object( const Object& ) = default;

  /**
    Default move constructor for h2sl::Object class
  **/
  Object( Object&& ) = default;

  /**
    Object class default destructor.
  */
  virtual ~Object() = default;

  /**
    Default copy assignment operator for h2sl::Object class
  **/
  Object& operator=( const Object& ) = default;

  /**
    Default move assignment operator for h2sl::Object class
  **/
  Object& operator=( Object&& ) = default;

  /**
    This method imports an Object class from an XML file.

    @brief Import an Object class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml(const std::string& filename);
  
  /**
    This method imports an Object class from an XMLElement pointer.

    @brief Import an Object class from an XMLElement pointer.
    @overload
    @param[in]    filename      filename of the XMLElement pointer from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml(const tinyxml2::XMLElement *root);

  /**
      This method imports an Object from a json formatted string.

      @brief This method imports an Object from a json formatted string.
      @param[in]    json_string   json formatting string to read from
      @returns                    success/failure
      @throws                     no expected throws
  */
  bool from_json( const std::string& json_string );

  /**
      This method imports an Object from a json value.

      @brief This method imports an Object from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  bool from_json( const Json::Value& root );

  /**
    This method exports an Object class to an XML file.

    @brief Export an Object to an XML file.
    @overload
    @param[in]    filename              XML filename to which Object class is exported
    @param[in]    object_states_count   number of object_states_to be exported
    @returns                            boolean flag; true if there are no errors
    @throws                             no expected throws
  */
  bool to_xml(const std::string& filename, const int object_states_count = 1) const;

  /**
    This method exports an Object class to an XMLDocument object. The root
    XMLElement* takes the current Object as a child XMLElement.

    @brief Export an Object class to an XMLDocument object.
    @overload
    @param[in]    doc                   XMLDocument object for the current document
    @param[in]    root                  XMLElement* to accept a Object as a child element
    @param[in]    object_states_count   number of object_states_to be exported
    @returns                            none
    @throws                             no expected throws
  */
  void to_xml(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement *root, const int object_states_count = 1) const;

  /**
      This method exports an Object to a json formatted string.

      @brief This method exports an Object to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  std::string to_json( void )const;

  /**
      This method exports an Object to a json value.

      @brief This method exports an Object to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  void to_json( Json::Value& root )const;
  
  /**
      This method exports an Object to an ObjectMessage.

      @brief This method exports an Object to an ObjectMessage.
      @returns                    message version of the Object
      @throws                     no expected throws
  */
  ObjectMessage to_msg( void )const;

  /**
  * Object class ostream operator.
  */
  friend std::ostream& operator<<(std::ostream& out, const Object& other);

  /** std::string to represent the unique id of the object. */ 
  std::string uid;

  /**
    A vector of h2sl::ObjectState that defines the state_history of the object.
    If this vector is empty then no state information is available.
    Currently copied from the ObjectProbabilistic whose properties were sampled.
  */ 
  ObjectState::vectorObjectState state_history;

  /** A map of deterministic properties of the object. */ 
  mapProperties properties;

 protected:
 private:
}; // class Object

}  // namespace h2sl
