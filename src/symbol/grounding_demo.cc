/**
 * @file    grounding_demo.cc
 * @author  Thomas M. Howard (tmhoward@csail.mit.edu)
 *          Matthew R. Walter (mwalter@csail.mit.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
 *
 * Copyright (C) 2014 by the Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * @section DESCRIPTION
 *
 * A Grounding class demo program
 */

#include <iostream>
#include <cstdlib>
#include "h2sl/grounding.h"
#include "h2sl/object.h"
#include "h2sl/world.h"
#include "grounding_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Grounding class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  World * world = NULL;
  if( args.world_given ){
    world = new World( args.world_arg );
  }

  Grounding * grounding = new Object();

  if( args.input_given ){
    grounding->from_xml( args.input_arg, world );
  }

  cout << "grounding:(" << *grounding << ")" << endl;

  if( args.output_given ){
    grounding->to_xml( args.output_arg );
  }

  if( grounding != NULL ){
    delete grounding;
    grounding = NULL;
  }

  cout << "end of Grounding class demo program" << endl;
  return status;
}
