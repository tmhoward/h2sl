/**
 * @file    grammar.h
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
 * The interface for a class used to describe a grammar for the parser
 */

#ifndef H2SL_GRAMMAR_H
#define H2SL_GRAMMAR_H

#include <iostream>
#include <vector>
#include <libxml/tree.h>

#include <h2sl/grammar_terminal.h>
#include <h2sl/grammar_non_terminal.h>
#include <h2sl/grammar_unit_production.h>

namespace h2sl {
  typedef enum {
    PHRASE_PLACEHOLDER_UNKNOWN,
    PHRASE_X0,
    PHRASE_X1,
    PHRASE_X2,
    PHRASE_X3,
    PHRASE_X4,
    PHRASE_X5,
    PHRASE_X6,
    PHRASE_X7,
    PHRASE_X8,
    PHRASE_X9,
    PHRASE_X10,
    PHRASE_X11,
    PHRASE_X12,
    PHRASE_X13,
    PHRASE_X14,
    PHRASE_X15,
    PHRASE_X16,
    PHRASE_X17,
    PHRASE_X18,
    NUM_PHRASE_PLACEHOLDERS
  } grammar_placeholder_t;

  class Grammar {
  public:
    Grammar();
    virtual ~Grammar();
    Grammar( const Grammar& other );
    Grammar& operator=( const Grammar& other );

    void cnyify( void );

    static void scrape_phrases( const Phrase* phrase, std::vector< Grammar_Terminal >& terminals, std::vector< Grammar_Non_Terminal >& nonTerminals );
    static void scrape_phrases( const Phrase* phrase, std::map< std::string, std::vector< Grammar_Terminal > >& terminals, std::map< std::string, std::vector< Grammar_Non_Terminal > >& nonTerminals );

    void to_xml( const std::string& filename )const;
    void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    void from_xml( const std::string& filename );
    void from_xml( xmlNodePtr root );

    static std::string grammar_placeholder_t_to_std_string( const grammar_placeholder_t& grammarPlaceholder );
    static grammar_placeholder_t grammar_placeholder_t_from_std_string( const std::string& arg ); 
 
    std::vector< std::string > unknown_words( const std::string& text )const;

    const Grammar_Terminal& terminal_rule( const std::string& symbol )const;
 
    inline std::vector< Grammar_Terminal >& terminals( void ){ return _terminals; };
    inline const std::vector< Grammar_Terminal >& terminals( void )const{ return _terminals; };
    inline std::vector< Grammar_Non_Terminal >& non_terminals( void ){ return _non_terminals; };
    inline const std::vector< Grammar_Non_Terminal >& non_terminals( void )const{ return _non_terminals; };
    inline std::vector< Grammar_Unit_Production >& unit_productions( void ){ return _unit_productions; };
    inline const std::vector< Grammar_Unit_Production >& unit_productions( void )const{ return _unit_productions; };
    inline const std::vector< std::string >& symbols( void )const{ return _symbols; };

  protected:
    void _compute_symbols( void );

    std::vector< Grammar_Terminal > _terminals;
    std::vector< Grammar_Non_Terminal > _non_terminals;
    std::vector< Grammar_Unit_Production > _unit_productions;
    std::vector< std::string > _symbols;
  
  private:

  };
  std::ostream& operator<<( std::ostream& out, const Grammar& other );
}

#endif /* H2SL_GRAMMAR_H */
