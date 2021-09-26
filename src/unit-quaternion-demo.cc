/**
 * @file unit-quaternion-demo.cc
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

#include "h2sl/unit-quaternion.h"

/**
 * UnitQuaternion class demo
 */
int main(int argc, char *argv[]) {
  std::cout << "start of UnitQuaternion class demo program" << std::endl;

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

  // Create a default UnitQuaternion
  auto unit_quaternion = h2sl::UnitQuaternion( h2sl::Vector3D(0.707,0.0,0.0), 0.707 );
  
  std::cout << "\nDefault unit quaternion: " << unit_quaternion << std::endl;

  if (vm.count("input")) {
    std::cout << "reading from \"" << vm["input"].as<std::string>() << "\""
              << std::endl;
    bool result = unit_quaternion.from_xml(vm["input"].as<std::string>());
    std::cout << "Read unit quaternion:" << unit_quaternion << std::endl;
    std::cout << "from_xml result: " << result << std::endl;
  }

  // Export XML then Import XML and compare
  const char* filename = "/tmp/h2sl-unit-quaternion-demo-export.xml";
  std::cout << "Exporting UnitQuaternion to xml..." << std::endl;
  unit_quaternion.to_xml(filename);

  std::cout << "Importing UnitQuaternion back from xml..." << std::endl;
  h2sl::UnitQuaternion i_uq = h2sl::UnitQuaternion();
  bool e_result = i_uq.from_xml( filename );
  std::cout << "Exported XML UnitQuaternion: " << unit_quaternion << std::endl;
  std::cout << "Imported XML UnitQuaternion: " << i_uq << std::endl;
  std::cout << "h2sl::UnitQuaternion::from_xml() return: " << e_result
            << std::endl << std::endl;

  // Create a few UnitQuaternion from RPY angles
  auto identity = h2sl::UnitQuaternion( 0.0, 0.0, 0.0 );
  std::cout << "\nIdentity UnitQuaternion created with RPY (0,0,0)" << std::endl;
  std::cout << identity << std::endl;
  identity.print_rpy();

  auto flipped_yz = h2sl::UnitQuaternion( 3.1415, 0.0, 0.0 );
  std::cout << "\nFlipped YZ UnitQuaternion created with RPY (3.1415,0,0)" << std::endl;
  std::cout << flipped_yz << std::endl;
  flipped_yz.print_rpy();

  auto flipped_xz = h2sl::UnitQuaternion( 0.0, 3.1415, 0.0 );
  std::cout << "\nFlipped XZ UnitQuaternion created with RPY (0,3.1415,0)" << std::endl;
  std::cout << flipped_xz << std::endl;
  flipped_xz.print_rpy();

  auto flipped_xy = h2sl::UnitQuaternion( 0.0, 0.0, 3.1415 );
  std::cout << "\nFlipped XY UnitQuaternion created with RPY (0,0,3.1415)" << std::endl;
  std::cout << flipped_xy << std::endl;
  flipped_xy.print_rpy();

  auto other_flipped_yz = h2sl::UnitQuaternion( 0.0, 3.1415, 3.1415 );
  std::cout << "\nAnother Flipped YZ UnitQuaternion created with RPY (0,3.1415,3.1415)" << std::endl;
  std::cout << other_flipped_yz << std::endl;
  other_flipped_yz.print_rpy();

  auto up_diagonal_right = h2sl::UnitQuaternion( 0.0, -0.7854, -0.7854 );
  std::cout << "\nUp Diagonal Right UnitQuaternion created with RPY (0,-0.7854,-0.7854)" << std::endl;
  std::cout << up_diagonal_right << std::endl;
  up_diagonal_right.print_rpy();

  if (vm.count("output")) {
    std::cout << "writing unit quaternion to \"" << vm["output"].as<std::string>()
              << "\"" << std::endl;
    unit_quaternion.to_xml(vm["output"].as<std::string>());
  }

  std::cout << "end of UnitQuaternion class demo program" << std::endl;
  return 0;
}
