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

namespace h2sl {
  class Object : public Grounding {
  public:
    Object( const std::string& name = "na", const std::string& objectType = "na", const Transform& transform = Transform(), const Vector3& linearVelocity = Vector3(), const Vector3& angularVelocity = Vector3() );
    Object( xmlNodePtr root );
    virtual ~Object();
    Object( const Object& other );
    Object& operator=( const Object& other );
    bool operator==( const Object& other )const;
    bool operator!=( const Object& other )const;
    virtual Object* dup( void )const;

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    inline std::string& name( void ){ return get_prop< std::string >( _properties, "name" ); };
    inline const std::string& name( void )const{ return get_prop< std::string >( _properties, "name" ); };
    inline std::string& object_type( void ){ return get_prop< std::string >( _properties, "object_type" ); };
    inline const std::string& object_type( void )const{ return get_prop< std::string >( _properties, "object_type" ); };
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
}

#endif /* H2SL_OBJECT_H */
