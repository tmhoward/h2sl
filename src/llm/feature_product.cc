/**
 * @file    feature_product.cc
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
 * The implementation of a class used to describe a product of features
 */

#include <assert.h>

#include "h2sl/rule_object_type.h"
#include "h2sl/rule_container_type.h"
#include "h2sl/rule_object_color.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/rule_constraint_type.h"
#include "h2sl/rule_constraint_payload_type.h"
#include "h2sl/rule_constraint_reference_type.h"
#include "h2sl/rule_index.h"
#include "h2sl/rule_number.h"
#include "h2sl/object.h"
#include "h2sl/object_property.h"
#include "h2sl/object_type.h"
#include "h2sl/container_type.h"
#include "h2sl/object_color.h"
#include "h2sl/index.h"
#include "h2sl/number.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/container.h"
#include "h2sl/region_container.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_abstract_container.h"
#include "h2sl/spatial_relation.h"

#include "h2sl/feature_cv.h"
#include "h2sl/feature_word.h"
#include "h2sl/feature_phrase_type_and_word.h"
#include "h2sl/feature_num_words.h"
#include "h2sl/feature_phrase_type_and_num_words.h"
#include "h2sl/feature_num_child_phrases.h"
#include "h2sl/feature_phrase_type_and_num_child_phrases.h"
#include "h2sl/feature_phrase_has_pos_tag.h"
#include "h2sl/feature_phrase_type_and_phrase_has_pos_tag.h"
#include "h2sl/feature_phrase_has_single_pos_tag.h"
#include "h2sl/feature_phrase_has_ordered_pos_tag_pair.h"

#include "h2sl/feature_merge_children.h"

#include "h2sl/feature_is_in_a_container.h"
#include "h2sl/feature_object_shape_abstract_container.h"
#include "h2sl/feature_container_shape_abstract_container.h"

#include "h2sl/feature_abstract_container_matches_child.h"

#include "h2sl/feature_constraint_payload_string_property_value.h"
#include "h2sl/feature_constraint_payload_string_property_value.h"
#include "h2sl/feature_constraint_reference_matches_child.h"
#include "h2sl/feature_constraint_payload_type.h"
#include "h2sl/feature_constraint_reference_type.h"
#include "h2sl/feature_constraint_object_relationship.h"

#include "h2sl/feature_container_dominant_x_shape.h"
#include "h2sl/feature_container_dominant_y_shape.h"
#include "h2sl/feature_container_dominant_z_shape.h"
#include "h2sl/feature_container_non_dominant_xyz_shape.h"
#include "h2sl/feature_container_is_empty.h"
#include "h2sl/feature_container_matches_child.h"
#include "h2sl/feature_container_matches_empty_child_container.h"
#include "h2sl/feature_container_merge_empty_container_container.h"
#include "h2sl/feature_container_merge_object_property_container.h"
#include "h2sl/feature_container_merge_container_region.h"
#include "h2sl/feature_container_min_distance.h"
#include "h2sl/feature_container_max_distance.h"
#include "h2sl/feature_container_number.h"
#include "h2sl/feature_container_object_type.h"
#include "h2sl/feature_container_object_color.h"
#include "h2sl/feature_container_number.h"
#include "h2sl/feature_container_number_equals_world_objects.h"
#include "h2sl/feature_container_type_matches_child_container_type.h"
#include "h2sl/feature_container_container_type_matches_child_region_container_container_container_type.h"

#include "h2sl/feature_is_abstract_container.h"
#include "h2sl/feature_is_object.h"
#include "h2sl/feature_is_container.h"
#include "h2sl/feature_is_region.h"
#include "h2sl/feature_is_region_container.h"
#include "h2sl/feature_is_region_abstract_container.h"
#include "h2sl/feature_is_spatial_relation.h"

#include "h2sl/feature_max_distance_object.h"
#include "h2sl/feature_max_distance_object_within.h"
#include "h2sl/feature_max_x_abstract_container.h"
#include "h2sl/feature_max_x_object.h"
#include "h2sl/feature_max_x_region_abstract_container.h"

