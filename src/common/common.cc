/**
 * @file common.cc
 * @author  Thomas M. Howard (thomas.howard@rochester.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
 *
 * Copyright (C) 2015 by the University of Rochester
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
 * @brief
 *
 * a class to represent a common
 */

#include <cmath>
#include <glib.h>

#include "h2sl/common.h"

using namespace std;

namespace h2sl {
  /** 
   * returns the current time
   */
  int64_t
  current_time( void ){
    GTimeVal tmp;
    g_get_current_time( &tmp );
    return ( int64_t )( tmp.tv_sec * 1000000 + tmp.tv_usec ); 
  }

  /**
   * bounds the value within a particular range
   */
  double
  bound( const double& value,
          const pair< double, double >& limits ){
    if( value < limits.first ){
      cout << value << " < " << limits.first << endl;
      return limits.first; 
    } else if ( value > limits.second ){
      cout << value << " > " << limits.second << endl;
      return limits.second;
    } else {
      return value;
    } 
  }

  /** 
   * radians to degrees
   */
  double
  radians_to_degrees( const double& radians ){
    return radians * 180.0 / M_PI; 
  }

  /**  
   * degrees to radians
   */
  double 
  degrees_to_radians( const double& degrees ){
    return degrees * M_PI / 180.0;
  }

  /**
   * interpolates a value
   */
  int
  interpolate( const double& value,
                const double& minValue,
                const double& maxValue,
                const int& minIndex,
                const int& maxIndex ){
    return ( int )( round( ( double )( maxIndex - minIndex ) * ( value - minValue ) / ( maxValue - minValue ) + minIndex ) );
  }

  /**
   * interpolates a value
   */
  double
  interpolate( const int& valueIndex,
                const double& minValue,
                const double& maxValue,
                const int& minIndex,
                const int& maxIndex ){
    return ( maxValue - minValue ) * ( double )( valueIndex - minIndex ) / ( double )( maxIndex - minIndex ) + minValue;
  }

  /**
   * time difference in microseconds
   */
  int64_t
  diff_time( const int64_t& start, const int64_t& end ){
    return ( end - start );
  }

  /**
   * time difference in microseconds
   */
  unsigned int 
  diff_time( const struct timeval& start, const struct timeval& end ){
    return ( end.tv_sec * 1000000 + end.tv_usec ) - ( start.tv_sec * 1000000 + start.tv_usec );
  }

  /**
   * converts microseconds to seconds
   */
  double 
  microseconds_to_seconds( const unsigned int& microseconds ){
    return microseconds / 1000000.0;
  } 
}
