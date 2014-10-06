/**
 * @file    parser_cyk.ipp
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
 * The implementation of a class that describes a cyk parser
 */

template< class T >
CYK_Table<T>::
CYK_Table( const std::vector< Word >& words,
            const std::vector< std::string >& symbols ) : _cells( NULL ),
                                                          _words( words ),
                                                          _symbols( symbols ) {
  if( ( _words.size() > 0 ) && ( _symbols.size() > 0 ) ){
    _cells = new CYK_Cell**[ _words.size() ];
    for( unsigned int i = 0; i < _words.size(); i++ ){
      _cells[ i ] = new CYK_Cell*[ _words.size() ];
      for( unsigned int j = 0; j < _words.size(); j++ ){
        _cells[ i ][ j ] = new CYK_Cell[ _symbols.size() ];
      }
    }
  }
}

template< class T >
CYK_Table<T>::
~CYK_Table() {
  if( _cells != NULL ){
    for( unsigned int i = 0; i < _words.size(); i++ ){
      for( unsigned int j = 0; j < _words.size(); j++ ){
        if( _cells[ i ][ j ] != NULL ){
          delete[] _cells[ i ][ j ];
          _cells[ i ][ j ] = NULL;
        }
      }
      if( _cells[ i ] != NULL ){
        delete[] _cells[ i ];
        _cells[ i ] = NULL;
      }
    }
    delete[] _cells;
    _cells = NULL;
  }
}

template< class T >
CYK_Cell&
CYK_Table<T>::
operator()( const unsigned int& i,
            const unsigned int& j,
            const unsigned int& k ){
  assert( ( i < _words.size() ) && ( j < _words.size() ) && ( k < _symbols.size() ) );
  return _cells[ i ][ j ][ k ];
}

template< class T >
const CYK_Cell&
CYK_Table<T>::
operator()( const unsigned int& i,
            const unsigned int& j,
            const unsigned int& k )const{
  assert( ( i < _words.size() ) && ( j < _words.size() ) && ( k < _symbols.size() ) );
  return _cells[ i ][ j ][ k ];
}

