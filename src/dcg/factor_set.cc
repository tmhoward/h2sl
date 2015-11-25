/**
 * @file    factor_set.cc
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
 * The implementation of a class used to represent a factor set
 */

#include "h2sl/common.h"
#include "h2sl/constraint.h"
#include "h2sl/factor_set.h"

using namespace std;
using namespace h2sl;

bool
factor_set_solution_sort( const Factor_Set_Solution& a,
                              const Factor_Set_Solution& b ){
  return a.pygx > b.pygx;
}

Factor_Set_Solution::
Factor_Set_Solution() : cv(),
                            children(),
                            groundings(),
                            pygx( 1.0 ) {

}

Factor_Set_Solution::
~Factor_Set_Solution() {

}

Factor_Set_Solution::
Factor_Set_Solution( const Factor_Set_Solution& other ) : cv( other.cv ),
                                                                  children( other.children ),
                                                                  groundings( other.groundings ),
                                                                  pygx( other.pygx ){

}

Factor_Set_Solution&
Factor_Set_Solution::
operator=( const Factor_Set_Solution& other ){
  cv = other.cv;
  children = other.children;
  groundings = other.groundings;
  pygx = other.pygx;
  return (*this);
}

Factor_Set::
Factor_Set( const Phrase* phrase ) : _phrase( phrase ),
                                          _children(),
                                          _solutions() {

}

Factor_Set::
~Factor_Set() {

}

Factor_Set::
Factor_Set( const Factor_Set& other ) : _phrase( other._phrase ),
                                                _children( other._children ),
                                                _solutions( other._solutions ){

}

Factor_Set&
Factor_Set::
operator=( const Factor_Set& other ) {
  _phrase = other._phrase;
  _children = other._children;
  _solutions = other._solutions;
  return (*this);
}

void
Factor_Set::
search( const vector< pair< unsigned int, Grounding* > >& searchSpace,
        const vector< vector< unsigned int > >& correspondenceVariables,
        const World* world, 
        LLM* llm,
        const unsigned int beamWidth,
        const bool& debug ){

  vector< vector< unsigned int > > child_solution_indices;
  for( unsigned int i = 0; i < _children.size(); i++ ){
    child_solution_indices.push_back( vector< unsigned int >() );
    for( unsigned int j = 0; j < _children[ i ]->solutions().size(); j++ ){
      child_solution_indices.back().push_back( j );
    }
  }

  vector< vector< unsigned int > > child_solution_indices_cartesian_power = cartesian_power( child_solution_indices );
  if( child_solution_indices_cartesian_power.empty() ){
    child_solution_indices_cartesian_power.push_back( vector< unsigned int >() );
  }

  vector< bool > evaluate_feature_types( NUM_FEATURE_TYPES, true );

  vector< vector< Factor_Set_Solution > > solutions_vector;
  for( unsigned int i = 0; i < child_solution_indices_cartesian_power.size(); i++ ){
    solutions_vector.push_back( vector< Factor_Set_Solution >() );
    solutions_vector.back().push_back( Factor_Set_Solution() );
    solutions_vector.back().back().children = child_solution_indices_cartesian_power[ i ];
    solutions_vector.back().back().cv.resize( NUM_CVS );

    vector< pair< const Phrase*, vector< Grounding* > > > child_groundings;
    for( unsigned int j = 0; j < child_solution_indices_cartesian_power[ i ].size(); j++ ){
      solutions_vector.back().back().pygx *= _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].pygx;
      child_groundings.push_back( pair< const Phrase*, vector< Grounding* > >( _children[ j ]->phrase(), vector< Grounding* >() ) );
      for( unsigned int k = 0; k < _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].groundings.size(); k++ ){
        child_groundings.back().second.push_back( _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].groundings[ k ] );
      }
    }

