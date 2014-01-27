/**
 * @file    feature_object.cc
 * @author  Thomas M. Howard (tmhoward@csail.mit.edu)
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
 * The implementation of a class that checks for a object
 */

#include <sstream>

#include "h2sl/object.h"
#include "h2sl/feature_object.h"

using namespace std;
using namespace h2sl;

Feature_Object::
Feature_Object( const bool& invert,
                const unsigned int& objectType ) : Feature( invert ),
                                                    _object_type( objectType ) {

}

Feature_Object::
~Feature_Object() {

}

Feature_Object::
Feature_Object( const Feature_Object& other ) : Feature( other ),
                                                _object_type( other._object_type ){

}

Feature_Object&
Feature_Object::
operator=( const Feature_Object& other ) {
  _invert = other._invert;
  _object_type = other._object_type;
  return (*this);
}

bool
Feature_Object::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< Grounding* >& children,
        const Phrase* phrase ){
  const Object * object = dynamic_cast< const Object* >( grounding );
  if( object != NULL ){
    if( object->type() == _object_type ){
      return !_invert;
    }
  }
  return false;
}

void
Feature_Object::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  stringstream object_type_string;
  object_type_string << _object_type;
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( object_type_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Object::
from_xml( xmlNodePtr root ){
  _invert = false;
  _object_type = 0;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "object_type" ) );
    if( tmp != NULL ){
      string object_type_string = ( char* )( tmp );
      _object_type = strtol( object_type_string.c_str(), NULL, 10 );
      xmlFree( tmp );
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Object& other ) {
    return out;
  }

}
