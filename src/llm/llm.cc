/**
 * @file    llm.cc
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
 * The implementation of a class used to represent a log-linear model
 */

#include <iomanip>
#include <sstream>
#include <cmath>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include <lbfgs.h>

#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include <h2sl/llm.h>

using namespace std;
using namespace h2sl;

lbfgsfloatval_t
evaluate( void * instance,
          const lbfgsfloatval_t * x,
          lbfgsfloatval_t * g,
          const int n,
          const lbfgsfloatval_t step ) {
  LLM_Train* llm_train = static_cast< LLM_Train* >( instance );

  for( unsigned int i = 0; i < llm_train->llms().size(); i++ ){
    llm_train->llms()[ i ]->weights().resize( n, 0.0 );
    for( int j = 0; j < n; j++ ){
      llm_train->llms()[ i ]->weights()[ j ] = x[ j ];
    }
  }  

  lbfgsfloatval_t objective = ( lbfgsfloatval_t )( llm_train->objective( *llm_train->examples(), llm_train->indices(), 0.001 ) );

  llm_train->gradient( 0.001 );

  for( unsigned int i = 0; i < llm_train->gradient().size(); i++ ){
    g[ i ] = -llm_train->gradient()[ i ];
  }

  return -objective;
}

int
progress( void * instance,
          const lbfgsfloatval_t *x,
          const lbfgsfloatval_t *g,
          const lbfgsfloatval_t fx,
          const lbfgsfloatval_t xnorm,
          const lbfgsfloatval_t gnorm,
          const lbfgsfloatval_t step,
          int n,
          int k,
          int ls ) {
  cout << setw(3) << setfill(' ') << k << " " << setw(8) << setfill(' ') <<  -fx << " (" << xnorm << ") (" << gnorm << ")" << endl;
  return 0;
}

LLM_X::
LLM_X( const Grounding* grounding,
        const Phrase* phrase,
        const World* world,
        const vector< string >& cvs,
        const vector< Feature* >& features,
        const string& filename ) : _grounding( grounding ),
                                            _phrase( phrase ),
                                            _world( world ),
                                            _context( NULL ),
                                            _children(),
                                            _cvs( cvs ),
                                            _filename( filename ) {
}

LLM_X::
LLM_X( const Grounding* grounding,
        const Phrase* phrase,
        const World* world,
        const Grounding* context,
        const vector< string >& cvs,
        const vector< Feature* >& features,
        const string& filename ) : _grounding( grounding ),
                                            _phrase( phrase ),
                                            _world( world ),
                                            _context( context ),
                                            _children(),
                                            _cvs( cvs ),
                                            _filename( filename ) {
}

LLM_X::
~LLM_X() {

}

LLM_X::
LLM_X( const LLM_X& other ) : _grounding( other._grounding ),
                              _phrase( other._phrase ),
                              _world( other._world ),
                              _context( other._context ),
                              _children(),
                              _cvs( other._cvs ),
                              _filename( other._filename ) {
  _children.resize( other._children.size() );
  for( unsigned int i = 0; i < other._children.size(); i++ ){
    if( other._children[ i ].first != NULL ){
      _children[ i ].first = other._children[ i ].first;
    }
    _children[ i ].second = other._children[ i ].second;
  }
}

LLM_X&
LLM_X::
operator=( const LLM_X& other ) {
  _children.resize( other._children.size() );
  for( unsigned int i = 0; i < other._children.size(); i++ ){
    if( other._children[ i ].first != NULL ){
      _children[ i ].first = other._children[ i ].first;
    }
    _children[ i ].second = other._children[ i ].second;
  }
  return *this;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const LLM_X& other ) {
    out << "filename:\"" << other.filename() << "\"" << endl;
    if( other.grounding() != NULL ){
      out << "grounding:(" << *other.grounding() << ") ";
    }
    if( other.context() != NULL ){
      out << "context:(" << *other.context() << ") ";
    }
    if( other.phrase() != NULL ){
      out << "phrase:(" << *other.phrase() << ")";
    } else {
      out << "phrase:(NULL)";
    }
    return out;
  }
}

LLM::
LLM( Feature_Set* featureSet ) : _weights(),
                                  _feature_set( featureSet ){

}

LLM::
~LLM() {

}

LLM::
LLM( const LLM& other ) : _weights( other._weights ),
                          _feature_set( other._feature_set ){

}

