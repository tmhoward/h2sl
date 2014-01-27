/**
 * @file    phrase.cc
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
 * The implementation of a class used to represent a phrase
 */

#include <fstream>
#include <sstream>

#include <h2sl/phrase.h>
#include <h2sl/region_set.h>
#include <h2sl/constraint_set.h>

using namespace std;
using namespace h2sl;

Phrase::
Phrase( const phrase_t& phrase,
        const string& text,
        const vector< Word >& words,
        const vector< Phrase >& children,
        Grounding* grounding ) : Grounding(),
                                              _phrase( phrase ),
                                              _text( text ),
                                              _words( words ),
                                              _children( children ),
                                              _grounding( grounding ){

}

Phrase::
~Phrase() {

}

Phrase::
Phrase( const Phrase& other ) : Grounding( other ),
                                _phrase( other._phrase ),
                                _text( other._text ),
                                _words( other._words ),
                                _children( other._children ),
                                _grounding( NULL ){
  if( other._grounding != NULL ){
    _grounding = other._grounding->dup();
  }
}

Phrase&
Phrase::
operator=( const Phrase& other ) {
  _phrase = other._phrase;
  _text = other._text;
  _words = other._words;
  _children = other._children;
  if( other._grounding != NULL ){
    if( other._grounding != NULL ){
      _grounding = other._grounding->dup();
    }
  } else {
    _grounding = NULL;  
  }
  return (*this);
}

bool
Phrase::
operator==( const Phrase& other )const{
  if( _phrase != other._phrase ){
    return false;
  } else if ( _text != other._text ){
    return false;
  } else if ( _words.size() != other._words.size() ){
    return false;
  } else if ( _children.size() != other._children.size() ){
    return false;
  } else {
    for( unsigned int i = 0; i < _words.size(); i++ ){
      if( _words[ i ] != other._words[ i ] ){
        return false;
      }
    }   
    for( unsigned int i = 0; i < _children.size(); i++ ){
      if( _children[ i ] != other._children[ i ] ){
        return false;
      }
    }
    return true;
  }
}

bool
Phrase::
operator!=( const Phrase& other )const{
  return !( *this == other );
}

Phrase*
Phrase::
dup( void )const{
  return new Phrase( *this );
}

void
Phrase::
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
Phrase::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( phrase_t_to_std_string( _phrase ).c_str() ), NULL );
  xmlNodePtr grounding_node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "grounding" ), NULL );
  if( _grounding != NULL ){
    _grounding->to_xml( doc, grounding_node );    
  } 
  xmlAddChild( node, grounding_node );
  for( unsigned int i = 0; i < _words.size(); i++ ){
    _words[ i ].to_xml( doc, node );
  }   
  for( unsigned int i = 0; i < _children.size(); i++ ){
    _children[ i ].to_xml( doc, node );
  }
  xmlAddChild( root, node );
  return;
}

void 
Phrase::
from_xml( const std::string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          for( unsigned int i = 0; i < NUM_PHRASE_TYPES; i++ ){
            if( xmlStrcmp( l1->name, ( const xmlChar* )( phrase_t_to_std_string( ( phrase_t )( i ) ).c_str() ) ) == 0 ){
              from_xml( l1 );
            }
          }
        }
      }
    }
    xmlFreeDoc( doc );
  }
  return;
}

void 
Phrase::
from_xml( xmlNodePtr root ){
  if( root->type == XML_ELEMENT_NODE ){
    for( unsigned int i = 0; i < NUM_PHRASE_TYPES; i++ ){
      if( xmlStrcmp( root->name, ( const xmlChar* )( phrase_t_to_std_string( ( phrase_t )( i ) ).c_str() ) ) == 0 ){
        _phrase = ( phrase_t )( i );
      }
    }
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "grounding" ) ) == 0 ){
          xmlNodePtr l2 = NULL;
          for( l2 = l1->children; l2; l2 = l2->next ){
            if( l2->type == XML_ELEMENT_NODE ){
              if( xmlStrcmp( l2->name, ( const xmlChar* )( "region_set" ) ) == 0 ){
                _grounding = new Region_Set();
                _grounding->from_xml( l2 );
              } else if ( xmlStrcmp( l2->name, ( const xmlChar* )( "constraint_set" ) ) == 0 ){
                _grounding = new Constraint_Set();
                _grounding->from_xml( l2 );
              }
            }
          }
        }
        for( unsigned int i = 0; i < NUM_POS_TAGS; i++ ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( pos_t_to_std_string( ( pos_t )( i ) ).c_str() ) ) == 0 ){
            _words.push_back( Word() );
            _words.back().from_xml( l1 );
          }
        }
        for( unsigned int i = 0; i < NUM_PHRASE_TYPES; i++ ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( phrase_t_to_std_string( ( phrase_t )( i ) ).c_str() ) ) == 0 ){
            _children.push_back( Phrase() );
            _children.back().from_xml( l1 );
          }
        }
      }
    }
  }
  return;
}

bool
Phrase::
has_word( const Word& word )const{
  for( unsigned int i = 0; i < _words.size(); i++ ){
    if( _words[ i ] == word ){
      return true;
    }
  } 
  return false;
}

bool
Phrase::
has_words( const vector< Word >& words )const{
  for( unsigned int i = 0; i < words.size(); i++ ){
    if( !has_word( words[ i ] ) ){
      return false;
    }
  }
  return true;
}

string
Phrase::
words_to_std_string( void )const{
  stringstream tmp;
  for( unsigned int i = 0; i < _words.size(); i++ ){
    tmp << _words[ i ].text();
    if( i != ( _words.size() - 1 ) ){
      tmp << " ";
    }
  }   
  return tmp.str();
}
  
string
Phrase::
phrase_t_to_std_string( const phrase_t& phrase ){
  switch( phrase ){
  case( PHRASE_NP ):
    return "NP";
    break;
  case( PHRASE_VP ):
    return "VP";
    break;
  case( PHRASE_PP ):
    return "PP";
    break;
  case( PHRASE_ADJP ):
    return "ADJP";
    break;
  case( PHRASE_ADVP ):
    return "ADVP";
    break;
  case( PHRASE_PRT ):
    return "PRT";
    break;
  case( PHRASE_S ):
    return "S";
    break;
  case( PHRASE_SBAR ):
    return "SBAR";
    break;
  case( PHRASE_UNKNOWN ):
  case( NUM_PHRASE_TYPES ):
  default:
    return "PHRASE_UNKNOWN";
  }
}

phrase_t
Phrase::
phrase_t_from_std_string( const string& arg ){
  for( unsigned int i = 0; i < NUM_PHRASE_TYPES; i++ ){
    if( phrase_t_to_std_string( ( phrase_t )( i ) ) == arg ){
      return ( phrase_t )( i );
    }
  }
  return PHRASE_UNKNOWN;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Phrase& other ) {
    out << Phrase::phrase_t_to_std_string( other.phrase() );
    for( unsigned int i = 0; i < other.words().size(); i++ ){
      out << "(" << other.words()[ i ] << ")";
      if( i != ( other.words().size() - 1 ) ){
        out << ",";
      }
    }
    for( unsigned int i = 0; i < other.children().size(); i++ ){
      out << "{" << other.children()[ i ] << "}";
      if( i != ( other.children().size() - 1 ) ){
        out << ",";
      }
    }
    out << " ";
    if( other.grounding() != NULL ){
      out << "grounding:{" << *other.grounding() << "}";
    } else {
      out << "grounding:{NULL}";
    }
    return out;
  }
}
