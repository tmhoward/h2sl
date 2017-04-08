/**
 * @file    factor_set.h
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
 * The interface for a class used to represent a factor set
 */

#ifndef H2SL_FACTOR_SET_H
#define H2SL_FACTOR_SET_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include "h2sl/grounding.h"
#include "h2sl/grounding_set.h"
#include "h2sl/phrase.h"
#include "h2sl/world.h"
#include "h2sl/llm.h"
#include "h2sl/symbol_dictionary.h"
#include "h2sl/search_space.h"

namespace h2sl {
  class Factor_Set_Solution {
  public:
    Factor_Set_Solution( const std::vector< unsigned int >& childSolutionIndices = std::vector< unsigned int >(), 
                         const double& pygx = 1.0, 
                         Grounding_Set* groundingSet = new Grounding_Set() );
    virtual ~Factor_Set_Solution();
    Factor_Set_Solution( const Factor_Set_Solution& other );
    Factor_Set_Solution& operator=( const Factor_Set_Solution& other );

    inline std::vector< unsigned int >& child_solution_indices( void ){ return _child_solution_indices; };
    inline const std::vector< unsigned int >& child_solution_indices( void )const{ return _child_solution_indices; };
    
    inline Grounding_Set* grounding_set( void ){ return _grounding_set; };
    inline const Grounding_Set* grounding_set( void )const{ return _grounding_set; };
    
    inline double& pygx( void ){ return _pygx; };
    inline const double& pygx( void )const{ return _pygx; };

  protected:
    std::vector< unsigned int > _child_solution_indices;
    double _pygx;
    Grounding_Set * _grounding_set;
  };

  std::ostream& operator<<( std::ostream& out, const Factor_Set_Solution& other );

  class Factor_Set {
  public:
    Factor_Set( const Phrase* phrase = NULL );
    virtual ~Factor_Set();
    Factor_Set( const Factor_Set& other );
    Factor_Set& operator=( const Factor_Set& other );

    virtual void search_subspace( std::vector< Factor_Set_Solution >& solutionsVector,
                        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& childPhraseGroundings,   
                        const std::pair< std::string, std::vector< Grounding* > >& searchSubspace,
                        const std::vector< std::string >& correspondenceVariables,
                        std::vector< bool >& evaluate_feature_types,
                        const Grounding* context,
                        const World* world, LLM* llm, const unsigned int beamWidth = 4, const bool& debug = false, const bool& printGroundings = false ); 

    virtual void search( const Search_Space* searchSpace,
                         const Symbol_Dictionary& symbolDictionary,
                         const World* world, LLM* llm, const unsigned int beamWidth = 4, const bool& debug = false );

    virtual void search( const Search_Space* searchSpace, 
                         const Symbol_Dictionary& symbolDictionary,
                         const World* world, const Grounding* context, LLM* llm, const unsigned int beamWidth = 4, const bool& debug = false );

    inline const Phrase* phrase( void )const{ return _phrase; };
    inline std::vector< Factor_Set* >& child_factor_sets( void ){ return _child_factor_sets; };
    inline const std::vector< Factor_Set* >& child_factor_sets( void )const{ return _child_factor_sets; };
    inline std::vector< Factor_Set_Solution >& solutions( void ){ return _solutions; };
    inline const std::vector< Factor_Set_Solution >& solutions( void )const{ return _solutions; };
    inline std::map< std::string, std::string >& properties( void ){ return _properties; };
    inline const std::map< std::string, std::string >& properties( void )const{ return _properties; };

  protected:
    const Phrase* _phrase;
    std::vector< Factor_Set* > _child_factor_sets;
    std::vector< Factor_Set_Solution > _solutions;
    std::map< std::string, std::string > _properties;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Factor_Set& other );
}

#endif /* H2SL_FACTOR_SET_H */
