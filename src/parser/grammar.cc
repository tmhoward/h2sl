/**
 * @file    grammar.cc
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
 * The implementation of a class used to describe a grammar for the parser
 */

#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include <h2sl/phrase.h>
#include <h2sl/grammar.h>

using namespace std;
using namespace h2sl;

Grammar::
Grammar() : _terminals(),
            _non_terminals(),
            _unit_productions() {
  for( unsigned int i = POS_CC; i < NUM_POS_TAGS; i++ ){
    _terminals.push_back( Grammar_Terminal( pos_t_to_std_string( ( pos_t )( i ) ) ) );
  }
  for( unsigned int i = PHRASE_NP; i < NUM_PHRASE_TYPES; i++ ){
    _terminals.push_back( Grammar_Terminal( Phrase::phrase_type_t_to_std_string( ( phrase_type_t )( i ) ) ) );
  }
}
  
Grammar::
~Grammar() {

}

Grammar::
Grammar( const Grammar& other ) : _terminals( other._terminals ),
                                  _non_terminals( other._non_terminals ),
                                  _unit_productions( other._unit_productions ){

}

Grammar&
Grammar::
operator=( const Grammar& other ) {
  _terminals = other._terminals;
  _non_terminals = other._non_terminals;
  _unit_productions = other._unit_productions;
  return (*this);
}

void
Grammar::
scrape_phrases( const Phrase* phrase,
                vector< Grammar_Terminal >& terminals,
                vector< Grammar_Non_Terminal >& nonTerminals ){
  // check for unique terminals
  for( unsigned int i = 0; i < phrase->words().size(); i++ ){
    for( unsigned int j = 0; j < terminals.size(); j++ ){
      if( pos_t_to_std_string( phrase->words()[ i ].pos() ) == terminals[ j ].symbol() ){
        bool found_match = false;
        for( unsigned int k = 0; k < terminals[ j ].words().size(); k++ ){
          if( phrase->words()[ i ].text() == terminals[ j ].words()[ k ] ){
            found_match = true;
          }
        }
        if( !found_match ){
          terminals[ j ].words().push_back( phrase->words()[ i ].text() );
        }
        break;
      }
    }
  }

  // check for unique non-terminals 
  Grammar_Non_Terminal grammar_non_terminal( Phrase::phrase_type_t_to_std_string( phrase->type() ) );
  // add words  
  for( unsigned int i = 0; i < phrase->words().size(); i++ ){
    grammar_non_terminal.elements().push_back( pos_t_to_std_string( phrase->words()[ i ].pos() ) + "[" + phrase->words()[ i ].text() + "]" );
  }
  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    grammar_non_terminal.elements().push_back( Phrase::phrase_type_t_to_std_string( phrase->children()[ i ]->type() ) );
  }
  bool found_match = false;
  for( unsigned int i = 0; i < nonTerminals.size(); i++ ){
    if( grammar_non_terminal == nonTerminals[ i ] ){
      found_match = true;
    }
  }
  if( !found_match ){
    nonTerminals.push_back( grammar_non_terminal );
  }

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    scrape_phrases( phrase->children()[ i ], terminals, nonTerminals );
  }
  return;
}

void
Grammar::
scrape_phrases( const Phrase* phrase,
                map< string, vector< Grammar_Terminal > >& terminals,
                map< string, vector< Grammar_Non_Terminal > >& nonTerminals ){
  // find the phrase type in the maps
  map< string, vector< Grammar_Terminal > >::iterator it_terminals = terminals.find( Phrase::phrase_type_t_to_std_string( phrase->type() ) );
  if( it_terminals == terminals.end() ){
    terminals.insert( pair< string, vector< Grammar_Terminal > >( Phrase::phrase_type_t_to_std_string( phrase->type() ), vector< Grammar_Terminal >() ) );
    it_terminals = terminals.find( Phrase::phrase_type_t_to_std_string( phrase->type() ) );
    for( unsigned int i = POS_CC; i < NUM_POS_TAGS; i++ ){
      it_terminals->second.push_back( Grammar_Terminal( pos_t_to_std_string( ( pos_t )( i ) ) ) );
    }
    for( unsigned int i = PHRASE_NP; i < NUM_PHRASE_TYPES; i++ ){
      it_terminals->second.push_back( Grammar_Terminal( Phrase::phrase_type_t_to_std_string( ( phrase_type_t )( i ) ) ) );
    }
  }

  map< string, vector< Grammar_Non_Terminal > >::iterator it_non_terminals = nonTerminals.find( Phrase::phrase_type_t_to_std_string( phrase->type() ) );
  if( it_non_terminals == nonTerminals.end() ){
    nonTerminals.insert( pair< string, vector< Grammar_Non_Terminal > >( Phrase::phrase_type_t_to_std_string( phrase->type() ), vector< Grammar_Non_Terminal >() ) );
    it_non_terminals = nonTerminals.find( Phrase::phrase_type_t_to_std_string( phrase->type() ) );
  }

  // check for unique terminals
  for( unsigned int i = 0; i < phrase->words().size(); i++ ){
    for( unsigned int j = 0; j < it_terminals->second.size(); j++ ){
      if( pos_t_to_std_string( phrase->words()[ i ].pos() ) == it_terminals->second[ j ].symbol() ){
        bool found_match = false;
        for( unsigned int k = 0; k < it_terminals->second[ j ].words().size(); k++ ){
          if( phrase->words()[ i ].text() == it_terminals->second[ j ].words()[ k ] ){
            found_match = true;
          }
        }
        if( !found_match ){
          it_terminals->second[ j ].words().push_back( phrase->words()[ i ].text() );
        }
        break;
      }
    }
  }

  // check for unique non-terminals 
  Grammar_Non_Terminal grammar_non_terminal( Phrase::phrase_type_t_to_std_string( phrase->type() ) );
  // add words  
  for( unsigned int i = 0; i < phrase->words().size(); i++ ){
    grammar_non_terminal.elements().push_back( pos_t_to_std_string( phrase->words()[ i ].pos() ) + "[" + phrase->words()[ i ].text() + "]" );
  }
  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    grammar_non_terminal.elements().push_back( Phrase::phrase_type_t_to_std_string( phrase->children()[ i ]->type() ) );
  }
  bool found_match = false;
  for( unsigned int i = 0; i < it_non_terminals->second.size(); i++ ){
    if( grammar_non_terminal == it_non_terminals->second[ i ] ){
      found_match = true;
    }
  }
  if( !found_match ){
    it_non_terminals->second.push_back( grammar_non_terminal );
  }

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    scrape_phrases( phrase->children()[ i ], terminals, nonTerminals );
  }
  return;
}

