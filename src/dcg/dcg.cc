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

#include "h2sl/grounding_set.h"

#include "h2sl/object_property.h"
#include "h2sl/object_type.h"
#include "h2sl/object_color.h"
#include "h2sl/object.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/container.h"
#include "h2sl/region_container.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_abstract_container.h"

#include "h2sl/dcg.h"

using namespace std;
using namespace h2sl;

DCG::
DCG() : _search_space(),
        _solutions(),
        _root( NULL ) {

}

DCG::
~DCG() {

}

DCG::
DCG( const DCG& other ) : _search_space( other._search_space ),
                          _solutions( other._solutions ),
                          _root( other._root ) {

}

DCG&
DCG::
operator=( const DCG& other ) {
  _search_space = other._search_space;
  _solutions = other._solutions;
  _root = other._root;
  return (*this);
}

void
DCG::
scrape_examples( const string& filename,
                  const Phrase* phrase,
                  const h2sl::World* world,
                  const Search_Space& searchSpace,
                  vector< pair< unsigned int, h2sl::LLM_X > >& examples ){
//  for( unsigned int i = 0; i < searchSpaces.size(); i++ ){
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
    scrape_examples( filename, dynamic_cast< Phrase* >( phrase->children()[ i ] ), world, searchSpace, examples );
  }
  return;
}

void
DCG::
fill_search_spaces( const World* world ){
  _search_space.clear();

  std::vector< unsigned int > binary_cvs;
  binary_cvs.push_back( CV_FALSE );
  binary_cvs.push_back( CV_TRUE );

  std::vector< unsigned int > ternary_cvs;
  ternary_cvs.push_back( CV_FALSE );
  ternary_cvs.push_back( CV_TRUE );
  ternary_cvs.push_back( CV_INVERTED );

  _search_space.cvs().insert( pair< string, vector< unsigned int > >( "binary", binary_cvs ) );
  _search_space.cvs().insert( pair< string, vector< unsigned int > >( "ternary", ternary_cvs ) );

  Object::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Object_Type::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Object_Color::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Object_Property::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Number::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Index::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Region::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Spatial_Relation::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Constraint::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Abstract_Container::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Region_Abstract_Container::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Container::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );
  Region_Container::fill_search_space( _symbol_dictionary, world, _search_space.grounding_pairs(), SYMBOL_TYPE_ALL );

  return;
}

bool
DCG::
leaf_search( const Phrase* phrase,
              const World* world,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  return leaf_search( phrase, world, NULL, llm, beamWidth, debug );
}
  
bool
DCG::
leaf_search( const Phrase* phrase,
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
      leaf->search( _search_space,
                    _symbol_dictionary,
                    world,
                    context,
                    llm,
                    beamWidth,
                    debug );
      leaf = NULL;
      _find_leaf( _root, leaf );
    }
  
    for( unsigned int i = 0; i < _root->solutions().size(); i++ ){
      _solutions.push_back( pair< double, Phrase* >( _root->solutions()[ i ].pygx, _root->phrase()->dup() ) );

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
  phrase->grounding_set() = new Grounding_Set();
  for( unsigned int i = 0; i < solution.groundings->groundings().size(); i++ ){
    dynamic_cast< Grounding_Set* >( phrase->grounding_set() )->groundings().push_back( solution.groundings->groundings()[ i ] );
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
    if( phrase->children().back()->grounding_set() != NULL ){
      delete phrase->children().back()->grounding_set();
      phrase->children().back()->grounding_set() = NULL;
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
