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

#include <assert.h>
#include <fstream>
#include <sstream>

#include <h2sl/phrase.h>

using namespace std;
using namespace h2sl;

Phrase::
Phrase( const phrase_type_t& type,
        const string& text,
        const vector< Word >& words,
        const vector< Phrase* >& children,
        Grounding_Set* groundingSet,
        const map< string, string >& properties ) : _type( type ),
                                                    _text( text ),
                                                    _words( words ),
                                                    _children( children ),
                                                    _grounding_set( groundingSet ),
                                                    _properties( properties ){

}

Phrase::
~Phrase() {

}

Phrase::
Phrase( const Phrase& other ) : _type( other._type ),
                                _text( other._text ),
                                _words( other._words ),
                                _children(),
                                _grounding_set( NULL ),
                                _properties( other._properties ){
  for( unsigned int i = 0; i < other._children.size(); i++ ){
    _children.push_back( other._children[ i ]->dup() );
  }
  if( other._grounding_set != NULL ){
    _grounding_set = other._grounding_set->dup();
  }
}

Phrase&
Phrase::
operator=( const Phrase& other ) {
  _type = other._type;
  _text = other._text;
  _words = other._words;
  for( unsigned int i = 0; i < _children.size(); i++ ){
    if( _children[ i ] != NULL ){
      delete _children[ i ];
      _children[ i ] = NULL;
    }
  }
  for( unsigned int i = 0; i < other._children.size(); i++ ){
    _children.push_back( other._children[ i ]->dup() );
  }
  if( other._grounding_set != NULL ){
    _grounding_set = other._grounding_set->dup();
  } else {
    _grounding_set = NULL;  
  }
  _properties = other._properties;
  return (*this);
}

