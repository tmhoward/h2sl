/**
 * @file    region.h
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
 * The interface for a class used to describe a region of space
 */

#ifndef H2SL_REGION_H
#define H2SL_REGION_H

#include <iostream>

#include "h2sl/grounding.h"
#include "h2sl/object.h"

namespace h2sl {
  class Region: public Grounding {
  public:
    Region( const std::string& spatialRelationType = "na", const std::string& objectId = "na" );
    Region( xmlNodePtr root, World* world );
    virtual ~Region();
    Region( const Region& other );
    Region& operator=( const Region& other );
    bool operator==( const Region& other )const;
    bool operator!=( const Region& other )const;
    virtual Grounding* dup( void )const;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "region" ); };
    virtual void scrape_grounding( const World * world, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    virtual void scrape_grounding( const World * world, std::vector< std::string >& classNames, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    static void fill_search_space( const Symbol_Dictionary& symbolDictionary, const World* world, std::map< std::string, std::pair< std::string, std::vector< Grounding* > > >& searchSpaces, const std::string& symbolType );
    virtual void fill_rules( const World* world, Grounding_Set* groundingSet )const;
    virtual bool equals( const Grounding& other )const;

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual std::string to_latex( void )const;

    virtual void from_xml( const std::string& filename, World* world );
    virtual void from_xml( xmlNodePtr root, World* world );

    inline std::string& spatial_relation_type( void ){ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };
    inline const std::string& spatial_relation_type( void )const{ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };
    inline std::string& object_id( void ){ return get_prop< std::string >( _string_properties, "object_id" ); };
    inline const std::string& object_id( void )const{ return get_prop< std::string >( _string_properties, "object_id" ); };

    static std::string class_name( void ){ return "region"; };

  protected:

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Region& other );
}

#endif /* H2SL_REGION_H */
