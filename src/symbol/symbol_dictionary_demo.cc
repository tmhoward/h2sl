/**
 * @file    symbol_dictionary_demo.cc
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
 * A Symbol_Dictionary class demo program
 */

#include <iostream>
#include <iomanip>
#include "h2sl/symbol_dictionary.h"
#include "symbol_dictionary_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Symbol_Dictionary class demo program" << endl;
  
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary();
  if( symbol_dictionary != NULL ){
    if( args.input_given ){
      symbol_dictionary->from_xml( args.input_arg );
    } 

    cout << "symbol_dictionary:(" << *symbol_dictionary << ")" << endl;

    if( args.output_given ){
      symbol_dictionary->to_xml( args.output_arg );
    }

    delete symbol_dictionary;
    symbol_dictionary = NULL;
  }

  cout << "end of Symbol_Dictionary class demo program" << endl;
  return status;
}
