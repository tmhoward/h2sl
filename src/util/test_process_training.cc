/**
 * @file test_process.cc
 *
 * @brief
 *
 * Test process. Uses a split training and test data set.
 * Runs the DCG and ADCG (ADCG) models and evaluates
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

#include "h2sl/dcg.h"
#include "h2sl/hdcg.h"
#include "h2sl/adcg.h"
#include "h2sl/hadcg.h"

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
    //if( pygx < 0.75 ){
    if( false ){
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

    // Vectors store list of training set filenames
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
      cout << "test " << i << endl << endl;
      cout << "training_set[" << training_set.size() << "]:{";
      for( unsigned int j = 0; j < training_set.size(); j++ ){
        cout << training_set[ j ];
        if( j != ( training_set.size() - 1 ) ){
          cout << ",";
        }
      }
      cout << "}" << endl;
   
      /********************** Train the Log-linear model *****************/ 
      vector< Search_Space* > training_search_spaces_groundings( training_set.size(), NULL );
      vector< Search_Space* > training_search_spaces_rules( training_set.size(), NULL );

      vector< Phrase* > truth_training_phrases( training_set.size(), NULL );
      vector< World* > training_worlds( training_set.size(), NULL );
      vector< string > training_filenames( training_set.size() );

      Symbol_Dictionary * symbol_dictionary_groundings = new Symbol_Dictionary( args.symbol_dictionary_groundings_arg );
      Symbol_Dictionary * symbol_dictionary_rules = new Symbol_Dictionary( args.symbol_dictionary_rules_arg );
     
      // Load examples from the training set for training the log-linear model.  
      vector< pair< string, LLM_X > > examples;
      for( unsigned int j = 0; j < training_set.size(); j++ ){
        // Get the filename.
        training_filenames[ j ] = training_set[ j ];
        cout << "reading file " << training_set[ j ] << endl;

        // Load world
        training_worlds[ j ] = new World();
        training_worlds[ j ]->from_xml( training_set[ j ] );

        // Load phrase
        truth_training_phrases[ j ] = new Phrase();
        truth_training_phrases[ j ]->from_xml( training_set[ j ], training_worlds[ j ] );

        /************ Search Space fill rules and scrape examples ************************/
        if( truth_training_phrases[ j ]->contains_symbol_in_symbol_dictionary( *symbol_dictionary_rules ) ){
          cout << "contains symbols in rules symbol dictionary" << endl;
          training_search_spaces_rules[ j ] = new Search_Space();
          training_search_spaces_rules[ j ]->fill_rules( *symbol_dictionary_rules, training_worlds[ j ] );
          training_search_spaces_rules[ j ]->scrape_examples( training_filenames[ j ], static_cast< Phrase* >( truth_training_phrases[ j ] ), training_worlds[ j ], examples );
        } else {
          cout << "does not contains any rules symbols in symbol dictionary" << endl;
        }

        /************ Search Space fill groundings and scrape examples. Note: examples is common. ***/
        // Check if the phrase has symbols from the symbol dictionary groundings.
        if( truth_training_phrases[ j ]->contains_symbol_in_symbol_dictionary( *symbol_dictionary_groundings ) ){
          cout << "contains symbols in symbol dictionary" << endl;
          training_search_spaces_groundings[ j ] = new Search_Space();
          training_search_spaces_groundings[ j ]->fill_groundings( *symbol_dictionary_groundings, training_worlds[ j ] );
          training_search_spaces_groundings[ j ]->scrape_examples( training_filenames[ j ], static_cast< Phrase* >( truth_training_phrases[ j ] ), training_worlds[ j ], examples );
        } else {
          cout << "does not contains any symbols in symbol dictionary" << endl;
        }

      }
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
      if( !feature_sets.empty() ){
       cout << "num features:" << feature_sets.front()->size() << endl;
      }

      // Add feature sets to LLMs.
      vector< LLM* > llms;
      for( unsigned int i = 0; i < ( unsigned int ) args.threads_arg; i++ ){
        llms.push_back( new LLM( feature_sets[ i ] ) );
        llms.back()->weights().resize( llms.back()->feature_set()->size() );
      }

      LLM_Train* llm_train = new LLM_Train( llms );

      // Main LLM Training call.
      uint64_t train_start_time = current_time();
      llm_train->train( examples, args.max_iterations_arg, args.lambda_arg, args.epsilon_arg );
      int64_t train_end_time = current_time();
      double train_time = microseconds_to_seconds( train_end_time - train_start_time );

      /********* Write out the LLM   **************/
      stringstream llm_filename_string;
      if( !args.solution_directory_given ){
        llm_filename_string << "/tmp/llm_" << setw( 4 ) << setfill( '0' ) << i << ".xml";
      } else{
        llm_filename_string << solution_directory.str() << "/llm_" << setw( 4 ) << setfill( '0' ) << i << ".xml";
      }
      
      // write the llm to xml
      llms.front()->to_xml( llm_filename_string.str() );

      stringstream training_set_size_string;
      training_set_size_string << training_set.size();
      xmlNewProp( test_node, ( const xmlChar* )( "training_set_size" ), ( const xmlChar* )( training_set_size_string.str().c_str() ) );

      stringstream test_set_size_string;
      test_set_size_string << test_set.size();
      xmlNewProp( test_node, ( const xmlChar* )( "test_set_size" ), ( const xmlChar* )( test_set_size_string.str().c_str() ) );

      stringstream training_ratio_string;
      training_ratio_string << ( double )( training_set.size() ) / ( double )( training_set.size() + test_set.size() );
      xmlNewProp( test_node, ( const xmlChar* )( "training_ratio" ), ( const xmlChar* )( training_ratio_string.str().c_str() ) );

      stringstream train_time_string;
      train_time_string << train_time;
      xmlNewProp( test_node, ( const xmlChar* )( "train_time" ), ( const xmlChar* )( train_time_string.str().c_str() ) );

      /********************* Evaluate the log-linear model ******************/
      cout << "evaluating llm model" << endl;
      evaluate_model( llms.front(), examples );
 
      /********************* Run Inference Tests******************/
      // Evaluate the DCG and ADCG models on the training set (if option).
      if (args.test_training_set_arg) {
        run_tests( training_set, llms.front(), tests_doc, test_node, "training_set", i, solution_directory.str(),
                   args.symbol_dictionary_groundings_arg, args.beam_width_arg, args.debug_arg );
        cout << "training_ratio:" << training_ratio_string.str() << endl << endl;
      }

      // Evaluate the DCG and ADCG models on the test set (if option).
      if (args.test_test_set_arg) {
        run_tests( training_set, llms.front(), tests_doc, test_node, "test_set", i, solution_directory.str(),
                   args.symbol_dictionary_groundings_arg, args.beam_width_arg, args.debug_arg );
        cout << "training_ratio:" << training_ratio_string.str() << endl << endl;
      }
     
      xmlAddChild( tests_node, test_node );

      /**** Clean up  *****/
      if( llm_train != NULL ){
        delete llm_train;
        llm_train = NULL;
      }

      if( symbol_dictionary_groundings != NULL ){
        delete symbol_dictionary_groundings;
        symbol_dictionary_groundings = NULL;
      }

      for( unsigned int i = 0; i < training_search_spaces_groundings.size(); i++ ){
        if( training_search_spaces_groundings[ i ] != NULL ){
          delete training_search_spaces_groundings[ i ];
          training_search_spaces_groundings[ i ] = NULL;
        }
      }
      training_search_spaces_groundings.clear();

      for( unsigned int i = 0; i < training_search_spaces_rules.size(); i++ ){
        if( training_search_spaces_rules[ i ] != NULL ){
          delete training_search_spaces_rules[ i ];
          training_search_spaces_rules[ i ] = NULL;
        }
      }
      training_search_spaces_rules.clear();

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

      for( unsigned int i = 0; i < truth_training_phrases.size(); i++ ){
        if( truth_training_phrases[ i ] != NULL ){
          delete truth_training_phrases[ i ];
          truth_training_phrases[ i ] = NULL;
        }
      }
      truth_training_phrases.clear();

      for( unsigned int i = 0; i < training_worlds.size(); i++ ){
        if( training_worlds[ i ] != NULL ){
          delete training_worlds[ i ];
          training_worlds[ i ] = NULL;
        }
      }
      training_worlds.clear();

      // Write output file.
      // get the test number from the filename
      string filename = args.inputs[ i ];
      string test_number_string = filename.substr( ( filename.size() - 8 ), 4 );
      // write out the output
      stringstream results_filename;
      results_filename << args.output_arg << "/result_" << test_number_string << ".xml";
      xmlSaveFormatFileEnc( results_filename.str().c_str(), tests_doc, "UTF-8", 1 );
    } else {
      cout << "could not read filename \"" << args.inputs[ i ] << "\"" << endl;
      assert( false );
    }
  }

  // Free the XML test_doc
  xmlFreeDoc( tests_doc );

  cout << "end of test_process program" << endl;
  return 0;
}

