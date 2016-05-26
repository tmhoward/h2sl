/**
 * @file feature_type_demo.cc
 *
 * @brief
 *
 * a template used to describe a feature that checks the value of a type field
 */

#include "h2sl/object.h"
#include "h2sl/feature_type.h"
#include "feature_type_demo_cmdline.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Type class demo
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of Feature_Type class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }  

  Feature_Type< Object >* feature_type = new Feature_Type< Object >();

  if( feature_type != NULL ){
    if( args.input_given ){
      feature_type->from_xml( args.input_arg );
    }
  
    cout << "feature_type:(" << *feature_type << ")" << endl;

    if( args.output_given ){
      feature_type->to_xml( args.output_arg );
    }

    delete feature_type;
    feature_type = NULL;
  } 

  cout << "end of Feature_Type class demo program" << endl;
  return 0;
} 
