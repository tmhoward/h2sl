/**
 * @file    index_demo.cc
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
 * An Index class demo program
 */

#include <iostream>
#include <cstdlib>
#include "h2sl/index.h"
#include "index_demo_cmdline.h"

using namespace std;
using namespace h2sl;

void
fill_index_vector( vector< Index* >& index_vector, const string filename ){
  xmlDoc * doc = NULL;
  xmlNode * root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "index" ) ) == 0 ){
            index_vector.push_back( new Index( l1, NULL ) );
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
  cout << "start of Index class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  vector< Index* > index_vector;

  if( args.input_given ){
    fill_index_vector( index_vector, args.input_arg );
  }

  if( !index_vector.empty() ){
    cout << "index_vector.back()->class_name(): " << index_vector.back()->class_name() << endl;
    for( unsigned int i = 0; i < index_vector.size(); i++ ){
      cout << "index_vector[ " << i << " ]: " << *index_vector[ i ] << endl;
    }
  } else{
    cout << "the input did not have examples of index, so we just demo the static functions and ostream operator for the default" << endl;
    index_vector.push_back( new Index() );
    cout << "index:( " << *index_vector.back() << endl;
    cout << "index->class_name(): " << index_vector.back()->class_name() << endl;
  }

  if( args.output_given ){
    xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
    xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
    xmlDocSetRootElement( doc, root );
    for( unsigned int i = 0; i < index_vector.size(); i++ ){
      index_vector[ i ]->to_xml( doc, root );
    }
    xmlSaveFormatFileEnc( args.output_arg, doc, "UTF-8", 1 );
    xmlFreeDoc( doc );
  }

  //memory management
  for( unsigned int i = 0; i < index_vector.size(); i++ ){
    if( index_vector[ i ] != NULL ){
      delete index_vector[ i ];
      index_vector[ i ] = NULL;
    }
  }

  cout << "end of Index class demo program" << endl;
  return 0;
}
