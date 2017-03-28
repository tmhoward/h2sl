/**
 * @file    feature_constraint_reference_type.cc
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
#include "h2sl/feature_constraint_reference_type.h"

using namespace std;
using namespace h2sl;

Feature_Constraint_Reference_Type::
Feature_Constraint_Reference_Type( const bool& invert,
                                    const string& referenceType ) : Feature( invert ),
                                                                    _reference_type( referenceType ) {

}

Feature_Constraint_Reference_Type::
~Feature_Constraint_Reference_Type() {

}

Feature_Constraint_Reference_Type::
Feature_Constraint_Reference_Type( const Feature_Constraint_Reference_Type& other ) : Feature( other ), 
                                                                                      _reference_type( other._reference_type ) {

}

Feature_Constraint_Reference_Type&
Feature_Constraint_Reference_Type::
operator=( const Feature_Constraint_Reference_Type& other ) {
  _invert = other._invert;
  _reference_type = other._reference_type;
  return (*this);
}

bool
Feature_Constraint_Reference_Type::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Constraint_Reference_Type::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Constraint * constraint = dynamic_cast< const Constraint* >( grounding );
  if( constraint != NULL ){
    for( map< string, Object* >::const_iterator it_world_object = world->objects().begin(); it_world_object != world->objects().end(); it_world_object++ ){
      if( it_world_object->second != NULL ){
        if( it_world_object->second->id() == constraint->reference() ){
          if( it_world_object->second->type() == _reference_type ){
            return !_invert;
          } else {
            return _invert;
          }
        }
      }
    }
  }
  return false;
}

void
Feature_Constraint_Reference_Type::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_constraint_reference_type" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "reference_type" ), ( const xmlChar* )( _reference_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Constraint_Reference_Type::
from_xml( xmlNodePtr root ){
  _invert = false;
  _reference_type = "na";
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ){
      _invert = invert_prop.second;
    }
    pair< bool, string > reference_type_prop = has_prop< std::string >( root, "reference_type" );
    if( reference_type_prop.first ){
      _reference_type = reference_type_prop.second;
    }
  }
  return;
}

string&
Feature_Constraint_Reference_Type::
reference_type( void ){
  return _reference_type;
}

const string&
Feature_Constraint_Reference_Type::
reference_type( void )const{
  return _reference_type;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Constraint_Reference_Type& other ) {
    out << "Feature_Constraint_Reference_Type:(invert:(" << other.invert() << "),reference_type=\"" << other.reference_type() << "\")";
    return out;
  }

}
