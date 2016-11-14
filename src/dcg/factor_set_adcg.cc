/**
 * @file    factor_set.cc
* The implementation of a class used to represent a factor set
 */

#include <assert.h>

#include "h2sl/common.h"
#include "h2sl/constraint.h"

#include "h2sl/world.h"
#include "h2sl/grounding.h"
#include "h2sl/object.h"
#include "h2sl/object_property.h"
#include "h2sl/index.h"
#include "h2sl/number.h"
#include "h2sl/phrase.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_abstract_container.h"
#include "h2sl/container.h"
#include "h2sl/region_container.h"
#include "h2sl/spatial_relation.h"

#include "h2sl/factor_set_adcg.h"

using namespace std;
using namespace h2sl;

bool
factor_set_adcg_solution_sort( const Factor_Set_Solution& a,
                              const Factor_Set_Solution& b ){
  return a.pygx > b.pygx;
}

Factor_Set_ADCG::
Factor_Set_ADCG( const Phrase* phrase ) : Factor_Set( phrase ),
                                              _abstract_search_spaces(),
                                              _abstract_correspondence_variables() {
  vector< unsigned int > binary_cvs;
  binary_cvs.push_back( CV_FALSE );
  binary_cvs.push_back( CV_TRUE );

  _abstract_correspondence_variables.push_back( binary_cvs );
}

Factor_Set_ADCG::
~Factor_Set_ADCG() {

}

Factor_Set_ADCG::
Factor_Set_ADCG( const Factor_Set_ADCG& other ) : Factor_Set( other ),
                                             _abstract_search_spaces( other._abstract_search_spaces ),
                                              _abstract_correspondence_variables( other._abstract_correspondence_variables ) {

}

Factor_Set_ADCG&
Factor_Set_ADCG::
operator=( const Factor_Set_ADCG& other ) {
  _phrase = other._phrase;
  _children = other._children;
  _solutions = other._solutions;
  _abstract_search_spaces = other._abstract_search_spaces;
  _abstract_correspondence_variables = other._abstract_correspondence_variables;
  return (*this);
}

void
Factor_Set_ADCG::
search( const vector< pair< unsigned int, Grounding* > >& searchSpace,
        const vector< vector< unsigned int > >& correspondenceVariables,
        const map< string, vector< string > >& symbolTypes,
        const World* world,
        LLM* llm,
        const unsigned int beamWidth,
        const bool& debug ){
  search( searchSpace, correspondenceVariables, symbolTypes, world, NULL, llm, beamWidth, debug );
  return;
}

void
Factor_Set_ADCG::
search( const vector< pair< unsigned int, Grounding* > >& searchSpace,
        const vector< vector< unsigned int > >& correspondenceVariables,
        const map< string, vector< string > >& symbolTypes,
        const World* world,
        const Grounding* grounding,
        LLM* llm,
        const unsigned int beamWidth,
        const bool& debug ){
  _search_physical( searchSpace, correspondenceVariables, symbolTypes, world, llm, beamWidth, debug );
  return;
}

