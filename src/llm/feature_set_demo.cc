/**
 * @file    feature_set_demo.cc
 * @author  Thomas M. Howard (tmhoward@csail.mit.edu)
 *          Matthew R. Walter (mwalter@csail.mit.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
 *
 * Copyright (C) 2014 by the Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * @section DESCRIPTION
 *
 * A Feature_Set class demo program
 */

#include <iostream>

#include "h2sl/grammar.h"
#include "h2sl/object.h"
#include "h2sl/region.h"
#include "h2sl/cv.h"
#include "h2sl/feature_cv.h"
#include "h2sl/feature_word.h"
#include "h2sl/feature_type.h"
#include "h2sl/feature_region_object.h"
#include "h2sl/feature_type.h"
#include "h2sl/feature_set.h"
#include "h2sl/feature_abstract_container_matches_child.h"
#include "h2sl/feature_container_matches_child.h"
#include "h2sl/feature_region_container_matches_child.h"
//#include "h2sl/feature_action_object_matches_child.h"
#include "h2sl/feature_spatial_relation_matches_child.h"
#include "h2sl/feature_is_abstract_container.h"
#include "h2sl/feature_is_object.h"
#include "h2sl/feature_is_container.h"
#include "h2sl/feature_is_region.h"
#include "h2sl/feature_is_region_container.h"
#include "h2sl/feature_is_region_abstract_container.h"
#include "h2sl/feature_is_spatial_relation.h"
#include "h2sl/feature_container_number.h"
#include "h2sl/feature_container_number_equals_world_objects.h"
#include "h2sl/feature_container_is_empty.h"
#include "h2sl/feature_container_type_matches_child_container_type.h"
#include "h2sl/feature_object_property_merge_object_property_container.h"
#include "h2sl/feature_phrase_has_pos_tag.h"
#include "h2sl/feature_phrase_has_single_pos_tag.h"
#include "h2sl/feature_phrase_has_ordered_pos_tag_pair.h"
#include "h2sl/feature_object_property_merge_object_property_spatial_relation.h"
#include "h2sl/feature_container_matches_empty_child_container.h"
#include "h2sl/feature_container_merge_empty_container_container.h"
#include "h2sl/feature_container_merge_object_property_container.h"
#include "h2sl/feature_container_merge_container_spatial_relation.h"
#include "h2sl/feature_region_container_merge_container_spatial_relation.h"
#include "h2sl/feature_region_container_container_matches_child_container.h"
#include "h2sl/feature_region_container_type.h"
#include "h2sl/feature_region_container_container_type.h"
#include "h2sl/feature_min_x_object.h"
#include "h2sl/feature_max_x_object.h"
#include "h2sl/feature_max_y_object.h"
#include "h2sl/feature_min_y_object.h"
#include "h2sl/feature_max_x_abstract_container.h"
#include "h2sl/feature_min_distance_object.h"
#include "h2sl/feature_max_distance_object.h"
#include "h2sl/feature_max_distance_object_within.h"
#include "h2sl/feature_min_distance_object_within.h"
#include "h2sl/feature_min_center_distance_object.h"
#include "h2sl/feature_max_y_abstract_container.h"
#include "h2sl/feature_min_y_index_abstract_container_region_container.h"
#include "h2sl/feature_min_x_abstract_container.h"
#include "h2sl/feature_min_y_abstract_container.h"
#include "h2sl/feature_min_distance_container.h"
#include "h2sl/feature_min_distance_abstract_container.h"
#include "h2sl/feature_min_abs_y_abstract_container.h"
#include "h2sl/feature_container_dominant_x_shape.h"
#include "h2sl/feature_container_dominant_y_shape.h"
#include "h2sl/feature_container_dominant_z_shape.h"
#include "h2sl/feature_container_non_dominant_xyz_shape.h"
#include "h2sl/feature_min_x_container_abstract_container_spatial_relation.h"
#include "h2sl/feature_objects_shape_matches_container_type.h"
#include "h2sl/feature_min_y_container_abstract_container_spatial_relation.h"
#include "h2sl/feature_max_y_container_abstract_container.h"
#include "h2sl/feature_min_abs_y_container_abstract_container.h"
#include "h2sl/feature_min_x_container_region_abstract_container.h"
#include "h2sl/feature_min_x_region_abstract_container_spatial_relation.h"
#include "h2sl/feature_max_x_region_abstract_container.h"
#include "h2sl/feature_object_matches_child.h"
#include "h2sl/feature_object_matches_child_container_object.h"
#include "h2sl/feature_container_min_distance.h"
#include "h2sl/feature_container_max_distance.h"
#include "h2sl/feature_object_number_matches_world_object_number.h"
#include "h2sl/feature_container_number_equals_world_objects.h"
#include "h2sl/feature_objects_shape_matches_container_type.h"
#include "h2sl/feature_abstract_container_number_equals_world_objects.h"

#include "h2sl/feature_object_merge_abstract_container_index_region_container.h"
#include "h2sl/feature_object_merge_container_pair.h"
#include "h2sl/feature_object_merge_object_different_container.h"
#include "h2sl/feature_object_merge_object_property_region_container.h"
#include "h2sl/feature_object_merge_object_region_container.h"
#include "h2sl/feature_object_merge_single_object_different_container.h"
#include "h2sl/feature_object_merge_abstract_container_region_container.h"
#include "h2sl/feature_object_merge_abstract_container_spatial_relation.h"

#include "feature_set_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Feature_Set class demo program" << endl;
  
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  Feature_Set * feature_set = new Feature_Set();

  if( args.input_given ){
    cout << "reading feature_set from " << args.input_arg << endl;
    feature_set->from_xml( args.input_arg );
  }

  cout << "feature_set->size(): " << feature_set->size() << endl;
    
  if( args.output_given ){
    cout << "writing feature_set to " << args.output_arg << endl;
    feature_set->to_xml( args.output_arg );
  }

  if( feature_set != NULL ){
    delete feature_set;
    feature_set = NULL;
  }

  cout << "end of Feature_Set class demo program" << endl;
  return status;
}
