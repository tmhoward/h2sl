/**
 * @file common.h
 * @author H2SL Developers
 *
 * @version 2.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
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
 */

#ifndef H2SL_COMMON_H
#define H2SL_COMMON_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <tinyxml2.h>

#include <boost/algorithm/string.hpp>

#include <ros/ros.h>
#include <std_msgs/String.h>

namespace h2sl {
  /**
   * Returns the sign of the given double (see Siciliano pg. 55)
   */
  inline double sign( const double& x ){
    if( x < 0.0 ){
      return -1.0;
    }
    return 1.0;
  }

  /**
   * converts unsigned int to std::string
   */
  inline std::string to_std_string( const unsigned int& arg, const unsigned int& precision = 4 ){
    std::stringstream tmp;
    tmp << arg;
    return tmp.str();
  }

  /**
   * converts double to std::string
   */
  inline std::string to_std_string( const double& arg, const unsigned int& precision = 4 ){
    std::stringstream tmp;
    tmp << std::setprecision( precision ) << arg;
    return tmp.str();
  }

  /**
   * converts three doubles to a std::string
   */
  inline std::string to_std_string( const double& val1Arg, const double& val2Arg, const double& val3Arg, const unsigned int& precision = 4 ){
    std::stringstream tmp;
    tmp << std::setprecision( precision ) << val1Arg << " " << val2Arg << " " << val3Arg;
    return tmp.str();
  }

  /**
   * converts four doubles to a std::string
   */
  inline std::string to_std_string( const double& val1Arg, const double& val2Arg, const double& val3Arg, const double& val4Arg, const unsigned int& precision = 4 ){
    std::stringstream tmp;
    tmp << std::setprecision( precision ) << val1Arg << " " << val2Arg << " " << val3Arg << " " << val4Arg;
    return tmp.str();
  }

  /**
   * converts c_str to double
   */
  inline unsigned int unsigned_int_from_std_string( const char* stringArg ){
    return strtol( stringArg, NULL, 10 );
  }

  /**
   * converts c_str to double
   */
  inline double from_std_string( const char* stringArg ){
    return strtod( stringArg, NULL );
  }

  /**
   * converts string to double
   */
  inline double from_std_sring( const std::string& stringArg ){
    return strtod( stringArg.c_str(), NULL );
  }

  /**
  * Method to convert an std::set of std::string to csv std::string
  */
  inline std::string std_string_set_to_std_string( const std::set<std::string>& set ){
    std::string out = "";
    for( auto it = set.cbegin(); it != set.cend(); ++it ){
      out += *it;
      if( std::next(it) != set.cend() )
        out += ",";
    }
    return out;
  }

  /**
  * Method to convert an std::unordered_set of std::string to a comma-separated std::string
  */
  inline std::string std_string_u_set_to_std_string(
                            const std::unordered_set<std::string>& u_set )
  {
    std::string out = "";
    for( auto it = u_set.cbegin(); it != u_set.cend(); ++it ){
      out += *it;
      if( std::next(it) != u_set.cend() )
        out += ",";
    }
    return out;
  }

  /**
  * Method to convert and std::pair of std::string to a csv std::string
  */
  inline std::string std_string_pair_to_std_string( const std::pair<std::string,std::string>& pair ){
    std::string out = "";
    out += pair.first;
    out += ",";
    out += pair.second;
    return out;
  }

  /**
  * Method to convert an std::vector of std::string to a comma-separated std::string
  */
  inline std::string std_string_vector_to_std_string( const std::vector<std::string>& vec ){
    std::string out = "";
    for( auto it = vec.cbegin(); it != vec.cend(); ++it ){
      out += *it;
      if( std::next(it) != vec.cend() )
        out +=",";
    }
    return out;
  }

  /**
  Method to convert a comma-separated std::string to an std::set of std::string
  */
  inline std::set<std::string> std_string_set_from_std_string( const std::string& str ){
    std::set<std::string> set;
    std::stringstream ss(str);
    while( ss.good() ){
      std::string substr;
      std::getline( ss, substr, ',');
      set.insert( substr );
    }
    return set;
  }

