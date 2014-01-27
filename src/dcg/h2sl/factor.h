/**
 * @file    factor.h
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
 * The interface for a class used to describe a model factor
 */

#ifndef H2SL_FACTOR_H
#define H2SL_FACTOR_H

#include <iostream>
#include <vector>

#include <h2sl/grounding.h>
#include <h2sl/phrase.h>
#include <h2sl/cv.h>
#include <h2sl/llm.h>

namespace h2sl {
  class Factor {
  public:
    Factor( const unsigned int& cv = 0, Grounding* grounding = NULL, Phrase* phrase = NULL, const std::vector< Factor* >& children = std::vector< Factor* >(), LLM* llm = NULL, const std::vector< unsigned int >& cvs = std::vector< unsigned int >(), const unsigned int& solutionIndex = 0 );
    virtual ~Factor();
    Factor( const Factor& other );
    Factor& operator=( const Factor& other );

    double value( const unsigned int& cv );

    inline unsigned int& cv( void ){ return _cv; };
    inline const unsigned int& cv( void )const{ return _cv; };
    inline Grounding*& grounding( void ){ return _grounding; };
    inline const Grounding* grounding( void )const{ return _grounding; };
    inline Phrase*& phrase( void ){ return _phrase; };
    inline const Phrase* phrase( void )const{ return _phrase; };
    inline std::vector< Factor* >& children( void ){ return _children; };
    inline const std::vector< Factor* >& children( void )const{ return _children; };
    inline LLM*& llm( void ){ return _llm; };
    inline const LLM* llm( void )const{ return _llm; };
    inline std::vector< unsigned int >& cvs( void ){ return _cvs; };
    inline const std::vector< unsigned int >& cvs( void )const{ return _cvs; }; 
    inline unsigned int& solution_index( void ){ return _solution_index; };
    inline const unsigned int& solution_index( void )const{ return _solution_index; }; 
    inline const double& pygx( void )const{ return _pygx; };

  protected:
    unsigned int _cv;
    Grounding* _grounding;
    Phrase* _phrase;
    std::vector< Factor* > _children;
    LLM * _llm;
    std::vector< unsigned int > _cvs;
    unsigned int _solution_index;
    double _pygx;   
 
  private:

  };
  std::ostream& operator<<( std::ostream& out, const Factor& other );
}

#endif /* H2SL_FACTOR_H */
