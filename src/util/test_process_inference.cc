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

#include "test_process_inference_cmdline.h"
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
 * Load the symbol dictionary for rules from the provided test xml file
 * Return type is a boolean:
 *    TRUE = a symbol_dictionary of dictionary_type was found and loaded
 *    FALSE = no symbol_dictionary of dictionary_type was found
 */
bool
load_symbol_dictionary( string filename, string dictionary_type, Symbol_Dictionary*& symbol_dictionary ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( dictionary_type.c_str() ) ) == 0 ){
            for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
              if( l2->type == XML_ELEMENT_NODE ){
                if( xmlStrcmp( l2->name, ( const xmlChar* )( "symbol_dictionary" ) ) == 0 ){
                  symbol_dictionary->from_xml( l2 );
                  if( doc != NULL ){
                    xmlFreeDoc( doc );
                  }
                  return true;
                }
              }
            }
          }
        }
      }
    }
  }
  if( doc != NULL ){
    xmlFreeDoc( doc );
  }
  return false;
}

/**
 * load the training runtime for the log-linear model
 * returns a string of the runtime value
 */
string
load_training_runtime( string filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "training_runtime" ) ) == 0 ){
            pair< bool, string > training_runtime_prop = has_prop< std::string >( l1, "train_time" );
            if( training_runtime_prop.first ){
              if( doc != NULL ){
                xmlFreeDoc( doc );
              }
              return training_runtime_prop.second;
            }
          }
        }
      }
    }
  }
  if( doc != NULL ){
    xmlFreeDoc( doc );
  }
  return "na";
}

/*
 * Helper function to write the solutions file.
 * Includes the world and parse information.
 */
void
solution_to_xml( const string& filename,
                 const World* const& world, 
                 const Phrase* const& phrase ){
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  world->to_xml( doc, root );
  phrase->to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

/* 
 * Run tests for the DCG model. 
 */
void
run_tests_dcg( const std::vector< std::string >& filenames,
            LLM*& llm,
            xmlDocPtr& docPtr,
            xmlNodePtr& parentNode,
            const string& testGroup,
            const unsigned int cur_test_num,
            const string& solution_directory, 
            const Symbol_Dictionary* symbol_dictionary, 
            const unsigned int beam_width,
            const bool debug ){
  /******* Variables to record statistics **************************/
  // Num objects. Phrases.
  double num_objects = 0.0;
  double num_phrases = 0.0;

  // DCG
  unsigned int num_correct_root_dcg = 0;
  unsigned int num_incorrect_root_dcg = 0;
  unsigned int num_correct_complete_dcg = 0;
  unsigned int num_incorrect_complete_dcg = 0;
  double num_concrete_dcg = 0.0;
  double num_abstract_avg_dcg = 0.0;
  double num_abstract_max_dcg = 0.0;

  // Runtime
  double runtime_dcg = 0.0;
  double runtime_dcg_norm = 0.0;
 
  // Search Space Sizes
  double dataset_concrete_dcg = 0.0;
  double dataset_abstract_avg_dcg = 0.0;
  double dataset_abstract_max_dcg = 0.0;

  // Averages.
  double average_runtime_ratio = 0.0;

  /******* Iterate over the test group **************************/
  xmlNodePtr test_group_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( testGroup.c_str() ), NULL );

  for( unsigned int j = 0; j < filenames.size(); j++ ){
    cout << "running " << testGroup << " " << j << " (" << filenames[ j ] << ")" << endl;

    string filename = boost::filesystem::path( filenames[ j ] ).filename().generic_string();
    xmlNodePtr example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "example" ), NULL );
    xmlNewProp( example_node, ( const xmlChar* )( "filename" ), ( const xmlChar* )( filenames[ j ].c_str() ) );

    // World
    World * world = new World();
    world->from_xml( filenames[ j ] );

    // Phrase truth. Note no parsing.
    Phrase* truth_phrase = new Phrase();
    truth_phrase->from_xml( filenames[ j ], world );
    // Phrase input.
    Phrase * input_phrase = new Phrase();
    input_phrase->from_xml( filenames[ j ], world );
    clear( input_phrase );

    // Search Space and fill the space of groundings.
    Search_Space * search_space = new Search_Space();
    struct timeval start_time;
    gettimeofday( &start_time, NULL );
    search_space->fill_groundings( *symbol_dictionary, world );
    struct timeval end_time;
    gettimeofday( &end_time, NULL );
    cout << "finished fill_seach_space in " << diff_time( start_time, end_time ) << " seconds" << endl;

    // Grounding context
    Grounding * context = NULL;

    /******************* Running DCG ********************************/
    //DCG: leaf search
    DCG * dcg = new DCG();
    cout << "  running dcg" << endl;
    uint64_t dcg_start_time = current_time();
    dcg->leaf_search( input_phrase, *symbol_dictionary, search_space, world, 
                      context, llm, beam_width, ( bool )( debug ) );
    uint64_t dcg_end_time = current_time();
    runtime_dcg += microseconds_to_seconds( dcg_end_time - dcg_start_time );

    assert( dynamic_cast< const Phrase* >( dcg->solutions().front().second ) != NULL );
    // If the number of phrases are more than one then compute norm.
    if ( dcg->solutions().front().second->num_phrases() ){
      runtime_dcg_norm += microseconds_to_seconds( ( dcg_end_time - dcg_start_time ) / ( dcg->solutions().front().second->num_phrases() ) );
    }

    // Write the solution
    stringstream dcg_solution_filename;
    dcg_solution_filename << solution_directory << "/test_" << setw( 4 ) << setfill( '0' ) << cur_test_num << "_dcg_solution_" << filename; 
    solution_to_xml( dcg_solution_filename.str(), world, dcg->solutions().front().second );

    // Write the metrics    
    xmlNodePtr dcg_example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "dcg" ), NULL );

    // Runtime.
    stringstream dcg_example_runtime_string;
    dcg_example_runtime_string << microseconds_to_seconds( dcg_end_time - dcg_start_time );
    xmlNewProp( dcg_example_node, ( const xmlChar* )( "runtime" ), ( const xmlChar* )( dcg_example_runtime_string.str().c_str() ) );
    cout << "    runtime:" << dcg_example_runtime_string.str() << endl;

    // Runtime normalised.
    stringstream dcg_example_runtime_norm_string;
    dcg_example_runtime_norm_string << microseconds_to_seconds( ( dcg_end_time - dcg_start_time ) / ( dcg->solutions().front().second->num_phrases() ) ); 
    xmlNewProp( dcg_example_node, ( const xmlChar* )( "runtime_norm" ), ( const xmlChar* )( dcg_example_runtime_norm_string.str().c_str() ) );
    cout << "    runtime norm:" << dcg_example_runtime_norm_string.str() << endl;

    // Search space size: concrete
    double tmp_dcg_stats = 0.0;
    
    tmp_dcg_stats = dcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" );
    num_concrete_dcg += tmp_dcg_stats;
    xmlNewProp( dcg_example_node, ( const xmlChar* )( "search_space_concrete_size" ), (const xmlChar* )( to_std_string( tmp_dcg_stats ).c_str() ) );

    // Search space size: abstract average
    tmp_dcg_stats = dcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" );
    num_abstract_avg_dcg += tmp_dcg_stats;
    xmlNewProp( dcg_example_node, ( const xmlChar* )( "search_space_abstract_avg_size" ), (const xmlChar* )( to_std_string( tmp_dcg_stats ).c_str() ) );

    // Search space size: abstract max 
    tmp_dcg_stats = dcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" );
    num_abstract_max_dcg += tmp_dcg_stats;
    xmlNewProp( dcg_example_node, ( const xmlChar* )( "search_space_abstract_max_size" ), (const xmlChar* )( to_std_string( tmp_dcg_stats ).c_str() ) );

    // Accuracy statistics.
    if( root_compare_phrases( *truth_phrase, *static_cast< Phrase* >( dcg->solutions().front().second ) ) ){
      cout << "    correct (root)" << endl;
      xmlNewProp( dcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_root_dcg++;
    } else {
      cout << "    incorrect (root)" << endl;
      xmlNewProp( dcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_root_dcg++;
    }

    if( complete_compare_phrases( *truth_phrase, *static_cast< Phrase* >( dcg->solutions().front().second ) ) ){
      cout << "    correct (complete)" << endl;
      xmlNewProp( dcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_complete_dcg++;
    } else {
      cout << "    incorrect (complete)" << endl;
      xmlNewProp( dcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_complete_dcg++;
    }

    // Add the dcg_example_node to the example_node in the xml file.
    xmlAddChild( example_node, dcg_example_node );
    // clear the input_phrase for groundings to run the test for the next model.
    clear( input_phrase );
 
    if( debug ) {
      cout << "dcg_concrete_size" << to_std_string( dcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" ) ) << endl;
      cout << "dcg_abstract_avg_size" << to_std_string( dcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" ) ) << endl;
      cout << "dcg_abstract_avg_size" << to_std_string( dcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" ) ) << endl;
    }
  
    //find example_runtime_object_ratio_dcg && example_runtime_object_ratio_adcg
    unsigned int example_num_objects = world->objects().size(); 
    
    num_objects = num_objects + ( double ) example_num_objects;
    num_phrases = num_phrases + ( double ) truth_phrase->num_phrases();

    double example_runtime_object_ratio_dcg = ( microseconds_to_seconds( dcg_end_time - dcg_start_time ) )/ ( example_num_objects ) ;

    //add # of phrases to example node
    stringstream example_num_phrases_string;
    example_num_phrases_string << truth_phrase->num_phrases();
    xmlNewProp( example_node, ( const xmlChar* )( "num_phrases" ), ( const xmlChar* )( example_num_phrases_string.str().c_str() ) );

    //add # of world objects to example node
    stringstream example_num_objects_string;
    example_num_objects_string << example_num_objects;
    xmlNewProp( example_node, ( const xmlChar* )( "world_objects" ), ( const xmlChar* )( example_num_objects_string.str().c_str() ) );

    stringstream example_runtime_object_ratio_dcg_string;
    example_runtime_object_ratio_dcg_string << example_runtime_object_ratio_dcg;
    xmlNewProp( example_node, ( const xmlChar* )( "runtime_object_ratio_dcg" ), ( const xmlChar* )( example_runtime_object_ratio_dcg_string.str().c_str() ) );

    // Export the world to xml. 
    world->to_xml( docPtr, example_node );

    // Clean up.
    //delete_ptr< Grounding >( context );
    delete_ptr< DCG >( dcg );
    delete_ptr< World >( world );
    delete_ptr< Phrase >( input_phrase );
    delete_ptr< Phrase >( truth_phrase );
    delete_ptr< Search_Space >( search_space );

    // Add the node in XML.
    xmlAddChild( test_group_node, example_node );
  }

/****************************** Write the statistics to XML: DCG *************************/

  // Average runtime.
  runtime_dcg /= ( double )( filenames.size() );
  runtime_dcg_norm /= ( double )( filenames.size() );

  // Avg. runtime ratio
  average_runtime_ratio /= ( double )( filenames.size() );
 
  // Average number of phrases and objects 
  num_phrases /= ( double )( filenames.size() );
  num_objects /= ( double )( filenames.size() );

  // Correct root for all models.
  double correct_root_dcg = ( double )( num_correct_root_dcg ) / ( double )( num_correct_root_dcg + num_incorrect_root_dcg );
  // Correct complete for all models.
  double correct_complete_dcg = ( double )( num_correct_complete_dcg ) / ( double )( num_correct_complete_dcg + num_incorrect_complete_dcg );
  // Search Space Sizes
  dataset_concrete_dcg = num_concrete_dcg / ( double )( filenames.size() ) ;
  dataset_abstract_avg_dcg = num_abstract_avg_dcg / ( double )( filenames.size() ) ;
  dataset_abstract_max_dcg = num_abstract_max_dcg / ( double )( filenames.size() );

  /******  XML: correct root **************************/ 
  stringstream correct_root_dcg_string;
  correct_root_dcg_string << correct_root_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_dcg" ), ( const xmlChar* )( correct_root_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_dcg" ).c_str() ), ( const xmlChar* )( correct_root_dcg_string.str().c_str() ) );

  /******  XML: correct complete  **************************/ 
  stringstream correct_complete_dcg_string;
  correct_complete_dcg_string << correct_complete_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_dcg" ), ( const xmlChar* )( correct_complete_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_dcg" ).c_str() ), ( const xmlChar* )( correct_complete_dcg_string.str().c_str() ) );

  /******  XML: runtime  **************************/ 
  stringstream runtime_dcg_string;
  runtime_dcg_string << runtime_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_dcg" ), ( const xmlChar* )( runtime_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_dcg" ).c_str() ), ( const xmlChar* )( runtime_dcg_string.str().c_str() ) );

  /******  XML: runtime  **************************/ 
  stringstream runtime_dcg_norm_string;
  runtime_dcg_norm_string << runtime_dcg_norm;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_dcg_norm" ), ( const xmlChar* )( runtime_dcg_norm_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_dcg_norm" ).c_str() ), ( const xmlChar* )( runtime_dcg_norm_string.str().c_str() ) );

  /******  XML: runtime  **************************/ 
  stringstream runtime_ratio_string;
  runtime_ratio_string << average_runtime_ratio;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_ratio" ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_ratio" ).c_str() ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );

  /******  XML: num phrases  **************************/ 
  stringstream num_phrases_string;
  num_phrases_string << num_phrases;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_phrases" ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_phrases" ).c_str() ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );

  /******  XML: num objects  **************************/ 
  stringstream num_objects_string;
  num_objects_string << num_objects;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_objects" ), ( const xmlChar* )( num_objects_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_objects" ).c_str() ), ( const xmlChar* )( num_objects_string.str().c_str() ) );

  /******  XML: num objects  **************************/ 
  stringstream runtime_object_ratio_dcg_string;
  runtime_object_ratio_dcg_string << ( runtime_dcg / num_objects );
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_object_ratio_dcg" ), ( const xmlChar* )( runtime_object_ratio_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "runtime_object_ratio_dcg" ).c_str() ), ( const xmlChar* )( runtime_object_ratio_dcg_string.str().c_str() ) );

  /******  XML: dataset concrete dcg   **************************/ 
  stringstream dataset_concrete_dcg_string;
  dataset_concrete_dcg_string << dataset_concrete_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "dataset_concrete_dcg" ), ( const xmlChar* )( dataset_concrete_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_dataset_concrete_dcg" ).c_str() ), ( const xmlChar* )( dataset_concrete_dcg_string.str().c_str() ) );

  /******  XML: dataset abstract avg dcg   **************************/ 
  stringstream dataset_abstract_avg_dcg_string;
  dataset_abstract_avg_dcg_string << dataset_abstract_avg_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "dataset_abstract_avg_dcg" ), ( const xmlChar* )( dataset_abstract_avg_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_dataset_abstract_avg_dcg" ).c_str() ), ( const xmlChar* )( dataset_abstract_avg_dcg_string.str().c_str() ) );

  /******  XML: dataset abstract max dcg   **************************/ 
  stringstream dataset_abstract_max_dcg_string;
  dataset_abstract_max_dcg_string << dataset_abstract_max_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "dataset_abstract_max_dcg" ), ( const xmlChar* )( dataset_abstract_max_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_dataset_abstract_max_dcg" ).c_str() ), ( const xmlChar* )( dataset_abstract_max_dcg_string.str().c_str() ) );

  /******  console output **************************/ 
  cout << endl;
  cout << "correct_root_dcg: " << correct_root_dcg << endl;

  cout << "correct_complete_dcg: " << correct_complete_dcg << endl;
  cout << "runtime_dcg: " << runtime_dcg << endl;
  cout << "runtime_dcg_norm: " << runtime_dcg_norm << endl;

  cout << "dcg_search_space_concrete: " << dataset_concrete_dcg << endl;
  cout << "dcg_search_space_abstract_avg: " << dataset_abstract_avg_dcg << endl;
  cout << "dcg_search_space_abstract_max: " << dataset_abstract_max_dcg << endl;
  xmlAddChild( parentNode, test_group_node );

  return;

}


