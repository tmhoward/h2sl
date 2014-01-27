/**
 * @file    world.cc
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
 * The implementation of a class used to represent the world
 */

#include <sstream>
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

World::
World( const unsigned long long& time,
        const Object_Set& objectSet ) : _time( time ),
                                              _object_set( objectSet ) {

}

World::
~World() {

}

World::
World( const World& other ) : _time( other._time ),
                              _object_set( other._object_set ){

}

World&
World::
operator=( const World& other ) {
  _time = other._time;
  _object_set = other._object_set;
  return (*this);
}

void
World::
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
World::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "world" ), NULL );
  stringstream time_string;
  time_string << _time;
  xmlNewProp( node, ( const xmlChar* )( "time" ), ( const xmlChar* )( time_string.str().c_str() ) );
  _object_set.to_xml( doc, node );
  xmlAddChild( root, node );
  return;
}

void
World::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "world" ) ) == 0 ){
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
World::
from_xml( xmlNodePtr root ){
  _object_set.clear();
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "time" ) );
    if( tmp != NULL ){
      string time_string = ( char* )( tmp );
      _time = strtol( time_string.c_str(), NULL, 10 );
      xmlFree( tmp );
    }
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "object_set" ) ) == 0 ){
          _object_set.from_xml( l1 );
        }
      }
    }
  }
  return;
}


namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const World& other ) {
    out << "time:\"" << other.time() << "\" ";
    out << "object_set[" << other.object_set().objects().size() << "]:{";
    for( unsigned int i = 0; i < other.object_set().objects().size(); i++ ){
      out << "(" << other.object_set().objects()[ i ] << ")";
      if( i != ( other.object_set().objects().size() - 1 ) ){
        out << ",";
      }
    }
    out << "}";
    return out;
  }

}
