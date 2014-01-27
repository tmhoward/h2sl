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

#include <h2sl/phrase.h>

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Phrase class demo program" << endl;
  
  Phrase * phrase = new Phrase();

  if( argc > 1 ){
    phrase->from_xml( argv[1] );
  } 

  cout << "phrase: " << *phrase << endl; 

  if( argc > 2 ){
    phrase->to_xml( argv[2] );
  }

  Phrase * dup_phrase = phrase->dup();

  cout << "dup: " << *dup_phrase << endl;

  if( phrase != NULL ){
    delete phrase;
    phrase = NULL;
  }

  cout << "dup: " << *dup_phrase << endl;

  cout << "end of Phrase class demo program" << endl;
  return status;
}
