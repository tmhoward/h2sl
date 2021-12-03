/**
 * @file factorset.h
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

#include "h2sl/factor.h"
#include "h2sl/llm.h"
#include "h2sl/dialogue.h"

namespace h2sl {
  /**
   * FactorSet class definition
   */
  class FactorSet {
  public:

  /** Structure for representing FactorSet child connections */
  struct factorset_connection_t{
    std::optional< std::string > label = std::nullopt;
    std::shared_ptr<h2sl::FactorSet> factorset = nullptr;
  };

  /**
    Structure to contain the solutions of a search call on the FactorSet
    prob: the probability associated with the solution
    language_variable: the labeled language variable for that solution
  **/
  struct fs_search_solution_t{
    fs_search_solution_t( const double prob, const std::shared_ptr<h2sl::LanguageVariable> lv ) : prob( prob ), language_variable( lv ){};
    double prob;
    std::shared_ptr< h2sl::LanguageVariable > language_variable;
  };

  /** Type alias to refer to a vector of solutions from FactorSet search **/
  using vecFSSearchSolutions = std::vector< fs_search_solution_t >;

  /**
    Default constructor for h2sl::FactorSet
  */
  FactorSet() = default;

  /**
    Parameter constructor for h2sl::FactorSet - Constructs factors for each symbol in the passed symbolspace
  */
  FactorSet( const std::shared_ptr<FeaturePool>& featurePool,
             const std::shared_ptr<SymbolSpace>& symbolSpace,
             const std::shared_ptr<LanguageVariable>& lvArg,
             const std::shared_ptr<WorldDCG>& worldArg,
             const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvsArg );

  /**
    Default copy constructor for h2sl::FactorSet
  */
  FactorSet( const FactorSet& other ) = default;

  /**
    Default move constructor for h2sl::FactorSet
  */
  FactorSet( FactorSet&& other ) = default;

  /**
    Default destructor for h2sl::FactorSet
  */
  virtual ~FactorSet() = default;

  /**
    Default assignment operator for h2sl::FactorSet
  */
  FactorSet& operator=( const FactorSet& other ) = default;

  /**
    Default move assignment operator for h2sl::FactorSet
  */
  FactorSet& operator=( FactorSet&& other ) = default;

  /**
    Method to search for the most likely correspondence variables for each Factor in the FactorSet. Uses a beamwidth.

    @brief Search the FactorSet
    @param[in]    llm         The trained log-linear model for computing each Factor's likelihood
    @param[in]    beamwidth   An optional beamwidth for the search process
    @param[in]	  debug	      Optional flag for verbose printouts
    @returns                  A const reference to the FactorSet's solutions member
  **/
  virtual const vecFSSearchSolutions&
  search( const std::shared_ptr<LLM>& llm, const unsigned int beamwidth = 4, const bool debug = false );

  /**
    This method imports a FactorSet from a file.

    @brief This method imports a FactorSet from a file.
    @param[in]    filename  file to be imported from
    @returns                success/failure
    @throws                 no expected throws
  */
  virtual bool from_file( const std::string& filename );

  /**
      This method imports a FactorSet from an XML file.

      @brief This method imports a FactorSet from an XML file.
      @param[in]    filename  XML file to be imported from
      @returns                none
      @throws                 no expected throws
  */
  virtual bool from_xml( const std::string& filename );

  /**
      This method imports a FactorSet from a tinyxml2 element.

      @brief This method imports a FactorSet from a tinyxml2 element.
      @param[in]    root      tinyxml2 element to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_xml( const tinyxml2::XMLElement* root );

  /**
      This method imports a FactorSet from a json formatted file.

      @brief This method imports a FactorSet from a json formatted file.
      @param[in]    root      json formatting file to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const std::string& value );

  /**
      This method imports a FactorSet from a json formatted string.

      @brief This method imports a FactorSet from a json formatted string.
      @param[in]    root      json formatting string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& value );

  /**
      This method imports a FactorSet from a json value.

      @brief This method imports a FactorSet from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
      This method exports a FactorSet to a file.

      @brief This method exports a FactorSet to a file.
      @param[in]    filename  file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_file( const std::string& filename )const;

  /**
      This method exports a FactorSet to an XML file.

      @brief This method exports a FactorSet to an XML file.
      @param[in]    filename  XML file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( const std::string& filename )const;

  /**
      This method exports a FactorSet to a tinyxml2 document.

      @brief This method exports a FactorSet to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const;

  /**
      This method exports a FactorSet to a json formatted file.

      @brief This method exports a FactorSet to a json formatted file.
      @param[in]    filename  filename to be written to in json format
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( const std::string& filename )const;

  /**
      This method exports a FactorSet to a json formatted string.

      @brief This method exports a FactorSet to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a FactorSet to a json value.

      @brief This method exports a FactorSet to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  /**
      This method counts the number of children with solutions

      @brief This method counts the number of children with solutions
      @returns      unsigned int  number of child factorsets with solutions
      @throws                     no expected throws
  */
  unsigned int num_children_with_solutions( void )const;

  std::vector< std::shared_ptr<Factor> > factors = std::vector< std::shared_ptr<Factor> >();
  std::shared_ptr<LanguageVariable> language_variable = nullptr;
  std::vector< factorset_connection_t > children = std::vector< factorset_connection_t >();
  vecFSSearchSolutions solutions = vecFSSearchSolutions();

  protected:

  private:

  };

  /**
      FactorSet class ostream operator

      @brief FactorSet class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const FactorSet& other );
} // namespace h2sl
