/**
 * @file world.h
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

#include "h2sl/object.h"
#include "h2sl/world-probabilistic.h"
#include "h2sl/WorldMessage.h"

namespace h2sl {
/**
 * World class represents a deterministic model of the world.
 */
class World {
 public:

  /**
    Type alias to represent the objects in the world.
  */
  using mapObjects = std::map<std::string, Object>;

  /**
    Default constructor for h2sl::World class.
    @brief Default World class constructor.
    @returns                  none
    @throws                   no expected throws
  */
  World() = default;

  /**
    Parameterized constructor for h2sl::World class.

    @brief Parameterized World class constructor.
    @param[in]  objectsArg    map of objects in the world model
    @returns                  none
    @throws                   no expected throws
  */
  World( const mapObjects& objectsArg );

  /**
    Class constructor for World from xml root element

    @brief Class constructor from xml root element.
    @param[in]    root    xml root element
    @returns              none
    @throws               no expected throws
  */
  World(const tinyxml2::XMLElement* root);

  /**
    Class constructor for World from WorldProbabilistic.
    Samples an Object from each ObjectProbabilistic in the given world.

    @brief Class constructor for World from WorldProbabilistic.
    @param[in]    world    probabilistic world model
    @returns               none
    @throws                no expected throws
  */
  World(const WorldProbabilistic& world);

  /**
    Class constructor for World from WorldMessage message.

    @brief Class constructor from WorldMessage.
    @param[in]  msg   WorldMessage message
    @returns          none
    @throws           no expected throws
  */
  World( const WorldMessage& msg );

  /**
    Default copy constructor for h2sl::World class
  **/
  World( const World & ) = default;

  /**
    Default move constructor for h2sl::World class
  **/
  World( World&& ) = default;

  /**
    World class default destructor
  **/
  virtual ~World() = default;

  /**
    Default copy assignment operator for h2sl::World class
  **/
  World &operator=( const World & ) = default;

  /**
    Default move assignment operator for h2sl::World class
  **/
  World& operator=( World&& ) = default;

  /**
    This method imports a World class from an XML file.

    @brief Import a World class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml(const std::string& filename);

  /**
    This method imports a World class from an XMLElement pointer.

    @brief Import a World class from an XMLElement pointer.
    @overload
    @param[in]    world_elem    XMLElement pointer from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml(const tinyxml2::XMLElement* world_elem);

  /**
      This method imports a World from a json formatted string.

      @brief This method imports a World from a json formatted string.
      @param[in]    json_string   json formatting string to read from
      @returns                    success/failure
      @throws                     no expected throws
  */
  bool from_json( const std::string& json_string );

  /**
      This method imports a World from a json value.

      @brief This method imports a World from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  bool from_json( const Json::Value& root );

  /**
    This method export a World class to an XML file.

    @brief Export a World to an XML file.
    @overload
    @param[in]    filename              XML filename to which World class is exported
    @param[in]    object_states_count   number of object_states_to be exported
    @returns                            boolean flag; true if there are no errors
    @throws                             no expected throws
  */
  bool to_xml(const std::string& filename, const int object_states_count = 1) const;

  /**
    This method exports a World class to an XMLDocument object. The root
    XMLElement* takes the current World as a child XMLElement.

    @brief Export a World class to an XMLDocument object.
    @overload
    @param[in]    document              XMLDocument object for the current document
    @param[in]    root                  XMLElement* to accept a World as a child element
    @param[in]    object_states_count   number of object_states_to be exported
    @returns                            none
    @throws                             no expected throws
  */
  void to_xml(tinyxml2::XMLDocument& document, tinyxml2::XMLElement *root, const int object_states_count = 1) const;

  /**
      This method exports a World to a json formatted string.

      @brief This method exports a World to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  std::string to_json( void )const;

  /**
      This method exports a World to a json value.

      @brief This method exports a World to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  void to_json( Json::Value& root )const;

  /**
      This method exports a World to a WorldMessage.

      @brief This method exports a World to a WorldMessage.
      @returns                message version of the World
      @throws                 no expected throws
  */
  WorldMessage to_msg( void )const;

