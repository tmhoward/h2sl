/**
 * @file    dcg.cc
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
 * The implementation of a class used to represent a Distributed 
 *   Correspondence Graph
 */

#include <fstream>
#include <utility>
#include <cassert>

#include "h2sl/grounding_set.h"
#include "h2sl/dcg.h"

using namespace std;
using namespace h2sl;

DCG::
DCG() : _solutions(),
        _root( NULL ) {

}

DCG::
~DCG() {

}

DCG::
DCG( const DCG& other ) : _solutions( other._solutions ),
                          _root( other._root ) {

}

DCG&
DCG::
operator=( const DCG& other ) {
  _solutions = other._solutions;
  _root = other._root;
  return (*this);
}

bool
DCG::
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
DCG::
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
    if( _root != NULL ){
      delete _root;
      _root = NULL;
    }

    _root = new Factor_Set( phrase->dup() );
    _fill_factors( _root, _root->phrase() );  

    Factor_Set * leaf = NULL;
    _find_leaf( _root, leaf );
    while( leaf != NULL ){
      leaf->search( searchSpace,
                    symbolDictionary,
                    world,
                    context,
                    llm,
                    beamWidth,
                    debug );
      leaf = NULL;
      _find_leaf( _root, leaf );
    }
  
    for( unsigned int i = 0; i < _root->solutions().size(); i++ ){
      _solutions.push_back( pair< double, Phrase* >( _root->solutions()[ i ].pygx(), _root->phrase()->dup() ) );

      for( unsigned int j = 0; j < _solutions.back().second->children().size(); j++ ){
        if( _solutions.back().second->children()[ j ] != NULL ){
          delete _solutions.back().second->children()[ j ];
          _solutions.back().second->children()[ j ];
        }
      }
      _solutions.back().second->children().clear();
      if( _solutions.back().second->grounding_set() != NULL ){
        delete _solutions.back().second->grounding_set();
        _solutions.back().second->grounding_set() = NULL;
      }
   
      _fill_phrase( _root, _root->solutions()[ i ], _solutions.back().second );
    }

    return true;
  } else {
    return false;
  }
}

void
DCG::
to_latex( const string& filename )const{
  ofstream outfile;
  outfile.open( filename.c_str() );
  outfile << "\\begin{tikzpicture}[textnode/.style={anchor=mid,font=\\tiny},nodeknown/.style={circle,draw=black!80,fill=black!10,minimum size=5mm,font=\\tiny,top color=white,bottom color=black!20},nodeunknown/.style={circle,draw=black!80,fill=white,minimum size=5mm,font=\\tiny},factor/.style={rectangle,draw=black!80,fill=black!80,minimum size=2mm,font=\\tiny,text=white},dot/.style={circle,draw=black!80,fill=black!80,minimum size=0.25mm,font=\\tiny}]" << endl;
  outfile << "\\end{tikzpicture}" << endl;
  outfile.close();
  return;
} 

void
DCG::
_find_leaf( Factor_Set* node, 
            Factor_Set*& leaf ){ 
  if( node->solutions().empty() ){
    bool all_child_factor_sets_known = true;
    for( unsigned int i = 0; i < node->child_factor_sets().size(); i++ ){
      if( node->child_factor_sets()[ i ]->solutions().empty() ){
        all_child_factor_sets_known = false;
      } 
    } 
    if( all_child_factor_sets_known ){
      leaf = node;
    }
  }

  for( unsigned int i = 0; i < node->child_factor_sets().size(); i++ ){
    _find_leaf( node->child_factor_sets()[ i ], leaf );
  }
  return;
}

void
DCG::
_fill_phrase( Factor_Set* node,
              Factor_Set_Solution& solution,
              Phrase* phrase ){
  cout << "filling phrase:" << *phrase << endl;
  cout << "solution:" << solution << endl;

  cout << "transfer properties from factor set to phrase:" << endl;
   // Transfer relevant properties. 
   std::map< std::string, std::string >::const_iterator it;
   it = node->properties().find( "concrete_size" );
   if( it != node->properties().end() ){
     insert_prop< std::string >( phrase->properties(), "concrete_size", it->second );
     //cout << "found key:" << "concrete_size" << "filling: " << it->second << endl;
   } else {
     insert_prop< std::string >( phrase->properties(), "concrete_size", "0" );
     //cout << "adding the key:" << "concrete_size" << "filling: " << "0" << endl;
   }

   it = node->properties().find( "abstract_max_size" );
   if( it != node->properties().end() ){
     insert_prop< std::string >( phrase->properties(), "abstract_max_size", it->second );
   } else {
     insert_prop< std::string >( phrase->properties(), "abstract_max_size", "0" );
   }

    it = node->properties().find( "abstract_avg_size" );
   if( it != node->properties().end() ){
     insert_prop< std::string >( phrase->properties(), "abstract_avg_size", it->second );
   } else {
     insert_prop< std::string >( phrase->properties(), "abstract_avg_size", "0.0" );
   }
     
  // Copy the grounding set
  phrase->grounding_set() = solution.grounding_set()->dup();

  // Examine children.
  for( unsigned int i = 0; i < node->child_factor_sets().size(); i++ ){
    phrase->children().push_back( node->child_factor_sets()[ i ]->phrase()->dup() );
    for( unsigned int j = 0; j < phrase->children().back()->children().size(); j++ ){
      if( phrase->children().back()->children()[ j ] != NULL ){
        delete phrase->children().back()->children()[ j ];
        phrase->children().back()->children()[ j ];
      }
    }
    phrase->children().back()->children().clear();
    if( phrase->children().back()->grounding_set() != NULL ){
      delete phrase->children().back()->grounding_set();
      phrase->children().back()->grounding_set() = NULL;
    }

    assert( i < node->child_factor_sets().size() );
    assert( i < solution.child_solution_indices().size() );
    assert( solution.child_solution_indices()[ i ] < node->child_factor_sets()[ i ]->solutions().size() );

    _fill_phrase( node->child_factor_sets()[ i ],
                  node->child_factor_sets()[ i ]->solutions()[ solution.child_solution_indices()[ i ] ],
                  phrase->children().back() );

  }
  return;
}

void
DCG::
_fill_factors( Factor_Set* node,
                const Phrase* phrase, 
                const bool& fill ){
  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    node->child_factor_sets().push_back( new Factor_Set( phrase->children()[ i ] ) );
    _fill_factors( node->child_factor_sets().back(), phrase->children()[ i ] );
  } 
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const DCG& other ) {
    
    return out;
  }
}
