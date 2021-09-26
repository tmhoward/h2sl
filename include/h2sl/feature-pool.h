/**
 * @file feature-pool.h
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

#include "h2sl/symbol.h"
#include "h2sl/world.h"
#include "h2sl/language-variable.h"
#include "h2sl/feature.h"

namespace h2sl {
  /**
   * FeaturePool class definition
   */
  class FeaturePool {
  public:

  /**
    Structure to contain a single feature's indices for both the constituent_feature_sets and the LLM::weight vector member
    weight_index: the index into the LLM weight vector
    feature_set_indices: the vector containing the constituent feature set indices for this feature tuple
    cv(): the index into the CV constituent feature set
    language(): the index into the Language constituent feature set
    symbol(): the index into the Symbol constituent feature set
  **/
  struct feature_indices_t{
    // Constructor to resize the feature_set_indices vector to 3 upon construction
    feature_indices_t() : weight(), feature_set_indices(){ feature_set_indices.resize(3); }
    // Data members
    size_t weight;
    std::vector<size_t> feature_set_indices = std::vector<size_t>();
    // Named accessors for legibility
    inline size_t& cv( void ){ return feature_set_indices.at(0); }
    inline const size_t& cv( void )const{ return feature_set_indices.at(0); }
    inline size_t& language( void ){ return feature_set_indices.at(1); }
    inline const size_t& language( void )const{ return feature_set_indices.at(1); }
    inline size_t& symbol( void ){ return feature_set_indices.at(2); }
    inline const size_t& symbol( void )const{ return feature_set_indices.at(2); }
  };

  /** Type alias for a std::vector of std::vector of Feature std::shared_ptrs.*/
  using vecVecFeatures = std::vector< std::vector< std::shared_ptr<Feature> > >;

  /** Type alias for a std::vector of std::vector of vector index values (size_t)*/
  using vecVecIndices = std::vector< std::vector< size_t > >;

  /** Type alias for a std::vector of feature_indices_t **/
  using vecFeatureIndices = std::vector< feature_indices_t >;

  /**
    Structure to contain the indices into both the weight vector and the individual constituent_feature_sets for a single CV value
    feature_indices: The vector of indices into the weight vector for a single tuple of constituent features.
    constituent_feature_set_indices: A vector of vector of indices into the constituent_feature_sets of FeaturePool and FeatureSet
  **/
  struct indices_t{
    vecFeatureIndices feature_indices = vecFeatureIndices();
    vecVecIndices constituent_feature_set_indices = vecVecIndices();
  };


  /**
    Default constructor for h2sl::FeaturePool
  */
  FeaturePool() = default;

  /**
    Parameter constructor for h2sl::FeaturePool to load from an XML file
  **/
  explicit FeaturePool( const tinyxml2::XMLElement* root );

  /**
    Default copy constructor for h2sl::FeaturePool
  */
  FeaturePool( const FeaturePool& other ) = default;

  /**
    Default move constructor for h2sl::FeaturePool
  */
  FeaturePool( FeaturePool&& other ) = default;

  /**
    Default destructor for h2sl::FeaturePool
  */
  virtual ~FeaturePool() = default;

  /**
    Default assignment operator for h2sl::FeaturePool
  */
  FeaturePool& operator=( const FeaturePool& other ) = default;

  /**
    Default move assignment operator for h2sl::FeaturePool
  */
  FeaturePool& operator=( FeaturePool&& other ) = default;

  /**
    This method evaluates the features using the provided parameters and returns the indices of the features evaluated as True

    @brief This method evaluates the features and returns the indices of the True features
    @param[in]    cvs       The possible CV values
    @param[in]    symbol    The symbol parameter to be passed to each feature evaluation
    @param[in]    lv        The language variable parameter to be passed to each feature evaluation
    @param[in]    world     The world parameter to be passed to each feature evaluation
    @param[in]    debug     An optional flag for more verbose printouts
    @returns                The feature indices for features evaluated as True
    @throws                 Throws runtime_error for several enforced assumptions
  **/
  virtual std::map< std::string, FeaturePool::indices_t >
  evaluate( const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvs,
            const std::shared_ptr<h2sl::Symbol>& symbol,
            const std::shared_ptr<h2sl::LanguageVariable>& lv,
            const std::shared_ptr<h2sl::WorldDCG>& world,
            const bool& debug = false );

  /**
    This method returns the number of features represented by the vector of feature vectors

    @brief Returns the number of features represented by the vector of feature vectors
    @returns                number of features
    @throws                 no expected throws
  */
  unsigned int num_constituent_features( void )const;

  /**
    This method converts from constituent feature indices to a flattened weight vector index

    @brief This method converts constituent feature indices to a weight index
    @param[in]    i   constituent feature index for the first set
    @param[in]    j   constituent feature index for the second set
    @param[in]    k   constituent feature index for the third set
    @returns          the converted weight index
    @throws           No expected throws
  **/
  size_t convert_to_weight_index( size_t i, size_t j, size_t k )const{
    size_t j_max = constituent_feature_sets[1].size();
    size_t k_max = constituent_feature_sets[2].size();
    return (i * j_max * k_max) + (j* k_max) + k;
  }

  /**
    This method imports a FeaturePool from a file.

    @brief This method imports a FeaturePool from a file.
    @param[in]    filename  file to be imported from
    @returns                success/failure
    @throws                 no expected throws
  */
  virtual bool from_file( const std::string& filename );

  /**
      This method imports a FeaturePool from an XML file.

      @brief This method imports a FeaturePool from an XML file.
      @param[in]    filename  XML file to be imported from
      @returns                none
      @throws                 no expected throws
  */
  virtual bool from_xml( const std::string& filename );

  /**
      This method imports a FeaturePool from a tinyxml2 element.

      @brief This method imports a FeaturePool from a tinyxml2 element.
      @param[in]    root      tinyxml2 element to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_xml( const tinyxml2::XMLElement* root );

  /**
      This method imports a FeaturePool from a json formatted file.

      @brief This method imports a FeaturePool from a json formatted file.
      @param[in]    root      json formatting file to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const std::string& value );

  /**
      This method imports a FeaturePool from a json formatted string.

      @brief This method imports a FeaturePool from a json formatted string.
      @param[in]    root      json formatting string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& value );

  /**
      This method imports a FeaturePool from a json value.

      @brief This method imports a FeaturePool from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
      This method exports a FeaturePool to a file.

      @brief This method exports a FeaturePool to a file.
      @param[in]    filename  file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_file( const std::string& filename )const;

  /**
      This method exports a FeaturePool to an XML file.

      @brief This method exports a FeaturePool to an XML file.
      @param[in]    filename  XML file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( const std::string& filename )const;

  /**
      This method exports a FeaturePool to a tinyxml2 document.

      @brief This method exports a FeaturePool to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const;

  /**
      This method exports a FeaturePool to a json formatted file.

      @brief This method exports a FeaturePool to a json formatted file.
      @param[in]    filename  filename to be written to in json format
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( const std::string& filename )const;

  /**
      This method exports a FeaturePool to a json formatted string.

      @brief This method exports a FeaturePool to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a FeaturePool to a json value.

      @brief This method exports a FeaturePool to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  /**
   * A data member to store the pool of features to be used during construction of a Factor's feature set.
   * The outer vector is an index into a particular set of constituent features (e.g. CV, Language, or Symbol features)
   * The inner vector is a set of constituent features.
   */
  vecVecFeatures constituent_feature_sets;

  /** A data member to hold symbol features created during Factor construction - these may be copied or duplicated */
  std::map< std::string, std::map< std::string, std::shared_ptr< Feature > > > symbol_features;

  /** A data member to hold language variable features created during Factor construction - these may be copied or duplicated */
  std::map< std::string, std::map< std::string, std::shared_ptr< Feature > > > language_variable_features;

  protected:

  private:

  };

  /**
      FeaturePool class ostream operator

      @brief FeaturePool class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const FeaturePool& other );
} // namespace h2sl