  /**
  * World class ostream operator
  */
  friend std::ostream& operator<<(std::ostream& out, const World& other);

  /**
   * A std::map to store the objects. Note that a std::map was chosen over the std::unordered_map so that
   * the objects can be sorted in lexicographic (alphabetic) order. This helps with human readability.
   */
  mapObjects objects = mapObjects();


  /**********************************************************************************/
  //  Below are class functionalities for sorting objects according to an enum type.
  /**********************************************************************************/

  /*
    A class enum for different types of sorting. The MIN/MAX prefix indicates the
    direction of the sorted order (MIN means the minimum value is at the front).

    MIN/MAX_*_AXIS: These keys indicate a sort for a particular axis
    MIN/MAX_ABS_*_AXIS: These keys indicate a sort of absolute values for a particular axis
    MIN/MAX_EUCLIDEAN_DISTANCE: This key indicates a sort of distance from the world origin
    MIN/MAX_CENTER_DISTANCE: This key indicates a sort of distance from the average object posisiton.
  */
  enum class SortKey{ MIN_X_AXIS, MAX_X_AXIS,
                      MIN_ABS_X_AXIS, MAX_ABS_X_AXIS,
                      MIN_Y_AXIS, MAX_Y_AXIS,
                      MIN_ABS_Y_AXIS, MAX_ABS_Y_AXIS,
                      MIN_Z_AXIS, MAX_Z_AXIS,
                      MIN_ABS_Z_AXIS, MAX_ABS_Z_AXIS,
                      MIN_EUCLIDEAN_DISTANCE, MAX_EUCLIDEAN_DISTANCE,
                      MIN_CENTER_DISTANCE, MAX_CENTER_DISTANCE,
                      UNKNOWN };

  /* Methods to export/import SortKey types as strings */
  inline static std::string sortkey_to_string( const SortKey& sort_key ){
    std::string ret_string;
    switch( sort_key ){
      case SortKey::MIN_X_AXIS:
        ret_string = "min_x_axis";
        break;
      case SortKey::MAX_X_AXIS:
        ret_string = "max_x_axis";
        break;
      case SortKey::MIN_ABS_X_AXIS:
        ret_string = "min_abs_x_axis";
        break;
      case SortKey::MAX_ABS_X_AXIS:
        ret_string = "max_abs_x_axis";
        break;
      case SortKey::MIN_Y_AXIS:
        ret_string = "min_y_axis";
        break;
      case SortKey::MAX_Y_AXIS:
        ret_string = "max_y_axis";
        break;
      case SortKey::MIN_ABS_Y_AXIS:
        ret_string = "min_abs_y_axis";
        break;
      case SortKey::MAX_ABS_Y_AXIS:
        ret_string = "max_abs_y_axis";
        break;
      case SortKey::MIN_Z_AXIS:
        ret_string = "min_z_axis";
        break;
      case SortKey::MAX_Z_AXIS:
        ret_string = "max_z_axis";
        break;
      case SortKey::MIN_ABS_Z_AXIS:
        ret_string = "min_abs_z_axis";
        break;
      case SortKey::MAX_ABS_Z_AXIS:
        ret_string = "max_abs_z_axis";
        break;
      case SortKey::MIN_EUCLIDEAN_DISTANCE:
        ret_string = "min_euclidean_distance";
        break;
      case SortKey::MAX_EUCLIDEAN_DISTANCE:
        ret_string = "max_euclidean_distance";
        break;
      case SortKey::MIN_CENTER_DISTANCE:
        ret_string = "min_center_distance";
        break;
      case SortKey::MAX_CENTER_DISTANCE:
        ret_string = "max_center_distance";
        break;
      case SortKey::UNKNOWN:
        ret_string = "unknown";
        break;
      default:
        throw std::runtime_error( "Passed sort_key did not match a SortKey type" );
        break;
    }
    return ret_string;
  }
  inline static World::SortKey string_to_sortkey( const std::string& sort_key_string ){
    if( sort_key_string == "min_x_axis" )
      return SortKey::MIN_X_AXIS;
    if( sort_key_string == "max_x_axis" )
      return SortKey::MAX_X_AXIS;
    if( sort_key_string == "min_abs_x_axis" )
      return SortKey::MIN_ABS_X_AXIS;
    if( sort_key_string == "max_abs_x_axis" )
      return SortKey::MAX_ABS_X_AXIS;
    if( sort_key_string == "min_y_axis" )
      return SortKey::MIN_Y_AXIS;
    if( sort_key_string == "max_y_axis" )
      return SortKey::MAX_Y_AXIS;
    if( sort_key_string == "min_abs_y_axis" )
      return SortKey::MIN_ABS_Y_AXIS;
    if( sort_key_string == "max_abs_y_axis" )
      return SortKey::MAX_ABS_Y_AXIS;
    if( sort_key_string == "min_z_axis" )
      return SortKey::MIN_Z_AXIS;
    if( sort_key_string == "max_z_axis" )
      return SortKey::MAX_Z_AXIS;
    if( sort_key_string == "min_abs_z_axis" )
      return SortKey::MIN_ABS_Z_AXIS;
    if( sort_key_string == "max_abs_z_axis" )
      return SortKey::MAX_ABS_Z_AXIS;
    if( sort_key_string == "min_euclidean_distance" )
      return SortKey::MIN_EUCLIDEAN_DISTANCE;
    if( sort_key_string == "max_euclidean_distance" )
      return SortKey::MAX_EUCLIDEAN_DISTANCE;
    if( sort_key_string == "min_center_distance" )
      return SortKey::MIN_CENTER_DISTANCE;
    if( sort_key_string == "max_center_distance" )
      return SortKey::MAX_CENTER_DISTANCE;
    if( sort_key_string == "unknown" )
      return SortKey::UNKNOWN;

    throw std::runtime_error( "Passed sort_key_string did not match a SortKey type" );
  }

