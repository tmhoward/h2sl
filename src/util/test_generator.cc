/**
 * @file test_generator.cc
 *
 * @brief
 *
 * Program used to generate cross-validation sets given:
 *  -the fraction resolution to be used given a minimum and maximum holdout fraction (args.fr_arg, default = 10)
 *  -the minimum and maximum fractions values (args.min_fr_arg, default = 0.2; args.max_fr_arg, default = 0.8)
 *  -the number of cross-validation sets per holdout fraction (args.num_arg, default = 13)
 *  -the output folder (args.output_arg, default = "/tmp/")
 *  -the set of examples (args.inputs[])
 */

#include "test_generator_cmdline.h"

#include <assert.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <libxml/tree.h>

using namespace std;

/**
 * Test Generator Program
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of test_generator program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }
 
  //print the number of provided example files
  cout << "args.inputs_num:" << args.inputs_num << endl;

  //set the minimum and maximum fraction values
  double min_fraction = args.min_fr_arg;
  double max_fraction = args.max_fr_arg;
 
  //counter used for output filename 
  unsigned int iteration = 0;

  //loop for each test we want per holdout fraction
  for( int i = 0; i < args.num_arg; i++ ){
    //loop through each holdout fraction
    for( unsigned int j = 0; j < args.fr_arg; j++ ){
      //determine holdout fraction
      double fraction = 0.0;
      assert( args.fr_arg > 0 );
      if( args.fr_arg != 1 ){
        fraction = min_fraction + ( max_fraction - min_fraction ) * ( double )( j ) / ( double )( args.fr_arg - 1 );
      } else {
        fraction = min_fraction; 
      }
      //set the size of the training and test datasets
      unsigned int training_set_size = floor( args.inputs_num * fraction );
      unsigned int test_set_size = args.inputs_num - training_set_size;  

      //print out the values for the holdout fraction, the training set size, and the test set size
      cout << "fraction:" << fraction << endl;
      cout << "  training_set_size:" << training_set_size << endl;
      cout << "  test_set_size:" << test_set_size << endl;

      //generate the output filename based on the number of iterations 
      stringstream filename;
      //prepend the filename with the output directory provided by args.output_arg
      filename << args.output_arg << "test_" << setw( 4 ) << setfill('0') << iteration << ".xml";
      cout << "  writing filename:\"" << filename.str() << "\"" << endl;

      //vectors for the filenames of examples in the training and test sets
      vector< string > test_set;
      vector< string > training_set;
      //initially fill the test set with all of the examples
      for( int i = 0; i < args.inputs_num; i++ ){
        test_set.push_back( args.inputs[ i ] );
      }

      //randomly fill the training set while simultaneously removing the random example from the test set
      while( training_set.size() < training_set_size ){
        unsigned int random_index = rand() % test_set.size();
        training_set.push_back( test_set[ random_index ] );
        test_set.erase( test_set.begin() + random_index );
      }

      //sort the training and test sets 
      sort( test_set.begin(), test_set.end() );
      sort( training_set.begin(), training_set.end() );

      //create an xml document to store the training and test sets
      xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
      xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
      xmlDocSetRootElement( doc, root );
      //create an xml node called "training set"
      xmlNodePtr training_set_node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "training_set" ), NULL );
      //add a "size" property to the "training_set" xml node
      stringstream training_set_size_string;
      training_set_size_string << training_set.size();
      xmlNewProp( training_set_node, ( const xmlChar* )( "size" ), ( const xmlChar* )( training_set_size_string.str().c_str() ) );
      //add a new xml node called "filename" to the "training_set" node for each example in the training set
        //add a "text" property that is the string of the filename for the example
      for( unsigned int i = 0; i < training_set.size(); i++ ){
        xmlNodePtr training_node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "filename" ), NULL );
        xmlNewProp( training_node, ( const xmlChar* )( "text" ), ( const xmlChar* )( training_set[ i ].c_str() ) );
        xmlAddChild( training_set_node, training_node );
      }
      //add the "training_set" node to the root node
      xmlAddChild( root, training_set_node );

      //create an xml node called "test_set"
      xmlNodePtr test_set_node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "test_set" ), NULL );
      //add a "size" property to the "test_set" xml node
      stringstream test_set_size_string;
      test_set_size_string << test_set.size();
      xmlNewProp( test_set_node, ( const xmlChar* )( "size" ), ( const xmlChar* )( test_set_size_string.str().c_str() ) );
      //add a new xml node called "filename" to the "test_set" node for each example in the test set
        //add a "text" property that is the string of the filename for the example
      for( unsigned int i = 0; i < test_set.size(); i++ ){
        xmlNodePtr test_node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "filename" ), NULL );
        xmlNewProp( test_node, ( const xmlChar* )( "text" ), ( const xmlChar* )( test_set[ i ].c_str() ) );
        xmlAddChild( test_set_node, test_node );
      }
      //add the "test_set" node to the root node
      xmlAddChild( root, test_set_node );
      xmlSaveFormatFileEnc( filename.str().c_str(), doc, "UTF-8", 1 );
      xmlFreeDoc( doc );
      //increment the iteration counter
      iteration++;
    }
  }  
 
  cout << "end of test_generator program" << endl;
  return 0;
} 
