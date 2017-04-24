/**
 * @file    object.h
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
 * The interface for a class used to describe an object
 */

#ifndef H2SL_OBJECT_H
#define H2SL_OBJECT_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/common.h"
#include "h2sl/grounding.h"
#include "h2sl/transform.h"
#include "h2sl/symbol_dictionary.h"

namespace h2sl {
  class Object : public Grounding {
  public:
    Object( const std::string& id = "na", const std::string& objectType = "na", const std::string& objectColor = "na", 
            const Transform& transform = Transform(), const Vector3& linearVelocity = Vector3(), 
            const Vector3& angularVelocity = Vector3() );
    Object( xmlNodePtr root, World* world );
    virtual ~Object();
    Object( const Object& other );
    Object& operator=( const Object& other );
    bool operator==( const Object& other )const;
    bool operator!=( const Object& other )const;
    virtual Object* dup( void )const;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "object" ); };
    virtual void scrape_grounding( const World * world, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    virtual void scrape_grounding( const World * world, std::vector< std::string >& classNames, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    static void fill_search_space( const Symbol_Dictionary& symbolDictionary, const World* world, std::map< std::string, std::pair< std::string, std::vector< Grounding* > > >& searchSpaces, const std::string& symbolType );
    virtual void fill_rules( const World* world, Grounding_Set* groundingSet )const;
    virtual bool equals( const Grounding& other )const;

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( const std::string& filename, const bool& writeAllProperties )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root, const bool& writeAllProperties )const;

    virtual std::string to_latex( void )const;

    virtual void from_xml( const std::string& filename, World* world );
    virtual void from_xml( xmlNodePtr root, World* world );

    inline std::string& id( void ){ return get_prop< std::string >( _string_properties, "id" ); };
    inline const std::string& id( void )const{ return get_prop< std::string >( _string_properties, "id" ); };
    inline std::string& type( void ){ return get_prop< std::string >( _string_properties, "object_type" ); };
    inline const std::string& type( void )const{ return get_prop< std::string >( _string_properties, "object_type" ); };
    inline std::string& color( void ){ return get_prop< std::string >( _string_properties, "object_color" ); };
    inline const std::string& color( void )const{ return get_prop< std::string >( _string_properties, "object_color" ); };
    inline Transform& transform( void ){ return _transform; };
    inline const Transform& transform( void )const{ return _transform; };
    inline Vector3& linear_velocity( void ){ return _linear_velocity; };
    inline const Vector3& linear_velocity( void )const{ return _linear_velocity; };
    inline Vector3& angular_velocity( void ){ return _angular_velocity; };
    inline const Vector3& angular_velocity( void )const{ return _angular_velocity; };

    static std::string class_name( void ){ return "object"; };
  
  protected:
    Transform _transform;
    Vector3 _linear_velocity;
    Vector3 _angular_velocity;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Object& other );

  std::ostream& operator<<( std::ostream& out, const std::vector< Object* >& other );
}

#endif /* H2SL_OBJECT_H */
