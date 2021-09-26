/**
 * @file world-demo.cc
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

#include "h2sl/symbol.h"
#include "h2sl/world.h"

/**
 * World classdemo
 */
int
main( int argc,
      char* argv[] ){
  std::cout << "start of World class demo program" << std::endl;

  boost::program_options::options_description desc{"Options"};
  desc.add_options()("help,h", "Help screen")(
      "input", boost::program_options::value<std::string>(), "input file")(
      "output", boost::program_options::value<std::string>(), "output file");

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  // h2sl::Object object_a( "a", h2sl::Object::vectorObjectState() );
  h2sl::Object object_a( "a", h2sl::ObjectState() );
  h2sl::Object object_b( "b", h2sl::ObjectState() );
  h2sl::World::mapObjects objects;
  objects.insert( std::pair<std::string, h2sl::Object>( object_a.uid, object_a ) );
  objects.insert( std::pair<std::string, h2sl::Object>( object_b.uid, object_b ) );

  h2sl::World world(objects);
  
  if ( vm.count( "input" ) ){
    std::cout << "reading from \"" << vm["input"].as<std::string>() << "\"" << std::endl;
    world.from_xml( vm["input"].as<std::string>() );
  }

  std::cout << "\nworld: " << world << std::endl;

  // testing to-from Json
  std::string msg = world.to_json();
  std::cout << "\nJSON string: " << msg << std::endl;

  h2sl::World world_json = h2sl::World();
  world_json.from_json(msg);
  std::cout << "\nnew world loaded from the json string: " << world_json << std::endl;

  // Test to- and from-message functions of world class and its dependencies

  // Create constituent parts to fill in various ObjectStates
  auto pose123 = std::make_shared<h2sl::Transform3D>( h2sl::Vector3D( 1.0, 2.0, 3.0 ),
      h2sl::UnitQuaternion( h2sl::Vector3D( 1.0, 2.0, 3.0 ), 1.0 ) );
  auto poseI = std::make_shared<h2sl::Transform3D>( h2sl::Vector3D( 0.0, 0.0, 0.0 ),
      h2sl::UnitQuaternion( 0.0, 0.0, 0.0 ) );
  auto bbox = std::make_shared<h2sl::BoundingBox>( h2sl::Vector3D( 2.0, 2.0, 2.0 ), *poseI );
  auto v = std::make_shared<h2sl::Vector3D>( 0.0, 0.0, 5.0 );
  auto w = std::make_shared<h2sl::Vector3D>( 1.0, 1.0, 1.0 );
  auto t = std::make_shared<double>( 5.0 );

  // Create object property maps to test import/export to message
  h2sl::Object::mapProperties properties1;
  properties1.emplace( "object_color", "red" );
  properties1.emplace( "danger", "high" );

  h2sl::Object::mapProperties properties2;
  properties2.emplace( "object_color", "green" );
  properties2.emplace( "danger", "low" );

  h2sl::Object::mapProperties empty_properties;

  // Create a few different ObjectStates to test message inclusion/exclusion of optional members
  h2sl::ObjectState state_full( pose123, bbox, v, w, t );
  h2sl::ObjectState state_pose( poseI, nullptr, nullptr, nullptr, nullptr );
  h2sl::ObjectState state_bbox( nullptr, bbox, nullptr, nullptr, nullptr );

  h2sl::Object object_full( "full", state_full, properties1 );
  h2sl::Object object_pose( "pose", state_pose, properties2 );
  h2sl::Object object_bbox( "bbox", state_bbox, empty_properties );
  h2sl::Object object_empty( "empty", h2sl::ObjectState(), empty_properties );

  h2sl::World::mapObjects objects2;
  objects2.emplace( object_full.uid, object_full );
  objects2.emplace( object_pose.uid, object_pose );
  objects2.emplace( object_bbox.uid, object_bbox );
  objects2.emplace( object_empty.uid, object_empty );

  h2sl::World world2( objects2 );
  h2sl::WorldMessage world_msg = world2.to_msg();
  h2sl::World world_result( world_msg );

  std::cout << "\nStarted with the world:" << std::endl;
  std::cout << world2 << std::endl;
  std::cout << "\nResulting world after converting to message and back:" << std::endl;
  std::cout << world_result << std::endl;
  std::cout << "\nPrinting the message next..." << std::endl;
  std::cin.get();
  
  std::cout << "Exported message:" << std::endl;
  std::cout << world_msg << std::endl;
  
  if ( vm.count( "output" ) ){
    std::cout << "writing world to \"" << vm["output"].as<std::string>() << "\"" << std::endl;
    world.to_xml( vm["output"].as<std::string>() );
  }

  std::cout << "end of World class demo program" << std::endl;
  return 0;
} 
