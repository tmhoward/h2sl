/**
 * @file test_process.cc
 *
 * @brief
 *
 * Test Process Training. Uses provided training data partitions.
 * Generates trained log-linear models.
 * 
 * The produced log-linear models are
 * stored as XML and appended to the end 
 * of the corresponding input file
 *
 */

#include "test_process_training_cmdline.h"
#include <boost/filesystem/path.hpp>

#include <assert.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <libxml/tree.h>
#include <glib.h>

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

//#include "h2sl/dcg.h"
//#include "h2sl/hdcg.h"
//#include "h2sl/adcg.h"
//#include "h2sl/hadcg.h"

using namespace std;
using namespace h2sl; 

/**
 * Evaluate the model. 
 */
void
evaluate_model( LLM* llm,
                vector< pair< string, LLM_X > >& examples ){
  vector< string > cvs;
  cvs.push_back( "false" );
  cvs.push_back( "true" );

  unsigned int num_correct = 0;
  for( unsigned int i = 0; i < examples.size(); i++ ){
    vector< pair< vector< Feature* >, unsigned int > > features;
    double pygx = llm->pygx( examples[ i ].first, examples[ i ].second, cvs, features );
    if( pygx < 0.75 ){
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
      cout << "     phrase:" << *examples[ i ].second.phrase() << endl;

      cout << "     features[" << features.size() << "]" << endl;
      for( unsigned int j = 0; j < features.size(); j++ ){
          cout << "      ";
        for( unsigned int k = 0; k < features[ j ].first.size(); k++ ){
          cout << "(" << *features[ j ].first[ k ] << ")";
          if( k != ( features[ j ].first.size() - 1 ) ){
            cout << ",";
          }
        }
        cout << ") index:" << features[ j ].second << " weight:" << llm->weights()[ features[ j ].second ] << endl;
      }

      cout << endl;
    } else {
      num_correct++;
    }
  }

  cout << ( double )( num_correct ) / ( double )( examples.size() ) * 100.0 << " accuracy (" << num_correct << "/" << examples.size() << ")" << endl;

  return;
}

/**
 * Program to generate the trained log-linear models for the provided cross-validation data partitions
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of test_process_llm_train program" << endl;

  // Determine the arguments provided.
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }

  /********** Train a llm for each provided training_set partition && 
              save it to a new xml file that also contains the data partitions *****************/
  for( unsigned int i = 0; i < args.inputs_num; i++ ){
    // Vector to store list of training set filenames
    vector< string > training_set;

    // XML pointers
    xmlDoc * input_doc = NULL;
    xmlNodePtr input_root = NULL;
    input_doc = xmlReadFile( args.inputs[ i ], NULL, 0 );

    /** Separate out the training and test set files. Each contain world and the instruction **/
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
                      // push_back the training set file name.
                      training_set.push_back( ( char* )( tmp ) );
                      xmlFree( tmp );
                    }
                  }
                }
              }
            }
          }
        }
      }
      
      /******************** Report ******************************************/
      // Test number (partition of the corpus), training set size.
      // get the test number from the filename
      string filename = args.inputs[ i ];
      string test_number_string = filename.substr( ( filename.size() - 8 ), 4 );
      cout << "test " << test_number_string << endl;
      cout << "  training_set[" << training_set.size() << "]:{";
      for( unsigned int j = 0; j < training_set.size(); j++ ){
        cout << training_set[ j ];
        if( j != ( training_set.size() - 1 ) ){
          cout << ",";
        }
      }
      cout << "}" << endl;
   
      /********************** Train the Log-linear model *****************/ 
      // Load examples from the training set for training the log-linear model.  
      vector< pair< string, LLM_X > > examples;
      cout << "training with " << examples.size() << " examples" << endl;

      // Feature sets in multiple threads.
      vector< Feature_Set* > feature_sets;
      for( unsigned int i = 0; i < ( unsigned int ) args.threads_arg; i++ ){
        feature_sets.push_back( new Feature_Set() );
        feature_sets.back()->from_xml( args.feature_set_arg );
        if( !feature_sets.empty() ){
          cout << "num features:" << feature_sets.back()->size() << endl;
        }
      }

      // Add feature sets to LLMs.
      vector< LLM* > llms;
      for( unsigned int i = 0; i < ( unsigned int ) args.threads_arg; i++ ){
        llms.push_back( new LLM( feature_sets[ i ] ) );
        llms.back()->weights().resize( llms.back()->feature_set()->size() );
      }

      LLM_Train* llm_train = new LLM_Train( llms );

      // Train the model && record the timing information.
      uint64_t train_start_time = current_time();
      llm_train->train( examples, args.max_iterations_arg, args.lambda_arg, args.epsilon_arg );
      int64_t train_end_time = current_time();
      double train_time = microseconds_to_seconds( train_end_time - train_start_time );

      /********* Append the LLM to the input file and save as new file**************/
      // append the llm to the xml document object
      llms.front()->to_xml( input_doc, input_root );

      // create the name for the new file
      // test_number_string comes from the "report" code above
      stringstream input_file_with_llm;
      input_file_with_llm << args.output_arg << "/test_with_llm" << test_number_string << ".xml";
      
      //save the file
      xmlSaveFormatFileEnc( input_file_with_llm.str().c_str(), input_doc, "UTF-8", 1 );
      

      /********************* Evaluate the log-linear model ******************/
      cout << "evaluating llm model" << endl;
      evaluate_model( llms.front(), examples );
 
      /**** Clean up  *****/
      if( llm_train != NULL ){
        delete llm_train;
        llm_train = NULL;
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

    }
    //free the xml pointers
    if( input_doc != NULL ){
      xmlFreeDoc( input_doc );
    }
  }

  cout << "end of test_process_training program" << endl;
  return 0;
}