LLM&
LLM::
operator=( const LLM& other ) {
  _weights = other._weights;
  _feature_set = other._feature_set;
  return (*this);
}

double
LLM::
pygx( const string& cv,
      const LLM_X& x,
      const vector< string >& cvs,
      const vector< vector< unsigned int > >& indices ){
  double numerator = 0.0;
  double denominator = 0.0;
  if( cvs.size() == indices.size() ){
    for( unsigned int i = 0; i < cvs.size(); i++ ){
    double dp = 0.0;
      for( unsigned int j = 0; j < indices[ i ].size(); j++ ){
        dp += _weights[ indices[ i ][ j ] ];
      }
      dp = exp( dp );
      if( cv == cvs[ i ] ){
        numerator += dp;
      }
      denominator += dp;
    }
  }
  return ( numerator / denominator );
}

double
LLM::
pygx( const string& cv,
      const LLM_X& x,
      const vector< string >& cvs,
      vector< unsigned int >& indices ){
  double numerator = 0.0;
  double denominator = 0.0;
  vector< unsigned int > tmp;
  vector< Feature* > tmp_features;
  vector< bool > evaluate_feature_types( NUM_FEATURE_TYPES, true );
  for( unsigned int i = 0; i < cvs.size(); i++ ){
    if( i != 0 ){
      evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
      evaluate_feature_types[ FEATURE_TYPE_GROUNDING ] = false;
    }
    double dp = 0.0;
    _feature_set->indices( cvs[ i ], x.grounding(), x.children(), x.phrase(), x.world(), x.context(), tmp, tmp_features, evaluate_feature_types );
    for( unsigned int j = 0; j < tmp.size(); j++ ){
      dp += _weights[ tmp[ j ] ];
    }
    dp = exp( dp );
    if( cv == cvs[ i ] ){
      numerator += dp;
      indices = tmp;
    }
    denominator += dp;
  }
  return ( numerator / denominator );
}

double
LLM::
pygx( const string& cv,
      const LLM_X& x,
      const vector< string >& cvs,
      vector< Feature* >& features ){
  double numerator = 0.0;
  double denominator = 0.0;
  vector< unsigned int > indices;
  vector< bool > evaluate_feature_types( NUM_FEATURE_TYPES, true );
  for( unsigned int i = 0; i < cvs.size(); i++ ){
    if( i != 0 ){
      evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
      evaluate_feature_types[ FEATURE_TYPE_GROUNDING ] = false; 
    }
    double dp = 0.0;
    _feature_set->indices( cvs[ i ], x.grounding(), x.children(), x.phrase(), x.world(), x.context(), indices, features, evaluate_feature_types );
    for( unsigned int j = 0; j < indices.size(); j++ ){
      dp += _weights[ indices[ j ] ];
    }
    dp = exp( dp );
    if( cv == cvs[ i ] ){
      numerator += dp;
    }
    denominator += dp;
  }
  return ( numerator / denominator );
}

double
LLM::
pygx( const string& cv,
      const LLM_X& x,
      const vector< string >& cvs,
      vector< pair< std::vector< Feature* >, unsigned int > >& weightedFeatures ){
  double numerator = 0.0;
  double denominator = 0.0;
  vector< unsigned int > indices;
  vector< bool > evaluate_feature_types( NUM_FEATURE_TYPES, true );
  for( unsigned int i = 0; i < cvs.size(); i++ ){
    if( i != 0 ){
      evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
      evaluate_feature_types[ FEATURE_TYPE_GROUNDING ] = false;
    }
    double dp = 0.0;
    _feature_set->indices( cvs[ i ], x.grounding(), x.children(), x.phrase(), x.world(), x.context(), indices, weightedFeatures, evaluate_feature_types );
    for( unsigned int j = 0; j < indices.size(); j++ ){
      dp += _weights[ indices[ j ] ];
    }
    dp = exp( dp );
    if( cv == cvs[ i ] ){
      numerator += dp;
    }
    denominator += dp;
  }
  return ( numerator / denominator );
}

double
LLM::
pygx( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const vector< string >& cvs ){
  return pygx( cv, grounding, children, phrase, world, NULL, cvs );
}

