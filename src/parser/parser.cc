/**
 * @file    parser.cc
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
 * The implementation of a class that converts a text string into an unstructured natural language class
 */

#include <iomanip>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include <h2sl/parser.h>

using namespace std;
using namespace h2sl;

namespace h2sl {
  class Table_Index {
  public:
    Table_Index( const int& i = -1, const int& j = -1, const int& k = -1 );
    virtual ~Table_Index();
    Table_Index( const Table_Index& other );
    Table_Index& operator=( const Table_Index& other );
  
    inline int& i( void ){ return _i; };
    inline const int& i( void )const{ return _i; };
    inline int& j( void ){ return _j; };
    inline const int& j( void )const{ return _j; };
    inline int& k( void ){ return _k; };
    inline const int& k( void )const{ return _k; };

  protected:
    int _i; 
    int _j;
    int _k;
  };
  ostream& operator<<( ostream& out, const Table_Index& other ){
    out << "[" << other.i() << "][" << other.j() << "][" << other.k() << "]";
    return out;
  }

  class Table_Element {
  public:
    Table_Element( const Table_Index& = Table_Index(), const Table_Element* first = NULL, const Table_Element* second = NULL );
    virtual ~Table_Element();  
    Table_Element( const Table_Element& other );
    Table_Element& operator=( const Table_Element& other );      

    inline Table_Index& index( void ){ return _index; };
    inline const Table_Index& index( void )const{ return _index; }; 
    inline Table_Element*& first( void ){ return _first; };
    inline const Table_Element* first( void )const{ return _first; };
    inline Table_Element*& second( void ){ return _second; };
    inline const Table_Element* second( void )const{ return _second; };

  protected:
    Table_Index _index;
    Table_Element * _first;
    Table_Element * _second;
 
  };
  ostream& operator<<( ostream& out, const Table_Element& other ){
    out << "{" << other.index() << "";
    out << "{";
    if( other.first() != NULL ){
      out << *other.first();
    }
    out << "}";
    out << "{";
    if( other.second() != NULL ){
      out << *other.second();
    }
    out << "}";
    out << "}";
    return out;
  } 
}

Table_Index::
Table_Index( const int& i,
              const int& j,
              const int& k ) : _i( i ),
                                _j( j ),
                                _k( k ) {
  
}

Table_Index::
~Table_Index(){
  
}

Table_Index::
Table_Index( const Table_Index& other ) : _i( other._i ),
                                          _j( other._j ),
                                          _k( other._k ) {

}

Table_Index&
Table_Index::
operator=( const Table_Index& other ){
  _i = other._i;  
  _j = other._j;
  _k = other._k;
  return ( *this );
}

Table_Element::
Table_Element( const Table_Index& index,
                const Table_Element* first,
                const Table_Element* second ) : _index( index ),
                                                _first( NULL ),
                                                _second( NULL ) {

}

Table_Element::
~Table_Element() {

}

Table_Element::
Table_Element( const Table_Element& other ) : _index( other._index ),
                                              _first( NULL ),
                                              _second( NULL ) {

}

Table_Element&
Table_Element::
operator=( const Table_Element& other ){
  _index = other._index;
  return (*this);
} 

string
symbol_from_index( const int& index, const vector< string >& symbols ){
  if( ( index >= 0 ) && ( index < symbols.size() ) ){
    return symbols[ index ];
  } else {
    return "UNKNOWN SYMBOL";
  } 
}

int
symbol_to_index( const std::string& symbol, const vector< string >& symbols ){
  for( unsigned int i = 0; i < symbols.size(); i++ ){
    if( symbol == symbols[ i ] ){
      return i;
    }
  }
  return -1;
}

bool
find_table_element_indices( bool* root, 
                            const vector< string >& words,
                            vector< string >& symbols,
                            bool*** table,
                            Table_Index& index ) {
  if( root ){
    for( unsigned int i = 0; i < words.size(); i++ ){
      for( unsigned int j = 0; j < words.size(); j++ ){
        for( unsigned int k = 0; k < symbols.size(); k++ ){
          if( root == &table[ i ][ j ][ k ] ){
            index.i() = i;
            index.j() = j;
            index.k() = k;
            return true;
          }
        }
      }
    }
  }
  return false;
}