#include "h2sl/feature_max_y_abstract_container.h"
#include "h2sl/feature_max_y_container_abstract_container.h"
#include "h2sl/feature_max_y_object.h"
#include "h2sl/feature_min_abs_y_abstract_container.h"
#include "h2sl/feature_min_center_distance_object.h"
#include "h2sl/feature_min_distance_abstract_container.h"
#include "h2sl/feature_min_distance_container.h"
#include "h2sl/feature_min_distance_object.h"
#include "h2sl/feature_min_distance_object_within.h"
#include "h2sl/feature_min_x_abstract_container.h"
#include "h2sl/feature_min_x_container_abstract_container_spatial_relation.h"
#include "h2sl/feature_min_x_container_region_abstract_container.h"
#include "h2sl/feature_min_x_object.h"
#include "h2sl/feature_min_x_region_abstract_container_spatial_relation.h"
#include "h2sl/feature_min_y_abstract_container.h"
#include "h2sl/feature_min_abs_y_container_abstract_container.h"
#include "h2sl/feature_min_y_index_abstract_container_region_container.h"
#include "h2sl/feature_min_y_object.h"
#include "h2sl/feature_object_matches_sorted_objects.h"
#include "h2sl/feature_container_matches_sorted_objects.h"
#include "h2sl/feature_object_matches_sorted_object.h"
#include "h2sl/feature_object_matches_child_container_object.h"
#include "h2sl/feature_object_matches_child_region_container_object.h"
#include "h2sl/feature_object_number_matches_world_object_number.h"
#include "h2sl/feature_object_property_merge_object_property_container.h"
#include "h2sl/feature_object_property_merge_object_property_container.h"
#include "h2sl/feature_object_property_merge_index_object_property.h"
#include "h2sl/feature_objects_shape_matches_container_type.h"
#include "h2sl/feature_region_container_container_matches_child_container.h"
#include "h2sl/feature_region_container_matches_child.h"

#include "h2sl/feature_grounding_string_property_value.h"
#include "h2sl/feature_grounding_int_property_value.h"
#include "h2sl/feature_grounding_string_property_value_matches_child_string_property_value.h"
#include "h2sl/feature_grounding_string_property_value_missing_child_string_property.h"
#include "h2sl/feature_grounding_string_property_value_matches_child_string_properties.h"
#include "h2sl/feature_grounding_class_name.h"
#include "h2sl/feature_region_object_string_property_value.h"
#include "h2sl/feature_matches_child.h"
#include "h2sl/feature_matches_only_child.h"
#include "h2sl/feature_matches_ordered_child.h"
#include "h2sl/feature_object_matches_child.h"
#include "h2sl/feature_region_object_matches_child.h"
#include "h2sl/feature_region_merge_spatial_relation_and_object.h"
#include "h2sl/feature_region_merge_spatial_relation_and_region.h"

#include "h2sl/feature_region_merge_partially_known_regions.h"
#include "h2sl/feature_constraint_parent_matches_child_region.h"
#include "h2sl/feature_constraint_child_matches_child_region.h"
#include "h2sl/feature_constraint_child_matches_child_object.h"
#include "h2sl/feature_container_type_merge_container_type_number.h"
#include "h2sl/feature_number_merge_container_type_number.h"

