/**
 * @file    feature.cc
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
 * The implementation of a class used to describe a feature
 */

#include <assert.h>

#include "h2sl/object.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"

#include "h2sl/feature_word.h"
#include "h2sl/feature_num_words.h"
#include "h2sl/feature_cv.h"
#include "h2sl/feature_type.h"
#include "h2sl/feature_matches_child.h"
#include "h2sl/feature_object_matches_child.h"
#include "h2sl/feature_region_merge_partially_known_regions.h"
#include "h2sl/feature_region_object_property_value.h"
#include "h2sl/feature_constraint_parent_matches_child_region.h"
#include "h2sl/feature_constraint_child_matches_child_region.h"
#include "h2sl/feature_constraint_parent_is_robot.h"
#include "h2sl/feature_constraint_child_is_robot.h"
#include "h2sl/feature_grounding_property_value.h"
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
//#include "h2sl/feature_container_number_equals_world_objects.h"
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
#include "h2sl/feature_object_property_index.h"
#include "h2sl/feature_abstract_container_type.h"
#include "h2sl/feature_abstract_container_color.h"
#include "h2sl/feature_abstract_container_number.h"
#include "h2sl/feature_abstract_container_index.h"
#include "h2sl/feature_region_container_type.h"
#include "h2sl/feature_region_container_container_type.h"
#include "h2sl/feature_object_property_type.h"
#include "h2sl/feature_object_property_relation_type.h"
#include "h2sl/feature_object_property_index.h"
#include "h2sl/feature_region_abstract_container_type.h"
#include "h2sl/feature_region_abstract_container_object_type.h"
#include "h2sl/feature_region_abstract_container_number.h"
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

#include "h2sl/feature.h"

using namespace std;
using namespace h2sl;

Feature::
Feature( const bool& invert ) : _invert( invert ) {

}

Feature::
~Feature() {

}

Feature::
Feature( const Feature& other ) : _invert( other._invert ) {

}

Feature&
Feature::
operator=( const Feature& other ) {
  _invert = other._invert;
  return (*this);
}

