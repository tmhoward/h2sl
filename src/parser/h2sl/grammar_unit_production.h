/**
 * @file    grammar_unit_production.h
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
 * The interface for a class used to describe a unit production
 */

#ifndef H2SL_GRAMMAR_UNIT_PRODUCTION_H
#define H2SL_GRAMMAR_UNIT_PRODUCTION_H

#include <iostream>
#include <libxml/tree.h>

#include <h2sl/phrase.h>

namespace h2sl {
  class Grammar_Unit_Production {
  public:
    Grammar_Unit_Production( const phrase_t& phrase = PHRASE_UNKNOWN, const std::string& symbol = "UNKNOWN" );
    virtual ~Grammar_Unit_Production();
    Grammar_Unit_Production( const Grammar_Unit_Production& other );
    Grammar_Unit_Production& operator=( const Grammar_Unit_Production& other );
 
    void to_xml( const std::string& filename )const;
    void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    void from_xml( const std::string& filename );
    void from_xml( xmlNodePtr root );
 
    inline phrase_t& phrase( void ){ return _phrase; };
    inline const phrase_t& phrase( void )const{ return _phrase; };
    inline std::string& symbol( void ){ return _symbol; };
    inline const std::string& symbol( void )const{ return _symbol; };

  protected:
    phrase_t _phrase;
    std::string _symbol;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Grammar_Unit_Production& other );
}

#endif /* H2SL_GRAMMAR_UNIT_PRODUCTION_H */