#include "h2sl/feature_region_container_merge_container_spatial_relation.h"
/*
#include "h2sl/feature_region_container_type.h"
#include "h2sl/feature_region_merge_partially_known_regions.h"
#include "h2sl/feature_region_object_string_property_value.h"
#include "h2sl/feature_region_object_matches_child.h"
#include "h2sl/feature_spatial_relation_matches_child.h"
#include "h2sl/feature_grounding_string_property_value.h"
#include "h2sl/feature_grounding_int_property_value.h"
#include "h2sl/feature_matches_child.h"
*/
#include "h2sl/feature_object_merge_object_property_spatial_relation.h"
/*
#include "h2sl/feature_object_merge_object_property_index_spatial_relation.h"
#include "h2sl/feature_object_merge_object_container.h"

#include "h2sl/feature_type.h"
#include "h2sl/feature_object_matches_child.h"
#include "h2sl/feature_constraint_parent_matches_child_region.h"
#include "h2sl/feature_constraint_child_matches_child_region.h"
#include "h2sl/feature_constraint_child_matches_child_object.h"
#include "h2sl/feature_container_merge_container_spatial_relation.h"
#include "h2sl/feature_region_container_container_type.h"
#include "h2sl/feature_min_y_container_abstract_container_spatial_relation.h"
#include "h2sl/feature_object_matches_child.h"
#include "h2sl/feature_container_number_equals_world_objects.h"
*/
#include "h2sl/feature_abstract_container_number_equals_world_objects.h"
/*
#include "h2sl/feature_object_merge_abstract_container_index_region_container.h"
*/
#include "h2sl/feature_object_merge_container_pair.h"
#include "h2sl/feature_container_merge_container_pair.h"
/*
#include "h2sl/feature_object_merge_object_different_container.h"
*/
#include "h2sl/feature_object_merge_object_property_region_container.h"
#include "h2sl/feature_object_merge_object_type_region_container.h"
#include "h2sl/feature_object_merge_object_region_container.h"
/*
#include "h2sl/feature_object_merge_single_object_different_container.h"
*/
#include "h2sl/feature_object_merge_abstract_container_region_container.h"
#include "h2sl/feature_object_merge_abstract_container_spatial_relation.h"
#include "h2sl/feature_object_merge_object_property_container.h"
#include "h2sl/feature_object_merge_region_abstract_container_container.h"
#include "h2sl/feature_object_merge_object_type_spatial_relation.h"
#include "h2sl/feature_object_merge_object_type_region.h"
#include "h2sl/feature_container_merge_abstract_container_spatial_relation.h"
#include "h2sl/feature_container_merge_abstract_container_container_type.h"
#include "h2sl/feature_container_merge_abstract_container_region.h"
#include "h2sl/feature_container_merge_abstract_container_region_container.h"
#include "h2sl/feature_container_merge_region_abstract_container_container.h"
#include "h2sl/feature_container_merge_container_type_container.h"

#include "h2sl/feature_rule_object_type_merge_rule_object_types.h"
#include "h2sl/feature_rule_object_type_merge_rule_container_type_rule_object_type.h"
#include "h2sl/feature_rule_object_color_merge_rule_object_type_rule_spatial_relation.h"
#include "h2sl/feature_rule_spatial_relation_merge_rule_spatial_relations.h"
#include "h2sl/feature_rule_spatial_relation_merge_rule_spatial_relation_rule_container_type.h"
#include "h2sl/feature_rule_container_type_merge_rule_container_types.h"
#include "h2sl/feature_rule_container_type_merge_rule_object_types.h"
#include "h2sl/feature_rule_container_type_equals_world_objects.h"
#include "h2sl/feature_rule_index_merge_rule_container_type_rule_index.h"

#include "h2sl/feature_product.h"

using namespace std;
using namespace h2sl;

Feature_Product::
Feature_Product() : _feature_groups(),
                _values() {

}

Feature_Product::
~Feature_Product() {

}

Feature_Product::
Feature_Product( const Feature_Product& other ) : _feature_groups( other._feature_groups ),
                                          _values( other._values ) {

}

Feature_Product&
Feature_Product::
operator=( const Feature_Product& other ) {
  _feature_groups = other._feature_groups;
  _values = other._values;
  return (*this);
}

