/**
 * @file    grounding.h
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
 * The interface for a class used to describe a grounding
 */

#ifndef H2SL_GROUNDING_H
#define H2SL_GROUNDING_H

#include <iostream>
#include <vector>
#include <map>
#include <libxml/tree.h>

namespace h2sl {
  class World;

  class Grounding_Set;

  class Grounding {
  public:
    Grounding( const std::map< std::string, std::string >& stringProperties = std::map< std::string, std::string >(), const std::map< std::string, int >& intProperties = std::map< std::string, int >() );
    virtual ~Grounding();
    Grounding( const Grounding& other );
    Grounding& operator=( const Grounding& other );
    bool operator==( const Grounding& other )const;
    bool operator!=( const Grounding& other )const;
    virtual Grounding* dup( void )const = 0;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const = 0;
    virtual bool matches_class_name( const std::string& arg )const = 0;
    virtual void scrape_grounding( const World* world, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const = 0;
    virtual void scrape_grounding( const World* world, std::vector< std::string >& classNames, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const = 0;
    virtual void fill_rules( const World* world, Grounding_Set* groundingSet )const = 0;

    virtual bool equals( const Grounding& other )const = 0;

    virtual void to_xml( const std::string& filename )const = 0;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const = 0;
    virtual std::string to_latex( void )const = 0;

    virtual void from_xml( const std::string& filename, World* world ) = 0;
    virtual void from_xml( xmlNodePtr root, World* world ) = 0;

    inline std::map< std::string, std::string >& string_properties( void ){ return _string_properties; };
    inline const std::map< std::string, std::string >& string_properties( void )const{ return _string_properties; };
    inline std::map< std::string, int >& int_properties( void ){ return _int_properties; };
    inline const std::map< std::string, int >& int_properties( void )const{ return _int_properties; };

  protected:
    std::map< std::string, std::string > _string_properties;
    std::map< std::string, int > _int_properties;  

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Grounding& other );
  
  std::ostream& operator<<( std::ostream& out, const std::vector< Grounding* >& other );
}

#endif /* H2SL_GROUNDING_H */
