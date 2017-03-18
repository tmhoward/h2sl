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

  cout << "end of test_process program" << endl;
  return 0;
}

