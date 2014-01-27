/**
 * @file    object.cc
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
 * The implementation of a class used to describe an object
 */

#include "h2sl/object.h"

using namespace std;
using namespace h2sl;

Object::
Object( const string& name,
        const unsigned int& type ) : Grounding(),   
                                    _name( name ),
                                    _type( type ) {

}

Object::
Object( const string& name,
        const object_type_t& type ) : Grounding(),
                                      _name( name ),
                                      _type( type ) {

}

Object::
~Object() {

}

Object::
Object( const Object& other ) : Grounding( other ),
                                _name( other._name ),
                                _type( other._type ) {

}

Object&
Object::
operator=( const Object& other ) {
  _name = other._name;
  _type = other._type;
  return (*this);
}

bool
Object::
operator==( const Object& other )const{
  if( _name != other._name ){
    return false;
  } else if ( _type != other._type ){
    return false;
  } else {
    return true;
  }   
}

bool
Object::
operator!=( const Object& other )const{
  return !( *this == other );
} 
 
Grounding*
Object::
dup( void )const{
  return new Object( *this );
}
 
string
Object::
type_to_std_string( const unsigned int& type ){
  switch( type ){
  case( OBJECT_TYPE_ROBOT ):
    return "robot";
    break;
  case( OBJECT_TYPE_BOX ):
    return "box";
    break;
  case( OBJECT_TYPE_TABLE ):
    return "table";
    break;
  case( OBJECT_TYPE_CHAIR ):
    return "chair";
    break;
  case( OBJECT_TYPE_COUCH ):
    return "couch";
    break;
  case( OBJECT_TYPE_SINK ):
    return "sink";
    break;
  case( OBJECT_TYPE_MUG ):
    return "mug";
    break;
  case( OBJECT_TYPE_BOTTLE ):
    return "bottle";
    break;
  case( OBJECT_TYPE_CAN ):
    return "can";
    break;
  case( OBJECT_TYPE_FRIDGE ):
    return "fridge";
    break;
  case( OBJECT_TYPE_HAMMER ):
    return "hammer";
    break;
  case( OBJECT_TYPE_SCREWDRIVER ):
    return "screwdriver";
    break;
  case( OBJECT_TYPE_PIPE ):
    return "pipe";
    break;
  case( OBJECT_TYPE_NEWSPAPER ):
    return "newspaper";
    break;
  case( OBJECT_TYPE_MARKER ):
    return "marker";
    break;
  case( OBJECT_TYPE_TUBE ):
    return "tube";
    break;
  case( OBJECT_TYPE_TAPE ):
    return "tape";
    break;
  case( OBJECT_TYPE_BONE ):
    return "bone";
    break;
  case( OBJECT_TYPE_KITCHEN ):
    return "kitchen";
    break;
  case( OBJECT_TYPE_BATHROOM ):
    return "bathroom";
    break;
  case( OBJECT_TYPE_PANTRY ):
    return "pantry";
    break;
  case( OBJECT_TYPE_BUILDING ):
    return "building";
    break;
  case( OBJECT_TYPE_LIVINGROOM ):
    return "livingroom";
    break;
  case( OBJECT_TYPE_BEDROOM ):
    return "bedroom";
    break;
  case( OBJECT_TYPE_STUDY ):
    return "STUDY";
    break;
  case( OBJECT_TYPE_HALLWAY ):
    return "STUDY";
    break;
  default:
    return "na";
    break;
  }
}

unsigned int
Object::
type_from_std_string( const string& type ){
  for( unsigned int i = 0; i < NUM_OBJECT_TYPES; i++ ){
    if( type == Object::type_to_std_string( i ) ){
      return i;
    }
  }
  return OBJECT_TYPE_UNKNOWN;
}

void
Object::
to_xml( const string& filename )const{
  return;
}

void
Object::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "object" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "name" ), ( const xmlChar* )( _name.c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "type" ), ( const xmlChar* )( Object::type_to_std_string( _type ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Object::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
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
Object::
from_xml( xmlNodePtr root ){
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "name" ) );
    if( tmp != NULL ){
      _name = ( char* )( tmp );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "type" ) );
    if( tmp != NULL ){
      string type_string = ( char* )( tmp );
      _type = Object::type_from_std_string( type_string );
      xmlFree( tmp );
    }
  }
  return;
}


namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Object& other ) {
    out << "name:\"" << other.name() << "\" type:\"" << Object::type_to_std_string( other.type() ) << "\"";
    return out;
  }
}
