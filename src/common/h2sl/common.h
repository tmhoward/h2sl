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
#ifndef H2SL_COMMON_H
#define H2SL_COMMON_H

#include <iostream>
#include <iomanip>
#include <assert.h>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <sys/time.h>
#include <libxml/tree.h>
#include <glib.h>


namespace h2sl {

  /**
   * return the current time
   */
  int64_t current_time( void );

  /**
   * bound a value within a particular range
   */
  double bound( const double& value, const std::pair< double, double >& limits );

  /**
   * radians to degrees
   */
  double radians_to_degrees( const double& radians );

  /**
   * degrees to radians
   */
  double degrees_to_radian( const double& degrees );

  /**
   * bounded random
   */
  inline double bounded_random( const double& min, const double& max, const unsigned int randomMax  = 100000 ){
    return min + ( max - min ) * ( double )( rand() % randomMax ) / ( double )( randomMax );
  }

  /**
   * interpolates a value
   */
  int interpolate( const double& value, const double& minValue, const double& maxValue, const int& minIndex, const int& maxIndex );

  /**
   * interpolate a value
   */
  double interpolate( const int& valueIndex, const double& minValue, const double& maxValue, const int& minIndex, const int& maxIndex );

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
   * time difference in milliseconds
   */ 
  int64_t diff_time( const int64_t& start, const int64_t& end );

  /**
   * time difference in microseconds
   */
  unsigned int diff_time( const struct timeval& start, const struct timeval& end );

  /**
   * converts microseconds to seconds
   */
  double microseconds_to_seconds( const unsigned int& microseconds );

