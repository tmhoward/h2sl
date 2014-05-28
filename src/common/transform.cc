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

#include <sstream>
#include <cmath>
#include <boost/algorithm/string.hpp>

#include "h2sl/transform.h"

using namespace std;
using namespace h2sl;

Transform::
Transform( const Unit_Quaternion& orientation,
          const Vector3& position ) : _orientation( orientation ),
                                      _position( position ) {

}

Transform::
~Transform() {

}

Transform::
Transform( const Transform& other ) : _orientation( other._orientation ),
                                      _position( other._position ){

}

Transform&
Transform::
operator=( const Transform& other ) {
  _orientation = other._orientation;
  _position = other._position;
  return (*this);
}

Transform&
Transform::
operator*=( const Transform& rhs ){
  _position += _orientation * rhs.position();
  _orientation *= rhs.orientation();
  return (*this);
}

Transform
Transform::
operator*( const Transform& rhs )const {
  return Transform( _orientation * rhs.orientation(), _position + _orientation * rhs.position() );
}

Transform
Transform::
inverse( void )const{
  return Transform( _orientation.conjugate(), _orientation.conjugate() * _position * -1.0 );
}

string
Transform::
to_std_string( void )const{
  stringstream tmp;
  tmp << _orientation.qv().x() << "," << _orientation.qv().y() << "," << _orientation.qv().z() << "," << _orientation.qs() << "," << _position.x() << "," << _position.y() << "," << _position.z();
  return tmp.str();  
}

void
Transform::
from_std_string( const string& arg ){
  _orientation = Unit_Quaternion();
  _position = Vector3();
  vector< string > data_strings;
  boost::split( data_strings, arg, boost::is_any_of( "," ) );
  if( data_strings.size() == 7 ){
    _orientation = Unit_Quaternion(  Vector3( strtof( data_strings[ 0 ].c_str(), NULL ),
                                              strtof( data_strings[ 1 ].c_str(), NULL ),
                                              strtof( data_strings[ 2 ].c_str(), NULL ) ),
                                      strtof( data_strings[ 3 ].c_str(), NULL ) );
    _position = Vector3( strtof( data_strings[ 4 ].c_str(), NULL ),
                          strtof( data_strings[ 5 ].c_str(), NULL ),
                          strtof( data_strings[ 6 ].c_str(), NULL ) );
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Transform& other ) {
    out << "Transform(";
    out << "orientation=" << other.orientation() << ",";
    out << "position=" << other.position();
    out << ")";
    return out;
  }
}
