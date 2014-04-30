/**
 * @file    vector3.h
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

#ifndef H2SL_VECTOR3_H
#define H2SL_VECTOR3_H

#include <iostream>
#include <vector>

namespace h2sl {
  class Vector3 {
  public:
    Vector3( const double& x = 0.0, const double& y = 0.0, const double& z = 0.0 );
    virtual ~Vector3();
    Vector3( const Vector3& other );
    double& operator[]( const unsigned int& index );
    const double& operator[]( const unsigned int& index )const;
    Vector3& operator=( const Vector3& other );
    Vector3& operator+=( const Vector3& rhs );
    Vector3& operator*=( const double& rhs );
    Vector3& operator/=( const double& rhs );
    Vector3 operator+( const Vector3& rhs )const;
    Vector3 operator-( const Vector3& rhs )const;
    Vector3 operator*( const double rhs )const;
    Vector3 operator/( const double rhs )const;

    double norm( void )const;

    static double distance( const Vector3& lhs, const Vector3& rhs );
    static double dot( const Vector3& lhs, const Vector3& rhs );
    static Vector3 cross( const Vector3& lhs, const Vector3& rhs );

    std::string to_std_string( void )const;
    void from_std_string( const std::string& arg );
  
    inline const std::vector< double >& data( void )const{ return _data; };
    inline double& x( void ){ return _data[ 0 ]; };
    inline const double& x( void )const{ return _data[ 0 ]; };
    inline double& y( void ){ return _data[ 1 ]; };
    inline const double& y( void )const{ return _data[ 1 ]; };
    inline double& z( void ){ return _data[ 2 ]; };
    inline const double& z( void )const{ return _data[ 2 ]; };

  protected:
    std::vector< double > _data;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Vector3& other );
}

#endif /* H2SL_VECTOR3_H */
