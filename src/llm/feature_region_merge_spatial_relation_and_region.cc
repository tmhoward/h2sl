/**
 * @file    feature_region_merge_spatial_relation_and_region.cc
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
#include "h2sl/spatial_relation.h"
#include "h2sl/object.h"
#include "h2sl/feature_region_merge_spatial_relation_and_region.h"

using namespace std;
using namespace h2sl;

Feature_Region_Merge_Spatial_Relation_And_Region::
Feature_Region_Merge_Spatial_Relation_And_Region( const bool& invert ) : Feature( invert ) {

}

Feature_Region_Merge_Spatial_Relation_And_Region::
Feature_Region_Merge_Spatial_Relation_And_Region( xmlNodePtr root ) : Feature() {
  from_xml( root );
}


Feature_Region_Merge_Spatial_Relation_And_Region::
~Feature_Region_Merge_Spatial_Relation_And_Region() {

}

Feature_Region_Merge_Spatial_Relation_And_Region::
Feature_Region_Merge_Spatial_Relation_And_Region( const Feature_Region_Merge_Spatial_Relation_And_Region& other ) : Feature( other ) {

}

Feature_Region_Merge_Spatial_Relation_And_Region&
Feature_Region_Merge_Spatial_Relation_And_Region::
operator=( const Feature_Region_Merge_Spatial_Relation_And_Region& other ) {
  _invert = other._invert;
  return (*this);
}

bool
Feature_Region_Merge_Spatial_Relation_And_Region::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Region_Merge_Spatial_Relation_And_Region::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Region * region = dynamic_cast< const Region* >( grounding );
  if( region != NULL ){
    vector< const Spatial_Relation* > child_spatial_relations = find_in_vector_of_pairs< Phrase, Grounding, Spatial_Relation >( children );
    vector< const Region* > child_regions = find_in_vector_of_pairs< Phrase, Grounding, Region >( children );

    if( ( !child_spatial_relations.empty() ) && ( !child_regions.empty() ) ){
      for( vector< const Spatial_Relation* >::const_iterator it_child_spatial_relation = child_spatial_relations.begin(); it_child_spatial_relation != child_spatial_relations.end(); it_child_spatial_relation++ ){
        for( vector< const Region* >::const_iterator it_child_region = child_regions.begin(); it_child_region != child_regions.end(); it_child_region++ ){
          if( region->spatial_relation_type() == (*it_child_spatial_relation)->spatial_relation_type() ){
            if( region->object_id() == (*it_child_region)->object_id() ){
              return !_invert;
            } 
          }
        }
      }
      return _invert;
    }
  }
  return false;
}

void
Feature_Region_Merge_Spatial_Relation_And_Region::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_region_merge_spatial_relation_and_region" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Region_Merge_Spatial_Relation_And_Region::
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
              const Feature_Region_Merge_Spatial_Relation_And_Region& other ) {
    out << "Feature_Region_Merge_Spatial_Relation_And_Region:(invert:(" << other.invert() << "))";
    return out;
  }

}
