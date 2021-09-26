/**
 * @file factor.h
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

#include <optional>
#include <iostream>
#include <tinyxml2.h>
#include <jsoncpp/json/value.h>

#include "h2sl/symbol.h"
#include "h2sl/language-variable.h"
#include "h2sl/world.h"
#include "h2sl/feature-set.h"
#include "h2sl/feature-pool.h"
#include "h2sl/llm.h"

namespace h2sl {
  /**
   * Factor class definition
   */
  class Factor {
  public:

  /**
    Factor constructor that uses a provided FeatureSet

    @brief Factor class constructor that uses a provided FeatureSet
    @param[in]    symbolArg     Symbol ptr associated with the Factor
    @param[in]    lvArg         LanguageVariable ptr associated with the Factor (shared across a FactorSet)
    @param[in]    worldArg      WorldDCG ptr (shared across all Factors in the same graph)
    @param[in]    cvsArg        CV value ptrs (shared across all Factors in the same graph)
    @param[in]    featureSet    optional FeatureSet to be associated with the Factor
    @returns                    none
    @throws                     no expected throws
  */
  Factor( const std::shared_ptr<Symbol>& symbolArg,
          const std::shared_ptr<LanguageVariable>& lvArg,
          const std::shared_ptr<WorldDCG>& worldArg,
          const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvsArg,
          const FeatureSet& featureSet = FeatureSet() );

  /**
    Factor Class constructor that constructs the feature set from the feature-pool.

    @brief Factor class constructor that constructs the feature set from the feature-pool.
    @param[in]    featurePool   FeaturePool from which to construct the FeatureSet for the Factor
    @param[in]    symbolArg     Symbol ptr associated with the Factor
    @param[in]    lvArg         LanguageVariable ptr associated with the Factor (shared across a FactorSet)
    @param[in]    worldArg      WorldDCG ptr (shared across all Factors in the same graph)
    @param[in]    cvsArg        CV value ptrs (shared across all Factors in the same graph)
    @returns                    none
    @throws                     no expected throws
  */
  Factor( const std::shared_ptr<FeaturePool>& featurePool,
          const std::shared_ptr<Symbol>& symbolArg,
          const std::shared_ptr<LanguageVariable>& lvArg,
          const std::shared_ptr<WorldDCG>& worldArg,
          const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvsArg,
          const bool debug = false );

  /**
    Default copy constructor for h2sl::Factor
  */
  Factor( const Factor& other ) = default;

  /**
    Default move constructor for h2sl::Factor
  */
  Factor( Factor&& other ) = default;

  /**
    Default destructor for h2sl::Factor
  */
  virtual ~Factor() = default;

  /**
    Default assignment operator for h2sl::Factor
  */
  Factor& operator=( const Factor& other ) = default;

  /**
    Default move assignment operator for h2sl::Factor
  */
  Factor& operator=( Factor&& other ) = default;

  /**
    This method evaluates the Factor's FeatureSet using the members of the Factor (cvs, symbol, lv, world)
    and stores the True evaluated features in the Factor's indices member.

    @brief Evaluate the Factor's FeatureSet and update the Factor's indices member to hold indices for True features
    @param[in]      expressedFeatures   A structure to hold information about expressed True features
    @param[in]      debug               An optional flag for more verbose printouts
  **/
  virtual void evaluate( std::map< std::string, std::vector< ExpressedFeature > >& expressedFeatures,
                         const bool& debug = false );

  /**
    This method imports a Factor from a file.

    @brief This method imports a Factor from a file.
    @param[in]    filename  file to be imported from
    @returns                success/failure
    @throws                 no expected throws
  */
  virtual bool from_file( const std::string& filename );

  /**
      This method imports a Factor from an XML file.

      @brief This method imports a Factor from an XML file.
      @param[in]    filename  XML file to be imported from
      @returns                none
      @throws                 no expected throws
  */
  virtual bool from_xml( const std::string& filename );

  /**
      This method imports a Factor from a tinyxml2 element.

      @brief This method imports a Factor from a tinyxml2 element.
      @param[in]    root      tinyxml2 element to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_xml( const tinyxml2::XMLElement* root );

  /**
      This method imports a Factor from a json formatted file.

      @brief This method imports a Factor from a json formatted file.
      @param[in]    root      json formatting file to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const std::string& value );

  /**
      This method imports a Factor from a json formatted string.

      @brief This method imports a Factor from a json formatted string.
      @param[in]    root      json formatting string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& value );

  /**
      This method imports a Factor from a json value.

      @brief This method imports a Factor from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
      This method exports a Factor to a file.

      @brief This method exports a Factor to a file.
      @param[in]    filename  file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_file( const std::string& filename )const;

  /**
      This method exports a Factor to an XML file.

      @brief This method exports a Factor to an XML file.
      @param[in]    filename  XML file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( const std::string& filename )const;

  /**
      This method exports a Factor to a tinyxml2 document.

      @brief This method exports a Factor to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const;

  /**
      This method exports a Factor to a json formatted file.

      @brief This method exports a Factor to a json formatted file.
      @param[in]    filename  filename to be written to in json format
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( const std::string& filename )const;

  /**
      This method exports a Factor to a json formatted string.

      @brief This method exports a Factor to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a Factor to a json value.

      @brief This method exports a Factor to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;
    
  std::shared_ptr< Symbol > symbol;
  std::shared_ptr< LanguageVariable > language_variable;
  std::shared_ptr< WorldDCG > world;
  std::shared_ptr< std::vector< std::shared_ptr< std::string > > > cvs;
  FeatureSet feature_set;


  /**
    Data member to hold the indices for TRUE features for each CV value associated with the Factor.
    Key: the value of the CV
    Value: There are two kinds of indices. The feature indices are the cartesian product of the constituent feature indices.
        feature indices: These are indices for a single tuple of constituent features and provides the index into the weight vector
        constituent feature indices: These are indices for single constituent features
  **/
  std::map< std::string, FeaturePool::indices_t > indices = std::map< std::string, FeaturePool::indices_t >();

  /**
    Optional data member to hold the result of a LLM::pygx() call for using the Factor's indices member.
  **/
  std::optional< LLM::pygx_result_t > pygx_result = std::nullopt;

  protected:

  private:

  };

  /**
      Factor class ostream operator

      @brief Factor class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const Factor& other );
} // namespace h2sl
