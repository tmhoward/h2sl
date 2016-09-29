/**
 * @file    feature_grounding_property_value.cc
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
#include "h2sl/feature_grounding_property_value.h"

using namespace std;
using namespace h2sl;

Feature_Grounding_Property_Value::
Feature_Grounding_Property_Value( const bool& invert,
                        const string& key,
                        const string& symbol ) : Feature( invert ),
                                                        _key( key ),
                                                        _symbol( symbol ) {

}

Feature_Grounding_Property_Value::
Feature_Grounding_Property_Value( xmlNodePtr root ) : Feature(),
                                                      _key( "na" ),
                                                      _symbol( "na" ) {
  from_xml( root );
}

Feature_Grounding_Property_Value::
~Feature_Grounding_Property_Value() {

}

Feature_Grounding_Property_Value::
Feature_Grounding_Property_Value( const Feature_Grounding_Property_Value& other ) : Feature( other ),
                                                                _key( other._key ),
                                                                _symbol( other._symbol ) {

}

Feature_Grounding_Property_Value&
Feature_Grounding_Property_Value::
operator=( const Feature_Grounding_Property_Value& other ) {
  _invert = other._invert;
  _key = other._key;
  _symbol = other._symbol;
  return (*this);
}

bool
Feature_Grounding_Property_Value::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Grounding_Property_Value::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  if( grounding != NULL ){
    map< std::string, std::string >::const_iterator it = grounding->properties().find( _key );
    if( it != grounding->properties().end() ){
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
Feature_Grounding_Property_Value::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_grounding_property_value" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "key" ), ( const xmlChar* )( _key.c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "symbol" ), ( const xmlChar* )( _symbol.c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Grounding_Property_Value::
from_xml( xmlNodePtr root ){
  _invert = false;
  _key = "na";
  _symbol = "na";
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "key" ) );
    if( tmp != NULL ){
      _key = ( char* )( tmp );
      xmlFree( tmp );
    } 
    tmp = xmlGetProp( root, ( const xmlChar* )( "symbol" ) );
    if( tmp != NULL ){
      _symbol = ( char* )( tmp );
      xmlFree( tmp );
    }
  }
  return;
}

std::string& 
Feature_Grounding_Property_Value::
key( void ){ 
  return _key; 
};

const std::string& 
Feature_Grounding_Property_Value::
key( void )const{ 
  return _key; 
};

std::string&
Feature_Grounding_Property_Value::
symbol( void ){ 
return _symbol;
};

const std::string& 
Feature_Grounding_Property_Value::
symbol( void )const{ 
  return _symbol; 
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Grounding_Property_Value& other ) {
    out << "Feature_Grounding_Property_Value:(invert:(" << other.invert() << ") key:(" << other.key() << ") symbol:(" << other.symbol() << "))";
    return out;
  }

}
