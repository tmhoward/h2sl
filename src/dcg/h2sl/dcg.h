/**
 * @file    dcg.h
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
 * The interface for a class used to represent the Distributed Correspondence Graph
 */

#ifndef H2SL_DCG_H
#define H2SL_DCG_H

#include <iostream>

#include <h2sl/phrase.h>
#include <h2sl/cv.h>
#include <h2sl/world.h>
#include <h2sl/llm.h>
#include <h2sl/factor.h>

namespace h2sl {
  class DCG {
  public:
    DCG();
    virtual ~DCG();
    DCG( const DCG& other );
    DCG& operator=( const DCG& other );

    void construct( Phrase& phrase, const World& world, LLM& llm, const bool& fill=false );
    bool search( Phrase& phrase, const World& world, LLM& llm, std::vector< std::pair< double, Phrase > >& solutions, const unsigned int& beamWidth = 4 );
    bool step( std::vector< std::pair< double, std::vector< unsigned int > > >& solutions, const unsigned int& beamWidth );
    void score( std::pair< double, std::vector< unsigned int > >& solution, const unsigned int searchIndex );

    inline std::vector< Phrase* >& phrases( void ){ return _phrases; };
    inline const std::vector< Phrase* >& phrases( void )const{ return _phrases; };
    inline std::vector< Factor* >& factors( void ){ return _factors; };
    inline const std::vector< Factor* >& factors( void )const{ return _factors; };

  protected:
    virtual void _add_phrase_to_model( Phrase& phrase, const World& world, LLM& llm, const bool& fill );
    virtual void _add_vp_phrase_to_model( Phrase& phrase, const World& world, LLM& llm, const bool& fill );
    virtual void _add_pp_phrase_to_model( Phrase& phrase, const World& world, LLM& llm, const bool& fill );
    virtual void _add_np_phrase_to_model( Phrase& phrase, const World& world, LLM& llm, const bool& fill );
    virtual void _connect_children( Phrase& phrase );

    virtual void _fill_phrase_from_model( Phrase& phrase );
    virtual void _fill_vp_phrase_from_model( Phrase& phrase );
    virtual void _fill_pp_phrase_from_model( Phrase& phrase );
    virtual void _fill_np_phrase_from_model( Phrase& phrase );

    Phrase * _phrase;    
    std::vector< Phrase* > _phrases;
    std::vector< Factor* > _factors;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const DCG& other );
}

#endif /* H2SL_DCG_H */
