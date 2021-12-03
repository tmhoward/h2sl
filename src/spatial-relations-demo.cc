/**
 * @file spatial-relations-demo.cc
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

#include <boost/program_options.hpp>

#include "h2sl/common.h"
#include "h2sl/world.h"
#include "h2sl/spatial-relation.h"

//
// Returns a string representation of the given std::vector< std::pair< std::string, double > >
//
std::string str_double_vector_to_str( const std::vector< std::pair< std::string, double > >& v ){
  std::stringstream output;
  output << "[";
  for( int i = 0; i < v.size() - 1; i++ ){
    output << v[i].first << ",";
  }
  if( v.size() - 1 >= 0 ){
    output << v[v.size() - 1].first;
  }
  output << "]";
  return output.str();
}

int main( int argc, char* argv[] ){
  std::cout << "\nStart of the spatial relations demo program" << std::endl << std::endl;

  boost::program_options::options_description desc("Options");
  desc.add_options()("help,h", "Help Screen")
    ("world,w", boost::program_options::value<std::string>(), "world file");
  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if( vm.count("help") ){
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  if( !vm.count( "world" ) ){
    std::cout << "No world file provided, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }

  // Load the world
  std::shared_ptr<h2sl::World> world  = std::make_shared<h2sl::World>();
  bool world_import_success = world->from_xml( vm["world"].as<std::string>().c_str() );

  if( world_import_success ){
    std::cout << "\nworld:" << *world << std::endl << std::endl;
  } else {
    std::cout << "Failed to load world" << std::endl;
    return EXIT_FAILURE;
  }

  // Extract ObjectStates from the world to allow easy construction of examples
  std::cout << "Extracting object states from the world..." << std::endl;
  auto human = std::make_optional<h2sl::ObjectState>( world->objects.find( "human" )->second.state_history.back() );
  auto robot = std::make_optional<h2sl::ObjectState>( world->objects.find( "robot" )->second.state_history.back() );
  auto b1 = std::make_optional<h2sl::ObjectState>( world->objects.find( "b1" )->second.state_history.back() );
  auto p1 = std::make_optional<h2sl::ObjectState>( world->objects.find( "p1" )->second.state_history.back() );
  auto p2 = std::make_optional<h2sl::ObjectState>( world->objects.find( "p2" )->second.state_history.back() );
  auto g1 = std::make_optional<h2sl::ObjectState>( world->objects.find( "g1" )->second.state_history.back() );
  auto g3 = std::make_optional<h2sl::ObjectState>( world->objects.find( "g3" )->second.state_history.back() );
  auto u2 = std::make_optional<h2sl::ObjectState>( world->objects.find( "u2" )->second.state_history.back() );
  auto o1 = std::make_optional<h2sl::ObjectState>( world->objects.find( "o1" )->second.state_history.back() );
  auto o2 = std::make_optional<h2sl::ObjectState>( world->objects.find( "o2" )->second.state_history.back() );
  auto y = std::make_optional<h2sl::ObjectState>( world->objects.find( "y" )->second.state_history.back() );
  auto r = std::make_optional<h2sl::ObjectState>( world->objects.find( "r" )->second.state_history.back() );

  // Arguments of check_spatial_relation are:
  //          figure, landmark, viewpoint, axis, direction

  std::cout << "Testing binary spatial relations:" << std::endl;

  // Example 1 - Left of u2 (human) => landmark u2, viewpoint human, axis y, direction +
  h2sl::Transform3D rf1 = h2sl::SpatialRelation::get_relation_frame( u2, human );
  // p1 should give true
  bool example1_t = h2sl::SpatialRelation::check_binary_spatial_relation( *p1, rf1, 'y', '+' );
  // g1 should give false
  bool example1_f = h2sl::SpatialRelation::check_binary_spatial_relation( *g1, rf1, 'y', '+' );
  std::cout << "Example 1: " << example1_t << " " << example1_f << std::endl << std::endl;

  // Example 2 - Behind o2 (robot) => landmark o2, viewpoint robot, axis x, direction +
  h2sl::Transform3D rf2 = h2sl::SpatialRelation::get_relation_frame( o2, robot );
  // g3 should give true
  bool example2_t = h2sl::SpatialRelation::check_binary_spatial_relation( *g3, rf2, 'x', '+' );
  // o1 should give false
  bool example2_f = h2sl::SpatialRelation::check_binary_spatial_relation( *o1, rf2, 'x', '+' );
  std::cout << "Example 2: " << example2_t << " " << example2_f << std::endl << std::endl;

  // Example 3 - My right (robot) => no landmark, viewpoint robot, axis y, direction -
  h2sl::Transform3D rf3 = h2sl::SpatialRelation::get_relation_frame( std::nullopt, robot );
  // p1 should give true
  bool example3_t = h2sl::SpatialRelation::check_binary_spatial_relation( *p1, rf3, 'y', '-' );
  // p2 should give false
  bool example3_f = h2sl::SpatialRelation::check_binary_spatial_relation( *p2, rf3, 'y', '-' );
  std::cout << "Example 3: " << example3_t << " " << example3_f << std::endl << std::endl;

  // Example 4 - Below o2 (human) => landmark o2, viewpoint human, axis z, direction -
  h2sl::Transform3D rf4 = h2sl::SpatialRelation::get_relation_frame( o2, human );
  // g3 should give true
  bool example4_t = h2sl::SpatialRelation::check_binary_spatial_relation( *g3, rf4, 'z', '-' );
  // g1 should give false
  bool example4_f = h2sl::SpatialRelation::check_binary_spatial_relation( *g1, rf4, 'z', '-' );
  std::cout << "Example 4: " << example4_t << " " << example4_f << std::endl << std::endl;

  // Example 5 - Right of u2 (p2) => landmark u2, viewpoint p2, axis y, direction -
  h2sl::Transform3D rf5 = h2sl::SpatialRelation::get_relation_frame( u2, p2 );
  // y should give true
  bool example5_t = h2sl::SpatialRelation::check_binary_spatial_relation( *y, rf5, 'y', '-' );
  // human should give false
  bool example5_f = h2sl::SpatialRelation::check_binary_spatial_relation( *human, rf5, 'y', '-' );
  std::cout << "Example 5: " << example5_t << " " << example5_f << std::endl << std::endl;

  // Example 6 - Behind o2 (u2) => landmark o2, viewpoint u2, axis x, direction +
  h2sl::Transform3D rf6 = h2sl::SpatialRelation::get_relation_frame( o2, u2 );
  // y should give true
  bool example6_t = h2sl::SpatialRelation::check_binary_spatial_relation( *y, rf6, 'x', '+' );
  // b1 should give false
  bool example6_f = h2sl::SpatialRelation::check_binary_spatial_relation( *b1, rf6, 'x', '+' );
  std::cout << "Example 6: " << example6_t << " " << example6_f << std::endl << std::endl;

  // Example 7 - Above g3 (robot) => landmark g3, viewpoint robot, axis z, direction +
  h2sl::Transform3D rf7 = h2sl::SpatialRelation::get_relation_frame( g3, robot );
  // human should give true
  bool example7_t = h2sl::SpatialRelation::check_binary_spatial_relation( *human, rf7, 'z', '+' );
  // p1 should give false
  bool example7_f = h2sl::SpatialRelation::check_binary_spatial_relation( *p1, rf7, 'z', '+' );
  std::cout << "Example 7: " << example7_t << " " << example7_f << std::endl << std::endl;

  std::cout << "Converting World to WorldDCG..." << std::endl;
  std::shared_ptr<h2sl::WorldDCG> world_dcg = std::make_shared< h2sl::WorldDCG >( *world );

  std::cout << "Testing ordinal spatial relations:" << std::endl;

  // Example 1 - Closest to u2 => landmark u2, no viewpoint, axis d-, extrema max
  std::cout << "\nSorting items by \"Closest to u2\"" << std::endl;
  h2sl::Transform3D lm_u2_rf = h2sl::SpatialRelation::get_relation_frame( u2, std::nullopt );
  auto sort_vec1 = h2sl::SpatialRelation::get_sorted_uids( world_dcg, lm_u2_rf,
    'd', '-', "max", std::nullopt, std::nullopt );
  std::cout << str_double_vector_to_str( sort_vec1 ) << std::endl;

  // Example 2 - Least far from u2 => landmark u2, no viewpoint, axis d+, extrema min
  std::cout << "\nSorting items by \"Least far from u2\"" << std::endl;
  auto sort_vec2 = h2sl::SpatialRelation::get_sorted_uids( world_dcg, lm_u2_rf,
    'd', '+', "min", std::nullopt, std::nullopt );
  std::cout << str_double_vector_to_str( sort_vec2 ) << std::endl;

  // Example 3 - Leftmost block (robot) => No landmark, viewpoint robot, axis y+, extrema max, type block
  std::cout << "\nSorting items by \"Leftmost block (robot)\"" << std::endl;
  h2sl::Transform3D vp_robot_rf = h2sl::SpatialRelation::get_relation_frame( std::nullopt, robot );
  auto block_type = std::make_optional< std::string >( "block" );
  auto sort_vec3 = h2sl::SpatialRelation::get_sorted_uids( world_dcg, vp_robot_rf,
    'y', '+', "max", std::nullopt, block_type );
  std::cout << str_double_vector_to_str( sort_vec3 ) << std::endl;

  // Example 4 - Least right block (robot) => No landmark, viewpoint robot, axis y-, extrema min, type block
  std::cout << "\nSorting items by \"Least right block (robot)\"" << std::endl;
  auto sort_vec4 = h2sl::SpatialRelation::get_sorted_uids( world_dcg, vp_robot_rf,
    'y', '-', "min", std::nullopt, block_type );
  std::cout << str_double_vector_to_str( sort_vec4 ) << std::endl;

  // Example 5 - Least left block (human) => No landmark, viewpoint human, axis y+, extrema min, type block
  std::cout << "\nSorting items by \"Least left block (human)\"" << std::endl;
  h2sl::Transform3D vp_human_rf = h2sl::SpatialRelation::get_relation_frame( std::nullopt, human );
  auto sort_vec5 = h2sl::SpatialRelation::get_sorted_uids( world_dcg, vp_human_rf,
    'y', '+', "min", std::nullopt, block_type );
  std::cout << str_double_vector_to_str( sort_vec5 ) << std::endl;

  // Example 6 - Frontmost (human) => No landmark, viewpoint human, axis x-, extrema max
  std::cout << "\nSorting items by \"Frontmost (human)\"" << std::endl;
  auto sort_vec6 = h2sl::SpatialRelation::get_sorted_uids( world_dcg, vp_human_rf,
    'x', '-', "max", std::nullopt, std::nullopt );
  std::cout << str_double_vector_to_str( sort_vec6 ) << std::endl;

  // Example 7 - Backmost (robot) => No landmark, viewpoint robot, axis x+, extrema max
  std::cout << "\nSorting items by \"Backmost (robot)\"" << std::endl;
  auto sort_vec7 = h2sl::SpatialRelation::get_sorted_uids( world_dcg, vp_robot_rf,
    'x', '+', "max", std::nullopt, std::nullopt );
  std::cout << str_double_vector_to_str( sort_vec7 ) << std::endl;

  // Example 8 - Farthest from r => landmark r, no viewpoint, axis d+, extrema max
  std::cout << "\nSorting items by \"Farthest from r\"" << std::endl;
  h2sl::Transform3D lm_r_rf = h2sl::SpatialRelation::get_relation_frame( r, std::nullopt );
  auto sort_vec8 = h2sl::SpatialRelation::get_sorted_uids( world_dcg, lm_r_rf,
    'd', '+', "max", std::nullopt, std::nullopt );
  std::cout << str_double_vector_to_str( sort_vec8 ) << std::endl;

  // Example 9 - Blue farthest from r => landmark r, no viewpoint, axis d+, extrema max, color blue
  std::cout << "\nSorting items by \"Blue farthest from r\"" << std::endl;
  auto blue = std::make_optional< std::string >( "blue" );
  auto sort_vec9 = h2sl::SpatialRelation::get_sorted_uids( world_dcg, lm_r_rf,
    'd', '+', "max", blue, std::nullopt );
  std::cout << str_double_vector_to_str( sort_vec9 ) << std::endl;

  // Example 10 - Green least far from p2 => landmark p2, no viewpoint, axis d+, extrema min, color green
  std::cout << "\nSorting items by \"Green least far from p2\"" << std::endl;
  h2sl::Transform3D lm_p2_rf = h2sl::SpatialRelation::get_relation_frame( p2, std::nullopt );
  auto green = std::make_optional< std::string >( "green" );
  auto sort_vec10 = h2sl::SpatialRelation::get_sorted_uids( world_dcg, lm_p2_rf,
    'd', '+', "min", green, std::nullopt );
  std::cout << str_double_vector_to_str( sort_vec10 ) << std::endl;

  std::cout << "End of the spatial relations demo program\n" << std::endl;
  return EXIT_SUCCESS;
}
