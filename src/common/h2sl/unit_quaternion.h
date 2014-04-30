/**
 * @file    unit_quaternion.h
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
 * The interface for a class used to describe a three-dimensional vector
 */

#ifndef H2SL_UNIT_QUATERNION_H
#define H2SL_UNIT_QUATERNION_H

#include <iostream>
#include <vector>

#include "h2sl/vector3.h"

namespace h2sl {
  class Unit_Quaternion {
  public:
    Unit_Quaternion( const Vector3& qv = Vector3(), const double& qs = 1.0 );
    Unit_Quaternion( const double& roll, const double& pitch, const double& yaw );
    virtual ~Unit_Quaternion();
    Unit_Quaternion( const Unit_Quaternion& other );
    Unit_Quaternion& operator=( const Unit_Quaternion& other );
    Unit_Quaternion& operator*=( const Unit_Quaternion& rhs );
    Unit_Quaternion& operator*=( const double& rhs );
    Unit_Quaternion operator*( const Unit_Quaternion& rhs )const;
    Vector3 operator*( const Vector3& rhs )const;
    Unit_Quaternion operator*( const double& rhs )const;

    void normalize( void );

    double norm( void )const;
    Unit_Quaternion conjugate( void )const;

    std::string to_std_string( void )const;
    void from_std_string( const std::string& arg );
  
    void from_axis_angle( const Vector3& arg ); 
    void from_axis_angle( const Vector3& axis, const double& angle ); 
    void from_rpy( const double& roll, const double& pitch, const double& yaw ); 

    inline Vector3& qv( void ){ return _qv; };
    inline const Vector3& qv( void )const{ return _qv; };
    inline double& qs( void ){ return _qs; };
    inline const double& qs( void )const{ return _qs; };

  protected:
    Vector3 _qv;
    double _qs;


  private:

  };
  std::ostream& operator<<( std::ostream& out, const Unit_Quaternion& other );
}

#endif /* H2SL_UNIT_QUATERNION_H */
