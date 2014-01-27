/**
 * @file    llm.h
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
 * The interface for a class used to represent a log-linear model
 */

#ifndef H2SL_LLM_H
#define H2SL_LLM_H

#include <iostream>
#include <vector>
#include <libxml/tree.h>

#include <h2sl/grounding.h>
#include <h2sl/cv.h>
#include <h2sl/feature_set.h>

namespace h2sl {
  class LLM_X {
  public:
    LLM_X();
    virtual ~LLM_X();
    LLM_X( const LLM_X& other );
    LLM_X& operator=( const LLM_X& other );
  
    inline Grounding*& grounding( void ){ return _grounding; };
    inline const Grounding* grounding( void )const{ return _grounding; };
    inline std::vector< Grounding* >& children( void ){ return _children; };   
    inline const std::vector< Grounding* >& children( void )const{ return _children; };
    inline Phrase*& phrase( void ){ return _phrase; };
    inline const Phrase* phrase( void )const{ return _phrase; }; 
 
  protected:
    Grounding* _grounding;
    std::vector< Grounding* > _children;
    Phrase * _phrase;
  };
  std::ostream& operator<<( std::ostream& out, const LLM_X& other );

  class LLM {
  public:
    LLM();
    virtual ~LLM();
    LLM( const LLM& other );
    LLM& operator=( const LLM& other );

    double pygx( const unsigned int& cv, const LLM_X& x, const std::vector< unsigned int >& cvs, std::vector< unsigned int >& indices );
    double pygx( const unsigned int& cv, const LLM_X& x, const std::vector< unsigned int >& cvs );
    double pygx( const unsigned int& cv, const Grounding* grounding, const std::vector< Grounding* >& children, const Phrase* phrase, const std::vector< unsigned int >& cvs );
    void train( std::vector< std::pair< unsigned int, LLM_X > >& examples, const unsigned int& maxIterations = 100, const double& lambda = 0.01 );

    static double objective( LLM& llm, const std::vector< std::pair< unsigned int, LLM_X > >& examples, double lambda );
    static void gradient( LLM& llm, const std::vector< std::pair< unsigned int, LLM_X > >& examples, std::vector< double >& g, double lambda );

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    inline std::vector< double >& weights( void ){ return _weights; };
    inline const std::vector< double >& weights( void )const{ return _weights; };
    inline Feature_Set& feature_set( void ){ return _feature_set; };
    inline const Feature_Set& feature_set( void )const{ return _feature_set; };

  protected:
    std::vector< double > _weights;
    Feature_Set _feature_set;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const LLM& other );
}

#endif /* H2SL_LLM_H */
