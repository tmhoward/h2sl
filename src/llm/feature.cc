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
#include <typeinfo>

#include "h2sl/rule_object_type.h"
#include "h2sl/rule_object_color.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/rule_constraint_type.h"
#include "h2sl/rule_constraint_payload_type.h"
#include "h2sl/rule_constraint_reference_type.h"
#include "h2sl/rule_index.h"
#include "h2sl/rule_number.h"
#include "h2sl/object.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/container.h"
#include "h2sl/object_type.h"
#include "h2sl/object_property.h"
#include "h2sl/object_color.h"
#include "h2sl/index.h"
#include "h2sl/number.h"
#include "h2sl/abstract_container.h"

#include "h2sl/feature_word.h"
#include "h2sl/feature_phrase_type_and_word.h"
#include "h2sl/feature_num_words.h"
#include "h2sl/feature_phrase_type_and_num_words.h"
#include "h2sl/feature_num_child_phrases.h"
#include "h2sl/feature_phrase_type_and_num_child_phrases.h"
#include "h2sl/feature_cv.h"
#include "h2sl/feature_type.h"
#include "h2sl/feature_matches_child.h"
#include "h2sl/feature_matches_only_child.h"
#include "h2sl/feature_merge_children.h"
#include "h2sl/feature_object_matches_child.h"
#include "h2sl/feature_region_object_matches_child.h"
#include "h2sl/feature_region_merge_partially_known_regions.h"
#include "h2sl/feature_region_merge_spatial_relation_and_object.h"
#include "h2sl/feature_region_merge_spatial_relation_and_region.h"
#include "h2sl/feature_region_object_string_property_value.h"
#include "h2sl/feature_constraint_parent_matches_child_region.h"
#include "h2sl/feature_constraint_child_matches_child_region.h"
#include "h2sl/feature_constraint_payload_type.h"
#include "h2sl/feature_constraint_reference_type.h"
#include "h2sl/feature_constraint_object_relationship.h"
#include "h2sl/feature_grounding_string_property_value.h"
#include "h2sl/feature_grounding_int_property_value.h"
#include "h2sl/feature_grounding_string_property_value_matches_child_string_property_value.h"
#include "h2sl/feature_grounding_string_property_value_missing_child_string_property.h"
#include "h2sl/feature_grounding_string_property_value_matches_child_string_properties.h"
#include "h2sl/feature_grounding_class_name.h"
#include "h2sl/feature_abstract_container_matches_child.h"
#include "h2sl/feature_container_matches_child.h"
#include "h2sl/feature_region_container_matches_child.h"
#include "h2sl/feature_object_matches_sorted_objects.h"
#include "h2sl/feature_container_matches_sorted_objects.h"
#include "h2sl/feature_object_merge_abstract_container_spatial_relation.h"
#include "h2sl/feature_object_merge_region_abstract_container_container.h"
#include "h2sl/feature_object_merge_object_property_container.h"
#include "h2sl/feature_object_merge_object_property_spatial_relation.h"
#include "h2sl/feature_object_merge_object_type_spatial_relation.h"
#include "h2sl/feature_object_merge_object_type_region.h"
#include "h2sl/feature_object_merge_object_type_region_container.h"
#include "h2sl/feature_container_object_type.h"
#include "h2sl/feature_container_object_color.h"
#include "h2sl/feature_container_number.h"
#include "h2sl/feature_container_merge_container_type_container.h"
#include "h2sl/feature_container_merge_container_region.h"
#include "h2sl/feature_container_merge_abstract_container_spatial_relation.h"
#include "h2sl/feature_container_merge_region_abstract_container_container.h"
//#include "h2sl/feature_action_object_matches_child.h"
//#include "h2sl/feature_spatial_relation_matches_child.h"
//#include "h2sl/feature_is_abstract_container.h"
//#include "h2sl/feature_is_object.h"
//#include "h2sl/feature_is_container.h"
//#include "h2sl/feature_is_region.h"
//#include "h2sl/feature_is_region_container.h"
//#include "h2sl/feature_is_region_abstract_container.h"
//#include "h2sl/feature_is_spatial_relation.h"
//#include "h2sl/feature_container_number.h"
//#include "h2sl/feature_container_number_equals_world_objects.h"
#include "h2sl/feature_container_is_empty.h"
//#include "h2sl/feature_container_type_matches_child_container_type.h"
//#include "h2sl/feature_object_property_merge_object_property_container.h"
#include "h2sl/feature_object_property_merge_index_object_property.h"
#include "h2sl/feature_phrase_has_pos_tag.h"
#include "h2sl/feature_phrase_type_and_phrase_has_pos_tag.h"
//#include "h2sl/feature_phrase_has_single_pos_tag.h"
//#include "h2sl/feature_phrase_has_ordered_pos_tag_pair.h"
//#include "h2sl/feature_object_property_merge_object_property_spatial_relation.h"
//#include "h2sl/feature_container_matches_empty_child_container.h"
//#include "h2sl/feature_container_merge_empty_container_container.h"
//#include "h2sl/feature_container_merge_object_property_container.h"
//#include "h2sl/feature_container_merge_container_spatial_relation.h"
//#include "h2sl/feature_region_container_merge_container_spatial_relation.h"
//#include "h2sl/feature_region_container_container_matches_child_container.h"
//#include "h2sl/feature_object_property_index.h"
//#include "h2sl/feature_region_container_type.h"
//#include "h2sl/feature_region_container_container_type.h"
//#include "h2sl/feature_object_property_type.h"
//#include "h2sl/feature_object_property_relation_type.h"
//#include "h2sl/feature_object_property_index.h"
//#include "h2sl/feature_min_x_object.h"
//#include "h2sl/feature_max_x_object.h"
//#include "h2sl/feature_max_y_object.h"
//#include "h2sl/feature_min_y_object.h"
//#include "h2sl/feature_max_x_abstract_container.h"
//#include "h2sl/feature_min_distance_object.h"
//#include "h2sl/feature_max_distance_object.h"
//#include "h2sl/feature_max_distance_object_within.h"
//#include "h2sl/feature_min_distance_object_within.h"
//#include "h2sl/feature_min_center_distance_object.h"
//#include "h2sl/feature_max_y_abstract_container.h"
//#include "h2sl/feature_min_y_index_abstract_container_region_container.h"
//#include "h2sl/feature_min_x_abstract_container.h"
//#include "h2sl/feature_min_y_abstract_container.h"
//#include "h2sl/feature_min_distance_container.h"
//#include "h2sl/feature_min_distance_abstract_container.h"
//#include "h2sl/feature_min_abs_y_abstract_container.h"
//#include "h2sl/feature_container_dominant_x_shape.h"
//#include "h2sl/feature_container_dominant_y_shape.h"
//#include "h2sl/feature_container_dominant_z_shape.h"
//#include "h2sl/feature_container_non_dominant_xyz_shape.h"
//#include "h2sl/feature_min_x_container_abstract_container_spatial_relation.h"
//#include "h2sl/feature_objects_shape_matches_container_type.h"
//#include "h2sl/feature_min_y_container_abstract_container_spatial_relation.h"
//#include "h2sl/feature_max_y_container_abstract_container.h"
//#include "h2sl/feature_min_abs_y_container_abstract_container.h"
//#include "h2sl/feature_min_x_container_region_abstract_container.h"
//#include "h2sl/feature_min_x_region_abstract_container_spatial_relation.h"
//#include "h2sl/feature_max_x_region_abstract_container.h"
//#include "h2sl/feature_object_matches_child.h"
#include "h2sl/feature_object_matches_child_container_object.h"
#include "h2sl/feature_object_matches_child_region_container_object.h"
//#include "h2sl/feature_container_min_distance.h"
//#include "h2sl/feature_container_max_distance.h"
//#include "h2sl/feature_object_number_matches_world_object_number.h"
//#include "h2sl/feature_container_number_equals_world_objects.h"
//#include "h2sl/feature_abstract_container_number_equals_world_objects.h"
//#include "h2sl/feature_object_merge_object_property_spatial_relation.h"

