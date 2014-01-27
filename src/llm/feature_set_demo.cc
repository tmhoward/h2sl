/**
 * @file    feature_set_demo.cc
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
 * A Feature_Set class demo program
 */

#include <iostream>

#include "h2sl/grammar.h"
#include "h2sl/object.h"
#include "h2sl/region.h"
#include "h2sl/cv.h"
#include "h2sl/feature_cv.h"
#include "h2sl/feature_word.h"
#include "h2sl/feature_object.h"
#include "h2sl/feature_region_object.h"
#include "h2sl/feature_region.h"
#include "h2sl/feature_set.h"
#include "feature_set_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Feature_Set class demo program" << endl;
  
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  Feature_Set * feature_set = new Feature_Set();

  if( args.input_given ){
    cout << "reading feature_set from " << args.input_arg << endl;
    feature_set->from_xml( args.input_arg );
  }

  if( args.grammar_given ){
    cout << "generating feature_set from grammar file " << args.grammar_arg << endl;
    Grammar * grammar = new Grammar();
    grammar->from_xml( args.grammar_arg );

    feature_set->feature_groups().push_back( vector< Feature* >() );
    for( unsigned int i = CV_FALSE; i < NUM_BINARY_CVS; i++ ){
      feature_set->feature_groups().back().push_back( new Feature_CV( false, i ) );
    }
    
    feature_set->feature_groups().push_back( vector< Feature* >() );
    for( unsigned int i = 0; i < grammar->terminals().size(); i++ ){
      for( unsigned int j = 0; j < grammar->terminals()[ i ].words().size(); j++ ){
        feature_set->feature_groups().back().push_back( new Feature_Word( false, Word( pos_t_from_std_string( grammar->terminals()[ i ].symbol() ), grammar->terminals()[ i ].words()[ j ] ) ) );
      }
    }

    feature_set->feature_groups().push_back( vector< Feature* >() );
    for( unsigned int i = 1; i < NUM_OBJECT_TYPES; i++ ){
      feature_set->feature_groups().back().push_back( new Feature_Object( false, i ) );
    }

    for( unsigned int i = 1; i < NUM_OBJECT_TYPES; i++ ){
      feature_set->feature_groups().back().push_back( new Feature_Region_Object( false, i ) );
    }

    for( unsigned int i = 1; i < NUM_REGION_TYPES; i++ ){
      feature_set->feature_groups().back().push_back( new Feature_Region( false, i ) );
    }

    cout << "feature_set->size(): " << feature_set->size() << endl;

    if( grammar != NULL ){
      delete grammar;
      grammar = NULL;
    } 
  }

  if( args.output_given ){
    cout << "writing feature_set to " << args.output_arg << endl;
    feature_set->to_xml( args.output_arg );
  }

  if( feature_set != NULL ){
    delete feature_set;
    feature_set = NULL;
  }

  cout << "end of Feature_Set class demo program" << endl;
  return status;
}
