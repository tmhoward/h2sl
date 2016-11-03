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

#include "h2sl/object.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"

#include "h2sl/feature_word.h"
#include "h2sl/feature_num_words.h"
#include "h2sl/feature_cv.h"
#include "h2sl/feature_type.h"
#include "h2sl/feature_grounding_property_value.h"
#include "h2sl/feature_region_object_property_value.h"
#include "h2sl/feature_matches_child.h"
#include "h2sl/feature_object_matches_child.h"
#include "h2sl/feature_region_merge_partially_known_regions.h"
#include "h2sl/feature_constraint_parent_matches_child_region.h"
#include "h2sl/feature_constraint_child_matches_child_region.h"
#include "h2sl/feature_constraint_parent_is_robot.h"
#include "h2sl/feature_constraint_child_is_robot.h"
#include "h2sl/feature_product.h"
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
//#include "h2sl/feature_object_property_index.h"
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
indices( const unsigned int& cv, 
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
indices( const unsigned int& cv,
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
evaluate( const unsigned int& cv,
          const Grounding* grounding, 
          const vector< pair< const Phrase*, vector< Grounding* > > >& children,
          const Phrase* phrase,
          const World* world,
          const Grounding* context,
          const vector< bool >& evaluateFeatureTypes ){

//  cout << "phrase:" << *phrase << endl;

  for( unsigned int i = 0; i < _feature_groups.size(); i++ ){
    for( unsigned int j = 0; j < _feature_groups[ i ].size(); j++ ){
      if( evaluateFeatureTypes[ _feature_groups[ i ][ j ]->type() ] ){
        _values[ i ][ j ] = _feature_groups[ i ][ j ]->value( cv, grounding, children, phrase, world, context );
      }
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
                _feature_groups.back().push_back( new Feature_CV() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_word" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Word() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_num_words" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Num_Words() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_grounding_property_value" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Grounding_Property_Value( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_object_property_value" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Region_Object_Property_Value( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_object" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Object >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_region" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Region >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_matches_child_constraint" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Matches_Child< Constraint >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_matches_child_region" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Object_Matches_Child< Region, Object >( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_merge_partially_known_regions" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Region_Merge_Partially_Known_Regions() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_parent_matches_child_region" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Parent_Matches_Child_Region() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_child_matches_child_region" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Child_Matches_Child_Region() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_parent_is_robot" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Parent_Is_Robot() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint_child_is_robot" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint_Child_Is_Robot() );
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
              //} else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_action_object_matches_child" ) ) == 0 ){
              //    _feature_groups.back().push_back( new Feature_Action_Object_Matches_Child() );
              //    _feature_groups.back().back()->from_xml( l2 );
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
                  _feature_groups.back().push_back( new Feature_Container_Number( l2 ) );
              //} else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_number_equals_world_objects" ) ) == 0 ){
              //    _feature_groups.back().push_back( new Feature_Container_Number_Equals_World_Objects() );
              //    _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_is_empty" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Is_Empty() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_container_type_matches_child_container_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Container_Type_Matches_Child_Container_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_property_merge_object_property_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Property_Merge_Object_Property_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_phrase_has_pos_tag" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Phrase_Has_POS_Tag( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_phrase_has_single_pos_tag" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Phrase_Has_Single_POS_Tag( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_phrase_has_ordered_pos_tag_pair" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Phrase_Has_Ordered_POS_Tag_Pair( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_property_merge_object_property_spatial_relation" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Property_Merge_Object_Property_Spatial_Relation() );
                  _feature_groups.back().back()->from_xml( l2 );
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
                  _feature_groups.back().push_back( new Feature_Region_Container_Merge_Container_Spatial_Relation( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_container_container_matches_child_container" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Container_Container_Matches_Child_Container() );
                  _feature_groups.back().back()->from_xml( l2 );
              //} else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_property_index" ) ) == 0 ){
              //    _feature_groups.back().push_back( new Feature_Object_Property_Index( l2 ) );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_abstract_container_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Abstract_Container_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_abstract_container_color" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Abstract_Container_Color() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_abstract_container_number" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Abstract_Container_Number() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_abstract_container_index" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Abstract_Container_Index() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_container_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Container_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_container_container_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Container_Container_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_property_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Property_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_property_relation_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Property_Relation_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object_property_index" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Object_Property_Index() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_abstract_container_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Abstract_Container_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_abstract_container_object_type" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Abstract_Container_Object_Type() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_abstract_container_number" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Region_Abstract_Container_Number() );
                  _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_min_x_object" ) ) == 0 ){
                  _feature_groups.back().push_back( new Feature_Min_X_Object() );
                  _feature_groups.back().back()->from_xml( l2 );
              }else {
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
