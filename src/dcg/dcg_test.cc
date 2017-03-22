/**
 * @file    dcg_test.cc
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
 * A DCG class test program
 */

#include <iostream>
#include <sys/time.h>
#include "h2sl/common.h"
#include "h2sl/phrase.h"
#include "h2sl/constraint.h"
#include "h2sl/grounding_set.h"
#include "h2sl/parser_cyk.h"
#include "h2sl/dcg.h"
#include "dcg_test_cmdline.h"

using namespace std;
using namespace h2sl;

bool
compare_phrases( const Phrase* first, 
                  const Phrase* second ){
  cout << "in compare_phrases" << endl;
  if( ( first != NULL ) && ( second != NULL ) ){
    const Grounding_Set * first_grounding_set = dynamic_cast< const Grounding_Set* >( first->grounding_set() );
    const Grounding_Set * second_grounding_set = dynamic_cast< const Grounding_Set* >( second->grounding_set() );
    if( ( first_grounding_set != NULL ) && ( second_grounding_set != NULL ) ){
      if( first_grounding_set->groundings().size() == second_grounding_set->groundings().size() ){
        for( unsigned int i = 0; i < first_grounding_set->groundings().size(); i++ ){
          if( dynamic_cast< const Constraint* >( first_grounding_set->groundings()[ i ] ) != NULL ){
            const Constraint* first_grounding_constraint = static_cast< const Constraint* >( first_grounding_set->groundings()[ i ] );
            bool found_match = false;
            for( unsigned int j = 0; j < second_grounding_set->groundings().size(); j++ ){
              if( dynamic_cast< const Constraint* >( second_grounding_set->groundings()[ j ] ) != NULL ){
                const Constraint* second_grounding_constraint = static_cast< const Constraint* >( second_grounding_set->groundings()[ j ] );
                if( *first_grounding_constraint == *second_grounding_constraint ){
                  found_match = true;
                }
              }
            }
            if( !found_match ){
              cout << "no match found, leaving compare_phrases" << endl;
              return false;
            }
          }
        }
      } else {
        cout << "grounding_sets were different sizes, leaving compare_phrases" << endl;
        return false;
      }
    } else {
      cout << "a grounding_set was NULL, leaving compare_phrases" << endl;
      return false;
    } 
  } else {
    cout << "a phrase was NULL, leaving compare_phrases" << endl;
    return false;
  }
  cout << "found a match, leaving compare_phrases" << endl;
  return true;
}

/**
 * Clear the phrase. 
 */
void
clear( Phrase* phrase ){
  if( phrase != NULL ){
    if( phrase->grounding_set() != NULL ){
      delete_ptr< Grounding_Set >( phrase->grounding_set() );
    }
    for( unsigned int i = 0; i < phrase->children().size(); i++ ){
      clear( phrase->children()[ i ] );
    }
  }
  return;
}


int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of DCG class test program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  Parser< Phrase > * parser = new Parser_CYK< Phrase >();
  Grammar * grammar = new Grammar();
  if( args.grammar_given ){
    grammar->from_xml( args.grammar_arg );
  }

  Feature_Set * feature_set = new Feature_Set();
  LLM * llm = new LLM( feature_set );
  if( args.llm_given ){
    llm->from_xml( args.llm_arg );
  }

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary( args.symbol_dictionary_groundings_arg );

  Search_Space * search_space = new Search_Space();

  DCG * dcg = new DCG();

  unsigned int num_correct = 0;
  unsigned int num_incorrect = 0;

  for( unsigned int i = 0; i < args.inputs_num; i++ ){
    cout << "reading file " << args.inputs[ i ] << endl;
    string instruction = extract_instruction( args.inputs[ i ] );
    cout << "  found instruction \"" << instruction << "\"" << endl;

    Grounding * context = NULL;

    World * world = new World();  
    world->from_xml( args.inputs[ i ] );

    Phrase * truth = new Phrase();
    truth->from_xml( args.inputs[ i ] );
            
    // update the search space when the world changes
    search_space->fill_groundings( *symbol_dictionary, world );
    cout << "search_space:" << *search_space << endl;

    vector< Phrase* > phrases;
    
    //conditional flow to allow use of parser or not
    if( !args.grammar_given ){    
      cout << "grammar not provided, scraping example..." << endl;
      phrases.push_back( new Phrase() );
      phrases.back() = truth->dup();
      clear( phrases.back() );
      dcg->leaf_search( phrases.back(), *symbol_dictionary, search_space, world, context, llm, args.beam_width_arg, ( bool )( args.debug_arg ) );
      if( !dcg->solutions().empty() ){
        cout << "solution: " << *dcg->solutions().front().second << " (" << dcg->solutions().front().first << ")" << endl;
        if( compare_phrases( truth, dcg->solutions().front().second ) ){
          cout << "solution matches" << endl;
          num_correct++;
        } else{
          cout << "solution does not match" << endl;
          num_incorrect++;
        }
      }
    } else{
      cout << "grammar provided, parsing..." << endl;
      if( parser->parse( *grammar, instruction, phrases ) ){
        if( !phrases.empty() ){
          cout << "found " << phrases.size() << " phrases" << endl;
          cout << "  truth:" << *truth << endl;
          bool found_match = false;
          unsigned int match_index = 0;
          for( unsigned int i = 0; i < phrases.size(); i++ ){
            if( phrases[ i ] != NULL ){
              dcg->leaf_search( phrases[ i ], *symbol_dictionary, search_space, world, context, llm, args.beam_width_arg, ( bool )( args.debug_arg ) );
              if( !dcg->solutions().empty() ){
                cout << "  parse[" << i << "]:" << *dcg->solutions().front().second << " (" << dcg->solutions().front().first << ")" << endl;
                if( compare_phrases( truth, dcg->solutions().front().second ) ){
                  found_match = true;
                  match_index = i;
                }
              }
            }
          }
          if( found_match ){
            cout << "  phrase[" << match_index << "] matches" << endl;
            num_correct++;
          } else {
            cout << "  did not find match" << endl;
            num_incorrect++;
            exit(0);
          }
        }
      } else {
        cout << "  could not parse \"" << instruction << "\"" << endl;
        num_incorrect++;
        exit(0);
      }
    }

    stringstream output_filename;
    output_filename << "/tmp/input_" << setw( 4 ) << setfill( '0' ) << i << "_dcg_solution.xml";
    dcg->solutions().front().second->to_xml( output_filename.str() );

    //memory cleanup
    cout << "beginning memory cleanup" << endl;
    cout << "deleting phrases[]" << endl;
    for( unsigned int i = 0; i < phrases.size(); i++ ){
      if( phrases[ i ] != NULL ){
        delete phrases[ i ];
        phrases[ i ] = NULL;
      }
    }

    cout << "deleting truth" << endl;
    if( truth != NULL ){
      delete truth;
      truth = NULL;
    }

    cout << "deleting world" << endl;
    if( world != NULL ){
      delete world;
      world = NULL;
    } 
    cout << "end of loop" << endl;
  }

  cout << "correctly inferred " << num_correct << " of " << num_correct + num_incorrect << " examples (" << ( double )( num_correct ) / ( double )( num_correct + num_incorrect ) * 100.0 << "%)" << endl;

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

  if( grammar != NULL ){
    delete grammar;
    grammar = NULL;
  }

  if( parser != NULL ){
    delete parser;
    parser = NULL;
  }

  cout << "end of DCG class test program" << endl;
  return status;
}
