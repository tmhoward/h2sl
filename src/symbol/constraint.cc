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
Constraint( const string& constraintType,
            const Region& parent,
            const Region& child ) : Grounding(),
                                    _parent( parent ),
                                    _child( child ) {
  insert_prop< std::string >( _properties, "constraint_type", constraintType );
}

Constraint::
Constraint( xmlNodePtr root ) : Grounding(),
                                _parent(),
                                _child() {
  insert_prop< std::string >( _properties, "constraint_type", "na" );
  from_xml( root );
}

Constraint::
~Constraint() {

}

Constraint::
Constraint( const Constraint& other ) : Grounding( other ),
                                        _parent( other._parent ),
                                        _child( other._child ){

}

Constraint&
Constraint::
operator=( const Constraint& other ) {
  _properties = other._properties;
  _parent = other._parent;
  _child = other._child;
  return (*this);
}

bool
Constraint::
operator==( const Constraint& other )const{
  if( constraint_type() != other.constraint_type() ){
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

void
Constraint::
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
Constraint::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "constraint" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "constraint_type" ), ( const xmlChar* )( constraint_type().c_str() ) );
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
  constraint_type() = "na";
  _parent = Region();
  _child = Region();
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > constraint_type_prop = has_prop< std::string >( root, "constraint_type" );
    if( constraint_type_prop.first ){
      constraint_type() = constraint_type_prop.second;
    }
    pair< bool, string > type_prop = has_prop< std::string >( root, "type" );
    if( type_prop.first ){
      constraint_type() = type_prop.second;
    }
    for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
      if( matches_name( l1, "parent" ) ) {
        for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
          if( matches_name( l2, "region" ) ){
            _parent.from_xml( l2 );
          }
        } 
      } else if( matches_name( l1, "child" ) ) {
        for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
          if( matches_name( l2, "region" ) ){
            _child.from_xml( l2 );
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
    out << "Constraint(";
    out << "type=\"" << other.constraint_type() << "\",";
    out << "parent=" << other.parent() << ",";
    out << "child=" << other.child();
    out << ")";
    return out;
  }
}
