/**
 * @file spatial-relation.h
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
#include <tinyxml2.h>
#include <jsoncpp/json/value.h>

#include "h2sl/object.h"
#include "h2sl/world.h"

namespace h2sl {
  /**
   * SpatialRelation class definition
   */
  class SpatialRelation {
  public:

  /**
      This method constructs the relation frame for the given spatial relation elements.

      @brief This method constructs the relation frame for the given spatial relation elements.
      @param[in]    landmark      optional landmark reference frame for the spatial relation
      @param[in]    viewpoint     optional viewpoint reference frame for the spatial relation
      @returns      Transform3D   constructed spatial relation reference frame
      @throws                     if neither a landmark nor viewpoint is provided, throws std::runtime_error
  */
  static Transform3D get_relation_frame( const std::optional<ObjectState>& landmark, const std::optional<ObjectState>& viewpoint );

  /**
      This method finds the value of the given figure w.r.t. the given frame along the given axis.

      @brief This method finds the value of the given figure w.r.t. the given frame along the given axis.
      @param[in]    figure          figure to be compared with the relation frame
      @param[in]    relation_frame  frame of the spatial relation
      @param[in]    axis            axis along which the figure's value is computed (x,y,z,d)
      @returns      double          position of the figure along the axis
      @throws                       if axis is not recognized, throws std::runtime_error
  */
  static double get_value( const ObjectState& figure, const Transform3D& relation_frame, const char axis );

  /**
      This method checks whether the specified binary spatial relation is satisfied.

      @brief This method checks whether the specified binary spatial relation is satisfied.
      @param[in]    figure          figure reference frame for the spatial relation
      @param[in]    relation_frame  reference frame of the spatial relation
      @param[in]    axis            axis of interest for the spatial relation (x,y,z,d)
      @param[in]    direction       direction along the axis for the spatial relation (+,-)
      @returns      bool            true/false satisfied relation
      @throws                       if spatial relation is invalid, throws std::runtime_error
  */
  static bool check_binary_spatial_relation( const ObjectState& figure,
    const Transform3D& relation_frame,
    const char axis,
    const char direction );

  /**
      This method returns a vector of all object uids in the world sorted by the given spatial relation.
      The returned vector can be filtered to contain only objects with a certain color.

      @brief This method returns a vector of all object uids in the world sorted by the given spatial relation.
      @param[in]    world           world model to be sorted
      @param[in]    relation_frame  reference frame of the spatial relation
      @param[in]    axis            axis of interest for the spatial relation (x,y,z,d)
      @param[in]    direction       direction along the axis for the spatial relation (+,-)
      @param[in]    extrema         sorting order for the spatial relation (max,min)
      @param[in]    color_filter    optional color used to filter included objects
      @param[in]    type_filter     optional object type used to filter included objects
      @returns      std::vector     sorted and optionally filtered vector of (uid,value) pairs
      @throws                       if spatial relation is not valid, throws std::runtime_error
  */
  static std::vector< std::pair< std::string, double > > get_sorted_uids( const std::shared_ptr<WorldDCG>& world,
    const Transform3D& relation_frame,
    const char axis,
    const char direction,
    const std::string& extrema,
    const std::optional< std::string >& color_filter,
    const std::optional< std::string >& type_filter );

  protected:

  inline static bool sort_max( const std::pair< std::string, double >& lhs, const std::pair< std::string, double >& rhs ){
    return (lhs.second > rhs.second);
  };
  
  inline static bool sort_min( const std::pair< std::string, double >& lhs, const std::pair< std::string, double >& rhs ){
    return (lhs.second < rhs.second);
  };

  private:
    // Disallow creating an instance of this class
    SpatialRelation(){}
  };
} // namespace h2sl
