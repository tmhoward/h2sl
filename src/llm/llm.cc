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
#include <boost/algorithm/string.hpp>
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
//  pair< vector< pair< unsigned int, LLM_X > >*, LLM* > * data = static_cast< pair< vector< pair< unsigned int, LLM_X > >*, LLM* >* >( instance );
  LLM_Train* llm_train = static_cast< LLM_Train* >( instance );

  llm_train->llm()->weights().resize( n, 0.0 );
  for( unsigned int i = 0; i < n; i++ ){
    llm_train->llm()->weights()[ i ] = x[ i ];
  }
  
  lbfgsfloatval_t objective = ( lbfgsfloatval_t )( LLM::objective( llm_train->llm(), *llm_train->examples(), llm_train->indices(), 0.001 ) );

  vector< double > gradient( n );
  LLM::gradient( llm_train->llm(), *llm_train->examples(), llm_train->indices(), gradient, 0.001 );

  for( unsigned int i = 0; i < gradient.size(); i++ ){
    g[i] = -gradient[i];
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
LLM_X() : _grounding( NULL ),
          _children(),
          _phrase( NULL ),
          _world( NULL ),
          _cvs() {

}

LLM_X::
~LLM_X() {

}

LLM_X::
LLM_X( const LLM_X& other ) : _grounding( NULL ),
                              _children(),
                              _phrase( NULL ),
                              _world( NULL ),
                              _cvs( other._cvs ) {
  if( other._grounding != NULL ){
    _grounding = other._grounding->dup();
  }
  _children.resize( other._children.size() );
  for( unsigned int i = 0; i < other._children.size(); i++ ){
    if( other._children[ i ] != NULL ){
      _children[ i ] = other._children[ i ]->dup();
    }
  }
  if( other._phrase != NULL ){
    _phrase = other._phrase->dup();
  }
  if( other._world != NULL ){
    _world = other._world->dup();
  }
}

LLM_X&
LLM_X::
operator=( const LLM_X& other ) {
  if( other._grounding != NULL ){
    _grounding = other._grounding->dup();
  }
  _children.resize( other._children.size() );
  for( unsigned int i = 0; i < other._children.size(); i++ ){
    if( other._children[ i ] != NULL ){
      _children[ i ] = other._children[ i ]->dup();
    }
  }
  if( other._phrase != NULL ){
    _phrase = other._phrase->dup();
  }
  if( other._world != NULL ){
    _world = other._world->dup();
  }
  _cvs = other._cvs;
  return *this;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const LLM_X& other ) {
    if( dynamic_cast< const Region* >( other.grounding() ) != NULL ){
      out << "grounding:(" << *static_cast< const Region* >( other.grounding() ) << ") ";
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
pygx( const unsigned int& cv,
      const LLM_X& x,
      const vector< unsigned int >& cvs,
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
pygx( const unsigned int& cv,
      const LLM_X& x,
      const vector< unsigned int >& cvs,
      vector< unsigned int >& indices ){
  double numerator = 0.0;
  double denominator = 0.0;
  vector< unsigned int > tmp;
  vector< bool > evaluate_feature_types( NUM_FEATURE_TYPES, true );
  for( unsigned int i = 0; i < cvs.size(); i++ ){
    if( i != 0 ){
      evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
      evaluate_feature_types[ FEATURE_TYPE_GROUNDING ] = false;
    }
    double dp = 0.0;
    _feature_set->indices( cvs[ i ], x.grounding(), x.children(), x.phrase(), x.world(), tmp, evaluate_feature_types );
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
pygx( const unsigned int& cv,
      const LLM_X& x,
      const vector< unsigned int >& cvs ){
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
    _feature_set->indices( cvs[ i ], x.grounding(), x.children(), x.phrase(), x.world(), indices, evaluate_feature_types );
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
pygx( const unsigned int& cv,
      const Grounding* grounding,
      const vector< Grounding* >& children,
      const Phrase* phrase,
      const World* world,
      const vector< unsigned int >& cvs ){
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
    _feature_set->indices( cvs[ i ], grounding, children, phrase, world, indices, evaluate_feature_types );
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
pygx( const unsigned int& cv,
      const Grounding* grounding,
      const vector< Grounding* >& children,
      const Phrase* phrase,
      const World* world,
      const vector< unsigned int >& cvs,
      const vector< bool >& evaluateFeatureTypes ){
  double numerator = 0.0;
  double denominator = 0.0;
  vector< unsigned int > indices;
  vector< bool > evaluate_feature_types = evaluateFeatureTypes;
  for( unsigned int i = 0; i < cvs.size(); i++ ){
    if( i != 0 ){
      evaluate_feature_types[ FEATURE_TYPE_LANGUAGE ] = false;
      evaluate_feature_types[ FEATURE_TYPE_GROUNDING ] = false;
    }
    double dp = 0.0;
    _feature_set->indices( cvs[ i ], grounding, children, phrase, world, indices, evaluate_feature_types );
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
LLM::
train( vector< pair< unsigned int, LLM_X > >& examples,
        const unsigned int& maxIterations,
        const double& lambda,
        const double& epsilon ){

  if( _feature_set->size() != _weights.size() ){
    _weights.resize( _feature_set->size(), 0.0 );
  }
  
  lbfgsfloatval_t fx;
  lbfgsfloatval_t * x = lbfgs_malloc( _feature_set->size() );

  for( unsigned int i = 0; i < _weights.size(); i++ ){
    x[ i ] = _weights[ i ];
  }

  lbfgs_parameter_t param;
  lbfgs_parameter_init(&param);
  param.epsilon = epsilon;
  param.max_iterations = maxIterations;

  LLM_Train* llm_train = new LLM_Train( this, &examples );  
  llm_train->compute_indices();

  lbfgs( _weights.size(), x, &fx, evaluate, progress, ( void* )( llm_train ), &param );

  for( unsigned int i = 0; i < _weights.size(); i++ ){
    _weights[ i ] = x[ i ];
  }

  lbfgs_free( x );

  if( llm_train != NULL ){
    delete llm_train;
    llm_train = NULL;
  }

  return;
}

double 
LLM::
objective( LLM* llm, 
            const vector< pair< unsigned int, LLM_X > >& examples, 
            const vector< vector< vector< unsigned int > > >& indices,
            double lambda ){
  double o = 0.0;
  for( unsigned int i = 0; i < examples.size(); i++ ){
    for( unsigned int k = 0; k < examples[ i ].second.cvs().size(); k++ ){
      if( examples[ i ].first == examples[ i ].second.cvs()[ k ] ){
        o += log( llm->pygx( examples[ i ].first, examples[ i ].second, examples[ i ].second.cvs(), indices[ i ] ) );
      }
    }
  }
  double half_lambda = lambda / 2.0;
  for( unsigned int i = 0; i < llm->weights().size(); i++ ){
    o -= half_lambda * llm->weights()[ i ] * llm->weights()[ i ];
  }
  return o;
}

void 
LLM::
gradient( LLM* llm, 
          const vector< pair< unsigned int, LLM_X > >& examples, 
          const vector< vector< vector< unsigned int > > >& indices,
          vector< double >& g, 
          double lambda ){
  g.resize( llm->weights().size(), 0.0 );
  for( unsigned int i = 0; i < examples.size(); i++ ){
    for( unsigned int k = 0; k < examples[ i ].second.cvs().size(); k++ ){
      double tmp = llm->pygx( examples[ i ].second.cvs()[ k ], examples[ i ].second, examples[ i ].second.cvs(), indices[ i ] );
      for( unsigned int l = 0; l < indices[ i ][ k ].size(); l++ ){
        g[ indices[ i ][ k ][ l ] ] -= tmp;
      }
      if( examples[ i ].first == examples[ i ].second.cvs()[ k ] ){
        for( unsigned int l = 0; l < indices[ i ][ k ].size(); l++ ){
          g[ indices[ i ][ k ][ l ] ] += 1.0;
        }
      }
    }
  }
  for( unsigned int i = 0; i < llm->weights().size(); i++ ){
    g[ i ] -= lambda * llm->weights()[ i ];
  }

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
LLM_Train( LLM* llm,
            vector< pair< unsigned int, LLM_X > >* examples ) : _llm( llm ),
                                                                _examples( examples ),
                                                                _indices() {

}

LLM_Train::
~LLM_Train(){
  
}

LLM_Train::
LLM_Train( const LLM_Train& other ) : _llm( other._llm ),
                                      _examples( other._examples ),
                                      _indices( other._indices ){

}
    
LLM_Train&
LLM_Train::
operator=( const LLM_Train& other ){
  _llm = other._llm;
  _examples = other._examples;
  _indices = other._indices;
  return (*this);
}

void
LLM_Train::
compute_indices( void ){
  vector< bool > evaluate_feature_types( NUM_FEATURE_TYPES, true );
  _indices.clear();
  for( unsigned int i = 0; i < _examples->size(); i++ ){
    _indices.push_back( vector< vector< unsigned int > >() );
    for( unsigned int k = 0; k < (*_examples)[ i ].second.cvs().size(); k++ ){
      _indices.back().push_back( vector< unsigned int >() );
      _llm->feature_set()->indices( (*_examples)[ i ].second.cvs()[ k ], 
                                    (*_examples)[ i ].second.grounding(), 
                                    (*_examples)[ i ].second.children(), 
                                    (*_examples)[ i ].second.phrase(), 
                                    (*_examples)[ i ].second.world(), _indices.back().back(), evaluate_feature_types );
    }
  }
  return;
}
