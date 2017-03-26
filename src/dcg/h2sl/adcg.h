/**
 * @file    adcg.h
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
 * The interface for a class used to represent the 
 * Adaptive Distributed Correspondence Graph model 
 * that uses adaptive search space generation
 */

#ifndef H2SL_ADCG_H
#define H2SL_ADCG_H

#include <iostream>
#include <libxml/tree.h>

#include <vector>
#include <map>

#include "h2sl/phrase.h"
#include "h2sl/world.h"
#include "h2sl/llm.h"
#include "h2sl/factor_set.h"
#include "h2sl/symbol_dictionary.h"
#include "h2sl/dcg.h"

namespace h2sl {
  class ADCG : public DCG {
  public:
    ADCG();
    virtual ~ADCG();
    ADCG( const ADCG& other );
    ADCG& operator=( const ADCG& other );

 virtual bool leaf_search( const Phrase* phrase, 
                           const Symbol_Dictionary& symbolDictionary, 
                           Search_Space* searchSpace, 
                           const World* world, LLM* llm, 
                           const unsigned int beamWidth = 4, 
                           const bool& debug = false );

 virtual bool leaf_search( const Phrase* phrase, 
                           const Symbol_Dictionary& symbolDictionary, 
                           Search_Space* searchSpace, 
                           const World* world, 
                           const Grounding* context, 
                           LLM* llm, 
                           const unsigned int beamWidth = 4, 
                          const bool& debug = false );

  
    // XML
    virtual bool from_xml( const std::string& file );
    virtual bool from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
  
    // Latex
    virtual void to_latex( const std::string& filename )const;

  protected:
    // helper functions.
    virtual void _fill_factors( Factor_Set* node, const Phrase* phrase, const bool& fill = false );

  private:

  };
  std::ostream& operator<<( std::ostream& out, const ADCG& other );
}

#endif /* H2SL_ADCG_H */
