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

#include "h2sl/feature_word.h"
#include "h2sl/feature_num_words.h"
#include "h2sl/feature_cv.h"
#include "h2sl/feature_object.h"
#include "h2sl/feature_region_object.h"
#include "h2sl/feature_region.h"
#include "h2sl/feature_constraint.h"
#include "h2sl/feature_region_object_matches_child.h"
#include "h2sl/feature_region_matches_child.h"
#include "h2sl/feature_region_merge_partially_known_regions.h"
#include "h2sl/feature_constraint_parent_matches_child_region.h"
#include "h2sl/feature_constraint_child_matches_child_region.h"
#include "h2sl/feature_constraint_parent_is_robot.h"
#include "h2sl/feature_constraint_child_is_robot.h"
#include "h2sl/feature_set.h"

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
indices( const unsigned int& cv, 
          const Grounding* grounding,
          const vector< pair< const Phrase*, vector< Grounding* > > >& children, 
          const Phrase* phrase,
          const World* world,
          vector< unsigned int >& indices,
          vector< Feature* >& features,
          const vector< bool >& evaluateFeatureTypes ){
  indices.clear();
//  evaluate( cv, grounding, children, phrase, world );
  unsigned int offset = 0;
//  cout << "phrase:" << *phrase << endl;
  for( unsigned int i = 0; i < _feature_products.size(); i++ ){
//    cout << "offset: " << offset << endl;
    vector< unsigned int > product_indices;
    _feature_products[ i ]->indices( cv, grounding, children, phrase, world, product_indices, features, evaluateFeatureTypes );
/*
    cout << "product_indices[" << product_indices.size() << "]:{";
    for( unsigned int j = 0; j < product_indices.size(); j++ ){
      cout << product_indices[ j ];
      if( j != ( product_indices.size() - 1 ) ){
        cout << ",";
      }
    }
    cout << "}" << endl; 
*/
    for( unsigned int j = 0; j < product_indices.size(); j++ ){
      indices.push_back( product_indices[ j ] + offset );
    }
    offset += _feature_products[ i ]->size();
  }
/*
  cout << "indices[" << indices.size() << "]:{";
  for( unsigned int j = 0; j < indices.size(); j++ ){
    cout << indices[ j ];
    if( j != ( indices.size() - 1 ) ){
      cout << ",";
    }
  }
  cout << "}" << endl;
*/
/*
  if( _values.size() == 3 ){
    for( unsigned int i = 0; i < _values[ 0 ].size(); i++ ){
      for( unsigned int j = 0; j < _values[ 1 ].size(); j++ ){
        for( unsigned int k = 0; k < _values[ 2 ].size(); k++ ){
          indices.push_back( _values[ 0 ][ i ] * _feature_groups[ 1 ].size() * _feature_groups[ 2 ].size() + _values[ 1 ][ j ] * _feature_groups[ 2 ].size() + _values[ 2 ][ k ] );
        }
      }
    }
  }
*/
  return;
}

void
Feature_Set::
evaluate( const unsigned int& cv,
          const Grounding* grounding, 
          const vector< pair< const Phrase*, vector< Grounding* > > >& children,
          const Phrase* phrase,
          const World* world,
          const vector< bool >& evaluateFeatureTypes ){

  for( unsigned int i = 0; i < _feature_products.size(); i++ ){
    _feature_products[ i ]->evaluate( cv, grounding, children, phrase, world, evaluateFeatureTypes );
/*
    for( unsigned int j = 0; j < _feature_groups[ i ].size(); j++ ){
      if( _feature_groups[ i ][ j ]->value( cv, grounding, children, phrase, world ) ){
        _values[ i ].push_back( j );
      }
    }
*/
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