/* 
 * Run tests for the ADCG model. 
 */
void
run_tests_adcg( const std::vector< std::string >& filenames,
            LLM*& llm,
            xmlDocPtr& docPtr,
            xmlNodePtr& parentNode,
            const string& testGroup,
            const unsigned int cur_test_num,
            const string& solution_directory,
            const Symbol_Dictionary* symbol_dictionary, 
            const unsigned int beam_width,
            const bool debug ){

  /******* Variables to record statistics **************************/
  // Num objects. Phrases.
  double num_objects = 0.0;
  double num_phrases = 0.0;

  // ADCG
  unsigned int num_correct_root_adcg = 0;
  unsigned int num_incorrect_root_adcg = 0;
  unsigned int num_correct_complete_adcg = 0;
  unsigned int num_incorrect_complete_adcg = 0;
  double num_concrete_adcg = 0.0;
  double num_abstract_avg_adcg = 0.0;
  double num_abstract_max_adcg = 0.0;

  // Runtime
  double runtime_adcg = 0.0;
  double runtime_adcg_norm = 0.0;

  // Search Space Sizes
  double dataset_concrete_adcg = 0.0;
  double dataset_abstract_avg_adcg = 0.0;
  double dataset_abstract_max_adcg = 0.0;

  // Averages.
  double average_runtime_ratio = 0.0;

 /******* Iterate over the test group **************************/
  xmlNodePtr test_group_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( testGroup.c_str() ), NULL );

  for( unsigned int j = 0; j < filenames.size(); j++ ){
    cout << "running " << testGroup << " " << j << " (" << filenames[ j ] << ")" << endl;

    string filename = boost::filesystem::path( filenames[ j ] ).filename().generic_string();
    xmlNodePtr example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "example" ), NULL );
    xmlNewProp( example_node, ( const xmlChar* )( "filename" ), ( const xmlChar* )( filenames[ j ].c_str() ) );

    // World
    World * world = new World();
    world->from_xml( filenames[ j ] );

    // Phrase truth. Note no parsing.
    Phrase* truth_phrase = new Phrase();
    truth_phrase->from_xml( filenames[ j ], world );
    // Phrase input.
    Phrase * input_phrase = new Phrase();
    input_phrase->from_xml( filenames[ j ], world );
    clear( input_phrase );

    // Search Space and fill the space of groundings.
    Search_Space * search_space = new Search_Space();
    struct timeval start_time;
    gettimeofday( &start_time, NULL );
    search_space->fill_groundings( *symbol_dictionary, world );
    struct timeval end_time;
    gettimeofday( &end_time, NULL );
    cout << "finished fill_seach_space in " << diff_time( start_time, end_time ) << " seconds" << endl;

    // Grounding context
    Grounding * context = NULL;

 /******************* Running ADCG ********************************/
    //ADCG: leaf search
    ADCG * adcg = new ADCG();
    cout << "  running adcg" << endl;
    uint64_t adcg_start_time = current_time();
    adcg->leaf_search( input_phrase, *symbol_dictionary, search_space, world,
                      context, llm, beam_width, ( bool )( debug ) );
    uint64_t adcg_end_time = current_time();
    runtime_adcg += microseconds_to_seconds( adcg_end_time - adcg_start_time );

    assert( dynamic_cast< const Phrase* >( adcg->solutions().front().second ) != NULL );
    // If the number of phrases are more than one then compute norm.
    if ( adcg->solutions().front().second->num_phrases() ){
      runtime_adcg_norm += microseconds_to_seconds( ( adcg_end_time - adcg_start_time ) / ( adcg->solutions().front().second->num_phrases() )  );
    }

    stringstream adcg_solution_filename;
    adcg_solution_filename << solution_directory << "/test_" << setw( 4 ) << setfill( '0' ) << cur_test_num << "_adcg_solution_" << filename;
    solution_to_xml( adcg_solution_filename.str(), world, adcg->solutions().front().second );

    xmlNodePtr adcg_example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "adcg" ), NULL );

    // Runtime.
    stringstream adcg_example_runtime_string;
    adcg_example_runtime_string << microseconds_to_seconds( adcg_end_time - adcg_start_time );
    xmlNewProp( adcg_example_node, ( const xmlChar* )( "runtime" ), ( const xmlChar* )( adcg_example_runtime_string.str().c_str() ) );
    cout << "    runtime:" << adcg_example_runtime_string.str() << endl;

    // Runtime normalised.
    stringstream adcg_example_runtime_norm_string;
    adcg_example_runtime_norm_string << microseconds_to_seconds( ( adcg_end_time - adcg_start_time ) / ( adcg->solutions().front().second->num_phrases() ) ); 
    xmlNewProp( adcg_example_node, ( const xmlChar* )( "runtime_norm" ), ( const xmlChar* )( adcg_example_runtime_norm_string.str().c_str() ) );
    cout << "    runtime norm:" << adcg_example_runtime_norm_string.str() << endl;

    // Search space size: concrete
    double tmp_adcg_stats = 0.0;

    tmp_adcg_stats = adcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" );
    num_concrete_adcg += tmp_adcg_stats;
    xmlNewProp( adcg_example_node, ( const xmlChar* )( "search_space_concrete_size" ), (const xmlChar* )( to_std_string( tmp_adcg_stats ).c_str() ) );

    // Search space size: abstract average
    tmp_adcg_stats = adcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" );
    num_abstract_avg_adcg += tmp_adcg_stats;
    xmlNewProp( adcg_example_node, ( const xmlChar* )( "search_space_abstract_avg_size" ), (const xmlChar* )( to_std_string( tmp_adcg_stats ).c_str() ) );

    // Search space size: abstract max 
    tmp_adcg_stats = adcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" );
    num_abstract_max_adcg += tmp_adcg_stats;
    xmlNewProp( adcg_example_node, ( const xmlChar* )( "search_space_abstract_max_size" ), (const xmlChar* )( to_std_string( tmp_adcg_stats ).c_str() ) );

    // Accuracy statistics.
 if( root_compare_phrases( *truth_phrase, *static_cast< Phrase* >( adcg->solutions().front().second ) ) ){
      cout << "    correct (root)" << endl;
      xmlNewProp( adcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_root_adcg++;
    } else {
      cout << "    incorrect (root)" << endl;
      xmlNewProp( adcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_root_adcg++;
    }

    if( complete_compare_phrases( *truth_phrase, *static_cast< Phrase* >( adcg->solutions().front().second ) ) ){
      cout << "    correct (complete)" << endl;
      xmlNewProp( adcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_complete_adcg++;
    } else {
      cout << "    incorrect (complete)" << endl;
      xmlNewProp( adcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_complete_adcg++;
    }

    // Add the adcg_example_node to the example_node in the xml file.
    xmlAddChild( example_node, adcg_example_node );
    // clear the input_phrase for groundings to run the test for the next model.
    clear( input_phrase );

    if( debug ) {
      cout << "adcg_concrete_size" << to_std_string( adcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" ) ) << endl;
      cout << "adcg_abstract_avg_size" << to_std_string( adcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" ) ) << endl;
      cout << "adcg_abstract_avg_size" << to_std_string( adcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" ) ) << endl;
    }

    //find example_runtime_object_ratio_adcg && example_runtime_object_ratio_aadcg
    unsigned int example_num_objects = world->objects().size();

    num_objects = num_objects + ( double ) example_num_objects;
    num_phrases = num_phrases + ( double ) truth_phrase->num_phrases();

    double example_runtime_object_ratio_adcg = ( microseconds_to_seconds( adcg_end_time - adcg_start_time ) )/ ( example_num_objects ) ;

    //add # of phrases to example node
    stringstream example_num_phrases_string;
    example_num_phrases_string << truth_phrase->num_phrases();
    xmlNewProp( example_node, ( const xmlChar* )( "num_phrases" ), ( const xmlChar* )( example_num_phrases_string.str().c_str() ) );

    //add # of world objects to example node
    stringstream example_num_objects_string;
    example_num_objects_string << example_num_objects;
    xmlNewProp( example_node, ( const xmlChar* )( "world_objects" ), ( const xmlChar* )( example_num_objects_string.str().c_str() ) );

    stringstream example_runtime_object_ratio_adcg_string;
    example_runtime_object_ratio_adcg_string << example_runtime_object_ratio_adcg;
    xmlNewProp( example_node, ( const xmlChar* )( "runtime_object_ratio_adcg" ), ( const xmlChar* )( example_runtime_object_ratio_adcg_string.str().c_str() ) );

    // Export the world to xml. 
    world->to_xml( docPtr, example_node );

    // Clean up.
    //delete_ptr< Grounding >( context );
    delete_ptr< ADCG >( adcg );
    delete_ptr< World >( world );
    delete_ptr< Phrase >( input_phrase );
    delete_ptr< Phrase >( truth_phrase );
    delete_ptr< Search_Space >( search_space );

    // Add the node in XML.
    xmlAddChild( test_group_node, example_node );
  }

/****************************** Write the statistics to XML: ADCG *************************/

  // Average runtime.
  runtime_adcg /= ( double )( filenames.size() );
  runtime_adcg_norm /= ( double )( filenames.size() );

  // Avg. runtime ratio
  average_runtime_ratio /= ( double )( filenames.size() );

  // Average number of phrases and objects 
  num_phrases /= ( double )( filenames.size() );
  num_objects /= ( double )( filenames.size() );

  // Correct root for all models.
  double correct_root_adcg = ( double )( num_correct_root_adcg ) / ( double )( num_correct_root_adcg + num_incorrect_root_adcg );
  // Correct complete for all models.
  double correct_complete_adcg = ( double )( num_correct_complete_adcg ) / ( double )( num_correct_complete_adcg + num_incorrect_complete_adcg );
  // Search Space Sizes
  dataset_concrete_adcg = num_concrete_adcg / ( double )( filenames.size() ) ;
  dataset_abstract_avg_adcg = num_abstract_avg_adcg / ( double )( filenames.size() ) ;
  dataset_abstract_max_adcg = num_abstract_max_adcg / ( double )( filenames.size() );

  /******  XML: correct root **************************/
  stringstream correct_root_adcg_string;
  correct_root_adcg_string << correct_root_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_adcg" ), ( const xmlChar* )( correct_root_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_adcg" ).c_str() ), ( const xmlChar* )( correct_root_adcg_string.str().c_str() ) );

  /******  XML: correct complete  **************************/
  stringstream correct_complete_adcg_string;
  correct_complete_adcg_string << correct_complete_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_adcg" ), ( const xmlChar* )( correct_complete_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_adcg" ).c_str() ), ( const xmlChar* )( correct_complete_adcg_string.str().c_str() ) );

 /******  XML: runtime  **************************/
  stringstream runtime_adcg_string;
  runtime_adcg_string << runtime_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_adcg" ), ( const xmlChar* )( runtime_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_adcg" ).c_str() ), ( const xmlChar* )( runtime_adcg_string.str().c_str() ) );

 /******  XML: runtime normalised  **************************/
  stringstream runtime_adcg_norm_string;
  runtime_adcg_norm_string << runtime_adcg_norm;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_adcg_norm" ), ( const xmlChar* )( runtime_adcg_norm_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_adcg_norm" ).c_str() ), ( const xmlChar* )( runtime_adcg_norm_string.str().c_str() ) );

  /******  XML: runtime  **************************/
  stringstream runtime_ratio_string;
  runtime_ratio_string << average_runtime_ratio;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_ratio" ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_ratio" ).c_str() ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );

  /******  XML: num phrases  **************************/
  stringstream num_phrases_string;
  num_phrases_string << num_phrases;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_phrases" ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_phrases" ).c_str() ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );

  /******  XML: num objects  **************************/
  stringstream num_objects_string;
  num_objects_string << num_objects;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_objects" ), ( const xmlChar* )( num_objects_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_objects" ).c_str() ), ( const xmlChar* )( num_objects_string.str().c_str() ) );

  /******  XML: num objects  **************************/
  stringstream runtime_object_ratio_adcg_string;
  runtime_object_ratio_adcg_string << ( runtime_adcg / num_objects );
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_object_ratio_adcg" ), ( const xmlChar* )( runtime_object_ratio_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "runtime_object_ratio_adcg" ).c_str() ), ( const xmlChar* )( runtime_object_ratio_adcg_string.str().c_str() ) );

  /******  XML: dataset concrete dcg   **************************/ 
  stringstream dataset_concrete_adcg_string;
  dataset_concrete_adcg_string << dataset_concrete_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "dataset_concrete_adcg" ), ( const xmlChar* )( dataset_concrete_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_dataset_concrete_adcg" ).c_str() ), ( const xmlChar* )( dataset_concrete_adcg_string.str().c_str() ) );

  /******  XML: dataset abstract avg dcg   **************************/ 
  stringstream dataset_abstract_avg_adcg_string;
  dataset_abstract_avg_adcg_string << dataset_abstract_avg_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "dataset_abstract_avg_adcg" ), ( const xmlChar* )( dataset_abstract_avg_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_dataset_abstract_avg_adcg" ).c_str() ), ( const xmlChar* )( dataset_abstract_avg_adcg_string.str().c_str() ) );

  /******  XML: dataset abstract max dcg   **************************/ 
  stringstream dataset_abstract_max_adcg_string;
  dataset_abstract_max_adcg_string << dataset_abstract_max_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "dataset_abstract_max_adcg" ), ( const xmlChar* )( dataset_abstract_max_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_dataset_abstract_max_adcg" ).c_str() ), ( const xmlChar* )( dataset_abstract_max_adcg_string.str().c_str() ) );

  /******  console output **************************/
  cout << endl;
  cout << "correct_root_adcg: " << correct_root_adcg << endl;

  cout << "correct_complete_adcg: " << correct_complete_adcg << endl;
  cout << "runtime_adcg: " << runtime_adcg << endl;
  cout << "runtime_adcg_norm: " << runtime_adcg_norm << endl;

  cout << "adcg_search_space_concrete: " << dataset_concrete_adcg << endl;
  cout << "adcg_search_space_abstract_avg: " << dataset_abstract_avg_adcg << endl;
  cout << "adcg_search_space_abstract_max: " << dataset_abstract_max_adcg << endl;
  xmlAddChild( parentNode, test_group_node );

  return;

}

