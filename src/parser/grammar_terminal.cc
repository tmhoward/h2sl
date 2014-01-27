/**
 * @file    grammar_terminal.cc
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
 * The implementation of a class used to desribe a terminal grammar rule
 */

#include <sstream>
#include <boost/algorithm/string.hpp>
#include <h2sl/grammar_terminal.h>

using namespace std;
using namespace h2sl;

Grammar_Terminal::
Grammar_Terminal( const std::string& symbol,
                        const vector< string >& words ) : _symbol( symbol ),
                                                          _words( words ) {

}

Grammar_Terminal::
~Grammar_Terminal() {

}

Grammar_Terminal::
Grammar_Terminal( const Grammar_Terminal& other ) : _symbol( other._symbol ),
                                                              _words( other._words ){

}

Grammar_Terminal&
Grammar_Terminal::
operator=( const Grammar_Terminal& other ) {
  _symbol = other._symbol;
  _words = other._words;
  return (*this);
}

void
Grammar_Terminal::
to_xml( const std::string& filename )const{
  return;
}

void
Grammar_Terminal::
to_xml( xmlDocPtr doc, 
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "grammar_terminal" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "symbol" ), ( const xmlChar* )( _symbol.c_str() ) );
  stringstream words_string;
  for( unsigned int i = 0; i < _words.size(); i++ ){
    words_string << _words[ i ];
    if( i != ( _words.size() - 1 ) ){
      words_string << ",";
    }
  }
  xmlNewProp( node, ( const xmlChar* )( "words" ), ( const xmlChar* )( words_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Grammar_Terminal::
from_xml( const std::string& filename ){
  return;
}

void
Grammar_Terminal::
from_xml( xmlNodePtr root ){
  _symbol = "na";
  _words.clear();
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "symbol" ) );
    if( tmp != NULL ){
      _symbol = ( char* )( tmp );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "words" ) );
    if( tmp != NULL ){
      string words_string = ( char* )( tmp );
      if( !words_string.empty() ){
        boost::split( _words, words_string, boost::is_any_of( "," ) );
      }
      xmlFree( tmp );
    }

  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Grammar_Terminal& other ) {
    out << other.symbol() << "[" << other.words().size() << "]:{";
    for( unsigned int i = 0; i < other.words().size(); i++ ){
      out << other.words()[ i ];
      if( i != ( other.words().size() - 1 ) ){
        out << ",";
      }
    }   
    out << "}";
    return out;
  }
}
