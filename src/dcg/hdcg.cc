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

bool
solution_sort_hdcg( const pair< double, Phrase* >& a,
                const pair< double, Phrase* >& b ){
  return a.first > b.first;
}

HDCG::
HDCG() : _dcg(),
          _inferred_symbol_dictionaries(),  
          _solutions() {

}

HDCG::
~HDCG() {

}

HDCG::
HDCG( const HDCG& other ) : _dcg( other._dcg ),
                            _inferred_symbol_dictionaries( other._inferred_symbol_dictionaries ),
                            _solutions( other._solutions ) {

}

HDCG&
HDCG::
operator=( const HDCG& other ) {
  _dcg = other._dcg;
  _inferred_symbol_dictionaries = other._inferred_symbol_dictionaries;
  _solutions = other._solutions;
  return (*this);
}

bool
HDCG::
leaf_search( const Phrase* phrase,
              const Symbol_Dictionary& symbolDictionary,
              Search_Space* searchSpace,
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
              Search_Space* searchSpace,
              const World* world,
              const Grounding* context,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  for( unsigned int i = 0; i < _solutions.size(); i++ ){
    if( _solutions[ i ].second != NULL ){
      delete _solutions[ i ].second;
      _solutions[ i ].second = NULL;
    }
  }
  _solutions.clear();

  if( phrase != NULL ){
    // first infer a distribution of reduced symbol dictionary from the rules
    searchSpace->fill_rules( symbolDictionary, world );

    _dcg.leaf_search( phrase, symbolDictionary, searchSpace, world, context, llm, beamWidth, debug );

    _inferred_symbol_dictionaries.clear();
    for( vector< pair< double, Phrase* > >::const_iterator it_solution = _dcg.solutions().begin(); it_solution != _dcg.solutions().end(); it_solution++ ){
      _inferred_symbol_dictionaries.push_back( pair< double, Symbol_Dictionary >( it_solution->first, Symbol_Dictionary() ) );
      it_solution->second->scrape_groundings( world, _inferred_symbol_dictionaries.back().second.string_types(), _inferred_symbol_dictionaries.back().second.int_types() );
      _inferred_symbol_dictionaries.back().second.class_names() = symbolDictionary.class_names();
    }

    for( vector< pair< double, Symbol_Dictionary > >::const_iterator it_symbol_dictionary = _inferred_symbol_dictionaries.begin(); it_symbol_dictionary != _inferred_symbol_dictionaries.end(); it_symbol_dictionary++ ){
      if( debug ){
        cout << "inferred symbol dictionary (" << it_symbol_dictionary->first << "):" << it_symbol_dictionary->second << endl;
      }
  
      // now ground the phrase with the inferred distribution of symbol dictionaries
      searchSpace->fill_groundings( it_symbol_dictionary->second, world );

      if( debug ){
        cout << "inferred search space:" << *searchSpace << endl;
      }

      _dcg.leaf_search( phrase, it_symbol_dictionary->second, searchSpace, world, context, llm, beamWidth, debug );
  
      for( vector< pair< double, Phrase* > >::const_iterator it_solution = _dcg.solutions().begin(); it_solution != _dcg.solutions().end(); it_solution++ ){
        _solutions.push_back( pair< double, Phrase* >( it_symbol_dictionary->first * it_solution->first, it_solution->second->dup() ) );
      }
      cout << endl; 
    }

    // Sort the solution vector.
    sort( _solutions.begin(), _solutions.end(), solution_sort_hdcg );

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
