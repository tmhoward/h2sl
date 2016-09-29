/**
 * @file feature_type_demo.cc
 *
 * @brief
 *
 * a template used to describe a feature that checks the value of a type field
 */

#include "h2sl/constraint.h"
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

  cout << "end of Feature_Type class demo program" << endl;
  return 0;
} 
