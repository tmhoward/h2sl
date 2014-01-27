/**
 * @file    constraint_set.cc
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
 * The implementation of a class used to describe a set of constraints
 */

#include "h2sl/constraint_set.h"

using namespace std;
using namespace h2sl;

Constraint_Set::
Constraint_Set( const vector< Constraint >& constraints ) : Grounding(),
                                                            _constraints( constraints ) {

}

Constraint_Set::
~Constraint_Set() {

}

Constraint_Set::
Constraint_Set( const Constraint_Set& other ) : Grounding( other ),
                                                _constraints( other._constraints ){

}

Constraint_Set&
Constraint_Set::
operator=( const Constraint_Set& other ) {
  _constraints = other._constraints;
  return (*this);
}

Grounding*
Constraint_Set::
dup( void )const{
  return new Constraint_Set( *this );
}

void
Constraint_Set::
clear( void ){
  _constraints.clear();
  return;
}

void
Constraint_Set::
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
Constraint_Set::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "constraint_set" ), NULL );
  for( unsigned int i = 0; i < _constraints.size(); i++ ){
    _constraints[ i ].to_xml( doc, node );
  }
  xmlAddChild( root, node );
  return;
}

void
Constraint_Set::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "constraint_set" ) ) == 0 ){
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
Constraint_Set::
from_xml( xmlNodePtr root ){
  _constraints.clear();
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "constraint" ) ) == 0 ){
          _constraints.push_back( Constraint() );
          _constraints.back().from_xml( l1 );
        }
      }
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Constraint_Set& other ) {
    out << "constraints[" << other.constraints().size() << "]:{";
    for( unsigned int i = 0; i < other.constraints().size(); i++ ){
      out << "(" << other.constraints()[ i ] << ")";
      if( i != ( other.constraints().size() - 1 ) ){
        out << ",";
      }
    }
    out << "}";
    return out;
  }
}