void 
Grammar::
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
Grammar::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "grammar" ), NULL );
  for( unsigned int i = 0; i < _terminals.size(); i++ ){
    _terminals[ i ].to_xml( doc, node );
  }
  for( unsigned int i = 0; i < _non_terminals.size(); i++ ){
    _non_terminals[ i ].to_xml( doc, node );
  }
  for( unsigned int i = 0; i < _unit_productions.size(); i++ ){
    _unit_productions[ i ].to_xml( doc, node );
  }
  xmlAddChild( root, node );
  return;
}

void 
Grammar::
from_xml( const string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "grammar" ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
    }
    xmlFreeDoc( doc );
  }
  return;
}

void 
Grammar::
from_xml( xmlNodePtr root ){
  _symbols.clear();
  _terminals.clear();
  _non_terminals.clear();
  _unit_productions.clear();
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "grammar_terminal" ) ) == 0 ){
          _terminals.push_back( Grammar_Terminal() );
          _terminals.back().from_xml( l1 );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "grammar_non_terminal" ) ) == 0 ){
          _non_terminals.push_back( Grammar_Non_Terminal() ); 
          _non_terminals.back().from_xml( l1 );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "grammar_unit_production" ) ) == 0 ){
          _unit_productions.push_back( Grammar_Unit_Production() );
          _unit_productions.back().from_xml( l1 );
        }
      }
    }
  }
  _compute_symbols();
  return;
}

string
Grammar::
grammar_placeholder_t_to_std_string( const grammar_placeholder_t& grammarPlaceholder ){
  switch( grammarPlaceholder ){
  case( PHRASE_X0 ):
    return "X0";
    break; 
  case( PHRASE_X1 ):
    return "X1";
    break; 
  case( PHRASE_X2 ):
    return "X2";
    break; 
  case( PHRASE_X3 ):
    return "X3";
    break; 
  case( PHRASE_X4 ):
    return "X4";
    break; 
  case( PHRASE_X5 ):
    return "X5";
    break; 
  case( PHRASE_X6 ):
    return "X6";
    break; 
  case( PHRASE_X7 ):
    return "X7";
    break; 
  case( PHRASE_X8 ):
    return "X8";
    break; 
  case( PHRASE_X9 ):
    return "X9";
    break; 
  case( PHRASE_X10 ):
    return "X10";
    break;
  case( PHRASE_X11 ):
    return "X11";
    break;
  case( PHRASE_X12 ):
    return "X12";
    break;
  case( PHRASE_X13 ):
    return "X13";
    break;
  case( PHRASE_X14 ):
    return "X14";
    break;
  case( PHRASE_X15 ):
    return "X15";
    break;
  case( PHRASE_X16 ):
    return "X16";
    break;
  case( PHRASE_X17 ):
    return "X17";
    break;    
  case( PHRASE_X18 ):
    return "X18";
    break;
  case( NUM_PHRASE_PLACEHOLDERS ):  
  default:
    return "na";
    break;
  }
}