double
LLM::
pygx( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const Grounding* context,
      const vector< string >& cvs ){
  double numerator = 0.0;
  double denominator = 0.0;
  vector< unsigned int > indices;
  vector< Feature* > features;
  vector< bool > evaluate_feature_types( NUM_FEATURE_TYPES, true );
  for( unsigned int i = 0; i < cvs.size(); i++ ){
    if( i != 0 ){
      evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
      evaluate_feature_types[ FEATURE_TYPE_GROUNDING ] = false;
    }
    double dp = 0.0;
    _feature_set->indices( cvs[ i ], grounding, children, phrase, world, context, indices, features, evaluate_feature_types );
    for( unsigned int j = 0; j < indices.size(); j++ ){
      dp += _weights[ indices[ j ] ];
    }
    dp = exp( dp );
    if( cv == cvs[ i ] ){
      numerator += dp;
    }
    denominator += dp;
  }
  return ( numerator / denominator );
}

double
LLM::
pygx( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const vector< string >& cvs,
      const vector< bool >& evaluateFeatureTypes ){
  return pygx( cv, grounding, children, phrase, world, NULL, cvs, evaluateFeatureTypes );
}

double
LLM::
pygx( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const Grounding* context,
      const vector< string >& cvs,
      const vector< bool >& evaluateFeatureTypes ){
  double numerator = 0.0;
  double denominator = 0.0;
  vector< unsigned int > indices;
  vector< Feature* > features;
  vector< bool > evaluate_feature_types = evaluateFeatureTypes;
  for( unsigned int i = 0; i < cvs.size(); i++ ){
    if( i != 0 ){
      evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
      evaluate_feature_types[ FEATURE_TYPE_GROUNDING ] = false;
    }
    double dp = 0.0;
    _feature_set->indices( cvs[ i ], grounding, children, phrase, world, context, indices, features, evaluate_feature_types );
    for( unsigned int j = 0; j < indices.size(); j++ ){
      dp += _weights[ indices[ j ] ];
    }
    dp = exp( dp );
    if( cv == cvs[ i ] ){
      numerator += dp;
    }
    denominator += dp;
  }
  return ( numerator / denominator );
}

void
LLM_Train::
train( vector< pair< string, LLM_X > >& examples,
        const unsigned int& maxIterations,
        const double& lambda,
        const double& epsilon ){

  _examples = &examples;

  if( _llms.front()->feature_set()->size() != _llms.front()->weights().size() ){
    _llms.front()->weights().resize( _llms.front()->feature_set()->size(), 0.0 );
  }
  
  lbfgsfloatval_t fx;
  lbfgsfloatval_t * x = lbfgs_malloc( _llms.front()->feature_set()->size() );

  for( unsigned int i = 0; i < _llms.front()->weights().size(); i++ ){
    x[ i ] = _llms.front()->weights()[ i ];
  }

  lbfgs_parameter_t param;
  lbfgs_parameter_init(&param);
  param.epsilon = epsilon;
  param.max_iterations = maxIterations;

  compute_indices();

  lbfgs( _llms.front()->weights().size(), x, &fx, evaluate, progress, ( void* )( this ), &param );

  for( unsigned int i = 0; i < _llms.front()->weights().size(); i++ ){
    _llms.front()->weights()[ i ] = x[ i ];
  }

  lbfgs_free( x );

  return;
}

void 
LLM::
to_xml( const string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

void 
LLM::
to_xml( xmlDocPtr doc, 
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "llm" ), NULL );
  _feature_set->to_xml( doc, node );
  stringstream weights_string;
  for( unsigned int i = 0; i < _weights.size(); i++ ){
    weights_string << _weights[ i ];
    if( i != ( _weights.size() - 1 ) ){
      weights_string << ",";
    }
  }
  xmlNewProp( node, ( const xmlChar* )( "weights" ), ( const xmlChar* )( weights_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void 
LLM::
from_xml( const string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "llm" ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
    }
    xmlFreeDoc( doc );
  }
  return;
}

void 
LLM::
from_xml( xmlNodePtr root ){
  _weights.clear();

  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "weights" ) );
    if( tmp != NULL ){
      string weights_string = ( char* )( tmp );
      vector< string > weights_strings;
      boost::split( weights_strings, weights_string, boost::is_any_of( "," ) );
      _weights.resize( weights_strings.size() );
      for( unsigned int i = 0; i < weights_strings.size(); i++ ){
        _weights[ i ] = strtof( weights_strings[ i ].c_str(), NULL );
      }
      xmlFree( tmp );
    }   
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_set" ) ) == 0 ){
          _feature_set->from_xml( l1 );
        }
      }
    }  
  }
  _weights.resize( _feature_set->size() );
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const LLM& other ) {
    return out;
  }
}

