/**
 * @file    factor_set_demo.cc
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
 * A Factor class demo program
 */

#include <iostream>
#include "h2sl/region.h"
#include "h2sl/phrase.h"
#include "h2sl/world.h"
#include "h2sl/factor_set.h"
#include "factor_set_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Factor class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  vector< vector< unsigned int > > correspondence_variables;
  
  correspondence_variables.push_back( vector< unsigned int >() );
  correspondence_variables.back().push_back( CV_TRUE );
  correspondence_variables.back().push_back( CV_FALSE );

  vector< pair< unsigned int, Grounding* > > search_space;

  for( unsigned int i = 0; i < NUM_OBJECT_TYPES; i++ ){
    search_space.push_back( make_pair< unsigned int, Grounding* >( 0, NULL ) );
    search_space.back().second =  new Region( REGION_TYPE_UNKNOWN, Object( "na", ( object_type_t )( i ) ) );
  }

  Phrase * phrase = new Phrase();
  if( args.phrase_given ){
    phrase->from_xml( args.phrase_arg );
  }

  cout << "phrase:" << *phrase << endl;

  World * world = new World();
  if( args.world_given ){
    world->from_xml( args.world_arg );
  }

  Feature_Set * feature_set = new Feature_Set();
  LLM * llm = new LLM( feature_set );
  if( args.llm_given ){
    llm->from_xml( args.llm_arg );
  }

  Factor_Set * factor_set = new Factor_Set( phrase );

  factor_set->search( search_space, correspondence_variables, world, llm );

  cout << "factor_set:" << *factor_set << endl;

  if( factor_set != NULL ){
    delete factor_set;
    factor_set = NULL;
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

  if( phrase != NULL ){
    delete phrase;
    phrase = NULL;
  }

  for( unsigned int i = 0; i < search_space.size(); i++ ){
    if( search_space[ i ].second != NULL ){
      delete search_space[ i ].second;
      search_space[ i ].second = NULL;
    }
  }

  cout << "end of Factor class demo program" << endl;
  return status;
}
