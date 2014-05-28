/**
 * @file    grounding_set.cc
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
 * The implementation of a class used to describe a set of groundings
 */

#include "h2sl/object.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"

#include "h2sl/grounding_set.h"

using namespace std;
using namespace h2sl;

Grounding_Set::
Grounding_Set( const vector< Grounding* >& groundings ) : Grounding(),
                                                _groundings( groundings ) {

}

Grounding_Set::
~Grounding_Set() {
  clear();
}

Grounding_Set::
Grounding_Set( const Grounding_Set& other ) : Grounding( other ),
                                        _groundings(){
  _groundings.resize( other._groundings.size(), NULL );
  for( unsigned int i = 0; i < other._groundings.size(); i++ ){
    _groundings[ i ] = other._groundings[ i ]->dup();
  }   
}

Grounding_Set&
Grounding_Set::
operator=( const Grounding_Set& other ) {
  clear();
  _groundings.resize( other._groundings.size(), NULL );
  for( unsigned int i = 0; i < other._groundings.size(); i++ ){
    _groundings[ i ] = other._groundings[ i ]->dup();
  }
  return (*this);
}

Grounding_Set*
Grounding_Set::
dup( void )const{
  return new Grounding_Set( *this );
}

void
Grounding_Set::
clear( void ){
  for( unsigned int i = 0; i < _groundings.size(); i++ ){
    if( _groundings[ i ] != NULL ){
      delete _groundings[ i ];
      _groundings[ i ] = NULL;
    }
  } 
  _groundings.clear();
  return;
}

void
Grounding_Set::
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
Grounding_Set::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "grounding_set" ), NULL );
  for( unsigned int i = 0; i < _groundings.size(); i++ ){
    if( _groundings[ i ] != NULL ){
      _groundings[ i ]->to_xml( doc, node );
    }
  }
  xmlAddChild( root, node );
  return;
}

void
Grounding_Set::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "grounding_set" ) ) == 0 ){
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
Grounding_Set::
from_xml( xmlNodePtr root ){
  clear();
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
          _groundings.push_back( new Object() );
          _groundings.back()->from_xml( l1 );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "region" ) ) == 0 ){
          _groundings.push_back( new Region() );
          _groundings.back()->from_xml( l1 );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "constraint" ) ) == 0 ){
          _groundings.push_back( new Constraint() );
          _groundings.back()->from_xml( l1 );
        } 
      }
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Grounding_Set& other ) {
    for( unsigned int i = 0; i < other.groundings().size(); i++ ){
      if( other.groundings()[ i ] != NULL ){
        out << *other.groundings()[ i ];
      }
      if( i != ( other.groundings().size() - 1 ) ){
        out << ",";
      } 
    }
    return out;
  }
}
