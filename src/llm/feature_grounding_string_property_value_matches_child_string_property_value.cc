/**
 * @file    feature_grounding_string_property_value_matches_child_string_property_value.cc
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
#include "h2sl/feature_grounding_string_property_value_matches_child_string_property_value.h"

using namespace std;
using namespace h2sl;

Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( const bool& invert,
                        const string& classNameGrounding,
                        const string& keyGrounding,
                        const string& classNameChild,
                        const string& keyChild ) : Feature( invert ),
                                                        _class_name_grounding( classNameGrounding ),
                                                        _key_grounding( keyGrounding ),
                                                        _class_name_child( classNameChild ),
                                                        _key_child( keyChild ) {

}

Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( xmlNodePtr root ) : Feature(),
                                                      _class_name_grounding( "na" ),
                                                      _key_grounding( "na" ),
                                                      _class_name_child( "na" ),
                                                      _key_child( "na" ) {
  from_xml( root );
}

Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
~Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value() {

}

Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( const Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value& other ) : Feature( other ),
                                                                _class_name_grounding( other._class_name_grounding ),
                                                                _key_grounding( other._key_grounding ),
                                                                _class_name_child( other._class_name_child ),
                                                                _key_child( other._key_child ) {

}

Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value&
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
operator=( const Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value& other ) {
  _invert = other._invert;
  _class_name_grounding = other._class_name_grounding;
  _key_grounding = other._key_grounding;
  _class_name_child = other._class_name_child;
  _key_child = other._key_child;
  return (*this);
}

bool
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  if( grounding != NULL ){
    if( grounding->matches_class_name( _class_name_grounding ) ){
      map< std::string, std::string >::const_iterator it_grounding = grounding->string_properties().find( _key_grounding );
      if( it_grounding == grounding->string_properties().end() ){
        cout << "could not find \"" << _key_grounding << "\" in class " << _class_name_grounding << endl;
        assert( false );
      }
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( children[ i ].second[ j ]->matches_class_name( _class_name_child ) ){
            map< std::string, std::string >::const_iterator it_child = children[ i ].second[ j ]->string_properties().find( _key_child );
            //assert( it_child != children[ i ].second[ j ]->string_properties().end() );
            if( it_child != children[ i ].second[ j ]->string_properties().end() ){
              if( it_grounding->second == it_child->second ){ 
                return !_invert;
              }
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
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_grounding_string_property_value_matches_child_string_property_value" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "class_name_grounding" ), ( const xmlChar* )( _class_name_grounding.c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "key_grounding" ), ( const xmlChar* )( _key_grounding.c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "class_name_child" ), ( const xmlChar* )( _class_name_child.c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "key_child" ), ( const xmlChar* )( _key_child.c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
from_xml( xmlNodePtr root ){
  _invert = false;
  _class_name_grounding = "na";
  _key_grounding = "na";
  _class_name_child = "na";
  _key_child = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "class_name_grounding", "key_grounding", "class_name_child", "key_child" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }

    pair< bool, std::string > key_grounding_prop = has_prop< std::string >( root, "key_grounding" );
    if( key_grounding_prop.first ) {
      _key_grounding = key_grounding_prop.second;
    }

    pair< bool, std::string > class_name_grounding_prop = has_prop< std::string >( root, "class_name_grounding" );
    if( !class_name_grounding_prop.first ){
      cout << *this << endl;
    }
    assert( class_name_grounding_prop.first );
    _class_name_grounding = class_name_grounding_prop.second;

    pair< bool, std::string > key_child_prop = has_prop< std::string >( root, "key_child" );
    if( key_child_prop.first ) {
      _key_child = key_child_prop.second;
    }

    pair< bool, std::string > class_name_child_prop = has_prop< std::string >( root, "class_name_child" );
    if( !class_name_child_prop.first ){
      cout << *this << endl;
    }
    assert( class_name_child_prop.first );
    _class_name_child = class_name_child_prop.second;
  }
  return;
}

std::string&
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
class_name_grounding( void ){
  return _class_name_grounding;
};

const std::string&
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
class_name_grounding( void )const{
  return _class_name_grounding;
};

std::string& 
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
key_grounding( void ){ 
  return _key_grounding; 
};

const std::string& 
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
key_grounding( void )const{ 
  return _key_grounding; 
};

std::string&
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
class_name_child( void ){
  return _class_name_child;
};

const std::string&
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
class_name_child( void )const{
  return _class_name_child;
};

std::string&
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
key_child( void ){
  return _key_child;
};

const std::string&
Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value::
key_child( void )const{
  return _key_child;
};

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value& other ) {
    out << "Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value:(invert:(" << other.invert() << ") class_name_grounding:(" << other.class_name_grounding() << ") key_grounding:(" << other.key_grounding() << ") class_name_child:(" << other.class_name_child() << ") key_child:(" << other.key_child() << "))";
    return out;
  }
}
