/**
 * @file feature-lv-role-value.h
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

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * FeatureLVRoleValue class definition
   */
  class FeatureLVRoleValue : public Feature {
  public:
  /**
    FeatureLVRoleValue default class constructor.

    @brief Default FeatureLVRoleValue class constructor.
    @returns              none
    @throws               no expected throws
  */
  explicit FeatureLVRoleValue( const std::string& lvTypeArg = "",
    const std::string& roleTypeArg = "", const std::string& roleValueArg = "" );

  /**
    FeatureLVRoleValue class constructor from an XMLElement.

    @brief FeatureLVRoleValue class constructor from an XMLElement.
    @param[in]  root      xml element
    @returns              none
    @throws               no expected throws
  */
  explicit FeatureLVRoleValue( const tinyxml2::XMLElement* root );

  /**
    Default FeatureLVRoleValue class copy constructor.
  */
  FeatureLVRoleValue( const FeatureLVRoleValue& other ) = default;

  /**
    Default FeatureLVRoleValue class move constructor.
  */
  FeatureLVRoleValue( FeatureLVRoleValue&& other ) = default;

  /**
    Default FeatureLVRoleValue class destructor.
  */
  virtual ~FeatureLVRoleValue() = default;

  /**
    Default FeatureLVRoleValue class copy assignment operator.
  */
  FeatureLVRoleValue& operator=( const FeatureLVRoleValue& other ) = default;

  /**
    Default FeatureLVRoleValue class move assignment operator.
  */
  FeatureLVRoleValue& operator=( FeatureLVRoleValue&& other ) = default;

  /**
      This method generates a copy of a feature

      @brief This method generates a copy of a feature
      @returns      std::shared_ptr<Feature>    a copy of a feature
      @throws                                   no expected throws
  */
  virtual std::shared_ptr<Feature> dup( void )const;
 
  /**
      This method evaluates a feature

      @brief This method evaluates a feature
      @param[in]    cv        correspondence variable
      @param[in]    lv        language variable
      @param[in]    world     WorldDCG that includes the sorted object information for world dependent features
      @param[in]    symbol    symbol
      @returns      bool      true/false expression of feature
      @throws                 no expected throws
  */ 
  virtual FeatureValue evaluate( const std::shared_ptr<std::string>& cvArg, 
                                  const std::shared_ptr<LanguageVariable>& lvArg,
                                  const std::shared_ptr<WorldDCG>& worldArg,
                                  const std::shared_ptr<Symbol>& symbolArg ) override;

  /**
      This method prints out the values of a feature

      @brief This method prints out the values of a feature
      @returns      string    value of a feature      
      @throws                 no expected throws
  */
  virtual std::string print_string( const bool& printValue = false )const override;

  /**
    Method to generate a unique key for the feature

    @brief Generate a unique key for the feature
    @returns                  std::string with unique key for feature
    @throws                   no expected throws
  */
  virtual std::string key( void ) const override;

  /**
      This method imports a FeatureLVRoleValue from a tinyxml2 element.

      @brief This method imports a FeatureLVRoleValue from a tinyxml2 element.
      @param[in]    root      tinyxml2 element to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_xml( const tinyxml2::XMLElement* root );

  /**
      This method imports a FeatureLVRoleValue from a json formatted string.

      @brief This method imports a FeatureLVRoleValue from a json formatted string.
      @param[in]    root      json formatted string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& valueArg );

  /**
      This method imports a FeatureLVRoleValue from a json value.

      @brief This method imports a FeatureLVRoleValue from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
      This method exports a FeatureLVRoleValue to a tinyxml2 document.

      @brief This method exports a FeatureLVRoleValue to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const;

  /**
      This method exports a FeatureLVRoleValue to a json formatted string.

      @brief This method exports a FeatureLVRoleValue to a json formatted string.
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a FeatureLVRoleValue to a json value.

      @brief This method exports a FeatureLVRoleValue to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  std::string lv_type;
  std::string role_type;
  std::string role_value;
  
  protected:
  private:

  };

  /**
      FeatureLVRoleValue class ostream operator

      @brief FeatureLVRoleValue class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const FeatureLVRoleValue& other );
} // namespace h2sl