bool 
Feature::
value( const unsigned int& cv, 
        const Grounding* grounding, 
        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, 
        const Phrase* phrase, 
        const World* world,
        const Grounding* context ){
  return value( cv, grounding, children, phrase, world );
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature& other ) {
    if( dynamic_cast< const Feature_Word* >( &other ) != NULL ){
      out << *static_cast< const Feature_Word* >( &other );
    } else if( dynamic_cast< const Feature_Num_Words* >( &other ) != NULL ){
      out << *static_cast< const Feature_Num_Words* >( &other );
    } else if( dynamic_cast< const Feature_CV* >( &other ) != NULL ){
      out << *static_cast< const Feature_CV* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Object >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Object >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Region >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Region >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Constraint >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Constraint >* >( &other );
    } else if( dynamic_cast< const Feature_Object_Matches_Child< Region, Object >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Object_Matches_Child< Region, Object >* >( &other );
    } else if( dynamic_cast< const Feature_Region_Merge_Partially_Known_Regions* >( &other ) != NULL ){
      out << *static_cast< const Feature_Region_Merge_Partially_Known_Regions* >( &other );
    } else if( dynamic_cast< const Feature_Constraint_Parent_Matches_Child_Region* >( &other ) != NULL ){
      out << *static_cast< const Feature_Constraint_Parent_Matches_Child_Region* >( &other );
    } else if( dynamic_cast< const Feature_Constraint_Child_Matches_Child_Region* >( &other ) != NULL ){
      out << *static_cast< const Feature_Constraint_Child_Matches_Child_Region* >( &other );
    } else if( dynamic_cast< const Feature_Constraint_Parent_Is_Robot* >( &other ) != NULL ){
      out << *static_cast< const Feature_Constraint_Parent_Is_Robot* >( &other );
    } else if( dynamic_cast< const Feature_Constraint_Child_Is_Robot* >( &other ) != NULL ){
      out << *static_cast< const Feature_Constraint_Child_Is_Robot* >( &other );
    } else if ( dynamic_cast< const Feature_Grounding_Property_Value* >( &other ) != NULL ){
      out << *static_cast< const Feature_Grounding_Property_Value* >( &other );
    } else if ( dynamic_cast< const Feature_Abstract_Container_Matches_Child* >( &other ) != NULL ){
        out << *static_cast< const Feature_Abstract_Container_Matches_Child* >( &other );
    } else if ( dynamic_cast< const Feature_Region_Object_Property_Value* >( &other ) != NULL ){
        out << *static_cast< const Feature_Region_Object_Property_Value* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Matches_Child* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Matches_Child* >( &other );
    } else if ( dynamic_cast< const Feature_Region_Container_Matches_Child* >( &other ) != NULL ){
        out << *static_cast< const Feature_Region_Container_Matches_Child* >( &other );
    //} else if ( dynamic_cast< const Feature_Action_Object_Matches_Child* >( &other ) != NULL ){
    //    out << *static_cast< const Feature_Action_Object_Matches_Child* >( &other );
    } else if ( dynamic_cast< const Feature_Spatial_Relation_Matches_Child* >( &other ) != NULL ){
        out << *static_cast< const Feature_Spatial_Relation_Matches_Child* >( &other );
    } else if ( dynamic_cast< const Feature_Is_Abstract_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Is_Abstract_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Is_Object* >( &other ) != NULL ){
        out << *static_cast< const Feature_Is_Object* >( &other );
    } else if ( dynamic_cast< const Feature_Is_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Is_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Is_Region* >( &other ) != NULL ){
        out << *static_cast< const Feature_Is_Region* >( &other );
    } else if ( dynamic_cast< const Feature_Is_Region_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Is_Region_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Is_Region_Abstract_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Is_Region_Abstract_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Is_Spatial_Relation* >( &other ) != NULL ){
        out << *static_cast< const Feature_Is_Spatial_Relation* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Number* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Number* >( &other );
    //} else if ( dynamic_cast< const Feature_Container_Number_Equals_World_Objects* >( &other ) != NULL ){
    //    out << *static_cast< const Feature_Container_Number_Equals_World_Objects* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Is_Empty* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Is_Empty* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Type_Matches_Child_Container_Type* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Type_Matches_Child_Container_Type* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Property_Merge_Object_Property_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Property_Merge_Object_Property_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Phrase_Has_POS_Tag* >( &other ) != NULL ){
        out << *static_cast< const Feature_Phrase_Has_POS_Tag* >( &other );
    } else if ( dynamic_cast< const Feature_Phrase_Has_Single_POS_Tag* >( &other ) != NULL ){
        out << *static_cast< const Feature_Phrase_Has_Single_POS_Tag* >( &other );
    } else if ( dynamic_cast< const Feature_Phrase_Has_Ordered_POS_Tag_Pair* >( &other ) != NULL ){
        out << *static_cast< const Feature_Phrase_Has_Ordered_POS_Tag_Pair* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Property_Merge_Object_Property_Spatial_Relation* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Property_Merge_Object_Property_Spatial_Relation* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Matches_Empty_Child_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Matches_Empty_Child_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Merge_Empty_Container_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Merge_Empty_Container_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Merge_Object_Property_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Merge_Object_Property_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Merge_Container_Spatial_Relation* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Merge_Container_Spatial_Relation* >( &other );
    } else if ( dynamic_cast< const Feature_Region_Container_Merge_Container_Spatial_Relation* >( &other ) != NULL ){
        out << *static_cast< const Feature_Region_Container_Merge_Container_Spatial_Relation* >( &other );
    //} else if ( dynamic_cast< const Feature_Region_Container_Container_Matches_Child_Container* >( &other ) != NULL ){
    //    out << *static_cast< const Feature_Region_Container_Container_Matches_Child_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Abstract_Container_Type* >( &other ) != NULL ){
        out << *static_cast< const Feature_Abstract_Container_Type* >( &other );
    } else if ( dynamic_cast< const Feature_Abstract_Container_Color* >( &other ) != NULL ){
        out << *static_cast< const Feature_Abstract_Container_Color* >( &other );
    } else if ( dynamic_cast< const Feature_Abstract_Container_Number* >( &other ) != NULL ){
        out << *static_cast< const Feature_Abstract_Container_Number* >( &other );
    } else if ( dynamic_cast< const Feature_Abstract_Container_Index* >( &other ) != NULL ){
        out << *static_cast< const Feature_Abstract_Container_Index* >( &other );
    } else if ( dynamic_cast< const Feature_Region_Container_Type* >( &other ) != NULL ){
        out << *static_cast< const Feature_Region_Container_Type* >( &other );
    } else if ( dynamic_cast< const Feature_Region_Container_Container_Type* >( &other ) != NULL ){
        out << *static_cast< const Feature_Region_Container_Container_Type* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Property_Type* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Property_Type* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Property_Relation_Type* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Property_Relation_Type* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Property_Index* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Property_Index* >( &other );
    } else if ( dynamic_cast< const Feature_Region_Abstract_Container_Type* >( &other ) != NULL ){
        out << *static_cast< const Feature_Region_Abstract_Container_Type* >( &other );
    } else if ( dynamic_cast< const Feature_Region_Abstract_Container_Object_Type* >( &other ) != NULL ){
        out << *static_cast< const Feature_Region_Abstract_Container_Object_Type* >( &other );
    } else if ( dynamic_cast< const Feature_Region_Abstract_Container_Number* >( &other ) != NULL ){
        out << *static_cast< const Feature_Region_Abstract_Container_Number* >( &other );
    } else if ( dynamic_cast< const Feature_Min_X_Object* >( &other ) != NULL ){
        out << *static_cast< const Feature_Min_X_Object* >( &other );
    } else if ( dynamic_cast< const Feature_Max_X_Object* >( &other ) != NULL ){
        out << *static_cast< const Feature_Max_X_Object* >( &other );
    } else if ( dynamic_cast< const Feature_Max_Y_Object* >( &other ) != NULL ){
        out << *static_cast< const Feature_Max_Y_Object* >( &other );
    } else if ( dynamic_cast< const Feature_Min_Y_Object* >( &other ) != NULL ){
        out << *static_cast< const Feature_Min_Y_Object* >( &other );
    } else if ( dynamic_cast< const Feature_Max_X_Abstract_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Max_X_Abstract_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Min_Distance_Object* >( &other ) != NULL ){
        out << *static_cast< const Feature_Min_Distance_Object* >( &other );
    } else if ( dynamic_cast< const Feature_Max_Distance_Object* >( &other ) != NULL ){
        out << *static_cast< const Feature_Max_Distance_Object* >( &other );
    } else if ( dynamic_cast< const Feature_Max_Distance_Object_Within* >( &other ) != NULL ){
        out << *static_cast< const Feature_Max_Distance_Object_Within* >( &other );
    } else if ( dynamic_cast< const Feature_Min_Distance_Object_Within* >( &other ) != NULL ){
        out << *static_cast< const Feature_Min_Distance_Object_Within* >( &other );
    } else if ( dynamic_cast< const Feature_Min_Center_Distance_Object* >( &other ) != NULL ){
        out << *static_cast< const Feature_Min_Center_Distance_Object* >( &other );
    } else if ( dynamic_cast< const Feature_Max_Y_Abstract_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Max_Y_Abstract_Container* >( &other );
    //} else if ( dynamic_cast< const Feature_Min_Y_Index_Abstract_Container_Region_Container* >( &other ) != NULL ){
    //    out << *static_cast< const Feature_Min_Y_Index_Abstract_Container_Region_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Min_X_Abstract_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Min_X_Abstract_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Min_Y_Abstract_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Min_Y_Abstract_Container* >( &other );
    } else {
      assert( false );
    } 
    return out;
  }
}
