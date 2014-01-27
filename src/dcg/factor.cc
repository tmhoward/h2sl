/**
 * @file    factor.cc
 * @author  Thomas M. Howard (tmhoward@csail.mit.edu)
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
 * The implementation of a class used to describe a model factor
 */

#include "h2sl/region.h"
#include <h2sl/factor.h>

using namespace std;
using namespace h2sl;

Factor::
Factor( const unsigned int& cv,
        Grounding* grounding,
        Phrase* phrase,
        const vector< Factor* >& children,
        LLM* llm,
        const vector< unsigned int >& cvs,
        const unsigned int& solutionIndex ) : _cv( cv ),
                                                  _grounding( grounding ),
                                                  _phrase( phrase ),
                                                  _children( children ),
                                                  _llm( llm ),
                                                  _cvs( cvs ),
                                                  _solution_index( solutionIndex ),
                                                  _pygx( 0.0 ){

}

Factor::
~Factor() {

}

Factor::
Factor( const Factor& other ) : _cv( other._cv ),
                                _grounding( other._grounding ),
                                _phrase( other._phrase ),
                                _children( other._children ),
                                _llm( other._llm ),
                                _cvs( other._cvs ),
                                _solution_index( other._solution_index ),
                                _pygx( other._pygx ){

}

Factor&
Factor::
operator=( const Factor& other ) {
  _cv = other._cv;
  _grounding = other._grounding;
  _children = other._children;
  _phrase = other._phrase;
  _llm = other._llm;
  _cvs = other._cvs;
  _solution_index = other._solution_index;
  _pygx = other._pygx;
  return (*this);
}

double
Factor::
value( const unsigned int& cv ){
  _cv = cv;
  if( _llm != NULL ){
    vector< Grounding* > children;
    for( unsigned int i = 0; i < _children.size(); i++ ){
      if( _children[ i ]->cv() == CV_TRUE ){
        children.push_back( _children[ i ]->grounding() );
      } 
    }
    _pygx = _llm->pygx( _cv, _grounding, children, _phrase, _cvs );
    return _pygx;
  } else {
    return 0.5;
  }
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Factor& other ) {
    out << "cv:\"" << other.cv() << "\" ";
    if( dynamic_cast< const Region* >( other.grounding() ) != NULL ){
      out << "grounding:(" << *static_cast< const Region* >( other.grounding() ) << ") ";
    } else {
      out << "grounding:(NULL) ";
    }
    if( other.phrase() != NULL ){
      out << "phrase:(\"" << other.phrase()->words_to_std_string() << "\") ";
    } else {
      out << "phrase:(NULL) ";
    }
    out << "children[" << other.children().size() << "]:{";
    for( unsigned int i = 0; i < other.children().size(); i++ ){
      out << other.children()[ i ];
      if( i != ( other.children().size() - 1 ) ){
        out << ",";
      }
    }
    out << "} ";
    out << "solution_index:\"" << other.solution_index() << "\"";
    return out;
  }
}
