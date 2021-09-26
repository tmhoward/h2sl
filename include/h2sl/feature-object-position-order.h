/**
 * @file feature-object-position-order.h
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
#include "h2sl/environment-constraint.h"

namespace h2sl {
  /**
   * FeatureObjectPositionOrder class definition
   */
  class FeatureObjectPositionOrder : public Feature {
  public:
  /**
      FeatureObjectPositionOrder argument class constructor.

      @brief Default FeatureObjectPositionOrder class constructor.
      @param[in]  sortKeyArg    Key that determines the evaluate comparison
      @returns                  none
      @throws                   no expected throws
  */
  FeatureObjectPositionOrder( const World::SortKey& sortKeyArg, const int offset );

  /**
      FeatureObjectPositionOrder class constructor from an XMLElement.

      @brief FeatureObjectPositionOrder class constructor from an XMLElement.
      @param[in]  root      XML element from which to construct this feature
      @returns              none
      @throws               no expected throws
  */
  explicit FeatureObjectPositionOrder( const tinyxml2::XMLElement* root );

  /**
      FeatureObjectPositionOrder default class copy constructor.
  */
  FeatureObjectPositionOrder( const FeatureObjectPositionOrder& other ) = default;

  /**
      FeatureObjectPositionOrder default class move constructor.
  */
  FeatureObjectPositionOrder( FeatureObjectPositionOrder&& other ) = default;

  /**
      FeatureObjectPositionOrder default class destructor.
  */
  virtual ~FeatureObjectPositionOrder() = default;

  /**
      FeatureObjectPositionOrder default copy assignment operator.
  */
  FeatureObjectPositionOrder& operator=( const FeatureObjectPositionOrder& other ) = default;

  /**
      FeatureObjectPositionOrder default move assignment operator.
  */
  FeatureObjectPositionOrder& operator=( FeatureObjectPositionOrder&& other ) = default;

  /**
      This method generates a copy of a feature

      @brief This method generates a copy of a feature
      @returns      std::shared_ptr<Feature>    a copy of a feature
      @throws                                   no expected throws
  */
  virtual std::shared_ptr<Feature> dup( void )const;

 // std::shared_ptr<FeatureObjectPositionOrder>
 // shared_from_this(){ return std::static_pointer_cast<FeatureObjectPositionOrder>( Feature::shared_from_this() ); }
 
  /**
      This method evaluates a feature

      @brief This method evaluates a feature
      @param[in]    cvArg        correspondence variable
      @param[in]    lvArg        language variable
      @param[in]    worldArg     WorldDCG that includes the sorted object information for world dependent features
      @param[in]    symbol       symbol
      @returns      value        true/false expression of feature
      @throws                    Throws runtime error if the sort key does not match one in the world
  */ 
  virtual FeatureValue evaluate( const std::shared_ptr< std::string >& cvArg,
                                 const std::shared_ptr<LanguageVariable>& lvArg,
                                 const std::shared_ptr<WorldDCG>& worldArg,
                                 const std::shared_ptr<Symbol>& symbolArg ) override;

  /**
    This method generates a set of EnvironmentConstraints according to an
    object symbol, a world, and the feature's order_key value. The returned object
    is a vector containing these constraints.

    @brief Generate an EnvironmentConstraintMap for an instance of this feature
    @param[in]    object_symbol   Symbol of type object for the feature instance
    @param[in]    world           World context from which to generate the constraint
    @returns                      An EnvironmentConstraintMap
    @throws                       Throws runtime error if assumptions fail
  **/
  virtual EnvironmentConstraintMap
  generate_environment_constraints( const std::shared_ptr<Symbol>& object_symbol,
                                   const std::shared_ptr<World>& world )const override;

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
      This method imports a FeatureObjectPositionOrder from a tinyxml2 element.

      @brief This method imports a FeatureObjectPositionOrder from a tinyxml2 element.
      @param[in]    root      tinyxml2 element to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_xml( const tinyxml2::XMLElement* root );

  /**
      This method imports a FeatureObjectPositionOrder from a json formatted string.

      @brief This method imports a FeatureObjectPositionOrder from a json formatted string.
      @param[in]    root      json formatting string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& valueArg );

  /**
      This method imports a FeatureObjectPositionOrder from a json value.

      @brief This method imports a FeatureObjectPositionOrder from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
      This method exports a FeatureObjectPositionOrder to a tinyxml2 document.

      @brief This method exports a FeatureObjectPositionOrder to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const;

  /**
      This method exports a FeatureObjectPositionOrder to a json formatted string.

      @brief This method exports a FeatureObjectPositionOrder to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a FeatureObjectPositionOrder to a json value.

      @brief This method exports a FeatureObjectPositionOrder to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;


  /*
    The data member to hold the key that determines which sort and order comparison to use during evaluate(). This is an enumerated type of World::SortKey.
  */
  World::SortKey sort_key;

  /*
    The data member to hold the offset from the extrema of the sort_key, e.g. offset = 1 for MIN_X_AXIS checks if the object symbol is
    at index begin() + 1 for that sort_key.
  */
  int offset;
  protected:
  private:

  };

  /**
      FeatureObjectPositionOrder class ostream operator

      @brief FeatureObjectPositionOrder class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const FeatureObjectPositionOrder& other );
} // namespace h2sl
