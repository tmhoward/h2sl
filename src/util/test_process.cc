/**
 * @file test_process.cc
 *
 * @brief
 *
 * Test process. Uses a split training and test data set.
 * Runs the DCG and DCG_SH (ADCG) models and evaluates
 * Comparative accuracies. 
 * 
 * The cumulated results and 
 * individual grounding results are written out.
 * 
 * Done for both root and complete accuracy metrics. 
 *
 * ToDo: also add the H-DCG and H-ADCG models for evaluation.
 */

#include "test_process_cmdline.h"
#include <boost/filesystem/path.hpp>

#include <assert.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <libxml/tree.h>

#include "h2sl/common.h"
#include "h2sl/cv.h"
#include "h2sl/llm.h"
#include "h2sl/feature.h"
#include "h2sl/feature_set.h"
#include "h2sl/grounding.h"
#include "h2sl/grounding_set.h"
#include "h2sl/world.h"
#include "h2sl/phrase.h"

#include "h2sl/abstract_container.h"
#include "h2sl/container.h"
#include "h2sl/container_type.h"
#include "h2sl/index.h"
#include "h2sl/number.h"
#include "h2sl/object.h"
#include "h2sl/object_color.h"
#include "h2sl/object_property.h"
#include "h2sl/object_type.h"
#include "h2sl/region_abstract_container.h"
#include "h2sl/region_container.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/spatial_relation.h"

#include "h2sl/dcg.h"
#include "h2sl/hdcg.h"
#include "h2sl/adcg.h"
#include "h2sl/hadcg.h"

using namespace std;
using namespace h2sl; 

/**
 * Root: Compare Phrases 
 */
bool
root_compare_phrases( const Phrase& a,
                      const Phrase& b ){
  assert( dynamic_cast< const Grounding_Set* >( a.grounding_set() ) != NULL );
  assert( dynamic_cast< const Grounding_Set* >( b.grounding_set() ) != NULL );

  bool value = ( *( static_cast< const Grounding_Set* >( a.grounding_set() ) ) == *( static_cast< const Grounding_Set* >( b.grounding_set() ) ) );

  return value;
}

/**
 * Complete: Compare Phrases 
 */
bool
complete_compare_phrases( const Phrase& a,
                          const Phrase& b ){
  assert( dynamic_cast< const Grounding_Set* >( a.grounding_set() ) != NULL );
  assert( dynamic_cast< const Grounding_Set* >( b.grounding_set() ) != NULL );

  bool value = true;
  value = value && ( a.children().size() == b.children().size() );
  if( a.children().size() == b.children().size() ){
    for( unsigned int i = 0; i < a.children().size(); i++ ){
      value = value && complete_compare_phrases( *a.children()[ i ], *b.children()[ i ] );
    }
  }

  value = value && ( *( static_cast< const Grounding_Set* >( a.grounding_set() ) ) == *( static_cast< const Grounding_Set* >( b.grounding_set() ) ) );

  return value;
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


void
evaluate_model( LLM* llm,
                vector< pair< string, LLM_X > >& examples ){
  vector< string > cvs;
  cvs.push_back( "false" );
  cvs.push_back( "true" );

  unsigned int num_correct = 0;
  for( unsigned int i = 0; i < examples.size(); i++ ){
    vector< pair < vector< Feature*>, unsigned int > > features;
   //vector< h2sl::Feature* > features;
    double pygx = llm->pygx( examples[ i ].first, examples[ i ].second, cvs, features );
    if( pygx < 0.5 ){
      cout << "example " << i << " had pygx " << pygx << endl;
      cout << "   filename:\"" << examples[ i ].second.filename() << "\"" << endl;
      cout << "         cv:" << examples[ i ].first << endl;
      if( dynamic_cast< const Grounding* >( examples[ i ].second.grounding() ) != NULL ){
        cout << "  grounding:" << *static_cast< const Grounding* >( examples[ i ].second.grounding() ) << endl;
      }

      for( unsigned int j = 0; j < examples[ i ].second.children().size(); j++ ){
        if( examples[ i ].second.children()[ j ].first != NULL ){
          cout << "child phrase:(" << *examples[ i ].second.children()[ j ].first << ")" << endl;
        }
        for( unsigned int k = 0; k < examples[ i ].second.children()[ j ].second.size(); k++ ){
          if( dynamic_cast< const Grounding* >( examples[ i ].second.children()[ j ].second[ k ] ) != NULL ){
            cout << "children[" << j << "][" << k << "]:" << *static_cast< const Grounding* >( examples[ i ].second.children()[ j ].second[ k ] ) << endl;
          }
        }
      }
      cout << "     phrase:" << *dynamic_cast< const Phrase* >( examples[ i ].second.phrase() ) << endl;
    } else {
      num_correct++;
    }
  }

  cout << ( double )( num_correct ) / ( double )( examples.size() ) * 100.0 << " accuracy (" << num_correct << "/" << examples.size() << ")" << endl;

  return;
}



/**
 * Test Generator Program
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of test_process program" << endl;

  // Determine the arguments provided.
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }

  cout << "end of test_process program" << endl;
  return 0;
}

