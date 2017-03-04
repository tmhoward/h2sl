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

#include <sstream>
#include <cmath>
#include <boost/algorithm/string.hpp>

#include "h2sl/vector3.h"

using namespace std;
using namespace h2sl;

Vector3::
Vector3( const double& x,
          const double& y,
          const double& z ) : _data( 3, 0.0 ) {
  _data[ 0 ] = x;
  _data[ 1 ] = y;
  _data[ 2 ] = z;
}

Vector3::
~Vector3() {

}

Vector3::
Vector3( const Vector3& other ) : _data( other._data ){

}

double&
Vector3::
operator[]( const unsigned int& index ){
  return _data[ index ];
}

const double&
Vector3::
operator[]( const unsigned int& index )const{
  return _data[ index ];
}

Vector3&
Vector3::
operator=( const Vector3& other ) {
  _data = other._data;
  return (*this);
}

Vector3&
Vector3::
operator+=( const Vector3& rhs ){
  for( unsigned int i = 0; i < _data.size(); i++ ){
    _data[ i ] += rhs._data[ i ];
  }
  return (*this);
}

Vector3&
Vector3::
operator*=( const double& rhs ){
  for( unsigned int i = 0; i < _data.size(); i++ ){
    _data[ i ] *= rhs;
  }
  return (*this );
} 

Vector3&
Vector3::
operator/=( const double& rhs ){
  for( unsigned int i = 0; i < _data.size(); i++ ){
    _data[ i ] /= rhs;
  }
  return (*this );
} 

Vector3 
Vector3::
operator+( const Vector3& rhs )const{
  return Vector3( _data[ 0 ] + rhs.x(), _data[ 1 ] + rhs.y(), _data[ 2 ] + rhs.z() );
}

Vector3 
Vector3::
operator-( const Vector3& rhs )const{
  return Vector3( _data[ 0 ] - rhs.x(), _data[ 1 ] - rhs.y(), _data[ 2 ] - rhs.z() );
}

Vector3 
Vector3::
operator*( const double rhs )const{
  return Vector3( _data[ 0 ] * rhs, _data[ 1 ] * rhs, _data[ 2 ] * rhs );
}

Vector3
Vector3::
operator/( const double rhs )const{
  return Vector3( _data[ 0 ] / rhs, _data[ 1 ] / rhs, _data[ 2 ] / rhs );
}

double
Vector3::
norm( void )const{
  return sqrt( _data[ 0 ] * _data[ 0 ] + _data[ 1 ] * _data[ 1 ] + _data[ 2 ] * _data[ 2 ] );
}

double
Vector3::
distance( const Vector3& lhs, 
          const Vector3& rhs ){
  double dx = lhs.x() - rhs.x();
  double dy = lhs.y() - rhs.y();
  double dz = lhs.z() - rhs.z();
  return sqrt( dx * dx + dy * dy + dz * dz );
}

double
Vector3::
dot( const Vector3& lhs,
      const Vector3& rhs ){
  return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
}

Vector3
Vector3::
cross( const Vector3& lhs,
        const Vector3& rhs ){
  return Vector3( lhs.y() * rhs.z() - lhs.z() * rhs.y(),
                  lhs.z() * rhs.x() - lhs.x() * rhs.z(),
                  lhs.x() * rhs.y() - lhs.y() * rhs.x() );
}

string
Vector3::
to_std_string( void )const{
  stringstream tmp;
  for( unsigned int i = 0; i < _data.size(); i++ ){
    tmp << _data[ i ];
    if( i != ( _data.size() - 1 ) ){
      tmp << ",";
    }
  }
  return tmp.str();  
}

void
Vector3::
from_std_string( const string& arg ){
  _data[ 0 ] = _data[ 1 ] = _data[ 2 ] = 0.0;
  vector< string > data_strings;
  boost::split( data_strings, arg, boost::is_any_of( "," ) );
  if( data_strings.size() == 3 ){
    for( unsigned int i = 0; i < data_strings.size(); i++ ){
      _data[ i ] = strtof( data_strings[ i ].c_str(), NULL );
    }
  } else if ( data_strings.size() == 1 ) {
    boost::split( data_strings, arg, boost::is_any_of( " " ) );
    if( data_strings.size() == 3 ){
      for( unsigned int i = 0; i < data_strings.size(); i++ ){
        _data[ i ] = strtof( data_strings[ i ].c_str(), NULL );
      }
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Vector3& other ) {
    out << "Vector3(" << other.to_std_string() << ")";
    return out;
  }
}
