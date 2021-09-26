/**
 * @file dcg.h
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

#include "h2sl/factorset.h"
#include "h2sl/llm.h"
#include "h2sl/symbolspace.h"

namespace h2sl {
/**
  DCG class definition
**/
class DCG {
  public:

  /**
    Structure to contain the solutions of a search call on the DCG
    prob: the probability associated with the solution
    sentence: The labeled sentence for that solution
  **/
  struct dcg_search_solution_t{
    dcg_search_solution_t( const double prob, const h2sl::Sentence& sentence ) : prob( prob ), sentence( sentence ){};
    double prob;
    h2sl::Sentence sentence;
  };

  /** Type alias to refer to a vector of solutions from DCG search **/
  using vecDCGSearchSolutions = std::vector< dcg_search_solution_t >;

  /** Type alias of a map of std::vector index keys storing vectors of feature tuples associated with that key **/
  using mapIndexFeatureIndices = std::map< size_t, std::vector< FeaturePool::feature_indices_t > >;

  /**
    Structure to contain the substantial features for a particular factor
    factor: the shared_ptr to the factor
    substantial_features: a map of feature tuples with substantial weight for the factor
        key: the constituent feature index for the world-dependent feature of the tuple
        value: a vector of feature tuples (feature_indices_t) that use the key world-dependent symbol constituent feature
  **/
  struct factor_substantial_features_t{
    std::shared_ptr<Factor> factor = nullptr;
    mapIndexFeatureIndices substantial_features = mapIndexFeatureIndices();
  };

  /** Type alias of a map of substantial features keyed by a string **/
  using vecSubstantialFeatures = std::vector< factor_substantial_features_t >;

  /**
    Do not allow the default constructor for h2sl::DCG class
  **/
  DCG() = delete;

  /**
    DCG parameter constructor for a factor graph. Language determines the columns of the graph (each column is a FactorSet). The 
    symbolspace determines the height of each column (each node in the column is a Factor with a unique symbol and cvs). The
    llm's FeaturePool provides the pool of features from which each Factor builds its FeatureSet.

    @brief DCG parameter constructor to construct the factor graph
    @param[in]    symbolSpace     The SymbolSpace from which each FactorSet builds its column of Factors
    @param[in]    sentenceArg     The sentence containing the language variable for each FactorSet
    @param[in]    worldArg        The world parameter common to all Factors in the graph
    @param[in]    llmArg          The log-linear model common to all Factors in the graph
    @param[in]    cvsArg          The correspondence variables to be shared with all Factors in the graph
    @param[in]    debug           An optional flag for more verbose printouts
    @throws                       Throws runtime error if various elements are null
  **/
  DCG( const std::shared_ptr<SymbolSpace>& symbolSpace,
      const std::shared_ptr<Sentence>& sentenceArg,
      const std::shared_ptr<World>& worldArg,
      const std::shared_ptr<LLM>& llmArg,
      const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvsArg,
      const bool debug = false );

  /**
    Default copy constructor for h2sl::DCG class
  **/
  DCG( const DCG& other ) = default;

  /**
    Default move constructor for h2sl::DCG class
  **/
  DCG( DCG&& other ) = default;

  /**
    Default destructor for h2sl::DCG class. Virtual to allow for derived variants.
  **/
  virtual ~DCG() = default;

  /**
    Default class assignment operator for h2sl::DCG class
  **/
  DCG& operator=( const DCG& other ) = default;

  /**
    Default move assignment operator for h2sl::DCG class
  **/
  DCG& operator=( DCG&& other ) = default;

  /**
    This method searches for the set of true correspondence variables (expressed symbols/groundings) that maximizes the log-likelihood
    of the factor graph. The search is performed for a given beamwidth.

    @brief Search for the N best solutions for the factor graph, where N is the beamwidth.
    @param[in]    beamwidth     The beamwidth of the search process (default is 4)
    @param[in]    debug         An optional flag for more verbose printouts
    @returns                    A ref to a vector of solutions (size beamwidth); Optional type to signal function failure
    @throws                     No expected throws
  **/
  virtual const std::optional< vecDCGSearchSolutions >&
  search( const unsigned int beamwidth, const bool debug );

 /**
    This method finds the next FactorSet to evaluate by looking for the lowest FactorSet with no associated solutions. It returns
    a pointer to that found FactorSet.
  **/
  std::shared_ptr<FactorSet> next_factorset( void )const;

  /**
    This method updates the pygx result of each Factor & the argmax solution of each FactorSet given a new world configuration

    @brief Update the DCG for a new world configuration
    @param[in]    new_world     The new world configuration
    @param[in]    debug         An optional flag for verbose printouts
    @returns                    A ref to the updated vector of solutions (size 1)
    @throws                     Can throw upon bad optional access
  **/
  const std::optional< vecDCGSearchSolutions >&
  update_world_configuration( const std::shared_ptr<World> new_world, const bool debug = false );

  /**
    This method introspects the state of the factors in the graph (after search) to identify which world-dependent constituent features 
    substantially contribute to a factor's evaluation.

    @returns          A map of world-dependent constituent features and the associated substantial weight-vector indices
  **/
  vecSubstantialFeatures
  extract_substantial_world_features( const bool debug = false )const;

  /**
    This method checks whether the prior evaluation of Factors is still valid in the context of a new world. It requires that the
    entities of the world have not changed beyond their configuration (i.e. no new objects, no missing objects).

    This method uses direct feature re-evaluation to check this validity. An alternative can use constraints.
  **/
  bool valid( const std::shared_ptr<World> new_world,
              const vecSubstantialFeatures& substantial_features,
              const bool debug = false )const;