/* 
 * Run tests for the HDCG model. 
 */
void
run_tests_hdcg( const std::vector< std::string >& filenames,
            LLM*& llm,
            xmlDocPtr& docPtr,
            xmlNodePtr& parentNode,
            const string& testGroup,
            const unsigned int cur_test_num,
            const string& solution_directory, 
            const Symbol_Dictionary* symbol_dictionary, 
            const unsigned int beam_width,
            const bool debug ){

  /******* Variables to record statistics **************************/
  // Num objects. Phrases.
  double num_objects = 0.0;
  double num_phrases = 0.0;

  // HADCG
  unsigned int num_correct_root_hdcg = 0;
  unsigned int num_incorrect_root_hdcg = 0;
  unsigned int num_correct_complete_hdcg = 0;
  unsigned int num_incorrect_complete_hdcg = 0;
  double num_concrete_hdcg = 0.0;
  double num_abstract_avg_hdcg = 0.0;
  double num_abstract_max_hdcg = 0.0;

  // Runtime
  double runtime_hdcg = 0.0;
  double runtime_hdcg_norm = 0.0;
 
  // Search Space Sizes
  double dataset_concrete_hdcg = 0.0;
  double dataset_abstract_avg_hdcg = 0.0;
  double dataset_abstract_max_hdcg = 0.0;

  // Averages.
  double average_runtime_ratio = 0.0;

  /******* Iterate over the test group **************************/
  xmlNodePtr test_group_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( testGroup.c_str() ), NULL );

  for( unsigned int j = 0; j < filenames.size(); j++ ){
    cout << "running " << testGroup << " " << j << " (" << filenames[ j ] << ")" << endl;

    string filename = boost::filesystem::path( filenames[ j ] ).filename().generic_string();
    xmlNodePtr example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "example" ), NULL );
    xmlNewProp( example_node, ( const xmlChar* )( "filename" ), ( const xmlChar* )( filenames[ j ].c_str() ) );

    // World
    World * world = new World();
    world->from_xml( filenames[ j ] );

    // Phrase truth. Note no parsing.
    Phrase* truth_phrase = new Phrase();
    truth_phrase->from_xml( filenames[ j ], world );
    // Phrase input.
    Phrase * input_phrase = new Phrase();
    input_phrase->from_xml( filenames[ j ], world );
    clear( input_phrase );

    // Search Space and fill the space of groundings.
    Search_Space * search_space = new Search_Space();
    struct timeval start_time;
    gettimeofday( &start_time, NULL );
    search_space->fill_groundings( *symbol_dictionary, world );
    struct timeval end_time;
    gettimeofday( &end_time, NULL );
    cout << "finished fill_seach_space in " << diff_time( start_time, end_time ) << " seconds" << endl;

    // Grounding context
    Grounding * context = NULL;

    /******************* Running HDCG ********************************/
    //HDCG: leaf search
    HDCG * hdcg = new HDCG();
    cout << "  running hdcg" << endl;
    uint64_t hdcg_start_time = current_time();
    hdcg->leaf_search( input_phrase, *symbol_dictionary, search_space, world, 
                      context, llm, beam_width, ( bool )( debug ) );
    uint64_t hdcg_end_time = current_time();
    runtime_hdcg += microseconds_to_seconds( hdcg_end_time - hdcg_start_time );

    assert( dynamic_cast< const Phrase* >( hdcg->solutions().front().second ) != NULL );
    // If the number of phrases are more than one then compute norm.
    if ( hdcg->solutions().front().second->num_phrases() ){
      runtime_hdcg_norm += microseconds_to_seconds( ( hdcg_end_time - hdcg_start_time ) / ( hdcg->solutions().front().second->num_phrases() )  );
    }

    stringstream hdcg_solution_filename;
    hdcg_solution_filename << solution_directory << "/test_" << setw( 4 ) << setfill( '0' ) << cur_test_num << "_hdcg_solution_" << filename;
    solution_to_xml( hdcg_solution_filename.str(), world, hdcg->solutions().front().second );

    xmlNodePtr hdcg_example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "hdcg" ), NULL );

    // Runtime.
    stringstream hdcg_example_runtime_string;
    hdcg_example_runtime_string << microseconds_to_seconds( hdcg_end_time - hdcg_start_time );
    xmlNewProp( hdcg_example_node, ( const xmlChar* )( "runtime" ), ( const xmlChar* )( hdcg_example_runtime_string.str().c_str() ) );
    cout << "    runtime:" << hdcg_example_runtime_string.str() << endl;

    // Runtime normalised.
    stringstream hdcg_example_runtime_norm_string;
    hdcg_example_runtime_norm_string << microseconds_to_seconds( ( hdcg_end_time - hdcg_start_time ) / ( hdcg->solutions().front().second->num_phrases() ) ); 
    xmlNewProp( hdcg_example_node, ( const xmlChar* )( "runtime_norm" ), ( const xmlChar* )( hdcg_example_runtime_norm_string.str().c_str() ) );
    cout << "    runtime norm:" << hdcg_example_runtime_norm_string.str() << endl;

    // Search space size: concrete
    double tmp_hdcg_stats = 0.0;
    
    tmp_hdcg_stats = hdcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" );
    num_concrete_hdcg += tmp_hdcg_stats;
    xmlNewProp( hdcg_example_node, ( const xmlChar* )( "search_space_concrete_size" ), (const xmlChar* )( to_std_string( tmp_hdcg_stats ).c_str() ) );

    // Search space size: abstract average
    tmp_hdcg_stats = hdcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" );
    num_abstract_avg_hdcg += tmp_hdcg_stats;
    xmlNewProp( hdcg_example_node, ( const xmlChar* )( "search_space_abstract_avg_size" ), (const xmlChar* )( to_std_string( tmp_hdcg_stats ).c_str() ) );

    // Search space size: abstract max 
    tmp_hdcg_stats = hdcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" );
    num_abstract_max_hdcg += tmp_hdcg_stats;
    xmlNewProp( hdcg_example_node, ( const xmlChar* )( "search_space_abstract_max_size" ), (const xmlChar* )( to_std_string( tmp_hdcg_stats ).c_str() ) );

    // Accuracy statistics.
    if( root_compare_phrases( *truth_phrase, *static_cast< Phrase* >( hdcg->solutions().front().second ) ) ){
      cout << "    correct (root)" << endl;
      xmlNewProp( hdcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_root_hdcg++;
    } else {
      cout << "    incorrect (root)" << endl;
      xmlNewProp( hdcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_root_hdcg++;
    }

    if( complete_compare_phrases( *truth_phrase, *static_cast< Phrase* >( hdcg->solutions().front().second ) ) ){
      cout << "    correct (complete)" << endl;
      xmlNewProp( hdcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_complete_hdcg++;
    } else {
      cout << "    incorrect (complete)" << endl;
      xmlNewProp( hdcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_complete_hdcg++;
    }

    // Add the hdcg_example_node to the example_node in the xml file.
    xmlAddChild( example_node, hdcg_example_node );
    // clear the input_phrase for groundings to run the test for the next model.
    clear( input_phrase );
 
    if( debug ) {
      cout << "hdcg_concrete_size" << to_std_string( hdcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" ) ) << endl;
      cout << "hdcg_abstract_avg_size" << to_std_string( hdcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" ) ) << endl;
      cout << "hdcg_abstract_avg_size" << to_std_string( hdcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" ) ) << endl;
    }
  
    //find example_runtime_object_ratio_hdcg && example_runtime_object_ratio_ahdcg
    unsigned int example_num_objects = world->objects().size(); 
    
    num_objects = num_objects + ( double ) example_num_objects;
    num_phrases = num_phrases + ( double ) truth_phrase->num_phrases();

    double example_runtime_object_ratio_hdcg = ( microseconds_to_seconds( hdcg_end_time - hdcg_start_time ) )/ ( example_num_objects ) ;

    //add # of phrases to example node
    stringstream example_num_phrases_string;
    example_num_phrases_string << truth_phrase->num_phrases();
    xmlNewProp( example_node, ( const xmlChar* )( "num_phrases" ), ( const xmlChar* )( example_num_phrases_string.str().c_str() ) );

    //add # of world objects to example node
    stringstream example_num_objects_string;
    example_num_objects_string << example_num_objects;
    xmlNewProp( example_node, ( const xmlChar* )( "world_objects" ), ( const xmlChar* )( example_num_objects_string.str().c_str() ) );

    stringstream example_runtime_object_ratio_hdcg_string;
    example_runtime_object_ratio_hdcg_string << example_runtime_object_ratio_hdcg;
    xmlNewProp( example_node, ( const xmlChar* )( "runtime_object_ratio_hdcg" ), ( const xmlChar* )( example_runtime_object_ratio_hdcg_string.str().c_str() ) );

    // Export the world to xml. 
    world->to_xml( docPtr, example_node );

    // Clean up.
    //delete_ptr< Grounding >( context );
    delete_ptr< HDCG >( hdcg );
    delete_ptr< World >( world );
    delete_ptr< Phrase >( input_phrase );
    delete_ptr< Phrase >( truth_phrase );
    delete_ptr< Search_Space >( search_space );

    // Add the node in XML.
    xmlAddChild( test_group_node, example_node );
  }

/****************************** Write the statistics to XML: HADCG and AHADCG*************************/

  // Average runtime.
  runtime_hdcg /= ( double )( filenames.size() );
  runtime_hdcg_norm /= ( double )( filenames.size() );

  // Avg. runtime ratio
  average_runtime_ratio /= ( double )( filenames.size() );
 
  // Average number of phrases and objects 
  num_phrases /= ( double )( filenames.size() );
  num_objects /= ( double )( filenames.size() );

  // Correct root for all models.
  double correct_root_hdcg = ( double )( num_correct_root_hdcg ) / ( double )( num_correct_root_hdcg + num_incorrect_root_hdcg );
  // Correct complete for all models.
  double correct_complete_hdcg = ( double )( num_correct_complete_hdcg ) / ( double )( num_correct_complete_hdcg + num_incorrect_complete_hdcg );
  // Search Space
  // Search Space Sizes
  dataset_concrete_hdcg = num_concrete_hdcg / ( double )( filenames.size() ) ;
  dataset_abstract_avg_hdcg = num_abstract_avg_hdcg / ( double )( filenames.size() ) ;
  dataset_abstract_max_hdcg = num_abstract_max_hdcg / ( double )( filenames.size() );

  /******  XML: correct root **************************/ 
  stringstream correct_root_hdcg_string;
  correct_root_hdcg_string << correct_root_hdcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_hdcg" ), ( const xmlChar* )( correct_root_hdcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_hdcg" ).c_str() ), ( const xmlChar* )( correct_root_hdcg_string.str().c_str() ) );

  /******  XML: correct complete  **************************/ 
  stringstream correct_complete_hdcg_string;
  correct_complete_hdcg_string << correct_complete_hdcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_hdcg" ), ( const xmlChar* )( correct_complete_hdcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_hdcg" ).c_str() ), ( const xmlChar* )( correct_complete_hdcg_string.str().c_str() ) );

  /******  XML: runtime  **************************/ 
  stringstream runtime_hdcg_string;
  runtime_hdcg_string << runtime_hdcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_hdcg" ), ( const xmlChar* )( runtime_hdcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_hdcg" ).c_str() ), ( const xmlChar* )( runtime_hdcg_string.str().c_str() ) );

  /******  XML: runtime normalised  **************************/ 
  stringstream runtime_hdcg_norm_string;
  runtime_hdcg_norm_string << runtime_hdcg_norm;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_hdcg_norm" ), ( const xmlChar* )( runtime_hdcg_norm_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_hdcg_norm" ).c_str() ), ( const xmlChar* )( runtime_hdcg_norm_string.str().c_str() ) );

  /******  XML: runtime  **************************/ 
  stringstream runtime_ratio_string;
  runtime_ratio_string << average_runtime_ratio;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_ratio" ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_ratio" ).c_str() ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );

  /******  XML: num phrases  **************************/ 
  stringstream num_phrases_string;
  num_phrases_string << num_phrases;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_phrases" ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_phrases" ).c_str() ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );

  /******  XML: num objects  **************************/ 
  stringstream num_objects_string;
  num_objects_string << num_objects;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_objects" ), ( const xmlChar* )( num_objects_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_objects" ).c_str() ), ( const xmlChar* )( num_objects_string.str().c_str() ) );

  /******  XML: num objects  **************************/ 
  stringstream runtime_object_ratio_hdcg_string;
  runtime_object_ratio_hdcg_string << ( runtime_hdcg / num_objects );
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_object_ratio_hdcg" ), ( const xmlChar* )( runtime_object_ratio_hdcg_string.str().c_str() ) );

  /******  console output **************************/ 
  cout << endl;
  cout << "correct_root_hdcg: " << correct_root_hdcg << endl;

  cout << "correct_complete_hdcg: " << correct_complete_hdcg << endl;
  cout << "runtime_hdcg: " << runtime_hdcg << endl;
  cout << "runtime_hdcg_norm: " << runtime_hdcg_norm << endl;

  cout << "hdcg_search_space_concrete: " << dataset_concrete_hdcg << endl;
  cout << "hdcg_search_space_abstract_avg: " << dataset_abstract_avg_hdcg << endl;
  cout << "hdcg_search_space_abstract_max: " << dataset_abstract_max_hdcg << endl;
  xmlAddChild( parentNode, test_group_node );

  return;

}

