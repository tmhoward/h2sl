/**
 * @file    word.cc
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
 * The implementation of a class used to describe a word
 */

#include <h2sl/word.h>

using namespace std;
using namespace h2sl;

Word::
Word( const pos_t& pos,
      const string& text ) : _pos( pos ),
                              _text( text ) {

}

Word::
~Word() {

}

Word::
Word( const Word& other ) : _pos( other._pos ),
                            _text( other._text ){

}

Word&
Word::
operator=( const Word& other ) {
  _pos = other._pos;
  _text = other._text;
  return (*this);
}

bool
Word::
operator==( const Word& other )const{
  if( _pos != other._pos ){
    return false;
  } else if ( _text != other._text ){
    return false;
  } else {
    return true;
  }
}

bool
Word::
operator!=( const Word& other )const{
  return !( *this == other );
}

void
Word::
to_xml( const string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

void
Word::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( pos_t_to_std_string( _pos ).c_str() ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "text" ), ( const xmlChar* )( _text.c_str() ) );
  xmlAddChild( root, node );
  return;
}

void 
Word::
from_xml( const string& filename ){
  return;
}

void 
Word::
from_xml( xmlNodePtr root ){
  if( root->type == XML_ELEMENT_NODE ){
    for( unsigned int i = 0; i < NUM_POS_TAGS; i++ ){
      if( xmlStrcmp( root->name, ( const xmlChar* )( pos_t_to_std_string( ( pos_t )( i ) ).c_str() ) ) == 0 ){
        _pos = ( pos_t )( i );
      }
    }
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "text" ) );
    if( tmp != NULL ){
      _text = ( char* )( tmp );
      xmlFree( tmp );
    }
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        _text = ( char* )( l1->name );
      }
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Word& other ) {
    out << pos_t_to_std_string( other.pos() ) << "/\"" << other.text() << "\"";
    return out;
  }

}
