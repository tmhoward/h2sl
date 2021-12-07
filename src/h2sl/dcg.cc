/**
 * @file dcg.cc
 * @author h2sl developers
 *
 * @version 2.0
 *
 * @section license
 *
 * this file is part of h2sl.
 *
 * this program is free software; you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation; either version 2 of the license, or (at
 * your option) any later version.
 *
 * this program is distributed in the hope that it will be useful, but
 * without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the gnu
 * general public license for more details.
 *
 * you should have received a copy of the gnu general public license
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the free
 * software foundation, inc., 51 franklin street, fifth floor, boston, ma
 * 02110-1301, usa.
 *
 * @section description
 */

#include <fstream>
#include <sstream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <boost/algorithm/string.hpp>

#include "h2sl/dcg.h"

namespace h2sl {

///
/// Constructor to construct the factor graph from a LLM, SymbolSpace, Sentence, World, and CVs
///
DCG::DCG( const std::shared_ptr<SymbolSpace>& symbolSpace,
          const std::shared_ptr<Sentence>& sentenceArg,
          const std::shared_ptr<World>& worldArg,
          const std::shared_ptr<LLM>& llmArg,
          const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvsArg,
          const bool debug )
          : _factorsets(),
            _sentence(),
            _world( std::make_shared<WorldDCG>( *worldArg ) ),
            _llm( llmArg ),
            _solutions()
{
  // Construct the FactorSet for the root of the graph
  // Use the child language_variable of the sentence
  if( sentenceArg == nullptr ) throw std::runtime_error("DCG cannot be constructed with a nullptr sentence.");
  _sentence = std::make_shared<Sentence>( *sentenceArg );
  _sentence->symbols().clear();
  _sentence->child()->clear_symbols();
  // Clear the LLM of any prior stored features
  _llm->feature_pool->symbol_features.clear();
  _llm->feature_pool->language_variable_features.clear();
  // Construct the factorset for the root
  if( debug ) std::cout << "Extracting the root language variable" << std::endl;
  std::shared_ptr<LanguageVariable> p_root_language_variable = _sentence->child();
  auto p_root_factorset = std::make_shared<FactorSet>( _llm->feature_pool, symbolSpace, p_root_language_variable, _world, cvsArg );

  // Add the root FactorSet to LLM
  if( debug )
    std::cout << "Adding the root factorset" << std::endl;
  _factorsets.emplace( p_root_language_variable->key(), p_root_factorset );

  // Recursively construct the remaining FactorSets
  for( const auto& child_connection : p_root_language_variable->children() ){
    _construct_child_factorset( symbolSpace, child_connection, cvsArg, p_root_factorset );
  }

  // Provide optional verbose printouts
  if( debug ){
    std::cout << "generated " << _factorsets.size() << " factorsets" << std::endl;
    for( auto const& p_factorset : _factorsets ){
      std::cout << "factorset" << std::endl;
      std::cout << "  language_variable:\"" << *p_factorset.second->language_variable << "\"" << std::endl;
      std::cout << "  factors[" << p_factorset.second->factors.size() << "]:" << std::endl;
      for( auto const& p_factor : p_factorset.second->factors ){
        std::cout << "    factor:" << *p_factor << std::endl;
      }
      std::cout << "  children[" << p_factorset.second->children.size() << "]:" << std::endl;
    }
  }
  return;
}

///
/// Method to construct the factor graph - uses the DCG's _world member
///
void DCG::_construct_child_factorset( const std::shared_ptr<SymbolSpace>& symbolSpace,
                                     const LanguageVariable::language_variable_connection_t& lvconnArg,
                                     const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvsArg,
                                     std::shared_ptr<FactorSet> parent )
{
  // Check that the FactorSet does not already exist
  auto it_factorsets = _factorsets.find( lvconnArg.child->key() );
  if( it_factorsets == _factorsets.end() ){
    auto child_factorset = std::make_shared<FactorSet>( _llm->feature_pool, symbolSpace, lvconnArg.child, _world, cvsArg );
    bool success;
    std::tie(it_factorsets, success) = _factorsets.emplace( lvconnArg.child->key(), child_factorset );
  }

  // Assign the child_factorset to the parent
  FactorSet::factorset_connection_t child_factorset_connection;
  child_factorset_connection.label = lvconnArg.label;
  child_factorset_connection.factorset = it_factorsets->second;
  parent->children.push_back( child_factorset_connection );

  // Construct all child FactorSets of the current
  for( auto const& child_lv_connection : lvconnArg.child->children() ){
    _construct_child_factorset( symbolSpace, child_lv_connection, cvsArg, it_factorsets->second );
  }
  return;
}

///
/// DCG search method
///
const std::optional< DCG::vecDCGSearchSolutions >&
DCG::search( const unsigned int beamwidth, const bool debug ){
  // Enforce that the graph has been constructed
  if( _factorsets.empty() ){
    std::cout << "The DCG has no factorsets, not performing a search." << std::endl;
    _solutions = std::nullopt;
    return _solutions;
  }

  if( debug )
    std::cout << "Performing search for \"" << *_sentence << "\"" << std::endl;

  // Find the leafiest solution-free FactorSet in the graph
  std::shared_ptr<FactorSet> next = next_factorset();
  while( next != nullptr ){
    // Search the leafiest solution-free FactorSet
    auto r_factorset_solutions = next->search( _llm, beamwidth, debug );
    if( debug ){
      std::cout << "\n\nnext->solutions.size():" << r_factorset_solutions.size() << std::endl;
      for( const auto& solution : r_factorset_solutions ){
        std::cout << "  solution(" << solution.prob << "):" << *solution.language_variable << "\n";
      }
      std::cout << "\n" << std::endl;
    }
    // Find the next leafiest solution-free FactorSet in the graph
    next = next_factorset();
  }

  // Initialize the solutions to a vector
  _solutions = DCG::vecDCGSearchSolutions();
  // Fill the vector of solutions from the root FactorSet
  auto p_root_factorset = _factorsets.find( _sentence->child()->key() );
  for( auto & solution : p_root_factorset->second->solutions ){
    _solutions->emplace_back( solution.prob, h2sl::Sentence( _sentence->uid(), solution.language_variable ) );
  }

  // Set the state of the Factors in the graph to reflect the argmax solution
  _set_factorset_state( _solutions->front().sentence.child(), debug );

  // Return the solutions
  return _solutions;
}

///
/// Method to find the next factorset to search
///
std::shared_ptr<FactorSet> DCG::next_factorset( void )const{
  for( const auto& [key, p_factorset] : _factorsets ){
    // check to see if the factorset already has any solutions
    if( p_factorset->solutions.empty() ){
      // if the factorset has no solutions, check to make sure that all child factorsets have solutions
      if( p_factorset->children.size() == p_factorset->num_children_with_solutions() ){
        return p_factorset;
      }
    }
  }
  return nullptr;
}

///
/// Method to set the state of a FactorSet to the provided solution (labeled language variable)
///
void DCG::_set_factorset_state( const std::shared_ptr< LanguageVariable > solution_lv, const bool debug ){
  // Recursively set any children
  for( const auto& child_connection : solution_lv->children() ){
    _set_factorset_state( child_connection.child, debug );
  }

  // Enforce that the solution_lv matches a FactorSet in the DCG
  auto it_factorset = _factorsets.find( solution_lv->key() );
  if( it_factorset == _factorsets.end() ){
    std::stringstream error_msg;
    error_msg << "Could not find a factorset for the solution LV \"" << solution_lv->key() << "\"";
    throw std::runtime_error( error_msg.str() );
  }

  if( debug )
    std::cout << "Setting the FactorSet state for the solution LV \"" << solution_lv->key() << "\"" << std::endl;

  // Update each Factor in the FactorSet to reflect the solution
  for( auto& p_factor : it_factorset->second->factors ){
    // Set the Factor's language variable to the solution - This provides access to the child symbol for factor evaluation
    p_factor->language_variable = solution_lv;

    // Update the Factor's indices by re-evaluating the Factor
    // Dummy expressed_features to call evaluate
    std::map< std::string, std::vector< ExpressedFeature > > expressed_features;
    p_factor->evaluate( expressed_features, debug );

    // Check if the Factor's symbol is in the solution
    bool found_match = false;
    for( const auto& symbol : solution_lv->symbols() ){
      if( *(p_factor->symbol) == *symbol ){
        found_match = true;
        break;
      }
    }

    // If there is a match, evaluate the pygx for the factor using TRUE cv
    if( found_match ){
      if( debug )
        std::cout << "Found a match for symbol: " << *(p_factor->symbol) << std::endl;
      p_factor->pygx_result = _llm->pygx( std::make_shared<std::string>("true"), p_factor->indices, true );
    } else{
      if( debug )
        std::cout << "No match found for symbol: " << *(p_factor->symbol) << std::endl;
      p_factor->pygx_result = _llm->pygx( std::make_shared<std::string>("false"), p_factor->indices, true );
    }
    if( debug ){
      std::cout << "The CV value was " << found_match << std::endl;
      std::cout << "\tProb: " << p_factor->pygx_result->prob << std::endl;
      std::cout << "\tNumerator: " << p_factor->pygx_result->numerator << std::endl;
      std::cout << "\tNenominator: " << p_factor->pygx_result->denominator << std::endl;
      std::cout << "\tSum of Weights:";
      if( p_factor->pygx_result->sum_of_weights ){
        for( const auto& [cv_key, sum_of_weights] : *(p_factor->pygx_result->sum_of_weights) ){
          std::cout << "\n\t" << cv_key << ":\t" << sum_of_weights;
        }
      }
      std::cout << "\n" << std::endl;
    }
  }
  return;
}

///
/// Method to update the pygx result of each Factor & argmax solution for each Factorset given a new world configuration
///
const std::optional< DCG::vecDCGSearchSolutions >&
DCG::update_world_configuration( const std::shared_ptr<World> new_world, const bool debug ){
  if( debug )
    std::cout << "Updating the DCG solution for a new world configuration" << std::endl;
  // Enforce that the new world is composed of the same objects
  if( new_world->objects.size() != _world->objects.size() ){
    if( debug )
      std::cout << "The new world had a different number of objects, not updating the world configuration." << std::endl;
    return _solutions;
  }
  for( const auto& [object_uid, object] : new_world->objects ){
    if( _world->objects.find( object_uid ) == _world->objects.end() ){
      if( debug ){
        std::cout << "The new world had a new object with uid \"" << object_uid
                  << "\", not updating the world configuration." << std::endl;
      }
      return _solutions;
    }
  }
  // Assign the new world to the DCG's _world member - this is reflected across all Factors
  *_world = WorldDCG( *new_world );

  // Get the root factorset
  auto it_root_factorset = _factorsets.find( _sentence->child()->key() );
  if( it_root_factorset == _factorsets.end() ){
    std::stringstream error_msg;
    error_msg << "Could not find the root factorset for \"" << _sentence->child()->key() << "\"";
    throw std::runtime_error( error_msg.str() );
  }

  // Extract the constituent feature indices that are world dependent & the indices that are child dependent (but not world dependent)
  // Can use any Factor's feature_set member to do this - use the root's since we have it
  // These should be constant over the full graph - this allows us to extract them only once
  auto parameter_dependent_constituent_indices = std::unordered_map< std::string, std::vector< size_t > >();
  // Get the world-dependent constituent features first
  auto [it_world_dependent_constituent_features, w_success] = parameter_dependent_constituent_indices.emplace( "world", std::vector<size_t>() );
  auto [it_children_dependent_constituent_features, c_success] = parameter_dependent_constituent_indices.emplace( "children", std::vector<size_t>() );
  auto& r_symbol_constituent_features = it_root_factorset->second->factors.back()->feature_set.constituent_feature_sets[2];

  for( size_t symbol_index = 0; symbol_index < r_symbol_constituent_features.size(); ++symbol_index ){
    // Add any indices for features that depend on the world
    if( r_symbol_constituent_features[symbol_index]->depends_on.world ){
      it_world_dependent_constituent_features->second.push_back( symbol_index );
      continue;
    }
    if( r_symbol_constituent_features[symbol_index]->depends_on.children ){
      it_children_dependent_constituent_features->second.push_back( symbol_index );
    }
  }

  // Recursively update the DCG's factorsets; Pass the root factorset
  _update_factorset_world_configuration( parameter_dependent_constituent_indices, it_root_factorset->second, debug );

  // Fill the vector of solutions from the root FactorSet
  _solutions->clear();
  auto p_root_factorset = _factorsets.find( _sentence->child()->key() );
  for( auto & solution : p_root_factorset->second->solutions ){
    _solutions->emplace_back( solution.prob, h2sl::Sentence( _sentence->uid(), solution.language_variable ) );
  }

  // Return the solutions
  return _solutions;
}


///
/// Method to update a factorset given a new world configuration
///
bool
DCG::_update_factorset_world_configuration(
  const std::unordered_map< std::string, std::vector<size_t> >& parameter_dependent_constituent_indices,
  std::shared_ptr<FactorSet> factorset,
  const bool debug )
{
  // Initialize the return value;
  bool argmax_changed = false;
  // Recursively update any child factorsets; If no children changed their argmax, no need to check child-dependent features
  bool child_solution_changed = false;
  for( auto& child_connection : factorset->children ){
    if( _update_factorset_world_configuration( parameter_dependent_constituent_indices, child_connection.factorset, debug ) )
      child_solution_changed = true;
  }

  if( debug )
    std::cout << "Updating the factorset for LV \"" << *factorset->language_variable << std::endl;
  // Clear old solutions and initialize with a probability of 1
  factorset->solutions.clear();
  factorset->solutions.emplace_back( 1.0, std::make_shared<LanguageVariable>( factorset->language_variable->current() ) );
  // Create the set of possible child solutions over which to search
  for( const auto& child_connection : factorset->children ){
    if( debug )
      std::cout << "Adding solutions for child " << *(child_connection.factorset->language_variable) << std::endl;
    FactorSet::vecFSSearchSolutions new_solutions;
    for( const auto& solution : factorset->solutions ){
      for( const auto& child_solution : child_connection.factorset->solutions ){
        new_solutions.emplace_back( solution.prob, std::make_shared<h2sl::LanguageVariable>( *solution.language_variable ) );
        new_solutions.back().prob *= child_solution.prob;
        new_solutions.back().language_variable->children().emplace_back( child_connection.label, child_solution.language_variable );
      }
    }
    factorset->solutions = new_solutions;
  }
  if( debug ){
    std::cout << "factorset->solutions.size(): " << factorset->solutions.size()
              << "\nfactorset->solutions[0].language_variable: " << *factorset->solutions[0].language_variable << std::endl;
  }

  // Update each Factor in the factorset by selectively checking world-dependent features
  // Check child dependent features if the child argmax changed
  for( auto& p_factor : factorset->factors ){
    if( debug )
      std::cout << "Updating the factor for symbol: " << *p_factor->symbol << std::endl;
    // Store the original state of the pygx_result - Access will throw bad_optional_access if nullopt
    bool original_factor_argmax = ( p_factor->pygx_result->sum_of_weights.value()["true"] > p_factor->pygx_result->sum_of_weights.value()["false"] );
    // Only search the top solution
    // Set the factor's language variable to the current solution
    p_factor->language_variable = factorset->solutions[0].language_variable;
    // Get a reference to the factor's symbol constituent_feature_sets, indices, and pygx_results
    auto& r_symbol_constituent_features = p_factor->feature_set.constituent_feature_sets[2];
    auto& r_indices = p_factor->indices;
    // Throws bad_optional_access if nullopt
    auto& r_pygx_result = p_factor->pygx_result.value();
    if( debug ){
      std::cout << "Original pygx information (original_factor_argmax is " << original_factor_argmax << ")" << std::endl;
      std::cout << "sum of weights:";
      for( const auto& [cv_key, sum_of_weights] : r_pygx_result.sum_of_weights.value() ){
        std::cout << "\n\t" << cv_key << ":\t" << sum_of_weights;
      }
      std::cout << "\nnumerator: " << r_pygx_result.numerator << "\ndenominator: " << r_pygx_result.denominator
                << "\nprob: " << r_pygx_result.prob << std::endl;
    }
    // Handle world-dependent features for object symbols
    if( p_factor->symbol->type == "object" ){
      // Iterate through each world-dependent constituent feature
      auto const cit_world_dependent_indices = parameter_dependent_constituent_indices.find("world");;
      if( cit_world_dependent_indices == parameter_dependent_constituent_indices.cend() )
        throw std::runtime_error("No \"world\" key in the parameter dependent indices map");
      for( const auto& symbol_index : cit_world_dependent_indices->second ){
        // Store the original value
        auto original_value = r_symbol_constituent_features[symbol_index]->value;
        // Evaluate the feature - CV value does not matter
        auto new_value = r_symbol_constituent_features[symbol_index]->evaluate( std::make_shared<std::string>("true"),
                                                                                p_factor->language_variable, p_factor->world,
                                                                                p_factor->symbol );
        // If the values are different, update the Factor's indices and pygx sum
        if( new_value != original_value ){
          // If true, add the weight, add the relevant indices; Else, subtract the weight, remove the relevant indices
          if( new_value == FeatureValue::True ){
            // Add it to the Factor's constituent_feature_set_indices
            r_indices["true"].constituent_feature_set_indices[2].push_back( symbol_index );
            r_indices["false"].constituent_feature_set_indices[2].push_back( symbol_index );
            // Add a tuple for each language constituent feature; Update the sum_of_weights for each new feature
            if( debug ){
            std::cout << "Adding weights for features using the symbol feature: "
                      << *r_symbol_constituent_features[symbol_index] << std::endl;
            }
            for( const auto& language_index : r_indices["true"].constituent_feature_set_indices[1] ){
              auto feature_index = FeaturePool::feature_indices_t();
              // Create a new feature_index for the FALSE cv case; Add it's weight to the pygx_result; Append to indices
              feature_index.cv() = 0;
              feature_index.language() = language_index;
              feature_index.symbol() = symbol_index;
              feature_index.weight = _llm->feature_pool->convert_to_weight_index( 0, language_index, symbol_index );
              r_pygx_result.sum_of_weights.value()["false"] += _llm->weights[feature_index.weight];
              r_indices["false"].feature_indices.push_back( feature_index );
              if( debug ){
                std::cout << "\tlanguage feature: " << *p_factor->feature_set.constituent_feature_sets[1][language_index] << std::endl;
                std::cout << "\t\ttrue weight: " << _llm->weights[feature_index.weight];
              }
              // Create a new feature_index for the True cv case; Add it's weight to the pygx_result; Append to indices
              feature_index.cv() = 1;
              feature_index.weight = _llm->feature_pool->convert_to_weight_index( 1, language_index, symbol_index );
              r_pygx_result.sum_of_weights.value()["true"] += _llm->weights[feature_index.weight];
              r_indices["true"].feature_indices.push_back( feature_index );
              if( debug )
                std::cout << "\t\tfalse weight: " << _llm->weights[feature_index.weight];
            }
          } else{
            // Remove the indices from the constituent_feature_set_indices
            if( debug ){
            std::cout << "Removing weights for features using the symbol feature: "
                      << *r_symbol_constituent_features[symbol_index] << std::endl;
            }
            auto& r_true_symbol_constituent_indices = r_indices["true"].constituent_feature_set_indices[2];
            auto& r_false_symbol_constituent_indices = r_indices["false"].constituent_feature_set_indices[2];
            size_t index_to_erase = 0;
            for( size_t index_to_erase = 0; index_to_erase < r_true_symbol_constituent_indices.size(); ++index_to_erase ){
              if( r_true_symbol_constituent_indices[index_to_erase] == symbol_index )
                break;
            }
            r_true_symbol_constituent_indices.erase( r_true_symbol_constituent_indices.begin() + index_to_erase );
            r_false_symbol_constituent_indices.erase( r_false_symbol_constituent_indices.begin() + index_to_erase );
            // Remove each tuple with that constituent symbol index
            for( auto it_feature_index = r_indices["true"].feature_indices.begin();
                  it_feature_index != r_indices["true"].feature_indices.end();)
            {
              // If it uses the symbol_index, substract the weight & erase the feature_index (returns iterator to next element)
              if( it_feature_index->symbol() == symbol_index ){
                if( debug ){
                  std::cout << "\tlanguage feature: "
                            << *p_factor->feature_set.constituent_feature_sets[1][it_feature_index->language()]
                            << "\n\t\ttrue weight: " << _llm->weights[it_feature_index->weight] << std::endl;;
                }
                r_pygx_result.sum_of_weights.value()["true"] -= _llm->weights[it_feature_index->weight];
                it_feature_index = r_indices["true"].feature_indices.erase( it_feature_index );
              } else
                ++it_feature_index;
            }
            for( auto it_feature_index = r_indices["false"].feature_indices.begin();
                  it_feature_index != r_indices["false"].feature_indices.end();)
            {
              // If it uses the symbol_index, substract the weight & erase the feature_index (returns iterator to next element)
              if( it_feature_index->symbol() == symbol_index ){
                if( debug ){
                  std::cout << "\tlanguage feature: "
                            << *p_factor->feature_set.constituent_feature_sets[1][it_feature_index->language()]
                            << "\n\t\tfalse weight: " << _llm->weights[it_feature_index->weight] << std::endl;;
                }
                r_pygx_result.sum_of_weights.value()["false"] -= _llm->weights[it_feature_index->weight];
                it_feature_index = r_indices["false"].feature_indices.erase( it_feature_index );
              } else
                ++it_feature_index;
            }
          }
        }
      }
    }
    // Handle children-dependent features
    if( child_solution_changed ){
      // Iterate through each children-dependent constituent feature
      auto const cit_children_dependent_indices = parameter_dependent_constituent_indices.find("children");;
      if( cit_children_dependent_indices == parameter_dependent_constituent_indices.cend() )
        throw std::runtime_error("No \"children\" key in the parameter depdenent indices map");
      for( const auto& symbol_index : cit_children_dependent_indices->second ){
        // Store the original value
        auto original_value = r_symbol_constituent_features[symbol_index]->value;
        // Evaluate the feature
        auto new_value = r_symbol_constituent_features[symbol_index]->evaluate( std::make_shared<std::string>("true"),
                                                                                p_factor->language_variable, p_factor->world,
                                                                                p_factor->symbol );
        // If the values are different, update the Factor's indices and pygx sum
        if( new_value != original_value ){
          // If true, add the weight, add the relevant indices; Else, subtract the weight, remove the relevant indices
          if( new_value == FeatureValue::True ){
            // Add it to the Factor's constituent_feature_set_indices
            r_indices["true"].constituent_feature_set_indices[2].push_back( symbol_index );
            r_indices["false"].constituent_feature_set_indices[2].push_back( symbol_index );
            // Add a tuple for each language constituent feature; Update the sum_of_weights for each new feature
            for( const auto& language_index : r_indices["true"].constituent_feature_set_indices[1] ){
              auto feature_index = FeaturePool::feature_indices_t();
              // Create a new feature_index for the FALSE cv case; Add it's weight to the pygx_result; Append to indices
              feature_index.cv() = 0;
              feature_index.language() = language_index;
              feature_index.symbol() = symbol_index;
              feature_index.weight = _llm->feature_pool->convert_to_weight_index( 0, language_index, symbol_index );
              r_pygx_result.sum_of_weights.value()["false"] += _llm->weights[feature_index.weight];
              r_indices["false"].feature_indices.push_back( feature_index );
              feature_index.cv() = 1;
              feature_index.weight = _llm->feature_pool->convert_to_weight_index( 1, language_index, symbol_index );
              r_pygx_result.sum_of_weights.value()["true"] += _llm->weights[feature_index.weight];
              r_indices["true"].feature_indices.push_back( feature_index );
            }
          } else{
            // Remove the indices from the constituent_feature_set_indices
            auto& r_true_symbol_constituent_indices = r_indices["true"].constituent_feature_set_indices[2];
            auto& r_false_symbol_constituent_indices = r_indices["false"].constituent_feature_set_indices[2];
            size_t index_to_erase = 0;
            for( size_t index_to_erase = 0; index_to_erase < r_true_symbol_constituent_indices.size(); ++index_to_erase ){
              if( r_true_symbol_constituent_indices[index_to_erase] == symbol_index )
                break;
            }
            r_true_symbol_constituent_indices.erase( r_true_symbol_constituent_indices.begin() + index_to_erase );
            r_false_symbol_constituent_indices.erase( r_false_symbol_constituent_indices.begin() + index_to_erase );
            // Remove each tuple with that constituent symbol index
            for( auto it_feature_index = r_indices["true"].feature_indices.begin();
                  it_feature_index != r_indices["true"].feature_indices.end();)
            {
              // If it uses the symbol_index, substract the weight & erase the feature_index (returns iterator to next element)
              if( it_feature_index->symbol() == symbol_index ){
                r_pygx_result.sum_of_weights.value()["true"] -= _llm->weights[it_feature_index->weight];
                it_feature_index = r_indices["true"].feature_indices.erase( it_feature_index );
              } else
                ++it_feature_index;
            }
            for( auto it_feature_index = r_indices["false"].feature_indices.begin();
                  it_feature_index != r_indices["false"].feature_indices.end();)
            {
              // If it uses the symbol_index, substract the weight & erase the feature_index (returns iterator to next element)
              if( it_feature_index->symbol() == symbol_index ){
                r_pygx_result.sum_of_weights.value()["false"] -= _llm->weights[it_feature_index->weight];
                it_feature_index = r_indices["false"].feature_indices.erase( it_feature_index );
              } else
                ++it_feature_index;
            }
          }
        }
      }
    }
    // Check if the pygx_result argmax has flipped
    bool updated_factor_argmax = ( r_pygx_result.sum_of_weights.value()["true"] > r_pygx_result.sum_of_weights.value()["false"] );
    if( original_factor_argmax != updated_factor_argmax )
      argmax_changed = true;
    // Update the pygx_result numerator, denominator, and probability
    r_pygx_result.numerator = 0.0;
    r_pygx_result.denominator = 0.0;
    for( const auto& [cv_key, sum_of_weights] : r_pygx_result.sum_of_weights.value() ){
      if( (updated_factor_argmax && (cv_key == "true")) || (!updated_factor_argmax && (cv_key == "false")) ){
        r_pygx_result.numerator += exp(sum_of_weights);
      }
      r_pygx_result.denominator += exp(sum_of_weights);
    }
    r_pygx_result.prob = r_pygx_result.numerator / r_pygx_result.denominator;
    if( debug ){
      std::cout << "Updated pygx information (updated_factor_argmax is " << updated_factor_argmax << ")" << std::endl;
      std::cout << "sum_of_weights:" << std::endl;
      for( const auto& [cv_key, sum_of_weights] : r_pygx_result.sum_of_weights.value() ){
        std::cout << "\n\t" << cv_key << ":\t" << sum_of_weights;
      }
      std::cout << "\nnumerator: " << r_pygx_result.numerator << "\ndenominator: " << r_pygx_result.denominator
                << "\nprob: " << r_pygx_result.prob << std::endl;
    }
    // Update the factorset solution - Add the symbol if the Factor is likely TRUE; Update the product of probabilities
    if( updated_factor_argmax )
      factorset->solutions[0].language_variable->symbols().push_back( std::make_shared<Symbol>( *p_factor->symbol ) );
    factorset->solutions[0].prob *= r_pygx_result.prob;
  }
  return argmax_changed;
}

///
/// Method to identify substantial world-dependent constituent features & their associated weight-vector indices
///
DCG::vecSubstantialFeatures
DCG::extract_substantial_world_features( const bool debug )const{
  std::stringstream error_msg;
  // Ensure that the DCG has been searched already by checking the _solutions member
  if( !_solutions )
    throw std::runtime_error("Attempted to extract world dependent features before searching the DCG");

  // Ensure that the DCG is not an empty graph
  if( _factorsets.empty() )
    throw std::runtime_error("The DCG _factorsets member is empty");

  // Initialize the return structure
  auto substantial_features = vecSubstantialFeatures();

  // Iterate through each FactorSet - the order does not matter
  for( const auto& [factorset_key, p_factorset] : _factorsets ){
    if( debug )
      std::cout << "Extracting features from the FactorSet for \"" << factorset_key << "\"" << std::endl;
    if( p_factorset->factors.empty() ){
      error_msg << "The factorset for \"" << factorset_key << "\" was empty";
      throw std::runtime_error( error_msg.str() );
    }

    // Extract the world dependent indices for all factors in the factorset by looking at the featureset of any factor
    // Note: The language and CV features for these factors should all be evaluated the same
    auto& r_feature_set = p_factorset->factors.back()->feature_set;
    auto world_dependent_indices = std::map< std::string, FeaturePool::indices_t >();
    world_dependent_indices.emplace( "false", FeaturePool::indices_t() );
    world_dependent_indices.emplace( "true", FeaturePool::indices_t() );
    for( size_t language_index = 0; language_index < r_feature_set.constituent_feature_sets[1].size(); ++language_index ){
    for( size_t symbol_index = 0; symbol_index < r_feature_set.constituent_feature_sets[2].size(); ++symbol_index ){
      // Store tuples for TRUE language features and depends_on.world TRUE symbol features
      if( (r_feature_set.constituent_feature_sets[1][language_index]->value == FeatureValue::True)
          && (r_feature_set.constituent_feature_sets[2][symbol_index]->depends_on.world) )
      {
        // Construct and populate a FeaturePool::indices_t with the feature indices information for CV False
        auto current_feature_indices = FeaturePool::feature_indices_t();
        current_feature_indices.weight = r_feature_set.convert_to_weight_index(0, language_index, symbol_index);
        current_feature_indices.cv() = 0;
        current_feature_indices.language() = language_index;
        current_feature_indices.symbol() = symbol_index;
        world_dependent_indices["false"].feature_indices.push_back( current_feature_indices );
        // Construct and populate a FeaturePool::indices_t with the feature indices information for CV False
        current_feature_indices.weight = r_feature_set.convert_to_weight_index(1, language_index, symbol_index);
        current_feature_indices.cv() = 1;
        current_feature_indices.language() = language_index;
        current_feature_indices.symbol() = symbol_index;
        world_dependent_indices["true"].feature_indices.push_back( current_feature_indices );
      }
    } }

    // Check each Factor for substantial world dependent features by computing the impact of each feature on the sum of weights
    if( debug )
      std::cout << "Checking each Factor for substantial world dependent features" << std::endl;
    for( const auto& p_factor : p_factorset->factors ){
      // Skip the factor if it is not for an object symbol
      if( p_factor->symbol->type != "object" ){
        if( debug )
          std::cout << "Skipping non-object factor (" << *(p_factor->symbol) << ")" << std::endl;
        continue;
      }
      if( debug )
        std::cout << "\nExtracting from the factor for symbol: " << *(p_factor->symbol) << std::endl;

      // Get the result for the current state of features for the True CV case
      auto factor_true_result = _llm->pygx( std::make_shared<std::string>("true"), p_factor->indices, true );
      if( !factor_true_result.sum_of_weights )
        throw std::runtime_error("The LLM did not provide the sum_of_weights in the solution");
      // Reuse the sum of weights to get the result for the False CV case
      auto factor_false_result = LLM::pygx_result_t();
      factor_false_result.sum_of_weights = *factor_true_result.sum_of_weights;
      for( const auto& [cv_key, sum_of_weights] : *factor_false_result.sum_of_weights ){
        if( cv_key == "false" )
          factor_false_result.numerator = exp( sum_of_weights );
        factor_false_result.denominator += exp( sum_of_weights );
      }
      factor_false_result.prob = (factor_false_result.numerator / factor_false_result.denominator);
      if( debug ){
        std::cout << "Results for True cv:\n\tprob: " << factor_true_result.prob << "\n\tnumerator:" << factor_true_result.numerator
                  << "\n\tdemonimator: " << factor_true_result.denominator << "\n\tsum of weights:";
        for( const auto& [cv_key, sum_of_weights] : *factor_true_result.sum_of_weights ){
          std::cout << "\n\t\t" << cv_key << ":\t" << sum_of_weights;
        }
        std::cout << "\nResults for False cv:\n\tprob: " << factor_false_result.prob
                  << "\n\tnumerator:" << factor_false_result.numerator << "\n\tdemonimator: " << factor_false_result.denominator
                  << "\n\tsum of weights:";
        for( const auto& [cv_key, sum_of_weights] : *factor_false_result.sum_of_weights ){
          std::cout << "\n\t\t" << cv_key << ":\t" << sum_of_weights;
        }
        std::cout << std::endl;
      }

      // Compute the delta in the sum of weights that would be required for the Factor's most likely CV to change
      auto sum_of_weights_delta = fabs( factor_true_result.sum_of_weights.value()["true"]
                                        - factor_true_result.sum_of_weights.value()["false"] );
      if( debug )
        std::cout << "Computed the delta for the sum of weights: " << sum_of_weights_delta << std::endl;

      // Create an entry in the return structure for the current Factor; Store the Factor as a shared_ptr
      // Use a reference to the factor_substantial_features_t::substantial_features for legibility
      substantial_features.push_back( DCG::factor_substantial_features_t() );
      substantial_features.back().factor = p_factor;
      auto& r_factor_substantial_features = substantial_features.back().substantial_features;

      // Compute the impact of turning on/off each world dependent feature
      // If it is greater than sum_of_weights_delta, then the feature is substantial
      auto num_world_dependent_indices = world_dependent_indices["true"].feature_indices.size();
      for( size_t i = 0; i < num_world_dependent_indices; ++i ){
        const double true_weight = _llm->weights[world_dependent_indices["true"].feature_indices[i].weight];
        const double false_weight = _llm->weights[world_dependent_indices["false"].feature_indices[i].weight];
        const double feature_weight_delta = fabs( true_weight - false_weight );
        if( debug ){
          std::cout << "Feature weight delta: " << feature_weight_delta
                    << " {fabs( " << true_weight << " - " << false_weight << " )}" << std::endl;
        }
        // Store the feature indices for those that are substantial
        auto symbol_index = world_dependent_indices["true"].feature_indices[i].symbol();
        if( feature_weight_delta > sum_of_weights_delta ){
          if( debug ){
            std::cout << "Feature was substantial!";
            auto lang_index = world_dependent_indices["true"].feature_indices[i].language();
            std::cout << "\n\t" << *r_feature_set.constituent_feature_sets[1][lang_index];
            std::cout << "\n\t" << *r_feature_set.constituent_feature_sets[2][symbol_index] <<std::endl;
          }
          // Check if the world dependent constituente feature is already in the return structure
          auto it_factor_substantial_features = r_factor_substantial_features.find( symbol_index );
          if( it_factor_substantial_features == r_factor_substantial_features.end() ){
            bool success;
            std::tie( it_factor_substantial_features, success ) =
                r_factor_substantial_features.emplace( symbol_index, std::vector<FeaturePool::feature_indices_t>() );
          }
          it_factor_substantial_features->second.push_back( world_dependent_indices["true"].feature_indices[i] );
        }
      }
    } // End of Factors per FactorSet loop
  } // End of FactorSets loop

  return substantial_features;
}

///
/// Method to check whether the prior evaluation is still valid for a new world configuration
///
bool DCG::valid( const std::shared_ptr<World> new_world,
                  const vecSubstantialFeatures& substantial_features,
                  const bool debug )const
{
  // Enforce that the new world is composed of the same objects
  if( new_world->objects.size() != _world->objects.size() ){
    if( debug )
      std::cout << "The new world had a different number of objects, cannot verify solution is still valid." << std::endl;
    return false;
  }
  for( const auto& [object_uid, object] : new_world->objects ){
    if( _world->objects.find( object_uid ) == _world->objects.end() ){
      if( debug ){
        std::cout << "The new world had a new object with uid \"" << object_uid
                  << "\", cannot verify solution is still valid." << std::endl;
      }
      return false;
    }
  }
  // Duplicate the new world into a WorldDCG (sorts the objects at construction)
  auto new_world_dcg = std::make_shared< WorldDCG >( *new_world );

  // Check whether any substantial features change their value
  // Iterate through each factor (stored by the object symbol's uid string)
  for( const auto& factor_substantial_features : substantial_features ){
    if( debug ){
      std::cout << "Checking substantial features for the factor with symbol: "
                << *factor_substantial_features.factor->symbol << std::endl;
    }
    // Variable to easily access the factor ptr
    const auto p_factor = factor_substantial_features.factor;
    // Re-evaluate the symbol consituent feature at the constituent_feature_index for the current factor
    for( const auto& [constituent_feature_index, feature_indices] : factor_substantial_features.substantial_features ){
      auto& r_symbol_constituent_feature_set = p_factor->feature_set.constituent_feature_sets[2];
      // Store the original FeatureValue
      auto initial_feature_value = r_symbol_constituent_feature_set[constituent_feature_index]->value;
      // Re-evaluate the feature (only the symbol and world matters)
      auto new_feature_value = r_symbol_constituent_feature_set[constituent_feature_index]->evaluate(
                                            std::make_shared<std::string>("true"), p_factor->language_variable, new_world_dcg, p_factor->symbol );
      if( debug ){
        std::cout << "\tConstituent Feature: " << *r_symbol_constituent_feature_set[constituent_feature_index]
                  << "\n\tInitial Value: " << initial_feature_value << "\n\tNew Value: " << new_feature_value << std::endl;
      }
      // Compare the feature values
      if( initial_feature_value != new_feature_value ){
        if( debug )
          std::cout << "The feature evaluated differently in the new world - solution is not valid!" << std::endl;
        // Reset the feature to the initial feature value; Return false
        r_symbol_constituent_feature_set[constituent_feature_index]->value = initial_feature_value;
        return false;
      }
    }
  }
  return true;
}

///
/// Method to load from a file
///
bool DCG::from_file( const std::string& filename ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return from_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return from_json( filename );
  } else {
    std::cerr << "[DCG Class Error] Erroneous file suffix (\"" << filename << "\") in from_file";
    return false;
  }
}

