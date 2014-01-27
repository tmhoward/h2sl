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
  pair< vector< pair< unsigned int, LLM_X > >*, LLM* > * data = static_cast< pair< vector< pair< unsigned int, LLM_X > >*, LLM* >* >( instance );

  data->second->weights().resize( n, 0.0 );
  for( unsigned int i = 0; i < n; i++ ){
    data->second->weights()[ i ] = x[ i ];
  }
  
  lbfgsfloatval_t objective = ( lbfgsfloatval_t )( LLM::objective( *data->second, *data->first, 0.001 ) );

  vector< double > gradient( n );
  LLM::gradient( *data->second, *data->first, gradient, 0.001 );

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
          _phrase( NULL ) {

}

LLM_X::
~LLM_X() {

}

LLM_X::
LLM_X( const LLM_X& other ) : _grounding( NULL ),
                              _children(),
                              _phrase( NULL ) {
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
LLM() : _weights(),
        _feature_set(){

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
      vector< unsigned int >& indices ){
  double numerator = 0.0;
  double denominator = 0.0;
  vector< unsigned int > tmp;
  for( unsigned int i = 0; i < cvs.size(); i++ ){
    double dp = 0.0;
    _feature_set.indices( cvs[ i ], x.grounding(), x.children(), x.phrase(), tmp );
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
  for( unsigned int i = 0; i < cvs.size(); i++ ){
    double dp = 0.0;
    _feature_set.indices( cvs[ i ], x.grounding(), x.children(), x.phrase(), indices );
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
      const vector< unsigned int >& cvs ){
  double numerator = 0.0;
  double denominator = 0.0;
  vector< unsigned int > indices;
  for( unsigned int i = 0; i < cvs.size(); i++ ){
    double dp = 0.0;
    _feature_set.indices( cvs[ i ], grounding, children, phrase, indices );
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
        const double& lambda ){

  if( _feature_set.size() != _weights.size() ){
    _weights.resize( _feature_set.size(), 0.0 );
  }
  
  lbfgsfloatval_t fx;
  lbfgsfloatval_t * x = lbfgs_malloc( _feature_set.size() );

  for( unsigned int i = 0; i < _weights.size(); i++ ){
    x[ i ] = _weights[ i ];
  }

  lbfgs_parameter_t param;
  lbfgs_parameter_init(&param);
  param.epsilon = 0.001;
  param.max_iterations = maxIterations;

  pair< vector< pair< unsigned int, LLM_X > >*, LLM* > instance( &examples, this );
  
  lbfgs( _weights.size(), x, &fx, evaluate, progress, ( void* )( &instance ), &param );

  for( unsigned int i = 0; i < _weights.size(); i++ ){
    _weights[ i ] = x[ i ];
  }

  lbfgs_free( x );

  return;
}

double 
LLM::
objective( LLM& llm, 
            const vector< pair< unsigned int, LLM_X > >& examples, 
            double lambda ){
  double o = 0.0;
  vector< unsigned int > cvs;
  cvs.push_back( CV_FALSE );
  cvs.push_back( CV_TRUE );
  for( unsigned int i = 0; i < examples.size(); i++ ){
    o += log( llm.pygx( examples[ i ].first, examples[ i ].second, cvs ) );
  }
  double half_lambda = lambda / 2.0;
  for( unsigned int i = 0; i < llm.weights().size(); i++ ){
    o -= half_lambda * llm.weights()[ i ] * llm.weights()[ i ];
  }
  return o;
}

void 
LLM::
gradient( LLM& llm, 
          const vector< pair< unsigned int, LLM_X > >& examples, 
          vector< double >& g, 
          double lambda ){
  g.resize( llm.weights().size(), 0.0 );
  vector< unsigned int > cvs;
  cvs.push_back( CV_FALSE );
  cvs.push_back( CV_TRUE );
  vector< unsigned int > indices;
  for( unsigned int i = 0; i < examples.size(); i++ ){
    for( unsigned int k = 0; k < cvs.size(); k++ ){
      double tmp = llm.pygx( cvs[ k ], examples[ i ].second, cvs, indices );
      for( unsigned int l = 0; l < indices.size(); l++ ){
        g[ indices[ l ] ] -= tmp;
      }
      if( examples[ i ].first == cvs[ k ] ){
        for( unsigned int l = 0; l < indices.size(); l++ ){
          g[ indices[ l ] ] += 1.0;
        }
      }
    }
  }
  for( unsigned int i = 0; i < llm.weights().size(); i++ ){
    g[ i ] -= lambda * llm.weights()[ i ];
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
  _feature_set.to_xml( doc, node );
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
          _feature_set.from_xml( l1 );
        }
      }
    }  
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const LLM& other ) {
    return out;
  }

}
