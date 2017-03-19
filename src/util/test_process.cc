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

// Determine the solution directory where solutions  will be created.
  stringstream solution_directory;
  if( !args.solution_directory_given ){
    solution_directory << "/tmp/";
  } else{
   solution_directory << args.solution_directory_arg;
  }
  cout << "solution_directory: " << solution_directory.str() << endl;

  // XML: test document
  xmlDocPtr tests_doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  // Node ptr for root
  xmlNodePtr tests_root = xmlNewDocNode( tests_doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( tests_doc, tests_root );
  // Node ptr for test node
  xmlNodePtr tests_node = xmlNewDocNode( tests_doc, NULL, ( xmlChar* )( "tests" ), NULL );
  // Number. Added as a node property.
  stringstream num_string;
  num_string << args.inputs_num;
  xmlNewProp( tests_node, ( const xmlChar* )( "num" ), ( const xmlChar* )( num_string.str().c_str() ) );
  // test_node child node for root.
  xmlAddChild( tests_root, tests_node );

  /********** Do evaluation for each training and test set partition of the full corpus *****************/
  for( unsigned int i = 0; i < args.inputs_num; i++ ){

    xmlNodePtr test_node = xmlNewDocNode( tests_doc, NULL, ( xmlChar* )( "test" ), NULL );
    xmlNewProp( test_node, ( const xmlChar* )( "filename" ), ( const xmlChar* )( args.inputs[ i ]  ) );

    // Vectors store list of training set and test set file names.
    vector< string > training_set;
    vector< string > test_set;

    xmlDoc * input_doc = NULL;
    xmlNodePtr input_root = NULL;
    input_doc = xmlReadFile( args.inputs[ i ], NULL, 0 );

    /** Separate out the training and test set files. Each contain world and the instruction **/
    if( input_doc != NULL ){
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
            } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "test_set" ) ) == 0 ){
              for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
                if( l2->type == XML_ELEMENT_NODE ){
                  if( xmlStrcmp( l2->name, ( const xmlChar* )( "filename" ) ) == 0 ){
                    xmlChar * tmp = xmlGetProp( l2, ( const xmlChar* )( "text" ) );
                    if( tmp != NULL ){
                      // push_back the test set file name.
                      test_set.push_back( ( char* )( tmp ) );
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
      
     /******************** Report ******************************************/
     // Test number (partition of the corpus), training set and test set size.
      cout << "test " << i << endl << endl;
      cout << "training_set[" << training_set.size() << "]:{";
      for( unsigned int j = 0; j < training_set.size(); j++ ){
        cout << training_set[ j ];
        if( j != ( training_set.size() - 1 ) ){
          cout << ",";
        }
      }
      cout << "}" << endl;
      cout << "test_set[" << test_set.size() << "]:{";
      for( unsigned int j = 0; j < test_set.size(); j++ ){
        cout << test_set[ j ];
        if( j != ( test_set.size() - 1 ) ){
          cout << ",";
        }
      }
      cout << "}" << endl;
   
      /********************** Train the Log-linear model *****************/ 
      vector< Search_Space* > training_search_spaces( training_set.size(), NULL );
      vector< Phrase* > truth_training_phrases( training_set.size(), NULL );
      vector< World* > training_worlds( training_set.size(), NULL );
      vector< string > training_filenames( training_set.size() );

      Symbol_Dictionary * symbol_dictionary_groundings = new Symbol_Dictionary( args.symbol_dictionary_groundings_arg );
     
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
        truth_training_phrases[ j ]->from_xml( training_set[ j ] );

        // Check if the phrase has symbols from the symbol dictionary.
        if( truth_training_phrases[ j ]->contains_symbol_in_symbol_dictionary( *symbol_dictionary_groundings ) ){
          cout << "contains symbols in symbol dictionary" << endl;
        } else {
          cout << "does not contains any symbols in symbol dictionary" << endl;
        }

        // Search Space: Fill groundings and scrape examples.
        training_search_spaces[ j ] = new Search_Space();
        training_search_spaces[ j ]->fill_groundings( *symbol_dictionary_groundings, training_worlds[ j ] );
        training_search_spaces[ j ]->scrape_examples( training_filenames[ j ], 
                                       	              static_cast< Phrase* >( truth_training_phrases[ j ] ), 
                                                      training_worlds[ j ], examples );
      }

      // Report the training set size.
      cout << "training with " << examples.size() << " examples" << endl;

      // Write output file.
      if( args.output_given ){
        xmlSaveFormatFileEnc( args.output_arg, tests_doc, "UTF-8", 1 );
      }


    } else {
      cout << "could not read filename \"" << args.inputs[ i ] << "\"" << endl;
      assert( false );
    }
  }

/*
  if( args.output_given ){
    xmlSaveFormatFileEnc( args.output_arg, tests_doc, "UTF-8", 1 );
  }
*/
  // Free the XML test_doc
  xmlFreeDoc( tests_doc );

  cout << "end of test_process program" << endl;
  return 0;
}

