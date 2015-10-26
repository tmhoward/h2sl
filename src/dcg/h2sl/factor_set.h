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
#include <algorithm>

#include "h2sl/grounding.h"
#include "h2sl/phrase.h"
#include "h2sl/world.h"
#include "h2sl/llm.h"

namespace h2sl {
  class Factor_Set_Solution {
  public:
    Factor_Set_Solution();
    virtual ~Factor_Set_Solution();
    Factor_Set_Solution( const Factor_Set_Solution& other );
    Factor_Set_Solution& operator=( const Factor_Set_Solution& other );
  
    std::vector< std::vector< unsigned int > > cv;
    std::vector< unsigned int > children;
    std::vector< Grounding* > groundings;
    double pygx;
  };

  class Factor_Set {
  public:
    Factor_Set( const Phrase* phrase = NULL );
    virtual ~Factor_Set();
    Factor_Set( const Factor_Set& other );
    Factor_Set& operator=( const Factor_Set& other );

    virtual void search( const std::vector< std::pair< unsigned int, Grounding* > >& searchSpace, const std::vector< std::vector< unsigned int > >& correspondenceVariables, const World* world, LLM* llm, const unsigned int beamWidth = 4, const bool& debug = false );

    inline const Phrase* phrase( void )const{ return _phrase; };

    inline std::vector< Factor_Set* >& children( void ){ return _children; };
    inline const std::vector< Factor_Set* >& children( void )const{ return _children; };

    inline std::vector< Factor_Set_Solution >& solutions( void ){ return _solutions; };
    inline const std::vector< Factor_Set_Solution >& solutions( void )const{ return _solutions; };

  protected:
    const Phrase* _phrase;
    std::vector< Factor_Set* > _children;
    std::vector< Factor_Set_Solution > _solutions;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Factor_Set& other );
}

#endif /* H2SL_FACTOR_SET_H */