  /**
  * Method to convert a comma-separated std::string to an std::unordered_set of std::string
  */
  inline std::unordered_set<std::string> std_string_u_set_from_std_string( const std::string& str ){
    std::unordered_set<std::string> u_set;
    std::stringstream ss(str);
    while( ss.good() ){
      std::string substr;
      std::getline( ss, substr, ',');
      u_set.insert( substr );
    }
    return u_set;
  }

  /**
  * Method to convert a comma-separated std::string to an std::vector of std::string
  */
  inline std::vector<std::string> std_string_vector_from_std_string( const std::string& str ){
    std::vector<std::string> vec;
    std::stringstream ss(str);
    while( ss.good() ){
      std::string substr;
      std::getline( ss, substr, ',');
      vec.push_back( substr );
    }
    return vec;
  }

  /**
  * Method to convert a comma-separated std::string to an std::vector of double
  */
  inline std::vector<double> double_vector_from_std_string( const std::string& str ){
    std::vector<double> vec;
    std::stringstream ss(str);
    while( ss.good() ){
      std::string substr;
      std::getline( ss, substr, ',');
      vec.push_back( strtod( substr.c_str(), NULL ) );
    }
    return vec;
  }

  /**
  * Method to convert a comma-separated std::string to an std::pair of std::string
  */
  inline std::pair< std::string, std::string > std_string_pair_from_std_string( const std::string& str ){
    std::pair< std::string, std::string > pair;
    std::stringstream ss(str);

    // Extract first string
    std::string substr;
    std::getline( ss, substr, ',');
    pair.first = substr;

    // Extract second string
    substr.clear();
    std::getline( ss, substr, ',');
    pair.second = substr;
    return pair;
  }

  /**
  * Method to sort two strings in alphabetial order
  */
  inline bool std_string_alphabetical_sort( const std::string& a, const std::string& b ){
    int index = 0;
    while( true ){
      if( a.size() == index ){
        return true;
      } else if ( b.size() == index ){
        return false;
      }
      if( a[index] > b[index] ){
        return false;
      } else if ( a[index] < b[index] ){
        return true;
      } else {
        index++;
      }
    }
  }

  /**
   * Method to load channels from xml file
  */
  inline std::unordered_map<std::string, std::string> channels_from_xml(const char* filename) {
    // Initialize an error_msg for exception handling
    std::stringstream error_msg;
    // Initialize the return object
    std::unordered_map<std::string, std::string> channels;
    // Load the file into an XMLDocument object
    tinyxml2::XMLDocument doc;
    doc.LoadFile( filename );
    if( doc.Error() ){
      error_msg << "Document error during LoadFile for \"" << filename << "\" with doc.Error(): " << doc.Error();
      throw std::runtime_error( error_msg.str() );
    }
    // Extract the root element
    const auto* root = doc.RootElement();
    if( root == nullptr ){
      error_msg << "Document error during RootElement for \"" << filename << "\"";
      throw std::runtime_error( error_msg.str() );
    }
    // Look for a channels child element
    const auto* channels_element = root->FirstChildElement("channels");
    if( channels_element == nullptr ){
      error_msg << "No channels element found as a child of the root element for \"" << filename << "\"";
      throw std::runtime_error( error_msg.str() );
    }
    // Look for channel child elements
    auto* channel_element = channels_element->FirstChildElement("channel");
    if( channel_element == nullptr ){
      error_msg << "No channel elements found as a child of the channels element for \"" << filename << "\"";
      throw std::runtime_error( error_msg.str() );
    }

    // Extract each channel element into the channels map
    while( channel_element != nullptr ){
      // Get the channel key
      const auto* channels_key_attribute = channel_element->FindAttribute("name");
      if( channels_key_attribute == nullptr )
        throw std::runtime_error("No \"name\" attribute found for a channel element");
      const auto* channels_value_attribute = channel_element->FindAttribute("value");
      if( channels_value_attribute == nullptr )
        throw std::runtime_error("No \"value\" attribute found for a channel element");

      // Add to the channels map
      channels.emplace( channels_key_attribute->Value(), channels_value_attribute->Value() );
      // Move to the next channel element
      channel_element = channel_element->NextSiblingElement("channel");
    }
    return channels;
  }


