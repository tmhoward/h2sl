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
 * The implementation of a class used to represent the Distributed Correspondence Graph
 */

#include <iomanip>
#include <sstream>
#include <algorithm>

#include <h2sl/object.h>
#include <h2sl/object_set.h>
#include <h2sl/region.h>
#include <h2sl/region_set.h>
#include <h2sl/constraint.h>
#include <h2sl/constraint_set.h>
#include "h2sl/cv.h"
#include <h2sl/dcg.h>

using namespace std;
using namespace h2sl;

struct solution_compare {
  bool
  operator()( const pair< double, vector< unsigned int > >& a,
                  const pair< double, vector< unsigned int > >& b ){
    return ( a.first > b.first );
  }
};

DCG::
DCG() : _phrase( NULL ),
        _phrases(),
        _factors() {

}

DCG::
~DCG() {

}

DCG::
DCG( const DCG& other ) : _phrase( other._phrase ),
                          _phrases( other._phrases ),
                          _factors( other._factors ) {

}

DCG&
DCG::
operator=( const DCG& other ) {
  _phrase = other._phrase;
  _phrases = other._phrases;
  _factors = other._factors;
  return (*this);
}

void
DCG::
construct( Phrase& phrase,
            const World& world,
            LLM& llm,
            const bool& fill ){
  _phrases.clear();
  _factors.clear();
  if( _phrase != NULL ){
    delete _phrase;
    _phrase = NULL;
  }  

  _phrase = phrase.dup();

  _add_phrase_to_model( *_phrase, world, llm, fill );
  _connect_children( *_phrase );
  return;
}

bool
DCG::
search( Phrase& phrase,
        const World& world,
        LLM& llm,
        vector< pair< double, Phrase > >& solutions,
        const unsigned int& beamWidth ){
  solutions.clear();

  construct( phrase, world, llm );

  vector< pair< double, vector< unsigned int > > > solution_cvs( 1 );
  for( unsigned int i = 0; i < solution_cvs.size(); i++ ){
    solution_cvs[ i ].first = 1.0;
    solution_cvs[ i ].second.resize( _factors.size() );
  }

  while( step( solution_cvs, beamWidth ) ){

  }

  for( unsigned int i = 0; i < solution_cvs.size(); i++ ){
    solutions.push_back( make_pair< double, Phrase >( solution_cvs[ i ].first, phrase ) );  
    for( unsigned int j = 0; j < _factors.size(); j++ ){
      _factors[ j ]->cv() = solution_cvs[ i ].second[ j ];
    }
    _fill_phrase_from_model( solutions.back().second );
  }
  
  return true;
}

bool
DCG::
step( vector< pair< double, vector< unsigned int > > >& solutions,
      const unsigned int& beamWidth ){
  int search_factor = -1;

  for( unsigned int i = 0; i < _factors.size(); i++ ){
    if( solutions.front().second[ i ] == 0 ){
      unsigned int num_unknown_children = 0;
      for( unsigned int j = 0; j < _factors[ i ]->children().size(); j++ ){
        if( solutions.front().second[ _factors[ i ]->children()[ j ]->solution_index() ] == 0 ){
          num_unknown_children++;
        }
      }
      if( num_unknown_children == 0 ){
        search_factor = i;
      }
    }
  }
  
  if( search_factor == -1 ){
    return false;
  }
  
  unsigned int num_solutions = solutions.size();
  for( unsigned int i = 1; i < _factors[ search_factor ]->cvs().size(); i++ ){
    for( unsigned int j = 0; j < num_solutions; j++ ){
      solutions.push_back( solutions[ j ] ); 
    }
  }

  for( unsigned int i = 0; i < _factors[ search_factor ]->cvs().size(); i++ ){
    for( unsigned int j = 0; j < num_solutions; j++ ){
      solutions[ i * num_solutions + j ].second[ search_factor ] = _factors[ search_factor ]->cvs()[ i ];
      score( solutions[ i * num_solutions + j ], search_factor );
    }
  }

  sort( solutions.begin(), solutions.end(), solution_compare() );

  if( solutions.size() > beamWidth ){
    solutions.erase( solutions.begin() + beamWidth, solutions.end() );
  }
 
  return true;
}

void
DCG::
score( pair< double, vector< unsigned int > >& solution,
        const unsigned int searchIndex ){
  for( unsigned int i = 0; i < _factors.size(); i++ ){
    _factors[ i ]->cv() = solution.second[ i ];
  }

  solution.first *= _factors[ searchIndex ]->value( solution.second[ searchIndex ] );
  return;
}

