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
 * Run the grounding model comparison tests.
 * Give the list of filenames.
 * Provide the trained LLM.
 **/

void
run_tests( const std::vector< std::string >& filenames,
            LLM*& llm,
            xmlDocPtr& docPtr,
            xmlNodePtr& parentNode,
            const string& testGroup,
            const unsigned int cur_test_num,
            const string& solution_directory, 
            const string& symbol_dictionary_groundings_path, 
            const unsigned int beam_width,
            const bool debug ){

  /******* Statistics **************************/
  unsigned int num_correct_root_dcg = 0;
  unsigned int num_incorrect_root_dcg = 0;
  unsigned int num_correct_root_adcg = 0;
  unsigned int num_incorrect_root_adcg = 0;
  unsigned int num_correct_complete_dcg = 0;
  unsigned int num_incorrect_complete_dcg = 0;
  unsigned int num_correct_complete_adcg = 0;
  unsigned int num_incorrect_complete_adcg = 0;

  double num_objects = 0.0;
  double num_phrases = 0.0;

  double runtime_dcg = 0.0;
  double runtime_adcg = 0.0;

  double average_runtime_ratio = 0.0;

  /******* Iterate over the test group **************************/
  xmlNodePtr test_group_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( testGroup.c_str() ), NULL );

  for( unsigned int j = 0; j < filenames.size(); j++ ){
    cout << "running " << testGroup << " " << j << " (" << filenames[ j ] << ")" << endl;

    string filename = boost::filesystem::path( filenames[ j ] ).filename().generic_string();
    xmlNodePtr example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "example" ), NULL );
    xmlNewProp( example_node, ( const xmlChar* )( "filename" ), ( const xmlChar* )( filenames[ j ].c_str() ) );

    // Phrase truth. Note no parsing.
    Phrase* truth_phrase = new Phrase();
    truth_phrase->from_xml( filenames[ j ] );
    // Phrase input.
    Phrase * input_phrase = new Phrase();
    input_phrase->from_xml( filenames[ j ] );
    clear( input_phrase );

    // World
    World * world = new World();
    world->from_xml( filenames[ j ] );

    // Symbol Dictionary and Search Space
    Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary( symbol_dictionary_groundings_path );
    Search_Space * search_space = new Search_Space();

    // Fill search space
    struct timeval start_time;
    gettimeofday( &start_time, NULL );
    search_space->fill_groundings( *symbol_dictionary, world );
    struct timeval end_time;
    gettimeofday( &end_time, NULL );
    cout << "finished fill_seach_space in " << diff_time( start_time, end_time ) << " seconds" << endl;

    // Context
    Grounding * context = NULL;

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
    stringstream dcg_example_runtime_string;
    dcg_example_runtime_string << microseconds_to_seconds( dcg_end_time - dcg_start_time );
    xmlNewProp( dcg_example_node, ( const xmlChar* )( "runtime" ), ( const xmlChar* )( dcg_example_runtime_string.str().c_str() ) );

    cout << "    runtime:" << dcg_example_runtime_string.str() << endl;

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

    xmlAddChild( example_node, dcg_example_node );
    clear( input_phrase );

    /******** ADCG **********/
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

    cout << "    runtime:" << adcg_example_runtime_string.str() << endl;

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


    /******* Comparative stats *********************/
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
    delete_ptr< Grounding >( context );
    delete_ptr< DCG >( dcg );
    delete_ptr< ADCG >( adcg );
    delete_ptr< World >( world );
    delete_ptr< Phrase >( input_phrase );
    delete_ptr< Phrase >( truth_phrase );

    // Add the node in XML.
    xmlAddChild( test_group_node, example_node );
  }

