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
                            groundings( NULL ),
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
search( const Search_Space& searchSpace,
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
search_solution_combination( const unsigned int child_combo_index, 
                             vector< vector< Factor_Set_Solution > >& solutions_vector, 
		             const vector< vector< unsigned int > >& child_solution_indices_cartesian_power, 
                             const Search_Space& searchSpace,
			     vector < bool >& evaluate_feature_types,
                             const Grounding* context,
                             const World * world, LLM* llm, const unsigned int beamWidth, const bool& debug) {

  // Push back the factor set solution in the solutions vector.
  solutions_vector.push_back( vector< Factor_Set_Solution >() );
  solutions_vector.back().push_back( Factor_Set_Solution() );
  solutions_vector.back().back().children = child_solution_indices_cartesian_power[ child_combo_index ];
  solutions_vector.back().back().cv.clear();
  
  // Iterate over the search space arranged by classes. FIll the cv vector in the factor set.  
  for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace.grounding_pairs().begin(); it_search_spaces != searchSpace.grounding_pairs().end(); it_search_spaces++ ){
    solutions_vector.back().back().cv.insert( pair< string, vector< vector< unsigned int > > >( it_search_spaces->first, vector< vector< unsigned int > >( NUM_CVS ) ) );
  }

  // Update the pygx prob in the current solutions vector  based on the child groundings.
  // Update the child groundings vector.
  vector< pair< const Phrase*, vector< Grounding* > > > child_groundings;
  for( unsigned int j = 0; j < child_solution_indices_cartesian_power[ child_combo_index ].size(); j++ ){
    solutions_vector.back().back().pygx *= _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ child_combo_index ][ j ] ].pygx;
    child_groundings.push_back( pair< const Phrase*, vector< Grounding* > >( _children[ j ]->phrase(), vector< Grounding* >() ) );
    for( unsigned int k = 0; k < _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ child_combo_index ][ j ] ].groundings->groundings().size(); k++ ){
      child_groundings.back().second.push_back( _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ child_combo_index ][ j ] ].groundings->groundings()[ k ] );
    }
  }

  // Iterate over each class in the search space.
  for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace.grounding_pairs().begin(); it_search_spaces != searchSpace.grounding_pairs().end(); it_search_spaces++ ){
    // Obtain the size of the search space for that class. 
    for( unsigned int j = 0; j < it_search_spaces->second.second.size(); j++ ){
      unsigned int num_solutions = solutions_vector.back().size();
      // Iterate over the search space cvs.   
      map< string, vector< unsigned int > >::const_iterator it_cvs = searchSpace.cvs().find( it_search_spaces->second.first );
      assert( it_cvs != searchSpace.cvs().end() );
      for( unsigned int k = 1; k < it_cvs->second.size(); k++ ){
        for( unsigned int l = 0; l < num_solutions; l++ ){
          solutions_vector.back().push_back( solutions_vector.back()[ l ] );
        } 
      }
      // Iterate over the cv values and obtain the log-likelihood values.  
      for( unsigned int k = 0; k < it_cvs->second.size(); k++ ){
        // Compute the log-likelihood for that factor.
        double value = llm->pygx( it_cvs->second[ k ], it_search_spaces->second.second[ j ], child_groundings, _phrase, world, context, it_cvs->second, evaluate_feature_types );
        evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
       
        // Iterate and update the pygx with the above likelihoods. 
        for( unsigned int l = 0; l < num_solutions; l++ ){
          map< string, vector< vector< unsigned int > > >::iterator it_cvs_solution = solutions_vector.back()[ k * num_solutions + l ].cv.find( it_search_spaces->first );
          assert( it_cvs_solution != solutions_vector.back()[ k * num_solutions + l ].cv.end() ); 
          it_cvs_solution->second[ it_cvs->second[ k ] ].push_back( j );
          solutions_vector.back()[ k * num_solutions + l ].pygx *= value; 
        }
      }

      // Sort the solution vector.
      sort( solutions_vector.back().begin(), solutions_vector.back().end(), factor_set_solution_sort );   
    
      // Trim solutions beyond the beam width.
      if( solutions_vector.back().size() > beamWidth ){
        solutions_vector.back().erase( solutions_vector.back().begin() + beamWidth, solutions_vector.back().end() );
      }
    }
  }

  return;