void
DCG::
_add_vp_phrase_to_model( Phrase& phrase,
                        const World& world,
                        LLM& llm,
                        const bool& fill ){
  Constraint_Set * constraint_set = NULL;
  if( fill ){
    constraint_set = dynamic_cast< Constraint_Set* >( phrase.grounding() );
  }

  vector< Constraint* > new_groundings;
  for( unsigned int i = CONSTRAINT_TYPE_INSIDE; i < NUM_CONSTRAINT_TYPES; i++ ){
    for( unsigned int j = 0; j < world.object_set().objects().size(); j++ ){
      for( unsigned int k = 0; k < NUM_REGION_TYPES; k++ ){
        for( unsigned int l = 0; l < world.object_set().objects().size(); l++ ){
          for( unsigned int m = 0; m < NUM_REGION_TYPES; m++ ){
            if( ( j != l ) || ( k != m ) ){
              new_groundings.push_back( new Constraint( i, Region( k, world.object_set().objects()[ j ] ), Region( m, world.object_set().objects()[ l ] ) ) );
            }
          }
        }
      }
    }
  }

  vector< unsigned int > cvs;
  cvs.push_back( CV_FALSE );  
  cvs.push_back( CV_TRUE );

  for( unsigned int i = 0; i < new_groundings.size(); i++ ){
    if( fill && ( constraint_set != NULL ) ){
      bool found_match = false;
      for( unsigned int j = 0; j < constraint_set->constraints().size(); j++ ){
        if( constraint_set->constraints()[ j ] == *new_groundings[ i ] ){
          found_match = true;
        }
      }
      if( found_match ){
        _factors.push_back( new Factor( CV_TRUE, new_groundings[ i ], _phrases.back(), vector< Factor* >(), &llm, cvs, _factors.size() ) );
      } else {
        _factors.push_back( new Factor( CV_FALSE, new_groundings[ i ], _phrases.back(), vector< Factor* >(), &llm, cvs, _factors.size() ) );
      }
    } else {
      _factors.push_back( new Factor( 0, new_groundings[ i ], _phrases.back(), vector< Factor* >(), &llm, cvs, _factors.size() ) );
    }
  }

  return;
}

void
DCG::
_add_pp_phrase_to_model( Phrase& phrase,
                        const World& world,
                        LLM& llm,
                        const bool& fill ){
  Region_Set * region_set = NULL;
  if( fill ){
    region_set = dynamic_cast< Region_Set* >( phrase.grounding() );
  }

  vector< Region* > new_groundings;
  for( unsigned int i = REGION_TYPE_UNKNOWN; i < NUM_REGION_TYPES; i++ ){
    if( i != REGION_TYPE_UNKNOWN ){
      new_groundings.push_back( new Region( i, Object() ) );  
    }
    for( unsigned int j = 0; j < world.object_set().objects().size(); j++ ){
      new_groundings.push_back( new Region( i, world.object_set().objects()[ j ] ) );
    }
  }

  vector< unsigned int > cvs;
  cvs.push_back( CV_FALSE );
  cvs.push_back( CV_TRUE );

  for( unsigned int i = 0; i < new_groundings.size(); i++ ){
    if( fill && ( region_set != NULL ) ){
      bool found_match = false;
      for( unsigned int j = 0; j < region_set->regions().size(); j++ ){
        if( region_set->regions()[ j ] == *new_groundings[ i ] ){
          found_match = true;
        }
      }
      if( found_match ){
        _factors.push_back( new Factor( CV_TRUE, new_groundings[ i ], _phrases.back(), vector< Factor* >(), &llm, cvs, _factors.size() ) );
      } else {
        _factors.push_back( new Factor( CV_FALSE, new_groundings[ i ], _phrases.back(), vector< Factor* >(), &llm, cvs, _factors.size() ) );
      }
    } else {
      _factors.push_back( new Factor( 0, new_groundings[ i ], _phrases.back(), vector< Factor* >(), &llm, cvs, _factors.size() ) );
    }
  }


  return;
}

void
DCG::
_add_np_phrase_to_model( Phrase& phrase,
                        const World& world,
                        LLM& llm,
                        const bool& fill ){
  Region_Set * region_set = NULL;
  if( fill ){
    region_set = dynamic_cast< Region_Set* >( phrase.grounding() );
  }

  vector< Region* > new_groundings;
  for( unsigned int i = REGION_TYPE_UNKNOWN; i < NUM_REGION_TYPES; i++ ){
    if( i != REGION_TYPE_UNKNOWN ){
      new_groundings.push_back( new Region( i, Object() ) );    
    }
    for( unsigned int j = 0; j < world.object_set().objects().size(); j++ ){
      new_groundings.push_back( new Region( i, world.object_set().objects()[ j ] ) );
    }
  }

  vector< unsigned int > cvs;
  cvs.push_back( CV_FALSE );
  cvs.push_back( CV_TRUE );

  for( unsigned int i = 0; i < new_groundings.size(); i++ ){
    if( fill && ( region_set != NULL ) ){
      bool found_match = false;
      for( unsigned int j = 0; j < region_set->regions().size(); j++ ){
        if( region_set->regions()[ j ] == *new_groundings[ i ] ){
          found_match = true;
        }
      }
      if( found_match ){
        _factors.push_back( new Factor( CV_TRUE, new_groundings[ i ], _phrases.back(), vector< Factor* >(), &llm, cvs, _factors.size() ) );
      } else {
        _factors.push_back( new Factor( CV_FALSE, new_groundings[ i ], _phrases.back(), vector< Factor* >(), &llm, cvs, _factors.size() ) );
      }
    } else {
      _factors.push_back( new Factor( 0, new_groundings[ i ], _phrases.back(), vector< Factor* >(), &llm, cvs, _factors.size() ) );
    }
  }

  return;
}

