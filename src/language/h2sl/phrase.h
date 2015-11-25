/**
 * @file    phrase.h
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
 * The interface for a class used to represent a phrase
 */

#ifndef H2SL_PHRASE_H
#define H2SL_PHRASE_H

#include <iostream>
#include <vector>
#include <libxml/tree.h>

#include <h2sl/grounding.h>
#include <h2sl/word.h>

namespace h2sl {
  typedef enum {
    PHRASE_UNKNOWN,
    PHRASE_NP,
    PHRASE_VP,
    PHRASE_PP,
    PHRASE_ADJP,
    PHRASE_ADVP,
    PHRASE_PRT,
    PHRASE_S,
    PHRASE_SBAR,
    PHRASE_WHNP,
    PHRASE_WHVP,
    NUM_PHRASE_TYPES
  } phrase_type_t;  

  class Phrase : public Grounding {
  public:
    Phrase( const phrase_type_t& type = PHRASE_UNKNOWN, const std::string& text = "na", const std::vector< Word >& words = std::vector< Word >(), const std::vector< Phrase* >& children = std::vector< Phrase* >(), Grounding* grounding = NULL );
    virtual ~Phrase();
    Phrase( const Phrase& other );
    Phrase& operator=( const Phrase& other );
    bool operator==( const Phrase& other )const;
    bool operator!=( const Phrase& other )const;
    virtual Phrase* dup( void )const;
    virtual Phrase* dup( const bool& empty )const;

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    bool has_word( const Word& word )const;
    bool has_words( const std::vector< Word >& words )const;
    unsigned int min_word_order( void )const;
    
    std::string words_to_std_string( void )const;

    static std::string phrase_type_t_to_std_string( const phrase_type_t& phrase );
    static phrase_type_t phrase_type_t_from_std_string( const std::string& arg );

    inline phrase_type_t& type( void ){ return _type; };
    inline const phrase_type_t& type( void )const{ return _type; };
    inline std::string& text( void ){ return _text; };
    inline const std::string& text( void )const{ return _text; };
    inline std::vector< Phrase* >& children( void ){ return _children; };
    inline const std::vector< Phrase* >& children( void )const{ return _children; };
    inline std::vector< Word >& words( void ){ return _words; };
    inline const std::vector< Word >& words( void )const{ return _words; };
    inline Grounding*& grounding( void ){ return _grounding; };
    inline const Grounding* grounding( void )const{ return _grounding; };

  protected:
    phrase_type_t _type;
    std::string _text;
    std::vector< Word > _words;
    std::vector< Phrase* > _children;
    Grounding * _grounding;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Phrase& other );
}

#endif /* H2SL_PHRASE_H */
