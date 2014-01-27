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
Feature_Set() : _feature_groups(),
                _values() {

}

Feature_Set::
~Feature_Set() {

}

Feature_Set::
Feature_Set( const Feature_Set& other ) : _feature_groups( other._feature_groups ),
                                          _values( other._values ){

}

Feature_Set&
Feature_Set::
operator=( const Feature_Set& other ) {
  _feature_groups = other._feature_groups;
  _values = other._values;
  return (*this);
}

void 
Feature_Set::
indices( const unsigned int& cv, 
          const Grounding* grounding,
          const vector< Grounding* >& children, 
          const Phrase* phrase,
          vector< unsigned int >& indices ){
  indices.clear();
  evaluate( cv, grounding, children, phrase );

  if( _values.size() == 3 ){
    for( unsigned int i = 0; i < _values[ 0 ].size(); i++ ){
      for( unsigned int j = 0; j < _values[ 1 ].size(); j++ ){
        for( unsigned int k = 0; k < _values[ 2 ].size(); k++ ){
          indices.push_back( _values[ 0 ][ i ] * _feature_groups[ 1 ].size() * _feature_groups[ 2 ].size() + _values[ 1 ][ j ] * _feature_groups[ 2 ].size() + _values[ 2 ][ k ] );
        }
      }
    }
  }

  return;
}

void
Feature_Set::
evaluate( const unsigned int& cv,
          const Grounding* grounding, 
          const vector< Grounding* >& children,
          const Phrase* phrase ){
  for( unsigned int i = 0; i < _feature_groups.size(); i++ ){
    _values[ i ].clear();
    for( unsigned int j = 0; j < _feature_groups[ i ].size(); j++ ){
      if( _feature_groups[ i ][ j ]->value( cv, grounding, children, phrase ) ){
        _values[ i ].push_back( j );
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
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_object" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Object() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_object" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Region_Object() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Region() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_constraint" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Constraint() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_object_matches_child" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Region_Object_Matches_Child() );
                _feature_groups.back().back()->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "feature_region_matches_child" ) ) == 0 ){
                _feature_groups.back().push_back( new Feature_Region_Matches_Child() );
                _feature_groups.back().back()->from_xml( l2 );
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
              } else {
                cout << "could not load feature " << l2->name << endl;
              } 
            }
          }
        }
      }
    }
  }
  _values.resize( _feature_groups.size() );
  return;
}

unsigned int
Feature_Set::
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
              const Feature_Set& other ) {
    return out;
  }

}