/* 
 * Run tests for the HADCG model. 
 */
void
run_tests_hadcg( const std::vector< std::string >& filenames,
            LLM*& llm,
            xmlDocPtr& docPtr,
            xmlNodePtr& parentNode,
            const string& testGroup,
            const unsigned int cur_test_num,
            const string& solution_directory, 
            const Symbol_Dictionary* symbol_dictionary, 
            const unsigned int beam_width,
            const bool debug ){

  /******* Variables to record statistics **************************/
  // Num objects. Phrases.
  double num_objects = 0.0;
  double num_phrases = 0.0;

  // HADCG
  unsigned int num_correct_root_hadcg = 0;
  unsigned int num_incorrect_root_hadcg = 0;
  unsigned int num_correct_complete_hadcg = 0;
  unsigned int num_incorrect_complete_hadcg = 0;
  double num_concrete_hadcg = 0.0;
  double num_abstract_avg_hadcg = 0.0;
  double num_abstract_max_hadcg = 0.0;

  // Runtime
  double runtime_hadcg = 0.0;
  double runtime_hadcg_norm = 0.0;
 
  // Search Space Sizes
  double dataset_concrete_hadcg = 0.0;
  double dataset_abstract_avg_hadcg = 0.0;
  double dataset_abstract_max_hadcg = 0.0;

  // Averages.
  double average_runtime_ratio = 0.0;

  /******* Iterate over the test group **************************/
  xmlNodePtr test_group_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( testGroup.c_str() ), NULL );

  for( unsigned int j = 0; j < filenames.size(); j++ ){
    cout << "running " << testGroup << " " << j << " (" << filenames[ j ] << ")" << endl;

    string filename = boost::filesystem::path( filenames[ j ] ).filename().generic_string();
    xmlNodePtr example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "example" ), NULL );
    xmlNewProp( example_node, ( const xmlChar* )( "filename" ), ( const xmlChar* )( filenames[ j ].c_str() ) );

    // World
    World * world = new World();
    world->from_xml( filenames[ j ] );

    // Phrase truth. Note no parsing.
    Phrase* truth_phrase = new Phrase();
    truth_phrase->from_xml( filenames[ j ], world );
    // Phrase input.
    Phrase * input_phrase = new Phrase();
    input_phrase->from_xml( filenames[ j ], world );
    clear( input_phrase );

    // Search Space and fill the space of groundings.
    Search_Space * search_space = new Search_Space();
    struct timeval start_time;
    gettimeofday( &start_time, NULL );
    search_space->fill_groundings( *symbol_dictionary, world );
    struct timeval end_time;
    gettimeofday( &end_time, NULL );
    cout << "finished fill_seach_space in " << diff_time( start_time, end_time ) << " seconds" << endl;

    // Grounding context
    Grounding * context = NULL;

    /******************* Running HADCG ********************************/
    //HADCG: leaf search
    HADCG * hadcg = new HADCG();
    cout << "  running hadcg" << endl;
    uint64_t hadcg_start_time = current_time();
    hadcg->leaf_search( input_phrase, *symbol_dictionary, search_space, world, 
                      context, llm, beam_width, ( bool )( debug ) );
    uint64_t hadcg_end_time = current_time();
    runtime_hadcg += microseconds_to_seconds( hadcg_end_time - hadcg_start_time );

    assert( dynamic_cast< const Phrase* >( hadcg->solutions().front().second ) != NULL );
    // If the number of phrases are more than one then compute norm.
    if ( hadcg->solutions().front().second->num_phrases() ){
      runtime_hadcg_norm += microseconds_to_seconds( ( hadcg_end_time - hadcg_start_time ) / ( hadcg->solutions().front().second->num_phrases() )  );
    }

    stringstream hadcg_solution_filename;
    hadcg_solution_filename << solution_directory << "/test_" << setw( 4 ) << setfill( '0' ) << cur_test_num << "_hadcg_solution_" << filename;
    solution_to_xml( hadcg_solution_filename.str(), world, hadcg->solutions().front().second );

    xmlNodePtr hadcg_example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "hadcg" ), NULL );

    // Runtime.
    stringstream hadcg_example_runtime_string;
    hadcg_example_runtime_string << microseconds_to_seconds( hadcg_end_time - hadcg_start_time );
    xmlNewProp( hadcg_example_node, ( const xmlChar* )( "runtime" ), ( const xmlChar* )( hadcg_example_runtime_string.str().c_str() ) );
    cout << "    runtime:" << hadcg_example_runtime_string.str() << endl;

    // Runtime normalised.
    stringstream hadcg_example_runtime_norm_string;
    hadcg_example_runtime_norm_string << microseconds_to_seconds( ( hadcg_end_time - hadcg_start_time ) / ( hadcg->solutions().front().second->num_phrases() ) ); 
    xmlNewProp( hadcg_example_node, ( const xmlChar* )( "runtime_norm" ), ( const xmlChar* )( hadcg_example_runtime_norm_string.str().c_str() ) );
    cout << "    runtime norm:" << hadcg_example_runtime_norm_string.str() << endl;

    // Search space size: concrete
    double tmp_hadcg_stats = 0.0;
    
    tmp_hadcg_stats = hadcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" );
    num_concrete_hadcg += tmp_hadcg_stats;
    xmlNewProp( hadcg_example_node, ( const xmlChar* )( "search_space_concrete_size" ), (const xmlChar* )( to_std_string( tmp_hadcg_stats ).c_str() ) );

    // Search space size: abstract average
    tmp_hadcg_stats = hadcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" );
    num_abstract_avg_hadcg += tmp_hadcg_stats;
    xmlNewProp( hadcg_example_node, ( const xmlChar* )( "search_space_abstract_avg_size" ), (const xmlChar* )( to_std_string( tmp_hadcg_stats ).c_str() ) );

    // Search space size: abstract max 
    tmp_hadcg_stats = hadcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" );
    num_abstract_max_hadcg += tmp_hadcg_stats;
    xmlNewProp( hadcg_example_node, ( const xmlChar* )( "search_space_abstract_max_size" ), (const xmlChar* )( to_std_string( tmp_hadcg_stats ).c_str() ) );

    // Accuracy statistics.
    if( root_compare_phrases( *truth_phrase, *static_cast< Phrase* >( hadcg->solutions().front().second ) ) ){
      cout << "    correct (root)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_root_hadcg++;
    } else {
      cout << "    incorrect (root)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_root_hadcg++;
    }

    if( complete_compare_phrases( *truth_phrase, *static_cast< Phrase* >( hadcg->solutions().front().second ) ) ){
      cout << "    correct (complete)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_complete_hadcg++;
    } else {
      cout << "    incorrect (complete)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_complete_hadcg++;
    }

    // Add the hadcg_example_node to the example_node in the xml file.
    xmlAddChild( example_node, hadcg_example_node );
    // clear the input_phrase for groundings to run the test for the next model.
    clear( input_phrase );
 
    if( debug ) {
      cout << "hadcg_concrete_size" << to_std_string( hadcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" ) ) << endl;
      cout << "hadcg_abstract_avg_size" << to_std_string( hadcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" ) ) << endl;
      cout << "hadcg_abstract_avg_size" << to_std_string( hadcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" ) ) << endl;
    }
  
    //find example_runtime_object_ratio_hadcg && example_runtime_object_ratio_ahadcg
    unsigned int example_num_objects = world->objects().size(); 
    
    num_objects = num_objects + ( double ) example_num_objects;
    num_phrases = num_phrases + ( double ) truth_phrase->num_phrases();

    double example_runtime_object_ratio_hadcg = ( microseconds_to_seconds( hadcg_end_time - hadcg_start_time ) )/ ( example_num_objects ) ;

    //add # of phrases to example node
    stringstream example_num_phrases_string;
    example_num_phrases_string << truth_phrase->num_phrases();
    xmlNewProp( example_node, ( const xmlChar* )( "num_phrases" ), ( const xmlChar* )( example_num_phrases_string.str().c_str() ) );

    //add # of world objects to example node
    stringstream example_num_objects_string;
    example_num_objects_string << example_num_objects;
    xmlNewProp( example_node, ( const xmlChar* )( "world_objects" ), ( const xmlChar* )( example_num_objects_string.str().c_str() ) );

    stringstream example_runtime_object_ratio_hadcg_string;
    example_runtime_object_ratio_hadcg_string << example_runtime_object_ratio_hadcg;
    xmlNewProp( example_node, ( const xmlChar* )( "runtime_object_ratio_hadcg" ), ( const xmlChar* )( example_runtime_object_ratio_hadcg_string.str().c_str() ) );

    // Export the world to xml. 
    world->to_xml( docPtr, example_node );

    // Clean up.
    //delete_ptr< Grounding >( context );
    delete_ptr< HADCG >( hadcg );
    delete_ptr< World >( world );
    delete_ptr< Phrase >( input_phrase );
    delete_ptr< Phrase >( truth_phrase );
    delete_ptr< Search_Space >( search_space );

    // Add the node in XML.
    xmlAddChild( test_group_node, example_node );
  }

/****************************** Write the statistics to XML: HADCG and AHADCG*************************/

  // Average runtime.
  runtime_hadcg /= ( double )( filenames.size() );
  runtime_hadcg_norm /= ( double )( filenames.size() );

  // Avg. runtime ratio
  average_runtime_ratio /= ( double )( filenames.size() );
 
  // Average number of phrases and objects 
  num_phrases /= ( double )( filenames.size() );
  num_objects /= ( double )( filenames.size() );

  // Correct root for all models.
  double correct_root_hadcg = ( double )( num_correct_root_hadcg ) / ( double )( num_correct_root_hadcg + num_incorrect_root_hadcg );
  // Correct complete for all models.
  double correct_complete_hadcg = ( double )( num_correct_complete_hadcg ) / ( double )( num_correct_complete_hadcg + num_incorrect_complete_hadcg );
  // Search Space
  // Search Space Sizes
  dataset_concrete_hadcg = num_concrete_hadcg / ( double )( filenames.size() ) ;
  dataset_abstract_avg_hadcg = num_abstract_avg_hadcg / ( double )( filenames.size() ) ;
  dataset_abstract_max_hadcg = num_abstract_max_hadcg / ( double )( filenames.size() );

  /******  XML: correct root **************************/ 
  stringstream correct_root_hadcg_string;
  correct_root_hadcg_string << correct_root_hadcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_hadcg" ), ( const xmlChar* )( correct_root_hadcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_hadcg" ).c_str() ), ( const xmlChar* )( correct_root_hadcg_string.str().c_str() ) );

  /******  XML: correct complete  **************************/ 
  stringstream correct_complete_hadcg_string;
  correct_complete_hadcg_string << correct_complete_hadcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_hadcg" ), ( const xmlChar* )( correct_complete_hadcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_hadcg" ).c_str() ), ( const xmlChar* )( correct_complete_hadcg_string.str().c_str() ) );

  /******  XML: runtime  **************************/ 
  stringstream runtime_hadcg_string;
  runtime_hadcg_string << runtime_hadcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_hadcg" ), ( const xmlChar* )( runtime_hadcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_hadcg" ).c_str() ), ( const xmlChar* )( runtime_hadcg_string.str().c_str() ) );

  /******  XML: runtime normalised **************************/ 
  stringstream runtime_hadcg_norm_string;
  runtime_hadcg_norm_string << runtime_hadcg_norm;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_hadcg_norm" ), ( const xmlChar* )( runtime_hadcg_norm_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_hadcg_norm" ).c_str() ), ( const xmlChar* )( runtime_hadcg_norm_string.str().c_str() ) );

  /******  XML: runtime  **************************/ 
  stringstream runtime_ratio_string;
  runtime_ratio_string << average_runtime_ratio;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_ratio" ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_ratio" ).c_str() ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );

  /******  XML: num phrases  **************************/ 
  stringstream num_phrases_string;
  num_phrases_string << num_phrases;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_phrases" ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_phrases" ).c_str() ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );

  /******  XML: num objects  **************************/ 
  stringstream num_objects_string;
  num_objects_string << num_objects;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_objects" ), ( const xmlChar* )( num_objects_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_objects" ).c_str() ), ( const xmlChar* )( num_objects_string.str().c_str() ) );

  /******  XML: num objects  **************************/ 
  stringstream runtime_object_ratio_hadcg_string;
  runtime_object_ratio_hadcg_string << ( runtime_hadcg / num_objects );
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_object_ratio_hadcg" ), ( const xmlChar* )( runtime_object_ratio_hadcg_string.str().c_str() ) );

  /******  console output **************************/ 
  cout << endl;
  cout << "correct_root_hadcg: " << correct_root_hadcg << endl;

  cout << "correct_complete_hadcg: " << correct_complete_hadcg << endl;
  cout << "runtime_hadcg: " << runtime_hadcg << endl;
  cout << "runtime_hadcg_norm: " << runtime_hadcg_norm << endl;

  cout << "hadcg_search_space_concrete: " << dataset_concrete_hadcg << endl;
  cout << "hadcg_search_space_abstract_avg: " << dataset_abstract_avg_hadcg << endl;
  cout << "hadcg_search_space_abstract_max: " << dataset_abstract_max_hadcg << endl;
  xmlAddChild( parentNode, test_group_node );

  return;

}

