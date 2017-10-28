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

#include <sstream>
#include "h2sl/world.h"
#include "world_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of World class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  World * world = new World();

  if( args.input_given ){
    world->from_xml( args.input_arg );
  }

  cout << "world:(" << *world << ")" << endl;

  for( map< string, map< string, vector< Object* > > >::const_iterator it_sorted_objects = world->sorted_objects().begin(); it_sorted_objects != world->sorted_objects().end(); it_sorted_objects++ ){
    cout << "sorting:\"" << it_sorted_objects->first << "\"" << endl;
    for( map< string, vector< Object* > >::const_iterator it_sorted_typed_objects = it_sorted_objects->second.begin(); it_sorted_typed_objects != it_sorted_objects->second.end(); it_sorted_typed_objects++ ){
      cout << "  " << it_sorted_typed_objects->first << "[" << it_sorted_typed_objects->second.size() << "]:{";
      for( unsigned int i = 0; i < it_sorted_typed_objects->second.size(); i++ ){
        cout << it_sorted_typed_objects->second[ i ]->id();
        if( i != ( it_sorted_typed_objects->second.size() - 1 ) ){
          cout << ",";
        }
      }
      cout << "}" << endl;
    } 
  }

  World * other_world = new World( *world );
  cout << "testing world equivalency operator, result should be true (1)" << endl;
  cout << "*world == *other_world: " << ( *world == *other_world ) << endl << endl;
  cout << "testing world inequivalency operator, result should be false (0)" << endl;
  cout << "*world == *other_world: " << ( *world != *other_world ) << endl;

  if( args.output_given ){
    world->to_xml( args.output_arg );
  }

  if( ! world->objects().empty() ) {
	const string reference_object_id = world->objects().begin()->first;
	world->transform_objects(reference_object_id);
	Transform origin = Transform();
	origin.from_std_string("0,0,0,1,0,0,0");
	cout << "testing reference object at origin, result should be true (1)" << endl;
	cout << "reference object transform == origin: " << ( world->objects().begin()->second->transform().to_std_string() == origin.to_std_string() ) << endl;
  }

  if( world != NULL ){
    delete world;
    world = NULL;
  }

  cout << "end of World class demo program" << endl;
  return status;
}
