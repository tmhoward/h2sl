/**
 * @file    object_property_demo.cc
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
 * An Object_Property class demo program
 */

#include <iostream>
#include <cstdlib>
#include "h2sl/object_property.h"
#include "object_property_demo_cmdline.h"

using namespace std;
using namespace h2sl;

void
fill_object_propertys( vector< Object_Property* >& object_propertys, const string filename ){
  xmlDoc * doc = NULL;
  xmlNode * root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "object_property" ) ) == 0 ){
            object_propertys.push_back( new Object_Property( l1, NULL ) );
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
  cout << "start of Object_Property class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  vector< Object_Property* > object_propertys;

  if( args.input_given ){
    fill_object_propertys( object_propertys, args.input_arg );
  }

  if( !object_propertys.empty() ){
    cout << "object_propertys.back()->class_name(): " << object_propertys.back()->class_name() << endl;
    for( unsigned int i = 0; i < object_propertys.size(); i++ ){
      cout << "object_propertys[ " << i << " ]: " << *object_propertys[ i ] << endl;
    }
  } else{
    cout << "the input did not have examples of object_property, so we just demo the static functions and ostream operator for the default" << endl;
    object_propertys.push_back( new Object_Property() );
    cout << "object_property:( " << *object_propertys.back() << endl;
    cout << "object_property->class_name(): " << object_propertys.back()->class_name() << endl;
  }

  if( args.output_given ){
    xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
    xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
    xmlDocSetRootElement( doc, root );
    for( unsigned int i = 0; i < object_propertys.size(); i++ ){
      object_propertys[ i ]->to_xml( doc, root );
    }
    xmlSaveFormatFileEnc( args.output_arg, doc, "UTF-8", 1 );
    xmlFreeDoc( doc );
  }

  //memory management
  for( unsigned int i = 0; i < object_propertys.size(); i++ ){
    if( object_propertys[ i ] != NULL ){
      delete object_propertys[ i ];
      object_propertys[ i ] = NULL;
    }
  }

  cout << "end of Object_Property class demo program" << endl;
  return 0;
}
