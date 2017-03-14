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
HDCG() : _dcg_rules(),
          _dcg_groundings() {

}

HDCG::
~HDCG() {

}

HDCG::
HDCG( const HDCG& other ) : _dcg_rules( other._dcg_rules ),
                            _dcg_groundings( other._dcg_groundings ) {

}

HDCG&
HDCG::
operator=( const HDCG& other ) {
  _dcg_rules = other._dcg_rules;
  _dcg_groundings = other._dcg_groundings;
  return (*this);
}

void
HDCG::
scrape_rules_examples( const string& filename,
                        const Phrase* phrase,
                        const h2sl::World* world,
                        const Search_Space& searchSpace,
                        vector< pair< unsigned int, h2sl::LLM_X > >& examples ){
  assert( phrase->grounding_set() != NULL );
  for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_groundings = searchSpace.grounding_pairs().begin(); it_groundings != searchSpace.grounding_pairs().end(); it_groundings++ ){
    for( vector< Grounding* >::const_iterator it_grounding = it_groundings->second.second.begin(); it_grounding != it_groundings->second.second.end(); it_grounding++ ){
      map< string, vector< unsigned int > >::const_iterator it_cvs = searchSpace.cvs().find( it_groundings->second.first );
      assert( it_cvs != searchSpace.cvs().end() );
      examples.push_back( pair< unsigned int, h2sl::LLM_X >( phrase->grounding_set()->evaluate_cv( *it_grounding ), h2sl::LLM_X( *it_grounding, phrase, world, it_cvs->second, vector< h2sl::Feature* >(), filename ) ) );
      for( unsigned int j = 0; j < phrase->children().size(); j++ ){
        examples.back().second.children().push_back( pair< const h2sl::Phrase*, vector< h2sl::Grounding* > >( phrase->children()[ j ], vector< h2sl::Grounding* >() ) );
        Grounding_Set * child_grounding_set = phrase->children()[ j ]->grounding_set();
        if( child_grounding_set ){
          for( unsigned int k = 0; k < child_grounding_set->groundings().size(); k++ ){
            examples.back().second.children().back().second.push_back( child_grounding_set->groundings()[ k ] );
          }
        }
      }
    }
  }

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    scrape_rules_examples( filename, dynamic_cast< Phrase* >( phrase->children()[ i ] ), world, searchSpace, examples );
  }
  return;
}

void
HDCG::
scrape_groundings_examples( const string& filename,
                            const Phrase* phrase,
                            const h2sl::World* world,
                            const Search_Space& searchSpace,
                            vector< pair< unsigned int, h2sl::LLM_X > >& examples ){
  assert( phrase->grounding_set() != NULL );
  for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_groundings = searchSpace.grounding_pairs().begin(); it_groundings != searchSpace.grounding_pairs().end(); it_groundings++ ){
    for( vector< Grounding* >::const_iterator it_grounding = it_groundings->second.second.begin(); it_grounding != it_groundings->second.second.end(); it_grounding++ ){
      map< string, vector< unsigned int > >::const_iterator it_cvs = searchSpace.cvs().find( it_groundings->second.first );
      assert( it_cvs != searchSpace.cvs().end() );
      examples.push_back( pair< unsigned int, h2sl::LLM_X >( phrase->grounding_set()->evaluate_cv( *it_grounding ), h2sl::LLM_X( *it_grounding, phrase, world, it_cvs->second, vector< h2sl::Feature* >(), filename ) ) );
      for( unsigned int j = 0; j < phrase->children().size(); j++ ){
        examples.back().second.children().push_back( pair< const h2sl::Phrase*, vector< h2sl::Grounding* > >( phrase->children()[ j ], vector< h2sl::Grounding* >() ) );
        Grounding_Set * child_grounding_set = phrase->children()[ j ]->grounding_set();
        if( child_grounding_set ){
          for( unsigned int k = 0; k < child_grounding_set->groundings().size(); k++ ){
            examples.back().second.children().back().second.push_back( child_grounding_set->groundings()[ k ] );
          }
        }
      }
    }
  }

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    scrape_groundings_examples( filename, dynamic_cast< Phrase* >( phrase->children()[ i ] ), world, searchSpace, examples );
  }
  return;
}

bool
HDCG::
leaf_search( const Phrase* phrase,
              const World* world,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  return leaf_search( phrase, world, NULL, llm, beamWidth, debug );
}
  
bool
HDCG::
leaf_search( const Phrase* phrase,
              const World* world,
              const Grounding* context,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  if( phrase != NULL ){
    // first infer a distribution of reduced symbol dictionary from the rules

    // now ground the phrase with the inferred distribution of symbol dictionaries

    return true;
  } else {
    return false;
  }
}

void
HDCG::
to_latex( const string& filename )const{
  ofstream outfile;
  outfile.open( filename.c_str() );
  outfile << "\\begin{tikzpicture}[textnode/.style={anchor=mid,font=\\tiny},nodeknown/.style={circle,draw=black!80,fill=black!10,minimum size=5mm,font=\\tiny,top color=white,bottom color=black!20},nodeunknown/.style={circle,draw=black!80,fill=white,minimum size=5mm,font=\\tiny},factor/.style={rectangle,draw=black!80,fill=black!80,minimum size=2mm,font=\\tiny,text=white},dot/.style={circle,draw=black!80,fill=black!80,minimum size=0.25mm,font=\\tiny}]" << endl;
  outfile << "\\end{tikzpicture}" << endl;
  outfile.close();
  return;
} 

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const HDCG& other ) {
    
    return out;
  }
}