bool
Phrase::
operator==( const Phrase& other )const{
  if( _type != other._type ){
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
      if( *_children[ i ] != *other._children[ i ] ){
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

Phrase*
Phrase::
dup( const bool& empty )const{
  if( empty ){
    return new Phrase();
  } else {
    return new Phrase( *this );
  }
}

void
Phrase::
scrape_groundings( const World * world,
                    map< string, vector< string > >& stringTypes,
                    map< string, vector< int > >& intTypes )const{

  _grounding_set->scrape_grounding( world, stringTypes, intTypes );

  for( vector< Phrase* >::const_iterator it_child = _children.begin(); it_child != _children.end(); it_child++ ){
    (*it_child)->scrape_groundings( world, stringTypes, intTypes );
  }

  return;
}

void
Phrase::
scrape_groundings( const World * world,
                    vector< string >& classNames,
                    map< string, vector< string > >& stringTypes,
                    map< string, vector< int > >& intTypes )const{

  _grounding_set->scrape_grounding( world, classNames, stringTypes, intTypes );

  for( vector< Phrase* >::const_iterator it_child = _children.begin(); it_child != _children.end(); it_child++ ){
    (*it_child)->scrape_groundings( world, classNames, stringTypes, intTypes );
  }

  return;
}

void
Phrase::
scrape_groundings( const World * world,
                    map< string, vector< string > >& classNames,
                    map< string, vector< string > >& stringTypes,
                    map< string, vector< int > >& intTypes )const{

  map< string, vector< string > >::iterator it_class_names = classNames.find( phrase_type_t_to_std_string( type() ) );
  assert( it_class_names != classNames.end() );

  _grounding_set->scrape_grounding( world, it_class_names->second, stringTypes, intTypes );

  for( vector< Phrase* >::const_iterator it_child = _children.begin(); it_child != _children.end(); it_child++ ){
    (*it_child)->scrape_groundings( world, classNames, stringTypes, intTypes );
  }

  return;
}

bool
Phrase::
contains_symbol_in_symbol_dictionary( const Symbol_Dictionary& symbolDictionary )const{
  bool contains_symbol = _grounding_set->contains_symbol_in_symbol_dictionary( symbolDictionary );
  for( vector< Phrase* >::const_iterator it_child = _children.begin(); it_child != _children.end(); it_child++ ){
    contains_symbol = contains_symbol || (*it_child)->contains_symbol_in_symbol_dictionary( symbolDictionary );
  }
  return contains_symbol;
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
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( phrase_type_t_to_std_string( _type ).c_str() ), NULL );
  if( _grounding_set != NULL ){
    _grounding_set->to_xml( doc, node );    
  } 
  for( unsigned int i = 0; i < _words.size(); i++ ){
    _words[ i ].to_xml( doc, node );
  }   
  for( unsigned int i = 0; i < _children.size(); i++ ){
    if( _children[ i ] != NULL ){
      _children[ i ]->to_xml( doc, node );
    }
  }
  for( map< string, string >::const_iterator it = _properties.begin(); it != _properties.end(); it++ ){
    xmlNewProp( node, ( const xmlChar* )( it->first.c_str() ), ( const xmlChar* )( it->second.c_str() ) );
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
            if( xmlStrcmp( l1->name, ( const xmlChar* )( phrase_type_t_to_std_string( ( phrase_type_t )( i ) ).c_str() ) ) == 0 ){
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
  for( unsigned int i = 0; i < _children.size(); i++ ){
    if( _children[ i ] != NULL ){
      delete _children[ i ];
      _children[ i ] = NULL;
    }
  }
  _children.clear();
  if( root->type == XML_ELEMENT_NODE ){
    for( unsigned int i = 0; i < NUM_PHRASE_TYPES; i++ ){
      if( xmlStrcmp( root->name, ( const xmlChar* )( phrase_type_t_to_std_string( ( phrase_type_t )( i ) ).c_str() ) ) == 0 ){
        _type = ( phrase_type_t )( i );
      }
    }
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "grounding_set" ) ) == 0 ){
          _grounding_set = new Grounding_Set( l1 );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "grounding" ) ) == 0 ){
          xmlNodePtr l2 = NULL;
          for( l2 = l1->children; l2; l2 = l2->next ){
            if( l2->type == XML_ELEMENT_NODE ){
              if( xmlStrcmp( l2->name, ( const xmlChar* )( "grounding_set" ) ) == 0 ){
                _grounding_set = new Grounding_Set( l2 );
              } else{
                cout << "could not load" << l2->name << endl;
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( phrase_type_t_to_std_string( ( phrase_type_t )( i ) ).c_str() ) ) == 0 ){
            _children.push_back( new Phrase() );
            _children.back()->from_xml( l1 );
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

unsigned int
Phrase::
min_word_order( void )const{
  unsigned int min_order = 0;
  for( unsigned int i = 0; i < _words.size(); i++ ){
    if( ( _words[ i ].order() < min_order ) || ( i == 0 ) ){
      min_order = _words[ i ].order();
    }
  }

  for( unsigned int i = 0; i < _children.size(); i++ ){
    if( _children[ i ] != NULL ){
      unsigned int min_order_child = _children[ i ]->min_word_order();
      if( ( min_order_child < min_order ) || _words.empty() ){
        min_order = min_order_child;      
      }
    }
  }

  return min_order;
}

/**
 * Function to return the number of child phrases
 */
unsigned int
Phrase::
num_phrases( void )const{
  return num_phrases( this );
}

unsigned int
Phrase::
num_phrases( const Phrase* phrase )const{
  unsigned int tmp = 1;
  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    tmp += num_phrases( phrase->children()[ i ] );
  }
  return tmp;
}

/**
 * Function to return the number of words in the phrase and child phrases
 */
unsigned int
Phrase::
num_words( void )const{
  return num_words( this );
}

unsigned int
Phrase::
num_words( const Phrase* phrase )const{

  unsigned int tmp = phrase->words().size();
  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    tmp += num_words( phrase->children()[ i ] );
  }
  return tmp;
}

/**
 * Function to output the phrase as a tikz figure
 */
void
Phrase::
to_tikz( const string& filename,
          const string& caption,
          const string& label ){
  ofstream outfile;
  outfile.open( filename );

  outfile << "\\begin{figure}[!htb]" << endl;
  outfile << "\\begin{center}" << endl;
  outfile << "\\begin{tikzpicture}[textnode/.style={anchor=mid,font=\\tiny},nodeknown/.style={circle,draw=black!80,fill=black!10,minimum size=6mm,font=\\tiny,top color=white,bottom color=black!20},nodeunknown/.style={circle,draw=black!80,fill=white,minimum size=6mm,font=\\tiny},factor/.style={rectangle,draw=black!80,fill=black!80,minimum size=3mm,font=\\tiny,text=white}]" << endl;

  map< string, pair< double, double > > nodes;

  outfile << to_tikz( this, nodes );

  outfile << "\\end{tikzpicture}" << endl;
  outfile << "\\end{center}" << endl;
  outfile << "\\caption{" << caption << "}" << endl;
  outfile << "\\label{" << label << "}" << endl;
  outfile << "\\end{figure}" << endl;

  outfile.close();

  return;
}

string
Phrase::
to_tikz( const h2sl::Phrase* phrase,
          map< string, pair< double, double > >& nodes )const{
  stringstream tmp;
  for( unsigned int i = 0; i < phrase->words().size(); i++ ){
    stringstream word_name;
    word_name << "W" << phrase->words()[ i ].order();
    pair< double, double > word_position( phrase->words()[ i ].order() * 1.0, 0.0 );
    tmp << "\\node[anchor=mid, align=center](" << word_name.str() << ") at (" << word_position.first << "," << word_position.second << ") {\\footnotesize{\\textit{" << phrase->words()[ i ].text() << "}}};" << endl;
//    nodes.insert( pair< string, pair< double, double > >( word_name, word_position ) );
  }

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    tmp << to_tikz( phrase->children()[ i ], nodes );
  }

  return tmp.str();
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
phrase_type_t_to_std_string( const phrase_type_t& type ){
  switch( type ){
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
  case( PHRASE_WHNP ):
    return "WHNP";
    break;
  case( PHRASE_WHVP ):
    return "WHVP";
    break;
  case( PHRASE_M ):
    return "M";
    break;
  case( PHRASE_UNKNOWN ):
  case( NUM_PHRASE_TYPES ):
  default:
    return "PHRASE_UNKNOWN";
  }
}

phrase_type_t
Phrase::
phrase_type_t_from_std_string( const string& arg ){
  for( unsigned int i = 0; i < NUM_PHRASE_TYPES; i++ ){
    if( phrase_type_t_to_std_string( ( phrase_type_t )( i ) ) == arg ){
      return ( phrase_type_t )( i );
    }
  }
  return PHRASE_UNKNOWN;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Phrase& other ) {
    out << Phrase::phrase_type_t_to_std_string( other.type() );
    for( unsigned int i = 0; i < other.words().size(); i++ ){
      out << "(" << other.words()[ i ] << ")";
      if( i != ( other.words().size() - 1 ) ){
        out << ",";
      }
    }
    for( unsigned int i = 0; i < other.children().size(); i++ ){
      out << "{" << *other.children()[ i ] << "}";
      if( i != ( other.children().size() - 1 ) ){
        out << ",";
      }
    }
    out << " ";
    if( other.grounding_set() != NULL ){
      out << "grounding_set:{" << *other.grounding_set() << "}";
    } else {
      out << "grounding_set:{NULL}";
    }
    out << " properties:{";
    for( map< string, string >::const_iterator it = other.properties().begin(); it != other.properties().end(); it++ ){
      out << "(" << it->first << ":" << it->second << ")";
      if( next( it ) != other.properties().end() ){
        out << ",";
      }
    }
    out << "}" << endl;
    return out;
  }
}