/**
 * Run the grounding model comparison tests.
 * Give the list of filenames.
 * Provide the trained LLM.a
 * Pass the symbol_dictionary_path and 
 **/
void
run_tests( const std::vector< std::string >& filenames,
            LLM*& llm,
            xmlDocPtr& docPtr,
            xmlNodePtr& parentNode,
            const string& testGroup,
            const unsigned int cur_test_num,
            const string& solution_directory, 
            const Symbol_Dictionary* symbol_dictionary, 
            const unsigned int beam_width,
            const bool debug ){

  /******* Variables to record statistics **************************/
  // Num objects. Phrases.
  double num_objects = 0.0;
  double num_phrases = 0.0;

  // DCG
  unsigned int num_correct_root_dcg = 0;
  unsigned int num_incorrect_root_dcg = 0;
  unsigned int num_correct_complete_dcg = 0;
  unsigned int num_incorrect_complete_dcg = 0;
  double num_concrete_dcg = 0.0;
  double num_abstract_avg_dcg = 0.0;
  double num_abstract_max_dcg = 0.0;

  // ADCG
  unsigned int num_correct_root_adcg = 0;
  unsigned int num_incorrect_root_adcg = 0;
  unsigned int num_correct_complete_adcg = 0;
  unsigned int num_incorrect_complete_adcg = 0;
  double num_concrete_adcg = 0.0;
  double num_abstract_avg_adcg = 0.0;
  double num_abstract_max_adcg = 0.0;

  // HDCG
  unsigned int num_correct_root_hdcg = 0;
  unsigned int num_incorrect_root_hdcg = 0;
  unsigned int num_correct_complete_hdcg = 0;
  unsigned int num_incorrect_complete_hdcg = 0;

  // HDCG
  unsigned int num_correct_root_hadcg = 0;
  unsigned int num_incorrect_root_hadcg = 0;
  unsigned int num_correct_complete_hadcg = 0;
  unsigned int num_incorrect_complete_hadcg = 0;

  // Runtime
  double runtime_dcg = 0.0;
  double runtime_adcg = 0.0;
  double runtime_hdcg = 0.0;
  double runtime_hadcg = 0.0;

// Search Space Sizes
  double dataset_concrete_dcg = 0.0;
  double dataset_abstract_avg_dcg = 0.0;
  double dataset_abstract_max_dcg = 0.0;
  double dataset_concrete_adcg = 0.0;
  double dataset_abstract_avg_adcg = 0.0;
  double dataset_abstract_max_adcg = 0.0;

  // Averages.
  double average_runtime_ratio = 0.0;

  /******* Iterate over the test group **************************/
  xmlNodePtr test_group_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( testGroup.c_str() ), NULL );

  for( unsigned int j = 0; j < filenames.size(); j++ ){
    cout << "running " << testGroup << " " << j << " (" << filenames[ j ] << ")" << endl;

    string filename = boost::filesystem::path( filenames[ j ] ).filename().generic_string();
    xmlNodePtr example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "example" ), NULL );
    xmlNewProp( example_node, ( const xmlChar* )( "filename" ), ( const xmlChar* )( filenames[ j ].c_str() ) );

    // World
    World * world = new World();
    world->from_xml( filenames[ j ] );

    // Phrase truth. Note no parsing.
    Phrase* truth_phrase = new Phrase();
    truth_phrase->from_xml( filenames[ j ], world );
    // Phrase input.
    Phrase * input_phrase = new Phrase();
    input_phrase->from_xml( filenames[ j ], world );
    clear( input_phrase );

    // Search Space and fill the space of groundings.
    Search_Space * search_space = new Search_Space();
    struct timeval start_time;
    gettimeofday( &start_time, NULL );
    search_space->fill_groundings( *symbol_dictionary, world );
    struct timeval end_time;
    gettimeofday( &end_time, NULL );
    cout << "finished fill_seach_space in " << diff_time( start_time, end_time ) << " seconds" << endl;

    // Grounding context
    Grounding * context = NULL;

    /******************* Running DCG ********************************/
    //DCG: leaf search
    DCG * dcg = new DCG();
    cout << "  running dcg" << endl;
    uint64_t dcg_start_time = current_time();
    dcg->leaf_search( input_phrase, *symbol_dictionary, search_space, world,
                      context, llm, beam_width, ( bool )( debug ) );
    uint64_t dcg_end_time = current_time();
    runtime_dcg += microseconds_to_seconds( dcg_end_time - dcg_start_time );

    assert( dynamic_cast< const Phrase* >( dcg->solutions().front().second ) != NULL );

    stringstream dcg_solution_filename;
    dcg_solution_filename << solution_directory << "/test_" << setw( 4 ) << setfill( '0' ) << cur_test_num << "_dcg_solution_" << filename;
    dcg->solutions().front().second->to_xml( dcg_solution_filename.str() );

    xmlNodePtr dcg_example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "dcg" ), NULL );
   // Runtime.
    stringstream dcg_example_runtime_string;
    dcg_example_runtime_string << microseconds_to_seconds( dcg_end_time - dcg_start_time );
    xmlNewProp( dcg_example_node, ( const xmlChar* )( "runtime" ), ( const xmlChar* )( dcg_example_runtime_string.str().c_str() ) );
    cout << "    runtime:" << dcg_example_runtime_string.str() << endl;

    // Search space size: concrete
    double tmp_dcg_stats = 0.0;

    tmp_dcg_stats = dcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" );
    num_concrete_dcg += tmp_dcg_stats;
    xmlNewProp( dcg_example_node, ( const xmlChar* )( "search_space_concrete_size" ), (const xmlChar* )( to_std_string( tmp_dcg_stats ).c_str() ) );

    // Search space size: abstract average
    tmp_dcg_stats = dcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" );
    num_abstract_avg_dcg += tmp_dcg_stats;
    xmlNewProp( dcg_example_node, ( const xmlChar* )( "search_space_abstract_avg_size" ), (const xmlChar* )( to_std_string( tmp_dcg_stats ).c_str() ) );

    // Search space size: abstract max 
    tmp_dcg_stats = dcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" );
    num_abstract_max_dcg += tmp_dcg_stats;
    xmlNewProp( dcg_example_node, ( const xmlChar* )( "search_space_abstract_max_size" ), (const xmlChar* )( to_std_string( tmp_dcg_stats ).c_str() ) );

    // Accuracy statistics.
    if( root_compare_phrases( *truth_phrase, *static_cast< Phrase* >( dcg->solutions().front().second ) ) ){
      cout << "    correct (root)" << endl;
      xmlNewProp( dcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_root_dcg++;
    } else {
      cout << "    incorrect (root)" << endl;
      xmlNewProp( dcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_root_dcg++;
    }

    if( complete_compare_phrases( *truth_phrase, *static_cast< Phrase* >( dcg->solutions().front().second ) ) ){
      cout << "    correct (complete)" << endl;
      xmlNewProp( dcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_complete_dcg++;
    } else {
      cout << "    incorrect (complete)" << endl;
      xmlNewProp( dcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_complete_dcg++;
    }

    // Add the dcg_example_node to the example_node in the xml file.
    xmlAddChild( example_node, dcg_example_node );
    // clear the input_phrase for groundings to run the test for the next model.
    clear( input_phrase );

    if( debug ) {
      cout << "dcg_concrete_size" << to_std_string( dcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" ) ) << endl;
      cout << "dcg_abstract_avg_size" << to_std_string( dcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" ) ) << endl;
      cout << "dcg_abstract_avg_size" << to_std_string( dcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" ) ) << endl;
    }

  /**************************************** ADCG ******************************************************************/
    ADCG * adcg = new ADCG();

    cout << "  running adcg" << endl;
    uint64_t adcg_start_time = current_time();
    adcg->leaf_search( input_phrase, *symbol_dictionary, search_space, world, context, llm, beam_width, debug );
    uint64_t adcg_end_time = current_time();
    runtime_adcg += microseconds_to_seconds( adcg_end_time - adcg_start_time );

    assert( dynamic_cast< const Phrase* >( adcg->solutions().front().second ) != NULL );

    stringstream adcg_solution_filename;
    adcg_solution_filename << solution_directory << "/test_" << setw( 4 ) << setfill( '0' ) << cur_test_num << "_adcg_solution_" << filename;
    adcg->solutions().front().second->to_xml( adcg_solution_filename.str() );

    xmlNodePtr adcg_example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "adcg" ), NULL );
    stringstream adcg_example_runtime_string;
    adcg_example_runtime_string << microseconds_to_seconds( adcg_end_time - adcg_start_time );
    xmlNewProp( adcg_example_node, ( const xmlChar* )( "runtime" ), ( const xmlChar* )( adcg_example_runtime_string.str().c_str() ) );

    // Runtime.
    cout << "    runtime:" << adcg_example_runtime_string.str() << endl;

    // Search space size: concrete
    double tmp_adcg_stats = 0.0;

    tmp_adcg_stats = adcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" );
    num_concrete_adcg += tmp_adcg_stats;
    xmlNewProp( adcg_example_node, ( const xmlChar* )( "search_space_concrete_size" ), (const xmlChar* )( to_std_string( tmp_adcg_stats ).c_str() ) );

    // Search space size: abstract average
    tmp_adcg_stats = adcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" );
    num_abstract_avg_adcg += tmp_adcg_stats;
    xmlNewProp( adcg_example_node, ( const xmlChar* )( "search_space_abstract_avg_size" ), (const xmlChar* )( to_std_string( tmp_adcg_stats ).c_str() ) );

    // Search space size: abstract max 
    tmp_adcg_stats = adcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" );
    num_abstract_max_adcg += tmp_adcg_stats;
    xmlNewProp( adcg_example_node, ( const xmlChar* )( "search_space_abstract_max_size" ), (const xmlChar* )( to_std_string( tmp_adcg_stats ).c_str() ) );

    // Accuracy
    if( root_compare_phrases( *truth_phrase, *static_cast< Phrase* >( adcg->solutions().front().second ) ) ){
      cout << "    correct (root)" << endl;
      xmlNewProp( adcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_root_adcg++;
    } else {
      cout << "    incorrect (root)" << endl;
      xmlNewProp( adcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_root_adcg++;
    }

    if( complete_compare_phrases( *truth_phrase, *static_cast< Phrase* >( adcg->solutions().front().second ) ) ){
      cout << "    correct (complete)" << endl;
      xmlNewProp( adcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_complete_adcg++;
    } else {
      cout << "    incorrect (complete)" << endl;
      xmlNewProp( adcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "false" ) );
     num_incorrect_complete_adcg++;
    }

    xmlAddChild( example_node, adcg_example_node );
    clear( input_phrase );

    if( debug ){
      cout << "adcg_concrete_size" << to_std_string( adcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" ) ) << endl;
      cout << "adcg_abstract_avg_size" << to_std_string( adcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" ) ) << endl;
      cout << "adcg_abstract_avg_size" << to_std_string( adcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" ) ) << endl;
   }

    /**************************************** HDCG ******************************************************************/
  HDCG * hdcg = new HDCG();

    cout << "  running hdcg" << endl;
    uint64_t hdcg_start_time = current_time();
    hdcg->leaf_search( input_phrase, *symbol_dictionary, search_space, world, context, llm, beam_width, debug );
    uint64_t hdcg_end_time = current_time();
    runtime_hdcg += microseconds_to_seconds( hdcg_end_time - hdcg_start_time );

    assert( dynamic_cast< const Phrase* >( hdcg->solutions().front().second ) != NULL );

    stringstream hdcg_solution_filename;
    hdcg_solution_filename << solution_directory << "/test_" << setw( 4 ) << setfill( '0' ) << cur_test_num << "_hdcg_solution_" << filename;
    hdcg->solutions().front().second->to_xml( hdcg_solution_filename.str() );

    xmlNodePtr hdcg_example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "hdcg" ), NULL );
    stringstream hdcg_example_runtime_string;
    hdcg_example_runtime_string << microseconds_to_seconds( hdcg_end_time - hdcg_start_time );
    xmlNewProp( hdcg_example_node, ( const xmlChar* )( "runtime" ), ( const xmlChar* )( hdcg_example_runtime_string.str().c_str() ) );

    cout << "    runtime:" << hdcg_example_runtime_string.str() << endl;

    if( root_compare_phrases( *truth_phrase, *static_cast< Phrase* >( hdcg->solutions().front().second ) ) ){
      cout << "    correct (root)" << endl;
      xmlNewProp( hdcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_root_hdcg++;
    } else {
      cout << "    incorrect (root)" << endl;
      xmlNewProp( hdcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_root_hdcg++;
    }

    if( complete_compare_phrases( *truth_phrase, *static_cast< Phrase* >( hdcg->solutions().front().second ) ) ){
      cout << "    correct (complete)" << endl;
      xmlNewProp( hdcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_complete_hdcg++;
    } else {
      cout << "    incorrect (complete)" << endl;
      xmlNewProp( hdcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_complete_hdcg++;
    }

    xmlAddChild( example_node, hdcg_example_node );
    clear( input_phrase );

   /**************************************** HADCG ******************************************************************/
    HADCG * hadcg = new HADCG();

    cout << "  running hadcg" << endl;
    uint64_t hadcg_start_time = current_time();

    hadcg->leaf_search( input_phrase, *symbol_dictionary, search_space, world, context, llm, beam_width, debug );

    uint64_t hadcg_end_time = current_time();
    runtime_hadcg += microseconds_to_seconds( hadcg_end_time - hadcg_start_time );

    assert( dynamic_cast< const Phrase* >( hadcg->solutions().front().second ) != NULL );

    stringstream hadcg_solution_filename;
    hadcg_solution_filename << solution_directory << "/test_" << setw( 4 ) << setfill( '0' ) << cur_test_num << "_hadcg_solution_" << filename;
    hadcg->solutions().front().second->to_xml( hadcg_solution_filename.str() );

    xmlNodePtr hadcg_example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "hadcg" ), NULL );
    stringstream hadcg_example_runtime_string;
    hadcg_example_runtime_string << microseconds_to_seconds( hadcg_end_time - hadcg_start_time );
    xmlNewProp( hadcg_example_node, ( const xmlChar* )( "runtime" ), ( const xmlChar* )( hadcg_example_runtime_string.str().c_str() ) );

    cout << "    runtime:" << hadcg_example_runtime_string.str() << endl;

    if( root_compare_phrases( *truth_phrase, *static_cast< Phrase* >( hadcg->solutions().front().second ) ) ){
      cout << "    correct (root)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_root_hadcg++;
    } else {
      cout << "    incorrect (root)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_root_hadcg++;
    }

    if( complete_compare_phrases( *truth_phrase, *static_cast< Phrase* >( hadcg->solutions().front().second ) ) ){
      cout << "    correct (complete)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_complete_hadcg++;
    } else {
      cout << "    incorrect (complete)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_complete_hadcg++;
    }

    xmlAddChild( example_node, hadcg_example_node );
    clear( input_phrase );
 /***************************************** Comparative stats ******************************************************/
    double runtime_ratio = microseconds_to_seconds( dcg_end_time - dcg_start_time ) / microseconds_to_seconds( adcg_end_time - adcg_start_time );
    average_runtime_ratio += runtime_ratio;

    stringstream runtime_ratio_string;
    runtime_ratio_string << runtime_ratio;
    xmlNewProp( example_node, ( const xmlChar* )( "runtime_ratio" ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );

    //find example_runtime_object_ratio_dcg && example_runtime_object_ratio_adcg
    unsigned int example_num_objects = world->objects().size();

    num_objects = num_objects + ( double ) example_num_objects;
    num_phrases = num_phrases + ( double ) truth_phrase->num_phrases();

    double example_runtime_object_ratio_dcg = ( microseconds_to_seconds( dcg_end_time - dcg_start_time ) )/ ( example_num_objects ) ;
    double example_runtime_object_ratio_adcg = ( microseconds_to_seconds( adcg_end_time - adcg_start_time ) )/ ( example_num_objects ) ;

    //add # of phrases to example node
    stringstream example_num_phrases_string;
    example_num_phrases_string << truth_phrase->num_phrases();
    xmlNewProp( example_node, ( const xmlChar* )( "num_phrases" ), ( const xmlChar* )( example_num_phrases_string.str().c_str() ) );

    //add # of world objects to example node
    stringstream example_num_objects_string;
    example_num_objects_string << example_num_objects;
    xmlNewProp( example_node, ( const xmlChar* )( "world_objects" ), ( const xmlChar* )( example_num_objects_string.str().c_str() ) );

    stringstream example_runtime_object_ratio_dcg_string;
    example_runtime_object_ratio_dcg_string << example_runtime_object_ratio_dcg;
    xmlNewProp( example_node, ( const xmlChar* )( "runtime_object_ratio_dcg" ), ( const xmlChar* )( example_runtime_object_ratio_dcg_string.str().c_str() ) );

    stringstream example_runtime_object_ratio_adcg_string;
    example_runtime_object_ratio_adcg_string << example_runtime_object_ratio_adcg;
    xmlNewProp( example_node, ( const xmlChar* )( "runtime_object_ratio_adcg" ), ( const xmlChar* )( example_runtime_object_ratio_adcg_string.str().c_str() ) );

    // Export the world to xml. 
    world->to_xml( docPtr, example_node );

    // Clean up.
    //delete_ptr< Grounding >( context );
    delete_ptr< DCG >( dcg );
    delete_ptr< ADCG >( adcg );
    delete_ptr< HDCG >( hdcg );
    delete_ptr< HADCG >( hadcg );
    delete_ptr< World >( world );
    delete_ptr< Phrase >( input_phrase );
    delete_ptr< Phrase >( truth_phrase );
    delete_ptr< Search_Space >( search_space );

    // Add the node in XML.
    xmlAddChild( test_group_node, example_node );
  }
/****************************** Write the statistics to XML: DCG and ADCG*************************/

  // Average runtime.
  runtime_dcg /= ( double )( filenames.size() );
  runtime_adcg /= ( double )( filenames.size() );
  runtime_hdcg /= ( double )( filenames.size() );
  runtime_hadcg /= ( double )( filenames.size() );

  // Avg. runtime ratio
  average_runtime_ratio /= ( double )( filenames.size() );

  // Average number of phrases and objects 
  num_phrases /= ( double )( filenames.size() );
  num_objects /= ( double )( filenames.size() );

  // Correct root for all models.
  double correct_root_dcg = ( double )( num_correct_root_dcg ) / ( double )( num_correct_root_dcg + num_incorrect_root_dcg );
  double correct_root_adcg = ( double )( num_correct_root_adcg ) / ( double )( num_correct_root_adcg + num_incorrect_root_adcg );
  double correct_root_hdcg = ( double )( num_correct_root_hdcg ) / ( double )( num_correct_root_hdcg + num_incorrect_root_hdcg );
  double correct_root_hadcg = ( double )( num_correct_root_hadcg ) / ( double )( num_correct_root_hadcg + num_incorrect_root_hadcg );

  // Correct complete for all models.
  double correct_complete_dcg = ( double )( num_correct_complete_dcg ) / ( double )( num_correct_complete_dcg + num_incorrect_complete_dcg );
  double correct_complete_adcg = ( double )( num_correct_complete_adcg ) / ( double )( num_correct_complete_adcg + num_incorrect_complete_adcg );
  double correct_complete_hdcg = ( double )( num_correct_complete_hdcg ) / ( double )( num_correct_complete_hdcg + num_incorrect_complete_hdcg );
  double correct_complete_hadcg = ( double )( num_correct_complete_hadcg ) / ( double )( num_correct_complete_hadcg + num_incorrect_complete_hadcg );

  // Search Space
  // Search Space Sizes
  dataset_concrete_dcg = num_concrete_dcg / ( double )( filenames.size() ) ;
  dataset_abstract_avg_dcg = num_abstract_avg_dcg / ( double )( filenames.size() ) ;
  dataset_abstract_max_dcg = num_abstract_avg_dcg / ( double )( filenames.size() );
  dataset_concrete_adcg = num_concrete_adcg / ( double )( filenames.size() ) ;
  dataset_abstract_avg_adcg = num_abstract_avg_adcg / ( double )( filenames.size() ) ;
  dataset_abstract_max_adcg = num_abstract_avg_adcg / ( double )( filenames.size() );

 /******  XML: correct root **************************/
  stringstream correct_root_dcg_string;
  correct_root_dcg_string << correct_root_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_dcg" ), ( const xmlChar* )( correct_root_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_dcg" ).c_str() ), ( const xmlChar* )( correct_root_dcg_string.str().c_str() ) );

  stringstream correct_root_adcg_string;
  correct_root_adcg_string << correct_root_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_adcg" ), ( const xmlChar* )( correct_root_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_adcg" ).c_str() ), ( const xmlChar* )( correct_root_adcg_string.str().c_str() ) );

  stringstream correct_root_hdcg_string;
  correct_root_hdcg_string << correct_root_hdcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_hdcg" ), ( const xmlChar* )( correct_root_hdcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_hdcg" ).c_str() ), ( const xmlChar* )( correct_root_hdcg_string.str().c_str() ) );

  stringstream correct_root_hadcg_string;
  correct_root_hadcg_string << correct_root_hadcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_hadcg" ), ( const xmlChar* )( correct_root_hadcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_hadcg" ).c_str() ), ( const xmlChar* )( correct_root_hadcg_string.str().c_str() ) );

  /******  XML: correct root ratio for dcg and adcg **************************/
  stringstream correct_root_ratio_dcg_adcg_string;
  correct_root_ratio_dcg_adcg_string << correct_root_dcg / correct_root_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_ratio_dcg_adcg" ), ( const xmlChar* )( correct_root_ratio_dcg_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_ratio_dcg_adcg" ).c_str() ), ( const xmlChar* )( correct_root_ratio_dcg_adcg_string.str().c_str() ) );

  /******  XML: correct root ratio for hdcg and hadcg **************************/
  stringstream correct_root_ratio_hdcg_hadcg_string;
  correct_root_ratio_hdcg_hadcg_string << correct_root_hdcg / correct_root_hadcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_ratio_hdcg_hadcg" ), ( const xmlChar* )( correct_root_ratio_hdcg_hadcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_ratio_hdcg_hadcg" ).c_str() ), ( const xmlChar* )( correct_root_ratio_hdcg_hadcg_string.str().c_str() ) );

 /******  XML: correct complete  **************************/
  stringstream correct_complete_dcg_string;
  correct_complete_dcg_string << correct_complete_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_dcg" ), ( const xmlChar* )( correct_complete_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_dcg" ).c_str() ), ( const xmlChar* )( correct_complete_dcg_string.str().c_str() ) );

  stringstream correct_complete_adcg_string;
  correct_complete_adcg_string << correct_complete_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_adcg" ), ( const xmlChar* )( correct_complete_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_adcg" ).c_str() ), ( const xmlChar* )( correct_complete_adcg_string.str().c_str() ) );

  stringstream correct_complete_hdcg_string;
  correct_complete_hdcg_string << correct_complete_hdcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_hdcg" ), ( const xmlChar* )( correct_complete_hdcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_hdcg" ).c_str() ), ( const xmlChar* )( correct_complete_hdcg_string.str().c_str() ) );

  stringstream correct_complete_hadcg_string;
  correct_complete_hadcg_string << correct_complete_hadcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_hadcg" ), ( const xmlChar* )( correct_complete_hadcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_hadcg" ).c_str() ), ( const xmlChar* )( correct_complete_hadcg_string.str().c_str() ) );

  /******  XML: correct complete ratio for dcg and adcg **************************/
  stringstream correct_complete_ratio_dcg_adcg_string;
  correct_complete_ratio_dcg_adcg_string << correct_complete_dcg / correct_complete_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_ratio_dcg_adcg" ), ( const xmlChar* )( correct_complete_ratio_dcg_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_ratio_dcg_adcg" ).c_str() ), ( const xmlChar* )( correct_complete_ratio_dcg_adcg_string.str().c_str() ) );

  /******  XML: correct complete ratio for hdcg and hadcg **************************/
  stringstream correct_complete_ratio_hdcg_hadcg_string;
  correct_complete_ratio_hdcg_hadcg_string << correct_complete_hdcg / correct_complete_hadcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_ratio_hdcg_hadcg" ), ( const xmlChar* )( correct_complete_ratio_hdcg_hadcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_ratio_hdcg_hadcg" ).c_str() ), ( const xmlChar* )( correct_complete_ratio_hdcg_hadcg_string.str().c_str() ) );

  /******  XML: runtime  **************************/
  stringstream runtime_dcg_string;
  runtime_dcg_string << runtime_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_dcg" ), ( const xmlChar* )( runtime_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_dcg" ).c_str() ), ( const xmlChar* )( runtime_dcg_string.str().c_str() ) );

  stringstream runtime_adcg_string;
  runtime_adcg_string << runtime_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_adcg" ), ( const xmlChar* )( runtime_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_adcg" ).c_str() ), ( const xmlChar* )( runtime_adcg_string.str().c_str() ) );

  stringstream runtime_hdcg_string;
  runtime_hdcg_string << runtime_hdcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_hdcg" ), ( const xmlChar* )( runtime_hdcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_hdcg" ).c_str() ), ( const xmlChar* )( runtime_hdcg_string.str().c_str() ) );

  stringstream runtime_hadcg_string;
  runtime_adcg_string << runtime_hadcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_hadcg" ), ( const xmlChar* )( runtime_hadcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_hadcg" ).c_str() ), ( const xmlChar* )( runtime_hadcg_string.str().c_str() ) );

  /******  XML: runtime  **************************/
  stringstream runtime_ratio_string;
  runtime_ratio_string << average_runtime_ratio;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_ratio" ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_ratio" ).c_str() ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );

  /******  XML: num phrases  **************************/
  stringstream num_phrases_string;
  num_phrases_string << num_phrases;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_phrases" ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_phrases" ).c_str() ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );

  /******  XML: num objects  **************************/
  stringstream num_objects_string;
  num_objects_string << num_objects;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_objects" ), ( const xmlChar* )( num_objects_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_objects" ).c_str() ), ( const xmlChar* )( num_objects_string.str().c_str() ) );
  /******  XML: num objects  **************************/
  stringstream runtime_object_ratio_dcg_string;
  runtime_object_ratio_dcg_string << ( runtime_dcg / num_objects );
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_object_ratio_dcg" ), ( const xmlChar* )( runtime_object_ratio_dcg_string.str().c_str() ) );

  stringstream runtime_object_ratio_adcg_string;
  runtime_object_ratio_adcg_string << ( runtime_adcg / num_objects );
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_object_ratio_adcg" ), ( const xmlChar* )( runtime_object_ratio_adcg_string.str().c_str() ) );

  stringstream runtime_object_ratio_hdcg_string;
  runtime_object_ratio_hdcg_string << ( runtime_hdcg / num_objects );
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_object_ratio_hdcg" ), ( const xmlChar* )( runtime_object_ratio_hdcg_string.str().c_str() ) );

  stringstream runtime_object_ratio_hadcg_string;
  runtime_object_ratio_hadcg_string << ( runtime_hadcg / num_objects );
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_object_ratio_hadcg" ), ( const xmlChar* )( runtime_object_ratio_hadcg_string.str().c_str() ) );

  /******  console output **************************/
  cout << endl;
  cout << "correct_root_dcg: " << correct_root_dcg << endl;
  cout << "correct_root_adcg: " << correct_root_adcg << endl;
  cout << "correct_root_hdcg: " << correct_root_hdcg << endl;
  cout << "correct_root_hadcg: " << correct_root_hadcg << endl;

  cout << "correct_root_ratio_dcg_adcg: " << correct_root_ratio_dcg_adcg_string.str() << endl;
  cout << "correct_root_ratio_hdcg_hadcg: " << correct_root_ratio_hdcg_hadcg_string.str() << endl;

  cout << "correct_complete_dcg: " << correct_complete_dcg << endl;
  cout << "correct_complete_adcg: " << correct_complete_adcg << endl;
  cout << "correct_complete_hdcg: " << correct_complete_hdcg << endl;
  cout << "correct_complete_hadcg: " << correct_complete_hadcg << endl;

  cout << "correct_complete_ratio_dcg_adcg: " << correct_complete_ratio_dcg_adcg_string.str() << endl;
  cout << "correct_complete_ratio_hdcg_hadcg: " << correct_complete_ratio_hdcg_hadcg_string.str() << endl;

  cout << "runtime_dcg: " << runtime_dcg << endl;
  cout << "runtime_adcg: " << runtime_adcg << endl;
  cout << "runtime_hdcg: " << runtime_hdcg << endl;
  cout << "runtime_hadcg: " << runtime_hadcg << endl;

  cout << "runtime_ratio: " << average_runtime_ratio << endl;

  cout << "dcg_search_space_concrete: " << dataset_concrete_dcg << endl;
  cout << "dcg_search_space_abstract_avg: " << dataset_abstract_avg_dcg << endl;
  cout << "dcg_search_space_abstract_max: " << dataset_abstract_max_dcg << endl;
  cout << "adcg_search_space_concrete: " << dataset_concrete_adcg << endl;
  cout << "adcg_search_space_abstract_avg: " << dataset_abstract_avg_adcg  << endl;
  cout << "adcg_search_space_abstract_max: " << dataset_abstract_max_adcg << endl;

  xmlAddChild( parentNode, test_group_node );

  return;

}