/****************************** Write the statistics to XML *************************/

  runtime_dcg /= ( double )( filenames.size() );
  runtime_adcg /= ( double )( filenames.size() );
  average_runtime_ratio /= ( double )( filenames.size() );
  num_phrases /= ( double )( filenames.size() );
  num_objects /= ( double )( filenames.size() );

  double correct_root_dcg = ( double )( num_correct_root_dcg ) / ( double )( num_correct_root_dcg + num_incorrect_root_dcg );
  double correct_root_adcg = ( double )( num_correct_root_adcg ) / ( double )( num_correct_root_adcg + num_incorrect_root_adcg );

  double correct_complete_dcg = ( double )( num_correct_complete_dcg ) / ( double )( num_correct_complete_dcg + num_incorrect_complete_dcg );
  double correct_complete_adcg = ( double )( num_correct_complete_adcg ) / ( double )( num_correct_complete_adcg + num_incorrect_complete_adcg );

  stringstream correct_root_dcg_string;
  correct_root_dcg_string << correct_root_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_dcg" ), ( const xmlChar* )( correct_root_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_dcg" ).c_str() ), ( const xmlChar* )( correct_root_dcg_string.str().c_str() ) );

  stringstream correct_root_adcg_string;
  correct_root_adcg_string << correct_root_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_adcg" ), ( const xmlChar* )( correct_root_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_adcg" ).c_str() ), ( const xmlChar* )( correct_root_adcg_string.str().c_str() ) );

  stringstream correct_root_ratio_string;
  correct_root_ratio_string << correct_root_dcg / correct_root_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_ratio" ), ( const xmlChar* )( correct_root_ratio_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_ratio" ).c_str() ), ( const xmlChar* )( correct_root_ratio_string.str().c_str() ) );

  stringstream correct_complete_dcg_string;
  correct_complete_dcg_string << correct_complete_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_dcg" ), ( const xmlChar* )( correct_complete_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_dcg" ).c_str() ), ( const xmlChar* )( correct_complete_dcg_string.str().c_str() ) );

  stringstream correct_complete_adcg_string;
  correct_complete_adcg_string << correct_complete_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_adcg" ), ( const xmlChar* )( correct_complete_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_adcg" ).c_str() ), ( const xmlChar* )( correct_complete_adcg_string.str().c_str() ) );

  stringstream correct_complete_ratio_string;
  correct_complete_ratio_string << correct_complete_dcg / correct_complete_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_ratio" ), ( const xmlChar* )( correct_complete_ratio_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_ratio" ).c_str() ), ( const xmlChar* )( correct_complete_ratio_string.str().c_str() ) );

  stringstream runtime_dcg_string;
  runtime_dcg_string << runtime_dcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_dcg" ), ( const xmlChar* )( runtime_dcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_dcg" ).c_str() ), ( const xmlChar* )( runtime_dcg_string.str().c_str() ) );

  stringstream runtime_adcg_string;
  runtime_adcg_string << runtime_adcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_adcg" ), ( const xmlChar* )( runtime_adcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_adcg" ).c_str() ), ( const xmlChar* )( runtime_adcg_string.str().c_str() ) );

  stringstream runtime_ratio_string;
  runtime_ratio_string << average_runtime_ratio;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_ratio" ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_ratio" ).c_str() ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );

  stringstream num_phrases_string;
  num_phrases_string << num_phrases;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_phrases" ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_phrases" ).c_str() ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );

  stringstream num_objects_string;
  num_objects_string << num_objects;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_objects" ), ( const xmlChar* )( num_objects_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_objects" ).c_str() ), ( const xmlChar* )( num_objects_string.str().c_str() ) );

  stringstream runtime_object_ratio_dcg_string;
  runtime_object_ratio_dcg_string << ( runtime_dcg / num_objects );
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_object_ratio_dcg" ), ( const xmlChar* )( runtime_object_ratio_dcg_string.str().c_str() ) );

  stringstream runtime_object_ratio_adcg_string;
  runtime_object_ratio_adcg_string << ( runtime_adcg / num_objects );
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_object_ratio_adcg" ), ( const xmlChar* )( runtime_object_ratio_adcg_string.str().c_str() ) );

 cout << endl;
  cout << "correct_root_dcg: " << correct_root_dcg << endl;
  cout << "correct_root_adcg: " << correct_root_adcg << endl;
  cout << "correct_root_ratio_dcg: " << correct_root_ratio_string.str() << endl;
  cout << "correct_complete_dcg: " << correct_complete_dcg << endl;
  cout << "correct_complete_adcg: " << correct_complete_adcg << endl;
  cout << "correct_complete_ratio_dcg: " << correct_complete_ratio_string.str() << endl;
  cout << "runtime_dcg: " << runtime_dcg << endl;
  cout << "runtime_adcg: " << runtime_adcg << endl;
  cout << "runtime_ratio: " << average_runtime_ratio << endl << endl;

  xmlAddChild( parentNode, test_group_node );

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

      for( unsigned int i = 0; i < training_search_spaces.size(); i++ ){
        if( training_search_spaces[ i ] != NULL ){
          delete training_search_spaces[ i ];
          training_search_spaces[ i ] = NULL;
        }
      }
      training_search_spaces.clear();

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

