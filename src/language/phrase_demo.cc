/**
 * @file    phrase_demo.cc
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
 * A Phrase class demo program
 */

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <boost/algorithm/string.hpp>

#include <h2sl/phrase.h>
#include "phrase_demo_cmdline.h"


using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Phrase class demo program" << endl;
  
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  World * world = new World();
  if( args.world_given ){
    world = new World( args.world_arg );
  }

  Phrase * phrase = new Phrase();

  if( args.input_given ){
    phrase->from_xml( args.input_arg, world );
  } 

  //create a duplicate for testing the dup() function
  bool phrase_exists = false;
  Phrase * dup_phrase = NULL;
  
  if( phrase != NULL ){
    phrase_exists = true;

    //check: ostream operator
    cout << "  PHRASE:(" << *phrase << ")" << endl;

    //assign to dup_phrase
    dup_phrase = phrase->dup();
    
    //check: num_phrases function
    cout << "  num_phrases: " << phrase->num_phrases() << endl;
    
    //check:: num_words function
    cout << "  num_words: " << phrase->num_words() << endl;
  }

  //if output provided, export as either xml or tex
  if( args.output_given ){
    phrase->to_file( args.output_arg );
  }

  //check: dup_phrase matches phrase
  if( dup_phrase != NULL ){
    cout << "  DUP (before deleting phrase): " << *dup_phrase << endl;
  } else if( phrase != NULL ){
    cout << "  Error: phrase->dup() returned null" << endl;
    assert( false );
  }

  //memory management
  if( phrase != NULL ){
    delete phrase;
    phrase = NULL;
  }

  //check: dup_phrase still exists after deleting phrase
  if( dup_phrase != NULL ){
    cout << "  DUP (after deleting phrase): " << *dup_phrase << endl;
    //memory management
    delete dup_phrase;
    dup_phrase = NULL;
  } else if( phrase_exists ){
    cout << "  Error: dup_phrase is NULL after deleting phrase" << endl;
    assert( false );
  }

  cout << "end of Phrase class demo program" << endl;
  return status;
}
