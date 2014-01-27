/**
 * @file    llm_train.cc
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

#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/llm.h"
#include "h2sl/dcg.h"
#include "llm_train_cmdline.h"

using namespace std;
using namespace h2sl;

void
evaluate_model( LLM* llm,
                vector< pair< unsigned int, LLM_X > >& examples ){
  vector< unsigned int > cvs;
  cvs.push_back( CV_FALSE );
  cvs.push_back( CV_TRUE );

  unsigned int num_correct = 0;
  for( unsigned int i = 0; i < examples.size(); i++ ){
    double pygx = llm->pygx( examples[ i ].first, examples[ i ].second, cvs );
    if( pygx < 0.75 ){
      cout << "example " << i << " had pygx " << pygx << endl;
      cout << "         cv:" << examples[ i ].first << endl;
      if( dynamic_cast< Region* >( examples[ i ].second.grounding() ) != NULL ){
        cout << "  grounding:" << *static_cast< Region* >( examples[ i ].second.grounding() ) << endl; 
      } else if ( dynamic_cast< Constraint* >( examples[ i ].second.grounding() ) != NULL ){
        cout << "  grounding:" << *static_cast< Constraint* >( examples[ i ].second.grounding() ) << endl; 
      }
      for( unsigned int j = 0; j < examples[ i ].second.children().size(); j++ ){
        if( dynamic_cast< Region* >( examples[ i ].second.children()[ j ] ) != NULL ){
          cout << "children[" << j << "]:" << *static_cast< Region* >( examples[ i ].second.children()[ j ] ) << endl;
        } else if( dynamic_cast< Constraint* >( examples[ i ].second.children()[ j ] ) != NULL ){
          cout << "children[" << j << "]:" << *static_cast< Constraint* >( examples[ i ].second.children()[ j ] ) << endl;
        }
      }
      cout << "     phrase:" << *examples[ i ].second.phrase() << endl << endl;
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

  LLM * llm = new LLM();
  llm->feature_set().from_xml( args.feature_set_arg );
  llm->weights().resize( llm->feature_set().size() );

  DCG * dcg = new DCG();

  vector< pair< unsigned int, LLM_X > > examples;
  for( unsigned int i = 0; i < args.inputs_num; i++ ){
    cout << "reading file " << args.inputs[ i ] << endl;
    World * world = new World();
    world->from_xml( args.inputs[ i ] );
    
    Phrase * phrase = new Phrase();
    phrase->from_xml( args.inputs[ i ] );

    dcg->construct( *phrase, *world, *llm, true );

    for( unsigned int j = 0; j < dcg->factors().size(); j++ ){
      examples.push_back( pair< unsigned int, LLM_X >() );
      examples.back().first = dcg->factors()[ j ]->cv();
      examples.back().second.grounding() = dcg->factors()[ j ]->grounding()->dup();
      for( unsigned int k = 0; k < dcg->factors()[ j ]->children().size(); k++ ){
        if( dcg->factors()[ dcg->factors()[ j ]->children()[ k ]->solution_index() ]->cv() == CV_TRUE ){
          examples.back().second.children().push_back( dcg->factors()[ dcg->factors()[ j ]->children()[ k ]->solution_index() ]->grounding()->dup() );
        }
      }
      examples.back().second.phrase() = dcg->factors()[ j ]->phrase()->dup(); 
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

  if( dcg != NULL ){
    delete dcg;
    dcg = NULL;
  }

  llm->train( examples );
 
  evaluate_model( llm, examples );

  if( args.output_given ){
    llm->to_xml( args.output_arg );
  }

  if( llm != NULL ){
    delete llm;
    llm = NULL;
  }

  return 0;
}
