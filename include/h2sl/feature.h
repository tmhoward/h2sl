/**
 * @file feature.h
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
#include "h2sl/environment-constraint-map.h"
#include "h2sl/language-constraint.h"

namespace h2sl {
  /**
   forward declare the symbol class
   */
  class Symbol;

  enum class FeatureType { CorrespondenceVariable, Language, StaticSymbol, DynamicSymbol };

  /**
      FeatureType class ostream operator

      @brief FeatureType class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const FeatureType& other );

  enum class FeatureValue { Unknown, False, True };

  /**
      FeatureValue class ostream operator

      @brief FeatureValue class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const FeatureValue& other );

  /**
   * Feature class definition
   */
  class Feature {
  public:

  struct dependent_parameters_t{
    dependent_parameters_t( bool cvArg, bool languageArg, bool symbolArg, bool childrenArg, bool worldArg ) :
      cv( cvArg ), language( languageArg ), symbol( symbolArg ), children( childrenArg ), world( worldArg ) {}
    dependent_parameters_t( ) noexcept {}
    bool cv = false;
    bool language = false;
    bool symbol = false;
    bool children = false;
    bool world = false;
  };


  /**
    Feature default class constructor.

    @brief Default Feature class constructor.
    @returns              none
    @throws               no expected throws
  */
  Feature( const FeatureType& typeArg = FeatureType::DynamicSymbol,
           const FeatureValue& valueArg = FeatureValue::Unknown,
           const dependent_parameters_t& dependsOnArg = dependent_parameters_t() );

  /**
    Default Feature class copy constructor.
  */
  Feature( const Feature& other ) = default;

  /**
    Default Feature class move constructor.
  */
  Feature( Feature&& other ) = default;

  /**
    Feature default class destructor.
  */
  virtual ~Feature() = default;

  /**
    Default Feature class copy assignment operator.
  */
  Feature& operator=( const Feature& other ) = default;

  /**
    Default Feature class move assignment operator.
  **/
  Feature& operator=( Feature&& other ) = default;

  /**
      This method generates a copy of a feature

      @brief This method generates a copy of a feature
      @returns      std::shared_ptr<Feature>    a copy of a feature
      @throws                                   no expected throws
  */
  virtual std::shared_ptr<Feature> dup( void )const = 0;
 
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
  virtual FeatureValue evaluate( const std::shared_ptr< std::string >& cvArg,
                                 const std::shared_ptr<LanguageVariable>& lvArg,
                                 const std::shared_ptr<WorldDCG>& worldArg,
                                 const std::shared_ptr<Symbol>& symbolArg ) = 0;

  /**
      This method prints out the values of a feature

      @brief This method prints out the values of a feature
      @param[in]    out       ostream
      @returns      none      
      @throws                 no expected throws
  */
  virtual void print( std::ostream& out )const;

  /**
    This method generates an EnvironmentConstraintMap according to an object
    symbol, a world, and the feature's relevant internal content. The returned object
    is a vector of these generated constraints (empty if the feature does not have any
    constraints on the environment). It is expected that this function will be
    overriden by derived classes.

    @brief Generate an EnvironmentConstraint for an instance of this feature
    @param[in]    object_symbol     Symbol of type object for the feature instance
    @param[in]    world             World context from which to generate the constraint
    @returns                        An EnvironmentConstraintMap
    @throws                         Throws runtime error if assumptions fail
  **/
  virtual EnvironmentConstraintMap
  generate_environment_constraints( const std::shared_ptr<Symbol>& object_symbol,
                                   const std::shared_ptr<World>& world )const;
  /**
    This method generates a set of LanguageConstraints according to a language variable
    and the feature's relevant internal content. The returned object is a vector of these
    LanguageConstraints (empty if the feature does not have any constraints on the
    language). It is expected that this function will be overriden by derived classes.

    @brief Generate an LanguageConstraint for an instance of this feature
    @param[in]    language_variable     Language context for the constraint
    @returns                            A vector of language constraints
    @throws                             Throws runtime error if assumptions fail
  **/
  virtual std::vector<LanguageConstraint>
  generate_language_constraints( const std::shared_ptr<LanguageVariable>& language_variable )const;

  /**
      This method prints out the values of a feature

      @brief This method prints out the values of a feature
      @returns      string    value of a feature      
      @throws                 no expected throws
  */
  virtual std::string print_string( const bool& printValue = false )const = 0;

  /**
    Method to generate a unique key for the feature

    @brief Generate a unique key for the feature
    @returns                  std::string with unique key for feature
    @throws                   no expected throws
  */
  virtual std::string key( void ) const = 0;

  /**
      This method imports a Feature from a tinyxml2 element.

      @brief This method imports a Feature from a tinyxml2 element.
      @param[in]    root      tinyxml2 element to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_xml( const tinyxml2::XMLElement* root ) = 0;

  /**
      This method imports a Feature from a json formatted string.

      @brief This method imports a Feature from a json formatted string.
      @param[in]    root      json formatting string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& valueArg ) = 0;

  /**
      This method imports a Feature from a json value.

      @brief This method imports a Feature from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root ) = 0;

  /**
      This method exports a Feature to a tinyxml2 document.

      @brief This method exports a Feature to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const = 0;

  /**
      This method exports a Feature to a json formatted string.

      @brief This method exports a Feature to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const = 0;

  /**
      This method exports a Feature to a json value.

      @brief This method exports a Feature to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const = 0;

  FeatureType type;
  FeatureValue value;
  dependent_parameters_t depends_on;

  protected:

  private:

  };

  /**
      Feature class ostream operator

      @brief Feature class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const Feature& other );
} // namespace h2sl
