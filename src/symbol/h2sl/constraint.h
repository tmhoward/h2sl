/**
 * @file    constraint.h
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
 * 02110-1301, USA. *
 * @section DESCRIPTION
 *
 * The interface for a class used to represent a constraint
 */

#ifndef H2SL_CONSTRAINT_H
#define H2SL_CONSTRAINT_H

#include <iostream>

#include "h2sl/grounding.h"
#include "h2sl/region.h"

namespace h2sl {
  class Constraint: public Grounding {
  public:
    Constraint( const std::string& constraintType = "na", const std::string& payload = "na", const std::string& reference = "na", const std::string& referenceRelation = "na" );
    Constraint( const std::string& constraintType,  const std::string& payload, const std::string& payloadRelation, const std::string& reference, const std::string& referenceRelation);
    Constraint( xmlNodePtr root, World* world );
    virtual ~Constraint();
    Constraint( const Constraint& other );
    Constraint& operator=( const Constraint& other );
    bool operator==( const Constraint& other )const;
    bool operator!=( const Constraint& other )const;
    virtual Grounding* dup( void )const;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const; 
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "constraint" ); };
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

    inline std::string& constraint_type( void ){ return get_prop< std::string >( _string_properties, "constraint_type" ); };
    inline const std::string& constraint_type( void )const{ return get_prop< std::string >( _string_properties, "constraint_type" ); };
    inline std::string& payload( void ){ return get_prop< std::string >( _string_properties, "payload" ); };
    inline const std::string& payload( void )const{ return get_prop< std::string >( _string_properties, "payload" ); };
    inline std::string& payload_relation( void ){ return get_prop< std::string >( _string_properties, "payload_relation" ); };
    inline const std::string& payload_relation( void )const{ return get_prop< std::string >( _string_properties, "payload_relation" ); };
    inline std::string& reference( void ){ return get_prop< std::string >( _string_properties, "reference" ); };
    inline const std::string& reference( void )const{ return get_prop< std::string >( _string_properties, "reference" ); };
    inline std::string& reference_relation( void ){ return get_prop< std::string >( _string_properties, "reference_relation" ); };
    inline const std::string& reference_relation( void )const{ return get_prop< std::string >( _string_properties, "reference_relation" ); };

    static std::string class_name( void ){ return "constraint"; };

  protected:

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Constraint& other );
}

#endif /* H2SL_CONSTRAINT_H */
