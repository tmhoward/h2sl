/**
 * @file    feature_region_object_matches_child.cc
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

#include "h2sl/constraint.h"
#include "h2sl/region.h"
#include "h2sl/feature_region_object_matches_child.h"

using namespace std;
using namespace h2sl;

Feature_Region_Object_Matches_Child::
Feature_Region_Object_Matches_Child( const bool& invert ) : Feature( invert ) {

}

Feature_Region_Object_Matches_Child::
Feature_Region_Object_Matches_Child( xmlNodePtr root ) : Feature() {
  from_xml( root );
}

Feature_Region_Object_Matches_Child::
~Feature_Region_Object_Matches_Child() {

}

Feature_Region_Object_Matches_Child::
Feature_Region_Object_Matches_Child( const Feature_Region_Object_Matches_Child& other ) : Feature( other ) {

}

Feature_Region_Object_Matches_Child&
Feature_Region_Object_Matches_Child::
operator=( const Feature_Region_Object_Matches_Child& other ) {
  _invert = other._invert;
  return (*this);
}

bool
Feature_Region_Object_Matches_Child::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Region_Object_Matches_Child::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Region * region = dynamic_cast< const Region* >( grounding );
  if( region != NULL ){
    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        const Object * child = dynamic_cast< const Object* >( children[ i ].second[ j ] );
        if( child != NULL ){
          if( region->object_id() == child->id() ){
            return !_invert;
          }
        }
      }   
    }
    return _invert;
  }
  return false;
}

void
Feature_Region_Object_Matches_Child::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_region_object_matches_child" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Region_Object_Matches_Child::
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
              const Feature_Region_Object_Matches_Child& other ) {
    out << "Feature_Region_Object_Matches_Child:(invert:(" << other.invert() << "))";
    return out;
  }

}