void
process_chart_element( Phrase& phrase,
                        const Grammar& grammar,
                        bool* root,
                        const vector< string >& words,
                        vector< string >& symbols,
                        bool*** table, 
                        vector< pair< bool*, bool* > >*** backpointers ){
  if( root ){
    Table_Index root_index;
    if( find_table_element_indices( root, words, symbols, table, root_index ) ){
    }
    for( unsigned int i = 0; i < NUM_PHRASE_TYPES; i++ ){
      if( Phrase::phrase_t_to_std_string( ( phrase_t )( i ) ) == symbols[ root_index.k() ] ){
        phrase.phrase() = ( phrase_t )( i );
      }
    }

    if( !backpointers[ root_index.i() ][ root_index.j() ][ root_index.k() ].empty() ){
      if( ( backpointers[root_index.i()][root_index.j()][root_index.k()].front().first == NULL ) && ( backpointers[root_index.i()][root_index.j()][root_index.k()].front().second == NULL ) ){
        for( unsigned int i = 0; i < grammar.unit_productions().size(); i++ ){
          if( phrase.phrase() == grammar.unit_productions()[ i ].phrase() ){
            for( unsigned int j = 0; j < NUM_POS_TAGS; j++ ){
              if( ( pos_t )( j ) == pos_t_from_std_string( grammar.unit_productions()[ i ].symbol() ) ){
                phrase.words().push_back( Word() );
                phrase.words().back().pos() = ( pos_t )( j );
                phrase.words().back().text() = words[ root_index.i() ];
              }
            }
          }
        }
      }
   
      Table_Index first_index;
      if( find_table_element_indices( backpointers[ root_index.i() ][ root_index.j() ][ root_index.k() ].front().first, words, symbols, table, first_index ) ) {
        for( unsigned int i = 0; i < NUM_POS_TAGS; i++ ){
          if( pos_t_to_std_string( ( pos_t )( i ) ) == symbols[ first_index.k() ] ){
            phrase.words().push_back( Word() );
            phrase.words().back().pos() = ( pos_t )( i );
            phrase.words().back().text() = words[ first_index.i() ];
          }
        }
      }

      if( backpointers[ root_index.i() ][ root_index.j() ][ root_index.k() ].front().first != NULL ){
        for( unsigned int i = 0; i < NUM_PHRASE_PLACEHOLDERS; i++ ){
          if( Grammar::grammar_placeholder_t_to_std_string( ( grammar_placeholder_t )( i ) ) == symbols[ first_index.k() ] ){
            process_chart_element( phrase, grammar, backpointers[ root_index.i() ][ root_index.j() ][ root_index.k() ].front().first, words, symbols, table, backpointers );
          }
        }
        for( unsigned int i = 0; i < NUM_PHRASE_TYPES; i++ ){
          if( Phrase::phrase_t_to_std_string( ( phrase_t )( i ) ) == symbols[ first_index.k() ] ){
            phrase.children().push_back( Phrase() );
            process_chart_element( phrase.children().back(), grammar, backpointers[ root_index.i() ][ root_index.j() ][ root_index.k() ].front().first, words, symbols, table, backpointers );
          }
        }
      }

      Table_Index second_index;
      if( find_table_element_indices( backpointers[ root_index.i() ][ root_index.j() ][ root_index.k() ].front().second, words, symbols, table, second_index ) ){
        for( unsigned int i = 0; i < NUM_POS_TAGS; i++ ){
          if( pos_t_to_std_string( ( pos_t )( i ) ) == symbols[ second_index.k() ] ){
            phrase.words().push_back( Word() );
            phrase.words().back().pos() = ( pos_t )( i );
            phrase.words().back().text() = words[ second_index.i() ];
          }
        }
      }

      if( backpointers[ root_index.i() ][ root_index.j() ][ root_index.k() ].front().second != NULL ){
        for( unsigned int i = 0; i < NUM_PHRASE_PLACEHOLDERS; i++ ){
          if( Grammar::grammar_placeholder_t_to_std_string( ( grammar_placeholder_t )( i ) ) == symbols[ second_index.k() ] ){
            process_chart_element( phrase, grammar, backpointers[ root_index.i() ][ root_index.j() ][ root_index.k() ].front().second, words, symbols, table, backpointers );
          }
        }
        for( unsigned int i = 0; i < NUM_PHRASE_TYPES; i++ ){
          if( Phrase::phrase_t_to_std_string( ( phrase_t )( i ) ) == symbols[ second_index.k() ] ){
            phrase.children().push_back( Phrase() );
            process_chart_element( phrase.children().back(), grammar, backpointers[ root_index.i() ][ root_index.j() ][ root_index.k() ].front().second, words, symbols, table, backpointers );
          }
        }
      }

      if( ( backpointers[ root_index.i() ][ root_index.j() ][ root_index.k() ].front().first == NULL )  && ( backpointers[ root_index.i() ][ root_index.j() ][ root_index.k() ].front().second == NULL ) ){
        for( unsigned int i = 0; i < grammar.unit_productions().size(); i++ ){
          if( Phrase::phrase_t_to_std_string( grammar.unit_productions()[ i ].phrase() ) == symbols[ root_index.k() ] ){
            for( unsigned int j = 0; j < NUM_POS_TAGS; j++ ){
              if( pos_t_to_std_string( ( pos_t )( j ) ) == grammar.unit_productions()[ i ].symbol() ){
                phrase.words().push_back( Word() );
                phrase.words().back().pos() = ( pos_t )( j );
                phrase.words().back().text() = words[ root_index.i() ];
              }
            }
          }
        }
      }
    } else { 
      for( unsigned int i = 0; i < grammar.unit_productions().size(); i++ ){
        if( Phrase::phrase_t_to_std_string( grammar.unit_productions()[ i ].phrase() ) == symbols[ root_index.k() ] ){
          for( unsigned int j = 0; j < NUM_POS_TAGS; j++ ){
            if( pos_t_to_std_string( ( pos_t )( j ) ) == grammar.unit_productions()[ i ].symbol() ){
              phrase.words().push_back( Word() );
              phrase.words().back().pos() = ( pos_t )( j );
              phrase.words().back().text() = words[ root_index.i() ];
            }
          }
        }
      }
    }
  }
  return;
}