  /* A method to return a sorted vector of world objects per a SortKey value */
  const std::map< std::string, std::vector< std::shared_ptr<Object> > >
  sort_objects( const SortKey sort_key )const;

  /* A set of boolean comparison functions for objects to be used by std::sort() */

  // XYZ sorting methods
  // minimum xyz sorts
  inline static bool sort_min_x_axis( const std::shared_ptr<Object>& lhs,
                                      const std::shared_ptr<Object>& rhs )
  {
    return (lhs->state_history.back().pose->x() < rhs->state_history.back().pose->x());
  };
  inline static bool sort_min_y_axis( const std::shared_ptr<Object>& lhs,
                                      const std::shared_ptr<Object>& rhs )
  {
    return (lhs->state_history.back().pose->y() < rhs->state_history.back().pose->y());
  };
  inline static bool sort_min_z_axis( const std::shared_ptr<Object>& lhs,
                                      const std::shared_ptr<Object>& rhs )
  {
    return (lhs->state_history.back().pose->z() < rhs->state_history.back().pose->z());
  };
  // maximum xyz sorts
  inline static bool sort_max_x_axis( const std::shared_ptr<Object>& lhs,
                                      const std::shared_ptr<Object>& rhs )
  {
    return (lhs->state_history.back().pose->x() > rhs->state_history.back().pose->x());
  };
  inline static bool sort_max_y_axis( const std::shared_ptr<Object>& lhs,
                                      const std::shared_ptr<Object>& rhs )
  {
    return (lhs->state_history.back().pose->y() > rhs->state_history.back().pose->y());
  };
  inline static bool sort_max_z_axis( const std::shared_ptr<Object>& lhs,
                                      const std::shared_ptr<Object>& rhs )
  {
    return (lhs->state_history.back().pose->z() > rhs->state_history.back().pose->z());
  };

