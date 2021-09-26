/**
 * @file spatial-relation.cc
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

#include <fstream>
#include <sstream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <boost/algorithm/string.hpp>

#include "h2sl/common.h"
#include "h2sl/spatial-relation.h"

namespace h2sl {

//
// This method constructs the relation frame for the given spatial relation elements.
//
Transform3D SpatialRelation::get_relation_frame( const std::optional<ObjectState>& landmark, const std::optional<ObjectState>& viewpoint ){

  Transform3D relation_frame = Transform3D();

  // Fill this in based on which of the landmark/viewpoint we have
  if( landmark && viewpoint ){
    // Case 1: We have a landmark and viewpoint => ternary relative relation
    // We project from Rv to Rl to form the frame's orientation
    // TODO - Handle absolute "world" viewpoint
    relation_frame.position = landmark->pose->position;

    // Find the relation's orientation by finding each x/y/z axis individually
    Vector3D offset = landmark->pose->position - viewpoint->pose->position;
    Vector3D x_axis = offset.unit();

    // The y-axis should be 90 degrees from the x-axis in the global xy-plane
    double x_axis_yaw = 0.0;
    if( x_axis.x() == 0.0 && x_axis.y() == 0.0 ){
      // Facing straight up => Use Viewpoint's orientation for y-axis
      x_axis_yaw = viewpoint->pose->orientation.yaw();
    } else{
      x_axis_yaw = atan2( x_axis.y(), x_axis.x() );
    }
    double y_axis_yaw = x_axis_yaw + M_PI / 2.0; // Add 90 degrees
    Vector3D y_axis = Vector3D( cos( y_axis_yaw ), sin( y_axis_yaw ), 0.0 ).unit();

    // The z-axis should be perpendicular to the x- and y-axis
    Vector3D z_axis = x_axis.cross( y_axis );

    //std::cout << "x-axis found as " << x_axis << std::endl;
    //std::cout << "y-axis found as " << y_axis << std::endl;
    //std::cout << "z-axis found as " << z_axis << std::endl;

    // We construct a quaternion directly from these axes
    // See Equation 2.34 in Siciliano
    double qs_under_root = x_axis.x() + y_axis.y() + z_axis.z() + 1.0;
    double qx_under_root = x_axis.x() - y_axis.y() - z_axis.z() + 1.0;
    double qy_under_root = y_axis.y() - z_axis.z() - x_axis.x() + 1.0;
    double qz_under_root = z_axis.z() - x_axis.x() - y_axis.y() + 1.0;
    //std::cout << "Values under square root are " << qx_under_root << "," << qy_under_root << "," << qz_under_root << std::endl;
    
    // Handle floating-point imprecision by checking for negative square roots
    if( qs_under_root < 0.0 ){ qs_under_root = 0.0; }
    if( qx_under_root < 0.0 ){ qx_under_root = 0.0; }
    if( qy_under_root < 0.0 ){ qy_under_root = 0.0; }
    if( qz_under_root < 0.0 ){ qz_under_root = 0.0; }

    double qs = 0.5 * sqrt( qs_under_root );
    double qx = 0.5 * h2sl::sign( y_axis.z() - z_axis.y() ) * sqrt( qx_under_root );
    double qy = 0.5 * h2sl::sign( z_axis.x() - x_axis.z() ) * sqrt( qy_under_root );
    double qz = 0.5 * h2sl::sign( x_axis.y() - y_axis.x() ) * sqrt( qz_under_root );
    //std::cout << "Found qs:" << qs << " qv=[" << qx << "," << qy << "," << qz << "]" << std::endl;
    
    relation_frame.orientation = UnitQuaternion( Vector3D( qx, qy, qz ), qs );
  } else if( landmark ){
    // Case 2: We have a landmark but no viewpoint => intrinsic relation
    relation_frame = *(landmark->pose);
  } else if( viewpoint ){
    // Case 3: We have a viewpoint but no landmark => binary relative relation
    relation_frame = *(viewpoint->pose);
  } else{ // Cannot occur for valid spatial relations
    std::stringstream error_msg;
    error_msg << "[SpatialRelation Class Error] No landmark nor viewpoint passed to get_relation_frame";
    throw std::runtime_error( error_msg.str() );
  }

  return relation_frame;
}

//
// This method finds the value of the given figure w.r.t. the given frame along the given axis.
//
double SpatialRelation::get_value( const ObjectState& figure, const Transform3D& relation_frame, const char axis ){

  // Find the figure's pose in the relation frame
  Transform3D figure_relative_pose = relation_frame.inverse() * (*figure.pose);
  //std::cout << "Relation frame found to be " << relation_frame << std::endl;
  //std::cout << "Relation frame RPY:" << std::endl;
  //relation_frame.orientation.print_rpy();
  //std::cout << "Figure pose in relative frame found to be " << figure_relative_pose << std::endl;

  // We can now uniformly find the particular axis value the relation cares about
  switch( axis ){
    case 'x':
      return figure_relative_pose.x();
    case 'y':
      return figure_relative_pose.y();
    case 'z':
      return figure_relative_pose.z();
    case 'd':
      return figure_relative_pose.position.norm();
    default: // We should not expect any other axes; throw error
      std::stringstream error_msg;
      error_msg << "[SpatialRelation Class Error] Unexpected axis '" << axis << "' passed to get_value";
      throw std::runtime_error( error_msg.str() );
  }

  return 0.0;
}

//
// This method checks whether the specified binary spatial relation is satisfied.
//
bool SpatialRelation::check_binary_spatial_relation( const ObjectState& figure, const Transform3D& relation_frame,
    const char axis, const char direction ){
  //std::cout << "\nChecking spatial relation:\nFigure: " << figure << std::endl;
  //std::cout << "Relation frame: " << relation_frame << std::endl;
  //std::cout << "Axis: " << axis << " Direction constraint: " << direction << std::endl;

  // Find the figure's value along the spatial relation's specified axis
  double value = get_value( figure, relation_frame, axis );
  
  // Finally, check if the found value obeys the specified direction constraint
  switch( direction ){
    case '+':
      return ( value > 0.0 ); 
    case '-':
      return ( value < 0.0 );
    default: // We should not expect any other direction constraints
      std::stringstream error_msg;
      error_msg << "[SpatialRelation Class Error] Unexpected direction constraint '" << direction << "' passed to check_binary_spatial_relation";
      throw std::runtime_error( error_msg.str() );
  }

  return false;
}

//
// This method returns a vector of all object uids in the world sorted by the given spatial relation.
//
std::vector< std::pair< std::string, double > > SpatialRelation::get_sorted_uids(
    const std::shared_ptr<WorldDCG>& world,
    const Transform3D& relation_frame,
    const char axis,
    const char direction,
    const std::string& extrema,
    const std::optional< std::string >& color_filter,
    const std::optional< std::string >& type_filter ){
  
  // 1. Find the value of each object in the world along the specified spatial relation axis
  auto object_uids = std::vector< std::pair< std::string, double > >();
  for( const auto& it_object : world->objects ){
    // Check that the object has a state with pose
    if( it_object.second.state_history.back().pose == nullptr ){
      continue; // Skip to next object in world
    }

    // Optionally filter the objects by color
    if( color_filter ){
      auto it_color = it_object.second.properties.find( "object_color" );
      if( it_color == it_object.second.properties.end() ) continue;
      if( it_color->second != color_filter.value() ) continue;
    }
    
    // Optionally filter the objects by their type
    if( type_filter ){
      auto it_object_type = it_object.second.properties.find( "object_type" );
      if( it_object_type == it_object.second.properties.end() ) continue;
      if( it_object_type->second != type_filter.value() ) continue;
    }

    // Otherwise extract figure ObjectState and compute desired value
    std::string curr_uid = it_object.first;
    double curr_value = get_value( it_object.second.state_history.back(), relation_frame, axis );
    
    // Negate the value if necessary
    switch( direction ){
      case '+':
        break; // Do nothing; axis is already 'positive'
      case '-':
	curr_value = -curr_value; // Negate the value to account for axis direction
	break;
      default: // We should not expect any other directions
        std::stringstream error_msg;
        error_msg << "[SpatialRelation Class Error] Unexpected direction '" << direction << "' passed to get_sorted_objects";
        throw std::runtime_error( error_msg.str() );
    }

    //std::cout << "Found uid \"" << curr_uid << "\" to have value " << curr_value << " along axis " << axis << direction << std::endl;
    
    // Push a pair onto the vector we're constructing and move to the next object
    object_uids.emplace_back( curr_uid, curr_value );
  }

  // 2. Sort the objects according to the given extrema
  if( extrema == "max" ){
    std::sort( object_uids.begin(), object_uids.end(), sort_max );
  } else if( extrema == "min" ){
    std::sort( object_uids.begin(), object_uids.end(), sort_min );
  } else{
    std::stringstream error_msg;
    error_msg << "[SpatialRelation Class Error] Unexpected extrema \"" << extrema << "\" passed to get_sorted_objects";
    throw std::runtime_error( error_msg.str() );
  }

  return object_uids;
}	

} // namespace h2sl
