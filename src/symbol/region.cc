/**
 * @file    region.cc
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
 * The implementation of a class used to describe a region of space
 */

#include "h2sl/region.h"

using namespace std;
using namespace h2sl;

Region::
Region( const unsigned int& type,
        const Object& object ) : Grounding(),
                                  _type( type ),
                                  _object( object ) {

}

Region::
Region( const region_type_t& type,
        const Object& object ) : Grounding(),
                                  _type( type ),
                                  _object( object ){

}

Region::
~Region() {

}

Region::
Region( const Region& other ) : Grounding( other ),
                                _type( other._type ),
                                _object( other._object ){

}

Region&
Region::
operator=( const Region& other ) {
  _type = other._type;
  _object = other._object;
  return (*this);
}

bool
Region::
operator==( const Region& other )const{
  if( _type != other._type ){
    return false;
  } if( _object != other._object ){
    return false;
  } else {
    return true;
  }
}

bool
Region::
operator!=( const Region& other )const{
  return !( *this == other );
}

Grounding*
Region::
dup( void )const{
  return new Region( *this );
}

string
Region::
type_to_std_string( const unsigned int& type ){
  switch( type ){
  case( REGION_TYPE_NEAR ):
    return "near";
    break;
  case( REGION_TYPE_FAR ):
    return "far";
    break;
  case( REGION_TYPE_LEFT ):
    return "left";
    break;
  case( REGION_TYPE_RIGHT ):
    return "right";
    break;
  case( REGION_TYPE_FRONT ):
    return "front";
    break;
  case( REGION_TYPE_BACK ):
    return "back";
    break;
  case( REGION_TYPE_ABOVE ):
    return "above";
    break;
  case( REGION_TYPE_BELOW ):
    return "below";
    break;
  case( REGION_TYPE_UNKNOWN ):
  default:
    return "na";
  }
}

unsigned int
Region::
type_from_std_string( const string& type ){
  for( unsigned int i = 0; i < NUM_OBJECT_TYPES; i++ ){
    if( type == Region::type_to_std_string( i ) ){
      return i;
    }
  }
  return OBJECT_TYPE_UNKNOWN;
}

void
Region::
to_xml( const string& filename )const{
  return;
}

void
Region::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "region" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "type" ), ( const xmlChar* )( Region::type_to_std_string( _type ).c_str() ) );
  _object.to_xml( doc, node );
  xmlAddChild( root, node );
  return;
}

void
Region::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "region" ) ) == 0 ){
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
Region::
from_xml( xmlNodePtr root ){
  _type = REGION_TYPE_UNKNOWN;
  _object = Object();
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "type" ) );
    if( tmp != NULL ){
      string type_string = ( char* )( tmp );
      _type = Region::type_from_std_string( type_string );
      xmlFree( tmp );
    }
    xmlNodePtr l1 = NULL; 
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
          _object.from_xml( l1 );
        }
      }
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Region& other ) {
    out << "type:\"" << Region::type_to_std_string( other.type() ) << "\" ";
    out << "object:(" << other.object() << ")";
    return out;
  }
}
