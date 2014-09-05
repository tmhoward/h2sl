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

#include "h2sl/grounding_set.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/dcg.h"

using namespace std;
using namespace h2sl;

DCG::
DCG() : _search_spaces( NUM_PHRASE_TYPES ),
        _solutions(),
        _root( NULL ) {

}

DCG::
~DCG() {

}

DCG::
DCG( const DCG& other ) : _search_spaces( other._search_spaces ),
                          _solutions( other._solutions ),
                          _root( other._root ) {

}

DCG&
DCG::
operator=( const DCG& other ) {
  _search_spaces = other._search_spaces;
  _solutions = other._solutions;
  _root = other._root;
  return (*this);
}

void
DCG::
fill_search_spaces( const World* world ){
  for( unsigned int i = 0; i < _search_spaces.size(); i++ ){
    for( unsigned int j = 0; j < _search_spaces[ i ].size(); j++ ){
      if( _search_spaces[ i ][ j ].second != NULL ){
        delete _search_spaces[ i ][ j ].second;
        _search_spaces[ i ][ j ].second = NULL;
      }
    }
    _search_spaces[ i ].clear();
  }

  std::vector< unsigned int > binary_cvs;
  binary_cvs.push_back( CV_FALSE );
  binary_cvs.push_back( CV_TRUE );

  std::vector< unsigned int > ternary_cvs;
  ternary_cvs.push_back( CV_FALSE );
  ternary_cvs.push_back( CV_TRUE );
  ternary_cvs.push_back( CV_INVERTED );

  // add the NP groundings
  for( unsigned int i = 0; i < NUM_REGION_TYPES; i++ ){
    if( i != REGION_TYPE_UNKNOWN ){
      _search_spaces[ PHRASE_NP ].push_back( make_pair< vector< unsigned int >, Grounding* >( binary_cvs, new Region( i, Object() ) ) );
    }
    for( unsigned int j = 0; j < world->objects().size(); j++ ){
      _search_spaces[ PHRASE_NP ].push_back( make_pair< vector< unsigned int >, Grounding* >( binary_cvs, new Region( i, *world->objects()[ j ] ) ) );
    }
  }

  // add the PP groundings
  for( unsigned int i = 0; i < NUM_REGION_TYPES; i++ ){
    if( i != REGION_TYPE_UNKNOWN ){
      _search_spaces[ PHRASE_PP ].push_back( make_pair< vector< unsigned int >, Grounding* >( binary_cvs, new Region( i, Object() ) ) );
    }
    for( unsigned int j = 0; j < world->objects().size(); j++ ){
      _search_spaces[ PHRASE_PP ].push_back( make_pair< vector< unsigned int >, Grounding* >( binary_cvs, new Region( i, *world->objects()[ j ] ) ) );
    }
  }

  // add the VP groundings
  for( unsigned int i = CONSTRAINT_TYPE_INSIDE; i < NUM_CONSTRAINT_TYPES; i++ ){
    for( unsigned int j = 0; j < world->objects().size(); j++ ){
      for( unsigned int k = 0; k < NUM_REGION_TYPES; k++ ){
        for( unsigned int l = 0; l < world->objects().size(); l++ ){
          for( unsigned int m = 0; m < NUM_REGION_TYPES; m++ ){
            if( ( j != l ) || ( k != m ) ){
              _search_spaces[ PHRASE_VP ].push_back( make_pair< vector< unsigned int >, Grounding* >( ternary_cvs, new Constraint( i, Region( k, *world->objects()[ j ] ), Region( m, *world->objects()[ l ] ) ) ) );
            }
          }
        }
      }
    }
  }

  return;
}
  
bool
DCG::
leaf_search( const Phrase* phrase,
              const World* world,
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
    fill_search_spaces( world );

    if( _root != NULL ){
      delete _root;
      _root = NULL;
    }

    _root = new Factor_Set( phrase->dup() );
    _fill_factors( _root, _root->phrase() );  

    Factor_Set * leaf = NULL;
    _find_leaf( _root, leaf );
    while( leaf != NULL ){
      leaf->search( _search_spaces[ leaf->phrase()->type() ],
                    world,
                    llm,
                    beamWidth,
                    debug );
      leaf = NULL;
      _find_leaf( _root, leaf );
    }
  
    for( unsigned int i = 0; i < _root->solutions().size(); i++ ){
      _solutions.push_back( make_pair< double, Phrase* >( _root->solutions()[ i ].pygx, _root->phrase()->dup() ) );

      for( unsigned int j = 0; j < _solutions.back().second->children().size(); j++ ){
        if( _solutions.back().second->children()[ j ] != NULL ){
          delete _solutions.back().second->children()[ j ];
          _solutions.back().second->children()[ j ];
        }
      }
      _solutions.back().second->children().clear();
      if( _solutions.back().second->grounding() != NULL ){
        delete _solutions.back().second->grounding();
        _solutions.back().second->grounding() = NULL;
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
    bool all_children_known = true;
    for( unsigned int i = 0; i < node->children().size(); i++ ){
      if( node->children()[ i ]->solutions().empty() ){
        all_children_known = false;
      } 
    } 
    if( all_children_known ){
      leaf = node;
    }
  }

  for( unsigned int i = 0; i < node->children().size(); i++ ){
    _find_leaf( node->children()[ i ], leaf );
  }
  return;
}

void
DCG::
_fill_phrase( Factor_Set* node,
              Factor_Set_Solution& solution,
              Phrase* phrase ){
  phrase->grounding() = new Grounding_Set();
  for( unsigned int i = 0; i < solution.groundings.size(); i++ ){
    dynamic_cast< Grounding_Set* >( phrase->grounding() )->groundings().push_back( solution.groundings[ i ] );
  }
  for( unsigned int i = 0; i < node->children().size(); i++ ){
    phrase->children().push_back( node->children()[ i ]->phrase()->dup() );
    for( unsigned int j = 0; j < phrase->children().back()->children().size(); j++ ){
      if( phrase->children().back()->children()[ j ] != NULL ){
        delete phrase->children().back()->children()[ j ];
        phrase->children().back()->children()[ j ];
      }
    }
    phrase->children().back()->children().clear();
    if( phrase->children().back()->grounding() != NULL ){
      delete phrase->children().back()->grounding();
      phrase->children().back()->grounding() = NULL;
    }

    _fill_phrase( node->children()[ i ],
                  node->children()[ i ]->solutions()[ solution.children[ i ] ],
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
    node->children().push_back( new Factor_Set( phrase->children()[ i ] ) );
    _fill_factors( node->children().back(), phrase->children()[ i ] );
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
