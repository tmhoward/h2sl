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
#include <string>
#include <map>
#include <libxml/tree.h>

#include "h2sl/word.h"
#include "h2sl/world.h"
#include "h2sl/grounding_set.h"

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
    PHRASE_M,
    NUM_PHRASE_TYPES
  } phrase_type_t;  

  class Phrase {
  public:
    Phrase( const phrase_type_t& type = PHRASE_UNKNOWN, const std::string& text = "na", const std::vector< Word >& words = std::vector< Word >(), const std::vector< Phrase* >& children = std::vector< Phrase* >(), Grounding_Set* groundingSet = NULL, const std::map< std::string, std::string >& properties = std::map< std::string, std::string >() );
    Phrase( const std::string& filename, World* world );
    virtual ~Phrase();
    Phrase( const Phrase& other );
    Phrase& operator=( const Phrase& other );
    bool operator==( const Phrase& other )const;
    bool operator!=( const Phrase& other )const;
    virtual Phrase* dup( void )const;
    virtual Phrase* dup( const bool& empty )const;

    virtual void clear_grounding_sets( void );

    virtual void scrape_groundings( const World * world, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    virtual void scrape_groundings( const World * world, std::vector< std::string >& classNames, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    virtual void scrape_groundings( const World * world, std::map< std::string, std::vector< std::string > >& classNames, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    bool contains_symbol_in_symbol_dictionary( const Symbol_Dictionary& symbolDictionary )const;

    virtual void to_file( const std::string& filename )const;

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename, World* world );
    virtual void from_xml( xmlNodePtr root, World* world );

    bool has_word( const Word& word )const;
    bool has_words( const std::vector< Word >& words )const;
    unsigned int min_word_order( void )const;

    unsigned int num_phrases( void )const;
    unsigned int num_phrases( const Phrase* phrase )const;
    unsigned int num_words( void )const;
    unsigned int num_words( const Phrase* phrase )const;

    double aggregate_property_phrases( const std::string& property )const;
    double aggregate_property_phrases( const Phrase* phrase, 
                                       const std::string& property )const;

    double statistic_aggregate_property_phrases( const std::string& property,
                                                 const std::string& statistic )const;

    double statistic_aggregate_property_phrases( const Phrase* phrase,
                                                 const std::string& property,
                                                 const std::string& statistic )const; 

    virtual void to_tikz( const std::string& filename, const std::string& caption = "tbd", const std::string& lavel = "fig:tbd" )const;
    virtual std::string to_tikz_nodes_gm( const Phrase* phrase, unsigned int& offset )const;
    virtual std::string to_tikz_edges_gm( const Phrase* phrase, unsigned int& offset )const;   
 
    std::string words_to_std_string( void )const;
    std::string all_words_to_std_string( void )const;

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
    inline Grounding_Set*& grounding_set( void ){ return _grounding_set; };
    inline const Grounding_Set* grounding_set( void )const{ return _grounding_set; };
    inline std::map< std::string, std::string >& properties( void ){ return _properties; };
    inline const std::map< std::string, std::string >& properties( void )const{ return _properties; };

  protected:
    phrase_type_t _type;
    std::string _text;
    std::vector< Word > _words;
    std::vector< Phrase* > _children;
    Grounding_Set * _grounding_set;
    std::map< std::string, std::string > _properties;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Phrase& other );
}

#endif /* H2SL_PHRASE_H */