  // Absolute Value XYZ sorting methods
  // minimum abs xyz sorts
  inline static bool sort_min_abs_x_axis( const std::shared_ptr<Object>& lhs,
                                          const std::shared_ptr<Object>& rhs )
  {
    return ( fabs( lhs->state_history.back().pose->x() )
              < fabs( rhs->state_history.back().pose->x() ) );
  };
  inline static bool sort_min_abs_y_axis( const std::shared_ptr<Object>& lhs,
                                          const std::shared_ptr<Object>& rhs )
  {
    return ( fabs( lhs->state_history.back().pose->y() )
              < fabs( rhs->state_history.back().pose->y() ) );
  };
  inline static bool sort_min_abs_z_axis( const std::shared_ptr<Object>& lhs,
                                          const std::shared_ptr<Object>& rhs )
  {
    return ( fabs( lhs->state_history.back().pose->z() )
              < fabs( rhs->state_history.back().pose->z() ) );
  };
  // maximum abs xyz sorts
  inline static bool sort_max_abs_x_axis( const std::shared_ptr<Object>& lhs,
                                          const std::shared_ptr<Object>& rhs )
  {
    return ( fabs( lhs->state_history.back().pose->x() )
              > fabs( rhs->state_history.back().pose->x() ) );
  };
  inline static bool sort_max_abs_y_axis( const std::shared_ptr<Object>& lhs,
                                          const std::shared_ptr<Object>& rhs )
  {
    return ( fabs( lhs->state_history.back().pose->y() )
              > fabs( rhs->state_history.back().pose->y() ) );
  };
  inline static bool sort_max_abs_z_axis( const std::shared_ptr<Object>& lhs,
                                          const std::shared_ptr<Object>& rhs )
  {
    return ( fabs( lhs->state_history.back().pose->z() )
              > fabs( rhs->state_history.back().pose->z() ) );
  };

  // Euclidean Distance sorting methods
  // minimum sort
  inline static bool sort_min_euclidean_distance( const std::shared_ptr<Object>& lhs,
                                                  const std::shared_ptr<Object>& rhs )
  {
    return( lhs->state_history.back().pose->position.norm()
            < rhs->state_history.back().pose->position.norm() );
  }
  // maximum sort
  inline static bool sort_max_euclidean_distance( const std::shared_ptr<Object>& lhs,
                                                  const std::shared_ptr<Object>& rhs )
  {
    return( lhs->state_history.back().pose->position.norm()
            > rhs->state_history.back().pose->position.norm() );
  }

 protected:
 private:

}; // class World



/*********************************************************************************/
/******************************** WorldDCG Header ********************************/
/*********************************************************************************/

/**
  WorldDCG class definition. A derived class of h2sl::World that provides storage
  for sorted objects per the World::SortKey enum.
**/
class WorldDCG : public World {

  public:

  /**
    Type alias to refer to a map of map of vectors of objects
    Key: a World::SortKey
    Value: map of objects categorized based on the object types
  **/
  using mapSortKeyMapObjects = std::map< World::SortKey, std::map< std::string, std::vector< std::shared_ptr<Object> > > >;

  /**
    WorldDCG default class constructor
  **/
  WorldDCG() = default;

  /**
    WorldDCG constructor from a World object
  **/
  WorldDCG( const World& base_world );

  /**
    Default copy constructor for WorldDCG
  **/
  WorldDCG( const WorldDCG& other ) = default;

  /**
    Default move constructor for WorldDCG
  **/
  WorldDCG( WorldDCG&& other ) = default;

  /**
    Default destructor for WorldDCG
  **/
  virtual ~WorldDCG() = default;

  /**
    Default copy assignment operator for WorldDCG
  **/
  WorldDCG& operator=( const WorldDCG& other ) = default;

  /**
    Default move assignment operator for WorldDCG
  **/
  WorldDCG& operator=( WorldDCG&& other ) = default;

  /**
    This method provides read-only access to the _sorted_objects member
  **/
  inline const mapSortKeyMapObjects&
  sorted_objects( void )const{ return _sorted_objects; }

  protected:
  private:
  /**
    This method fills the _sorted_objects member

    @brief Fill the _sorted_objects member
    @returns              None - Modifies the content of the _sorted_objects member
    @throws               No expected throws
  **/
  virtual void _fill_sorted_objects( void );

  /**
    A member to hold sorted versions of the world to be used during evaluation
    Key: a World::SortKey enum type
    Value: a vector of sorted worlds objects per the Key sort type
  **/
  mapSortKeyMapObjects _sorted_objects = mapSortKeyMapObjects();

}; // Class WorldDCG

}  // namespace h2sl

