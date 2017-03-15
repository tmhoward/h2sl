/**
 * @file    grounding_set.h
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
 * The interface for a class used to describe a set of groundings
 */

#ifndef H2SL_GROUNDING_SET_H
#define H2SL_GROUNDING_SET_H

#include <iostream>
#include <vector>

#include "h2sl/grounding.h"

namespace h2sl {
  class Grounding_Set: public Grounding {
  public:
    Grounding_Set( const std::vector< Grounding* >& groundings = std::vector< Grounding* >() );
    Grounding_Set( xmlNodePtr root );
    virtual ~Grounding_Set();
    Grounding_Set( const Grounding_Set& other );
    Grounding_Set& operator=( const Grounding_Set& other );
    virtual Grounding_Set* dup( void )const;

    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "grounding_set" ); };
    virtual void scrape_grounding( const World * world, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    virtual void scrape_grounding( const World * world, std::vector< std::string >& classNames, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    virtual void fill_rules( const World* world, Grounding_Set* groundingSet )const;

    void clear( void );
 
    virtual unsigned int evaluate_cv( const h2sl::Grounding* grounding )const;

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    inline std::vector< Grounding* >& groundings( void ){ return _groundings; };
    inline const std::vector< Grounding* >& groundings( void )const{ return _groundings; };

    static std::string class_name( void ){ return "grounding_set"; };

  protected:
    std::vector< Grounding* > _groundings;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Grounding_Set& other );

  template< class C >
  void insert_unique_grounding( Grounding_Set* groundingSet, 
                                C& grounding ){
    std::cout << "checking grounding " << grounding << std::endl;
    if( groundingSet != NULL ){
      bool new_grounding = true;
      for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
        C* tmp = dynamic_cast< C* >( groundingSet->groundings()[ i ] );
        if( tmp != NULL ){
          if( grounding == *tmp ){
            new_grounding = false;
          }
        }
      }
      if( new_grounding ){
        std::cout << "adding grounding " << grounding << " to grounding set" << std::endl;
        groundingSet->groundings().push_back( grounding.dup() );
        std::cout << "groundingSet->groundings().size():" << groundingSet->groundings().size() << std::endl;
      }
    }
    return;
  } 
}

#endif /* H2SL_GROUNDING_SET_H */
