/**
 * @file    common.h
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
 * The implementation of a class used for the graphical user interface for h2sl
 */

#include <iostream>
#include <vector>

#ifndef H2SL_COMMON_H
#define H2SL_COMMON_H

namespace h2sl {
  inline double
  diff_time( struct timeval& startTime,
              struct timeval& endTime ){
    if( endTime.tv_usec < startTime.tv_usec ){
      return ( ( double )( endTime.tv_sec - startTime.tv_sec - 1 ) + ( double )( 1000000 + endTime.tv_usec - startTime.tv_usec ) / 1000000.0 );
    } else {
      return ( ( double )( endTime.tv_sec - startTime.tv_sec ) + ( double )( endTime.tv_usec - startTime.tv_usec ) / 1000000.0 );
    }
  }

  inline std::vector< std::vector< unsigned int > >
  cartesian_power( const std::vector< std::vector< unsigned int > >& vectors, const std::vector< std::vector< unsigned int > >& prev = std::vector< std::vector< unsigned int > >(), const unsigned int index = 0 ){
    if( vectors.size() > index ){
      std::vector< std::vector< unsigned int > > tmp;
      if( prev.empty() ){
        for( unsigned int i = 0; i < vectors[ index ].size(); i++ ){
          tmp.push_back( std::vector< unsigned int >( 1, vectors[ index ][ i ] ) );
        }
      } else {
        for( unsigned int i = 0; i < prev.size(); i++ ){
          for( unsigned int j = 0; j < vectors[ index ].size(); j++ ){
            tmp.push_back( prev[ i ] );
            tmp.back().push_back( vectors[ index ][ j ] );
          }
        }
      }
      tmp = cartesian_power( vectors, tmp, index + 1 ); 
      return tmp;
    } else {
      return prev;
    }
  }
}

#endif /* H2SL_COMMON_H */
