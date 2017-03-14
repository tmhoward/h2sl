/**
 * @file    search_space.h
 *
 * @section DESCRIPTION
 *
 * A class to encapsulate the Grounding Search Space. 
 *  
 */

#ifndef H2SL_SEARCH_SPACE_H
#define H2SL_SEARCH_SPACE_H

#include <iostream>
#include <vector>
#include <map>

#include "h2sl/grounding.h"
#include "h2sl/phrase.h"
#include "h2sl/llm.h"
#include "h2sl/cv.h"
#include "h2sl/symbol_dictionary.h"

namespace h2sl {
  class Search_Space {
  public:

    Search_Space( const std::map< std::string, std::pair< std::string, std::vector< Grounding* > > >& groundingPairs = 
                        std::map< std::string, std::pair< std::string, std::vector< Grounding* > > >(),
    		  const std::map< std::string, std::vector< unsigned int > >& cvs = 
                        std::map< std::string, std::vector< unsigned int> >() );

    Search_Space( xmlNodePtr root );
    virtual ~Search_Space();
    Search_Space( const Search_Space& other );
    Search_Space& operator=( const Search_Space& other );
    bool operator==( const Search_Space& other )const;
    bool operator!=( const Search_Space& other )const;
    virtual Search_Space* dup( void )const;

    virtual void fill_groundings( const Symbol_Dictionary& symbolDictionary, const World* world );
    virtual void fill_rules( const Symbol_Dictionary& symbolDictionary, const World* world );
    virtual void clear( void );
    virtual void scrape_examples( const std::string& filename, const Phrase* phrase, const h2sl::World* world, std::vector< std::pair< unsigned int, h2sl::LLM_X > >& examples );

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    inline std::map< std::string, std::pair< std::string, std::vector< Grounding* > > >& grounding_pairs( void ) { return _grounding_pairs; };
    inline const std::map< std::string, std::pair< std::string, std::vector< Grounding* > > >& grounding_pairs( void )const{ return _grounding_pairs; };

    inline std::map< std::string, std::vector< unsigned int > >& cvs( void) { return _cvs; };
    inline const std::map< std::string, std::vector< unsigned int > >& cvs( void )const{ return _cvs; };

    static std::string class_name( void ){ return "search_space"; };

  protected:

    std::map< std::string, std::pair< std::string, std::vector< Grounding* > > > _grounding_pairs;
    std::map< std::string, std::vector< unsigned int > > _cvs;
  
  private:

  };
  std::ostream& operator<<( std::ostream& out, const Search_Space& other );
}

#endif /* H2SL_SEARCH_SPACE_H */