void
DCG::
_add_phrase_to_model( Phrase& phrase,
                      const World& world,
                      LLM& llm,
                      const bool& fill ){
  _phrases.push_back( &phrase );

  switch( phrase.phrase() ){
  case( PHRASE_VP ):
    _add_vp_phrase_to_model( phrase, world, llm, fill );
    break;
  case( PHRASE_PP ):
    _add_pp_phrase_to_model( phrase, world, llm, fill );
    break;
  case( PHRASE_NP ):
    _add_np_phrase_to_model( phrase, world, llm, fill );
    break;
  default:
    break;
  }

  for( unsigned int i = 0; i < phrase.children().size(); i++ ){
    _add_phrase_to_model( phrase.children()[ i ], world, llm, fill );
  }

  return;
}

void
DCG::
_connect_children( Phrase& phrase ){
  for( unsigned int i = 0; i < phrase.children().size(); i++ ){
    for( unsigned int j = 0; j < _factors.size(); j++ ){
      if( _factors[ j ]->phrase() == &phrase ){
        for( unsigned int k = 0; k < _factors.size(); k++ ){
          if( _factors[ k ]->phrase() == &phrase.children()[ i ] ){
            _factors[ j ]->children().push_back( _factors[ k ] );
          }
        }
      }
    } 
    _connect_children( phrase.children()[ i ] );  
  }
  return;
}

void 
DCG::
_fill_phrase_from_model( Phrase& phrase ){
  switch( phrase.phrase() ){
  case( PHRASE_VP ):
    _fill_vp_phrase_from_model( phrase );
    break;
  case( PHRASE_PP ):
    _fill_pp_phrase_from_model( phrase );
    break;
  case( PHRASE_NP ):
    _fill_np_phrase_from_model( phrase );
    break;
  default:
    break;
  }

  for( unsigned int i = 0; i < phrase.children().size(); i++ ){
    _fill_phrase_from_model( phrase.children()[ i ] );
  }
  return;
}

void 
DCG::
_fill_vp_phrase_from_model( Phrase& phrase ){
  if( phrase.grounding() != NULL ){
    delete phrase.grounding(); 
    phrase.grounding() = NULL;
  }
  Constraint_Set * constraint_set = new Constraint_Set();
  for( unsigned int i = 0; i < _factors.size(); i++ ){
    if( _factors[ i ]->cv() == CV_TRUE ){
      if( dynamic_cast< Constraint* >( _factors[ i ]->grounding() ) != NULL ){
        if( *_factors[ i ]->phrase() == phrase ){
          constraint_set->constraints().push_back( *static_cast< Constraint* >( _factors[ i ]->grounding()->dup() ) );
        }
      }
    }
  }
  phrase.grounding() = constraint_set;
  return;
}

void 
DCG::
_fill_pp_phrase_from_model( Phrase& phrase ){
  if( phrase.grounding() != NULL ){
    delete phrase.grounding(); 
    phrase.grounding() = NULL;
  }
  Region_Set * region_set = new Region_Set();
  for( unsigned int i = 0; i < _factors.size(); i++ ){
    if( _factors[ i ]->cv() == CV_TRUE ){
      if( dynamic_cast< Region* >( _factors[ i ]->grounding() ) != NULL ){
        if( *_factors[ i ]->phrase() == phrase ){
          region_set->regions().push_back( *static_cast< Region* >( _factors[ i ]->grounding()->dup() ) );
        }
      }
    }
  }
  phrase.grounding() = region_set;
  return;
}

void DCG::
_fill_np_phrase_from_model( Phrase& phrase ){
  if( phrase.grounding() != NULL ){
    delete phrase.grounding();
    phrase.grounding() = NULL;
  }
  Region_Set * region_set = new Region_Set();
  for( unsigned int i = 0; i < _factors.size(); i++ ){
    if( _factors[ i ]->cv() == CV_TRUE ){
      if( dynamic_cast< Region* >( _factors[ i ]->grounding() ) != NULL ){
        if( *_factors[ i ]->phrase() == phrase ){
          region_set->regions().push_back( *static_cast< Region* >( _factors[ i ]->grounding()->dup() ) );
        }
      }
    }
  }
  phrase.grounding() = region_set;
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const DCG& other ) {
    out << "phrases[" << other.phrases().size() << "]:{";
    for( unsigned int i = 0; i < other.phrases().size(); i++ ){
      out << "\"" << other.phrases()[ i ]->words_to_std_string() << "\"";
      if( i != ( other.phrases().size() - 1 ) ){
        out << ",";
      }
    }
    out << "} ";
    out << "factors[" << other.factors().size() << "]:{";
    for( unsigned int i = 0; i < other.factors().size(); i++ ){
      out << *other.factors()[ i ];
      if( i != ( other.factors().size() - 1 ) ){
        out << ",";
      }
    }
    out << "} ";
    return out;
  }
}