/*
 
  // Code extracted from the main function. To be deleted.


  for( unsigned int i = 0; i < child_solution_indices_cartesian_power.size(); i++ ){
    solutions_vector.push_back( vector< Factor_Set_Solution >() );
    solutions_vector.back().push_back( Factor_Set_Solution() );
    solutions_vector.back().back().children = child_solution_indices_cartesian_power[ i ];
    solutions_vector.back().back().cv.clear();
    for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace.grounding_pairs().begin(); it_search_spaces != searchSpace.grounding_pairs().end(); it_search_spaces++ ){
      solutions_vector.back().back().cv.insert( pair< string, vector< vector< unsigned int > > >( it_search_spaces->first, vector< vector< unsigned int > >( NUM_CVS ) ) );
//      solutions_vector.back().back().cv.resize( NUM_CVS );
    }

    vector< pair< const Phrase*, vector< Grounding* > > > child_groundings;
    for( unsigned int j = 0; j < child_solution_indices_cartesian_power[ i ].size(); j++ ){
      solutions_vector.back().back().pygx *= _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].pygx;
      child_groundings.push_back( pair< const Phrase*, vector< Grounding* > >( _children[ j ]->phrase(), vector< Grounding* >() ) );
      for( unsigned int k = 0; k < _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].groundings->groundings().size(); k++ ){
        child_groundings.back().second.push_back( _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].groundings->groundings()[ k ] );
      }
    }

    for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace.grounding_pairs().begin(); it_search_spaces != searchSpace.grounding_pairs().end(); it_search_spaces++ ){
      for( unsigned int j = 0; j < it_search_spaces->second.second.size(); j++ ){
        unsigned int num_solutions = solutions_vector.back().size();
        map< string, vector< unsigned int > >::const_iterator it_cvs = searchSpace.cvs().find( it_search_spaces->second.first );
        assert( it_cvs != searchSpace.cvs().end() );
        for( unsigned int k = 1; k < it_cvs->second.size(); k++ ){
          for( unsigned int l = 0; l < num_solutions; l++ ){
            solutions_vector.back().push_back( solutions_vector.back()[ l ] );
          } 
        }
        for( unsigned int k = 0; k < it_cvs->second.size(); k++ ){
          double value = llm->pygx( it_cvs->second[ k ], it_search_spaces->second.second[ j ], child_groundings, _phrase, world, context, it_cvs->second, evaluate_feature_types );
          evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
          for( unsigned int l = 0; l < num_solutions; l++ ){
            map< string, vector< vector< unsigned int > > >::iterator it_cvs_solution = solutions_vector.back()[ k * num_solutions + l ].cv.find( it_search_spaces->first );
            assert( it_cvs_solution != solutions_vector.back()[ k * num_solutions + l ].cv.end() ); 
            it_cvs_solution->second[ it_cvs->second[ k ] ].push_back( j );
            solutions_vector.back()[ k * num_solutions + l ].pygx *= value; 
          }
        }

        sort( solutions_vector.back().begin(), solutions_vector.back().end(), factor_set_solution_sort );   
      
        if( solutions_vector.back().size() > beamWidth ){
          solutions_vector.back().erase( solutions_vector.back().begin() + beamWidth, solutions_vector.back().end() );
        }
      }
    }
  }

*/
}


