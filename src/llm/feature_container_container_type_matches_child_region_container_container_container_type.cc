/**
 * @file    feature_container_container_type_matches_child_region_container_container_container_type.cc
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

#include "h2sl/region_container.h"
#include "h2sl/feature_container_container_type_matches_child_region_container_container_container_type.h"

using namespace std;
using namespace h2sl;

Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type::
Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type( const bool& invert ) : Feature( invert ) {

}

Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type::
Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type( xmlNodePtr root ) : Feature() {
  from_xml( root );
}

Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type::
~Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type() {

}

Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type::
Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type( const Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type& other ) : Feature( other ) {

}

Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type&
Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type::
operator=( const Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type& other ) {
  _invert = other._invert;
  return (*this);
}

bool
Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Container * container = dynamic_cast< const Container* >( grounding );
  if( container != NULL ){
    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        const Region_Container * child = dynamic_cast< const Region_Container* >( children[ i ].second[ j ] );
        if( child != NULL ){
          if( container->type() == child->container().type() ){
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
Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_container_type_matches_child_region_container_container_container_type" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type::
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
              const Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type& other ) {
    out << "Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type:(invert:(" << other.invert() << "))";
    return out;
  }

}
