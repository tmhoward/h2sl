/**
 * @file world-probabilistic-demo.cc
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
#include "h2sl/world-probabilistic.h"

/**
 * WorldProbabilistic classdemo
 */
int
main( int argc,
      char* argv[] ){
  std::cout << "start of WorldProbabilistic class demo program" << std::endl;

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

  h2sl::ObjectProbabilistic object_a( "a", h2sl::ObjectState() );
  h2sl::ObjectProbabilistic object_b( "b", h2sl::ObjectState() );
  h2sl::WorldProbabilistic::mapProbObjects objects;
  objects.emplace( object_a.uid, object_a );
  objects.emplace( object_b.uid, object_b );

  h2sl::WorldProbabilistic world(objects);
  
  if( vm.count( "input" ) ){
    std::cout << "reading from \"" << vm["input"].as<std::string>() << "\"" << std::endl;
    bool input_result = world.from_xml( vm["input"].as<std::string>() );
    std::cout << "Import call to from_xml() returned " << input_result << std::endl;
  }

  std::cout << "\nworld: " << world << std::endl;
  
  // Export XML then Import XML and compare
  const char* filename = "/tmp/h2sl-world-probabilistic-demo-export.xml";
  std::cout << "Exporting WorldProbabilistic to xml..." << std::endl;
  world.to_xml(filename);

  std::cout << "Importing WorldProbabilistic back from xml..." << std::endl;
  h2sl::WorldProbabilistic i_world = h2sl::WorldProbabilistic();
  bool e_result = i_world.from_xml( filename );
  std::cout << "Exported XML WorldProbabilistic: " << world << std::endl;
  std::cout << "Imported XML WorldProbabilistic: " << i_world << std::endl;
  std::cout << "h2sl::WorldProbabilistic::from_xml() return: " << e_result
            << std::endl << std::endl;

  // Now sample a few deterministic worlds from this WorldProbabilistic
  h2sl::World sample1 = h2sl::World( world );
  h2sl::World sample2 = h2sl::World( world );
  h2sl::World sample3 = h2sl::World( world );

  std::cout << "Sampled world 1: " << sample1 << std::endl << std::endl;
  std::cout << "Sampled world 2: " << sample2 << std::endl << std::endl;
  std::cout << "Sampled world 3: " << sample3 << std::endl << std::endl;

  if( vm.count( "output" ) ){
    std::cout << "writing world to \"" << vm["output"].as<std::string>() << "\"" << std::endl;
    world.to_xml( vm["output"].as<std::string>() );
  }

  std::cout << "end of WorldProbabilistic class demo program" << std::endl;
  return 0;
} 