void
Factor_Set_ADCG::
_search_physical( const vector< pair< unsigned int, Grounding* > >& searchSpace,
                  const vector< vector< unsigned int > >& correspondenceVariables,
                  const map< string, vector< string> >& symbolTypes,
                  const World* world, LLM* llm, const unsigned int beamWidth, const bool& debug ){
  if( debug ){
    cout << " Factor Set ADCG: Beginning of physical search" << endl;
    cout << " phrase:" << *_phrase << endl;
  }

  // Abstract search space data structure. 
  for( unsigned int i = 0; i < _abstract_search_spaces.size(); i++ ){
    for( unsigned int j = 0; j < _abstract_search_spaces[ i ].size(); j++ ){
      if( _abstract_search_spaces[ i ][ j ].second != NULL ){
        delete _abstract_search_spaces[ i ][ j ].second;
        _abstract_search_spaces[ i ][ j ].second = NULL;
      }
    }
    _abstract_search_spaces[ i ].clear();
  }
  _abstract_search_spaces.clear();

  // Form the child solution indices vector.
  vector< vector< unsigned int > > child_solution_indices;
  for( unsigned int i = 0; i < _children.size(); i++ ){
    child_solution_indices.push_back( vector< unsigned int >() );
    for( unsigned int j = 0; j < _children[ i ]->solutions().size(); j++ ){
      child_solution_indices.back().push_back( j );
    }
  }

  // Create the child solution indices cartesian power vector.
  vector< vector< unsigned int > > child_solution_indices_cartesian_power = cartesian_power( child_solution_indices );
  if( child_solution_indices_cartesian_power.empty() ){
    child_solution_indices_cartesian_power.push_back( vector< unsigned int >() );
  }

  // Resize the abstract search space with the child solution cartesian power set.
  _abstract_search_spaces.resize( child_solution_indices_cartesian_power.size() );

  // Evaluate feature types vector. True.
  vector< bool > evaluate_feature_types( NUM_FEATURE_TYPES, true );

  // Create the factor set solutions vector 
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

    for( unsigned int j = 0; j < searchSpace.size(); j++ ){
      unsigned int num_solutions = solutions_vector.back().size();
      for( unsigned int k = 1; k < correspondenceVariables[ searchSpace[ j ].first ].size(); k++ ){
        for( unsigned int l = 0; l < num_solutions; l++ ){
          solutions_vector.back().push_back( solutions_vector.back()[ l ] );
        }
      }

      if( debug ){
/*
        cout << "considering " << *static_cast< Grounding* >( searchSpace[ j ].second ) << endl;
        for( unsigned int k = 0; k < child_groundings.size(); k++ ){
          for( unsigned int l = 0; l < child_groundings[ k ].second.size(); l++ ){
            cout << "grounding set " << *static_cast< Grounding* >( child_groundings[ k ].second[ l ] ) << endl;
          }
        }
*/
      }
  
      // Prob. of individual factors. Context of child groundings. Multiply with the child groundings.
      for( unsigned int k = 0; k < correspondenceVariables[ searchSpace[ j ].first ].size(); k++ ){
        double value = llm->pygx( correspondenceVariables[ searchSpace[ j ].first ][ k ], 
                                  searchSpace[ j ].second, 
                                  child_groundings, _phrase, world, 
                                  correspondenceVariables[ searchSpace[ j ].first ], 
                                  evaluate_feature_types );
        evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
        for( unsigned int l = 0; l < num_solutions; l++ ){
          solutions_vector.back()[ k * num_solutions + l ].cv[ correspondenceVariables[ searchSpace[ j ].first ][ k ] ].push_back( j );
          solutions_vector.back()[ k * num_solutions + l ].pygx *= value;
        }
      }

      // Most probable set. Trim the solutions.
      sort( solutions_vector.back().begin(), solutions_vector.back().end(), factor_set_adcg_solution_sort );
      if( solutions_vector.back().size() > beamWidth ){
        solutions_vector.back().erase( solutions_vector.back().begin() + beamWidth, solutions_vector.back().end() );
      }

      for( unsigned int k = 0; k < solutions_vector.back().size(); k++ ){
        solutions_vector.back()[ k ].groundings.clear();
        for( unsigned int l = 0; l < solutions_vector.back()[ k ].cv[ CV_TRUE ].size(); l++ ){
          solutions_vector.back()[ k ].groundings.push_back( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ l ] ].second );
        }
      }
    }
  
    // look for the expressed symbols for objects, indices
    vector< vector< unsigned int > > observed_true_solution_vectors;
    vector< vector< Object* > > observed_object_vectors;
    vector< string > observed_spatial_relations( 1, string( "na" ) );
    vector< string > observed_object_types( 1, string( "na" ) );
    vector< string > observed_object_colors( 1, string( "na") );
    vector< string > observed_indices( 1, string( "first" ) );
    vector< string > observed_numbers; 
  
    // Collect the solutions at the concrete level. 
    for( unsigned int k = 0; k < solutions_vector.back().size(); k++ ){
      // look for a new observed_new_solution_vector
      bool new_true_solution_vector = true;
      for( unsigned int l = 0; l < observed_true_solution_vectors.size(); l++ ){
        if( solutions_vector.back()[ k ].cv[ CV_TRUE ] == observed_true_solution_vectors[ l ] ){
          new_true_solution_vector = false;
        }
      }
      if( new_true_solution_vector ){
        observed_true_solution_vectors.push_back( solutions_vector.back()[ k ].cv[ CV_TRUE ] );
        observed_object_vectors.push_back( vector< Object* >() );

        cout << "solutions_vector.back()[" << k << "].cv[ CV_TRUE ].size():" << solutions_vector.back()[ k ].cv[ CV_TRUE ].size() << endl;
        for( unsigned int m = 0; m < solutions_vector.back()[ k ].cv[ CV_TRUE ].size(); m++ ){
          if( dynamic_cast< Object* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second ) != NULL ){
            observed_object_vectors.back().push_back( static_cast< Object* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second ) );
          } else if ( dynamic_cast< Index* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second ) != NULL ){
            cout << "found Index:" << *static_cast< Index* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second ) << endl;
            string tmp = static_cast< Index* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second )->index_type();
            if( find( observed_indices.begin(), observed_indices.end(), tmp ) == observed_indices.end() ){
              observed_indices.push_back( tmp );
            }
          } else if ( dynamic_cast< Number* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second ) != NULL ){
            string tmp_number = static_cast< Number* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second )->number_value();
            if( find( observed_numbers.begin(), observed_numbers.end(), tmp_number ) == observed_numbers.end() ){
              observed_numbers.push_back( tmp_number );
            }
          } else if ( dynamic_cast< Object_Type* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second ) != NULL ){
            cout << "found Object_Type:" << *static_cast< Object_Type* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second ) << endl;
            string tmp_object_type = static_cast< Object_Type* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second )->type();
            if( find( observed_object_types.begin(), observed_object_types.end(), tmp_object_type ) == observed_object_types.end() ){
              observed_object_types.push_back( tmp_object_type );
            }
          } else if ( dynamic_cast< Object_Color* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second ) != NULL ){
            string tmp_object_color = static_cast< Object_Color* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second )->object_color_type();
            if( find( observed_object_colors.begin(), observed_object_colors.end(), tmp_object_color ) == observed_object_colors.end() ){
              observed_object_colors.push_back( tmp_object_color );
            }
          } else if ( dynamic_cast< Spatial_Relation* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second ) != NULL ){
            cout << "found Spatial_Relation:" << *static_cast< Spatial_Relation* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second ) << endl;
            string tmp_spatial_relation = static_cast< Spatial_Relation* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second )->relation_type();
            if( find( observed_spatial_relations.begin(), observed_spatial_relations.end(), tmp_spatial_relation ) == observed_spatial_relations.end() ){
              observed_spatial_relations.push_back( tmp_spatial_relation );
            }
          } else if ( dynamic_cast< Abstract_Container* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second ) != NULL ){
            Abstract_Container* tmp_abstract_container = static_cast< Abstract_Container* >( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ m ] ].second );
            string tmp_number =  tmp_abstract_container->number() ;
            if( find( observed_numbers.begin(), observed_numbers.end(), tmp_number ) == observed_numbers.end() ){
              observed_numbers.push_back( tmp_number );
            }
            string tmp_index =  tmp_abstract_container->index();
            if( find( observed_indices.begin(), observed_indices.end(), tmp_index ) == observed_indices.end() ){
              observed_indices.push_back( tmp_index );
            }
            string tmp_object_type = tmp_abstract_container->type();
            if( find( observed_object_types.begin(), observed_object_types.end(), tmp_object_type ) == observed_object_types.end() ){
              observed_object_types.push_back( tmp_object_type );
            }
            string tmp_object_color = tmp_abstract_container->color();
            if( find( observed_object_colors.begin(), observed_object_colors.end(), tmp_object_color ) == observed_object_colors.end() ){
              observed_object_colors.push_back( tmp_object_color );
            }
          }
        }

        // Looking for pairs of objects. If there is only one then do not.
        if( observed_object_vectors.back().size() < 2 ){
          observed_object_vectors.pop_back();
        }
   
        // If the solution vectors are identical, remove them. 
        if( observed_object_vectors.size() > 1 ){
          for( unsigned m = 0; m < ( observed_object_vectors.size() - 1 ); m++ ){
            if( observed_object_vectors[ m ] == observed_object_vectors.back() ){
              observed_object_vectors.pop_back();
            }
          }
        }
      }
    }

    // add in the observed indices from the children
    for( unsigned int j = 0; j < child_groundings.size(); j++ ){
      for( unsigned int k = 0; k < child_groundings[ j ].second.size(); k++ ){
        if( dynamic_cast< Index* >( child_groundings[ j ].second[ k ] ) != NULL ){
          string tmp = static_cast< Index* >( child_groundings[ j ].second[ k ] )->index_type();
          if( find( observed_indices.begin(), observed_indices.end(), tmp ) == observed_indices.end() ){
            observed_indices.push_back( tmp );
          }
        } else if ( dynamic_cast< Object_Property* >( child_groundings[ j ].second[ k ] ) != NULL ){
          Object_Property* tmp_object_property = static_cast< Object_Property* >( child_groundings[ j ].second[ k ] );
          string tmp_index = tmp_object_property->index();
          if( find( observed_indices.begin(), observed_indices.end(), tmp_index ) == observed_indices.end() ){
            observed_indices.push_back( tmp_index );
          }
          string tmp_object_type = tmp_object_property->type();
          if( find( observed_object_types.begin(), observed_object_types.end(), tmp_object_type ) == observed_object_types.end() ){
            observed_object_types.push_back( tmp_object_type );
          }
          string tmp_spatial_relation = tmp_object_property->relation_type();
          if( find( observed_spatial_relations.begin(), observed_spatial_relations.end(), tmp_spatial_relation ) == observed_spatial_relations.end() ){
            observed_spatial_relations.push_back( tmp_spatial_relation );
          }
        } else if ( dynamic_cast< Spatial_Relation* >( child_groundings[ j ].second[ k ] ) != NULL ){
          string tmp_spatial_relation = static_cast< Spatial_Relation* >( child_groundings[ j ].second[ k ] )->relation_type();
          if( find( observed_spatial_relations.begin(), observed_spatial_relations.end(), tmp_spatial_relation ) == observed_spatial_relations.end() ){
            observed_spatial_relations.push_back( tmp_spatial_relation );
          }
        } else if ( dynamic_cast< Region_Container* >( child_groundings[ j ].second[ k ] ) != NULL ){
          string tmp_spatial_relation = static_cast< Region_Container* >( child_groundings[ j ].second[ k ] )->relation_type();
          if( find( observed_spatial_relations.begin(), observed_spatial_relations.end(), tmp_spatial_relation ) == observed_spatial_relations.end() ){
            observed_spatial_relations.push_back( tmp_spatial_relation );
          }
        }
      }
    }

    if ( world != NULL) {
      for ( unsigned int j = 0; j < observed_object_types.size(); j++ ) {
        if ( symbolTypes.find( string( "object_type" ) ) != symbolTypes.end() ) {
          map< string, vector< Object* > >::const_iterator it1 =  world->min_x_sorted_objects().find( observed_object_types[ j ] );
          if ( it1 !=  world->min_x_sorted_objects().end() ) {
            map< string, vector< string> >::const_iterator it2 = symbolTypes.find( string( "object_type" ) );
            if ( it2 != symbolTypes.end() ) {
              if ( it1->second.size() < it2->second.size() ) {
                string tmp_number = "na";
                for( map< string, unsigned int>::const_iterator it3 = world->numeric_map().begin(); it3 != world->numeric_map().end(); ++it3 ) {
                  map< string, vector< Object* > >::const_iterator it4 = world->min_x_sorted_objects().find( observed_object_types[ j ]  );
                  if( it4 != world->min_x_sorted_objects().end() ) {
                    if( it3->second == it4->second.size() ) {
                      tmp_number = it3->first; 
                    }
                  }
                }
                if ( !tmp_number.compare(string( "na")) ) {
                  if( find( observed_numbers.begin(), observed_numbers.end(), tmp_number ) == observed_numbers.end() ){
                    observed_numbers.push_back( tmp_number );
                  }
                }
              }
            } 
          }  
        }
      }
    }   


  // TODO: NEED TO ACCOUNT FOR OTHER EXPRESSIONS OF THE CONTAINERS HERE, ADD TO ALL SOLUTIONS
    // fill search space
     
    map< string, vector< string> >::const_iterator it1;
    map< string, vector< string> >::const_iterator it2;
 
    it1  = symbolTypes.find( string( "container" ) );
    if ( it1 != symbolTypes.end() ) {
      for( unsigned int j = 0; j < it1->second.size(); j++ ) {
        for( unsigned int k = 0; k < observed_object_vectors.size(); k++ ) {
          vector< Grounding* > observed_object_grounding_vectors;
          for ( unsigned int m = 0; m < observed_object_vectors[ k ].size(); m++ ) {
            if ( dynamic_cast< Grounding* >( observed_object_vectors[ k ][ m ] ) != NULL ) {
             observed_object_grounding_vectors.push_back( observed_object_vectors[ k ][ m ] );
            }
          }
          _abstract_search_spaces[ i ].push_back( pair< unsigned int, Grounding* >( 0, new Container( 
                                                                                             observed_object_grounding_vectors, 
                                                                                             it1->second[ j ] ) ) );
        }
      }  
    }

    // Create the space of abstract containers.
    it1 = symbolTypes.find( string( "spatial_relation" ) );
    it2 = symbolTypes.find( string( "container" ) );
    if ( it1 != symbolTypes.end() && it2 != symbolTypes.end() )  {
      // Iterate over spatial relation
      //for( unsigned int j = 0; j < it1->second.size(); j++ ) {
      for( unsigned int j = 0; j < observed_spatial_relations.size(); j++ ) {
        // Iterate over container types
        for( unsigned int k = 0; k < it2->second.size(); k++ ) {
          for( unsigned int l = 0; l < observed_object_vectors.size(); l++ ) {
            vector< Grounding* > observed_object_grounding_vectors;
            for ( unsigned int m = 0; m < observed_object_vectors[ l ].size(); m++ ) {
              if ( dynamic_cast< Grounding* >( observed_object_vectors[ l ][ m ]) != NULL ) {
                observed_object_grounding_vectors.push_back( observed_object_vectors[ l ][ m ] );
              }
            }
            _abstract_search_spaces[ i ].push_back( pair< unsigned int, Grounding* >( 0, new Region_Container( 
                                                                                               observed_spatial_relations [ j ], 
                                                                                               Container( observed_object_grounding_vectors, 
                                                                                                          it2->second[ k ] ) ) ) );
          } 
        }
      }   
    }

    cout << "observed_object_types[" << observed_object_types.size() << "]:{";
    for( unsigned int i = 0; i < observed_object_types.size(); i++ ){
      cout << observed_object_types[ i ];
      if( i != ( observed_object_types.size() - 1 ) ){
        cout << ",";
      }
    }
    cout << "}" << endl;

    cout << "observed_spatial_relations[" << observed_spatial_relations.size() << "]:{";
    for( unsigned int i = 0; i < observed_spatial_relations.size(); i++ ){
      cout << observed_spatial_relations[ i ];
      if( i != ( observed_spatial_relations.size() - 1 ) ){
        cout << ",";
      }
    }
    cout << "}" << endl;

    cout << "observed_indices[" << observed_indices.size() << "]:{";
    for( unsigned int i = 0; i < observed_indices.size(); i++ ){
      cout << observed_indices[ i ];
      if( i != ( observed_indices.size() - 1 ) ){
        cout << ",";
      }
    }
    cout << "}" << endl;

   // Create the space of Object Property symbols.
    for( unsigned int j = 0; j < observed_object_types.size(); j++ ){
      for( unsigned int k = 0; k < observed_spatial_relations.size(); k++ ){
        for( unsigned int l = 0; l < observed_indices.size(); l++ ){
          _abstract_search_spaces[ i ].push_back( pair< unsigned int, Grounding* >( 0, new Object_Property( 
                                                                                             observed_object_types[ j ], 
                                                                                             observed_spatial_relations[ k ], 
                                                                                             observed_indices[ l ] ) ) );
        }
      }
    }

    // Create the space of Abstract Containers and Region Abstract Containers.
    it1 = symbolTypes.find( string( "index" ) );
    if ( it1 != symbolTypes.end() ) {
      for( unsigned int j = 0; j < observed_object_types.size(); j++ ){
        for( unsigned int k = 0; k < observed_numbers.size(); k++ ){
          for( unsigned int l = 0; l < observed_object_colors.size(); l++ ){
            _abstract_search_spaces[ i ].push_back( pair< unsigned int, Grounding* >( 0, new Abstract_Container( 
                                                                                               observed_object_types[ j ], 
                                                                                               observed_numbers[ k ], 
                                                                                               it1->second.front(), //Index::TYPE_FIRST, 
                                                                                               observed_object_colors[ l ] ) ) );
            for( unsigned int m = 0; m < observed_spatial_relations.size(); m++ ){
              _abstract_search_spaces[ i ].push_back( pair< unsigned int, Grounding* >( 0, new Region_Abstract_Container( 
                                                                                               //Region_Abstract_Container::Type( m ), 
                                                                                               observed_spatial_relations[ m ], 
                                                                                               Abstract_Container( observed_object_types[ j ], 
                                                                                                                   observed_numbers[ k ], 
                                                                                                                   it1->second.front(), //Index::TYPE_FIRST, 
                                                                                                                   observed_object_colors[ l ] ) ) ) );
            }
          }
        }
      }
    }
 
    // search
    for( unsigned int j = 0; j < _abstract_search_spaces[ i ].size(); j++ ){
      unsigned int num_solutions = solutions_vector.back().size();
      for( unsigned int k = 1; k < _abstract_correspondence_variables[ _abstract_search_spaces[ i ][ j ].first ].size(); k++ ){
        for( unsigned int l = 0; l < num_solutions; l++ ){
          solutions_vector.back().push_back( solutions_vector.back()[ l ] );
        }
      }

      for( unsigned int k = 0; k < _abstract_correspondence_variables[ _abstract_search_spaces[ i ][ j ].first ].size(); k++ ){
        double value = llm->pygx( _abstract_correspondence_variables[ _abstract_search_spaces[ i ][ j ].first ][ k ], _abstract_search_spaces[ i ][ j ].second, child_groundings, _phrase, world, _abstract_correspondence_variables[ _abstract_search_spaces[ i ][ j ].first ], evaluate_feature_types );
        evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
        for( unsigned int l = 0; l < num_solutions; l++ ){
          solutions_vector.back()[ k * num_solutions + l ].cv[ _abstract_correspondence_variables[ _abstract_search_spaces[ i ][ j ].first ][ k ] ].push_back( searchSpace.size() + j );
          solutions_vector.back()[ k * num_solutions + l ].pygx *= value;
        }
      }

      sort( solutions_vector.back().begin(), solutions_vector.back().end(), factor_set_adcg_solution_sort );
      if( solutions_vector.back().size() > beamWidth ){
        solutions_vector.back().erase( solutions_vector.back().begin() + beamWidth, solutions_vector.back().end() );
      }

      for( unsigned int k = 0; k < solutions_vector.back().size(); k++ ){
        solutions_vector.back()[ k ].groundings.clear();
        for( unsigned int l = 0; l < solutions_vector.back()[ k ].cv[ CV_TRUE ].size(); l++ ){
          if( solutions_vector.back()[ k ].cv[ CV_TRUE ][ l ] < searchSpace.size() ){
            // add the symbol from the normal search space
            solutions_vector.back()[ k ].groundings.push_back( searchSpace[ solutions_vector.back()[ k ].cv[ CV_TRUE ][ l ] ].second );
          } else {
            // add the symbol from the abstract search space
            solutions_vector.back()[ k ].groundings.push_back( _abstract_search_spaces[ i ][ solutions_vector.back()[ k ].cv[ CV_TRUE ][ l ] - searchSpace.size() ].second );
          }
        }
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

  sort( _solutions.begin(), _solutions.end(), factor_set_adcg_solution_sort );
  if( _solutions.size() > beamWidth ){
    _solutions.erase( _solutions.begin() + beamWidth, _solutions.end() );
  }


  if( debug ){
    cout << *this << endl;
  }

  return;
  
}


namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Factor_Set_ADCG& other ){

    for( unsigned int i = 0; i < other.solutions().size(); i++ ){
      out << "solutions[" << i << "] TRUE [" << other.solutions()[ i ].cv[ CV_TRUE ].size() << "]:{";
      for( unsigned int j = 0; j < other.solutions()[ i ].cv[ CV_TRUE ].size(); j++ ){
        out << other.solutions()[ i ].cv[ CV_TRUE ][ j ];
        if( j != ( other.solutions()[ i ].cv[ CV_TRUE ].size() - 1 ) ){
          out << ",";
        }
      }
      out << "} ";
      out << "groundings[" << other.solutions()[ i ].groundings.size() << "]:{";
      for( unsigned int j = 0; j < other.solutions()[ i ].groundings.size(); j++ ){
        if( dynamic_cast< Grounding* >( other.solutions()[ i ].groundings[ j ] ) != NULL ){
          out << "(" << *static_cast< Grounding* >( other.solutions()[ i ].groundings[ j ] ) << ")";
        }
        if( j != ( other.solutions()[ i ].groundings.size() - 1 ) ){
          out << ",";
        }
      }
      out << "} ";
      out << "children[" << other.solutions()[ i ].children.size() << "]:{";
      for( unsigned int j = 0; j < other.solutions()[ i ].children.size(); j++ ){
        out << other.solutions()[ i ].children[ j ];
        if( j != ( other.solutions()[ i ].children.size() - 1 ) ){
          out << ",";
        }
      }
      out << "} ";
      out << "pygx:" << other.solutions()[ i ].pygx << endl;
    }
    for( unsigned int i = 0; i < other.abstract_search_spaces().size(); i++ ){
      out << "abstract_search_space[" << i << "] size:" << other.abstract_search_spaces()[ i ].size() << endl;
    }
  
    return out;

  }
}






