template< class T >
void
CYK_Table<T>::
fill( const Grammar& grammar ){
  for( unsigned int i = 0; i < _words.size(); i++ ){
    for( unsigned int j = 0; j < grammar.terminals().size(); j++ ){
      for( unsigned int k = 0; k < grammar.terminals()[ j ].words().size(); k++ ){
        if( _words[ i ].text() == grammar.terminals()[ j ].words()[ k ] ){
          int index = _symbol_to_index( grammar.terminals()[ j ].symbol(), grammar.symbols() );
          if( index >= 0 ){
            (*this)( i, 0, index ) = true;
          }
        }
      }
    }
  }
  for( unsigned int i = 2; i <= _words.size(); i++ ){
    int aj = i - 1;
    for( int j = 1; j < ( int )( _words.size() - i + 2 ); j++ ){
      int ai = j - 1;
      int bi = j - 1;
      for( unsigned int k = 1; k < i; k++ ){
        int bj = k - 1;
        int ci = j + k - 1;
        int cj = i - k - 1;
        for( unsigned int l = 0; l < grammar.non_terminals().size(); l++ ){
          if( grammar.non_terminals()[ l ].is_cny() ){
            int ak = _symbol_to_index( grammar.non_terminals()[ l ].a(), grammar.symbols() );
            int bk = _symbol_to_index( grammar.non_terminals()[ l ].b(), grammar.symbols() );
            int ck = _symbol_to_index( grammar.non_terminals()[ l ].c(), grammar.symbols() );
            CYK_Cell& a_cell = (*this)( ai, aj, ak );
            CYK_Cell& b_cell = (*this)( bi, bj, bk );
            CYK_Cell& c_cell = (*this)( ci, cj, ck );
            if( b_cell.value() && c_cell.value() ){
              a_cell.value() = true;
              a_cell.add_backpointer( std::make_pair< CYK_Cell*, CYK_Cell* >( &b_cell, &c_cell ) );
            }
            for( unsigned int m = 0; m < grammar.unit_productions().size(); m++ ){
              int dk = _symbol_to_index( grammar.unit_productions()[ m ].symbol(), grammar.symbols() );
              if( grammar.non_terminals()[ l ].b() == Phrase::phrase_type_t_to_std_string( grammar.unit_productions()[ m ].type() ) ){
                if( dk > 0 ){
                  CYK_Cell& d_cell = (*this)( bi, bj, dk );
                  if( d_cell.value() && c_cell.value() ){
                    a_cell.value() = true;
                    a_cell.add_backpointer( std::make_pair< CYK_Cell*, CYK_Cell* >( &b_cell, &c_cell ) );
                    b_cell.value() = true;
                    b_cell.add_backpointer( std::make_pair< CYK_Cell*, CYK_Cell* >( &d_cell, NULL ) );
                  }
                  for( unsigned int n = 0; n < grammar.unit_productions().size(); n++ ){
                    if( m != n ){
                      int ek = _symbol_to_index( grammar.unit_productions()[ n ].symbol(), grammar.symbols() );
                      if( grammar.non_terminals()[ l ].c() == Phrase::phrase_type_t_to_std_string( grammar.unit_productions()[ n ].type() ) ){
                        if( ek > 0 ){
                          CYK_Cell& e_cell = (*this)( ci, cj, ek );
                          if( d_cell.value() && e_cell.value() ){
                            a_cell.value() = true;
                            a_cell.add_backpointer( std::make_pair< CYK_Cell*, CYK_Cell* >( &( b_cell ), &( c_cell ) ) );
                            b_cell.value() = true;
                            b_cell.add_backpointer( std::make_pair< CYK_Cell*, CYK_Cell* >( &( d_cell ), NULL ) );
                            c_cell.value() = true;
                            c_cell.add_backpointer( std::make_pair< CYK_Cell*, CYK_Cell* >( NULL, &( e_cell ) ) );
                          }
                        }
                      }
                    }
                  }
                }
              } 
              if ( grammar.non_terminals()[ l ].c() == Phrase::phrase_type_t_to_std_string( grammar.unit_productions()[ m ].type() ) ){
                if( dk > 0 ){
                  CYK_Cell& d_cell = (*this)( ci, cj, dk );
                  if( b_cell.value() && d_cell.value() ){
                    a_cell.value() = true;
                    a_cell.add_backpointer( std::make_pair< CYK_Cell*, CYK_Cell* >( &b_cell, &c_cell ) );
                    c_cell.value() = true;
                    c_cell.add_backpointer( std::make_pair< CYK_Cell*, CYK_Cell* >( &d_cell, NULL ) );
                  }
                  for( unsigned int n = 0; n < grammar.unit_productions().size(); n++ ){
                    if( m != n ){
                      int ek = _symbol_to_index( grammar.unit_productions()[ n ].symbol(), grammar.symbols() );
                      if( grammar.non_terminals()[ l ].b() == Phrase::phrase_type_t_to_std_string( grammar.unit_productions()[ n ].type() ) ){
                        if( ek > 0 ){
                          CYK_Cell& e_cell = (*this)( bi, bj, ek );
                          if( e_cell.value() && d_cell.value() ){
                            a_cell.value() = true;
                            a_cell.add_backpointer( std::make_pair< CYK_Cell*, CYK_Cell* >( &( b_cell ), &( c_cell ) ) );
                            b_cell.value() = true;
                            b_cell.add_backpointer( std::make_pair< CYK_Cell*, CYK_Cell* >( &( e_cell ), NULL ) );
                            c_cell.value() = true;
                            c_cell.add_backpointer( std::make_pair< CYK_Cell*, CYK_Cell* >( NULL, &( d_cell ) ) );
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  } 
  return;
}

template< class T >
void
CYK_Table<T>::
parse( const Grammar& grammar,
        std::vector< T* >& phrases ){
  for( unsigned int i = 0; i < phrases.size(); i++ ){
    if( phrases[ i ] != NULL ){
      delete phrases[ i ];
      phrases[ i ] = NULL;
    }
  }
  phrases.clear();

  std::vector< std::vector< unsigned int > > indices;
  std::vector< std::vector< unsigned int > > sets;
  for( unsigned int i = 0; i < _words.size(); i++ ){
    for( unsigned int j = 0; j < _words.size(); j++ ){
      for( unsigned int k = 0; k < _symbols.size(); k++ ){
        if( !(*this)( i, j, k ).backpointers().empty() ){
          indices.push_back( std::vector< unsigned int >( 3 ) );
          indices.back()[ 0 ] = i;
          indices.back()[ 1 ] = j;
          indices.back()[ 2 ] = k;
          sets.push_back( std::vector< unsigned int >( (*this)( i, j, k ).backpointers().size() ) );
          for( unsigned int l = 0; l < sets.back().size(); l++ ){
            sets.back()[ l ] = l;
          }
        }
      }
    }
  }
  std::vector< std::vector< unsigned int > > power_sets = cartesian_power( sets );
  for( unsigned int i = 0; i < power_sets.size(); i++ ){
    CYK_Table * tmp = new CYK_Table( _words, _symbols );
    for( unsigned int j = 0; j < _words.size(); j++ ){
      for( unsigned int k = 0; k < _words.size(); k++ ){
        for( unsigned int l = 0; l < _symbols.size(); l++ ){
          const CYK_Cell& this_cell = (*this)( j, k, l );
          CYK_Cell& tmp_cell = (*tmp)( j, k, l );
          for( unsigned int m = 0; m < indices.size(); m++ ){
            if( ( indices[ m ][ 0 ] == j ) && ( indices[ m ][ 1 ] == k ) && ( indices[ m ][ 2 ] == l ) ){
              tmp_cell.value() = true;
              std::vector< unsigned int > first_index, second_index;
              tmp_cell.backpointers().push_back( std::pair< CYK_Cell*, CYK_Cell* >() );
              if( _find_index( this_cell.backpointers()[ power_sets[ i ][ m ] ].first, first_index ) ){
                tmp_cell.backpointers().back().first = &( (*tmp)( first_index[ 0 ], first_index[ 1 ], first_index[ 2 ] ) );
              }
              if( _find_index( this_cell.backpointers()[ power_sets[ i ][ m ] ].second, second_index ) ){
                tmp_cell.backpointers().back().second = &( (*tmp)( second_index[ 0 ], second_index[ 1 ], second_index[ 2 ] ) );
              }
            }
          }
        }
      }
    }

    

    for( unsigned int j = 0; j < tmp->symbols().size(); j++ ){
      if( (*tmp)( 0, tmp->words().size() - 1, j ).value() ){
        phrases.push_back( new T() );
        tmp->traverse( grammar, phrases.back(), &( (*tmp)( 0, tmp->words().size() - 1, j ) ) );
    
        bool new_phrase = true; 
        for( unsigned int j = 0; j < ( phrases.size() - 1 ); j++ ){
          if( *phrases.back() == *phrases[ j ] ){
            new_phrase = false;
            j = phrases.size();
          }
        }
        if( !new_phrase ){
          delete phrases.back();
          phrases.pop_back();
        } 
      }
    }
    if( tmp != NULL ){
      delete tmp;
      tmp = NULL;
    }
  }

  return;
}

template< class T >
void
CYK_Table<T>::
traverse( const Grammar& grammar, 
          T* phrase,
          CYK_Cell* root )const{
  if( root ){
    std::vector< unsigned int > root_index, first_index, second_index;
    if( _find_index( root, root_index ) ){
      for( unsigned int i = 0; i < NUM_PHRASE_TYPES; i++ ){
        if( Phrase::phrase_type_t_to_std_string( ( phrase_type_t )( i ) ) == _symbols[ root_index[ 2 ] ] ){
          phrase->type() = ( phrase_type_t )( i );
        }
      }

      const CYK_Cell& root_cell = (*this)( root_index[ 0 ], root_index[ 1 ], root_index[ 2 ] );
      if( root_cell.backpointers().size() == 1 ){
        if( _find_index( root_cell.backpointers()[ 0 ].first, first_index ) ){
          for( unsigned int l = 0; l < NUM_POS_TAGS; l++ ){
            if( pos_t_to_std_string( ( pos_t )( l ) ) == _symbols[ first_index[ 2 ] ] ){
              phrase->words().push_back( _words[ first_index[ 0 ] ] );
              phrase->words().back().pos() = ( pos_t )( l );
            }
          }
          for( unsigned int l = 0; l < NUM_PHRASE_TYPES; l++ ){
            if( Phrase::phrase_type_t_to_std_string( ( phrase_type_t )( l ) ) == _symbols[ first_index[ 2 ] ] ){
              phrase->children().push_back( new T() );
              traverse( grammar, dynamic_cast< T* >( phrase->children().back() ), root_cell.backpointers()[ 0 ].first );
            }
          }
          for( unsigned int l = 0; l < NUM_PHRASE_PLACEHOLDERS; l++ ){
            if( Grammar::grammar_placeholder_t_to_std_string( ( grammar_placeholder_t )( l ) ) == _symbols[ first_index[ 2 ] ] ){
              traverse( grammar, phrase, root_cell.backpointers()[ 0 ].first );
            }
          }
        }
        if( _find_index( root_cell.backpointers()[ 0 ].second, second_index ) ){
          for( unsigned int l = 0; l < NUM_POS_TAGS; l++ ){
            if( pos_t_to_std_string( ( pos_t )( l ) ) == _symbols[ second_index[ 2 ] ] ){
              phrase->words().push_back( _words[ second_index[ 0 ] ] );
              phrase->words().back().pos() = ( pos_t )( l );
            }
          }
          for( unsigned int l = 0; l < NUM_PHRASE_TYPES; l++ ){
            if( Phrase::phrase_type_t_to_std_string( ( phrase_type_t )( l ) ) == _symbols[ second_index[ 2 ] ] ){
              phrase->children().push_back( new T() );
              traverse( grammar, dynamic_cast< T* >( phrase->children().back() ), root_cell.backpointers()[ 0 ].second );
            }
          }
          for( unsigned int l = 0; l < NUM_PHRASE_PLACEHOLDERS; l++ ){
            if( Grammar::grammar_placeholder_t_to_std_string( ( grammar_placeholder_t )( l ) ) == _symbols[ second_index[ 2 ] ] ){
              traverse( grammar, phrase, root_cell.backpointers()[ 0 ].second );
            }
          }
        }
      } else {
        assert( false );
      }
    }
  }
  return;
}

template< class T >
std::string
CYK_Table<T>::
_symbol_from_index( const unsigned int& index,
                    const std::vector< std::string >& symbols ){
  if( index < symbols.size() ){
    return symbols[ index ];
  } else {
    return "NA";
  }
  return symbols[ index ];
}

template< class T >
bool
CYK_Table<T>::
_find_index( const CYK_Cell* cell,
              std::vector< unsigned int >& index )const{
  if( cell != NULL ){
    for( unsigned int i = 0; i < _words.size(); i++ ){
      for( unsigned int j = 0; j < _words.size(); j++ ){
        for( unsigned int k = 0; k < _symbols.size(); k++ ){
          if( cell == &( _cells[ i ][ j ][ k ] ) ){
            index.resize( 3 );
            index[ 0 ] = i;
            index[ 1 ] = j;
            index[ 2 ] = k;
            return true;
          }
        }
      }
    }
  }
  return false;
}

template< class T >
int
CYK_Table<T>::
_symbol_to_index( const std::string& symbol,
                  const std::vector< std::string >& symbols ){
  for( unsigned int i = 0; i < symbols.size(); i++ ){
    if( symbol == symbols[ i ] ){
      return i;
    }
  }
  return -1;
}

template< class T >
std::ostream&
operator<<( std::ostream& out,
            const CYK_Table<T>& other ) {
  for( int i = ( other.words().size() - 1 ); i >= 0; i-- ){
    for( unsigned int j = 0; j < other.words().size(); j++ ){
      std::stringstream table_string;
      for( unsigned int k = 0; k < other.symbols().size(); k++ ){
        if( other( j, i, k ).value() ){
          if( !table_string.str().empty() ){
            table_string << "/";
          }
          table_string << other( j, i, k ).backpointers().size();
        }
      }
      out << std::setw( 10 ) << table_string.str();
    }
    out << std::endl;
  }
  for( unsigned int i = 0; i < other.words().size(); i++ ){
    out << std::setw( 10 ) << other.words()[ i ].text();
  }
  out << std::endl;
  for( int i = ( other.words().size() - 1 ); i >= 0; i-- ){
    for( unsigned int j = 0; j < other.words().size(); j++ ){
      std::stringstream table_string;
      for( unsigned int k = 0; k < other.symbols().size(); k++ ){
        if( other( j, i, k ).value() ){
          if( !table_string.str().empty() ){
            table_string << "/";
          }
          table_string << other.symbols()[ k ];
        }
      }
      out << std::setw( 10 ) << table_string.str();
    }
    out << std::endl;
  }
  for( unsigned int i = 0; i < other.words().size(); i++ ){
    out << std::setw( 10 ) << other.words()[ i ].text();
  }
  out << std::endl;
  return out;
}

template< class T >
Parser_CYK<T>::
Parser_CYK() {

}

template< class T >
Parser_CYK<T>::
~Parser_CYK() {

}

template< class T >
Parser_CYK<T>::
Parser_CYK( const Parser_CYK<T>& other ) {

}

template< class T >
Parser_CYK<T>&
Parser_CYK<T>::
operator=( const Parser_CYK<T>& other ) {

  return (*this);
}

template< class T >
bool
Parser_CYK<T>::
parse( const Grammar& grammar,
        const std::string& text,
        std::vector<T*>& phrases,
        const bool& debug )const{
  std::vector< Word > words;
  this->_text_to_words( text, words );

  CYK_Table<T> * cyk_table = new CYK_Table<T>( words, grammar.symbols() );

  cyk_table->fill( grammar );
  cyk_table->parse( grammar, phrases );

  if( debug ){
    if( cyk_table != NULL ){
      std::cout << *cyk_table << std::endl;
    }
  }

  if( cyk_table != NULL ){
    delete cyk_table;
    cyk_table = NULL;
  }

  return !phrases.empty();
}

template< class T >
std::ostream&
operator<<( std::ostream& out,
            const Parser_CYK<T>& other ) {
  return out;
}