  ///
  /// Struct to help setup the ROS node objects (publishers, subscribers, service clients & servers)
  ///
  template <typename C>
  struct ROS_Helper{
    // The ROS node handler object for which the ROS objects are created
    ros::NodeHandle& node_handle;
    // The class template object to store the ROS objects
    C& c_ros;
    // A Map of channel names for node connections
    const std::unordered_map< std::string, std::string >& channels;

    // Method to create a ROS publisher, default of a string message type
    template <typename T = std_msgs::String>
    void create_publisher( const std::string& channel_name ){
      // Look for a channel that matches the provided channel name
      auto const it_channel = channels.find( channel_name );
      // Create a publisher and store it in the class object
      if( it_channel != channels.cend() ){
        c_ros.publishers().emplace( channel_name, node_handle.advertise<T>( it_channel->second, 1 ) );
      } else{
        std::stringstream error_msg;
        error_msg << "No matching channel name found for requested publisher on \"" << channel_name << "\"";
        throw std::runtime_error( error_msg.str() );
      }
    }

    // Method to create a ROS subscriber
    template <typename T>
    void create_subscriber( const std::string& channel_name, void (C::*fp)(T) ){
      // Look for a channel that matches the provided channel name
      auto const it_channel = channels.find( channel_name );
      if( it_channel != channels.cend() ){
        c_ros.subscribers().emplace( channel_name, node_handle.subscribe( it_channel->second, 1, fp, &c_ros ) );
      } else{
        std::stringstream error_msg;
        error_msg << "No matching channel name found for requested subscriber on \"" << channel_name << "\"";
        throw std::runtime_error( error_msg.str() );
      }
    }

    // Method to create a ROS Service Server
    template <typename T, typename R>
    void create_service_server( const std::string& channel_name, bool (C::*fp)(T,R) ){
      // Look for a channel that matches the provided channel name
      auto const it_channel = channels.find( channel_name );
      if( it_channel != channels.cend() ){
        c_ros.service_servers().emplace( channel_name, node_handle.advertiseService( it_channel->second, fp, &c_ros ) );
      } else{
        std::stringstream error_msg;
        error_msg << "No matching channel name found for requested server on \"" << channel_name << "\"";
        throw std::runtime_error( error_msg.str() );
      }
    }

    // Method to create a ROS Service Client, default of a string message type
    template <typename T = std_msgs::String>
    void create_service_client( const std::string& channel_name ){
      // Look for a channel that matches the provded channel name
      auto const it_channel = channels.find( channel_name );
      if( it_channel != channels.cend() ){
        c_ros.service_clients().emplace( channel_name, node_handle.serviceClient<T>( it_channel->second ) );
      } else{
        std::stringstream error_msg;
        error_msg << "No matching channel name found for requested client on \"" << channel_name << "\"";
        throw std::runtime_error( error_msg.str() );
      }
    }
  }; // struct ROS_Helper


  /**
   * Method to convert a file and line to a string
   */
  inline std::string log_error( const char* file, const int line ){
    std::stringstream preamble;
    preamble << "[" << file << ":" << std::to_string( line ) << "] ";
    return preamble.str();
  }

  /**
   * Method to print a vector of shared_ptr< std::string > values
   */
  inline std::ostream& operator<<( std::ostream& out, const std::vector< std::shared_ptr< std::string > >& other ){
    out << "[" << other.size() << "]:{";
    for( auto it_other = other.cbegin(); it_other != other.cend(); ++it_other ){
      out << *it_other;
      if( std::next( it_other ) != other.cend() ){
        out << ",";
      }
    }
    out << "}";
    return out;
  }

  /**
   * Method to print a map of std::vector< unsigned int > values
   */
  inline std::ostream& operator<<( std::ostream& out, const std::map< std::string, std::vector< unsigned int > >& other ){
    out << "[" << other.size() << "]:{";
    for( auto it_other = other.cbegin(); it_other != other.cend(); ++it_other ){
      out << it_other->first << "[" << it_other->second.size() << "]:{";
      for( auto it_other_elem = it_other->second.cbegin(); it_other_elem != it_other->second.cend(); ++it_other_elem ){
        out << *it_other_elem;
        if( std::next( it_other_elem ) != it_other->second.cend() ){
          out << ",";
        }
      }
      out << "}";
      if( std::next( it_other ) != other.cend() ){
        out << ",";
      }
    }
    out << "}";
    return out;
  }
}

#endif /* H2SL_COMMON_H */
