/**
 * @file    region_set.cc
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
 * The implementation of a class used to describe a set of regions
 */

#include "h2sl/region_set.h"

using namespace std;
using namespace h2sl;

Region_Set::
Region_Set( const vector< Region >& regions ) : Grounding(),
                                                _regions( regions ) {

}

Region_Set::
~Region_Set() {

}

Region_Set::
Region_Set( const Region_Set& other ) : Grounding( other ),
                                        _regions( other._regions ){

}

Region_Set&
Region_Set::
operator=( const Region_Set& other ) {
  _regions = other._regions;
  return (*this);
}

Grounding*
Region_Set::
dup( void )const{
  return new Region_Set( *this );
}

void
Region_Set::
clear( void ){ 
  _regions.clear();
  return;
}

void
Region_Set::
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
Region_Set::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "region_set" ), NULL );
  for( unsigned int i = 0; i < _regions.size(); i++ ){
    _regions[ i ].to_xml( doc, node );
  }
  xmlAddChild( root, node );
  return;
}

void
Region_Set::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "region_set" ) ) == 0 ){
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
Region_Set::
from_xml( xmlNodePtr root ){
  _regions.clear();
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "region" ) ) == 0 ){
          _regions.push_back( Region() );
          _regions.back().from_xml( l1 );
        }
      }
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Region_Set& other ) {
    out << "regions[" << other.regions().size() << "]:{";
    for( unsigned int i = 0; i < other.regions().size(); i++ ){
      out << "(" << other.regions()[ i ] << ")";
      if( i != ( other.regions().size() - 1 ) ){
        out << ",";
      }
    }
    out << "}";
    return out;
  }

}
