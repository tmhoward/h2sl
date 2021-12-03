/**
 * @file environment-constraint-demo.cc
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
#include "h2sl/environment-constraint.h"
#include "h2sl/environment-constraint-map.h"

int main( int argc, char* argv[] ){

  boost::program_options::options_description desc{"Options"};
  desc.add_options()("help,h", "Help screen")(
      "input,i", boost::program_options::value<std::string>(), "Input file")(
      "world,w", boost::program_options::value<std::string>(), "World file")(
      "output,o", boost::program_options::value<std::string>(), "Output file");

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  h2sl::World world;

  if( vm.count("world")) {
    std::cout << "here" << std::endl;
  } else {
    h2sl::Object object1( "object1",
                          h2sl::ObjectState( std::make_shared< h2sl::Transform3D >( h2sl::Vector3D( 1.0, 1.0, 0.0 ), h2sl::UnitQuaternion( h2sl::Vector3D( 0.0, 0.0, 0.0 ), 1.0 ) ),
                                              nullptr,
                                              nullptr,
                                              nullptr,
                                              nullptr ) );

    h2sl::Object object2( "object2",
                          h2sl::ObjectState( std::make_shared< h2sl::Transform3D >( h2sl::Vector3D( 1.0, 0.0, 0.0 ), h2sl::UnitQuaternion( h2sl::Vector3D( 0.0, 0.0, 0.0 ), 1.0 ) ),
                                              nullptr,
                                              nullptr,
                                              nullptr,
                                              nullptr ) );

    h2sl::Object object3( "object3",
                          h2sl::ObjectState( std::make_shared< h2sl::Transform3D >( h2sl::Vector3D( 1.0, -1.0, 0.0 ), h2sl::UnitQuaternion( h2sl::Vector3D( 0.0, 0.0, 0.0 ), 1.0 ) ),
                                              nullptr,
                                              nullptr,
                                              nullptr,
                                              nullptr ) );

    world.objects.insert( std::make_pair( object1.uid, object1 ) );
    world.objects.insert( std::make_pair( object2.uid, object2 ) );
    world.objects.insert( std::make_pair( object3.uid, object3 ) );
  }

  std::cout << "world:(" << world << ")" << std::endl;

  // Initialize an EnvironmentConstraintMap
  auto e_constraint_map = h2sl::EnvironmentConstraintMap();

  // Create a set of constraints that check if object1 has the greatest y value
  auto e_constraint_1 =
    h2sl::EnvironmentConstraint(
      h2sl::EnvironmentConstraint::operand_t{
        "object1", h2sl::EnvironmentConstraint::ObjectProperty::POSITION_Y},
      h2sl::EnvironmentConstraint::operand_t{
        "object2", h2sl::EnvironmentConstraint::ObjectProperty::POSITION_Y},
      h2sl::EnvironmentConstraint::ConstraintOperator::GREATER_THAN
    );
  auto e_constraint_2 =
    h2sl::EnvironmentConstraint(
      h2sl::EnvironmentConstraint::operand_t{
        "object1", h2sl::EnvironmentConstraint::ObjectProperty::POSITION_Y},
      h2sl::EnvironmentConstraint::operand_t{
        "object3", h2sl::EnvironmentConstraint::ObjectProperty::POSITION_Y},
      h2sl::EnvironmentConstraint::ConstraintOperator::GREATER_THAN
    );

  // Add the constraints to the constraint map
  e_constraint_map.environment_constraints.emplace( e_constraint_1.to_string(), e_constraint_1 );
  e_constraint_map.environment_constraints.emplace( e_constraint_2.to_string(), e_constraint_2 );


  // Evaluate the set of constraints for the world
  std::cout << "Evaluating the following set of constraints...\n" << e_constraint_map
            << std::endl;
  if( e_constraint_map.evaluate( world ) )
    std::cout << "true\n" << std::endl;
  else
    std::cout << "false\n" << std::endl;

  // Create a new set of constraints that check if object2 has a greater y value than 1
  auto e_constraint_3 =
    h2sl::EnvironmentConstraint(
      h2sl::EnvironmentConstraint::operand_t{
        "object2", h2sl::EnvironmentConstraint::ObjectProperty::POSITION_Y},
      h2sl::EnvironmentConstraint::operand_t{
        "object1", h2sl::EnvironmentConstraint::ObjectProperty::POSITION_Y},
      h2sl::EnvironmentConstraint::ConstraintOperator::GREATER_THAN
    );
  auto e_constraint_4 =
    h2sl::EnvironmentConstraint(
      h2sl::EnvironmentConstraint::operand_t{
        "object1", h2sl::EnvironmentConstraint::ObjectProperty::POSITION_Y},
      h2sl::EnvironmentConstraint::operand_t{
        "object3", h2sl::EnvironmentConstraint::ObjectProperty::POSITION_Y},
      h2sl::EnvironmentConstraint::ConstraintOperator::GREATER_THAN
    );

  // Clear the old constraints and add the new constraints to the constraint map
  e_constraint_map.environment_constraints.clear();
  e_constraint_map.environment_constraints.emplace( e_constraint_3.to_string(), e_constraint_3 );
  e_constraint_map.environment_constraints.emplace( e_constraint_4.to_string(), e_constraint_4 );

  // Evaluate the set of constraints for the world
  std::cout << "Evaluating the following set of constraints...\n" << e_constraint_map
            << std::endl;
  if( e_constraint_map.evaluate( world ) )
    std::cout << "true\n" << std::endl;
  else
    std::cout << "false\n" << std::endl;

  // Adjust the world to make e_constraint_3 true
  world.objects.find("object2")->second.state_history.back().pose->position = h2sl::Vector3D( 1.0, 2.0, 0.0 );

  std::cout << "adjusted world model:(" << world << ")" << std::endl;
  std::cout << "reevaluating with new world model" << std::endl;

  // Evaluate the set of constraints for the world
  std::cout << "Evaluating the following set of constraints...\n" << e_constraint_map
            << std::endl;
  if( e_constraint_map.evaluate( world ) )
    std::cout << "true\n" << std::endl;
  else
    std::cout << "false\n" << std::endl;

  // Export the constraint map to an xml file
  if (vm.count("output")) {
    std::cout << "writing environment constraint to \"" << vm["output"].as<std::string>() << "\"" << std::endl;
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement * root = document.NewElement( "root" );
    document.InsertEndChild( root );
    //e_constraint_map.to_xml( document, root );
    document.SaveFile( vm["output"].as<std::string>().c_str() );
  }
  return EXIT_SUCCESS;
}
