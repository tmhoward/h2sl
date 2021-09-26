/**
 * @file object-probabilistic-demo.cc
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

#include "h2sl/object.h"
#include "h2sl/object-probabilistic.h"

/**
 * ObjectProbabilistic class demo
 */
int main(int argc, char *argv[]) {
  std::cout << "start of ObjectProbabilistic class demo program" << std::endl;

  boost::program_options::options_description desc{"Options"};
  desc.add_options()("help,h", "Help screen")(
      "input", boost::program_options::value<std::string>(), "input file")(
      "output", boost::program_options::value<std::string>(), "output file");

  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  // Create two poses
  auto pose_1 = std::make_shared< h2sl::Transform3D >( h2sl::Transform3D( h2sl::Vector3D(0.0,0.0,0.0), h2sl::UnitQuaternion( h2sl::Vector3D(0.707,0.0,0.0), 0.707 ) ) );
  auto pose_2 = std::make_shared< h2sl::Transform3D >( h2sl::Transform3D( h2sl::Vector3D(0.1,0.0,0.0), h2sl::UnitQuaternion( h2sl::Vector3D(0.707,0.0,0.0), 0.707 ) ) );

  // Create two bounding boxes
  auto bbox_1 = std::make_shared< h2sl::BoundingBox >( h2sl::BoundingBox( h2sl::Vector3D(10, 10, 10), h2sl::Transform3D() ) );
  auto bbox_2 = std::make_shared< h2sl::BoundingBox >( h2sl::BoundingBox( h2sl::Vector3D(10, 10, 20), h2sl::Transform3D() ) );

  // Create two object states corresponding to the above poses
  h2sl::ObjectState os_pose_1 = h2sl::ObjectState(pose_1, bbox_1, nullptr, nullptr, nullptr);
  h2sl::ObjectState os_pose_2 = h2sl::ObjectState(pose_2, bbox_2, nullptr, nullptr, nullptr);

  // Push the object states into a state history vector
  std::vector<h2sl::ObjectState> state_history;
  state_history.push_back(os_pose_1);
  state_history.push_back(os_pose_2);

  std::map<std::string, std::map<std::string, double>> property_distributions;

  property_distributions["label"] = std::map<std::string, double>();
  property_distributions["label"]["box"] = 0.75;
  property_distributions["label"]["ball"] = 0.2;
  property_distributions["label"]["drill"] = 0.05;

  property_distributions["danger"] = std::map<std::string, double>();
  property_distributions["danger"]["high"] = 0.7;
  property_distributions["danger"]["low"] = 0.3;

  h2sl::ObjectProbabilistic object("box1", state_history, property_distributions);

  std::cout << "\nDefault probabilistic object: " << object << std::endl << std::endl;

  // Now test invalid property distribution detection
  std::map<std::string, std::map<std::string, double>> invalid_distributions;
  
  invalid_distributions["color"] = std::map<std::string, double>();
  invalid_distributions["color"]["red"] = 0.8;
  invalid_distributions["color"]["blue"] = 0.21; // 0.01 too high

  invalid_distributions["temperature"] = std::map<std::string, double>();
  invalid_distributions["temperature"]["high"] = 0.5;
  invalid_distributions["temperature"]["low"] = 0.49; // 0.01 too low

  h2sl::ObjectProbabilistic invalid_object("invalid", state_history, invalid_distributions);
  std::cout << "Created invalid object: " << invalid_object << std::endl << std::endl;

  if (vm.count("input")) {
    std::cout << "reading from \"" << vm["input"].as<std::string>() << "\""
              << std::endl;
    bool result = object.from_xml(vm["input"].as<std::string>());
    std::cout << "Read object: " << object << std::endl;
    std::cout << "from_xml result: " << result << std::endl;
  }

  // Export XML then import XML and compare
  const char* filename = "/tmp/h2sl-object-probabilistic-demo-export.xml";
  std::cout << "Exporting ObjectProbabilistic to xml..." << std::endl;
  object.to_xml(filename);

  std::cout << "Importing ObjectProbabilistic back from xml..." << std::endl;
  h2sl::ObjectProbabilistic i_object = h2sl::ObjectProbabilistic("imported");
  bool e_result = i_object.from_xml( filename );
  std::cout << "Exported XML ObjectProbabilistic: " << object << std::endl;
  std::cout << "Imported XML ObjectProbabilistic: " << i_object << std::endl;
  std::cout << "h2sl::ObjectProbabilistic::from_xml() return: " << e_result
            << std::endl << std::endl;
/*
  // Testing to-from Json
  std::string msg = object.to_json();
  std::cout << "\nJSON string: " << msg << std::endl;

  h2sl::ObjectProbabilistic object_tmp;
  e_result = object_tmp.from_json(msg);
  std::cout << "Exported JSON ObjectProbabilistic: " << object << std::endl;
  std::cout << "Imported JSON ObjectProbabilistic: " << object_tmp << std::endl;
  std::cout << "h2sl::ObjectProbabilistic::from_json() return: " << e_result
            << std::endl << std::endl;
*/

  // Finally, test conversion to Object class, which uses an Object constructor
  std::map<std::string, std::map<std::string, int>> sampled_property_counts;
  
  srand( time( NULL ) ); // Initialize random seed
  int num_samples = 500000;
  std::cout << "Now sampling " << num_samples << " deterministic objects..." << std::endl << std::endl;
  for( int i = 0; i < num_samples; i++ ){
    h2sl::Object sampled_object( object ); // Samples each property to some deterministic value
    
    // Count instances of semantic properties
    for( const auto& property : sampled_object.properties ){
      std::string property_key = property.first;
      std::string property_value = property.second;
	
      // Attempt to add, or look up, the map for this particular property
      auto result = sampled_property_counts.emplace( property_key, std::map<std::string, int>() );
      auto it_property_map = result.first; // Iterator to the property count map 
      
      // Attempt to initialize the count for this property value
      auto value_result = it_property_map->second.emplace( property_value, 1 );
      if( !value_result.second ){ // Could not insert because a value existed
        it_property_map->second.insert_or_assign( property_value, value_result.first->second + 1 );
      }
    }
  }

  // Print the above results
  std::cout << "Observed sampled property counts:" << std::endl;
  for (auto it_prop_count = sampled_property_counts.begin();
      it_prop_count != sampled_property_counts.end(); it_prop_count++) {
    std::cout << it_prop_count->first << "(";
    for (auto it_prop = it_prop_count->second.begin();
        it_prop != it_prop_count->second.end(); it_prop++) {
      std::cout << "\"" << it_prop->first << "\"=" << it_prop->second;
      if ( next(it_prop) != it_prop_count->second.end() ) {
        std::cout << ",";
      }
    }
    std::cout << ")";
    if ( next(it_prop_count) != sampled_property_counts.end() ) {
      std::cout << ", ";
    }
  }
  std::cout << std::endl << std::endl;

  // Print probability version of sampled results
  std::cout << "Sampled property distributions:" << std::endl;
  for (auto it_prop_count = sampled_property_counts.begin();
      it_prop_count != sampled_property_counts.end(); it_prop_count++) {
    std::cout << it_prop_count->first << "(";
    for (auto it_prop = it_prop_count->second.begin();
        it_prop != it_prop_count->second.end(); it_prop++) {
      std::cout << "\"" << it_prop->first << "\"=" << ( ((double) it_prop->second) / num_samples );
      if ( next(it_prop) != it_prop_count->second.end() ) {
        std::cout << ",";
      }
    }
    std::cout << ")";
    if ( next(it_prop_count) != sampled_property_counts.end() ) {
      std::cout << ", ";
    }
  }
  std::cout << std::endl;

  std::cout << "Object sampled from: " << object << std::endl << std::endl;
  
  if (vm.count("output")) {
    std::cout << "writing object to \"" << vm["output"].as<std::string>()
              << "\"" << std::endl;
    object.to_xml(vm["output"].as<std::string>());
  }

  std::cout << "end of ObjectProbabilistic class demo program" << std::endl;
  return 0;
}
