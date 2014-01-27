/**
 * @file    feature_region.cc
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
 * The implementation of a class used to check for a match with a region's type
 */

#include <sstream>

#include "h2sl/region.h"
#include "h2sl/feature_region.h"

using namespace std;
using namespace h2sl;

Feature_Region::
Feature_Region( const bool& invert,
                const unsigned int& regionType ) : Feature( invert ),
                                                    _region_type( regionType ) {

}

Feature_Region::
~Feature_Region() {

}

Feature_Region::
Feature_Region( const Feature_Region& other ) : Feature( other ),
                                                _region_type( other._region_type ) {

}

Feature_Region&
Feature_Region::
operator=( const Feature_Region& other ) {
  _invert = other._invert;
  _region_type = other._region_type;
  return (*this);
}

bool
Feature_Region::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< Grounding* >& children,
        const Phrase* phrase ){
  const Region * region = dynamic_cast< const Region* >( grounding );
  if( region != NULL ){
    if( region->type() == _region_type ){
      return !_invert;
    }
  }
  return false;
}

void
Feature_Region::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_region" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  stringstream region_type_string;
  region_type_string << _region_type;
  xmlNewProp( node, ( const xmlChar* )( "region_type" ), ( const xmlChar* )( region_type_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Region::
from_xml( xmlNodePtr root ){
  _invert = false;
  _region_type = 0;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "region_type" ) );
    if( tmp != NULL ){
      string region_type_string = ( char* )( tmp );
      _region_type = strtol( region_type_string.c_str(), NULL, 10 );
      xmlFree( tmp );
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Region& other ) {
    return out;
  }

}
