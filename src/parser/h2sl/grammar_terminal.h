/**
 * @file    grammar_terminal.h
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
 * The interface for a class used to desribe a terminal grammar rule
 */

#ifndef H2SL_GRAMMAR_TERMINAL_H
#define H2SL_GRAMMAR_TERMINAL_H

#include <iostream>
#include <vector>
#include <libxml/tree.h>

#include <h2sl/pos.h>

namespace h2sl {
  class Grammar_Terminal {
  public:
    Grammar_Terminal( const std::string& symbol = "na", const std::vector< std::string >& words = std::vector< std::string >() );
    virtual ~Grammar_Terminal();
    Grammar_Terminal( const Grammar_Terminal& other );
    Grammar_Terminal& operator=( const Grammar_Terminal& other );

    void to_xml( const std::string& filename )const;
    void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    void from_xml( const std::string& filename );
    void from_xml( xmlNodePtr root );
 
    inline std::string& symbol( void ){ return _symbol; }; 
    inline const std::string& symbol( void )const{ return _symbol; };
    inline std::vector< std::string >& words( void ){ return _words; };
    inline const std::vector< std::string >& words( void )const{ return _words; };

  protected:
    std::string _symbol;
    std::vector< std::string > _words;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Grammar_Terminal& other );
}

#endif /* H2SL_GRAMMAR_TERMINAL_H */
