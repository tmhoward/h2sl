/**
 * @file    constraint.cc
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
 * The implementation of a class used to represent a constraint
 */

#include "h2sl/constraint.h"

using namespace std;
using namespace h2sl;

Constraint::
Constraint( const unsigned int& type,
            const Region& parent,
            const Region& child ) : Grounding(),
                                    _type( type ),
                                    _parent( parent ),
                                    _child( child ) {

}

Constraint::
Constraint( const constraint_type_t& type,
            const Region& parent,
            const Region& child ) : Grounding(),
                                    _type( type ),
                                    _parent( parent ),
                                    _child( child ) {

}

Constraint::
~Constraint() {

}

Constraint::
Constraint( const Constraint& other ) : Grounding( other ),
                                        _type( other._type ),
                                        _parent( other._parent ),
                                        _child( other._child ){

}

Constraint&
Constraint::
operator=( const Constraint& other ) {
  _type = other._type;
  _parent = other._parent;
  _child = other._child;
  return (*this);
}

bool
Constraint::
operator==( const Constraint& other )const{
  if( _type != other._type ){
    return false;
  } else if ( _parent != other._parent ){
    return false;
  } else if ( _child != other._child ){
    return false; 
  } else {
    return true;
  }
}

bool
Constraint::
operator!= ( const Constraint& other )const{
  return !( *this == other );
}

Grounding*
Constraint::
dup( void )const{
  return new Constraint( *this );
}

string
Constraint::
type_to_std_string( const unsigned int& type ){
  switch( type ){
  case( CONSTRAINT_TYPE_INSIDE ):
    return "inside";
    break;
  case( CONSTRAINT_TYPE_OUTSIDE ):
    return "outside";
    break;
  case( CONSTRAINT_TYPE_UNKNOWN ):
  default:
    return "na";
  }
}

unsigned int
Constraint::
type_from_std_string( const string& type ){
  for( unsigned int i = 0; i < NUM_OBJECT_TYPES; i++ ){
    if( type == Constraint::type_to_std_string( i ) ){
      return i;
    }
  }
  return OBJECT_TYPE_UNKNOWN;
}

void
Constraint::
to_xml( const string& filename )const{
  return;
}

void
Constraint::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "constraint" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "type" ), ( const xmlChar* )( Constraint::type_to_std_string( _type ).c_str() ) );
  xmlNodePtr parent = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "parent" ), NULL );
  _parent.to_xml( doc, parent );
  xmlAddChild( node, parent );
  xmlNodePtr child = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "child" ), NULL );
  _child.to_xml( doc, child );
  xmlAddChild( node, child );
  xmlAddChild( root, node );
  return;
}

void
Constraint::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "constraint" ) ) == 0 ){
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
Constraint::
from_xml( xmlNodePtr root ){
  _type = CONSTRAINT_TYPE_UNKNOWN;
  _parent = Region();
  _child = Region();
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "type" ) );
    if( tmp != NULL ){
      string type_string = ( char* )( tmp );
      _type = Constraint::type_from_std_string( type_string );
      xmlFree( tmp );
    }
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "parent" ) ) == 0 ){
          xmlNodePtr l2 = NULL;
          for( l2 = l1->children; l2; l2 = l2->next ){
            if( l2->type == XML_ELEMENT_NODE ){
              if( xmlStrcmp( l2->name, ( const xmlChar* )( "region" ) ) == 0 ){
                _parent.from_xml( l2 );
              }
            }
          }
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "child" ) ) == 0 ){
          xmlNodePtr l2 = NULL;
          for( l2 = l1->children; l2; l2 = l2->next ){
            if( l2->type == XML_ELEMENT_NODE ){
              if( xmlStrcmp( l2->name, ( const xmlChar* )( "region" ) ) == 0 ){
                _child.from_xml( l2 ); 
              }
            }
          }
        } 
      }
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Constraint& other ) {
    out << "type:\"" << Constraint::type_to_std_string( other.type() ) << "\" ";
    out << "parent:(" << other.parent() << ") ";
    out << "child:(" << other.child() << ")";
    return out;
  }
}