//#include "h2sl/feature_object_merge_object_container.h"

//#include "h2sl/feature_object_merge_abstract_container_index_region_container.h"
//#include "h2sl/feature_object_merge_container_pair.h"
//#include "h2sl/feature_object_merge_object_different_container.h"
//#include "h2sl/feature_object_merge_object_property_region_container.h"
//#include "h2sl/feature_object_merge_object_region_container.h"
//#include "h2sl/feature_object_merge_single_object_different_container.h"
#include "h2sl/feature_object_merge_abstract_container_region_container.h"
//#include "h2sl/feature_object_merge_abstract_container_spatial_relation.h"

#include "h2sl/feature_rule_container_type_equals_world_objects.h"

#include "h2sl/feature_is_in_a_container.h"
#include "h2sl/feature_object_shape_abstract_container.h"
#include "h2sl/feature_object_merge_container_pair.h"
#include "h2sl/feature.h"

using namespace std;
using namespace h2sl;

Feature::
Feature( const bool& invert,
          const map< string, string >& stringProperties,
          const map< string, int >& intProperties ) : _invert( invert ),
                                                      _string_properties( stringProperties ),
                                                      _int_properties( intProperties ) {

}

Feature::
~Feature() {

}

Feature::
Feature( const Feature& other ) : _invert( other._invert ),
                                  _string_properties( other._string_properties ),
                                  _int_properties( other._int_properties ) {

}

