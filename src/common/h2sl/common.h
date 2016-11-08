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

#include <assert.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <libxml/tree.h>

#ifndef H2SL_COMMON_H
#define H2SL_COMMON_H

namespace h2sl {
  /**
   * returns the difference in time
   */
  inline double
  diff_time( struct timeval& startTime,
              struct timeval& endTime ){
    if( endTime.tv_usec < startTime.tv_usec ){
      return ( ( double )( endTime.tv_sec - startTime.tv_sec - 1 ) + ( double )( 1000000 + endTime.tv_usec - startTime.tv_usec ) / 1000000.0 );
    } else {
      return ( ( double )( endTime.tv_sec - startTime.tv_sec ) + ( double )( endTime.tv_usec - startTime.tv_usec ) / 1000000.0 );
    }
  }

  /** 
    * returns the cartesian power of a set of vectors 
    */
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

  /**
   * checks if the xmlNodePtr has a property with a particular name
   */
  template< typename C >
  inline std::pair< bool, C >
  has_prop( const xmlNodePtr& node, const std::string& string ){
    std::pair< bool, std::string > ret( false, "" );
    if( node->type == XML_ELEMENT_NODE ){
      xmlChar* tmp = xmlGetProp( node, ( const xmlChar* )( string.c_str() ) );
      if( tmp != NULL ){
        ret.first = true;
        ret.second = ( C )( tmp );
        xmlFree( tmp );
      }
    }
    return ret;
  }

  /** 
   * checks if the name of the xmlNodePtr matches a string
   */
  inline bool
  matches_name( const xmlNodePtr& node, const std::string& string ){
    if( node->type == XML_ELEMENT_NODE ){
      if( xmlStrcmp( node->name, ( const xmlChar* )( string.c_str() ) ) == 0 ){
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }

  /**
   * checks if the xmlNodePtr has a property with a particular name
   */
  template<>
  inline std::pair< bool, std::string >
  has_prop< std::string >( const xmlNodePtr& node, const std::string& string ){
    std::pair< bool, std::string > ret( false, "" );
    if( node->type == XML_ELEMENT_NODE ){
      xmlChar* tmp = xmlGetProp( node, ( const xmlChar* )( string.c_str() ) );
      if( tmp != NULL ){
        ret.first = true;
        ret.second = ( char* )( tmp );
        xmlFree( tmp );
      }
    }
    return ret;
  }

  /**
   * safely inserts a property 
   */
  template< typename C >
  inline void
  insert_prop( std::map< std::string, C >& properties, const std::string& key, const C& item ){
    typename std::map< std::string, C >::iterator it = properties.find( key );
    if( it == properties.end() ){
      properties.insert( std::pair< std::string, C >( key, item ) );
    } else {
      it->second = item;
    }
    return;
  }

  /** 
   * safely get reference to property
   */
  template< typename C >
  inline C&
  get_prop( std::map< std::string, C >& properties, const std::string& key ){
    typename std::map< std::string, C >::iterator it = properties.find( key );
    if( it == properties.end() ) {
      std::cout << "could not find property \"" << key << "\"" << std::endl;
    }
    assert( it != properties.end() );
    return it->second;
  }

  /** 
   * safely get const reference to property
   */
  template< typename C >
  inline const C&
  get_prop( const std::map< std::string, C >& properties, const std::string& key ){
    typename std::map< std::string, C >::const_iterator it = properties.find( key );
    if( it == properties.end() ) {
      std::cout << "could not find property \"" << key << "\"" << std::endl;
    }
    assert( it != properties.end() );
    return it->second;
  }

  template< typename C >
  inline std::string
  map_to_std_string( const std::map< std::string, C >& map ){
    std::stringstream tmp;
    for( typename std::map< std::string, C >::const_iterator it = map.begin(); it != map.end(); it++ ){
      tmp << "(" << it->first << "," << it->second << ")";
      if( next( it ) != map.end() ){
        tmp << ",";
      }
    }
    return tmp.str();
  }

  /**
   * safely deletes a pointer of type C
   */
  template< typename C >
  inline void delete_ptr( C*& arg ){
    if( arg != NULL ){
      delete arg;
      arg = NULL;
    }
    return;
  };

  /**
   * safely deletes the map pointers of type std::string and C
   */
  template< typename C >
  inline void delete_vector_ptrs( std::vector< C* >& vector ){
    for( typename std::vector< C* >::iterator it = vector.begin(); it != vector.end(); it++ ){
      delete_ptr< C >( *it );
    }
    return;
  }

  /**
   * safely deletes the map pointers of type std::string and C
   */
  template< typename C >
  inline void delete_map_ptrs( std::map< std::string, C* >& map ){
    for( typename std::map< std::string, C* >::iterator it = map.begin(); it != map.end(); it++ ){
      delete_ptr< C >( it->second );
    }
    return;
  }


}

#endif /* H2SL_COMMON_H */
