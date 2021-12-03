/**
 * @file object-demo.cc
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

/**
 * Object class demo
 */
int main(int argc, char *argv[]) {
  std::cout << "start of Object class demo program" << std::endl;

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

  std::map<std::string, std::string> object_properties;
  object_properties.insert(std::pair<std::string, std::string>("mass", "0.0"));
  object_properties.insert(std::pair<std::string, std::string>("type", "box"));
/*
  std::map<std::string, std::map<std::string, double>>
      object_property_distributions;

  object_property_distributions["label"] = std::map<std::string, double>();

  object_property_distributions["label"]["box"] = 0.6;
  object_property_distributions["label"]["ball"] = 0.3;
  object_property_distributions["label"]["drill"] = 0.1;
*/

  h2sl::Object object("box1", state_history, object_properties);

  std::cout << "\nDefault object: " << object << std::endl;

  if (vm.count("input")) {
    std::cout << "reading from \"" << vm["input"].as<std::string>() << "\""
              << std::endl;
    bool result = object.from_xml(vm["input"].as<std::string>());
    std::cout << "Read object: " << object << std::endl;
    std::cout << "from_xml result: " << result << std::endl;
  }

  // Export XML then Import XML and compare
  const char* filename = "/tmp/h2sl-object-demo-export.xml";
  std::cout << "Exporting Object to xml..." << std::endl;
  object.to_xml(filename);

  std::cout << "Importing Object back from xml..." << std::endl;
  h2sl::Object i_object = h2sl::Object("imported",
		 state_history, object_properties);
  bool e_result = i_object.from_xml( filename );
  std::cout << "Exported XML Object: " << object << std::endl;
  std::cout << "Imported XML Object: " << i_object << std::endl;
  std::cout << "h2sl::Object::from_xml() return: " << e_result
            << std::endl << std::endl;

  // Testing to-from Json
  std::string msg = object.to_json();
  std::cout << "\nJSON string: " << msg << std::endl;

  h2sl::Object object_tmp;
  e_result = object_tmp.from_json(msg);
  std::cout << "Exported JSON Object: " << object << std::endl;
  std::cout << "Imported JSON Object: " << object_tmp << std::endl;
  std::cout << "h2sl::Object::from_json() return: " << e_result
            << std::endl << std::endl;

  if (vm.count("output")) {
    std::cout << "writing object to \"" << vm["output"].as<std::string>()
              << "\"" << std::endl;
    object.to_xml(vm["output"].as<std::string>());
  }

  std::cout << "end of Object class demo program" << std::endl;
  return 0;
}
