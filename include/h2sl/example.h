/**
 * @file example.h
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

#include "h2sl/language-variable.h"
#include "h2sl/world.h"
#include "h2sl/symbol.h"
#include "h2sl/feature-pool.h"

namespace h2sl {
  /**
   * Example class definition
   */
  class Example {
  public:
  /**
      Example default class constructor.

      @brief Default Example class constructor.
      @returns              none
      @throws               no expected throws
  */
  Example( const std::string& filenameArg,
            const std::string& instructionArg,
            const std::shared_ptr<std::string>& cvArg,
            const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvsArg,
            const std::shared_ptr<h2sl::LanguageVariable>& lvArg,
            const std::shared_ptr<h2sl::World>& worldArg,
            const std::shared_ptr<h2sl::Symbol>& symbolArg );

  /**
    Default copy constructor for h2sl::Example
  */
  Example( const Example& other ) = default;

  /**
    Default move constructor for h2sl::Example
  */
  Example( Example&& other ) = default;

  /**
    Default destructor for h2sl::Example
  */
  virtual ~Example() = default;

  /**
    Default assignment operator for h2sl::Example
  */
  Example& operator=( const Example& other ) = default;

  /**
    Default move assignment operator for h2sl::Example
  */
  Example& operator=( Example&& other ) = default;

  /**
    This method imports a Example from a file.

    @brief This method imports a Example from a file.
    @param[in]    filename  file to be imported from
    @returns                success/failure
    @throws                 no expected throws
  */
  virtual bool from_file( const std::string& filename );

  /**
      This method imports a Example from an XML file.

      @brief This method imports a Example from an XML file.
      @param[in]    filename  XML file to be imported from
      @returns                none
      @throws                 no expected throws
  */
  virtual bool from_xml( const std::string& filename );

  /**
      This method imports a Example from a tinyxml2 element.

      @brief This method imports a Example from a tinyxml2 element.
      @param[in]    root      tinyxml2 element to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_xml( const tinyxml2::XMLElement* root );

  /**
      This method imports a Example from a json formatted file.

      @brief This method imports a Example from a json formatted file.
      @param[in]    root      json formatting file to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const std::string& value );

  /**
      This method imports a Example from a json formatted string.

      @brief This method imports a Example from a json formatted string.
      @param[in]    root      json formatting string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& value );

  /**
      This method imports a Example from a json value.

      @brief This method imports a Example from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
      This method exports a Example to a file.

      @brief This method exports a Example to a file.
      @param[in]    filename  file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_file( const std::string& filename )const;

  /**
      This method exports a Example to an XML file.

      @brief This method exports a Example to an XML file.
      @param[in]    filename  XML file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( const std::string& filename )const;

  /**
      This method exports a Example to a tinyxml2 document.

      @brief This method exports a Example to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const;

  /**
      This method exports a Example to a json formatted file.

      @brief This method exports a Example to a json formatted file.
      @param[in]    filename  filename to be written to in json format
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( const std::string& filename )const;

  /**
      This method exports a Example to a json formatted string.

      @brief This method exports a Example to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a Example to a json value.

      @brief This method exports a Example to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  std::string filename;
  std::string instruction;
  std::shared_ptr<std::string> cv;
  std::shared_ptr<std::vector< std::shared_ptr<std::string > > > cvs;
  std::shared_ptr<h2sl::LanguageVariable> lv;
  std::shared_ptr<h2sl::WorldDCG> world;
  std::shared_ptr<h2sl::Symbol> symbol;

  /**
    Data member to hold the indices for TRUE features for each CV value associated with the Example.
    Key: the value of the CV
    Value: There are two kinds of indices. The feature indices are the cartesian product of the constituent feature indices.
        feature_indices: These are indices for a single tuple of constituent features and provides the index into the feature weight vector
        constituent feature indices: These are indices for single constituent features
  **/
  std::map< std::string, FeaturePool::indices_t > indices = std::map< std::string, FeaturePool::indices_t >();

  protected:

  private:

  };

  /**
      Example class ostream operator

      @brief Example class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const Example& other );
} // namespace h2sl