/*
    vector< Grounding* > child_groundings;
    for( unsigned int j = 0; j < child_solution_indices_cartesian_power[ i ].size(); j++ ){
      solutions_vector.back().back().pygx *= _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].pygx;
      for( unsigned int k = 0; k < _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].groundings.size(); k++ ){
        child_groundings.push_back( _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].groundings[ k ] );
      }
    }
*/    
    for( unsigned int j = 0; j < searchSpace.size(); j++ ){
      unsigned int num_solutions = solutions_vector.back().size();
      for( unsigned int k = 1; k < correspondenceVariables[ searchSpace[ j ].first ].size(); k++ ){
        for( unsigned int l = 0; l < num_solutions; l++ ){
          solutions_vector.back().push_back( solutions_vector.back()[ l ] );
        } 
      }
  
      for( unsigned int k = 0; k < correspondenceVariables[ searchSpace[ j ].first ].size(); k++ ){
        double value = llm->pygx( correspondenceVariables[ searchSpace[ j ].first ][ k ], searchSpace[ j ].second, child_groundings, _phrase, world, correspondenceVariables[ searchSpace[ j ].first ], evaluate_feature_types );
        evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
        for( unsigned int l = 0; l < num_solutions; l++ ){
          solutions_vector.back()[ k * num_solutions + l ].cv[ correspondenceVariables[ searchSpace[ j ].first ][ k ] ].push_back( j ); 
          solutions_vector.back()[ k * num_solutions + l ].pygx *= value; 
        }
      }
      
      sort( solutions_vector.back().begin(), solutions_vector.back().end(), factor_set_solution_sort );   
      
      if( solutions_vector.back().size() > beamWidth ){
        solutions_vector.back().erase( solutions_vector.back().begin() + beamWidth, solutions_vector.back().end() );
      }
    }
  }

  // flatten solutions
  _solutions.clear();
  for( unsigned int i = 0; i < solutions_vector.size(); i++ ){
    for( unsigned int j = 0; j < solutions_vector[ i ].size(); j++ ){
      _solutions.push_back( solutions_vector[ i ][ j ] );
    }
  } 

  if( debug ){
    cout << "  sorting through " << _solutions.size() << " solutions for \"" << _phrase->words_to_std_string() << "\"" << endl;
  }

  sort( _solutions.begin(), _solutions.end(), factor_set_solution_sort );
  if( _solutions.size() > beamWidth ){
    _solutions.erase( _solutions.begin() + beamWidth, _solutions.end() );
  }

  for( unsigned int i = 0; i < _solutions.size(); i++ ){
    for( unsigned int j = 0; j < _solutions[ i ].cv[ CV_TRUE ].size(); j++ ){
      _solutions[ i ].groundings.push_back( searchSpace[ _solutions[ i ].cv[ CV_TRUE ][ j ] ].second );
    }
  }

  if( debug ){
    for( unsigned int i = 0; i < _solutions.size(); i++ ){
      cout << "solutions[" << i << "] TRUE [" << _solutions[ i ].cv[ CV_TRUE ].size() << "]:{";
      for( unsigned int j = 0; j < _solutions[ i ].cv[ CV_TRUE ].size(); j++ ){
        cout << _solutions[ i ].cv[ CV_TRUE ][ j ];
        if( j != ( _solutions[ i ].cv[ CV_TRUE ].size() - 1 ) ){
          cout << ",";
        }
      }
      cout << "} ";
      cout << "groundings[" << _solutions[ i ].groundings.size() << "]:{";
      cout << "} ";
      cout << "children[" << _solutions[ i ].children.size() << "]:{";
      for( unsigned int j = 0; j < _solutions[ i ].children.size(); j++ ){
        cout << _solutions[ i ].children[ j ];
        if( j != ( _solutions[ i ].children.size() - 1 ) ){
          cout << ",";
        }
      }
      cout << "} ";
      cout << "pygx:" << _solutions[ i ].pygx << endl;
    }  
  }

  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Factor_Set& other ) {
    out << endl;
    out << "phrase:\"" << other.phrase() << "\" ";
    return out;
  }
}
