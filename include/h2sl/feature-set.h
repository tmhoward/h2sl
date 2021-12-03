/**
 * @file feature-set.h
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

#include "h2sl/feature-pool.h"

namespace h2sl {
  /**
   * FeatureSet class definition
   */
  class FeatureSet {
  public:
  /**
    Default constructor for h2sl::FeatureSet
  */
  FeatureSet() = default;

  /**
    Default copy constructor for h2sl::FeatureSet
  */
  FeatureSet( const FeatureSet& other ) = default;

  /**
    Default move constructor for h2sl::FeatureSet
  */
  FeatureSet( FeatureSet&& other ) = default;

  /**
    Default destructor for h2sl::FeatureSet
  */
  virtual ~FeatureSet() = default;

  /**
    Default assignment operator for h2sl::FeatureSet
  */
  FeatureSet& operator=( const FeatureSet& other ) = default;

  /**
    Default move assignment operator for h2sl::FeatureSet
  */
  FeatureSet& operator=( FeatureSet&& other ) = default;

  /**
    This method evaluates the features and returns the indices of the features evaluated as True

    @brief This method evaluates the features and returns the indices of the True features
    @param[in]    cvs               The possible CV values
    @param[in]    symbol            The symbol parameter to be passed to each feature evaluation
    @param[in]    lv                The language variable parameter to be passed to each feature evaluation
    @param[in]    world             The world parameter to be passed to each feature evaluation
    @param[in]    expressedFeatures A structure of expressed features to be filled for True features
    @param[in]    debug             An optional flag for more verbose printouts
    @returns                        The feature indices for features evaluated as true
    @throws                         Throws runtime_error for several enforced assumptions
  */
  std::map< std::string, FeaturePool::indices_t >
  evaluate( const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvs,
             const std::shared_ptr< Symbol >& symbol,
             const std::shared_ptr< LanguageVariable >& lv,
             const std::shared_ptr< WorldDCG >& world,
             std::map< std::string, std::vector< ExpressedFeature > >& expressedFeatures,
             const bool& debug = false );

  /**
    This method returns the number of features represented by the vector of feature vectors

    @brief This method runs the number of features represented by the vector of feature vectors
    @returns                number of features
    @throws                 no expected throws
  */
  unsigned int num_constituent_features( void )const;

  /**
    This method converts from constituent feature indices to a flattened weight vector index

    @brief This method converts constituent feature indices to a weight index
    @param[in]      i       constituent feature index for the first set
    @param[in]      j       constituent feature index for the second set
    @param[in]      k       constituent feature index for the third set
    @returns                the converted weight index
    @throws                 No expected throws
  **/
  size_t convert_to_weight_index( size_t i, size_t j, size_t k )const{
    size_t j_max = constituent_feature_sets[1].size();
    size_t k_max = constituent_feature_sets[2].size();
    return (i * j_max * k_max) + (j * k_max) + k;
  }

  /**
    This method imports a FeatureSet from a file.

    @brief This method imports a FeatureSet from a file.
    @param[in]    filename  file to be imported from
    @returns                success/failure
    @throws                 no expected throws
  */
  virtual bool from_file( const std::string& filename );

  /**
      This method imports a FeatureSet from an XML file.

      @brief This method imports a FeatureSet from an XML file.
      @param[in]    filename  XML file to be imported from
      @returns                none
      @throws                 no expected throws
  */
  virtual bool from_xml( const std::string& filename );

  /**
      This method imports a FeatureSet from a tinyxml2 element.

      @brief This method imports a FeatureSet from a tinyxml2 element.
      @param[in]    root      tinyxml2 element to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_xml( const tinyxml2::XMLElement* root );

  /**
      This method imports a FeatureSet from a json formatted file.

      @brief This method imports a FeatureSet from a json formatted file.
      @param[in]    root      json formatting file to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const std::string& value );

  /**
      This method imports a FeatureSet from a json formatted string.

      @brief This method imports a FeatureSet from a json formatted string.
      @param[in]    root      json formatting string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& value );

  /**
      This method imports a FeatureSet from a json value.

      @brief This method imports a FeatureSet from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
      This method exports a FeatureSet to a file.

      @brief This method exports a FeatureSet to a file.
      @param[in]    filename  file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_file( const std::string& filename )const;

  /**
      This method exports a FeatureSet to an XML file.

      @brief This method exports a FeatureSet to an XML file.
      @param[in]    filename  XML file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( const std::string& filename )const;

  /**
      This method exports a FeatureSet to a tinyxml2 document.

      @brief This method exports a FeatureSet to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const;

  /**
      This method exports a FeatureSet to a json formatted file.

      @brief This method exports a FeatureSet to a json formatted file.
      @param[in]    filename  filename to be written to in json format
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( const std::string& filename )const;

  /**
      This method exports a FeatureSet to a json formatted string.

      @brief This method exports a FeatureSet to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a FeatureSet to a json value.

      @brief This method exports a FeatureSet to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  /**
    A data member to store the individual sets of constituent features. The outer vector is an index into a particular set. The inner
    vector are the constituent features for that set.
  **/
  FeaturePool::vecVecFeatures constituent_feature_sets;

  protected:

  private:

  };

  /**
      FeatureSet class ostream operator

      @brief FeatureSet class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const FeatureSet& other );
} // namespace h2sl
