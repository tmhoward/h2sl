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
  return a.pygx() > b.pygx();
}

Factor_Set_Solution::
Factor_Set_Solution( const vector< unsigned int >& childSolutionIndices,
                      const double& pygx,
                      Grounding_Set* groundingSet ) : _child_solution_indices( childSolutionIndices ),
                                                      _pygx( pygx ),
                                                      _grounding_set( groundingSet ) {

}

Factor_Set_Solution::
~Factor_Set_Solution() {
  if( _grounding_set != NULL ){
    delete _grounding_set;
    _grounding_set = NULL;
  }
}

Factor_Set_Solution::
Factor_Set_Solution( const Factor_Set_Solution& other ) : _child_solution_indices( other._child_solution_indices ),
                                                          _pygx( other._pygx ),
                                                          _grounding_set( new Grounding_Set() ){
  assert( _grounding_set != NULL );
  assert( other._grounding_set != NULL );
  _grounding_set->groundings() = other._grounding_set->groundings();
}

Factor_Set_Solution&
Factor_Set_Solution::
operator=( const Factor_Set_Solution& other ){
  _child_solution_indices = other._child_solution_indices;
  _pygx = other._pygx;
  assert( _grounding_set != NULL );
  assert( other._grounding_set != NULL );
  _grounding_set->groundings() = other._grounding_set->groundings();
  return (*this);
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Factor_Set_Solution& other ) {
    out << "child_solution_indices[" << other.child_solution_indices().size() << "]:{";
    for( unsigned int i = 0; i < other.child_solution_indices().size(); i++ ){
      out << other.child_solution_indices()[ i ];
      if( i != ( other.child_solution_indices().size() - 1 ) ){
        out << ",";
      } 
    }
    out << "} ";
    out << "pygx:" << other.pygx() << " ";
    if( other.grounding_set() != NULL ){
      out << "grounding_set:(" << *other.grounding_set() << ")";
    } else{
      out << "grounding_set:(NULL)";
    }
    return out;
  }
}


Factor_Set::
Factor_Set( const Phrase* phrase ) : _phrase( phrase ),
                                          _child_factor_sets(),
                                          _solutions(),
                                          _properties() {

}

Factor_Set::
~Factor_Set() {

}

Factor_Set::
Factor_Set( const Factor_Set& other ) : _phrase( other._phrase ),
                                                _child_factor_sets( other._child_factor_sets ),
                                                _solutions( other._solutions ),
                                                _properties( other._properties ){

}

Factor_Set&
Factor_Set::
operator=( const Factor_Set& other ) {
  _phrase = other._phrase;
  _child_factor_sets = other._child_factor_sets;
  _solutions = other._solutions;
  return (*this);
}

void
Factor_Set::
search( const Search_Space* searchSpace,
        const Symbol_Dictionary& symbolDictionary,
        const World* world,
        LLM* llm,
        const unsigned int beamWidth,
        const bool& debug ){
  search( searchSpace, symbolDictionary, world, NULL, llm, beamWidth, debug );
  return;
}


