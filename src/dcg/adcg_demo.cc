/**
 * @file    adcg_demo.cc
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
 * A ADCG class demo program
 */

#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include "h2sl/common.h"
#include "h2sl/phrase.h"
#include "h2sl/constraint.h"
#include "h2sl/parser_cyk.h"
#include "h2sl/adcg.h"
#include "adcg_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of ADCG class demo program" << endl;
  
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  vector< Phrase* > phrases;
  
  Grounding * context = NULL;

  Feature_Set * feature_set = new Feature_Set();
  LLM * llm = new LLM( feature_set );
  if( args.llm_given ){
    llm->from_xml( args.llm_arg );
  }

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary( args.symbol_dictionary_groundings_arg );

  Search_Space * search_space = new Search_Space();

  ADCG * adcg = new ADCG();
 
  struct timeval start_time;
  struct timeval end_time;

  if( args.example_given ){
    World * world = new World();
    world->from_xml( args.example_arg );
    
    Phrase * phrase = new Phrase();
    phrase->from_xml( args.example_arg, world );
    phrase->clear_grounding_sets();

    cout << endl << "search_spaces grounding pairs size(): " << search_space->grounding_pairs().size() << endl << endl;
    phrase->to_xml( "/tmp/phrase.xml" ); 

    search_space->fill_groundings( *symbol_dictionary, world );

    gettimeofday( &start_time, NULL );

    adcg->leaf_search( phrase, *symbol_dictionary, search_space, world, context, llm, args.beam_width_arg, ( bool )( args.debug_arg ) );

    gettimeofday( &end_time, NULL );

    cout << "finished search in " << diff_time( start_time, end_time ) << " seconds" << endl;
    for( unsigned int j = 0; j < adcg->solutions().size(); j++ ){
      cout << "  solutions[" << j << "]:" << *adcg->solutions()[ j ].second << " (" << adcg->solutions()[ j ].first << ")" << endl;
    }

    if( args.output_given ){
      string filename = args.output_arg;
      if( filename.find( ".xml" ) != string::npos ){
        if( phrases.size() == 1 ){
          adcg->solutions().front().second->to_xml( filename );
        } else {
          adcg->solutions().front().second->to_xml( args.output_arg );
        }
      }
    }
  
    if( phrase != NULL ){
      delete phrase;
      phrase = NULL;
    }

    if( world != NULL ){
      delete world;
      world = NULL;
    } 
  }
  
  if( args.grammar_given && args.command_given && args.world_given ){
    World * world = new World();
    if( args.world_given ){
      world->from_xml( args.world_arg );
    }

    Parser< Phrase > * parser = new Parser_CYK< Phrase >();
    Grammar * grammar = new Grammar();
    grammar->from_xml( args.grammar_arg );

    cout << "parsing \"" << args.command_arg << "\"" << endl;
    if( parser->parse( *grammar, args.command_arg, phrases ) ){
      for( unsigned int i = 0; i < phrases.size(); i++ ){
        if( phrases[ i ] != NULL ){
          cout << "phrases[" << i << "]:" << *phrases[ i ] << endl;
    
          gettimeofday( &start_time, NULL );

          adcg->leaf_search( phrases[ i ], *symbol_dictionary, search_space, world, context, llm, args.beam_width_arg, ( bool )( args.debug_arg ) );

          gettimeofday( &end_time, NULL );

          cout << "finished search in " << diff_time( start_time, end_time ) << " seconds" << endl;   
          for( unsigned int j = 0; j < adcg->solutions().size(); j++ ){
            cout << "  solutions[" << j << "]:" << *adcg->solutions()[ j ].second << " (" << adcg->solutions()[ j ].first << ")" << endl;
          }
   
          if( args.output_given ){
            string filename = args.output_arg;
            if( filename.find( ".xml" ) != string::npos ){
              if( phrases.size() == 1 ){
                adcg->solutions().front().second->to_xml( filename );
              } else {
                boost::trim_if( filename, boost::is_any_of( ".xml" ) );
                stringstream tmp;
                tmp << filename << "_" << setw( 4 ) << setfill( '0' ) << i << ".xml";
                adcg->solutions().front().second->to_xml( tmp.str() );
              }
            }
          }

          if( args.latex_output_given ){
            string filename = args.latex_output_arg;
            if( filename.find( ".tex" ) != string::npos ){
              if( phrases.size() == 1 ){
                adcg->to_latex( filename );
              } else {
                boost::trim_if( filename, boost::is_any_of( ".tex" ) );
                stringstream tmp;
                tmp << filename << "_" << setw( 4 ) << setfill( '0' ) << i << ".tex";
                adcg->to_latex( tmp.str() );
              }
            }
          } 
        }
      cout << "after adcg inference printing out the phrase:" << endl;
      cout << *phrases[ i ] << endl;
      }
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

    if( world != NULL ){
      delete world;
      world = NULL;
    }
  }

  if( adcg != NULL ){
    delete adcg;
    adcg = NULL;
  }

  if( llm != NULL ){
    delete llm;
    llm = NULL;
  }

  if( feature_set != NULL ){
    delete feature_set;
    feature_set = NULL;
  }

  cout << "end of ADCG class demo program" << endl;
  return status;
}
