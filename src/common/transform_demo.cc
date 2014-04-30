/**
 * @file    transform_demo.cc
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
 * A Transform class demo program
 */

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "h2sl/transform.h"
#include "transform_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Transform class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  Transform * a = new Transform( Unit_Quaternion( 0.0, 0.0, 0.0 ), Vector3( 1.0, 2.0, 3.0 ) );
  Transform * b = new Transform( Unit_Quaternion( 0.0, 0.0, 0.0 ), Vector3( 1.0, 2.0, 3.0 ) );
  Transform * c = new Transform( ( *a ) * ( *b ) );
  Transform * d = new Transform( ( a->inverse() ) * ( *b ) );
  Transform * e = new Transform( Unit_Quaternion( 0.0, 0.0, M_PI/2.0 ), Vector3( 1.0, 1.0, 0.0 ) );
  Transform * f = new Transform( Unit_Quaternion( 0.0, 0.0, M_PI/2.0 ), Vector3( 1.0, 0.0, 0.0 ) );
  Transform * g = new Transform( ( *e ) * ( *f ) );
  Transform * h = new Transform( ( e->inverse() ) * ( *f ) );

  cout << "a: " << *a << endl;
  cout << "b: " << *b << endl;
  cout << "c: " << *c << endl;
  cout << "d: " << *d << endl;
  cout << "e: " << *e << endl;
  cout << "f: " << *f << endl;
  cout << "g: " << *g << endl;
  cout << "h: " << *h << endl;

  if( h != NULL ){
    delete h;
    h = NULL;
  }
    
  if( g != NULL ){
    delete g;
    g = NULL;
  }
  
  if( f != NULL ){
    delete f;
    f = NULL;
  }

  if( e != NULL ){
    delete e;
    e = NULL;
  }

  if( d != NULL ){
    delete d;
    d = NULL;
  }

  if( c != NULL ){
    delete c;
    c = NULL;
  }

  if( b != NULL ){
    delete b;
    b = NULL;
  }

  if( a != NULL ){
    delete a;
    a = NULL;
  }

  cout << "end of Transform class demo program" << endl;
  return status;
}
