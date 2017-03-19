/**
 * @file test_combine_results.cc
 *
 * @brief
 *
 * Program used to combine the results of cross-validation tests that are in separate xml files given:
 *  -the output filepath and filename (args.output_arg, default = "/tmp/test_combined_results.xml")
 *  -the set of xml files containing the results to be combined (args.inputs[])
 */

#include "test_combine_results_cmdline.h"
#include <boost/filesystem/path.hpp>

#include <assert.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <libxml/tree.h>

using namespace std;

/**
 * Combine Test Results Program
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of test_combine_results program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }

  //create a new xml document for the combined results
  xmlDocPtr results_doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr results_root = xmlNewDocNode( results_doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( results_doc, results_root );

  //create a "tests" xml node
  xmlNodePtr tests_node = xmlNewDocNode( results_doc, NULL, ( xmlChar* )( "tests" ), NULL );
  unsigned int num_results = 0;
  xmlAddChild( results_root, tests_node );

  //loop through each provided results xml file
  for( int i = 0; i < args.inputs_num; i++ ){
    //read the current input xml file
    xmlDoc * input_doc = NULL;
    xmlNodePtr input_root = NULL;
    input_doc = xmlReadFile( args.inputs[ i ], NULL, 0 );

    //march down the xml tree structure to find the tests
    if( input_doc != NULL ){
      input_root = xmlDocGetRootElement( input_doc );
      if( input_root->type == XML_ELEMENT_NODE ){
        //loop through each child of the root node
        for( xmlNodePtr l1 = input_root->children; l1; l1 = l1->next ){
          if( l1->type == XML_ELEMENT_NODE ){
            //look for a node named "tests"
            if( xmlStrcmp( l1->name, ( const xmlChar* )( "tests" ) ) == 0 ){
              //loop through each child node of the "tests" xml node
              for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
                if( l2->type == XML_ELEMENT_NODE ){
                  //look for a node named "test"
                  if( xmlStrcmp( l2->name, ( const xmlChar* )( "test" ) ) == 0 ){
                    //increment the num_results counter
                    num_results++;
                    //add the test to the "tests" xml node of the combined results xml file
                    xmlAddChild( tests_node, l2 );
                  }
                }
              }
            }
          }
        }
      }
    } else {
      //assert and warn the user if an input file is not readable
      cout << "could not read filename \"" << args.inputs[ i ] << "\"" << endl;
      assert( false );
    }
  }  
  
  //add a "num" property to the "test" xml node of the combined results xml file
    //num is the number of tests
  stringstream num_results_string;
  num_results_string << num_results;
  xmlNewProp( tests_node, ( const xmlChar* )( "num" ), ( const xmlChar* )( num_results_string.str().c_str() ) );

  //save the new combined results file to the provided filepath & filename
  cout << "saving to file " << args.output_arg << endl;
  xmlSaveFormatFileEnc( args.output_arg, results_doc, "UTF-8", 1 );
 
  cout << "end of test_combine_results program" << endl;
  return 0;
} 