  /**
    This method imports a DCG from a file.

    @brief This method imports a DCG from a file.
    @param[in]    filename  file to be imported from
    @returns                success/failure
    @throws                 no expected throws
  */
  virtual bool from_file( const std::string& filename );

  /**
      This method imports a DCG from an XML file.

      @brief This method imports a DCG from an XML file.
      @param[in]    filename  XML file to be imported from
      @returns                none
      @throws                 no expected throws
  */
  virtual bool from_xml( const std::string& filename );

  /**
      This method imports a DCG from a tinyxml2 element.

      @brief This method imports a DCG from a tinyxml2 element.
      @param[in]    root      tinyxml2 element to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_xml( const tinyxml2::XMLElement* root );

  /**
      This method imports a DCG from a json formatted file.

      @brief This method imports a DCG from a json formatted file.
      @param[in]    root      json formatting file to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const std::string& value );

  /**
      This method imports a DCG from a json formatted string.

      @brief This method imports a DCG from a json formatted string.
      @param[in]    root      json formatting string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& value );

  /**
      This method imports a DCG from a json value.

      @brief This method imports a DCG from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
      This method exports a DCG to a file.

      @brief This method exports a DCG to a file.
      @param[in]    filename  file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_file( const std::string& filename )const;

  /**
      This method exports a DCG to an XML file.

      @brief This method exports a DCG to an XML file.
      @param[in]    filename  XML file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( const std::string& filename )const;

  /**
      This method exports a DCG to a tinyxml2 document.

      @brief This method exports a DCG to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const;

  /**
      This method exports a DCG to a json formatted file.

      @brief This method exports a DCG to a json formatted file.
      @param[in]    filename  filename to be written to in json format
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( const std::string& filename )const;

  /**
      This method exports a DCG to a json formatted string.

      @brief This method exports a DCG to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a DCG to a json value.

      @brief This method exports a DCG to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  /**
    This method provides read-only access to the _factorsets member
  **/
  inline const std::unordered_map< std::string, std::shared_ptr<FactorSet> >&
  factorsets( void )const{ return _factorsets; }

  /**
    This method provides read-only access to the _sentence member
  **/
  inline const std::shared_ptr<Sentence>
  sentence( void )const{ return _sentence; }

  /**
    This method provides read-only access to the _world member
  **/
  inline const std::shared_ptr<World>
  world( void )const{ return _world; }

  /**
    This method provides read-only access to the _llm member
  **/
  inline const std::shared_ptr<LLM>
  llm( void )const{ return _llm; }

  /**
    This method provides read-only access to the _solutions member
  **/
  inline const std::optional<vecDCGSearchSolutions>&
  solutions( void )const{ return _solutions; }
 
  protected:

  private:

  /**
    This method constructs a child FactorSet.

    @brief Construct a child FactorSet
    @param[in]    symbolSpace     The SymbolSpace from which each FactorSet builds its column of Factors
    @param[in]    lvconnArg       The LanguageVariable & edge label parameter common among all Factors in a single Factorset
    @param[in]    cvsArg          The correspondence variables to be shared with all Factors in the graph
    @param[in]    parent          A pointer to the parent FactorSet, if there is one; Used to fill in child connections
    @returns                      None - Updates the DCG factorsets data member
    @throws                       No expected throws
    
  **/
  virtual void
  _construct_child_factorset( const std::shared_ptr<SymbolSpace>& symbolSpace,
                              const LanguageVariable::language_variable_connection_t& lvconnArg,
                              const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvsArg,
                              std::shared_ptr<FactorSet> parent );


  /**
    This method sets the state of each Factor in a FactorSet to reflect the state of the provided solution (labeled language variable)

    @brief Set the matching FactorSet's state to reflect a provided solution language variable
    @param[in]    solution    The labeled language variable associated with the FactorSet
    @returns                  None - Recursively updates the DCG factorsets data member to reflect the solution
  **/
  void _set_factorset_state( const std::shared_ptr< LanguageVariable > solution_lv, const bool debug = false );

  /**
    This method updates a factorset given a new world configuration. It recursively updates all children first. The result is 
    an efficiently updates solution that is equivalent to a search of beamwidth 1. The old solutions are discarded, each Factor
    is updated, and the new solution is stored.

    @param[in]    parameter_dependent_constituent_indices    Indices of world-or-child dependent symbol constituent features
    @param[in]    factorset                                  The factorset to update
    @param[in]    debug                                      Optional flag for verbose prinouts
    @returns                                                 A boolean indicating if the argmax solution has changed
    @throws                                                  Can throw upon bad optional access
  **/
  bool
  _update_factorset_world_configuration(
              const std::unordered_map< std::string, std::vector<size_t> >& parameter_dependent_constituent_indices,
              std::shared_ptr<FactorSet> factorset,
              const bool debug = false );

  /**
    A structure to hold the FactorSets (columns) of the graph. Each column is uniquely associated with a language variable,
    so the map key can be the language's string and used to directly access a particular FactorSet (column).

    key: unique string associated with a language variable
    value: FactorSet (column) for that language variable
  **/
  std::unordered_map<std::string,std::shared_ptr<FactorSet>> _factorsets = std::unordered_map<std::string,std::shared_ptr<FactorSet>>();

  /** A member to hold the sentence constituting the language used to compose the factor graph **/
  std::shared_ptr<Sentence> _sentence = nullptr;

  /** A member to hold the world associated with the factor graph **/
  std::shared_ptr<WorldDCG> _world = nullptr;

  /** A member to hold the log-linear model associated with the factor graph **/
  std::shared_ptr<LLM> _llm = nullptr;

  /** A member to hold the found solutions after search **/
  std::optional<vecDCGSearchSolutions> _solutions = std::nullopt; 

};

  /**
      DCG class ostream operator

      @brief DCG class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const DCG& other );
} // namespace h2sl
