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

#ifndef H2SL_FACTOR_SET_ADCG_H
#define H2SL_FACTOR_SET_ADCG_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "h2sl/grounding.h"
#include "h2sl/phrase.h"
#include "h2sl/world.h"
#include "h2sl/llm.h"

#include "h2sl/factor_set.h"

//namespace std {
namespace h2sl {

class Factor_Set_ADCG : public Factor_Set {
  public:
    Factor_Set_ADCG( const Phrase* phrase = NULL );
    virtual ~Factor_Set_ADCG();
    Factor_Set_ADCG( const Factor_Set_ADCG& other );
    Factor_Set_ADCG& operator=( const Factor_Set_ADCG& other );

    // Factor Set Search
    virtual void search( const std::vector< std::pair< unsigned int, Grounding* > >& searchSpace, 
                         const std::vector< std::vector< unsigned int > >& correspondenceVariables, 
                         const std::map< std::string, std::vector< std::string > >& symbolTypes,
                         const World* world, LLM* llm, 
                         const unsigned int beamWidth = 4, 
                         const bool& debug = false );

    virtual void search( const std::vector< std::pair< unsigned int, Grounding* > >& searchSpace, 
                         const std::vector< std::vector< unsigned int > >& correspondenceVariables, 
                         const std::map< std::string, std::vector< std::string > >& symbolTypes,
                         const World* world, const Grounding* grounding, LLM* llm, 
                         const unsigned int beamWidth = 4, 
                         const bool& debug = false );

    // Accessors
    inline std::vector< std::vector< std::pair< unsigned int, Grounding* > > >& abstract_search_spaces( void ){ return _abstract_search_spaces; };
    inline const std::vector< std::vector< std::pair< unsigned int, Grounding* > > >& abstract_search_spaces( void )const{ return _abstract_search_spaces; };
    inline std::vector< std::vector< unsigned int > >& abstract_correspondence_variables( void ){ return _abstract_correspondence_variables; };
    inline const std::vector< std::vector< unsigned int > >& abstract_correspondence_variables( void )const{ return _abstract_correspondence_variables; };

  protected:

    // Physical search space search
    virtual void _search_physical( const std::vector< std::pair< unsigned int, Grounding* > >& searchSpace, 
                                   const std::vector< std::vector< unsigned int > >& correspondenceVariables, 
                                   const std::map< std::string, std::vector< std::string > >& symbolTypes,
                                   const World* world, LLM* llm, 
                                   const unsigned int beamWidth = 4, 
                                   const bool& debug = false );

    std::vector< std::vector< std::pair< unsigned int, Grounding* > > > _abstract_search_spaces;
    std::vector< std::vector< unsigned int > > _abstract_correspondence_variables;
  private:

  };
  std::ostream& operator<<( std::ostream& out, const Factor_Set_ADCG& other );
}
//}

#endif /* H2SL_FACTOR_SET_ADCG_H */

