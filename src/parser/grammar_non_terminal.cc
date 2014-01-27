/**
 * @file    grammar_non_terminal.cc
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
 * The implementation of a class used to describe a non-terminal grammar rule
 */

#include <sstream>
#include <boost/algorithm/string.hpp>
#include <h2sl/grammar_non_terminal.h>

using namespace std;
using namespace h2sl;

Grammar_Non_Terminal::
Grammar_Non_Terminal( const string& a ) : _elements( 1 ) {
  _elements[ 0 ] = a;
}

Grammar_Non_Terminal::
Grammar_Non_Terminal( const string& a,
                            const string& b ) : _elements( 2 ) {
  _elements[ 0 ] = a;
  _elements[ 1 ] = b;
}

Grammar_Non_Terminal::
Grammar_Non_Terminal( const string& a,
                            const string& b,
                            const string& c ) : _elements( 3 ) {
  _elements[ 0 ] = a;
  _elements[ 1 ] = b;
  _elements[ 2 ] = c;
}

Grammar_Non_Terminal::
~Grammar_Non_Terminal() {

}

Grammar_Non_Terminal::
Grammar_Non_Terminal( const Grammar_Non_Terminal& other ) : _elements( other._elements ) {

}

Grammar_Non_Terminal&
Grammar_Non_Terminal::
operator=( const Grammar_Non_Terminal& other ) {
  _elements = other._elements;
  return (*this);
}

bool 
Grammar_Non_Terminal::
operator==( const Grammar_Non_Terminal& other )const{
  if( _elements.size() != other._elements.size() ){
    return false; 
  } else {
    for( unsigned int i = 0; i < _elements.size(); i++ ){
      if( _elements[ i ] != other._elements[ i ] ){
        return false;
      }
    }
    return true;
  }
}

void
Grammar_Non_Terminal::
to_xml( const string& filename )const{
  return;
}

void
Grammar_Non_Terminal::
to_xml( xmlDocPtr doc, 
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "grammar_non_terminal" ), NULL );
  if( _elements.size() > 0 ){
    xmlNewProp( node, ( const xmlChar* )( "root" ), ( const xmlChar* )( _elements[ 0 ].c_str() ) ); 
    stringstream children_string;
    for( unsigned int i = 1; i < _elements.size(); i++ ){
      children_string << _elements[ i ];
      if( i != ( _elements.size() - 1 ) ){
        children_string << ",";
      }
    }
    xmlNewProp( node, ( const xmlChar* )( "children" ), ( const xmlChar* )( children_string.str().c_str() ) );
  }
  xmlAddChild( root, node );
  return;
}

void
Grammar_Non_Terminal::
from_xml( const string& filename ){
  return;
}

void
Grammar_Non_Terminal::
from_xml( xmlNodePtr root ){
  _elements.clear();
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "root" ) );
    if( tmp != NULL ){
      string root_string = ( char* )( tmp );
      _elements.push_back( root_string );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "children" ) );
    if( tmp != NULL ){
      string children_string = ( char* )( tmp );
      vector< string > children;
      boost::split( children, children_string, boost::is_any_of( "," ) );
      for( unsigned int i = 0; i < children.size(); i++ ){
        _elements.push_back( children[ i ] );
      }
      xmlFree( tmp );
    }
  }     
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Grammar_Non_Terminal& other ) {
    for( unsigned int i = 0; i < other.elements().size(); i++ ){
      out << other.elements()[ i ];
      if( i == 0 ){
        out << " = ";
      } else if ( i != ( other.elements().size() - 1 ) ){
        out << " ";
      }
    }
    return out;
  }

}