/**
 * Test Generator Program
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of test_process_inference program" << endl;

  // Determine the arguments provided.
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }

  // Determine the solution directory where solutions will be created.
  stringstream solution_directory;
  if( !args.solution_directory_given ){
    solution_directory << "/tmp/";
  } else{
   solution_directory << args.solution_directory_arg;
  }
  cout << "solution_directory: " << solution_directory.str() << endl;

  // XML: results document
  xmlDocPtr results_doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  // Node ptr for root node of the results document
  xmlNodePtr results_root = xmlNewDocNode( results_doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( results_doc, results_root );
  // Node ptr for test node
  xmlNodePtr results_node = xmlNewDocNode( results_doc, NULL, ( xmlChar* )( "results" ), NULL );
  // Number. Added as a node property.
  stringstream num_string;
  num_string << args.inputs_num;
  xmlNewProp( results_node, ( const xmlChar* )( "num" ), ( const xmlChar* )( num_string.str().c_str() ) );
  // test_node child node for root.
  xmlAddChild( results_root, results_node );

  /********** Load up the LLM for the corresponding partition *****************/
  
  // Iterate over the partitions. 
  for( unsigned int i = 0; i < args.inputs_num; i++ ){
    // Load the files for testing.
    xmlNodePtr test_node = xmlNewDocNode( results_doc, NULL, ( xmlChar* )( "test" ), NULL );
    xmlNewProp( test_node, ( const xmlChar* )( "filename" ), ( const xmlChar* )( args.inputs[ i ]  ) );

    vector< string > training_set;
    vector< string > test_set;

    // Load the filename for the current partition. 
    xmlDoc * input_doc = NULL;
    xmlNodePtr input_root = NULL;
    input_doc = xmlReadFile( args.inputs[ i ], NULL, 0 );

    // Separate out the training and test set files. Each contain world and the instruction
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
 
      // Create the LLM. Load from the xml.
      cout << "Creating the LLM model. " << endl;
      cout << "Loading: " << args.inputs[ i ] << endl;
      Feature_Set * feature_set = new Feature_Set();
      LLM * llm  = new LLM( feature_set );
      llm->from_xml( args.inputs[ i ] );

      //Create the symbol dictionary for groundings. Load from the xml
      cout << "Loading the symbol_dictionary_groundings" << endl;
      Symbol_Dictionary * symbol_dictionary_groundings = new Symbol_Dictionary();
      if( !load_symbol_dictionary( args.inputs[ i ], "symbol_dictionary_groundings", symbol_dictionary_groundings ) ){
        cout << "failed to load symbol_dictionary_groundings" << endl;
        assert( false );
      }

      //Create the symbol dictionary for rules. Load from the xml
      //cout << "Loading the symbol_dictionary_rules" << endl;
      //Symbol_Dictionary * symbol_dictionary_rules = new Symbol_Dictionary();
      //if( !load_symbol_dictionary( args.inputs[ i ], "symbol_dictionary_rules", symbol_dictionary_rules ) ){
        //cout << "failed to load symbol_dictionary_rules" << endl;
        //assert( false );
      //}
 
      // Write out the statistics related ot the data set partition.
      stringstream training_set_size_string;
      training_set_size_string << training_set.size();
      xmlNewProp( test_node, ( const xmlChar* )( "training_set_size" ), ( const xmlChar* )( training_set_size_string.str().c_str() ) );

      stringstream test_set_size_string;
      test_set_size_string << test_set.size();
      xmlNewProp( test_node, ( const xmlChar* )( "test_set_size" ), ( const xmlChar* )( test_set_size_string.str().c_str() ) );

      stringstream training_ratio_string;
      training_ratio_string << ( double )( training_set.size() ) / ( double )( training_set.size() + test_set.size() );
      xmlNewProp( test_node, ( const xmlChar* )( "training_ratio" ), ( const xmlChar* )( training_ratio_string.str().c_str() ) );
      
      // maintain the training runtime information
      xmlNewProp( test_node, ( const xmlChar* )( "training_runtime" ), ( const xmlChar* )( load_training_runtime( args.inputs[ i ] ).c_str() ) ); 

      /********************* Run Inference Tests******************/
      stringstream model_string;
      model_string << args.model_arg;
      // Evaluate the DCG and ADCG models on the training set (if option).
      if (args.test_training_set_arg) {
        cout << "evaluation on training set" << endl;  
       if( !model_string.str().compare( "dcg" ) ) { 
          cout << " starting dcg " << endl; 
          run_tests_dcg( training_set, llm, results_doc, test_node, "training_set", i, solution_directory.str(),
                   symbol_dictionary_groundings, args.beam_width_arg, args.debug_arg );
        } else if ( !model_string.str().compare( "adcg" ) ) {
          cout << " starting adcg " << endl; 
          run_tests_adcg( training_set, llm, results_doc, test_node, "training_set", i, solution_directory.str(),
                   symbol_dictionary_groundings, args.beam_width_arg, args.debug_arg ); 
        } else if ( !model_string.str().compare( "hdcg" ) ) {
          cout << " starting hdcg " << endl; 
          run_tests_hdcg( training_set, llm, results_doc, test_node, "training_set", i, solution_directory.str(),
                   symbol_dictionary_groundings, args.beam_width_arg, args.debug_arg );
        } else if ( !model_string.str().compare( "hadcg" ) ) {
          cout << " starting hadcg " << endl; 
          run_tests_hadcg( training_set, llm, results_doc, test_node, "training_set", i, solution_directory.str(),
                   symbol_dictionary_groundings, args.beam_width_arg, args.debug_arg );
        } else if ( !model_string.str().compare( "all" ) ) {
          cout << " starting all " << endl; 
          run_tests( training_set, llm, results_doc, test_node, "training_set", i, solution_directory.str(),
                      symbol_dictionary_groundings, args.beam_width_arg, args.debug_arg );
        } else {
          cout << "model: " << model_string.str() << "does not exist."  <<endl;
          exit(0);
        }
        cout << "training_ratio:" << training_ratio_string.str() << endl << endl;
      }

      // Evaluate the DCG and ADCG models on the test set (if option).
      if (args.test_test_set_arg) {
        cout << "evaluation on test set" << endl; 
        if( !model_string.str().compare( "dcg" ) ) { 
          cout << " starting dcg " << endl; 
          run_tests_dcg( test_set, llm, results_doc, test_node, "test_set", i, solution_directory.str(),
                   symbol_dictionary_groundings, args.beam_width_arg, args.debug_arg );
        } else if ( !model_string.str().compare( "adcg" ) ) {
          cout << " starting adcg " << endl; 
          run_tests_adcg( test_set, llm, results_doc, test_node, "test_set", i, solution_directory.str(),
                   symbol_dictionary_groundings, args.beam_width_arg, args.debug_arg ); 
        } else if ( !model_string.str().compare( "hdcg" ) ) {
          cout << " starting hdcg " << endl; 
          run_tests_hdcg( test_set, llm, results_doc, test_node, "test_set", i, solution_directory.str(),
                   symbol_dictionary_groundings, args.beam_width_arg, args.debug_arg );
        } else if ( !model_string.str().compare( "hadcg" ) ) {
          cout << " starting hadcg " << endl; 
          run_tests_hadcg( test_set, llm, results_doc, test_node, "test_set", i, solution_directory.str(),
                   symbol_dictionary_groundings, args.beam_width_arg, args.debug_arg );
        } else if ( !model_string.str().compare( "all" ) ) {
          cout << " starting all " << endl; 
          run_tests( test_set, llm, results_doc, test_node, "test_set", i, solution_directory.str(),
                      symbol_dictionary_groundings, args.beam_width_arg, args.debug_arg );
        } else {
          cout << "model: " << model_string.str() << "does not exist."  <<endl;
          exit(0);
        }
       cout << "training_ratio:" << training_ratio_string.str() << endl << endl;
      }

      // Add the child node in the xml document.
      xmlAddChild( results_node, test_node );

      /**** Memory clean up  *****/
      delete_ptr< LLM >( llm );
      delete_ptr< Feature_Set  >( feature_set );
      delete_ptr< Symbol_Dictionary  >( symbol_dictionary_groundings );
      //delete_ptr< Symbol_Dictionary  >( symbol_dictionary_rules );

      // write out the output
      // grab the test number from the filename
      string filename = args.inputs[ i ];
      string test_number_string = filename.substr( ( filename.size() - 8 ), 4 );
      // use the test number to name the output file
      stringstream results_filename;
      results_filename << args.output_arg << "/result_" << test_number_string << ".xml";
      xmlSaveFormatFileEnc( results_filename.str().c_str(), results_doc, "UTF-8", 1 );

    } else {
      cout << "could not read filename \"" << args.inputs[ i ] << "\"" << endl;
      assert( false );
    }
  }
 
  // Free the XML test_doc
  xmlFreeDoc( results_doc );

  cout << "end of test_process program" << endl;
  return 0;
}
