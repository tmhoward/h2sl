/**
 * @file expressed-feature.h
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

#pragma once

#include <iostream>
#include <tinyxml2.h>
#include <jsoncpp/json/value.h>

namespace h2sl {
  // Forward declarations for classes of which only a pointer is defined
  class Feature;
  class Symbol;
  class LanguageVariable;

/**
 * ExpressedFeature class definition
 */
class ExpressedFeature {
  public:
  /**
    Default class constructor for ExpressedFeature
  **/
  ExpressedFeature() = default;

  /**
    Parameter constructor for ExpressedFeature class
    ExpressedFeature default class constructor.

    @brief Default ExpressedFeature class constructor.
    @param[in]    weight    The weight associated with the feature
    @param[in]    index     The index into the factor's feature cartesian product vector
    @param[in]    features  The features constituting the tuple in the cartesian product
    @returns                none
    @throws                 no expected throws
  */
  ExpressedFeature( const double& weight, const unsigned int& index, const std::vector< std::shared_ptr< Feature > >& features );

  /**
    Default copy constructor for h2sl::ExpressedFeature
  */
  ExpressedFeature( const ExpressedFeature& other ) = default;

  /**
    Default move constructor for h2sl::ExpressedFeature
  */
  ExpressedFeature( ExpressedFeature&& other ) = default;

  /**
    Default destructor for h2sl::ExpressedFeature
  */
  virtual ~ExpressedFeature() = default;

  /**
    Default assignment operator for h2sl::ExpressedFeature
  */
  ExpressedFeature& operator=( const ExpressedFeature& other ) = default;

  /**
    Default move assignment operator for h2sl::ExpressedFeature
  */
  ExpressedFeature& operator=( ExpressedFeature&& other ) = default;

  /**
    This method imports a ExpressedFeature from a file.

    @brief This method imports a ExpressedFeature from a file.
    @param[in]    filename  file to be imported from
    @returns                success/failure
    @throws                 no expected throws
  */
  virtual bool from_file( const std::string& filename );

  /**
      This method imports a ExpressedFeature from an XML file.

      @brief This method imports a ExpressedFeature from an XML file.
      @param[in]    filename  XML file to be imported from
      @returns                none
      @throws                 no expected throws
  */
  virtual bool from_xml( const std::string& filename );

  /**
      This method imports a ExpressedFeature from a tinyxml2 element.

      @brief This method imports a ExpressedFeature from a tinyxml2 element.
      @param[in]    root      tinyxml2 element to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_xml( const tinyxml2::XMLElement* root );

  /**
      This method imports a ExpressedFeature from a json formatted file.

      @brief This method imports a ExpressedFeature from a json formatted file.
      @param[in]    root      json formatting file to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const std::string& value );

  /**
      This method imports a ExpressedFeature from a json formatted string.

      @brief This method imports a ExpressedFeature from a json formatted string.
      @param[in]    root      json formatting string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& value );

  /**
      This method imports a ExpressedFeature from a json value.

      @brief This method imports a ExpressedFeature from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
      This method exports a ExpressedFeature to a file.

      @brief This method exports a ExpressedFeature to a file.
      @param[in]    filename  file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_file( const std::string& filename )const;

  /**
      This method exports a ExpressedFeature to an XML file.

      @brief This method exports a ExpressedFeature to an XML file.
      @param[in]    filename  XML file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( const std::string& filename )const;

  /**
      This method exports a ExpressedFeature to a tinyxml2 document.

      @brief This method exports a ExpressedFeature to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const;

  /**
      This method exports a ExpressedFeature to a json formatted file.

      @brief This method exports a ExpressedFeature to a json formatted file.
      @param[in]    filename  filename to be written to in json format
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( const std::string& filename )const;

  /**
      This method exports a ExpressedFeature to a json formatted string.

      @brief This method exports a ExpressedFeature to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a ExpressedFeature to a json value.

      @brief This method exports a ExpressedFeature to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  double weight = 0.0;
  unsigned int index = 0;
  std::vector< std::shared_ptr< Feature > > features = std::vector< std::shared_ptr<Feature> >();
  std::map< std::string, unsigned int > jkl_indices = std::map< std::string, unsigned int >();
  std::shared_ptr< h2sl::Symbol > symbol = nullptr;
  std::shared_ptr< h2sl::LanguageVariable > language_variable = nullptr;

  /**
    ExpressedFeature ostream operator. Prints the ExpressedFeature to ostream.

    @brief ExpressedFeature ostream operator
    @param[in]    out     ostream operator class reference
    @param[in]    other   ExpressedFeature reference to be printed to ostream
    @returns              reference to modified ostream object
    @throws               no expected throws
  **/
  // TODO Commented out until actually implemented
  //friend std::ostream& operator<<( std::ostream& out, const ExpressedFeature& other );

  protected:
  private:

  }; // class ExpressedFeature

} // namespace h2sl
