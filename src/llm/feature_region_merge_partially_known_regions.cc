/**
 * @file    feature_region_merge_partially_known_regions.cc
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
#include "h2sl/feature_region_merge_partially_known_regions.h"

using namespace std;
using namespace h2sl;

Feature_Region_Merge_Partially_Known_Regions::
Feature_Region_Merge_Partially_Known_Regions( const bool& invert ) : Feature( invert ) {

}

Feature_Region_Merge_Partially_Known_Regions::
~Feature_Region_Merge_Partially_Known_Regions() {

}

Feature_Region_Merge_Partially_Known_Regions::
Feature_Region_Merge_Partially_Known_Regions( const Feature_Region_Merge_Partially_Known_Regions& other ) : Feature( other ) {

}

Feature_Region_Merge_Partially_Known_Regions&
Feature_Region_Merge_Partially_Known_Regions::
operator=( const Feature_Region_Merge_Partially_Known_Regions& other ) {
  _invert = other._invert;
  return (*this);
}

bool
Feature_Region_Merge_Partially_Known_Regions::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< Grounding* >& children,
        const Phrase* phrase ){
  const Region * region = dynamic_cast< const Region* >( grounding );
  if( region != NULL ){
    std::vector< const Region* > known_region_type_and_unknown_object_type;
    std::vector< const Region* > known_object_type_and_unknown_region_type;
    for( unsigned int i = 0; i < children.size(); i++ ){
      const Region * child = dynamic_cast< const Region* >( children[ i ] );
      if( child != NULL ){
        if( ( child->object().type() != OBJECT_TYPE_UNKNOWN ) && ( child->type() == REGION_TYPE_UNKNOWN ) ){
          known_object_type_and_unknown_region_type.push_back( child );
        } else if( ( child->object().type() == OBJECT_TYPE_UNKNOWN ) && ( child->type() != REGION_TYPE_UNKNOWN ) ){
          known_region_type_and_unknown_object_type.push_back( child );
        }
      }
    }
    for( unsigned int i = 0; i < known_region_type_and_unknown_object_type.size(); i++ ){
      for( unsigned int j = 0; j < known_object_type_and_unknown_region_type.size(); j++ ){
        if( ( region->type() == known_region_type_and_unknown_object_type[ i ]->type() ) && ( region->object().type() == known_object_type_and_unknown_region_type[ j ]->object().type() ) ){
          return !_invert;
        }
      }
    }   
       
    return _invert;
  }
  return false;
}

void
Feature_Region_Merge_Partially_Known_Regions::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_region_merge_partially_known_regions" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Region_Merge_Partially_Known_Regions::
from_xml( xmlNodePtr root ){
  _invert = false;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Region_Merge_Partially_Known_Regions& other ) {
    return out;
  }

}
