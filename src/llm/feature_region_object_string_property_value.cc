/**
 * @file    feature_region_object_string_property_value.cc
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
 * The implementation of a class to check for a region object's property
 */

#include <sstream>

#include "h2sl/region.h"
#include "h2sl/feature_region_object_string_property_value.h"

using namespace std;
using namespace h2sl;

Feature_Region_Object_String_Property_Value::
Feature_Region_Object_String_Property_Value( const bool& invert,
                        const string& key,
                        const string& symbol ) : Feature_Grounding_String_Property_Value( invert, "object", key, symbol ) {

}

Feature_Region_Object_String_Property_Value::
Feature_Region_Object_String_Property_Value( xmlNodePtr root ) : Feature_Grounding_String_Property_Value( root ) {

}

Feature_Region_Object_String_Property_Value::
~Feature_Region_Object_String_Property_Value() {

}

Feature_Region_Object_String_Property_Value::
Feature_Region_Object_String_Property_Value( const Feature_Region_Object_String_Property_Value& other ) : Feature_Grounding_String_Property_Value( other ) {

}

Feature_Region_Object_String_Property_Value&
Feature_Region_Object_String_Property_Value::
operator=( const Feature_Region_Object_String_Property_Value& other ) {
  _invert = other._invert;
  _key = other._key;
  _symbol = other._symbol;
  return (*this);
}

bool
Feature_Region_Object_String_Property_Value::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Region_Object_String_Property_Value::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Region * region = dynamic_cast< const Region* >( grounding );
  if( region != NULL ){
    map< string, Object* >::const_iterator it_region_object = world->objects().find( region->object_id() );
    if( it_region_object == world->objects().end() ){
      cout << "cannot find object \"" << region->object_id() << "\"" << endl;
      assert( false );
    }
    //assert( it_region_object != world->objects().end() );
    map< std::string, std::string >::const_iterator it = it_region_object->second->string_properties().find( _key );
    if( it != it_region_object->second->string_properties().end() ){
      if( it->second == _symbol ){
        return !_invert;
      } else {
        return _invert;
      } 
    }
  }
  return false;
}

void
Feature_Region_Object_String_Property_Value::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_region_object_string_property_value" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "class_name" ), ( const xmlChar* )( _class_name.c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "key" ), ( const xmlChar* )( _key.c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "symbol" ), ( const xmlChar* )( _symbol.c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Region_Object_String_Property_Value& other ) {
    out << "Feature_Region_Object_String_Property_Value:(invert:(" << other.invert() << ") key:(" << other.key() << ") symbol:(" << other.symbol() << "))";
    return out;
  }

}
