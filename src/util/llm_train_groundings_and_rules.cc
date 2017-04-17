/**
 * @file    llm_train_groundings_and_rules.cc
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
 * A LLM model training program
 */

#include <iostream>

#include "h2sl/cv.h"
#include "h2sl/grounding_set.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/llm.h"
#include "h2sl/search_space.h"
#include "llm_train_groundings_and_rules_cmdline.h"

using namespace std;
using namespace h2sl;

void
evaluate_model( LLM* llm,
                vector< pair< string, LLM_X > >& examples,
                const bool& showPhrase = false,
                const bool& showFeatures = true ){
  vector< string > cvs;
  cvs.push_back( "false" );
  cvs.push_back( "true" );

  unsigned int num_correct = 0;
  for( unsigned int i = 0; i < examples.size(); i++ ){
    vector< pair< vector< Feature* >, unsigned int > > features;
    double pygx = llm->pygx( examples[ i ].first, examples[ i ].second, cvs, features );
    if( pygx < 0.9 ){
//    if( examples[ i ].first == "true" ){
      cout << "example " << i << " had pygx " << pygx << endl;
      cout << "   filename:\"" << examples[ i ].second.filename() << "\"" << endl;
      cout << "         cv:" << examples[ i ].first << endl;
      cout << "  grounding:" << *examples[ i ].second.grounding() << endl;
      for( unsigned int j = 0; j < examples[ i ].second.children().size(); j++ ){
        if( examples[ i ].second.children()[ j ].first != NULL ){
          cout << "child phrase:(" << *examples[ i ].second.children()[ j ].first << ")" << endl;
        }
        for( unsigned int k = 0; k < examples[ i ].second.children()[ j ].second.size(); k++ ){
          cout << "children[" << j << "]:" << *examples[ i ].second.children()[ j ].second[ k ] << endl;
        }
      }
      if( showPhrase ){
        cout << "     phrase:" << *examples[ i ].second.phrase() << endl;
      }
    
      if( showFeatures ){
        cout << "     features[" << features.size() << "]" << endl;
        for( unsigned int j = 0; j < features.size(); j++ ){
          cout << "      index:" << features[ j ].second << " weight:" << llm->weights()[ features[ j ].second ] << endl;
          for( unsigned int k = 0; k < features[ j ].first.size(); k++ ){
            cout << "        [" << k << "]:" << *features[ j ].first[ k ] << endl;
          }
        }
      }

      cout << endl;
    } else {
      num_correct++;
    }
  }

  cout << ( double )( num_correct ) / ( double )( examples.size() ) * 100.0 << " accuracy (" << num_correct << "/" << examples.size() << ")" << endl; 

  return;
}

int
main( int argc,
      char* argv[] ) {
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  vector< Search_Space* > search_spaces_rules( args.inputs_num, NULL );
  vector< Search_Space* > search_spaces_groundings( args.inputs_num, NULL );
  vector< Phrase* > phrases( args.inputs_num, NULL );
  vector< World* > worlds( args.inputs_num, NULL );
  vector< string > filenames( args.inputs_num );

  Symbol_Dictionary * symbol_dictionary_rules = new Symbol_Dictionary( args.sdr_arg );
  Symbol_Dictionary * symbol_dictionary_groundings = new Symbol_Dictionary( args.sdg_arg );

  vector< pair< string, LLM_X > > examples;
  for( unsigned int i = 0; i < args.inputs_num; i++ ){
    cout << "reading file " << args.inputs[ i ] << endl;
    filenames[ i ] = args.inputs[ i ];

    worlds[ i ] = new World();
    worlds[ i ]->from_xml( args.inputs[ i ] ); 
  
    phrases[ i ] = new Phrase();
    phrases[ i ]->from_xml( args.inputs[ i ], worlds[ i ] ); 

    if( phrases[ i ]->contains_symbol_in_symbol_dictionary( *symbol_dictionary_rules ) ){
      cout << "contains symbols in rules symbol dictionary" << endl;
      search_spaces_rules[ i ] = new Search_Space();
      search_spaces_rules[ i ]->fill_rules( *symbol_dictionary_rules, worlds[ i ] );
      cout << "search_spaces_rules:" << *search_spaces_rules[ i ] << endl;
      search_spaces_rules[ i ]->scrape_examples( filenames[ i ], static_cast< Phrase* >( phrases[ i ] ), worlds[ i ], examples );
    } else {
      cout << "does not contains any rules symbols in symbol dictionary" << endl;
    }
 
    if( phrases[ i ]->contains_symbol_in_symbol_dictionary( *symbol_dictionary_groundings ) ){
      cout << "contains symbols in groundings symbol dictionary" << endl;
      search_spaces_groundings[ i ] = new Search_Space();
      search_spaces_groundings[ i ]->fill_groundings( *symbol_dictionary_groundings, worlds[ i ] ); 
      cout << "search_spaces_groundings:" << *search_spaces_groundings[ i ] << endl;
      search_spaces_groundings[ i ]->scrape_examples( filenames[ i ], static_cast< Phrase* >( phrases[ i ] ), worlds[ i ], examples );
    } else {
      cout << "does not contains any groundings symbols in symbol dictionary" << endl;
    }
  }

  cout << "training with " << examples.size() << " examples" << endl;

  vector< Feature_Set* > feature_sets;
  for( int i = 0; i < args.threads_arg; i++ ){
    feature_sets.push_back( new Feature_Set() );
    feature_sets.back()->from_xml( args.feature_set_arg );
  }
  
  if( !feature_sets.empty() ){
    cout << "num features:" << feature_sets.front()->size() << endl;
  }

  vector< LLM* > llms;
  for( int i = 0; i < args.threads_arg; i++ ){
    llms.push_back( new LLM( feature_sets[ i ] ) );
    llms.back()->weights().resize( llms.back()->feature_set()->size() );
  }

  LLM_Train* llm_train = new LLM_Train( llms );

  llm_train->train( examples, args.max_iterations_arg, args.lambda_arg, args.epsilon_arg );
 
  evaluate_model( llms.front(), examples );

  if( args.output_given ){
    llms.front()->to_xml( args.output_arg );
  }

  for( unsigned int i = 0; i < llms.size(); i++ ){
    if( llms[ i ] != NULL ){
      delete llms[ i ];
      llms[ i ] = NULL;
    }
  }
  llms.clear();
  
  for( unsigned int i = 0; i < feature_sets.size(); i++ ){
    if( feature_sets[ i ] != NULL ){
      delete feature_sets[ i ];
      feature_sets[ i ] = NULL;
    }
  } 
  feature_sets.clear();

  if( llm_train != NULL ){
    delete llm_train;
    llm_train = NULL;
  }

  return 0;
}
