/**
 * @file    transform.h
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
 * The interface for a class used to describe a three-dimensional transform
 */

#ifndef H2SL_TRANSFORM_H
#define H2SL_TRANSFORM_H

#include <iostream>
#include <vector>

#include "h2sl/unit_quaternion.h"
#include "h2sl/vector3.h"

namespace h2sl {
  class Transform {
  public:
    Transform( const Unit_Quaternion& orientation = Unit_Quaternion(), const Vector3& position = Vector3() );
    virtual ~Transform();
    Transform( const Transform& other );
    Transform& operator=( const Transform& other );
    Transform& operator*=( const Transform& other );
    Transform operator*( const Transform& rhs )const;
    
    Transform inverse( void )const;

    // string representation: 7 numbers separated by comma, first 4 for orientation, next 3 for position
    std::string to_std_string( void )const;
    void from_std_string( const std::string& arg );
  
    inline Unit_Quaternion& orientation( void ){ return _orientation; };
    inline const Unit_Quaternion& orientation( void )const{ return _orientation; };
    inline Vector3& position( void ){ return _position; };
    inline const Vector3& position( void )const{ return _position; };

  protected:
    Unit_Quaternion _orientation;
    Vector3 _position;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Transform& other );
}

#endif /* H2SL_TRANSFORM_H */
