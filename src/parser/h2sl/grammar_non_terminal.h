/**
 * @file    grammar_non_terminal.h
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
 * 02110-1301, USA. *
 * @section DESCRIPTION
 *
 * The interface for a class used to describe a non-terminal grammar rule
 */

#ifndef H2SL_GRAMMAR_NON_TERMINAL_H
#define H2SL_GRAMMAR_NON_TERMINAL_H

#include <iostream>
#include <vector>

#include <libxml/tree.h>

namespace h2sl {
  class Grammar_Non_Terminal {
  public:
    Grammar_Non_Terminal( const std::string& a = "na" );
    Grammar_Non_Terminal( const std::string& a, const std::string& b );
    Grammar_Non_Terminal( const std::string& a, const std::string& b, const std::string& c );
    virtual ~Grammar_Non_Terminal();
    Grammar_Non_Terminal( const Grammar_Non_Terminal& other );
    Grammar_Non_Terminal& operator=( const Grammar_Non_Terminal& other );
    bool operator==( const Grammar_Non_Terminal& other )const;

    void to_xml( const std::string& filename )const;
    void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    void from_xml( const std::string& filename );
    void from_xml( xmlNodePtr root );

    bool is_cny( void )const{ if( _elements.size() == 3 ){ return true; } else { return false; } };

    inline std::string& a( void ){ return _elements[ 0 ]; };
    inline const std::string& a( void )const{ return _elements[ 0 ]; };
    inline std::string& b( void ){ return _elements[ 1 ]; };
    inline const std::string& b( void )const{ return _elements[ 1 ]; };
    inline std::string& c( void ){ return _elements[ 2 ]; };
    inline const std::string& c( void )const{ return _elements[ 2 ]; };
    inline std::vector< std::string >& elements( void ){ return _elements; };
    inline const std::vector< std::string >& elements( void )const{ return _elements; };

  protected:
    std::vector< std::string > _elements;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Grammar_Non_Terminal& other );
}

#endif /* H2SL_GRAMMAR_NON_TERMINAL_H */
