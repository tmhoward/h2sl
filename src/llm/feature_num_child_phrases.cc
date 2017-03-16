/**
 * @file    feature_num_child_phrases.cc
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
 * The implementation of a class used to check for a match with a phrase's number
 * of words.
 */

#include <sstream>

#include "h2sl/region.h"
#include "h2sl/feature_num_child_phrases.h"

using namespace std;
using namespace h2sl;

Feature_Num_Child_Phrases::
Feature_Num_Child_Phrases( const bool& invert,
                          const unsigned int& numChildPhrases ) : Feature( invert ),
                                                                  _num_child_phrases( numChildPhrases ) {

}

Feature_Num_Child_Phrases::
Feature_Num_Child_Phrases( xmlNodePtr root ) : Feature(),
                                                _num_child_phrases( 0 ) {
  from_xml( root );
}

Feature_Num_Child_Phrases::
~Feature_Num_Child_Phrases() {

}

Feature_Num_Child_Phrases::
Feature_Num_Child_Phrases( const Feature_Num_Child_Phrases& other ) : Feature( other ),
                                                               _num_child_phrases( other._num_child_phrases ) {

}

Feature_Num_Child_Phrases&
Feature_Num_Child_Phrases::
operator=( const Feature_Num_Child_Phrases& other ) {
  _invert = other._invert;
  _num_child_phrases = other._num_child_phrases;
  return (*this);
}

bool
Feature_Num_Child_Phrases::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Num_Child_Phrases::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  if( phrase != NULL ){
    if( phrase->children().size() == _num_child_phrases ){
      return !_invert;
    } else {
      return _invert;
    }
  }
  return false;
}

void
Feature_Num_Child_Phrases::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_num_child_phrases" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  stringstream num_child_phrases_string;
  num_child_phrases_string << _num_child_phrases;
  xmlNewProp( node, ( const xmlChar* )( "num_child_phrases" ), ( const xmlChar* )( num_child_phrases_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Num_Child_Phrases::
from_xml( xmlNodePtr root ){
  _invert = false;
  _num_child_phrases = 0;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "num_child_phrases" ) );
    if( tmp != NULL ){
      string num_child_phrases_string = ( char* )( tmp );
      _num_child_phrases = strtol( num_child_phrases_string.c_str(), NULL, 10 );
      xmlFree( tmp );
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Num_Child_Phrases& other ) {
    out << "Feature_Num_Child_Phrases:(invert:(" << other.invert() << ") num_child_phrases:(" << other.num_child_phrases() << "))";
    return out;
  }

}
