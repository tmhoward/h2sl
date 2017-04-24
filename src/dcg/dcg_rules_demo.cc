/**
 * @file    dcg_rules_demo.cc
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
 * A DCG class demo program
 */

#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include "h2sl/common.h"
#include "h2sl/phrase.h"
#include "h2sl/constraint.h"
#include "h2sl/parser_cyk.h"
#include "h2sl/dcg.h"
#include "dcg_rules_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of DCG class demo program" << endl;
  
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  Parser< Phrase > * parser = new Parser_CYK< Phrase >();
  Grammar * grammar = new Grammar();
  grammar->from_xml( args.grammar_arg );

  vector< Phrase* > phrases;
  
  Grounding * context = NULL;

  World * world = new World();
  if( args.world_given ){
    world->from_xml( args.world_arg );
  }

  Feature_Set * feature_set = new Feature_Set();
  LLM * llm = new LLM( feature_set );
  if( args.llm_given ){
    llm->from_xml( args.llm_arg );
  }

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary( args.symbol_dictionary_arg );

  Search_Space * search_space = new Search_Space();

  DCG * dcg = new DCG();
 
  struct timeval start_time;
  gettimeofday( &start_time, NULL );
  
  search_space->fill_rules( *symbol_dictionary, world );

  struct timeval end_time;
  gettimeofday( &end_time, NULL );

  cout << "finished fill_seach_space in " << diff_time( start_time, end_time ) << " seconds" << endl;

  cout << endl << "search_spaces.size(): " << search_space->grounding_pairs().size() << endl << endl;

  cout << "parsing \"" << args.command_arg << "\"" << endl;
  if( parser->parse( *grammar, args.command_arg, phrases ) ){
    for( unsigned int i = 0; i < phrases.size(); i++ ){
      if( phrases[ i ] != NULL ){
        cout << "phrases[" << i << "]:" << *phrases[ i ] << endl;
    
        gettimeofday( &start_time, NULL );

        dcg->leaf_search( phrases[ i ], *symbol_dictionary, search_space, world, context, llm, args.beam_width_arg );

        gettimeofday( &end_time, NULL );

        cout << "finished search in " << diff_time( start_time, end_time ) << " seconds" << endl;   
        for( unsigned int j = 0; j < dcg->solutions().size(); j++ ){
          cout << "  solutions[" << j << "]:" << *dcg->solutions()[ j ].second << " (" << dcg->solutions()[ j ].first << ")" << endl;
        }
   
        if( args.output_given ){
          string filename = args.output_arg;
          if( filename.find( ".xml" ) != string::npos ){
            if( phrases.size() == 1 ){
              dcg->solutions().front().second->to_xml( filename );
            } else {
              boost::trim_if( filename, boost::is_any_of( ".xml" ) );
              stringstream tmp;
              tmp << filename << "_" << setw( 4 ) << setfill( '0' ) << i << ".xml";
              dcg->solutions().front().second->to_xml( tmp.str() );
            }
          }
        }

        if( args.latex_output_given ){
          string filename = args.latex_output_arg;
          if( filename.find( ".tex" ) != string::npos ){
            if( phrases.size() == 1 ){
              dcg->to_tikz( search_space, phrases[ i ], filename );
            } else {
              boost::trim_if( filename, boost::is_any_of( ".tex" ) );
              stringstream tmp;
              tmp << filename << "_" << setw( 4 ) << setfill( '0' ) << i << ".tex";
              dcg->to_tikz( search_space, phrases[ i ], tmp.str() );
            }
          }
        } 
      }
    }
  }

  if( dcg != NULL ){
    delete dcg;
    dcg = NULL;
  }

  if( llm != NULL ){
    delete llm;
    llm = NULL;
  }

  if( feature_set != NULL ){
    delete feature_set;
    feature_set = NULL;
  }

  if( world != NULL ){
    delete world;
    world = NULL;
  }

  for( unsigned int i = 0; i < phrases.size(); i++ ){
    if( phrases[ i ] != NULL ){
      delete phrases[ i ];
      phrases[ i ] = NULL;
    }
  }

  if( grammar != NULL ){
    delete grammar;
    grammar = NULL;
  }

  if( parser != NULL ){
    delete parser;
    parser = NULL;
  }

  cout << "end of DCG class demo program" << endl;
  return status;
}