void 
Feature_Product::
indices( const string& cv, 
          const Grounding* grounding,
          const vector< pair< const Phrase*, vector< Grounding* > > >& children, 
          const Phrase* phrase,
          const World* world,
          const Grounding* context,
          vector< unsigned int >& indices,
          vector< Feature* >& features,
          const vector< bool >& evaluateFeatureTypes ){
  indices.clear();
  evaluate( cv, grounding, children, phrase, world, context, evaluateFeatureTypes );

  std::vector< std::vector< unsigned int > > group_indices( _values.size() );
  for( unsigned int i = 0; i < _values.size(); i++ ){
    for( unsigned int j = 0; j < _values[ i ].size(); j++ ){
      if( _values[ i ][ j ] ){
        group_indices[ i ].push_back( j );
        features.push_back( _feature_groups[ i ][ j ] );
      }
    }
  }

  if( _values.size() == 3 ){
    for( unsigned int i = 0; i < group_indices[ 0 ].size(); i++ ){
      for( unsigned int j = 0; j < group_indices[ 1 ].size(); j++ ){
        for( unsigned int k = 0; k < group_indices[ 2 ].size(); k++ ){
          indices.push_back( group_indices[ 0 ][ i ] * _feature_groups[ 1 ].size() * _feature_groups[ 2 ].size() + group_indices[ 1 ][ j ] * _feature_groups[ 2 ].size() + group_indices[ 2 ][ k ] );
        }
      }
    }
  }
  return;
}

void
Feature_Product::
indices( const string& cv,
          const Grounding* grounding,
          const vector< pair< const Phrase*, vector< Grounding* > > >& children,
          const Phrase* phrase,
          const World* world,
          const Grounding* context,
          vector< unsigned int >& indices,
          vector< pair< vector< Feature* >, unsigned int > >& weightedFeatures,
          const vector< bool >& evaluateFeatureTypes ){
  indices.clear();
  evaluate( cv, grounding, children, phrase, world, context, evaluateFeatureTypes );

  std::vector< std::vector< unsigned int > > group_indices( _values.size() );
  for( unsigned int i = 0; i < _values.size(); i++ ){
    for( unsigned int j = 0; j < _values[ i ].size(); j++ ){
      if( _values[ i ][ j ] ){
        group_indices[ i ].push_back( j );
      }
    }
  }

  if( _values.size() == 3 ){
    for( unsigned int i = 0; i < group_indices[ 0 ].size(); i++ ){
      for( unsigned int j = 0; j < group_indices[ 1 ].size(); j++ ){
        for( unsigned int k = 0; k < group_indices[ 2 ].size(); k++ ){
          indices.push_back( group_indices[ 0 ][ i ] * _feature_groups[ 1 ].size() * _feature_groups[ 2 ].size() + group_indices[ 1 ][ j ] * _feature_groups[ 2 ].size() + group_indices[ 2 ][ k ] );
          weightedFeatures.push_back( pair< vector< Feature* >, unsigned int >( vector< Feature* >(), indices.back() ) );
          weightedFeatures.back().first.push_back( _feature_groups[ 0 ][ group_indices[ 0 ][ i ] ] );
          weightedFeatures.back().first.push_back( _feature_groups[ 1 ][ group_indices[ 1 ][ j ] ] );
          weightedFeatures.back().first.push_back( _feature_groups[ 2 ][ group_indices[ 2 ][ k ] ] );
        }
      }
    }
  }

  return;
}

void
Feature_Product::
evaluate( const string& cv,
          const Grounding* grounding, 
          const vector< pair< const Phrase*, vector< Grounding* > > >& children,
          const Phrase* phrase,
          const World* world,
          const Grounding* context,
          const vector< bool >& evaluateFeatureTypes ){

  for( unsigned int i = 0; i < _feature_groups.size(); i++ ){
    bool expressed_feature_in_feature_group = false;
    for( unsigned int j = 0; j < _feature_groups[ i ].size(); j++ ){
      if( evaluateFeatureTypes[ _feature_groups[ i ][ j ]->type() ] ){
        _values[ i ][ j ] = _feature_groups[ i ][ j ]->value( cv, grounding, children, phrase, world, context );
      }
      if( _values[ i ][ j ] ){
        expressed_feature_in_feature_group = true;
      }
    }
    if( !expressed_feature_in_feature_group ){
      return;
    }
/*
    cout << "values[" << _values[ i ].size() << "]:{"; 
    for( unsigned int j = 0; j < _values[ i ].size(); j++ ){
      cout << _values[ i ][ j ];
      if( j != ( _values[ i ].size() - 1 ) ){
        cout << ",";
      }
    }
    cout << "}" << endl;
*/
  }
  return;
}

