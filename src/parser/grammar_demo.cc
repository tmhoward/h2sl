/**
 * @file    grammar_demo.cc
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
 * A Grammar class demo program
 */

#include <iostream>

#include "h2sl/grammar.h"
#include "grammar_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  cout << "start of Grammar class demo program" << endl;

  Grammar * grammar = new Grammar();

  grammar->from_xml( args.grammar_arg );
  
  cout << "grammar:(" << *grammar << ")" << endl;
  
  if( args.output_given ){
    cout << "writing grammar to " << args.output_arg << endl;
    grammar->to_xml( args.output_arg );
  }

  if( grammar != NULL ){
    delete grammar;
    grammar = NULL;
  }

  cout << "end of Grammar class demo program" << endl;
  return 0;
}
