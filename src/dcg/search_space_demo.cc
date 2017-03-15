/**
 * @file    search_space_demo.cc
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
 * A Search_Space class demo program
 */

#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include "h2sl/common.h"
#include "h2sl/symbol_dictionary.h"
#include "h2sl/world.h"
#include "h2sl/search_space.h"
#include "search_space_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Search_Space class demo program" << endl << endl;
  
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  World * world = new World();
  if( args.world_given ){
    world->from_xml( args.world_arg );
    cout << "world:(" << *world << ")" << endl << endl;
  }

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary();
  if( args.symbol_dictionary_given ){   
    symbol_dictionary->from_xml( args.symbol_dictionary_arg );
    cout << "symbol_dictionary:(" << *symbol_dictionary << ")" << endl << endl;
  }

  Search_Space * search_space = new Search_Space();

  if( search_space != NULL ){
    if( args.input_given ){
      search_space->from_xml( args.input_arg );
      cout << "search_space:(" << *search_space << ")" << endl;
    }

    cout << "filling search space with groundings" << endl << endl;
    search_space->fill_groundings( *symbol_dictionary, world );
    cout << "search_space:(" << *search_space << ")" << endl << endl;

    cout << "filling search space with rules" << endl << endl;
    search_space->fill_rules( *symbol_dictionary, world );
    cout << "search_space:(" << *search_space << ")" << endl << endl;

    Symbol_Dictionary * symbol_dictionary_rules = new Symbol_Dictionary();
    
    search_space->scrape_grounding( *symbol_dictionary_rules, world );

    cout << "symbol_dictionary_extracted:(" << *symbol_dictionary_rules << ")" << endl << endl;

    if( *symbol_dictionary_rules == *symbol_dictionary ){
      cout << "extracted symbol dictionary matches symbol dictionary" << endl;
    } else {
      cout << "extracted symbol dictionary does not match symbol dictionary" << endl;
    }

    if( symbol_dictionary_rules != NULL ){
      delete symbol_dictionary_rules;
      symbol_dictionary_rules = NULL;
    }

    if( args.output_given ){
      search_space->to_xml( args.output_arg );
    }

    delete search_space;
    search_space = NULL;
  }

  if( symbol_dictionary != NULL ){
    delete symbol_dictionary;
    symbol_dictionary = NULL;
  }

  if( world != NULL ){
    delete world;
    world = NULL;
  }

  cout << "end of Search_Space class demo program" << endl;
  return status;
}
