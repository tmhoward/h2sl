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
    out << "Search_Space(";
    out << ")";
    return out;
  }
}
