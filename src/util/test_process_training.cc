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

#include "h2sl/search_space.h"


using namespace std;
using namespace h2sl; 

/**
 * Function to replace the groundings with rules for the hdcg/hadcg rule examples
 */
void
replace_groundings_with_rules( Phrase* phrase, 
                                World* world ){
  //create a new grounding_set object for the rule symbols
  Grounding_Set * grounding_set_rules = new Grounding_Set();

  //loop through each grounding in the current phrase's grounding_set
  for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
    //replace the current grounding with rules
    (*it_grounding)->fill_rules( world, grounding_set_rules );
  }

  //assign the new grounding_set to the phrase
  phrase->grounding_set() = grounding_set_rules;
  
  //recursive call to replace all child phrase groundings
  for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
    replace_groundings_with_rules( *it_child, world );
  }
  return;
}



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

  /********** Train a llm for each provided training_set partition && save it to a new xml 
              file that also contains both the data partitions and the symbol dictionaries *****************/
  for( unsigned int i = 0; i < args.inputs_num; i++ ){
    // Vector to store list of training set filenames
    vector< string > training_set_filenames;

    // XML pointers
    xmlDoc * input_doc = NULL;
    xmlNodePtr input_root = NULL;
    input_doc = xmlReadFile( args.inputs[ i ], NULL, 0 );

    /** Read the training_set files. Each contain world and the instruction **/
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
                      training_set_filenames.push_back( ( char* )( tmp ) );
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
      cout << "  training_set_filenames[" << training_set_filenames.size() << "]:{";
      for( unsigned int j = 0; j < training_set_filenames.size(); j++ ){
        cout << training_set_filenames[ j ];
        if( j != ( training_set_filenames.size() - 1 ) ){
          cout << ",";
        }
      }
      cout << "}" << endl;

      /********************** Train the Log-linear model *****************/ 
      // load the symbol_dictionaries;
      Symbol_Dictionary * symbol_dictionary_groundings = new Symbol_Dictionary( args.symbol_dictionary_groundings_arg );
      Symbol_Dictionary * symbol_dictionary_rules = new Symbol_Dictionary();
      // only load the rules dictionary if provided
      if( args.symbol_dictionary_rules_given ){
        symbol_dictionary_rules->from_xml( args.symbol_dictionary_rules_arg );
      } else{
        cout << "No symbol_dictionary_rules file provided" << endl;
        if( symbol_dictionary_rules != NULL ){
          delete symbol_dictionary_rules;
          symbol_dictionary_rules = NULL;
        }
      }

      // load the worlds, phrases, and symbol_spaces && fill the examples
      vector< Phrase* > training_phrases;
      vector< World* > training_worlds;
      vector< Search_Space* > training_search_spaces_groundings;
      vector< Search_Space* > training_search_spaces_rules;
      vector< pair< string, LLM_X > > examples;
      for( unsigned int j = 0; j < training_set_filenames.size(); j++ ){
        // load world
        training_worlds.push_back( new World() );
        training_worlds.back()->from_xml( training_set_filenames[ j ] );

        // load phrase
        training_phrases.push_back( new Phrase() );
        training_phrases.back()->from_xml( training_set_filenames[ j ], training_worlds.back() );

        // Search Space fill groundings and scrape examples. Note: examples is common
        // Check if the phrase has symbols from the symbol dictionary groundings.
        if( training_phrases.back()->contains_symbol_in_symbol_dictionary( *symbol_dictionary_groundings ) ){
          cout << "contains symbols in symbol dictionary" << endl;
          training_search_spaces_groundings.push_back( new Search_Space() );
          training_search_spaces_groundings.back()->fill_groundings( *symbol_dictionary_groundings, training_worlds.back() );
          training_search_spaces_groundings.back()->scrape_examples( training_set_filenames[ j ], training_phrases.back(), training_worlds.back(), examples );
        } else {
          cout << "does not contains any symbols in symbol dictionary" << endl;
        }

        // if symbol_dictionary_rules given, add the rule phrases
        if( args.symbol_dictionary_rules_given ){
          // generate the rule phrase
          Phrase * rule_phrase = training_phrases.back()->dup();
          replace_groundings_with_rules( rule_phrase, training_worlds.back() );
          
          // add the rule phrase to the vector of training phrases
          training_phrases.push_back( rule_phrase );

          if( training_phrases.back()->contains_symbol_in_symbol_dictionary( *symbol_dictionary_rules ) ){
            cout << "contains symbols in rules symbol dictionary" << endl;
            training_search_spaces_rules.push_back( new Search_Space() );
            training_search_spaces_rules.back()->fill_rules( *symbol_dictionary_rules, training_worlds.back() );
            training_search_spaces_rules.back()->scrape_examples( training_set_filenames[ j ], training_phrases.back(), training_worlds.back(), examples );
          } else {
            cout << "does not contains any rules symbols in symbol dictionary" << endl;
          }
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

      /********* Append the LLM and both symbol_dictionaries to the input file and save as new file**************/
      // force libxml to apply indentation for human readability
      //xmlKeepBlanksDefault(0);
    
      // add the training runtime information
      xmlNodePtr training_runtime_node = xmlNewDocNode( input_doc, NULL, ( const xmlChar* )( "training_runtime" ), NULL ); 
      xmlNewProp( training_runtime_node, ( const xmlChar* )( "train_time" ), ( const xmlChar* )( std::to_string( train_time ).c_str() ) );
      xmlAddChild( input_root, training_runtime_node );
      

      // add two new xmlNodes to differentiate the symbol_dictionary_rules and symbol_dictionary_groundings
      // add the the symbol_dictionary_groundings first
      xmlNodePtr symbol_dictionary_groundings_node = xmlNewDocNode( input_doc, NULL, ( const xmlChar* )( "symbol_dictionary_groundings" ), NULL ); 
      xmlAddChild( input_root, symbol_dictionary_groundings_node );
      symbol_dictionary_groundings->to_xml( input_doc, symbol_dictionary_groundings_node ); 

      if( args.symbol_dictionary_rules_given ){
        xmlNodePtr symbol_dictionary_rules_node = xmlNewDocNode( input_doc, NULL, ( const xmlChar* )( "symbol_dictionary_rules" ), NULL ); 
        xmlAddChild( input_root, symbol_dictionary_rules_node );
        symbol_dictionary_rules->to_xml( input_doc, symbol_dictionary_rules_node ); 
      }

      // append the llm to the xml document object
      llms.front()->to_xml( input_doc, input_root );

      // create the name for the new file
      // test_number_string comes from the "report" code above
      stringstream input_file_with_llm;
      input_file_with_llm << args.output_arg << "/test_collated_" << test_number_string << ".xml";
      
      //save the file
      xmlSaveFormatFileEnc( input_file_with_llm.str().c_str(), input_doc, "UTF-8", 1 );
      

      /********************* Evaluate the log-linear model ******************/
      if( args.evaluate_model_arg ){
        cout << "evaluating llm model" << endl;
        evaluate_model( llms.front(), examples );
      }
 
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

      for( unsigned int i = 0; i < training_phrases.size(); i++ ){
        if( training_phrases[ i ] != NULL ){
          delete training_phrases[ i ];
          training_phrases[ i ] = NULL;
        }
      }
      training_phrases.clear();

      for( unsigned int i = 0; i < training_worlds.size(); i++ ){
        if( training_worlds[ i ] != NULL ){
          delete training_worlds[ i ];
          training_worlds[ i ] = NULL;
        }
      }
      training_worlds.clear();

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

      if( symbol_dictionary_groundings != NULL ){
        delete symbol_dictionary_groundings;
        symbol_dictionary_groundings = NULL;
      }

      if( symbol_dictionary_rules != NULL ){
        delete symbol_dictionary_rules;
        symbol_dictionary_rules = NULL;
      }
    }
    //free the xml pointers
    if( input_doc != NULL ){
      xmlFreeDoc( input_doc );
    }
  }
  cout << "end of test_process_training program" << endl;
  return 0;
}
