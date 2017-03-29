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
#include <map>
#include <libxml/tree.h>

#include <h2sl/grounding.h>
#include <h2sl/cv.h>
#include <h2sl/feature_set.h>

namespace h2sl {
  class LLM_X {
  public:
    LLM_X( const Grounding* grounding, const Phrase* phrase, const World* world, const std::vector< std::string >& cvs, const std::vector< Feature* >& features, const std::string& filename );
    LLM_X( const Grounding* grounding, const Phrase* phrase, const World* world, const Grounding* context, const std::vector< std::string >& cvs, const std::vector< Feature* >& features, const std::string& filename );
    virtual ~LLM_X();
    LLM_X( const LLM_X& other );
    LLM_X& operator=( const LLM_X& other );
  
    inline const Grounding* grounding( void )const{ return _grounding; };
    inline const Phrase* phrase( void )const{ return _phrase; }; 
    inline const World* world( void )const{ return _world; }; 
    inline const Grounding* context( void )const{ return _context; };
    inline std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children( void ){ return _children; };   
    inline const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children( void )const{ return _children; };
    inline const std::vector< std::string >& cvs( void )const{ return _cvs; };
    inline std::vector< Feature* >& features( void ){ return _features; };
    inline const std::string& filename( void )const{ return _filename; };

  protected:
    const Grounding* _grounding;
    const Phrase * _phrase;
    const World * _world;
    const Grounding* _context;
    std::vector< std::pair< const Phrase*, std::vector< Grounding* > > > _children;
    const std::vector< std::string >& _cvs;
    std::vector< Feature* > _features;
    const std::string& _filename;
  };
  std::ostream& operator<<( std::ostream& out, const LLM_X& other );

  class LLM {
  public:
    LLM( Feature_Set* featureSet = NULL );
    virtual ~LLM();
    LLM( const LLM& other );
    LLM& operator=( const LLM& other );

    double pygx( const std::string& cv, const LLM_X& x, const std::vector< std::string >& cvs, const std::vector< std::vector< unsigned int > >& indices );
    double pygx( const std::string& cv, const LLM_X& x, const std::vector< std::string >& cvs, std::vector< unsigned int >& indices );
    double pygx( const std::string& cv, const LLM_X& x, const std::vector< std::string >& cvs, std::vector< Feature* >& features );
    double pygx( const std::string& cv, const LLM_X& x, const std::vector< std::string >& cvs, std::vector< std::pair< std::vector< Feature* >, unsigned int > >& weightedFeatures );
    double pygx( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const std::vector< std::string >& cvs );
    double pygx( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context, const std::vector< std::string >& cvs );
    double pygx( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const std::vector< std::string >& cvs, const std::vector< bool >& evaluateFeatureTypes );
    double pygx( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context, const std::vector< std::string >& cvs, const std::vector< bool >& evaluateFeatureTypes );

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    inline std::vector< double >& weights( void ){ return _weights; };
    inline const std::vector< double >& weights( void )const{ return _weights; };
    inline Feature_Set*& feature_set( void ){ return _feature_set; };
    inline const Feature_Set* feature_set( void )const{ return _feature_set; };

  protected:
    std::vector< double > _weights;
    Feature_Set* _feature_set;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const LLM& other );

  class LLM_Index_Map_Cell {
  public:
    LLM_Index_Map_Cell( const unsigned int& index, const std::string& cv, const LLM_X& llmX, std::vector< std::vector< unsigned int > >& indices ) : _index( index ), _cv( cv ), _llm_x( llmX ), _indices( indices ) {};
    virtual ~LLM_Index_Map_Cell(){};

    inline const unsigned int& index( void )const{ return _index; };
    inline const std::string& cv( void )const{ return _cv; };
    inline const LLM_X& llm_x( void )const{ return _llm_x; };
    inline std::vector< std::vector< unsigned int > >& indices( void ){ return _indices; };

  protected:
    const unsigned int& _index;
    const std::string& _cv;
    const LLM_X& _llm_x;
    std::vector< std::vector< unsigned int > >& _indices;
  };

  class LLM_Train {
  public:
    LLM_Train( const std::vector< LLM* >& llms = std::vector< LLM* >(), std::vector< std::pair< std::string, LLM_X > >* examples = NULL );  
    ~LLM_Train();
    LLM_Train( const LLM_Train& other );
    LLM_Train& operator=( const LLM_Train& other );
 
    void train( std::vector< std::pair< std::string, LLM_X > >& examples, const unsigned int& maxIterations = 100, const double& lambda = 0.01, const double& epsilon = 0.001 );
    static void compute_objective_thread( std::vector< LLM_Index_Map_Cell >& cells, LLM* llm, double& objective );
    double objective( const std::vector< std::pair< std::string, LLM_X > >& examples, const std::vector< std::vector< std::vector< unsigned int > > >& indices, double lambda );
    static void compute_gradient_thread( std::vector< LLM_Index_Map_Cell >& cells, LLM* llm, std::vector< double >& gradient );
    void gradient( double lambda ); 
    static void compute_indices_thread( std::vector< LLM_Index_Map_Cell >& cells, LLM* llm );
    void compute_indices( void );

    inline std::vector< LLM* >& llms( void ){ return _llms; };
    inline std::vector< std::pair< std::string, LLM_X > >*& examples( void ){ return _examples; };
    inline std::vector< double >& gradient( void ){ return _gradient; };
    inline std::vector< std::vector< std::vector< unsigned int > > >& indices( void ){ return _indices; };
    inline std::vector< std::vector< std::vector< Feature* > > >& features( void ){ return _features; };

  protected:
    std::vector< LLM* > _llms;
    std::vector< std::pair< std::string, LLM_X > >* _examples; 
    std::vector< std::vector< LLM_Index_Map_Cell > > _index_vector;
    std::vector< double > _gradient;
    std::vector< std::vector< std::vector< unsigned int > > > _indices;
    std::vector< std::vector< std::vector< Feature* > > > _features;
  };
}

#endif /* H2SL_LLM_H */
