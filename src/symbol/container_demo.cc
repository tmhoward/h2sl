/**
 * @file    container_demo.cc
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
 * An Container class demo program
 */

#include <iostream>
#include <cstdlib>
#include "h2sl/container.h"
#include "h2sl/world.h"
#include "container_demo_cmdline.h"

using namespace std;
using namespace h2sl;

void
fill_containers( vector< Container* >& containers, const string filename,
                  World* world ){
  xmlDoc * doc = NULL;
  xmlNode * root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "container" ) ) == 0 ){
            containers.push_back( new Container( l1, world ) );
          }
        }
      }
    }
  }
  return;
}

int
main( int argc,
      char* argv[] ) {
  cout << "start of Container class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  World * world = NULL;
  if( args.world_given ){
    world = new World( args.world_arg );
  }

  vector< Container* > containers;

  if( args.input_given ){
    fill_containers( containers, args.input_arg, world );
  }

  if( !containers.empty() ){
    cout << "containers.back()->class_name(): " << containers.back()->class_name() << endl;
    cout << "Testing the has_object function by passing in one novel object \"test_object\" (should return '0') and all objects from the container (should return '1')." << endl;
    const Object * test_object = new Object( "test_object" );
    for( unsigned int i = 0; i < containers.size(); i++ ){
      cout << "  containers[ " << i << " ]->has_object( test_object ): " << containers[ i ]->has_object( test_object ) << endl;
      for( unsigned int j = 0; j < containers[ i ]->container().size(); j++ ){
        if( dynamic_cast< const Object* >( containers [ 0 ]->container()[ j ] ) != NULL ){
          const Object* tmp = static_cast< const Object* >( containers [ 0 ]->container()[ j ] );
          cout << "  containers[ " << i << " ]->has_object( containers[ " << i << " ]->container()[ " << j << " ] ): " << containers[ 0 ]->has_object( tmp ) << endl;
        }
      }
      cout << endl;
    }
    cout << endl;
    cout << "Testing the min_distance_to_object function for each object in each container." << endl;
    for( unsigned int i = 0; i < containers.size(); i++ ){
      for( unsigned int j = 0; j < containers[ i ]->container().size(); j++ ){
        if( dynamic_cast< const Object* >( containers [ 0 ]->container()[ j ] ) != NULL ){
          const Object* tmp = static_cast< const Object* >( containers [ 0 ]->container()[ j ] );
          cout << "  containers[ " << i << " ]->min_distance_to_object( *containers[ " << i << " ]->container()[ " << j << " ] ): " << containers[ 0 ]->min_distance_to_object( *tmp ) << endl;
        }
      }
      cout << endl;
    }

    cout << endl;
    cout << "Testing the deviation function for each object in each container." << endl;
    for( unsigned int i = 0; i < containers.size(); i++ ){
      for( unsigned int j = 0; j < containers[ i ]->container().size(); j++ ){
        if( dynamic_cast< const Object* >( containers [ 0 ]->container()[ j ] ) != NULL ){
          const Object* tmp = static_cast< const Object* >( containers [ 0 ]->container()[ j ] );
          cout << "  containers[ " << i << " ]->deviation( *containers[ " << i << " ]->container()[ " << j << " ] ): " << containers[ 0 ]->deviation( *tmp ) << endl;
        }
      }
      cout << endl;
    }

    cout << endl;
    cout << "Testing the mean_position(), variance_position(), max_deviation(), bounds(), and principal_axis() functions for each container." << endl;
    for( unsigned int i = 0; i < containers.size(); i++ ){
      cout << "  containers[ " << i << " ]->mean_position(): " << containers[ i ]->mean_position() << endl;
      cout << "  containers[ " << i << " ]->variance_positon(): " << containers[ i ]->variance_position() << endl;
      cout << "  containers[ " << i << " ]->max_deviation(): " << containers[ i ]->max_deviation() << endl;
      cout << "  *containers[ " << i << " ]->bounds().first: " << *containers[ i ]->bounds().first << endl;
      cout << "  *containers[ " << i << " ]->bounds().second: " << *containers[ i ]->bounds().second << endl;
      cout << "  containers[ " << i << " ]->principal_axis(): " << containers[ i ]->principal_axis() << endl;
      cout << endl;
    }
    

    cout << "Testing the ostream operator for all provided containers" << endl;
    for( unsigned int i = 0; i < containers.size(); i++ ){
      cout << "  containers[ " << i << " ]: " << *containers[ i ] << endl;
    }
  } else{
    cout << "the input did not have examples of container, so we just demo the static functions and ostream operator for the default" << endl;
    containers.push_back( new Container() );
    cout << "container:( " << *containers.back() << endl;
    cout << "container->class_name(): " << containers.back()->class_name() << endl;
  }

  if( args.output_given ){
    xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
    xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
    xmlDocSetRootElement( doc, root );
    for( unsigned int i = 0; i < containers.size(); i++ ){
      containers[ i ]->to_xml( doc, root );
    }
    xmlSaveFormatFileEnc( args.output_arg, doc, "UTF-8", 1 );
    xmlFreeDoc( doc );
  }

  //memory management
  for( unsigned int i = 0; i < containers.size(); i++ ){
    if( containers[ i ] != NULL ){
      delete containers[ i ];
      containers[ i ] = NULL;
    }
  }

  cout << "end of Container class demo program" << endl;
  return 0;
}
