/**
 * @file    search_space.cc
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
 * 02110-1301, USA. *
 * @section DESCRIPTION
 *
 * The implementation of a class used to describe an search_space
 */

#include "h2sl/common.h"
#include "h2sl/rule_object_type.h"
#include "h2sl/rule_object_color.h"
#include "h2sl/rule_spatial_relation.h"

#include "h2sl/object_property.h"
#include "h2sl/object_type.h"
#include "h2sl/object_color.h"
#include "h2sl/object.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/container.h"
#include "h2sl/region_container.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_abstract_container.h"

#include "h2sl/search_space.h"

using namespace std;
using namespace h2sl;

Search_Space::
Search_Space ( const map< string, pair< string, vector< Grounding* > > >& groundingPairs, 
	       const map< string, vector< unsigned int > >& cvs ) : _grounding_pairs( groundingPairs ),
                        			                    _cvs( cvs ) {
}

Search_Space::
Search_Space( xmlNodePtr root ) {
  from_xml( root );
}

Search_Space::
~Search_Space() {

}

Search_Space::
Search_Space( const Search_Space& other ) :  
                                _grounding_pairs( other._grounding_pairs ), 
                                _cvs( other._cvs ) {

}

Search_Space&
Search_Space::
operator=( const Search_Space& other ) {
  _grounding_pairs = other._grounding_pairs; 
  _cvs = other._cvs;
  return (*this);
}

bool
Search_Space::
operator==( const Search_Space& other )const{
  if( grounding_pairs() != other.grounding_pairs() ){
    return false;
  } else if ( cvs() != other.cvs() ){
    return false;
  } else {
    return true;
  }   
}

bool
Search_Space::
operator!=( const Search_Space& other )const{
  return !( *this == other );
} 
 
Search_Space*
Search_Space::
dup( void )const{
  return new Search_Space( *this );
}

void 
Search_Space::
fill_groundings( const Symbol_Dictionary& symbolDictionary, 
                  const World* world ){
  clear();

  std::vector< unsigned int > binary_cvs;
  binary_cvs.push_back( CV_FALSE );
  binary_cvs.push_back( CV_TRUE );

  std::vector< unsigned int > ternary_cvs;
  ternary_cvs.push_back( CV_FALSE );
  ternary_cvs.push_back( CV_TRUE );
  ternary_cvs.push_back( CV_INVERTED );

  _cvs.insert( pair< string, vector< unsigned int > >( "binary", binary_cvs ) );
  _cvs.insert( pair< string, vector< unsigned int > >( "ternary", ternary_cvs ) );

  Object::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Object_Type::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Object_Color::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Object_Property::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Number::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Index::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Region::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Spatial_Relation::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Constraint::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Abstract_Container::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Region_Abstract_Container::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Container::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Region_Container::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  return;
}

void 
Search_Space::
fill_rules( const Symbol_Dictionary& symbolDictionary, 
            const World* world ){
  clear();

  std::vector< unsigned int > binary_cvs;
  binary_cvs.push_back( CV_FALSE );
  binary_cvs.push_back( CV_TRUE );

  _cvs.insert( pair< string, vector< unsigned int > >( "binary", binary_cvs ) );

  Rule_Object_Type::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Rule_Object_Color::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  Rule_Spatial_Relation::fill_search_space( symbolDictionary, world, _grounding_pairs, SYMBOL_TYPE_ALL );
  return;
}


void
Search_Space::
clear( void ){
  for( map< string, pair< string, vector< Grounding* > > >::iterator it_groundings = _grounding_pairs.begin(); it_groundings != _grounding_pairs.end(); it_groundings++ ){
    for( vector< Grounding* >::iterator it_grounding = it_groundings->second.second.begin(); it_grounding != it_groundings->second.second.end(); it_grounding++ ){
      Grounding * grounding = *it_grounding;
      if( grounding != NULL ){
        delete grounding;
        grounding = NULL;
      }
    }
    it_groundings->second.second.clear();
  }
  _grounding_pairs.clear();

  for( map< string, vector< unsigned int > >::iterator it_cvs = _cvs.begin(); it_cvs != _cvs.end(); it_cvs++ ){
    it_cvs->second.clear();
  }
  _cvs.clear();
  return;
}

void
Search_Space::
scrape_examples( const string& filename,
                  const Phrase* phrase,
                  const World* world,
                  vector< pair< unsigned int, h2sl::LLM_X > >& examples ){
  assert( phrase->grounding_set() != NULL );
  for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_groundings = _grounding_pairs.begin(); it_groundings != _grounding_pairs.end(); it_groundings++ ){
    for( vector< Grounding* >::const_iterator it_grounding = it_groundings->second.second.begin(); it_grounding != it_groundings->second.second.end(); it_grounding++ ){
      map< string, vector< unsigned int > >::const_iterator it_cvs = _cvs.find( it_groundings->second.first );
      assert( it_cvs != _cvs.end() );
      examples.push_back( pair< unsigned int, h2sl::LLM_X >( phrase->grounding_set()->evaluate_cv( *it_grounding ), h2sl::LLM_X( *it_grounding, phrase, world, it_cvs->second, vector< h2sl::Feature* >(), filename ) ) );
      for( unsigned int j = 0; j < phrase->children().size(); j++ ){
        examples.back().second.children().push_back( pair< const h2sl::Phrase*, vector< h2sl::Grounding* > >( phrase->children()[ j ], vector< h2sl::Grounding* >() ) );
        Grounding_Set * child_grounding_set = phrase->children()[ j ]->grounding_set();
        if( child_grounding_set ){
          for( unsigned int k = 0; k < child_grounding_set->groundings().size(); k++ ){
            examples.back().second.children().back().second.push_back( child_grounding_set->groundings()[ k ] );
          }
        }
      }
    }
  }

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    scrape_examples( filename, dynamic_cast< Phrase* >( phrase->children()[ i ] ), world, examples );
  }
  return;
}
 
void
Search_Space::
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
Search_Space::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
 return;
}

void
Search_Space::
from_xml( const string& filename ){
/*
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
    }
    xmlFreeDoc( doc );
  }
*/
  return;
}

void
Search_Space::
from_xml( xmlNodePtr root ){
  return;
}


namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Search_Space& other ) {
    out << "grounding_pairs[" << other.grounding_pairs().size() << "]:(";
    for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_grounding_pairs = other.grounding_pairs().begin(); it_grounding_pairs != other.grounding_pairs().end(); it_grounding_pairs++ ){
      out << it_grounding_pairs->first << "[" << it_grounding_pairs->second.second.size() << "]:{";
      out << "}";
      if( next( it_grounding_pairs ) != other.grounding_pairs().end() ){
        out << ",";
      }
    }
    out << "),correspondence_variables[" << other.cvs().size() << "]:(";
    for( map< string, vector< unsigned int > >::const_iterator it_cvs = other.cvs().begin(); it_cvs != other.cvs().end(); it_cvs++ ){
      out << it_cvs->first << "[" << it_cvs->second.size() << "]:{";
      for( unsigned int i = 0; i < it_cvs->second.size(); i++ ){
        out << it_cvs->second[ i ];
        if( i != ( it_cvs->second.size() - 1 ) ){
          out << ",";
        }
      }
      out << "}";
      if( next( it_cvs ) != other.cvs().end() ){
        out << ",";
      }
    }
    out << ")";
    return out;
  }
}
