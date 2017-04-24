/**
 * @file    dcg_tikz_generator.cc
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
 * A DCG tikz generator program
 */

#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include "h2sl/common.h"
#include "h2sl/phrase.h"
#include "h2sl/constraint.h"
#include "h2sl/parser_cyk.h"
#include "h2sl/dcg.h"
#include "dcg_tikz_generator_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of DCG tikz generator program" << endl;
  
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary( args.sd_arg );

  Search_Space * search_space = new Search_Space();

  DCG * dcg = new DCG();
 
  World * world = new World();
  world->from_xml( args.example_arg );
    
  Phrase * phrase = new Phrase();
  phrase->from_xml( args.example_arg, world );

  search_space->fill_groundings( *symbol_dictionary, world );

  if( boost::algorithm::ends_with( args.output_arg, ".tex" ) ){
    string filename = args.output_arg;
    boost::trim_if( filename, boost::is_any_of( ".tex" ) );
    dcg->to_tikz( search_space, phrase, filename + "_gm.tex", "gm" );
    dcg->to_tikz( search_space, phrase, filename + "_egm.tex", "egm" );
  }

  if( phrase != NULL ){
    delete phrase;
    phrase = NULL;
  }

  if( world != NULL ){
    delete world;
    world = NULL;
  }

  if( dcg != NULL ){
    delete dcg;
    dcg = NULL;
  }

  if( search_space != NULL ){
    delete search_space;
    search_space = NULL;
  }

  if( symbol_dictionary != NULL ){
    delete symbol_dictionary;
    symbol_dictionary = NULL;
  }

  cout << "end of DCG tikz generator program" << endl;
  return status;
}
