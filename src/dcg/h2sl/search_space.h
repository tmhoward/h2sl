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
#include "h2sl/cv.h"
#include "h2sl/symbol_dictionary.h"

namespace h2sl {
  class Search_Space {
  public:

    Search_Space( const std::map< std::string, std::pair< unsigned int, std::vector< Grounding* > > >& searchSpaces = 
                        std::map< std::string, std::pair< unsigned int, std::vector< Grounding* > > >(),
    		  const std::vector< std::vector< unsigned int > >& cvs = 
                        std::vector< std::vector< unsigned int> >(),
                  const Symbol_Dictionary& symbolDictionary = Symbol_Dictionary() );

    Search_Space( xmlNodePtr root );
    virtual ~Search_Space();
    Search_Space( const Search_Space& other );
    Search_Space& operator=( const Search_Space& other );
    bool operator==( const Search_Space& other )const;
    bool operator!=( const Search_Space& other )const;
    virtual Search_Space* dup( void )const;

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    inline std::map< std::string, std::pair< unsigned int, std::vector< Grounding* > > >& search_spaces( void ) { return _search_spaces; };
    inline const std::map< std::string, std::pair< unsigned int, std::vector< Grounding* > > >& search_spaces( void )const{ return _search_spaces; };

    inline std::vector< std::vector< unsigned int > >& cvs( void) { return _cvs; };
    inline const std::vector< std::vector< unsigned int > >& cvs( void)const{ return _cvs; };

    inline Symbol_Dictionary& symbol_dictionary( void ) { return _symbol_dictionary; };
    inline const Symbol_Dictionary& symbol_dictionary( void )const{ return _symbol_dictionary; };

    static std::string class_name( void ){ return "search_space"; };

  protected:

    std::map< std::string, std::pair< unsigned int, std::vector< Grounding* > > > _search_spaces;
    std::vector< std::vector< unsigned int > > _cvs;
    Symbol_Dictionary _symbol_dictionary;
  
  private:

  };
  std::ostream& operator<<( std::ostream& out, const Search_Space& other );
}

#endif /* H2SL_SEARCH_SPACE_H */
