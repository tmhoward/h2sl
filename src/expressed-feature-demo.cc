/**
 * @file expressed-feature-demo.cc
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
#include "h2sl/expressed-feature.h"

int main( int argc, char* argv[] ){

  boost::program_options::options_description desc{"Options"};
  desc.add_options()("help,h", "Help screen")(
      "input,i", boost::program_options::value<std::string>(), "Input file")(
      "output,o", boost::program_options::value<std::string>(), "Output file");

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  h2sl::ExpressedFeature expressed_feature;

  if (vm.count("input")) {
    std::cout << "reading expressed_feature from \"" << vm["input"].as<std::string>() << "\"" << std::endl;
    expressed_feature.from_file(vm["input"].as<std::string>());
  }

  std::cout << expressed_feature << std::endl;

  if (vm.count("output")) {
    std::cout << "writing expressed_feature to \"" << vm["output"].as<std::string>() << "\"" << std::endl;
    expressed_feature.to_file(vm["output"].as<std::string>());
  }

  return EXIT_SUCCESS;
}
