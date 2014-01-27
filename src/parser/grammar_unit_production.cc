/**
 * @file    grammar_unit_production.cc
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
 * The implementation of a class used to describe a unit production
 */

#include <h2sl/grammar_unit_production.h>

using namespace std;
using namespace h2sl;

Grammar_Unit_Production::
Grammar_Unit_Production( const phrase_t& phrase,
                              const string& symbol ) : _phrase( phrase ),
                                                    _symbol( symbol ) {

}

Grammar_Unit_Production::
~Grammar_Unit_Production() {

}

Grammar_Unit_Production::
Grammar_Unit_Production( const Grammar_Unit_Production& other ) : _phrase( other._phrase ),
                                                                            _symbol( other._symbol ){

}

Grammar_Unit_Production&
Grammar_Unit_Production::
operator=( const Grammar_Unit_Production& other ) {
  _phrase = other._phrase;
  _symbol = other._symbol;
  return (*this);
}

void
Grammar_Unit_Production::
to_xml( const std::string& filename )const{
  return;
}

void
Grammar_Unit_Production::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "grammar_unit_production" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "symbol" ), ( const xmlChar* )( _symbol.c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "phrase" ), ( const xmlChar* )( Phrase::phrase_t_to_std_string( _phrase ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Grammar_Unit_Production::
from_xml( const std::string& filename ){
  return;
}

void
Grammar_Unit_Production::
from_xml( xmlNodePtr root ){
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "phrase" ) );
    if( tmp != NULL ){
      string phrase_string = ( char* )( tmp );
      _phrase = Phrase::phrase_t_from_std_string( phrase_string );
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

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Grammar_Unit_Production& other ) {
    out << Phrase::phrase_t_to_std_string( other.phrase() ) << "-" << other.symbol();
    return out;
  }

}
