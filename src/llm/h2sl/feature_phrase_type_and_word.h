/**
 * @file    feature_phrase_type_and_word.h
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
 * The interface for a class that looks for a word in a phrase
 */

#ifndef H2SL_FEATURE_PHRASE_TYPE_AND_WORD_H
#define H2SL_FEATURE_PHRASE_TYPE_AND_WORD_H

#include <iostream>

#include "h2sl/phrase.h"
#include "h2sl/feature.h"

namespace h2sl {
  class Feature_Phrase_Type_And_Word: public Feature {
  public:
    Feature_Phrase_Type_And_Word( const bool& invert = false, const Word& word = Word(), const phrase_type_t& phraseType = PHRASE_UNKNOWN );
    virtual ~Feature_Phrase_Type_And_Word();
    Feature_Phrase_Type_And_Word( const Feature_Phrase_Type_And_Word& other );
    Feature_Phrase_Type_And_Word& operator=( const Feature_Phrase_Type_And_Word& other );

    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );

    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( xmlNodePtr root );

    inline Word& word( void ){ return _word; };
    inline const Word& word( void )const{ return _word; };
    inline phrase_type_t& phrase_type( void ){ return _phrase_type; };
    inline const phrase_type_t& phrase_type( void )const{ return _phrase_type; };

    virtual inline const feature_type_t type( void )const{ return FEATURE_TYPE_LANGUAGE; };

  protected:
    Word _word;
    phrase_type_t _phrase_type;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Feature_Phrase_Type_And_Word& other );
}

#endif /* H2SL_FEATURE_PHRASE_TYPE_AND_WORD_H */
