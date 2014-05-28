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

#include <sstream>
#include <cmath>
#include <boost/algorithm/string.hpp>

#include "h2sl/unit_quaternion.h"

using namespace std;
using namespace h2sl;

Unit_Quaternion::
Unit_Quaternion( const Vector3& qv,
                  const double& qs ) : _qv( qv ),
                                        _qs( qs ) {
  normalize();
}

Unit_Quaternion::
Unit_Quaternion( const double& roll,
                  const double& pitch,
                  const double& yaw ) : _qv(),
                                        _qs( 1.0 ) {
  from_rpy( roll, pitch, yaw );
}

Unit_Quaternion::
~Unit_Quaternion() {

}

Unit_Quaternion::
Unit_Quaternion( const Unit_Quaternion& other ) : _qv( other._qv ),
                                                  _qs( other._qs ){

}

Unit_Quaternion&
Unit_Quaternion::
operator=( const Unit_Quaternion& other ) {
  _qv = other._qv;
  _qs = other._qs;
  return (*this);
}

Unit_Quaternion&
Unit_Quaternion::
operator*=( const Unit_Quaternion& rhs ){
  Unit_Quaternion tmp = (*this) * rhs;
  (*this) = tmp;
  return (*this);
}

Unit_Quaternion&
Unit_Quaternion::
operator*=( const double& rhs ){
  _qv *= rhs;  
  _qs *= rhs;
  normalize();
  return (*this);
}

Unit_Quaternion
Unit_Quaternion::
operator*( const Unit_Quaternion& rhs )const{
  return Unit_Quaternion( rhs.qv() * _qs + _qv * rhs.qs() + Vector3::cross( _qv, rhs.qv() ), _qs * rhs.qs() - Vector3::dot( _qv, rhs.qv() ) );
}

Vector3
Unit_Quaternion::
operator*( const Vector3& rhs )const{
  return rhs + Vector3::cross( qv() * 2.0, ( Vector3::cross( qv(), rhs ) + rhs * qs() ) );
}

Unit_Quaternion
Unit_Quaternion::
operator*( const double& rhs )const{
  return Unit_Quaternion( _qv * rhs, _qs * rhs );
}

void
Unit_Quaternion::
normalize( void ){
  double tmp = norm();
  _qv /= tmp;
  _qs /= tmp;
  if( _qs < 0.0 ){
    _qs *= -1.0;
    _qv *= -1.0;
  }
}

double
Unit_Quaternion::
norm( void )const{
  return sqrt( _qv.x() * _qv.x() + _qv.y() * _qv.y() + _qv.z() * _qv.z() + _qs * _qs );
}

Unit_Quaternion
Unit_Quaternion::
conjugate( void )const{
  return Unit_Quaternion( _qv * -1.0, _qs );
}


string
Unit_Quaternion::
to_std_string( void )const{
  stringstream tmp;
  tmp << _qv.x() << "," << _qv.y() << "," << _qv.z() << "," << _qs;
  return tmp.str();  
}

void
Unit_Quaternion::
from_std_string( const string& arg ){
  _qv = Vector3();
  _qs = 1.0;
  vector< string > data_strings;
  boost::split( data_strings, arg, boost::is_any_of( "," ) );
  if( data_strings.size() == 4 ){
    _qv.x() = strtof( data_strings[ 0 ].c_str(), NULL );
    _qv.y() = strtof( data_strings[ 1 ].c_str(), NULL );
    _qv.z() = strtof( data_strings[ 2 ].c_str(), NULL );
    _qs = strtof( data_strings[ 3 ].c_str(), NULL );
    normalize();
  }
  return;
}

void
Unit_Quaternion::
from_axis_angle( const Vector3& arg ){
  Vector3 axis = arg / arg.norm();
  double angle = arg.norm();
  from_axis_angle( axis, angle );
  return;
}

void
Unit_Quaternion::
from_axis_angle( const Vector3& axis,
                  const double& angle ){
  _qs = cos( angle ) / 2.0;
  _qv = axis * sin( angle / 2.0 );
  normalize();
  return;
}

void
Unit_Quaternion::
from_rpy( const double& roll,
          const double& pitch,
          const double& yaw ){
  _qs = cos( roll / 2.0 ) * cos( pitch / 2.0 ) * cos( yaw / 2.0 ) + sin( roll / 2.0 ) * sin ( pitch / 2.0 ) * sin( yaw / 2.0 );
  _qv.x() = sin( roll / 2.0 ) * cos( pitch / 2.0 ) * cos( yaw / 2.0 ) - cos( roll / 2.0 ) * sin( pitch / 2.0 ) * sin( yaw / 2.0 );
  _qv.y() = cos( roll / 2.0 ) * sin( pitch / 2.0 ) * cos( yaw / 2.0 ) + sin( roll / 2.0 ) * cos( pitch / 2.0 ) * sin( yaw / 2.0 );
  _qv.z() = cos( roll / 2.0 ) * cos( pitch / 2.0 ) * sin( yaw / 2.0 ) - sin( roll / 2.0 ) * sin( pitch / 2.0 ) * cos( yaw / 2.0 );
  normalize();
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Unit_Quaternion& other ) {
    out << "Unit_Quaternion(";
    out << "qv=" << other.qv() << ",";
    out << "qs=" << other.qs();
    out << ")";
    return out;
  }
}
