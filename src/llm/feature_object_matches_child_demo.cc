/**
 * @file feature_object_matches_child_demo.cc
 *
 * @brief
 *
 * a template used to describe a feature that checks the value of a type field
 */

#include "h2sl/object.h"
#include "h2sl/region.h"
#include "h2sl/feature_object_matches_child.h"
#include "feature_object_matches_child_demo_cmdline.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Matches_Child class demo
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of Feature_Object_Matches_Child class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }  

  Feature_Object_Matches_Child< Region, Object >* feature_object_matches_child = new Feature_Object_Matches_Child< Region, Object >();

  if( feature_object_matches_child != NULL ){
    if( args.input_given ){
      feature_object_matches_child->from_xml( args.input_arg );
    }
  
    cout << "feature_object_matches_child:(" << *feature_object_matches_child << ")" << endl;

    if( args.output_given ){
      feature_object_matches_child->to_xml( args.output_arg );
    }

    delete feature_object_matches_child;
    feature_object_matches_child = NULL;
  } 

  cout << "end of Feature_Object_Matches_Child class demo program" << endl;
  return 0;
} 
