/**
 * @file    dcg.h
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
 * The interface for a class used to represent a Distributed Correspondence
 *   Graph
 */

#ifndef H2SL_HDCG_H
#define H2SL_HDCG_H

#include <iostream>
#include <vector>
#include <map>

#include "h2sl/dcg.h"

namespace h2sl {
  class HDCG {
  public:
    HDCG();
    virtual ~HDCG();
    HDCG( const HDCG& other );
    HDCG& operator=( const HDCG& other );

    static void scrape_rules_examples( const std::string& filename, const Phrase* phrase, const h2sl::World* world, const Search_Space& searchSpace, std::vector< std::pair< unsigned int, h2sl::LLM_X > >& examples );
    static void scrape_groundings_examples( const std::string& filename, const Phrase* phrase, const h2sl::World* world, const Search_Space& searchSpace, std::vector< std::pair< unsigned int, h2sl::LLM_X > >& examples );
    virtual void fill_search_spaces( const World* world );
    virtual bool leaf_search( const Phrase* phrase, const World* world, LLM* llm, const unsigned int beamWidth = 4, const bool& debug = false );
    virtual bool leaf_search( const Phrase* phrase, const World* world, const Grounding* context, LLM* llm, const unsigned int beamWidth = 4, const bool& debug = false );

    virtual void to_latex( const std::string& filename )const;

    inline DCG& dcg_rules( void ){ return _dcg_rules; };
    inline const DCG& dcg_rules( void )const{ return _dcg_rules; };
    inline DCG& dcg_groundings( void ){ return _dcg_groundings; };
    inline const DCG& dcg_groundings( void )const{ return _dcg_groundings; };

  protected:
    virtual void _fill_phrase( Factor_Set* node, Factor_Set_Solution& solution, Phrase* phrase );
    virtual void _fill_factors( Factor_Set* node, const Phrase* phrase, const bool& fill = false );

    DCG _dcg_rules;
    DCG _dcg_groundings;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const HDCG& other );
}

#endif /* H2SL_HDCG_H */