void 
Feature_Product::
to_xml( const string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

void 
Feature_Product::
to_xml( xmlDocPtr doc, 
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_product" ), NULL );
  for( unsigned int i = 0; i < _feature_groups.size(); i++ ){
    xmlNodePtr feature_group_node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "feature_group" ), NULL );
    for( unsigned int j = 0; j < _feature_groups[ i ].size(); j++ ){
      _feature_groups[ i ][ j ]->to_xml( doc, feature_group_node );
    }
    xmlAddChild( node, feature_group_node );
  }
  xmlAddChild( root, node );
  return;
}

void 
Feature_Product::
from_xml( const string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_product" ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
    }
    xmlFreeDoc( doc );
  }
  return;
} 

void 
Feature_Product::
from_xml( xmlNodePtr root ){
  _values.clear();

  for( unsigned int i = 0; i < _feature_groups.size(); i++ ){
    for( unsigned int j = 0; j < _feature_groups[ i ].size(); j++ ){
      if( _feature_groups[ i ][ j ] != NULL ){
        delete _feature_groups[ i ][ j ];
        _feature_groups[ i ][ j ] = NULL;
      }
    }
    _feature_groups[ i ].clear();
  }
  _feature_groups.clear();

  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_group" ) ) == 0 ){
          _feature_groups.push_back( vector< Feature* >() );
          xmlNodePtr l2 = NULL;
          for( l2 = l1->children; l2; l2 = l2->next ){
            if( l2->type == XML_ELEMENT_NODE ){
              if( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_cv" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_CV( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_word" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Word() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_phrase_type_and_word" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Phrase_Type_And_Word() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_num_words" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Num_Words() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_phrase_type_and_num_words" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Phrase_Type_And_Num_Words() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_num_child_phrases" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Num_Child_Phrases( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_phrase_type_and_num_child_phrases" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Phrase_Type_And_Num_Child_Phrases( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_grounding_string_property_value" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Grounding_String_Property_Value( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_grounding_int_property_value" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Grounding_Int_Property_Value( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_grounding_string_property_value_matches_child_string_property_value" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_grounding_string_property_value_missing_child_string_property" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Grounding_String_Property_Value_Missing_Child_String_Property( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_grounding_string_property_value_matches_child_string_properties" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Properties( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_grounding_class_name" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Grounding_Class_Name( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_object_string_property_value" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Region_Object_String_Property_Value( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_object_matches_child" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Region_Object_Matches_Child( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_merge_spatial_relation_and_object" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Object( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_merge_spatial_relation_and_region" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Region( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_rule_object_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Rule_Object_Type >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_rule_container_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Rule_Container_Type >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_rule_object_color" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Rule_Object_Color >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_rule_spatial_relation" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Rule_Spatial_Relation >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_rule_constraint_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Rule_Constraint_Type >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_rule_constraint_payload_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Rule_Constraint_Payload_Type >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_rule_constraint_reference_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Rule_Constraint_Reference_Type >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_rule_index" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Rule_Index >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_rule_number" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Rule_Number >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_object" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Object >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_object_property" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Object_Property >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_object_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Object_Type >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_container_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Container_Type >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_object_color" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Object_Color >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_number" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Number >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_index" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Index >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_region" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Region >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_constraint" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Constraint >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_container" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Container >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_region_container" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Region_Container >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_abstract_container" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Abstract_Container >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_region_abstract_container" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Region_Abstract_Container >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_spatial_relation" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Spatial_Relation >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_rule_spatial_relation" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Rule_Spatial_Relation >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_rule_container_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Rule_Container_Type >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_rule_object_color" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Rule_Object_Color >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_object" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Object >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_object_property" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Object_Property >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_object_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Object_Type >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_container_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Container_Type >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_object_color" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Object_Color >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_number" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Number >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_index" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Index >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_region" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Region >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_constraint" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Constraint >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_container" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Container >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_region_container" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Region_Container >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_abstract_container" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Abstract_Container >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_region_abstract_container" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Region_Abstract_Container >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_only_child_spatial_relation" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Only_Child< Spatial_Relation >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_ordered_child_rule_spatial_relation" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Ordered_Child< Rule_Spatial_Relation >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_ordered_child_rule_container_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Ordered_Child< Rule_Container_Type >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_matches_sorted_objects" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Object_Matches_Sorted_Objects( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_matches_sorted_objects" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Container_Matches_Sorted_Objects( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_matches_sorted_object" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Object_Matches_Sorted_Object( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_matches_child_region" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Object_Matches_Child< Region, Object >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_merge_partially_known_regions" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Region_Merge_Partially_Known_Regions() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_payload_string_property_value" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Payload_String_Property_Value( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_reference_matches_child" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Reference_Matches_Child( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_parent_matches_child_region" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Parent_Matches_Child_Region() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_child_matches_child_region" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Child_Matches_Child_Region() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_child_matches_child_object" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Child_Matches_Child_Object() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_type_merge_container_type_number" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Container_Type_Merge_Container_Type_Number() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_number_merge_container_type_number" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Number_Merge_Container_Type_Number() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_payload_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Payload_Type() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_reference_type" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Reference_Type() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_object_relationship" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Object_Relationship() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_abstract_container_matches_child" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Abstract_Container_Matches_Child() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_matches_child" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Matches_Child() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_container_matches_child" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Container_Matches_Child() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_container_merge_container_spatial_relation" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Container_Merge_Container_Spatial_Relation() );
                  _feature_groups.back().back()->from_xml( l2 );
              //} else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_action_object_matches_child" ) ) == 0 ){
              //    _feature_groups.back().push_back( new Feature_Action_Object_Matches_Child() );
              //    _feature_groups.back().back()->from_xml( l2 );
/*
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_spatial_relation_matches_child" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Spatial_Relation_Matches_Child() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_is_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Is_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_is_object" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Is_Object() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_is_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Is_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_is_region" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Is_Region() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_is_region_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Is_Region_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_is_region_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Is_Region_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_is_spatial_relation" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Is_Spatial_Relation() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_number" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Number() );
                  _feature_groups.back().back()->from_xml( l2 );
*/
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_object_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Object_Type( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_object_color" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Object_Color( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_number" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Number( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_number_equals_world_objects" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Number_Equals_World_Objects( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_abstract_container_number_equals_world_objects" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Abstract_Container_Number_Equals_World_Objects( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_is_empty" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Is_Empty( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_container_type_matches_child_region_container_container_container_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type( l2 ) );
/*
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_type_matches_child_container_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Type_Matches_Child_Container_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_property_merge_object_property_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Property_Merge_Object_Property_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
*/
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_phrase_has_pos_tag" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Phrase_Has_POS_Tag() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_phrase_type_and_phrase_has_pos_tag" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Phrase_Type_And_Phrase_Has_POS_Tag() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_phrase_has_single_pos_tag" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Phrase_Has_Single_POS_Tag() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_phrase_has_ordered_pos_tag_pair" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Phrase_Has_Ordered_POS_Tag_Pair() );
                  _feature_groups.back().back()->from_xml( l2 );
/*
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_matches_empty_child_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Matches_Empty_Child_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_merge_empty_container_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Merge_Empty_Container_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_merge_object_property_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Merge_Object_Property_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_merge_container_spatial_relation" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Merge_Container_Spatial_Relation() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_container_merge_container_spatial_relation" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Container_Merge_Container_Spatial_Relation() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_container_container_matches_child_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Container_Container_Matches_Child_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_container_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Container_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_container_container_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Container_Container_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_x_object" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_X_Object() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_max_x_object" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Max_X_Object() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_max_y_object" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Max_Y_Object() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_y_object" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_Y_Object() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_max_x_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Max_X_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_distance_object" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_Distance_Object() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_max_distance_object" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Max_Distance_Object() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_distance_object_within" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_Distance_Object_Within() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_max_distance_object_within" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Max_Distance_Object_Within() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_center_distance_object" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_Center_Distance_Object() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_max_y_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Max_Y_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_y_index_abstract_container_region_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_Y_Index_Abstract_Container_Region_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_x_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_X_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_y_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_Y_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_distance_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_Distance_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_distance_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_Distance_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_abs_y_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_Abs_Y_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_dominant_x_shape" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Dominant_X_Shape() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_dominant_y_shape" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Dominant_Y_Shape() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_dominant_z_shape" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Dominant_Z_Shape() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_non_dominant_xyz_shape" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Non_Dominant_XYZ_Shape() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_x_container_abstract_container_spatial_relation" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_X_Container_Abstract_Container_Spatial_Relation() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_objects_shape_matches_container_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Objects_Shape_Matches_Container_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_y_container_abstract_container_spatial_relation" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_Y_Container_Abstract_Container_Spatial_Relation() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_max_y_container_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Max_Y_Container_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_abs_y_container_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_Abs_Y_Container_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_x_container_region_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_X_Container_Region_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_x_region_abstract_container_spatial_relation" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_X_Region_Abstract_Container_Spatial_Relation() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_max_x_region_abstract_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Max_X_Region_Abstract_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              //} else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_matches_child" ) ) == 0 ){
              //    _feature_groups.back().push_back( new Feature_Object_Matches_Child() );
              //    _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_min_distance" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Min_Distance() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_max_distance" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Max_Distance() );
                  _feature_groups.back().back()->from_xml( l2 );
*/
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_matches_child_container_object" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Matches_Child_Container_Object( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_matches_child_region_container_object" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Matches_Child_Region_Container_Object( l2 ) );
/*
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_number_matches_world_object_number" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Number_Matches_World_Object_Number() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_number_equals_world_objects" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Number_Equals_World_Objects() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_objects_shape_matches_container_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Objects_Shape_Matches_Container_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_abstract_container_number_equals_world_objects" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Abstract_Container_Number_Equals_World_Objects() );
                  _feature_groups.back().back()->from_xml( l2 );
*/
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_object_property_spatial_relation" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_property_merge_index_object_property" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Property_Merge_Index_Object_Property() );
                  _feature_groups.back().back()->from_xml( l2 );
/*
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_object_property_index_spatial_relation" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Merge_Object_Property_Index_Spatial_Relation() );
                  _feature_groups.back().back()->from_xml( l2 );
	      } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_object_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Merge_Object_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
          //} else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_abstract_container_index_region_container" ) ) == 0 ){
          //    _feature_groups.back().push_back( new Feature_Object_Merge_Abstract_Container_Index_Region_Container() );
          //    _feature_groups.back().back()->from_xml( l2 );
*/
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_container_pair" ) ) == 0 ){
              _feature_groups.back().push_back( new Feature_Object_Merge_Container_Pair() );
              _feature_groups.back().back()->from_xml( l2 );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_merge_container_pair" ) ) == 0 ){
              _feature_groups.back().push_back( new Feature_Container_Merge_Container_Pair() );
              _feature_groups.back().back()->from_xml( l2 );
/*
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_object_different_container" ) ) == 0 ){
              _feature_groups.back().push_back( new Feature_Object_Merge_Object_Different_Container() );
              _feature_groups.back().back()->from_xml( l2 );
*/
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_object_property_region_container" ) ) == 0 ){
              _feature_groups.back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_object_type_region_container" ) ) == 0 ){
              _feature_groups.back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_object_region_container" ) ) == 0 ){
              _feature_groups.back().push_back( new Feature_Object_Merge_Object_Region_Container( l2 ) );
/*
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_single_object_different_container" ) ) == 0 ){
              _feature_groups.back().push_back( new Feature_Object_Merge_Single_Object_Different_Container() );
              _feature_groups.back().back()->from_xml( l2 );
*/
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_abstract_container_region_container" ) ) == 0 ){
              _feature_groups.back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container() );
              _feature_groups.back().back()->from_xml( l2 );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_abstract_container_spatial_relation" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_object_property_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Object_Merge_Object_Property_Container( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_region_abstract_container_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_object_type_spatial_relation" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_merge_object_type_region" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Object_Merge_Object_Type_Region( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_merge_abstract_container_spatial_relation" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_merge_abstract_container_container_type" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Container_Merge_Abstract_Container_Container_Type( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_merge_abstract_container_region" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Container_Merge_Abstract_Container_Region( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_merge_abstract_container_region_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_merge_region_abstract_container_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_merge_container_type_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Container_Merge_Container_Type_Container( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_merge_container_region" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Container_Merge_Container_Region( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_rule_object_type_merge_rule_object_types" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Rule_Object_Type_Merge_Rule_Object_Types( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_rule_object_type_merge_rule_container_type_rule_object_type" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_rule_object_color_merge_rule_object_type_rule_spatial_relation" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Rule_Object_Color_Merge_Rule_Object_Type_Rule_Spatial_Relation( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_rule_spatial_relation_merge_rule_spatial_relations" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_rule_spatial_relation_merge_rule_spatial_relation_rule_container_type" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_rule_container_type_merge_rule_container_types" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Container_Types( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_rule_container_type_equals_world_objects" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Rule_Container_Type_Equals_World_Objects( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_rule_container_type_merge_rule_object_types" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Object_Types( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_rule_index_merge_rule_container_type_rule_index" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_object_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Object, Container >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_container_spatial_relation" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Container, Spatial_Relation >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_container_region_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Container, Region_Container >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_object_region_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Object, Region_Container >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_abstract_container_object" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Abstract_Container, Object >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_abstract_container_region_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Abstract_Container, Region_Container >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_abstract_container_region" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Abstract_Container, Region >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_number_region_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Number, Region_Container >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_index_region_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Index, Region_Container >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_object_type_rule_spatial_relation" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Object_Type, Rule_Spatial_Relation >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_object_type_rule_container_type" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Object_Type, Rule_Container_Type >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_index_rule_spatial_relation" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Spatial_Relation >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_index_rule_object_type" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Object_Type >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_index_rule_container_type" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Container_Type >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_number_rule_object_type" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Object_Type >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_number_rule_spatial_relation" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Spatial_Relation >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_number_rule_container_type" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Container_Type >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_container_type_rule_spatial_relation" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Spatial_Relation >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_container_type_rule_index" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Index >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_container_type_rule_number" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Number >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_object_color_rule_spatial_relation" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Object_Color, Rule_Spatial_Relation >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_merge_children_rule_object_color_rule_container_type" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Merge_Children< Rule_Object_Color, Rule_Container_Type >( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_is_in_a_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Is_In_A_Container( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_shape_abstract_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Object_Shape_Abstract_Container( l2 ) );
          } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_shape_abstract_container" ) ) == 0 ){
            _feature_groups.back().push_back( new Feature_Container_Shape_Abstract_Container( l2 ) );
          } else {
                cout << "could not load feature " << l2->name << endl;
                assert( false );
              } 
            }
          }
        }
      }
    }
  }
  _values.resize( _feature_groups.size() );
  for( unsigned int i = 0; i < _feature_groups.size(); i++ ){
    _values[ i ].resize( _feature_groups[ i ].size() );
  }
  return;
}


unsigned int
Feature_Product::
size( void )const{
  unsigned int tmp = 0;
  for( unsigned int i = 0; i < _feature_groups.size(); i++ ){
    if( i == 0 ){
      tmp = _feature_groups[ i ].size();
    } else {
      tmp *= _feature_groups[ i ].size();
    }
  }
  return tmp;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Product& other ) {
    return out;
  }
}
