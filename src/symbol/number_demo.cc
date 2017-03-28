/**
 * @file    number_demo.cc
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
 * An Number class demo program
 */

#include <iostream>
#include <cstdlib>
#include "h2sl/number.h"
#include "number_demo_cmdline.h"

using namespace std;
using namespace h2sl;

void
fill_numbers( vector< Number* >& numbers, const string filename ){
  xmlDoc * doc = NULL;
  xmlNode * root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "number" ) ) == 0 ){
            numbers.push_back( new Number( l1, NULL ) );
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
  cout << "start of Number class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  vector< Number* > numbers;

  if( args.input_given ){
    fill_numbers( numbers, args.input_arg );
  }

  if( !numbers.empty() ){
    cout << "numbers.back()->class_name(): " << numbers.back()->class_name() << endl;
    for( unsigned int i = 0; i < numbers.size(); i++ ){
      cout << "numbers[ " << i << " ]: " << *numbers[ i ] << endl;
    }
  } else{
    cout << "the input did not have examples of number, so we just demo the static functions and ostream operator for the default" << endl;
    numbers.push_back( new Number() );
    cout << "number:( " << *numbers.back() << endl;
    cout << "number->class_name(): " << numbers.back()->class_name() << endl;
  }

  if( args.output_given ){
    xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
    xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
    xmlDocSetRootElement( doc, root );
    for( unsigned int i = 0; i < numbers.size(); i++ ){
      numbers[ i ]->to_xml( doc, root );
    }
    xmlSaveFormatFileEnc( args.output_arg, doc, "UTF-8", 1 );
    xmlFreeDoc( doc );
  }

  //memory management
  for( unsigned int i = 0; i < numbers.size(); i++ ){
    if( numbers[ i ] != NULL ){
      delete numbers[ i ];
      numbers[ i ] = NULL;
    }
  }

  cout << "end of Number class demo program" << endl;
  return 0;
}
