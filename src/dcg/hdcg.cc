/**
 * @file    hdcg.cc
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
 * The implementation of a class used to represent a Hierarchial 
 *   Distributed Correspondence Graph
 */

#include <fstream>
#include <utility>

#include "h2sl/grounding_set.h"

#include "h2sl/hdcg.h"

using namespace std;
using namespace h2sl;

HDCG::
HDCG() : _search_space_rules(),
          _dcg_rules(),
          _inferred_symbol_dictionaries(),  
          _search_space_groundings(),
          _dcg_groundings(),
          _solutions() {

}

HDCG::
~HDCG() {

}

HDCG::
HDCG( const HDCG& other ) : _search_space_rules( other._search_space_rules ),
                            _dcg_rules( other._dcg_rules ),
                            _inferred_symbol_dictionaries( other._inferred_symbol_dictionaries ),
                            _search_space_groundings( other._search_space_groundings ),
                            _dcg_groundings( other._dcg_groundings ),
                            _solutions( other._solutions ) {

}

HDCG&
HDCG::
operator=( const HDCG& other ) {
  _search_space_rules = other._search_space_rules;
  _dcg_rules = other._dcg_rules;
  _inferred_symbol_dictionaries = other._inferred_symbol_dictionaries;
  _search_space_groundings = other._search_space_groundings;
  _dcg_groundings = other._dcg_groundings;
  _solutions = other._solutions;
  return (*this);
}

bool
HDCG::
leaf_search( const Phrase* phrase,
              const Symbol_Dictionary& symbolDictionary,
              const Search_Space* searchSpace,
              const World* world,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  return leaf_search( phrase, symbolDictionary, searchSpace, world, NULL, llm, beamWidth, debug );
}

bool
HDCG::
leaf_search( const Phrase* phrase,
              const Symbol_Dictionary& symbolDictionary,
              const Search_Space* searchSpace,
              const World* world,
              const Grounding* context,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  if( phrase != NULL ){
    // first infer a distribution of reduced symbol dictionary from the rules
    _search_space_rules.fill_rules( symbolDictionary, world );
    _dcg_rules.leaf_search( phrase, symbolDictionary, &_search_space_rules, world, context, llm, beamWidth, debug );

    _inferred_symbol_dictionaries.clear();
    for( vector< pair< double, Phrase* > >::const_iterator it_solution = _dcg_rules.solutions().begin(); it_solution != _dcg_rules.solutions().end(); it_solution++ ){
      _inferred_symbol_dictionaries.push_back( pair< double, Symbol_Dictionary >( it_solution->first, Symbol_Dictionary() ) );
      // it_solution->second->scrape_grounding( _inferred_symbol_dictionaries.back().second, world );
    }

    for( vector< pair< double, Symbol_Dictionary > >::const_iterator it_symbol_dictionary = _inferred_symbol_dictionaries.begin(); it_symbol_dictionary != _inferred_symbol_dictionaries.end(); it_symbol_dictionary++ ){
      if( debug ){
        cout << "inferred symbol dictionary:" << it_symbol_dictionary->second << endl;
      }

      // now ground the phrase with the inferred distribution of symbol dictionaries
      _search_space_groundings.push_back( pair< double, Search_Space >( it_symbol_dictionary->first, Search_Space() ) );
      _search_space_groundings.back().second.fill_groundings( it_symbol_dictionary->second, world );

      if( debug ){
        cout << "inferred symbol dictionary:" << _search_space_groundings.back().second << endl;
      }

      _dcg_groundings.leaf_search( phrase, it_symbol_dictionary->second, &_search_space_groundings.back().second, world, context, llm, beamWidth, debug );
    }
    
    return true;
  } else {
    return false;
  }
}

void
HDCG::
to_latex( const string& filename )const{
  return;
} 

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const HDCG& other ) {
    
    return out;
  }
}