// Solution search for a single combination.
void
Factor_Set::
search_subspace( vector< Factor_Set_Solution >& solutionsVector, 
                  const vector< pair< const Phrase*, vector< Grounding* > > >& childPhraseGroundings,                           
                  const pair< string, vector< Grounding* > >& searchSubspace, 
                  const vector< string >& correspondenceVariables,
		  vector < bool >& evaluate_feature_types,
                  const Grounding* context,
                  const World * world, LLM* llm, const unsigned int beamWidth, const bool& debug, const bool& printGroundings ) {
  // Obtain the size of the search space for that class. 
  for( unsigned int i = 0; i < searchSubspace.second.size(); i++ ){
    vector< Factor_Set_Solution > new_solutions_vector;
    // iterate over all solutions
    for( unsigned int j = 0; j < solutionsVector.size(); j++ ){
      // Iterate over the cv values and obtain the log-likelihood values.  
      for( unsigned int k = 0; k < correspondenceVariables.size(); k++ ){
        // Compute the log-likelihood for that factor.
        double value = llm->pygx( correspondenceVariables[ k ], searchSubspace.second[ i ], 
                                  childPhraseGroundings, _phrase, world, context, 
                                  correspondenceVariables, evaluate_feature_types );
        // Language feature sets are evaluated only once for efficiency.
        evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
        // add the new solution to the solutions vector (make a copy of the previous solution)
        new_solutions_vector.push_back( solutionsVector[ j ] );
        if( correspondenceVariables[ k ] == "true" ){
          if( printGroundings ){
            cout << "value:" << value << " grounding:" << *searchSubspace.second[ i ]  << endl;
          }
          new_solutions_vector.back().grounding_set()->groundings().push_back( searchSubspace.second[ i ] );   
        }
        // Update the pygx likelihoods (all multiplied as independent).
        new_solutions_vector.back().pygx() *= value;
      }
    }

    // Sort the solution vector.
    sort( new_solutions_vector.begin(), new_solutions_vector.end(), factor_set_solution_sort );   
    
    // Trim solutions beyond the beam width.
    if( new_solutions_vector.size() > beamWidth ){
      new_solutions_vector.erase( new_solutions_vector.begin() + beamWidth, new_solutions_vector.end() );
    }

    // solution vector is assigned the newly created one.
    solutionsVector = new_solutions_vector;
  }
  
  return;
}


