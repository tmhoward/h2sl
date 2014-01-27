/**
 * @file    dgc_demo.cc
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
 * A demo program that uses the DCG class
 */

#include <iostream>
#include <queue>
#include <getopt.h>

#include "h2sl/parser.h"
#include "h2sl/dcg.h"
#include "dcg_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  cout << "start of DCG class demo program" << endl;
 
  priority_queue<vector<int>,vector<vector<int> > > pq;
   
  DCG * dcg = new DCG();
  
  World * world = new World();
  world->from_xml( args.world_arg ); 
 
  LLM * llm = new LLM();
  llm->from_xml( args.llm_arg );

  Parser * parser = new Parser();
  parser->grammar().from_xml( args.grammar_arg ); 

  Phrase * phrase = new Phrase();

  if( parser->parse( args.command_arg, *phrase ) ){
    vector< pair< double, Phrase > > solutions;

    if( dcg->search( *phrase, *world, *llm, solutions ) ){
      cout << "dcg search succeeded" << endl;
    } else {
      cout << "dcg search failed" << endl;
    }
  }
 
  if( phrase != NULL ){
    delete phrase;
    phrase = NULL;
  }
 
  if( parser != NULL ){
    delete parser;
    parser = NULL;
  }
      
  if( llm != NULL ){
    delete llm;
    llm = NULL;
  }

  if( world != NULL ){
    delete world;
    world = NULL;
  }

  if( dcg != NULL ){
    delete dcg;
    dcg = NULL;
  }

  cout << "end of DCG class demo program" << endl;
  return 0;
}