Parser::
Parser( const Grammar& grammar ) : _grammar( grammar ) {

}

Parser::
~Parser() {

}

Parser::
Parser( const Parser& other ) : _grammar( other._grammar ){

}

Parser&
Parser::
operator=( const Parser& other ) {
  _grammar = other._grammar;
  return (*this);
}

bool
Parser::
parse( const string& text,
        Phrase& phrase )const{
  Grounding * grounding = phrase.grounding();
  phrase.grounding() = NULL;
  phrase.words().clear();
  phrase.children().clear();
  vector< string > words;
  boost::split( words, text, boost::is_any_of( " " ) );

  vector< string > symbols;
  for( unsigned int i = 0; i < _grammar.non_terminals().size(); i++ ){
    for( unsigned int j = 0; j < _grammar.non_terminals()[ i ].elements().size(); j++ ){
      bool new_symbol = true;
      for( unsigned int k = 0; k < symbols.size(); k++ ){
        if( _grammar.non_terminals()[ i ].elements()[ j ] == symbols[ k ] ){
          new_symbol = false;
        }
      }
      if( new_symbol ){
        symbols.push_back( _grammar.non_terminals()[ i ].elements()[ j ] );
      }
    }
  }
/*
  cout << "found " << symbols.size() << " symbols (";
  for( unsigned int i = 0; i < symbols.size(); i++ ){
    cout << symbols[ i ];
    if( i != ( symbols.size() - 1 ) ){
      cout << ",";
    }   
  }
  cout << ")" << endl;
*/
  unsigned int n = words.size();
  bool *** table = new bool**[ n ];
  for( unsigned int i = 0; i < n; i++ ){
    table[ i ] = new bool*[ n ];
    for( unsigned int j = 0; j < n; j++ ){
      table[ i ][ j ] = new bool[ symbols.size() ];
      for( unsigned int k = 0; k < symbols.size(); k++ ){
        table[ i ][ j ][ k ] = false;
      } 
    }
  }

  vector< pair<bool*,bool*> > ***backpointers = new vector< pair<bool*,bool*> >**[ n ];
  for( unsigned int i = 0; i < n; i++ ){
    backpointers[ i ] = new vector< pair<bool*,bool*> >*[ n ];
    for( unsigned int j = 0; j < n; j++ ){
      backpointers[ i ][ j ] = new vector< pair<bool*,bool*> >[ symbols.size() ];
      for( unsigned int k = 0; k < symbols.size(); k++ ){
//        backpointers[ i ][ j ][ k ].first = NULL;
//        backpointers[ i ][ j ][ k ].second = NULL;
      }
    }
  }

  for( unsigned int i = 0; i < n; i++ ){
    for( unsigned int j = 0; j < _grammar.terminals().size(); j++ ){
      for( unsigned int k = 0; k < _grammar.terminals()[ j ].words().size(); k++ ){
        if( words[ i ] == _grammar.terminals()[ j ].words()[ k ] ){
          if( symbol_to_index( _grammar.terminals()[ j ].symbol(), symbols ) >= 0 ){
            table[ i ][ 0 ][ symbol_to_index( _grammar.terminals()[ j ].symbol(), symbols ) ] = true;
          }
        }
      }
    } 
  } 

  for( int i = 2; i <= n; i++ ){
    int a_col = i - 1;
    for( int j = 1; j < ( n - i + 2 ); j++ ){
      int a_row = j - 1;
      int b_row = j - 1;
      for( int k = 1; k < i; k++ ){
        int b_col = k - 1;
        int c_row = j + k - 1;
        int c_col = i - k - 1;
        for( unsigned int l = 0; l < _grammar.non_terminals().size(); l++ ){
          if( _grammar.non_terminals()[ l ].is_cny() ){
            int a_index = symbol_to_index( _grammar.non_terminals()[ l ].a(), symbols );
            int b_index = symbol_to_index( _grammar.non_terminals()[ l ].b(), symbols );
            int c_index = symbol_to_index( _grammar.non_terminals()[ l ].c(), symbols );
            if( table[ b_row ][ b_col ][ b_index ] and table[ c_row ][ c_col ][ c_index ] ){
              table[ a_row ][ a_col ][ a_index ] = true;
              backpointers[ a_row ][ a_col ][ a_index ].push_back( pair< bool*, bool* >( NULL, NULL ) );
              backpointers[ a_row ][ a_col ][ a_index ].back().first = &table[ b_row ][ b_col ][ b_index ];
              backpointers[ a_row ][ a_col ][ a_index ].back().second = &table[ c_row ][ c_col ][ c_index ];
//              cout << "table[" << a_row << "][" << a_col << "][" << a_index << "] is true ";
//              cout << "backpointers ([" << b_row << "][" << b_col << "][" << b_index << "] and [" << c_row << "][" << c_col << "][" << c_index << "]) ";
//              cout << _grammar.non_terminals()[ l ] << endl;
            }
          } else {
            cout << "grammar rule is not cny" << endl;
          }
        }
      }
    }
  }

  bool* root = NULL;
  for( unsigned int k = 0; k < symbols.size(); k++ ){
    if( table[ 0 ][ n - 1 ][ k ] ){
      root = &table[ 0 ][ n - 1 ][ k ];
    }
  }
  
  process_chart_element( phrase, _grammar, root, words, symbols, table, backpointers );
  

  if( phrase.phrase() != PHRASE_S ){
    for( unsigned int i = 0; i < _grammar.unit_productions().size(); i++ ){
      if( ( _grammar.unit_productions()[ i ].phrase() == PHRASE_S ) && ( Phrase::phrase_t_from_std_string( _grammar.unit_productions()[ i ].symbol() ) == phrase.phrase() ) ){
        Phrase new_phrase( _grammar.unit_productions()[ i ].phrase() );
        new_phrase.children().push_back( phrase );
        phrase = new_phrase;
      }
    }
  }
  
  phrase.grounding() = grounding; 
 
  for( int i = ( n - 1 ); i >= 0; i-- ){
    for( int j = 0; j < n; j++ ){
      stringstream table_string;
      for( int k = 0; k < symbols.size(); k++ ){
        if( table[ j ][ i ][ k ] ){
          table_string << symbols[ k ];
        }
      }
      cout << setw( 10 ) << table_string.str();
    }
    cout << endl;
  }
  for( unsigned int i = 0; i < n; i++ ){
    cout << setw( 10 ) << words[ i ];
  }
  cout << endl;
 
  for( unsigned int i = 0; i < n; i++ ){
    for( unsigned int j = 0; j < n; j++ ){
      if( table[ i ][ j ] != NULL ){
        delete[] table[ i ][ j ];
        table[ i ][ j ] = NULL;
      }
    }
    if( table[ i ] != NULL ){
      delete[] table[ i ];
      table[ i ] = NULL;
    }
  } 
  if( table != NULL ){
    delete[] table;
    table = NULL;
  }
  
  if( root != NULL ){ 
    return true;
  } else {
    return false;
  }
} 

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Parser& other ) {
    return out;
  }

}