// The main search for solutions.
void
Factor_Set::
search( const Search_Space* searchSpace,
        const Symbol_Dictionary& symbolDictionary,
        const World* world, 
        const Grounding* context, 
        LLM* llm,
        const unsigned int beamWidth,
        const bool& debug ){

  // Create the vector that will have the child solutions indices.	
  vector< vector< unsigned int > > child_solution_indices;
  for( unsigned int i = 0; i < _child_factor_sets.size(); i++ ){
    child_solution_indices.push_back( vector< unsigned int >() );
    assert( i < _child_factor_sets.size() );
    for( unsigned int j = 0; j < _child_factor_sets[ i ]->solutions().size(); j++ ){
      child_solution_indices.back().push_back( j );
    }
  }

  // Create the indices for the child solution combos (power set).
  vector< vector< unsigned int > > child_solution_indices_cartesian_power = cartesian_power( child_solution_indices );
  if( child_solution_indices_cartesian_power.empty() ){
    child_solution_indices_cartesian_power.push_back( vector< unsigned int >() );
  }

  vector< bool > evaluate_feature_types( NUM_FEATURE_TYPES, true );

  if( debug ){
    for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace->grounding_pairs().begin(); it_search_spaces != searchSpace->grounding_pairs().end(); it_search_spaces++ ){
      cout << "searching through symbols for \"" << it_search_spaces->first << "\" (" << it_search_spaces->second.second.size() << " symbols)" << endl;
    }
  }

  unsigned int concrete_size = 0;
  unsigned int abstract_avg_size = 0;  
  unsigned int abstract_max_size = 0;  

  // Record the search space size.
  for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace->grounding_pairs().begin(); it_search_spaces != searchSpace->grounding_pairs().end(); it_search_spaces++ ){
    if( ( it_search_spaces->first == "container" ) || ( it_search_spaces->first == "region_container" ) || ( it_search_spaces->first == "abstract_container" ) || ( it_search_spaces->first == "region_abstract_container" ) || ( it_search_spaces->first == "object_property" ) ){
      abstract_avg_size += it_search_spaces->second.second.size();
      abstract_max_size += it_search_spaces->second.second.size();
    } else {
      concrete_size += it_search_spaces->second.second.size(); 
    } 
  }
  insert_prop< std::string >( _properties, "concrete_size", to_std_string( concrete_size ) ); 
  insert_prop< std::string >( _properties, "abstract_avg_size", to_std_string( abstract_avg_size ) ); 
  insert_prop< std::string >( _properties, "abstract_max_size", to_std_string( abstract_max_size ) ); 

  vector< vector< Factor_Set_Solution > > solutions_vector;
  // add a Factor_Set_Solution for each combination of children
  for( unsigned int i = 0; i < child_solution_indices_cartesian_power.size(); i++ ){
    solutions_vector.push_back( vector< Factor_Set_Solution >() );

    // start with an empty solution vector, set the childen equal to the children generated by the cartesian power
    solutions_vector.back().push_back( Factor_Set_Solution() );
    solutions_vector.back().back().child_solution_indices() = child_solution_indices_cartesian_power[ i ];

    // generate the child groundings for the cartesian power
    vector< pair< const Phrase*, vector< Grounding* > > > child_groundings;
    assert( i < child_solution_indices_cartesian_power.size() );
    for( unsigned int j = 0; j < child_solution_indices_cartesian_power[ i ].size(); j++ ){
      assert( j < child_solution_indices_cartesian_power[ i ].size() );
      assert( j < _child_factor_sets.size() );
      assert( ( child_solution_indices_cartesian_power[ i ][ j ] ) < _child_factor_sets[ j ]->solutions().size() );
      solutions_vector.back().back().pygx() *= _child_factor_sets[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].pygx();
      child_groundings.push_back( pair< const Phrase*, vector< Grounding* > >( _child_factor_sets[ j ]->phrase(), _child_factor_sets[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].grounding_set()->groundings() ) );
    }

    // search over all of the class-based search spaces
    for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace->grounding_pairs().begin(); it_search_spaces != searchSpace->grounding_pairs().end(); it_search_spaces++ ){
      map< string, vector< string > >::const_iterator it_cvs = searchSpace->cvs().find( it_search_spaces->second.first );
      assert( it_cvs != searchSpace->cvs().end() );

      search_subspace( solutions_vector.back(), 
                        child_groundings,
                        it_search_spaces->second,
                        it_cvs->second, 
                        evaluate_feature_types,
                        context, world, llm, beamWidth, debug ); 
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

  // sort and cull solutions
  sort( _solutions.begin(), _solutions.end(), factor_set_solution_sort );
  if( _solutions.size() > beamWidth ){
    _solutions.erase( _solutions.begin() + beamWidth, _solutions.end() );
  }

  if( debug ){
    for( unsigned int i = 0; i < _solutions.size(); i++ ){
      cout << "solution[" << i << "] (pygx:" << _solutions[ i ].pygx() << ")" << endl;
      cout << "  grounding_set" << *_solutions[ i ].grounding_set() << endl;
      cout << "  child_solution_indices[" << _solutions[ i ].child_solution_indices().size() << "]:{";
      for( unsigned int j = 0; j < _solutions[ i ].child_solution_indices().size(); j++ ){
        cout << "(\"" << _child_factor_sets[ j ]->phrase()->words_to_std_string() << "\":" << _solutions[ i ].child_solution_indices()[ j ] << ")";
        if( j != ( _solutions[ i ].child_solution_indices().size() - 1 ) ){
          cout << ",";
        }
      }
      cout << "}" << endl;
      cout << "  child grounding_sets[" << _solutions[ i ].child_solution_indices().size() << "]:{";
      for( unsigned int j = 0; j < _solutions[ i ].child_solution_indices().size(); j++ ){
        cout <<  "(\"" << _child_factor_sets[ j ]->phrase()->words_to_std_string() << "\":(groundings" << *_child_factor_sets[ j ]->solutions()[ _solutions[ i ].child_solution_indices()[ j ] ].grounding_set() << "))";
        if( j != ( _solutions[ i ].child_solution_indices().size() - 1 ) ){
          cout << ",";
        }
      }
      cout << "}" << endl;
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
