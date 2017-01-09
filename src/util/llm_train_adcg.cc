/**
 * @file llm_train_adcg.cc
*
 * A LLM model training program for ADCG
 */

#include <iostream>
#include "assert.h"

#include "h2sl/cv.h"
#include "h2sl/llm.h"
#include "h2sl/phrase.h"
#include "h2sl/world.h"

#include "h2sl/adcg_base.h"
#include "h2sl/grounding_set.h"
#include "h2sl/feature.h"
#include "h2sl/feature_set.h"

#include "h2sl/object.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/container.h"
#include "h2sl/region_container.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_abstract_container.h"
#include "h2sl/object_property.h"
#include "h2sl/index.h"
#include "h2sl/number.h"
#include "h2sl/object_type.h"
#include "h2sl/object_color.h"

#include "llm_train_adcg_cmdline.h"

using namespace std;
using namespace h2sl;

void
evaluate_model( h2sl::LLM* llm,
                vector< pair< unsigned int, LLM_X > >& examples ){
  vector< unsigned int > cvs;
  cvs.push_back( h2sl::CV_FALSE );
  cvs.push_back( h2sl::CV_TRUE );

  unsigned int num_correct = 0;
  for( unsigned int i = 0; i < examples.size(); i++ ){
    vector< h2sl::Feature* > features;
    double pygx = llm->pygx( examples[ i ].first, examples[ i ].second, cvs, features );
    if( pygx < 0.75 ){
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
      cout << "     phrase:" << examples[ i ].second.phrase()->words_to_std_string() << endl;
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

  // Select the model
  vector< ADCG_Base* > dcgs( args.inputs_num, NULL );

  vector< Phrase* > phrases( args.inputs_num, NULL );
  vector< World* > worlds( args.inputs_num, NULL );
  vector< string > filenames( args.inputs_num );
  vector< pair< unsigned int, h2sl::LLM_X > > examples;


  if( args.input_given ){
    filenames.clear();
    dcgs.clear();
    worlds.clear();
    phrases.clear();

    xmlDoc * input_doc = NULL;
    xmlNodePtr input_root = NULL;
    input_doc = xmlReadFile( args.input_arg, NULL, 0 );
    if( input_doc != NULL ){
      input_root = xmlDocGetRootElement( input_doc );
      if( input_root->type == XML_ELEMENT_NODE ){
        for( xmlNodePtr l1 = input_root->children; l1; l1 = l1->next ){
          if( l1->type == XML_ELEMENT_NODE ){
            if( xmlStrcmp( l1->name, ( const xmlChar* )( "training_set" ) ) == 0 ){
              for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
                if( l2->type == XML_ELEMENT_NODE ){
                  if( xmlStrcmp( l2->name, ( const xmlChar* )( "filename" ) ) == 0 ){
                    xmlChar * tmp = xmlGetProp( l2, ( const xmlChar* )( "text" ) );
                    if( tmp != NULL ){


                     string filename = ( char* )( tmp );
                      cout << "loading \"" << filename << "\"" << endl;
                      filenames.push_back( filename );
                      worlds.push_back( new World() );
                      worlds.back()->from_xml( filenames.back() );


                      phrases.push_back( new Phrase() );
                      phrases.back()->from_xml( filenames.back() );

      		      // Symbol grounding model used.
                      dcgs.push_back( new ADCG_Base() );
                      dcgs.back()->fill_search_spaces( worlds.back() );

                      // Scrape examples. 
                      ADCG_Base::scrape_examples( filenames.back(), static_cast< Phrase* >( phrases.back() ), worlds.back(), 
                                                  dcgs.back()->search_spaces(), dcgs.back()->correspondence_variables(), examples );

                      xmlFree( tmp );
                    }
                  }
                }
              }
            }
          }
        }
        xmlFreeDoc( input_doc );
      }
    }
  } else {
    for( unsigned int i = 0; i < args.inputs_num; i++ ){
      filenames[ i ] = args.inputs[ i ];
      cout << "reading file " << args.inputs[ i ] << endl;

      worlds[ i ] = new World(); 
      worlds[ i ]->from_xml( args.inputs[ i ] );

      worlds[ i ] = new World();
      worlds[ i ]->from_xml( args.inputs[ i ] );

      phrases[ i ] = new Phrase();
      phrases[ i ]->from_xml( args.inputs[ i ] );

     // Symbol grounding model used.
      dcgs[ i ] = new ADCG_Base();

      dcgs[ i ]->fill_search_spaces( worlds[ i ] );

      ADCG_Base::scrape_examples( filenames[ i ], phrases[ i ], worlds[ i ], 
                                 dcgs[ i ]->search_spaces(), 
                                 dcgs[ i ]->correspondence_variables(), 
                                 examples );
    }

  }

  cout << "training with " << examples.size() << " examples" << endl;

  vector< Feature_Set* > feature_sets;
  for( unsigned int i = 0; i < (unsigned int) args.threads_arg; i++ ){
    feature_sets.push_back( new Feature_Set() );
    feature_sets.back()->from_xml( args.feature_set_arg );
    //cout << "num_features:"  << feature_sets.back()->size() << endl;
  }

   if( !feature_sets.empty() ){
    cout << "num features:" << feature_sets.front()->size() << endl;
  }

  vector< LLM* > llms;
  for( unsigned int i = 0; i < ( unsigned int) args.threads_arg; i++ ){
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
   