Feature&
Feature::
operator=( const Feature& other ) {
  _invert = other._invert;
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature& other ) {
    if( dynamic_cast< const Feature_Word* >( &other ) != NULL ){
      out << *static_cast< const Feature_Word* >( &other );
    } else if( dynamic_cast< const Feature_Phrase_Type_And_Word* >( &other ) != NULL ){
      out << *static_cast< const Feature_Phrase_Type_And_Word* >( &other );
    } else if( dynamic_cast< const Feature_Num_Words* >( &other ) != NULL ){
      out << *static_cast< const Feature_Num_Words* >( &other );
    } else if( dynamic_cast< const Feature_Phrase_Type_And_Num_Words* >( &other ) != NULL ){
      out << *static_cast< const Feature_Phrase_Type_And_Num_Words* >( &other );
    } else if( dynamic_cast< const Feature_Num_Child_Phrases* >( &other ) != NULL ){
      out << *static_cast< const Feature_Num_Child_Phrases* >( &other );
    } else if( dynamic_cast< const Feature_Phrase_Type_And_Num_Child_Phrases* >( &other ) != NULL ){
      out << *static_cast< const Feature_Phrase_Type_And_Num_Child_Phrases* >( &other );
    } else if( dynamic_cast< const Feature_CV* >( &other ) != NULL ){
      out << *static_cast< const Feature_CV* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Rule_Object_Type >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Rule_Object_Type >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Rule_Object_Color >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Rule_Object_Color >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Rule_Spatial_Relation >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Rule_Spatial_Relation >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Rule_Constraint_Type >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Rule_Constraint_Type >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Rule_Constraint_Payload_Type >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Rule_Constraint_Payload_Type >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Rule_Constraint_Reference_Type >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Rule_Constraint_Reference_Type >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Rule_Index >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Rule_Index >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Rule_Number >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Rule_Number >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Object >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Object >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Region >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Region >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Constraint >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Constraint >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Object_Type >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Object_Type >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Object_Color >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Object_Color >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Object_Property >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Object_Property >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Number >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Number >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Index >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Index >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Spatial_Relation >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Spatial_Relation >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Abstract_Container >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Abstract_Container >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Child< Container >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Child< Container >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Only_Child< Object >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Only_Child< Object >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Only_Child< Region >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Only_Child< Region >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Only_Child< Constraint >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Only_Child< Constraint >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Only_Child< Object_Type >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Only_Child< Object_Type >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Only_Child< Object_Color >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Only_Child< Object_Color >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Only_Child< Object_Property >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Only_Child< Object_Property >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Only_Child< Number >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Only_Child< Number >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Only_Child< Index >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Only_Child< Index >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Only_Child< Spatial_Relation >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Only_Child< Spatial_Relation >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Only_Child< Abstract_Container >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Only_Child< Abstract_Container >* >( &other );
    } else if( dynamic_cast< const Feature_Matches_Only_Child< Container >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Matches_Only_Child< Container >* >( &other );
    } else if( dynamic_cast< const Feature_Object_Matches_Child< Region, Object >* >( &other ) != NULL ){
      out << *static_cast< const Feature_Object_Matches_Child< Region, Object >* >( &other );
    } else if( dynamic_cast< const Feature_Region_Object_Matches_Child* >( &other ) != NULL ){
      out << *static_cast< const Feature_Region_Object_Matches_Child* >( &other );
    } else if( dynamic_cast< const Feature_Region_Merge_Partially_Known_Regions* >( &other ) != NULL ){
      out << *static_cast< const Feature_Region_Merge_Partially_Known_Regions* >( &other );
    } else if( dynamic_cast< const Feature_Region_Merge_Spatial_Relation_And_Object* >( &other ) != NULL ){
      out << *static_cast< const Feature_Region_Merge_Spatial_Relation_And_Object* >( &other );
    } else if( dynamic_cast< const Feature_Region_Merge_Spatial_Relation_And_Region* >( &other ) != NULL ){
      out << *static_cast< const Feature_Region_Merge_Spatial_Relation_And_Region* >( &other );
    } else if( dynamic_cast< const Feature_Constraint_Parent_Matches_Child_Region* >( &other ) != NULL ){
      out << *static_cast< const Feature_Constraint_Parent_Matches_Child_Region* >( &other );
    } else if( dynamic_cast< const Feature_Constraint_Child_Matches_Child_Region* >( &other ) != NULL ){
      out << *static_cast< const Feature_Constraint_Child_Matches_Child_Region* >( &other );
    } else if( dynamic_cast< const Feature_Constraint_Payload_Type* >( &other ) != NULL ){
      out << *static_cast< const Feature_Constraint_Payload_Type* >( &other );
    } else if( dynamic_cast< const Feature_Constraint_Reference_Type* >( &other ) != NULL ){
      out << *static_cast< const Feature_Constraint_Reference_Type* >( &other );
    } else if( dynamic_cast< const Feature_Constraint_Object_Relationship* >( &other ) != NULL ){
      out << *static_cast< const Feature_Constraint_Object_Relationship* >( &other );
    } else if ( dynamic_cast< const Feature_Grounding_String_Property_Value* >( &other ) != NULL ){
      out << *static_cast< const Feature_Grounding_String_Property_Value* >( &other );
    } else if ( dynamic_cast< const Feature_Grounding_Int_Property_Value* >( &other ) != NULL ){
      out << *static_cast< const Feature_Grounding_Int_Property_Value* >( &other );
    } else if ( dynamic_cast< const Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value* >( &other ) != NULL ){
      out << *static_cast< const Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value* >( &other );
    } else if ( dynamic_cast< const Feature_Grounding_String_Property_Value_Missing_Child_String_Property* >( &other ) != NULL ){
      out << *static_cast< const Feature_Grounding_String_Property_Value_Missing_Child_String_Property* >( &other );
    } else if ( dynamic_cast< const Feature_Grounding_String_Property_Value_Matches_Child_String_Properties* >( &other ) != NULL ){
      out << *static_cast< const Feature_Grounding_String_Property_Value_Matches_Child_String_Properties* >( &other );
    } else if ( dynamic_cast< const Feature_Grounding_Class_Name* >( &other ) != NULL ){
      out << *static_cast< const Feature_Grounding_Class_Name* >( &other );
    } else if ( dynamic_cast< const Feature_Abstract_Container_Matches_Child* >( &other ) != NULL ){
        out << *static_cast< const Feature_Abstract_Container_Matches_Child* >( &other );
    } else if ( dynamic_cast< const Feature_Region_Object_String_Property_Value* >( &other ) != NULL ){
        out << *static_cast< const Feature_Region_Object_String_Property_Value* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Matches_Child* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Matches_Child* >( &other );
    } else if ( dynamic_cast< const Feature_Region_Container_Matches_Child* >( &other ) != NULL ){
        out << *static_cast< const Feature_Region_Container_Matches_Child* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Matches_Sorted_Objects* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Matches_Sorted_Objects* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Matches_Sorted_Objects* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Matches_Sorted_Objects* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Merge_Abstract_Container_Spatial_Relation* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Merge_Abstract_Container_Spatial_Relation* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Merge_Abstract_Container_Region_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Merge_Abstract_Container_Region_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Merge_Region_Abstract_Container_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Merge_Region_Abstract_Container_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Merge_Object_Property_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Merge_Object_Property_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Merge_Object_Property_Spatial_Relation* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Merge_Object_Property_Spatial_Relation* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Merge_Object_Type_Spatial_Relation* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Merge_Object_Type_Spatial_Relation* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Merge_Object_Type_Region* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Merge_Object_Type_Region* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Merge_Object_Type_Region_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Merge_Object_Type_Region_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Object_Type* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Object_Type* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Object_Color* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Object_Color* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Number* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Number* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Merge_Container_Type_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Merge_Container_Type_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Merge_Container_Region* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Merge_Container_Region* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Merge_Abstract_Container_Spatial_Relation* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Merge_Abstract_Container_Spatial_Relation* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Merge_Region_Abstract_Container_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Merge_Region_Abstract_Container_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Container_Is_Empty* >( &other ) != NULL ){
        out << *static_cast< const Feature_Container_Is_Empty* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Property_Merge_Index_Object_Property* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Property_Merge_Index_Object_Property* >( &other );
    } else if ( dynamic_cast< const Feature_Phrase_Has_POS_Tag* >( &other ) != NULL ){
        out << *static_cast< const Feature_Phrase_Has_POS_Tag* >( &other );
    } else if ( dynamic_cast< const Feature_Phrase_Type_And_Phrase_Has_POS_Tag* >( &other ) != NULL ){
        out << *static_cast< const Feature_Phrase_Type_And_Phrase_Has_POS_Tag* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Matches_Child_Container_Object* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Matches_Child_Container_Object* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Matches_Child_Region_Container_Object* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Matches_Child_Region_Container_Object* >( &other );
    } else if ( dynamic_cast< const Feature_Is_In_A_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Is_In_A_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Shape_Abstract_Container* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Shape_Abstract_Container* >( &other );
    } else if ( dynamic_cast< const Feature_Object_Merge_Container_Pair* >( &other ) != NULL ){
        out << *static_cast< const Feature_Object_Merge_Container_Pair* >( &other );
    } else if ( dynamic_cast< const Feature_Rule_Container_Type_Equals_World_Objects* >( &other ) != NULL ){
        out << *static_cast< const Feature_Rule_Container_Type_Equals_World_Objects* >( &other );
    } else if ( dynamic_cast< const Feature_Merge_Children< Object, Container >* >( &other ) != NULL ){
        out << *static_cast< const Feature_Merge_Children< Object, Container >* >( &other );
    } else {
      cout << "could not load \"" << typeid( other ).name() << "\"" << endl;
    } 
    return out;
  }
}