LLM_Train::
LLM_Train( const vector< LLM* >& llms,
            vector< pair< string, LLM_X > >* examples ) : _llms( llms ),
                                                                _examples( examples ),
                                                                _index_vector(),
                                                                _gradient(),
                                                                _indices() {
  if( !_llms.empty() ){
    _gradient.resize( _llms.front()->weights().size() );
  }
}

LLM_Train::
~LLM_Train(){
  
}

LLM_Train::
LLM_Train( const LLM_Train& other ) : _llms( other._llms ),
                                      _examples( other._examples ),
                                      _indices( other._indices ){

}
    
LLM_Train&
LLM_Train::
operator=( const LLM_Train& other ){
  _llms = other._llms;
  _examples = other._examples;
  _indices = other._indices;
  return (*this);
}

void
LLM_Train::
compute_objective_thread( vector< LLM_Index_Map_Cell >& cells, LLM* llm, double& objective ){
  objective = 0.0;
  for( unsigned int i = 0; i < cells.size(); i++ ){
    for( unsigned int k = 0; k < cells[ i ].llm_x().cvs().size(); k++ ){
      if( cells[ i ].cv() == cells[ i ].llm_x().cvs()[ k ] ){
        objective += log( llm->pygx( cells[ i ].cv(), cells[ i ].llm_x(), cells[ i ].llm_x().cvs(), cells[ i ].indices() ) );
      }
    }
  }
  return;
}

double
LLM_Train::
objective( const vector< pair< string, LLM_X > >& examples,
            const vector< vector< vector< unsigned int > > >& indices,
            double lambda ){
  double objective = 0.0;

//  map< const h2sl::World*, vector< LLM_Index_Map_Cell > >::iterator it = _index_map.begin();
  vector< vector< LLM_Index_Map_Cell > >::iterator it = _index_vector.begin();
//  while( it != _index_map.end() ){
  while( it != _index_vector.end() ){
    vector< boost::thread > threads;
    vector< double > objectives( _llms.size(), 0.0 );
    for( unsigned int i = 0; i < _llms.size(); i++ ){
//      if( it != _index_map.end() ){
      if( it != _index_vector.end() ){
//        threads.push_back( boost::thread( LLM_Train::compute_objective_thread, it->second, _llms[ i ], boost::ref( objectives[ i ] ) ) );
        threads.push_back( boost::thread( LLM_Train::compute_objective_thread, *it, _llms[ i ], boost::ref( objectives[ i ] ) ) );
        it++;
      }
    }
   
    for( unsigned int i = 0; i < threads.size(); i++ ){
      threads[ i ].join();
    }

    for( unsigned int i = 0; i < objectives.size(); i++ ){
      objective += objectives[ i ];
    }   
  }

  double half_lambda = lambda / 2.0;
  for( unsigned int i = 0; i < _llms.front()->weights().size(); i++ ){
    objective -= half_lambda * _llms.front()->weights()[ i ] * _llms.front()->weights()[ i ];
  }
  return objective;
}

void
LLM_Train::
compute_gradient_thread( vector< LLM_Index_Map_Cell >& cells, LLM* llm, std::vector< double >& gradient ){
  for( unsigned int i = 0; i < cells.size(); i++ ){
    for( unsigned int k = 0; k < cells[ i ].llm_x().cvs().size(); k++ ){
      double tmp = llm->pygx( cells[ i ].llm_x().cvs()[ k ], cells[ i ].llm_x(), cells[ i ].llm_x().cvs(), cells[ i ].indices() );
      for( unsigned int l = 0; l < cells[ i ].indices()[ k ].size(); l++ ){
        gradient[ cells[ i ].indices()[ k ][ l ] ] -= tmp;
      }
      if( cells[ i ].cv() == cells[ i ].llm_x().cvs()[ k ] ){
        for( unsigned int l = 0; l < cells[ i ].indices()[ k ].size(); l++ ){
          gradient[ cells[ i ].indices()[ k ][ l ] ] += 1.0;
        }
      }
    }
  }
  return;
}

