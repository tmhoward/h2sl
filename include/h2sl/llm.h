/**
 * @file llm.h
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
#include "h2sl/world.h"
#include "h2sl/dialogue.h"
#include "h2sl/feature-pool.h"
#include "h2sl/feature-set.h"

namespace h2sl {
  /**
   * LLM class definition
   */
  class LLM {
  public:

  /**
    Structure to contain the result of a pygx computation and the components used during computation.
    prob: the resulting probability of the computation
    numerator: the numerator value (exponentiated sum of weights for the Y provided CV value)
    denominator: the denominator value (sum of exponentiated sum of weights for each possible CV value)
    sum_of_weights: the sum of feature weights for each CV value (key: CV value, value: the sum of the weights)
  **/
  struct pygx_result_t{
    double prob = 0.0;
    double numerator = 0.0;
    double denominator = 0.0;
    std::optional<std::map< std::string, double >> sum_of_weights = std::nullopt;
  };

  /**
    Default constructor for the LLM class
  **/
  LLM() = default;

  /**
      LLM parameter class constructor from a vector of weights

      @brief Construct an LLM from a vector of weights
      @param[in]    weights   vector of weights
      @returns                none
      @throws                 no expected throws
  */
  explicit LLM( const std::vector< double >& weights );

  /**
      LLM parameter class constructor from a FeaturePool

      @brief Construct an LLM from a FeaturePool
      @param[in]    featurePoolArg   FeaturePool from which to construct an LLM
      @returns                        none
      @throws                         no expected throws
  */
  explicit LLM( const std::shared_ptr< FeaturePool >& featurePoolArg );

  /**
    Default copy constructor for h2sl::LLM
  */
  LLM( const LLM& other ) = default;

  /**
    Default move constructor for h2sl::LLM
  */
  LLM( LLM&& other ) = default;

  /**
    Default destructor for h2sl::LLM
  */
  virtual ~LLM() = default;

  /**
    Default assignment operator for h2sl::LLM
  */
  LLM& operator=( const LLM& other ) = default;

  /**
    Default move assignment operator for h2sl::LLM
  */
  LLM& operator=( LLM&& other ) = default;

  /**
    LLM probabilty of y (correspondence variable) given x (symbol, world, dialogue). The provided indices indicate the feature weights
    for features evaluated to be true for the x parameters.

    @brief Compute the approximate conditional probability for a provided y (cv) using the provided feature indices.
    @param[in]    cv        The CV value for which the function provides a probability
    @param[in]    indices   The set of weight indices for features evaluated to be True
    @returns                A structure containing the numerator, denominator, sum of weights per CV, and the probability
    @throws                 Propogates an out-of-range error if a feature index is outside the weights vector
  **/
  virtual const pygx_result_t
  pygx( const std::shared_ptr< std::string >& cv,
        const std::map< std::string, FeaturePool::indices_t >& indices,
        const bool store_sums = false )const;

  /**
  **/
/*
  virtual double pygx( std::shared_ptr<FeaturePool>& featureSpace,
                       std::shared_ptr< std::string >& cv,
                       const std::map< std::string, std::shared_ptr< std::string > >& cvs,
                       const std::shared_ptr<Symbol>& symbol,
                       const std::shared_ptr<LanguageVariable>& lv,
                       const std::shared_ptr<World>& world,
                       const bool& debug = false )const;
*/
  /**
      LLM probability of y (correspondence variable) given x (symbol, world, dialogue).

      @brief Calculate probability of y (correspondence variable) given x (symbol, world, dialogue)
      @param[in]  cv        correspondence variable
      @param[in]  cvs       space of correspondence variables
      @param[in]  symbol    symbol context
      @param[in]  world     world model context
      @param[in]  dialogue  language input
      @returns              probability of cv given world model and language
      @throws               no expected throws
  */
/*
  virtual void pygxs( std::shared_ptr<FeaturePool>& featureSpace,
                      std::vector< std::pair< std::shared_ptr<std::string>, double > >& cvs,
                      const std::shared_ptr<Symbol>& symbol,
                      const std::shared_ptr<LanguageVariable>& lv,
                      const std::shared_ptr<World>& world )const;
*/

  /**
    Method to compute the sum of feature weights for a vector of vector index values

    @brief Compute the sum of feature weights
    @param[in]    indices   Vector of vector index values
    @return                 The sum of feature weights
    @throws                 Throws an out-of-range error if an index goes beyond the weight vector size
  **/
  virtual double sum_of_feature_weights( const std::vector< size_t >& indices )const;

  /**
    Method to compute the sum of feature weights for a vector of feature_indices_t

    @brief Compute the sum of feature weights
    @param[in]    feature_indices   Vector of feature_indices_t
    @return                         The sum of feature weights
    @throws                         Throws an out-of-range error if an index goes beyond the weight vector size
  **/
  virtual double sum_of_feature_weights( const FeaturePool::vecFeatureIndices& feature_indices )const;

  /**
    This method imports a LLM from a file.

    @brief This method imports a LLM from a file.
    @param[in]    filename  file to be imported from
    @returns                success/failure
    @throws                 no expected throws
  */
  virtual bool from_file( const std::string& filename );

  /**
      This method imports a LLM from an XML file.

      @brief This method imports a LLM from an XML file.
      @param[in]    filename  XML file to be imported from
      @returns                none
      @throws                 no expected throws
  */
  virtual bool from_xml( const std::string& filename );

  /**
      This method imports a LLM from a tinyxml2 element.

      @brief This method imports a LLM from a tinyxml2 element.
      @param[in]    root      tinyxml2 element to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_xml( const tinyxml2::XMLElement* root );

  /**
      This method imports a LLM from a json formatted file.

      @brief This method imports a LLM from a json formatted file.
      @param[in]    root      json formatting file to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const std::string& value );

  /**
      This method imports a LLM from a json formatted string.

      @brief This method imports a LLM from a json formatted string.
      @param[in]    root      json formatting string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& value );

  /**
      This method imports a LLM from a json value.

      @brief This method imports a LLM from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
      This method exports a LLM to a file.

      @brief This method exports a LLM to a file.
      @param[in]    filename  file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_file( const std::string& filename )const;

  /**
      This method exports a LLM to an XML file.

      @brief This method exports a LLM to an XML file.
      @param[in]    filename  XML file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( const std::string& filename )const;

  /**
      This method exports a LLM to a tinyxml2 document.

      @brief This method exports a LLM to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const;

  /**
      This method exports a LLM to a json formatted file.

      @brief This method exports a LLM to a json formatted file.
      @param[in]    filename  filename to be written to in json format
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( const std::string& filename )const;

  /**
      This method exports a LLM to a json formatted string.

      @brief This method exports a LLM to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a LLM to a json value.

      @brief This method exports a LLM to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  std::shared_ptr< FeaturePool > feature_pool = std::shared_ptr< FeaturePool >();
  std::vector< double > weights = std::vector< double >();

  protected:

  private:

  };

  /**
      LLM class ostream operator

      @brief LLM class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const LLM& other );
} // namespace h2sl