  /** 
    * returns the cartesian power of a set of unsigned int vectors 
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
    * returns the cartesian power of a set of vectors of pairs 
    */
  inline std::vector< std::vector< std::pair< std::string, unsigned int > > >
  cartesian_power( const std::vector< std::vector< std::pair< std::string, unsigned int > > >& vectors, const std::vector< std::vector< std::pair< std::string, unsigned int > > >& prev = std::vector< std::vector< std::pair< std::string, unsigned int > > >(), const unsigned int index = 0 ){
    if( vectors.size() > index ){
      std::vector< std::vector< std::pair< std::string, unsigned int > > > tmp;
      if( prev.empty() ){
        for( unsigned int i = 0; i < vectors[ index ].size(); i++ ){
          tmp.push_back( std::vector< std::pair< std::string, unsigned int > >( 1, vectors[ index ][ i ] ) );
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
   * check keys
   */
  inline bool
  check_keys( const xmlNodePtr& node, const std::vector< std::string >& keys ){
    if( node->type == XML_ELEMENT_NODE ){
      for( xmlAttrPtr l1 = node->properties; l1 != NULL; l1 = l1->next ){
        if( std::find( keys.begin(), keys.end(), ( char* )( l1->name ) ) == keys.end() ){
          std::cout << "key \"" << l1->name << "\" not in list of authorized keys" << std::endl;
          return false;
        }
      }
      return true;
    } else {
      return false;
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
   * checks if the xmlNodePtr has a property with a particular name
   */
  template<>
  inline std::pair< bool, double >
  has_prop< double >( const xmlNodePtr& node, const std::string& string ){
    std::pair< bool, double > ret( false, 0.0 );
    if( node->type == XML_ELEMENT_NODE ){
      xmlChar* tmp = xmlGetProp( node, ( const xmlChar* )( string.c_str() ) );
      if( tmp != NULL ){
        ret.first = true;
        std::string tmp_string = ( char* )( tmp );
        ret.second = strtod( tmp_string.c_str(), NULL );
        xmlFree( tmp );
      }
    }
    return ret;
  }

  /**
   * checks if the xmlNodePtr has a property with a particular name
   */
  template<>
  inline std::pair< bool, unsigned short >
  has_prop< unsigned short >( const xmlNodePtr& node, const std::string& string ){
    std::pair< bool, unsigned short > ret( false, 0 );
    if( node->type == XML_ELEMENT_NODE ){
      xmlChar* tmp = xmlGetProp( node, ( const xmlChar* )( string.c_str() ) );
      if( tmp != NULL ){
        ret.first = true;
        ret.second = ( unsigned short )( strtol( ( char* )( tmp ), NULL, 10 ) );
        xmlFree( tmp );
      }
    }
    return ret;
  }

  /**
   * checks if the xmlNodePtr has a property with a particular name
   */
  template<>
  inline std::pair< bool, unsigned int >
  has_prop< unsigned int >( const xmlNodePtr& node, const std::string& string ){
    std::pair< bool, unsigned int > ret( false, 0 );
    if( node->type == XML_ELEMENT_NODE ){
      xmlChar* tmp = xmlGetProp( node, ( const xmlChar* )( string.c_str() ) );
      if( tmp != NULL ){
        ret.first = true;
        ret.second = strtol( ( char* )( tmp ), NULL, 10 );
        xmlFree( tmp );
      }
    }
    return ret;
  }

  /**
   * checks if the xmlNodePtr has a property with a particular name
   */
  template<>
  inline std::pair< bool, int >
  has_prop< int >( const xmlNodePtr& node, const std::string& string ){
    std::pair< bool, int > ret( false, 0 );
    if( node->type == XML_ELEMENT_NODE ){
      xmlChar* tmp = xmlGetProp( node, ( const xmlChar* )( string.c_str() ) );
      if( tmp != NULL ){
        ret.first = true;
        ret.second = strtol( ( char* )( tmp ), NULL, 10 );
        xmlFree( tmp );
      }
    }
    return ret;
  }
 
  /**
   * checks if the xmlNodePtr has a property with a particular name
   */
  template<>
  inline std::pair< bool, bool >
  has_prop< bool >( const xmlNodePtr& node, const std::string& string ){
    std::pair< bool, bool > ret( false, false );
    if( node->type == XML_ELEMENT_NODE ){
      xmlChar* tmp = xmlGetProp( node, ( const xmlChar* )( string.c_str() ) );
      if( tmp != NULL ){
        ret.first = true;
        ret.second = ( bool )( strtol( ( char* )( tmp ), NULL, 10 ) );
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

  /**
   * converts double to string
   */
  inline std::string to_std_string( const double& arg, const unsigned int& precision = 4 ){
    std::stringstream tmp;
    tmp << std::setprecision( precision ) << arg;
    return tmp.str();
  }

  /** 
   * converts int to string
   */
  inline std::string to_std_string( const int& arg ){
    std::stringstream tmp;
    tmp << arg;
    return tmp.str();
  }

  /** 
   * converts unsigned int to string
   */
  inline std::string to_std_string( const unsigned int& arg ){
    std::stringstream tmp;
    tmp << arg;
    return tmp.str();
  }

  /**
   * std:vector string formatted
   */
  template< typename C >
  inline std::string std_vector_to_std_string( const std::vector< C >& vector, const bool& additionalFormatting = true ){
    std::stringstream tmp;
    if( additionalFormatting ){
      tmp << "[" << vector.size() << "]:{";
    }
    for( typename std::vector< C >::const_iterator it = vector.begin(); it != vector.end(); it++ ){
      tmp << *it;
      if( next( it ) != vector.end() ){
        tmp << ",";
      }
    }
    if( additionalFormatting ){
      tmp << "}";
    }
    return tmp.str();
  };

  /** 
   * std::vector from std::string
   */
  template< class C >
  inline std::vector< C > std_vector_from_std_string( const std::string& arg ){
    std::vector< C > tmp;
    boost::split( tmp, arg, boost::is_any_of( "," ) );
    return tmp;
  };

  template<>
  inline std::vector< int > std_vector_from_std_string( const std::string& arg ){
    std::vector< std::string > tmp_string;
    boost::split( tmp_string, arg, boost::is_any_of( "," ) );
    std::vector< int > tmp( tmp_string.size() );
    for( unsigned int i = 0; i < tmp.size(); i++ ){
      tmp[ i ] = strtol( tmp_string[ i ].c_str(), NULL, 10 );
    }
    return tmp;
  }   

  /**
   * std:vector string formatted
   */
  template< typename C >
  inline std::string std_vector_to_std_string( const std::vector< std::pair< C, bool > >& vector ){
    std::stringstream tmp;
    tmp << "[" << vector.size() << "]:{";
    for( typename std::vector< std::pair< C, bool > >::const_iterator it = vector.begin(); it != vector.end(); it++ ){
      tmp << it->first;
      if( next( it ) != vector.end() ){
        tmp << ",";
      }
    }
    tmp << "}";
    return tmp.str();
  };

  /**
   * std:vector string formatted
   */
  template< typename C >
  inline std::string std_vector_to_std_string( const std::vector< C* >& vector ){
    std::stringstream tmp;
    tmp << "[" << vector.size() << "]:{";
    for( typename std::vector< C* >::const_iterator it = vector.begin(); it != vector.end(); it++ ){
      tmp << *(*it);
      if( next( it ) != vector.end() ){
        tmp << ",";
      }
    }
    tmp << "}";
    return tmp.str();
  };

  /** 
   * std::map string formatted
   */
  template< typename C >
  inline std::string
  std_map_to_std_string( const std::map< std::string, C >& map ){
    std::stringstream tmp;
    tmp << "[" << map.size() << "]:{";
    for( typename std::map< std::string, C >::const_iterator it = map.begin(); it != map.end(); it++ ){
      tmp << "\"" << it->first << "\":\"" << it->second << "\"";
      if( next( it ) != map.end() ){
        tmp << ",";
      }
    }
    tmp << "}";
    return tmp.str();
  };

  /**
   * array string formatted
   */
  template< typename C >
  inline std::string array_to_std_string( const C* array, unsigned int size ){
    std::stringstream tmp;
    tmp << "[" << size << "]:{";
    for( unsigned int i = 0; i < size; i++ ){
      tmp << array[ i ];
      if( i != ( size - 1 ) ){
        tmp << ",";
      }
    }
    tmp << "}";
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

    /**
   * safely creates a new instance of type C
   */
  template< typename C >
  inline void new_ptr( C*& arg ){
    delete_ptr< C >( arg );
    arg = new C();
    return;
  };

  /**
   * reverse lookup in map
   */
  template< typename C >
  inline void reverse_lookup( const std::map< std::string, C* >& map,
                              const C* value,
                              std::string& index ){
    for( typename std::map< std::string, C* >::const_iterator it = map.begin(); it != map.end(); it++ ){
      if( it->second == value ){
        index = it->first;
        return;
      }
    }
    assert( true );
  }

  /**
   * adds a unique label to a vector
   */
  template< class C >
  inline void insert_unique( const C& value,
                              std::vector< C >& types ){
    typename std::vector< C >::const_iterator it = std::find( types.begin(), types.end(), value );
    if( it == types.end() ){
      types.push_back( value );
    }
    return;
  }

  /**
   * adds a unique label to a map
   */
  template< class C >
  inline void insert_unique( const std::string& key, 
                              const C& value,
                              std::map< std::string, std::vector< C > >& types ){
    typename std::map< std::string, std::vector< C > >::iterator it_types = types.find( key );
    if( it_types == types.end() ){
      std::pair< typename std::map< std::string, std::vector< C > >::iterator, bool > types_ret = types.insert( std::pair< std::string, std::vector< C > >( key, std::vector< C >() ) );
      typename std::vector< C >::iterator it_type = find( types_ret.first->second.begin(), types_ret.first->second.end(), value );
      if( it_type == types_ret.first->second.end() ){
        types_ret.first->second.push_back( value );
      }
    } else {
      typename std::vector< C >::iterator it_type = find( it_types->second.begin(), it_types->second.end(), value );
      if( it_type == it_types->second.end() ){
        it_types->second.push_back( value );
      } 
    } 
    return;
  } 

  /** 
   * reads the channels from an xml file
   */
  inline std::map< std::string, std::string > channels_from_xml( const std::string& filename ){
    std::map< std::string, std::string > tmp;
    xmlDoc * doc = xmlReadFile( filename.c_str(), NULL, 0 );
    if( doc != NULL ){
      xmlNodePtr root = xmlDocGetRootElement( doc );
      if( root->type == XML_ELEMENT_NODE ){
        for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
          if( matches_name( l1, "channels" ) ){
            for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
              if( matches_name( l2, "channel" ) ){
                std::pair< bool, std::string > name_prop = has_prop< std::string >( l2, "name" );
                std::pair< bool, std::string > value_prop = has_prop< std::string >( l2, "value" );
                if( name_prop.first && value_prop.first ){
                  tmp.insert( std::pair< std::string, std::string >( name_prop.second, value_prop.second ) );
                }
              }
            }
          }
        }
      }
      xmlFreeDoc( doc );
    }
    return tmp;
  };

  /** 
   * check to see if a symbol is in a vector
   */
  template< class A >
  inline bool has_in_vector( const A& arg, std::vector< A >& args ){
    for( unsigned int i = 0; i < args.size(); i++ ){
      if( arg == args[ i ] ){
        return true;
      }
    }
    return false;
  }  

  /**  
   * find a symbol in a vector of pairs, where the pair consists of a symbol and a vector
   */
  template< class A, class B, class C >
  inline std::vector< const C* > find_in_vector_of_pairs( const std::vector< std::pair< const A*, std::vector< B* > > >& arg ){
    std::vector< const C* > tmp_vector;
    for( unsigned int i = 0; i < arg.size(); i++ ){
      for( unsigned int j = 0; j < arg[ i ].second.size(); j++ ){
        const C* tmp = dynamic_cast< const C* >( arg[ i ].second[ j ] );
        if( tmp != NULL ){
          tmp_vector.push_back( tmp );
        }
      }
    }
    return tmp_vector;
  }

  /**
   * extract an instruction from a xml file
   */
  inline std::string extract_instruction( const std::string& filename ){
    std::string instruction = "";
    xmlDoc * doc = NULL;
    xmlNodePtr root = NULL;
    doc = xmlReadFile( filename.c_str(), NULL, 0 );
    if( doc != NULL ){
      root = xmlDocGetRootElement( doc );
      if( root->type == XML_ELEMENT_NODE ){
        xmlNodePtr l1 = NULL;
        for( l1 = root->children; l1; l1 = l1->next ){
          if( l1->type == XML_ELEMENT_NODE ){
            if( xmlStrcmp( l1->name, ( const xmlChar* )( "instruction" ) ) == 0 ){
              xmlChar * tmp = xmlGetProp( l1, ( const xmlChar* )( "text" ) );
              if( tmp != NULL ){
                instruction = ( char* )( tmp );
                xmlFree( tmp );
              }
            }
          }
        }
      }
      xmlFreeDoc( doc );
    }
    return instruction;
  }

}

#endif /* H2SL_COMMON_H */
