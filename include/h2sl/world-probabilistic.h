/**
 * @file world-probabilistic.h
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
#include <map>
#include <vector>

#include <tinyxml2.h>
#include <jsoncpp/json/value.h>

#include "h2sl/object-probabilistic.h"

namespace h2sl {
/**
 * WorldProbabilistic class represents a probabilistic model of the world.
 */
class WorldProbabilistic {
 public:

  /**
    Type alias to represent the probabilistic objects in the world.
  */
  using mapProbObjects = std::map<std::string, ObjectProbabilistic>;

  /**
    Default constructor for h2sl::WorldProbabilistic class.
    @brief Default WorldProbabilistic class constructor.
    @returns                  none
    @throws                   no expected throws
  */
  WorldProbabilistic() = default;

  /**
    Parameterized constructor for h2sl::WorldProbabilistic class.

    @brief Parameterized WorldProbabilistic class constructor.
    @returns                  none
    @throws                   no expected throws
  */
  WorldProbabilistic( const mapProbObjects& objectsArg );

  /**
    Class constructor for WorldProbabilistic from xml root element

    @brief Class constructor from xml root element.
    @param[in]    root    xml root element
    @returns              none
    @throws               no expected throws
  */
  WorldProbabilistic(const tinyxml2::XMLElement* root);

  /**
    Default copy constructor for h2sl::WorldProbabilistic class
  **/
  WorldProbabilistic( const WorldProbabilistic & ) = default;

  /**
    Default move constructor for h2sl::WorldProbabilistic class
  **/
  WorldProbabilistic( WorldProbabilistic&& ) = default;

  /**
    WorldProbabilistic class default destructor
  **/
  virtual ~WorldProbabilistic() = default;

  /**
    Default copy assignment operator for h2sl::WorldProbabilistic class
  **/
  WorldProbabilistic &operator=( const WorldProbabilistic & ) = default;

  /**
    Default move assignment operator for h2sl::WorldProbabilistic class
  **/
  WorldProbabilistic& operator=( WorldProbabilistic&& ) = default;

  /**
    This method imports a WorldProbabilistic class from an XML file.

    @brief Import a WorldProbabilistic class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml(const std::string& filename);

  /**
    This method imports a WorldProbabilistic class from an XMLElement pointer.

    @brief Import a WorldProbabilistic class from an XMLElement pointer.
    @overload
    @param[in]    world_elem    XMLElement pointer from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml(const tinyxml2::XMLElement* world_elem);

  /**
      This method imports a WorldProbabilistic from a json formatted string.

      @brief This method imports a WorldProbabilistic from a json formatted string.
      @param[in]    json_string   json formatting string to read from
      @returns                    success/failure
      @throws                     no expected throws
  */
  bool from_json( const std::string& json_string );

  /**
      This method imports a WorldProbabilistic from a json value.

      @brief This method imports a WorldProbabilistic from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  bool from_json( const Json::Value& root );

  /**
    This method exports a WorldProbabilistic class to an XML file.

    @brief Export a WorldProbabilistic to an XML file.
    @overload
    @param[in]    filename              XML filename to which WorldProbabilistic class is exported
    @param[in]    object_states_count   number of object states to be exported
    @returns                            boolean flag; true if there are no errors
    @throws                             no expected throws
  */
  bool to_xml(const std::string& filename, const int object_states_count = 1) const;

  /**
    This method exports a WorldProbabilistic class to an XMLDocument object. The root
    XMLElement* takes the current WorldProbabilistic as a child XMLElement.

    @brief Export a WorldProbabilistic class to an XMLDocument object.
    @overload
    @param[in]    document              XMLDocument object for the current document
    @param[in]    root                  XMLElement* to accept a WorldProbabilistic as a child element
    @param[in]    object_states_count   number of object states to be exported
    @returns                            none
    @throws                             no expected throws
  */
  void to_xml(tinyxml2::XMLDocument& document, tinyxml2::XMLElement *root, const int object_states_count = 1) const;

  /**
      This method exports a WorldProbabilistic to a json formatted string.

      @brief This method exports a WorldProbabilistic to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  std::string to_json( void )const;

  /**
      This method exports a WorldProbabilistic to a json value.

      @brief This method exports a WorldProbabilistic to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  void to_json( Json::Value& root )const;

  /**
  * WorldProbabilistic class ostream operator
  */
  friend std::ostream& operator<<(std::ostream& out, const WorldProbabilistic& other);

  /**
   * A std::map to store the probabilistic objects. A std::map was chosen over a std::unordered_map so that
   * the objects can be sorted in lexicographic (alphabetic) order. This helps with human readability.
   */
  mapProbObjects objects = mapProbObjects();

}; // class WorldProbabilistic

}  // namespace h2sl

