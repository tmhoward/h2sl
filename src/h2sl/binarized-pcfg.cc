/**
 * @file binarized-pcfg.cc
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

#include <math.h>
#include <float.h>
#include <sstream>

#include "h2sl/binarized-pcfg.h"

namespace h2sl{

///
/// BinarizedPCFG constructor from PCFG object
///
BinarizedPCFG::BinarizedPCFG( const h2sl::PCFG& pcfgArg ){
  try{
    std::stringstream error_msg;
    // Store pcfgArg as initial_pcfg
    initial_pcfg = pcfgArg;

    // Copy the terminals, preterminals, nonterminals
    terminals = pcfgArg.get_terminals();
    preterminals = pcfgArg.get_preterminals();
    nonterminals = pcfgArg.get_nonterminals();

    // Convert the lexical rules from PCFG::lexical_rule_t to BinarizedPCFG::unary_rule_t
    for( const auto& v_pcfg_lexical_rules : pcfgArg.get_lexical_rules() ){
      std::vector< unary_rule_t > v_lexical_rules;
      auto ret_lexical_rule = lexical_rules.insert( std::make_pair( 
                                                    v_pcfg_lexical_rules.first,
                                                    v_lexical_rules ) );
      if( !ret_lexical_rule.second ){
        error_msg << "Unable to insert a new vector of lexical rules for lhs \""
                  << v_pcfg_lexical_rules.first << "\"";
        throw error_msg.str();
      }
      for( const auto& pcfg_lexical_rule : v_pcfg_lexical_rules.second ){
        unary_rule_t lexical_rule;
        lexical_rule.lhs = pcfg_lexical_rule.lhs;
        lexical_rule.rhs = pcfg_lexical_rule.rhs;
        lexical_rule.prob = pcfg_lexical_rule.prob;
        lexical_rule.freq = pcfg_lexical_rule.freq;
        ret_lexical_rule.first->second.push_back( lexical_rule );
      }
    }

    // Convert the production rules to unary/binary rules
    _binarize_production_rules( pcfgArg.get_production_rules() );
    
    // Check that the class invariants are satisfied
    _check_invariants();
  }
  catch( std::string msg ){
    // Prepend the class preamble
    msg.insert(0, BinarizedPCFGPreamble);
    throw std::runtime_error( msg );
  }
  return;
}

///
/// Method to determine the size of the lexicon
///
int BinarizedPCFG::lexicon_size( void )const{
  return initial_pcfg.lexicon_size();
}

///
/// Method to convert n-ary production rules to unary and binary rules
/// Update nonterminals member data with new x production rules lhs symbols
///
void BinarizedPCFG::_binarize_production_rules(
  const h2sl::PCFG::mapVectorProductionRules& production_rules )
{
  std::stringstream error_msg;
  // If the rhs is of size 1, add it unmodified to unary_production_rules
  // If the rhs is of size 2, add it unmodified to binary_production_rules
  // If the rhs is of size >2, create x production rules
  //    the probability of each x production rule is necessarily 1.0
  //    the probability of the terminating binary production of the chain is the
  //      probability of the original rule from production_rules
  for( const auto& v_production_rules : production_rules ){
    for( const auto& production_rule : v_production_rules.second ){
      if( production_rule.rhs.size() == 0 ){
        error_msg << "Encountered a production rule with an empty rhs with lhs \""
                  << production_rule.lhs << "\"";
        throw error_msg.str();
      }
      // Handle unary production rules
      if( production_rule.rhs.size() == 1 ){
        // Get vector of unary rules with the lhs symbol
        std::vector< unary_rule_t > v_unary_rules;
        auto ret_unary_production_rules = unary_production_rules.insert(std::make_pair(
                                                 production_rule.lhs, v_unary_rules ));
        // Copy production_rule to a unary_rule_t
        unary_rule_t unary_production_rule;
        unary_production_rule.lhs = production_rule.lhs;
        unary_production_rule.rhs = production_rule.rhs[0];
        unary_production_rule.prob = production_rule.prob;
        unary_production_rule.freq = production_rule.freq;
        // Push the new unary rule onto the vector
        ret_unary_production_rules.first->second.push_back( unary_production_rule );
        continue;
      }
      // Handle n-ary production rules with n > 2
      std::vector< std::string > rhs_symbols = production_rule.rhs;
      while( rhs_symbols.size() > 2 ){
        // Create the rhs for x production rule; Pop the 2 rightmost symbols off
        std::pair<std::string, std::string > x_rhs;
        x_rhs.second = rhs_symbols.back(); rhs_symbols.pop_back();
        x_rhs.first = rhs_symbols.back(); rhs_symbols.pop_back();
        // Check if an existing x production rule has this rhs pair
        bool found_match = false;
        for( auto& x_production_rule : x_production_rules ){
          if( x_rhs == x_production_rule.second.rhs ){
            found_match = true;
            x_production_rule.second.freq += production_rule.freq;
            rhs_symbols.push_back( x_production_rule.first );
            break;
          }
        }
        if( found_match )
          continue;
        // Create X_(N) where N is the current number of x production rules
        binary_rule_t x_production_rule;
        x_production_rule.lhs = "X";
        auto x_index_string = std::to_string( x_production_rules.size() );
        x_production_rule.lhs.append( x_index_string );
        x_production_rule.rhs = x_rhs;
        x_production_rule.prob = 1.0;
        x_production_rule.freq = production_rule.freq;
        x_production_rules.insert( std::make_pair( x_production_rule.lhs,
                                                   x_production_rule ) );
        rhs_symbols.push_back( x_production_rule.lhs );
      }

      // Handle binary production rules
      if( rhs_symbols.size() != 2 ){
        error_msg << "Expected rhs_symbols to have 2 symbols";
        throw error_msg.str();
      }
      // Get vector of binary rules with the lhs symbol
      std::vector< binary_rule_t > v_binary_rules;
      auto ret_binary_production_rules = binary_production_rules.insert(
                       std::make_pair( production_rule.lhs, v_binary_rules ) );
      // Copy production_rule to a binary_rule_t; Use rhs_symbols for rhs
      binary_rule_t binary_production_rule;
      binary_production_rule.lhs = production_rule.lhs;
      binary_production_rule.rhs.first = rhs_symbols[0];
      binary_production_rule.rhs.second = rhs_symbols[1];
      binary_production_rule.prob = production_rule.prob;
      binary_production_rule.freq = production_rule.freq;
      // Push the new binary rule onto the vector
      ret_binary_production_rules.first->second.push_back( binary_production_rule );
    }
  }
  return;
}

///
/// Method to check the enforcement of BinarizedPCFG class invariants
///   1. No rules are repeated
///   2. The probabilities are appropriately constrained
void BinarizedPCFG::_check_invariants( void )const{
  std::stringstream error_msg;

  /********************************************************************************/
  /**************************** Check for redundant rules *************************/
  /********************************************************************************/
  // Check for repeated lexical_rules
  for( const auto& v_lexical_rules : lexical_rules ){
    for( size_t i = 0; i < v_lexical_rules.second.size(); i++ ){
      for( size_t j = i+1; j < v_lexical_rules.second.size(); j++ ){
        if( v_lexical_rules.second[i].rhs == v_lexical_rules.second[j].rhs ){
          error_msg << "Found redundant lexical rule with lhs \""
                    << v_lexical_rules.second[i].lhs << "\" and rhs \""
                    << v_lexical_rules.second[i].rhs << "\"";
          throw error_msg.str();
        }
      }
    }
  }
  // Check for repeated unary_production_rules
  for( const auto& v_unary_production_rules : unary_production_rules ){
    for( size_t i = 0; i < v_unary_production_rules.second.size(); i++ ){
      for( size_t j = i+1; j < v_unary_production_rules.second.size(); j++ ){
        if( v_unary_production_rules.second[i].rhs
            == v_unary_production_rules.second[j].rhs )
        {
          error_msg << "Found redundant unary production rule with lhs \""
                    << v_unary_production_rules.second[i].lhs << "\" and rhs \""
                    << v_unary_production_rules.second[i].rhs << "\"";
          throw error_msg.str();
        }
      }
    }
  }
  // Check for repeated binary_production_rules
  for( const auto& v_binary_production_rules : binary_production_rules ){
    for( size_t i = 0; i < v_binary_production_rules.second.size(); i++ ){
      for( size_t j = i+1; j < v_binary_production_rules.second.size(); j++ ){
        if( v_binary_production_rules.second[i].rhs
            == v_binary_production_rules.second[j].rhs )
        {
          error_msg << "Found redundant binary production rule with lhs \""
                    << v_binary_production_rules.second[i].lhs << "\" and rhs \""
                    << v_binary_production_rules.second[i].rhs.first << " "
                    << v_binary_production_rules.second[i].rhs.second << "\"";
          throw error_msg.str();
        }
      }
    }
  }
  // Check that no x_production_rules have the same rhs
  for( auto it1_x_production_rules = x_production_rules.begin();
        it1_x_production_rules != x_production_rules.end();
        ++it1_x_production_rules )
  {
    for( auto it2_x_production_rules = std::next( it1_x_production_rules );
          it2_x_production_rules != x_production_rules.end();
          ++it2_x_production_rules )
    {
      if( it2_x_production_rules->second.rhs == it1_x_production_rules->second.rhs ){
        error_msg << "Found x production rules (\"" << it1_x_production_rules->first
                  << "\", \"" << it2_x_production_rules->first << "\")"
                  << " with the same rhs \""
                  << it1_x_production_rules->second.rhs.first << " "
                  << it1_x_production_rules->second.rhs.second << "\"";
        throw error_msg.str();
      }
    }
  }
 

  /********************************************************************************/
  /************** Check that probabilities for a given lhs sum to 1.0  ************/
  /********************************************************************************/
  int epsilon = 5;
  // Check the probabilities constraint for lexical_rule
  for( const auto& v_lexical_rules : lexical_rules ){
    double sum = 0.0;
    for( const auto& lexical_rule : v_lexical_rules.second ){
      sum += lexical_rule.prob;
    }
    if( !(fabs(1.0 - sum) < (epsilon * DBL_EPSILON * fabs(1.0 + sum))) ){
      error_msg << "The sum of probabilities for lexical rules with lhs \""
                << v_lexical_rules.first << "\" did NOT sum to 1.0.";

      throw error_msg.str();
    }
  }
  // Check the probabilities constraint for x_production_rules
  for( const auto& x_production_rule : x_production_rules ){
    double prob = x_production_rule.second.prob;
    if( !(fabs(1.0 - prob) < (epsilon * DBL_EPSILON * fabs(1.0 + prob))) ){
      error_msg << "The x_production_rule for lhs \"" << x_production_rule.first
                << "\" has probability " << std::to_string(prob)
                << " but should have probability 1.0";
      throw error_msg.str();
    }
  }
  // Check the probabilities constraint for the production rules
  // Note that a lhs symbol prob needs to be summed for both unary and binary rules
  // If lhs symbol has both unary and binary rules, this implementation checks it twice
  //  as an artifact of ensuring that unique unary or binary lhs symbols are not missed
  for( const auto& v_unary_production_rules : unary_production_rules ){
    double sum = 0.0;
    // Add the unary production rules for the current lhs
    for( const auto& unary_production_rule : v_unary_production_rules.second ){
      sum += unary_production_rule.prob;
    }
    // If there are binary production rules for the current lhs, add them
    auto it_binary_production_rules =
                      binary_production_rules.find( v_unary_production_rules.first );
    if( it_binary_production_rules != binary_production_rules.end() ){
      for( const auto& binary_production_rule : it_binary_production_rules->second ){
        sum += binary_production_rule.prob;
      }
    }
    if( !(fabs(1.0 - sum) < (epsilon * DBL_EPSILON * fabs(1.0 + sum))) ){
      error_msg << "The sum of probabilities for production rules with the lhs \""
                << v_unary_production_rules.first << "\" did NOT sum to 1.0.";
      throw error_msg.str();
    }
  }
  for( const auto& v_binary_production_rules : binary_production_rules ){
    double sum = 0.0;
    // Add the binary production rules for the current lhs
    for( const auto& binary_production_rule : v_binary_production_rules.second ){
      sum += binary_production_rule.prob;
    }
    // If there are unary production rules for the current lhs, add them
    auto it_unary_production_rules =
                      unary_production_rules.find( v_binary_production_rules.first );
    if( it_unary_production_rules != unary_production_rules.end() ){
      for( const auto& unary_production_rule : it_unary_production_rules->second ){
        sum += unary_production_rule.prob;
      }
    }
    if( !(fabs(1.0 - sum) < (epsilon * DBL_EPSILON * fabs(1.0 + sum))) ){
      error_msg << "The sum of probabilities for production rules with the lhs \""
                << v_binary_production_rules.first << "\" did NOT sum to 1.0.";
      throw error_msg.str();
    }
  }
  return;
}

///
/// BinarizedPCFG class ostream operator
///
/*
std::ostream& operator<<( std::ostream& out, const BinarizedPCFG& other ){
  out << "BinarizedPCFG( ";
  // TODO
  out << ")";
  return out;
}
*/

///
/// Constant used for exception preambles
///
const char* BinarizedPCFG::
BinarizedPCFGPreamble = "[BinarizedPCFG Class Error] ";

} // namespace h2sl
