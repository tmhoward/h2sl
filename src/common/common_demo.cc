/**
 * @file    common_demo.cc
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
 * A Vector3 class demo program
 */

#include <iostream>
#include <cstdlib>
#include "h2sl/common.h"
#include "common_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Vector3 class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  vector< unsigned int > v1;
  vector< unsigned int > v2;
  vector< unsigned int > v3;
  vector< unsigned int > v4;

  v1.push_back( 1 );
  v1.push_back( 2 );
  
  v2.push_back( 3 );
  v2.push_back( 4 );
  v2.push_back( 5 );

  v3.push_back( 6 );
  v3.push_back( 7 );
  v3.push_back( 8 );

  v4.push_back( 9 );

  vector< vector< unsigned int > > vectors;
  vectors.push_back( v1 );
  vectors.push_back( v2 );
  vectors.push_back( v3 );
  vectors.push_back( v4 );

  vector< vector< unsigned int > > set = cartesian_power( vectors ); 

  cout << "set[" << set.size() << "]:{";
  for( unsigned int i = 0; i < set.size(); i++ ){
    cout << "{";
    for( unsigned int j = 0; j < set[ i ].size(); j++ ){
      cout << set[ i ][ j ];
      if( j != ( set[ i ].size() - 1 ) ){
        cout << ",";
      }
    }
    cout << "}";   
    if( i != ( set.size() - 1 ) ){
      cout << ",";
    } 
  }
  cout << "}" << endl;

  cout << "end of Vector3 class demo program" << endl;
  return status;
}
