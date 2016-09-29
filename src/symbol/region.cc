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
Region( const string& regionType,
        const Object& object ) : Grounding(),
                                  _object( object ) {
  insert_prop< std::string >( _properties, "region_type", regionType );
}

Region::
Region( xmlNodePtr root ) : Grounding(),
                            _object() {
  insert_prop< std::string >( _properties, "region_type", "na" );
  from_xml( root );
}

Region::
~Region() {

}

Region::
Region( const Region& other ) : Grounding( other ),
                                _object( other._object ){

}

Region&
Region::
operator=( const Region& other ) {
  _properties = other._properties;
  _object = other._object;
  return (*this);
}

bool
Region::
operator==( const Region& other )const{
  if( region_type() != other.region_type() ){
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

void
Region::
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
Region::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "region" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "region_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "region_type" ).c_str() ) );
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
  region_type() = "na";
  _object = Object();
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > region_type_prop = has_prop< std::string >( root, "region_type" );
    if( region_type_prop.first ){
      region_type() = region_type_prop.second;
    }
    for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
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
    out << "Region(";
    out << "region_type=\"" << other.region_type() << "\",";
    out << "object=" << other.object();
    out << ")";
    return out;
  }
}
