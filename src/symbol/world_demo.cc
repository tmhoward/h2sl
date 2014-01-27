/**
 * @file    world_demo.cc
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
 * A World class demo program
 */

#include <iostream>
#include <sstream>
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of World class demo program" << endl;

  World * world = new World();

  if( argc > 1 ){
    unsigned int num_objects = strtol( argv[1], NULL, 10 );
    for( unsigned int i = 0; i < num_objects; i++ ){
      stringstream object_name;
      object_name << "o" << i;
      world->object_set().objects().push_back( Object( object_name.str(), i + 1 ) );
    }

    if( argc > 2 ){
      world->to_xml( argv[2] );
    }
  }

  if( world != NULL ){
    delete world;
    world = NULL;
  }

  cout << "end of World class demo program" << endl;
  return status;
}
