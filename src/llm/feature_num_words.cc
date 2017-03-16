/**
 * @file    feature_num_words.cc
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
#include "h2sl/feature_num_words.h"

using namespace std;
using namespace h2sl;

Feature_Num_Words::
Feature_Num_Words( const bool& invert,
                    const unsigned int& numWords ) : Feature( invert ),
                                                      _num_words( numWords ) {

}

Feature_Num_Words::
~Feature_Num_Words() {

}

Feature_Num_Words::
Feature_Num_Words( const Feature_Num_Words& other ) : Feature( other ),
                                                _num_words( other._num_words ) {

}

Feature_Num_Words&
Feature_Num_Words::
operator=( const Feature_Num_Words& other ) {
  _invert = other._invert;
  _num_words = other._num_words;
  return (*this);
}

bool
Feature_Num_Words::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Num_Words::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  if( phrase != NULL ){
    if( phrase->words().size() == _num_words ){
      return !_invert;
    } else {
      return _invert;
    }
  }
  return false;
}

void
Feature_Num_Words::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_num_words" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  stringstream num_words_string;
  num_words_string << _num_words;
  xmlNewProp( node, ( const xmlChar* )( "num_words" ), ( const xmlChar* )( num_words_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Num_Words::
from_xml( xmlNodePtr root ){
  _invert = false;
  _num_words = 0;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "num_words" ) );
    if( tmp != NULL ){
      string num_words_string = ( char* )( tmp );
      _num_words = strtol( num_words_string.c_str(), NULL, 10 );
      xmlFree( tmp );
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Num_Words& other ) {
    out << "Feature_Num_Words:(invert:(" << other.invert() << ") num_words:(" << other.num_words() << "))";
    return out;
  }

}
