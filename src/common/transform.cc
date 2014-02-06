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

string
Transform::
to_std_string( void )const{
  stringstream tmp;
  return tmp.str();  
}

void
Transform::
from_std_string( const string& arg ){
  _orientation = Unit_Quaternion();
  _position = Vector3();
  vector< string > data_strings;
  boost::split( data_strings, arg, boost::is_any_of( "," ) );
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Transform& other ) {
    out << "orientation:(" << other.orientation() << ") ";
    out << "position" << other.position();
    return out;
  }
}