grammar_placeholder_t
Grammar::
grammar_placeholder_t_from_std_string( const std::string& arg ){
  for( unsigned int i = 0; i < NUM_PHRASE_PLACEHOLDERS; i++ ){
    if( grammar_placeholder_t_to_std_string( ( grammar_placeholder_t )( i ) ) == arg ){
      return ( grammar_placeholder_t )( i );
    }
  }
  return PHRASE_PLACEHOLDER_UNKNOWN;
}

void
Grammar::
cnyify( void ){
  unsigned int rule_changes = 0;
  for( unsigned int i = 0; i < _non_terminals.size(); i++ ){
    if( _non_terminals[ i ].elements().size() == 2 ){
      _unit_productions.push_back( Grammar_Unit_Production( Phrase::phrase_type_t_from_std_string( _non_terminals[ i ].elements().front() ), _non_terminals[ i ].elements().back() ) );
      for( unsigned int j = 0; j < _terminals.size(); j++ ){
        if( _terminals[ j ].symbol() == _non_terminals[ i ].elements().back() ){
          for( unsigned int k = 0; k < _terminals.size(); k++ ){
            if( _terminals[ k ].symbol() == _non_terminals[ i ].elements().front() ){
              for( unsigned int l = 0; l < _terminals[ j ].words().size(); l++ ){
                _terminals[ k ].words().push_back( _terminals[ j ].words()[ l ] );
              }
            }   
          }
        }
      }
      _non_terminals.erase( _non_terminals.begin() + i, _non_terminals.begin() + i + 1 );
      i--;
    } else if( _non_terminals[ i ].elements().size() > 3 ){
      stringstream new_element_name;
      new_element_name << "X" << rule_changes;
      Grammar_Non_Terminal new_rule_1( _non_terminals[ i ].elements()[ 0 ], new_element_name.str() );
      for( unsigned int j = 3; j < _non_terminals[ i ].elements().size(); j++ ){
        new_rule_1.elements().push_back( _non_terminals[ i ].elements()[ j ] ); 
      }    
      Grammar_Non_Terminal new_rule_2( new_element_name.str(), _non_terminals[ i ].elements()[ 1 ], _non_terminals[ i ].elements()[ 2 ] );
      _non_terminals.erase( _non_terminals.begin() + i, _non_terminals.begin() + i + 1 );
      i--; 
      _non_terminals.push_back( new_rule_1 );
      _non_terminals.push_back( new_rule_2 );
      rule_changes++;  
    }
  }   
  return;
}

vector< string >
Grammar::
unknown_words( const string& text )const{
  vector< string > unknown_words;
  vector< string > words;
  boost::split( words, text, boost::is_any_of( " " ) );
  for( unsigned int i = 0; i < words.size(); i++ ){
    bool known_word = false;
    for( unsigned int j = 0; j < _terminals.size(); j++ ){
      for( unsigned int k = 0; k < _terminals[ j ].words().size(); k++ ){
        if( words[ i ] == _terminals[ j ].words()[ k ] ){
          known_word = true;
        }
      } 
    }
    if( !known_word ){
      unknown_words.push_back( words[ i ] );
    }
  }
  return unknown_words;
}

const Grammar_Terminal&
Grammar::
terminal_rule( const string& symbol )const{
  for( unsigned int i = 0; i < _terminals.size(); i++ ){
    if( symbol == _terminals[ i ].symbol() ){
      return _terminals[ i ];
    }
  }
  return _terminals.front();
}

void
Grammar::
_compute_symbols( void ){
  _symbols.clear();
  for( unsigned int i = 0; i < _non_terminals.size(); i++ ){
    for( unsigned int j = 0; j < _non_terminals[ i ].elements().size(); j++ ){
      bool new_symbol = true;
      for( unsigned int k = 0; k < _symbols.size(); k++ ){
        if( _non_terminals[ i ].elements()[ j ] == _symbols[ k ] ){
          new_symbol = false;
        }
      }
      if( new_symbol ){
        _symbols.push_back( _non_terminals[ i ].elements()[ j ] );
      }
    }
  }

  for( unsigned int i = 0; i < _unit_productions.size(); i++ ){
    bool new_symbol = true;
    for( unsigned int j = 0; j < _symbols.size(); j++ ){
      if( _unit_productions[ i ].symbol() == _symbols[ j ] ){
        new_symbol = false;
      }
    }
    if( new_symbol ){
      _symbols.push_back( _unit_productions[ i ].symbol() );
    }
  }  

  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Grammar& other ) {
    out << "terminals[" << other.terminals().size() << "]:{";
    for( unsigned int i = 0; i < other.terminals().size(); i++ ){
      out << other.terminals()[ i ];
      if( i != ( other.terminals().size() - 1 ) ){
        out << ",";
      } 
    }
    out << "} ";
    out << "non_terminals[" << other.non_terminals().size() << "]:{";
    for( unsigned int i = 0; i < other.non_terminals().size(); i++ ){
      out << other.non_terminals()[ i ];
      if( i != ( other.non_terminals().size() - 1 ) ){
        out << ",";
      }
    }
    out << "} ";
    return out;
  }
}
