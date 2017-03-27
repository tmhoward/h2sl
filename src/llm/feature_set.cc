/**
 * @file    feature_set.cc
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
 * The implementation of a class used to describe a set of features
 */

#include "h2sl/feature_set.h"

#include "h2sl/feature_word.h"
#include "h2sl/feature_num_words.h"
#include "h2sl/feature_cv.h"
#include "h2sl/feature_phrase_has_pos_tag.h"
#include "h2sl/feature_phrase_has_single_pos_tag.h"
#include "h2sl/feature_phrase_has_ordered_pos_tag_pair.h"

#include "h2sl/feature_abstract_container_matches_child.h"

#include "h2sl/feature_constraint_payload_type.h"
#include "h2sl/feature_constraint_reference_type.h"

#include "h2sl/feature_container_dominant_x_shape.h"
#include "h2sl/feature_container_dominant_y_shape.h"
#include "h2sl/feature_container_dominant_z_shape.h"
#include "h2sl/feature_container_non_dominant_xyz_shape.h"
#include "h2sl/feature_container_is_empty.h"
#include "h2sl/feature_container_matches_child.h"
#include "h2sl/feature_container_matches_empty_child_container.h"
#include "h2sl/feature_container_merge_empty_container_container.h"
#include "h2sl/feature_container_merge_object_property_container.h"
#include "h2sl/feature_container_min_distance.h"
#include "h2sl/feature_container_max_distance.h"
#include "h2sl/feature_container_number.h"
#include "h2sl/feature_container_number_equals_world_objects.h"
#include "h2sl/feature_container_type_matches_child_container_type.h"

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
#include "h2sl/feature_min_y_container_abstract_container_spatial_relation.h"
#include "h2sl/feature_min_y_index_abstract_container_region_container.h"
#include "h2sl/feature_min_y_object.h"
#include "h2sl/feature_object_matches_child_container_object.h"
#include "h2sl/feature_object_number_matches_world_object_number.h"
#include "h2sl/feature_object_property_merge_object_property_container.h"
#include "h2sl/feature_object_property_merge_object_property_spatial_relation.h"
#include "h2sl/feature_objects_shape_matches_container_type.h"
#include "h2sl/feature_region_container_container_matches_child_container.h"
#include "h2sl/feature_region_container_matches_child.h"
#include "h2sl/feature_region_container_merge_container_spatial_relation.h"
#include "h2sl/feature_region_container_type.h"
#include "h2sl/feature_region_merge_partially_known_regions.h"
#include "h2sl/feature_region_object_string_property_value.h"
#include "h2sl/feature_spatial_relation_matches_child.h"
#include "h2sl/feature_grounding_string_property_value.h"
#include "h2sl/feature_grounding_int_property_value.h"
#include "h2sl/feature_matches_child.h"
#include "h2sl/feature_object_merge_object_property_spatial_relation.h"

#include "h2sl/feature_type.h"
#include "h2sl/feature_object_matches_child.h"
#include "h2sl/feature_constraint_parent_matches_child_region.h"
#include "h2sl/feature_constraint_child_matches_child_region.h"

#include "h2sl/feature_container_merge_container_spatial_relation.h"
#include "h2sl/feature_region_container_container_type.h"
#include "h2sl/feature_object_matches_child.h"
#include "h2sl/feature_container_number_equals_world_objects.h"
#include "h2sl/feature_abstract_container_number_equals_world_objects.h"

#include "h2sl/feature_object_merge_object_container.h"

#include "h2sl/feature_object_merge_abstract_container_index_region_container.h"
#include "h2sl/feature_object_merge_container_pair.h"
#include "h2sl/feature_object_merge_object_different_container.h"
#include "h2sl/feature_object_merge_object_property_region_container.h"
#include "h2sl/feature_object_merge_object_region_container.h"
#include "h2sl/feature_object_merge_single_object_different_container.h"
#include "h2sl/feature_object_merge_abstract_container_region_container.h"
#include "h2sl/feature_object_merge_abstract_container_spatial_relation.h"

using namespace std;
using namespace h2sl;

Feature_Set::
Feature_Set() : _feature_products() {

}

Feature_Set::
~Feature_Set() {

}

Feature_Set::
Feature_Set( const Feature_Set& other ) : _feature_products( other._feature_products ) {

}

Feature_Set&
Feature_Set::
operator=( const Feature_Set& other ) {
  _feature_products = other._feature_products;
  return (*this);
}

void 
Feature_Set::
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
  unsigned int offset = 0;
  for( unsigned int i = 0; i < _feature_products.size(); i++ ){
    vector< unsigned int > product_indices;
    _feature_products[ i ]->indices( cv, grounding, children, phrase, world, context, product_indices, features, evaluateFeatureTypes );
    for( unsigned int j = 0; j < product_indices.size(); j++ ){
      indices.push_back( product_indices[ j ] + offset );
    }
    offset += _feature_products[ i ]->size();
  }
  return;
}

void
Feature_Set::
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
  unsigned int offset = 0;
  for( unsigned int i = 0; i < _feature_products.size(); i++ ){
    vector< unsigned int > product_indices;
    _feature_products[ i ]->indices( cv, grounding, children, phrase, world, context, product_indices, weightedFeatures, evaluateFeatureTypes );
    for( unsigned int j = 0; j < product_indices.size(); j++ ){
      indices.push_back( product_indices[ j ] + offset );
    }
    offset += _feature_products[ i ]->size();
  }
  return;
}

void
Feature_Set::
evaluate( const string& cv,
          const Grounding* grounding, 
          const vector< pair< const Phrase*, vector< Grounding* > > >& children,
          const Phrase* phrase,
          const World* world,
          const Grounding* context,
          const vector< bool >& evaluateFeatureTypes ){

  cout << "evaluating " << _feature_products.size() << " feature products" << endl;

  for( unsigned int i = 0; i < _feature_products.size(); i++ ){
    _feature_products[ i ]->evaluate( cv, grounding, children, phrase, world, context, evaluateFeatureTypes );
  }
  return;
}

void 
Feature_Set::
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
Feature_Set::
to_xml( xmlDocPtr doc, 
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_set" ), NULL );
  for( unsigned int i = 0; i < _feature_products.size(); i++ ){
    _feature_products[ i ]->to_xml( doc, node );
  }
  xmlAddChild( root, node );
  return;
}

void 
Feature_Set::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_set" ) ) == 0 ){
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
Feature_Set::
from_xml( xmlNodePtr root ){
  for( unsigned int i = 0; i < _feature_products.size(); i++ ){
    if( _feature_products[ i ] != NULL ){
      delete _feature_products[ i ];
      _feature_products[ i ] = NULL; 
    }
  }
  _feature_products.clear();

  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_product" ) ) == 0 ){
          _feature_products.push_back( new Feature_Product() );
          _feature_products.back()->from_xml( l1 );
        }
      }
    }
  }
  return;
}

unsigned int
Feature_Set::
size( void )const{
  unsigned int tmp = 0;
  for( unsigned int i = 0; i < _feature_products.size(); i++ ){
    tmp += _feature_products[ i ]->size();
  }
  return tmp;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Set& other ) {
    return out;
  }
}