// The main search for solutions.
void
Factor_Set::
search( const Search_Space& searchSpace,
        const Symbol_Dictionary& symbolDictionary,
        const World* world, 
        const Grounding* context, 
        LLM* llm,
        const unsigned int beamWidth,
        const bool& debug ){

  // Create the vector that will have the child solutions indices.	
  vector< vector< unsigned int > > child_solution_indices;
  for( unsigned int i = 0; i < _children.size(); i++ ){
    child_solution_indices.push_back( vector< unsigned int >() );
    for( unsigned int j = 0; j < _children[ i ]->solutions().size(); j++ ){
      child_solution_indices.back().push_back( j );
    }
  }

  // Create the indices for the child solution combos (power set).
  vector< vector< unsigned int > > child_solution_indices_cartesian_power = cartesian_power( child_solution_indices );
  if( child_solution_indices_cartesian_power.empty() ){
    child_solution_indices_cartesian_power.push_back( vector< unsigned int >() );
  }

  vector< bool > evaluate_feature_types( NUM_FEATURE_TYPES, true );

  cout << "starting factor set search" << endl;
  cout << "child_solution_indices_cartesian_power.size():" << child_solution_indices_cartesian_power.size() << endl;

  if( debug ){
    for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace.grounding_pairs().begin(); it_search_spaces != searchSpace.grounding_pairs().end(); it_search_spaces++ ){
      cout << "searching through symbols for \"" << it_search_spaces->first << "\" (" << it_search_spaces->second.second.size() << " symbols)" << endl;
    }
  }

  vector< vector< Factor_Set_Solution > > solutions_vector;
  // add a Factor_Set_Solution for each combination of children
  for( unsigned int i = 0; i < child_solution_indices_cartesian_power.size(); i++ ){

    search_solution_combination( i, solutions_vector, child_solution_indices_cartesian_power, searchSpace, evaluate_feature_types, 
                                 context, world, llm, beamWidth, debug);
   /*
    solutions_vector.push_back( vector< Factor_Set_Solution >() );
    solutions_vector.back().push_back( Factor_Set_Solution() );
    solutions_vector.back().back().children = child_solution_indices_cartesian_power[ i ];
    solutions_vector.back().back().cv.clear();
    for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace.grounding_pairs().begin(); it_search_spaces != searchSpace.grounding_pairs().end(); it_search_spaces++ ){
      solutions_vector.back().back().cv.insert( pair< string, vector< vector< unsigned int > > >( it_search_spaces->first, vector< vector< unsigned int > >( NUM_CVS ) ) );
//      solutions_vector.back().back().cv.resize( NUM_CVS );
    }

    vector< pair< const Phrase*, vector< Grounding* > > > child_groundings;
    for( unsigned int j = 0; j < child_solution_indices_cartesian_power[ i ].size(); j++ ){
      solutions_vector.back().back().pygx *= _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].pygx;
      child_groundings.push_back( pair< const Phrase*, vector< Grounding* > >( _children[ j ]->phrase(), vector< Grounding* >() ) );
      for( unsigned int k = 0; k < _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].groundings->groundings().size(); k++ ){
        child_groundings.back().second.push_back( _children[ j ]->solutions()[ child_solution_indices_cartesian_power[ i ][ j ] ].groundings->groundings()[ k ] );
      }
    }

    for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace.grounding_pairs().begin(); it_search_spaces != searchSpace.grounding_pairs().end(); it_search_spaces++ ){
      for( unsigned int j = 0; j < it_search_spaces->second.second.size(); j++ ){
        unsigned int num_solutions = solutions_vector.back().size();
        map< string, vector< unsigned int > >::const_iterator it_cvs = searchSpace.cvs().find( it_search_spaces->second.first );
        assert( it_cvs != searchSpace.cvs().end() );
        for( unsigned int k = 1; k < it_cvs->second.size(); k++ ){
          for( unsigned int l = 0; l < num_solutions; l++ ){
            solutions_vector.back().push_back( solutions_vector.back()[ l ] );
          } 
        }
        for( unsigned int k = 0; k < it_cvs->second.size(); k++ ){
          double value = llm->pygx( it_cvs->second[ k ], it_search_spaces->second.second[ j ], child_groundings, _phrase, world, context, it_cvs->second, evaluate_feature_types );
          evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
          for( unsigned int l = 0; l < num_solutions; l++ ){
            map< string, vector< vector< unsigned int > > >::iterator it_cvs_solution = solutions_vector.back()[ k * num_solutions + l ].cv.find( it_search_spaces->first );
            assert( it_cvs_solution != solutions_vector.back()[ k * num_solutions + l ].cv.end() ); 
            it_cvs_solution->second[ it_cvs->second[ k ] ].push_back( j );
            solutions_vector.back()[ k * num_solutions + l ].pygx *= value; 
          }
        }

        sort( solutions_vector.back().begin(), solutions_vector.back().end(), factor_set_solution_sort );   
      
        if( solutions_vector.back().size() > beamWidth ){
          solutions_vector.back().erase( solutions_vector.back().begin() + beamWidth, solutions_vector.back().end() );
        }
      }
    }
   */
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

  for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace.grounding_pairs().begin(); it_search_spaces != searchSpace.grounding_pairs().end(); it_search_spaces++ ){
    for( unsigned int i = 0; i < _solutions.size(); i++ ){
      map< string, vector< vector< unsigned int > > >::const_iterator it_cvs = _solutions[ i ].cv.find( it_search_spaces->first );
      assert( it_cvs != _solutions[ i ].cv.end() );
//      for( unsigned int j = 0; j < _solutions[ i ].cv[ CV_TRUE ].size(); j++ ){   
      for( unsigned int j = 0; j < it_cvs->second[ CV_TRUE ].size(); j++ ){
//        _solutions[ i ].groundings.push_back( searchSpace[ _solutions[ i ].cv[ CV_TRUE ][ j ] ].second );
        _solutions[ i ].groundings->groundings().push_back( it_search_spaces->second.second[ it_cvs->second[ CV_TRUE ][ j ] ] );
      }
    }
  }

  cout << "finished setting groundings" << endl;

  if( debug ){
    for( unsigned int i = 0; i < _solutions.size(); i++ ){
      map< string, vector< unsigned int > > expressed_indices;
      for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_search_spaces = searchSpace.grounding_pairs().begin(); it_search_spaces != searchSpace.grounding_pairs().end(); it_search_spaces++ ){
        pair< map< string, vector< unsigned int > >::iterator, bool > expressed_indices_ret = expressed_indices.insert( pair< string, vector< unsigned int > >( it_search_spaces->first, vector< unsigned int >() ) );
         
        map< string, vector< vector< unsigned int > > >::const_iterator it_cvs = _solutions[ i ].cv.find( it_search_spaces->first );
        assert( it_cvs != _solutions[ i ].cv.end() );
       
        // only print symbols that are not unknown or false 
        for( unsigned int j = 2; j < it_cvs->second.size(); j++ ){
          for( unsigned int k = 0; k < it_cvs->second[ j ].size(); k++ ){
            expressed_indices_ret.first->second.push_back( it_cvs->second[ j ][ k ] ); 
          }
        }
      }     

/*        
      cout << "solutions[" << i << "] TRUE [" << it_cvs->second[ CV_TRUE ].size() << "]:{";
      for( unsigned int j = 0; j < it_cvs->second[ CV_TRUE ].size(); j++ ){
        cout << it_cvs->second[ CV_TRUE ][ j ];
        if( j != ( it_cvs->second[ CV_TRUE ].size() - 1 ) ){
          cout << ",";
        }
      }
*/
      cout << "solutions[" << i << "] TRUE {";
      stringstream tmp_string;
      for( map< string, vector< unsigned int > >::const_iterator it_expressed_indices = expressed_indices.begin(); it_expressed_indices != expressed_indices.end(); it_expressed_indices++ ){
        if( !it_expressed_indices->second.empty() ){
          if( !tmp_string.str().empty() ){
            tmp_string << ",";
          }   
          tmp_string << it_expressed_indices->first << "[" << it_expressed_indices->second.size() << "]:{";
          for( unsigned int j = 0; j < it_expressed_indices->second.size(); j++ ){
            tmp_string << it_expressed_indices->second[ j ];
            if( j != ( it_expressed_indices->second.size() - 1 ) ){
              tmp_string << ",";
            }
          } 
          tmp_string << "}";
        } 
      }
      cout << tmp_string.str() << "} ";
      cout << "groundings[" << _solutions[ i ].groundings->groundings().size() << "]:{";
      // Added the printing on the groundings with each.
      for( unsigned int j = 0; j < _solutions[ i ].groundings->groundings().size(); j++ ){
        cout << *(_solutions[ i ].groundings->groundings()[ j ]);
        if( j != ( _solutions[ i ].groundings->groundings().size() - 1 ) ){
          cout << ",";
        }
      }

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
