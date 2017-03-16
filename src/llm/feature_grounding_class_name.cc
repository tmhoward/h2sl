/**
 * @file    feature_grounding_class_name.cc
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
 * The implementation of a class to check a grounding's property
 */

#include <sstream>

#include "h2sl/region.h"
#include "h2sl/feature_grounding_class_name.h"

using namespace std;
using namespace h2sl;

Feature_Grounding_Class_Name::
Feature_Grounding_Class_Name( const bool& invert,
                        const string& className ) : Feature( invert ),
                                                        _class_name( className ) {

}

Feature_Grounding_Class_Name::
Feature_Grounding_Class_Name( xmlNodePtr root ) : Feature(),
                                                      _class_name( "na" ) {
  from_xml( root );
}

Feature_Grounding_Class_Name::
~Feature_Grounding_Class_Name() {

}

Feature_Grounding_Class_Name::
Feature_Grounding_Class_Name( const Feature_Grounding_Class_Name& other ) : Feature( other ),
                                                                _class_name( other._class_name ) {

}

Feature_Grounding_Class_Name&
Feature_Grounding_Class_Name::
operator=( const Feature_Grounding_Class_Name& other ) {
  _invert = other._invert;
  _class_name = other._class_name;
  return (*this);
}

bool
Feature_Grounding_Class_Name::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Grounding_Class_Name::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  if( grounding != NULL ){
    if( grounding->matches_class_name( _class_name ) ){
      return !_invert;
    } else {
      return _invert;
    }
  }
  return false;
}

void
Feature_Grounding_Class_Name::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_grounding_class_name" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "class_name" ), ( const xmlChar* )( _class_name.c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Grounding_Class_Name::
from_xml( xmlNodePtr root ){
  _invert = false;
  _class_name = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "class_name" };
    assert( check_keys( root, feature_keys ) );
  
    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }
    pair< bool, std::string > class_name_prop = has_prop< std::string >( root, "class_name" );
    assert( class_name_prop.first );
    _class_name = class_name_prop.second;
  }
  return;
}

std::string&
Feature_Grounding_Class_Name::
class_name( void ){
  return _class_name;
};

const std::string&
Feature_Grounding_Class_Name::
class_name( void )const{
  return _class_name;
};

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Grounding_Class_Name& other ) {
    out << "Feature_Grounding_Class_Name:(invert:(" << other.invert() << ") class:(" << other.class_name() << ")";
    return out;
  }
}
