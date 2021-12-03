/**
 * @file object-probabilistic.h
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

#include "objectstate.h"

namespace h2sl {
/**
  A class to represent ObjectProbabilistic in h2sl
*/
class ObjectProbabilistic {
 public:

  /**
    Type alias to represent a distribution over the semantic properties of the object.
  */
  using mapPropertyDistributions = std::map<std::string, std::map<std::string, double>>;

  /**
    ObjectProbabilistic default class constructor.

    @brief Default ObjectProbabilistic class constructor.
    @param[in] uidArg                     unique identifier of object
    @param[in] stateHistoryArg            state_history of the object
    @param[in] propertyDistributionsArg   uncertain properties
    @returns                              none
    @throws                               no expected throws
  */
  ObjectProbabilistic( const std::string& uidArg = "",
          const ObjectState::vectorObjectState& stateHistoryArg =
	      ObjectState::vectorObjectState(),
          const mapPropertyDistributions& propertyDistributionsArg =
              mapPropertyDistributions() );

  /**
    Class constructor for ObjectProbabilistic with static object state.

    @brief Class constructor for ObjectProbabilistic with static object state.
    @param[in] uid                        unique identifier of object
    @param[in] objectStateArg             state of the object (assumed static)
    @param[in] propertyDistributionsArg   uncertain properties
    @returns                              none
    @throws                               no expected throws
  */
  ObjectProbabilistic( const std::string& uidArg,
          const ObjectState& objectStateArg,
          const mapPropertyDistributions& propertyDistributionsArg =
             mapPropertyDistributions() );

  /**
    Class constructor for ObjectProbabilistic from xml root element

    @brief Class constructor from xml root element.
    @param[in] root   xml root element
    @returns          none
    @throws           no expected throws
  */
  ObjectProbabilistic( const tinyxml2::XMLElement *root );

  /**
    Default copy constructor for h2sl::ObjectProbabilistic class
  **/
  ObjectProbabilistic( const ObjectProbabilistic & ) = default;

  /**
    Default move constructor for h2sl::ObjectProbabilistic class
  **/

  ObjectProbabilistic( ObjectProbabilistic&& ) = default;

  /**
    ObjectProbabilistic class default destructor.
  **/
  virtual ~ObjectProbabilistic() = default;

  /**
    Default copy assignment operator for h2sl::ObjectProbabilistic class
  **/
  ObjectProbabilistic &operator=( const ObjectProbabilistic & ) = default;

  /**
    Default move assignment operator for h2sl::ObjectProbabilistic class
  **/
  ObjectProbabilistic& operator=( ObjectProbabilistic&& ) = default;

  /**
    Checks whether the ObjectProbabilistic's semantic property distributions are valid.

    @brief Checks whether the ObjectProbabilistic's semantic property distributions are valid.
    @returns           boolean flag; true if all distributions sum to one
    @throws            no expected throws
  */
  bool property_distributions_valid( void )const;

  /**
    This method imports an ObjectProbabilistic class from an XML file.

    @brief Import an ObjectProbabilistic class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml(const std::string& filename);

  /**
    This method imports an ObjectProbabilistic class from an XMLElement pointer.

    @brief Import an ObjectProbabilistic class from an XMLElement pointer.
    @overload
    @param[in]    filename      filename of the XMLElement pointer from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml(const tinyxml2::XMLElement *root);

  /**
      This method imports an ObjectProbabilistic from a json formatted string.

      @brief This method imports an ObjectProbabilistic from a json formatted string.
      @param[in]    json_string   json formatted string to read from
      @returns                    success/failure
      @throws                     no expected throws
  */
  bool from_json( const std::string& json_string );

  /**
      This method imports an ObjectProbabilistic from a json value.

      @brief This method imports an ObjectProbabilistic from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  bool from_json( const Json::Value& root );

  /**
    This method exports an ObjectProbabilistic class to an XML file.

    @brief Export an ObjectProbabilistic to an XML file.
    @overload
    @param[in]    filename              XML filename to which ObjectProbabilistic class is exported
    @param[in]    object_states_count   number of object_states_to be exported
    @returns                            boolean flag; true if there are no errors
    @throws                             no expected throws
  */
  bool to_xml(const std::string& filename, const int object_states_count = 1) const;

  /**
    This method exports an ObjectProbabilistic class to an XMLDocument object. The root
    XMLElement* takes the current ObjectProbabilistic as a child XMLElement.

    @brief Export an ObjectProbabilistic class to an XMLDocument object.
    @overload
    @param[in]    doc                   XMLDocument object for the current document
    @param[in]    root                  XMLElement* to accept an ObjectProbabilistic as a child element
    @param[in]    object_states_count   number of object_states_to be exported
    @returns                            none
    @throws                             no expected throws
  */
  void to_xml(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement *root, const int object_states_count = 1) const;

  /**
      This method exports an ObjectProbabilistic to a json formatted string.

      @brief This method exports an ObjectProbabilistic to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  std::string to_json( void )const;

  /**
      This method exports an ObjectProbabilistic to a json value.

      @brief This method exports an ObjectProbabilistic to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  void to_json( Json::Value& root )const;

  /**
  * ObjectProbabilistic class ostream operator.
  */
  friend std::ostream& operator<<(std::ostream& out, const ObjectProbabilistic& other);

  /** std::string to represent the unique id of the object. */
  std::string uid;

  /**
   * A vector of h2sl::ObjectState that defines the state_history of the object.
   * If this vector is empty then no state information is available.
   */
  ObjectState::vectorObjectState state_history;

  /**
   * A map from property names to probability distributions over their possible values.
   * Each probability distribution maps (std::string) values to (double) probabilities.
   */
  mapPropertyDistributions property_distributions;

  protected:
  private:
}; // class ObjectProbabilistic

}  // namespace h2sl
