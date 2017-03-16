/**
 * @file    feature_grounding_string_property_value.cc
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
#include "h2sl/feature_grounding_string_property_value.h"

using namespace std;
using namespace h2sl;

Feature_Grounding_String_Property_Value::
Feature_Grounding_String_Property_Value( const bool& invert,
                        const string& className,
                        const string& key,
                        const string& symbol ) : Feature( invert ),
                                                        _class_name( className ),
                                                        _key( key ),
                                                        _symbol( symbol ) {

}

Feature_Grounding_String_Property_Value::
Feature_Grounding_String_Property_Value( xmlNodePtr root ) : Feature(),
                                                      _class_name( "na" ),
                                                      _key( "na" ),
                                                      _symbol( "na" ) {
  from_xml( root );
}

Feature_Grounding_String_Property_Value::
~Feature_Grounding_String_Property_Value() {

}

Feature_Grounding_String_Property_Value::
Feature_Grounding_String_Property_Value( const Feature_Grounding_String_Property_Value& other ) : Feature( other ),
                                                                _class_name( other._class_name ),
                                                                _key( other._key ),
                                                                _symbol( other._symbol ) {

}

Feature_Grounding_String_Property_Value&
Feature_Grounding_String_Property_Value::
operator=( const Feature_Grounding_String_Property_Value& other ) {
  _invert = other._invert;
  _class_name = other._class_name;
  _key = other._key;
  _symbol = other._symbol;
  return (*this);
}

bool
Feature_Grounding_String_Property_Value::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Grounding_String_Property_Value::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  if( grounding != NULL ){
    if( grounding->matches_class_name( _class_name ) ){
      map< std::string, std::string >::const_iterator it = grounding->string_properties().find( _key );
      if( it != grounding->string_properties().end() ){
        if( it->second == _symbol ){
          return !_invert;
        } else {
          return _invert;
        }
      } 
    }
  }
  return false;
}

void
Feature_Grounding_String_Property_Value::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_grounding_string_property_value" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "class_name" ), ( const xmlChar* )( _class_name.c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "key" ), ( const xmlChar* )( _key.c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "symbol" ), ( const xmlChar* )( _symbol.c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Grounding_String_Property_Value::
from_xml( xmlNodePtr root ){
  _invert = false;
  _class_name = "na";
  _key = "na";
  _symbol = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "class_name", "key", "symbol" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }

    pair< bool, std::string > key_prop = has_prop< std::string >( root, "key" );
    if( key_prop.first ) {
      _key = key_prop.second;
    }

    pair< bool, std::string > symbol_prop = has_prop< std::string >( root, "symbol" );
    if( symbol_prop.first ) {
      _symbol = symbol_prop.second;
    }

    pair< bool, std::string > class_name_prop = has_prop< std::string >( root, "class_name" );
    if( !class_name_prop.first ){
      cout << *this << endl;
    }
    assert( class_name_prop.first );
    _class_name = class_name_prop.second;
  }
  return;
}

std::string&
Feature_Grounding_String_Property_Value::
class_name( void ){
  return _class_name;
};

const std::string&
Feature_Grounding_String_Property_Value::
class_name( void )const{
  return _class_name;
};

std::string& 
Feature_Grounding_String_Property_Value::
key( void ){ 
  return _key; 
};

const std::string& 
Feature_Grounding_String_Property_Value::
key( void )const{ 
  return _key; 
};

std::string&
Feature_Grounding_String_Property_Value::
symbol( void ){ 
return _symbol;
};

const std::string& 
Feature_Grounding_String_Property_Value::
symbol( void )const{ 
  return _symbol; 
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Grounding_String_Property_Value& other ) {
    out << "Feature_Grounding_String_Property_Value:(invert:(" << other.invert() << ") class:(" << other.class_name() << ") key:(" << other.key() << ") symbol:(" << other.symbol() << "))";
    return out;
  }
}