///
/// Method to load from an XML file
///
bool DCG::from_xml( const std::string& filename ) {
  tinyxml2::XMLDocument document;
  document.LoadFile(filename.c_str());
  const tinyxml2::XMLElement *root = document.FirstChildElement("root");
  if (root != nullptr) {
    const tinyxml2::XMLElement *child = root->FirstChildElement();
    from_xml(child);
  }
  return document.ErrorID();
}

///
/// Method to load from a tinyxml2 element
///
bool DCG::from_xml( const tinyxml2::XMLElement* tmp ){
  return true;
}

///
/// Method to load from a json formatted file
///
bool DCG::from_json( const std::string& filename ){
  std::ifstream json_file( filename );
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_file, root );
  return from_json( root );
}

///
/// Method to load from a json formatted string
///
bool DCG::from_json_string( const std::string& value ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( value, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool DCG::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a file
///
void DCG::to_file( const std::string& filename )const {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return to_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return to_json( filename );
  } else {
    try {
      std::stringstream error_msg;
      error_msg << "[DCG Class Error] Erroneous file suffix (\"" << filename << "\") in to_file";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
  }
  return;
}

///
/// Method to write to an XML file
///
void DCG::to_xml( const std::string& filename )const {
  tinyxml2::XMLDocument document;
  tinyxml2::XMLElement * root = document.NewElement( "root" );
  document.InsertEndChild( root );
  to_xml( document, root );
  document.SaveFile( filename.c_str() );
  return;
}

///
/// Method to write to a tinyxml2 document
///
void DCG::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* tmp = document.NewElement( "d-c-g" );
  root->InsertEndChild( tmp );
}

///
/// Method to write to a json file
///
void DCG::to_json( const std::string& filename )const{
  std::ofstream outfile;
  outfile.open( filename );
  outfile << to_json();
  outfile.close();
  return;
}

///
/// Method to write to a json formatted string
///
std::string DCG::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void DCG::to_json( Json::Value& root )const{
  return;
}

///
/// DCG class ostream operator
///
std::ostream& operator<<(std::ostream& out, const DCG& other) {

  return out;
}

} // namespace h2sl
