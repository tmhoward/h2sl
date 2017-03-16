/**
 * @file    feature_phrase_type_and_word.cc
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
 * The implementation of a class that looks for a word in a phrase
 */

#include <sstream>

#include <h2sl/feature_phrase_type_and_word.h>

using namespace std;
using namespace h2sl;

Feature_Phrase_Type_And_Word::
Feature_Phrase_Type_And_Word( const bool& invert,
                              const Word& word,
                              const phrase_type_t& phraseType ) : Feature( invert ),
                                                                  _word( word ),
                                                                  _phrase_type( phraseType ) {

}

Feature_Phrase_Type_And_Word::
~Feature_Phrase_Type_And_Word() {

}

Feature_Phrase_Type_And_Word::
Feature_Phrase_Type_And_Word( const Feature_Phrase_Type_And_Word& other ) : Feature( other ),
                                                                            _word( other._word ),
                                                                            _phrase_type( other._phrase_type ){

}

Feature_Phrase_Type_And_Word&
Feature_Phrase_Type_And_Word::
operator=( const Feature_Phrase_Type_And_Word& other ) {
  _invert = other._invert;
  _word = other._word;
  _phrase_type = other._phrase_type;
  return (*this);
}

bool
Feature_Phrase_Type_And_Word::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}


bool
Feature_Phrase_Type_And_Word::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  if( phrase != NULL ){
    if( phrase->type() == _phrase_type ){
      if( phrase->has_word( _word ) ){
        return !_invert;
      }
    }
  }
  return false;
}

void 
Feature_Phrase_Type_And_Word::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_phrase_type_and_word" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "pos" ), ( const xmlChar* )( pos_t_to_std_string( _word.pos() ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "text" ), ( const xmlChar* )( _word.text().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "phrase_type" ), ( const xmlChar* )( Phrase::phrase_type_t_to_std_string( _phrase_type ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

void 
Feature_Phrase_Type_And_Word::
from_xml( xmlNodePtr root ){
  _invert = false;
  _word = Word();
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "pos" ) );
    if( tmp != NULL ){
      string pos_string = ( char* )( tmp );
      _word.pos() = pos_t_from_std_string( pos_string );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "text" ) );
    if( tmp != NULL ){
      _word.text() = ( char* )( tmp );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "phrase_type" ) );
    if( tmp != NULL ){
      string phrase_type_string = ( char* )( tmp );
      _phrase_type = Phrase::phrase_type_t_from_std_string( phrase_type_string );
      xmlFree( tmp );
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_Phrase_Type_And_Word& other ) {
    out << "Feature_Phrase_Type_And_Word(invert:(" << other.invert() << ") word:(" << other.word() << ") phrase_type:" << Phrase::phrase_type_t_to_std_string( other.phrase_type() ) << ")";
    return out;
  }

}