void
LLM_Train::
gradient( double lambda ){
  for( unsigned int i = 0; i < _gradient.size(); i++ ){
    _gradient[ i ] = 0.0;
  }

  vector< vector< LLM_Index_Map_Cell > >::iterator it = _index_vector.begin();
  while( it != _index_vector.end() ){
    vector< boost::thread > threads;
    vector< vector< double > > gradients( _llms.size(), vector< double >( _llms.front()->weights().size(), 0.0 ) );
    for( unsigned int i = 0; i < _llms.size(); i++ ){
      if( it != _index_vector.end() ){
        threads.push_back( boost::thread( LLM_Train::compute_gradient_thread, *it, _llms[ i ], boost::ref( gradients[ i ] ) ) );
        it++;
      }
    }
    
    for( unsigned int i = 0; i < threads.size(); i++ ){
      threads[ i ].join();
    }

    for( unsigned int i = 0; i < gradients.size(); i++ ){
      assert( _gradient.size() == gradients[ i ].size() );
      for( unsigned int j = 0; j < gradients[ i ].size(); j++ ){
        _gradient[ j ] += gradients[ i ][ j ];
      }
    }     
  }
  
  for( unsigned int i = 0; i < _llms.front()->weights().size(); i++ ){
    _gradient[ i ] -= lambda * _llms.front()->weights()[ i ];
  }

  return;
}

void
LLM_Train::
compute_indices_thread( vector< LLM_Index_Map_Cell >& cells, LLM* llm ){
  vector< bool > evaluate_feature_types( NUM_FEATURE_TYPES, true );
  const h2sl::Phrase * last_phrase = NULL;

  for( unsigned int i = 0; i < cells.size(); i++ ){
    if( last_phrase != cells[ i ].llm_x().phrase() ){
      evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = true;
    } else {
      evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
    }
    last_phrase = cells[ i ].llm_x().phrase();

    for( unsigned int k = 0; k < cells[ i ].llm_x().cvs().size(); k++ ){
      cells[ i ].indices().push_back( vector< unsigned int >() );
      vector< Feature* > features;


      llm->feature_set()->indices( cells[ i ].llm_x().cvs()[ k ],
                                              cells[ i ].llm_x().grounding(),
                                              cells[ i ].llm_x().children(),
                                              cells[ i ].llm_x().phrase(),
                                              cells[ i ].llm_x().world(), 
                                              cells[ i ].llm_x().context(), 
                                              cells[ i ].indices().back(), 
                                              features,
                                              evaluate_feature_types );
    }
  }

  return;
}

void
LLM_Train::
compute_indices( void ){
  _indices.clear();
  _indices.resize( _examples->size() );
  _index_vector.clear();

  vector< const h2sl::World* > world_vector;
  for( unsigned int i = 0; i < _examples->size(); i++ ){
    const LLM_X& example = (*_examples)[ i ].second;
    bool new_world = true;
    for( unsigned int j = 0; j < world_vector.size(); j++ ){
      if( world_vector[ j ] == example.world() ){
        new_world = false;
      }
    }  
    if( new_world ){
      world_vector.push_back( example.world() );
    }
  }

  map< const h2sl::World*, unsigned int > world_map;
  for( unsigned int i = 0; i < world_vector.size(); i++ ){
    world_map.insert( pair< const h2sl::World*, unsigned int >( world_vector[ i ], i % _llms.size() ) ); 
  }
 
  _index_vector.resize( _llms.size() );
 
  for( unsigned int i = 0; i < _examples->size(); i++ ){
    const string& cv = (*_examples)[ i ].first;
    const LLM_X& example = (*_examples)[ i ].second;
    map< const h2sl::World*, unsigned int >::iterator it = world_map.find( example.world() );
    assert( it != world_map.end() );
    _index_vector[ it->second ].push_back( LLM_Index_Map_Cell( i, cv, example, _indices[ i ] ) );
  }

  vector< vector< LLM_Index_Map_Cell > >::iterator it = _index_vector.begin();
  while( it != _index_vector.end() ){
    vector< boost::thread > threads;
    for( unsigned int i = 0; i < _llms.size(); i++ ){
      if( it != _index_vector.end() ){
        cout << "starting thread with " << (*it).size() << " examples" << endl;
        threads.push_back( boost::thread( LLM_Train::compute_indices_thread, *it, _llms[ i ] ) );
        it++;
      }
    }

    for( unsigned int i = 0; i < threads.size(); i++ ){
      threads[ i ].join();
    }
  }

  return;
}
