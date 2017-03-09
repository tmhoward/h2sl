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

namespace h2sl {
  class Search_Space {
  public:
    Search_Space();
    virtual ~Search_Space();
    Search_Space( const Search_Space& other );
    Search_space& operator=( const Search_Space& other );

  protected:

    std::map< std::string, std::pair< unsigned int, std::vector< Grounding* > > > _search_spaces;
//    std::vector< std::pair< unsigned int, Grounding* > > _search_spaces;
    std::vector< std::vector< unsigned int > > _correspondence_variables;
    Symbol_Dictionary _symbol_dictionary;
    std::vector< std::pair< double, Phrase* > > _solutions;
    Factor_Set * _root;
  
  private:

  };
  std::ostream& operator<<( std::ostream& out, const Search_Space& other );
}

#endif /* H2SL_SEARCH_SPACE_H */
