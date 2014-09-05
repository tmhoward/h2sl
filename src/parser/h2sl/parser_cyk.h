/**
 * @file    parser_cyk.h
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
 * The definition of a class that describes a cyk parser
 */

#ifndef H2SL_PARSER_CYK_H
#define H2SL_PARSER_CYK_H

#include <iostream>
#include <iomanip>
#include <sstream>

#include "h2sl/common.h"
#include "h2sl/parser.h"

namespace h2sl {
  class CYK_Cell {
  public:
    CYK_Cell( const bool& value = false, const std::vector< std::pair< CYK_Cell*, CYK_Cell* > >& backpointers = std::vector< std::pair< CYK_Cell*, CYK_Cell* > >() ) : _value( value ), _backpointers( backpointers ) {};
    virtual ~CYK_Cell(){};
    CYK_Cell( const CYK_Cell& other ) : _value( other._value ), _backpointers( other._backpointers ){};

    void add_backpointer( const std::pair< CYK_Cell*, CYK_Cell* >& backpointer ){
      bool new_backpointer = true;
      for( unsigned int i = 0; i < _backpointers.size(); i++ ){
        if( ( ( _backpointers[ i ].first == backpointer.first ) && ( _backpointers[ i ].second == backpointer.second ) ) || ( ( _backpointers[ i ].second == backpointer.first ) && ( _backpointers[ i ].first == backpointer.second ) ) ){
          new_backpointer = false;
        }
      }
      if( new_backpointer ){
        _backpointers.push_back( backpointer );
      }
      return;
    }

    inline bool& value( void ){ return _value; };
    inline const bool& value( void )const{ return _value; };
    inline std::vector< std::pair< CYK_Cell*, CYK_Cell* > >& backpointers( void ){ return _backpointers; };
    inline const std::vector< std::pair< CYK_Cell*, CYK_Cell* > >& backpointers( void )const{ return _backpointers; };

  protected:
    bool _value;
    std::vector< std::pair< CYK_Cell*, CYK_Cell* > > _backpointers;
  };

  template< class T >
  class CYK_Table {
  public:
    CYK_Table( const std::vector< Word >& words = std::vector< Word >(), const std::vector< std::string >& symbols = std::vector< std::string >() );
    virtual ~CYK_Table();
    CYK_Cell& operator()( const unsigned int& i, const unsigned int& j, const unsigned int& k );
    const CYK_Cell& operator()( const unsigned int& i, const unsigned int& j, const unsigned int& k )const;
    void fill( const Grammar& grammar );
    void parse( const Grammar& grammar, std::vector< T* >& phrases );
    void traverse( const Grammar& grammar, T* phrase, CYK_Cell* root )const;

    inline const std::vector< Word >& words( void )const{ return _words; };
    inline const std::vector< std::string >& symbols( void )const{ return _symbols; };

  protected:
    bool _find_index( const CYK_Cell* cell, std::vector< unsigned int >& index )const;
    int _symbol_to_index( const std::string& symbol, const std::vector< std::string >& symbols );
    std::string _symbol_from_index( const unsigned int& index, const std::vector< std::string >& symbols );

    CYK_Cell *** _cells;
    const std::vector< Word > _words;
    const std::vector< std::string > _symbols;

  };
  
  template< class T >
  std::ostream& operator<<( std::ostream& out, const CYK_Table<T>& other );

  template< class T >
  class Parser_CYK: public Parser<T> {
  public:
    Parser_CYK();
    virtual ~Parser_CYK();
    Parser_CYK( const Parser_CYK<T>& other );
    Parser_CYK<T>& operator=( const Parser_CYK<T>& other );

    virtual bool parse( const Grammar& grammar, const std::string& text, std::vector<T*>& phrases, const bool& debug = false )const;

  protected:

  private:

  };

  template< class T >
  std::ostream& operator<<( std::ostream& out, const Parser_CYK<T>& other );
  #include "h2sl/parser_cyk.ipp"
}

#endif /* H2SL_PARSER_CYK_H */
