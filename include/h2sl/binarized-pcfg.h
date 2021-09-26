/**
 * @file binarized-pcfg.h
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
#include <map>
#include <set>
#include <vector>
#include <memory>

#include "pcfg.h"

namespace h2sl{

/**
  Class to represent a binarized Probabilistic Context-Free Grammar (PCFG) for use
  by efficient, polynomial-time parsing algorithms (CKY). This class is designed
  purely for such a use case and therefore does not include many functions for
  general use.

  This is not a Chomsky Normal Form grammar, which would include elimination of unary
  production rules. This elimination step is not necessary for parsing and makes
  transformation between parse trees more complex.

  Therefore, the transformation step from a PCFG to a Binarized-PCFG is just the
  conversion from n-ary rules to unary or binary rules via the introduction of
  intermediate production rules (x production rules). Note that the probability of
  each x production rule is 1.
  
  Consider an n-ary rule where A,B,C,D are all nonterminals:
    A -> B C D

  The binarization step introduces the x production rule:
    X -> C D
  allowing:
    A -> B X


  This class contains the following:
    initial_pcfg: the initial PCFG below transformation to a binarized version.

    terminals: a set containing the terminals T (text of words)

    preterminals: a set containing the preterminals C (POS tags)

    nonterminals: a set containing the nonterminals N (phrase-types & binarized rules)

    lexical_rules: a container to hold the rules for producing words from POS tags;
                    this container also holds the probabilities for each rule.

    unary_production_rules: a container to hold the rules for producing a phrase type
                            from a single nonterminal or preterminal symbol. This
                            container also holds the probabilities of the rules.

    binary_production_rules: a container to hold the rules for producing phrase types
                          from a pair of nonterminal or preterminal symbols. It does
                          not contain the x production rules, but a rhs nonterminal
                          can be of the type introduced to enforce binary rules. It
                          holds the probabilities of the rules.

    x_production_rules: a container to hold the rules introduced to convert n-ary
                        rules to binary rules. Each rule of this type has a
                        probability of 1.
**/

class BinarizedPCFG{
public:

  /**
    A structure to contain the constituents of a unary rule. Both lexical rules and
    production rules can be unary. A unary production rule is of the form X -> Y,
    where X is a nonterminal and Y is a nonterminal or preterminal. A unary lexical
    rule (all lexical rules are unary) is of the form W -> w where W is a preterminal
    and w is a terminal.

    lhs: the single symbol lhs of the rule
    rhs: the single symbol rhs of the rule, type depends on the rule type
    prob: the probability of the rule
    freq: the frequency of occurrence of the rule
  **/
  struct unary_rule_t{
    std::string lhs;
    std::string rhs;
    double prob;
    int freq;
  };

  /**
    A structure to contain the constituents of a binary rule. Only production rules and
    x production rules can be binary. A binary rule is of the form X -> Y Z where X is
    a nonterminal and Y Z are a pair of nonterminal or preterminal symbols.

    lhs: the single nonterminal symbol lhs of the rule
    rhs: the pair of symbols on the rhs of the rule
    prob: the probability of the rule
    freq: the frequency of occurence of the rule
  **/
  struct binary_rule_t{
    std::string lhs;
    std::pair< std::string, std::string > rhs;
    double prob;
    int freq;
  };

  /**
    Type alias for a container of unary rules. This is used both for the lexical rules
    and the unary production rules. It is a map consisting of the rules organized by
    their lhs symbol. The map is ordered to allow for alphabetical sorting.

    key: the lhs symbol
    value: a vector of unary_rule_t objects where the lhs matches the key
  **/
  using mapVectorUnaryRules = std::map<
                                std::string,
                                std::vector< unary_rule_t >
                              >;

  /**
    Type alias for a container of binary rules. This is used for the binary
    production rules. It is a map consisting of the rules organized by the lhs symbol.
    The map is ordered to allow for alphabetical sorting.

    key: the lhs symbol (nonterminal)
    value: a vector of binary_rule_t objects where the lhs matches the key
  **/
  using mapVectorBinaryRules = std::map<
                                  std::string,
                                  std::vector< binary_rule_t >
                                >;

  /**
    Type alias for a container of binary rules. This is used for the binary
    x production rules. It is a map consisting of the rules organized by the lhs
    symbol.

    key: the lhs symbol (nonterminal)
    value: a single binary_rule_t object (each lhs has only one rule)
  **/
  using mapBinaryRules = std::map< std::string, binary_rule_t >;

  /**
    Default constructor for h2sl::BinarizedPCFG class
  **/
  BinarizedPCFG() = default;

  /**
    Default copy constructor for h2sl::PCFG class
  **/
  BinarizedPCFG( const BinarizedPCFG& ) = default;

  /**
    Default move constructor for h2sl::BinarizedPCFG class
  **/
  BinarizedPCFG( BinarizedPCFG&& ) = default;

  /**
    Parameter constructor for h2sl::BinarizedPCFG class from an h2sl::PCFG object

    @brief Construct a BinarizedPCFG from an h2sl::PCFG
    @param[in]    pcfgArg     PCFG object from which to construct a binarized version
    @returns                  none
    @throws                   Throws runtime error on conversion error
  **/
  explicit BinarizedPCFG( const h2sl::PCFG& pcfgArg );

  /**
    Default destructor for h2sl::BinarizedPCFG class
  **/
  ~BinarizedPCFG() = default;

  /**
    Default copy assignment operator for h2sl::BinarizedPCFG class
  **/
  BinarizedPCFG& operator=( const BinarizedPCFG& ) = default;

  /**
    Default move assignment operator for h2sl::BinarizedPCFG class
  **/
  BinarizedPCFG& operator=( BinarizedPCFG&& ) = default;

  /**
    This method determines the size of the current lexicon by counting each
    lexical_rule. The size of the lexicon for a BinarizedPCFG is the same as the associated
    PCFG.

    @brief Determine the number of rules in the current lexicon
    @returns      size      The size of the current lexicon (int)
    @throws                 no expected throws
  **/
  int lexicon_size( void )const;

  /**
    This method provides read-only access to the initial_pcfg member data
  **/
  inline const h2sl::PCFG&
  get_initial_pcfg( void )const{ return initial_pcfg; }

  /**
    This method provides read-only access to the terminals member data
  **/
  inline const std::set< std::string >&
  get_terminals( void )const{ return terminals; }

  /**
    This method provides read-only access to the preterminals member data
  **/
  inline const std::set< std::string >&
  get_preterminals( void )const{ return preterminals; }

  /**
    This method provides read-only access to the nonterminals member data
  **/
  inline const std::set< std::string >&
  get_nonterminals( void )const{ return nonterminals; }

  /**
    This method provides read-only access to the lexical_rules member data
  **/
  inline const mapVectorUnaryRules&
  get_lexical_rules( void )const{ return lexical_rules; }

  /**
    This method provides read-only access to the unary_production_rules member data
  **/
  inline const mapVectorUnaryRules&
  get_unary_production_rules( void )const{ return unary_production_rules; }

  /**
    This method provides read-only access to the binary_production_rules member data
  **/
  inline const mapVectorBinaryRules&
  get_binary_production_rules( void )const{ return binary_production_rules; }

  /**
    This method provides read-only access to the x_production_rules member data
  **/
  inline const mapBinaryRules&
  get_x_production_rules( void )const{ return x_production_rules; }

  /**
    BinarizedPCFG ostream operator. Prints the BinarizedPCFG to the ostream

    @brief BinarizedPCFG ostream operator
    @param[in]    out     ostream operator class reference
    @param[in]    other   BinarizedPCFG reference to be printed to ostream
    @returns              reference to modified ostream object
    @throws               no expected throws
  **/
  // TODO: Commented out until actually implemented
  //friend std::ostream& operator<<( std::ostream& out, const BinarizedPCFG& other );

  /** Constant to define the class preamble for exception handling */
  static const char* BinarizedPCFGPreamble;

protected:
private:

  /**
    This method converts a PCFG's production rules to unary and binary rules by
    eliminating n-ary rules and introducing x production rules. Note that the
    any nonterminals introduced via x production rules will be added to the
    nonterminals member data.

    @brief Convert n-ary production rules to unary and binary rules
    @param[in]    production_rules    PCFG::mapVectorProductionRules to be converted
    @returns                          none
    @throws                           No throws expected
  **/
  void _binarize_production_rules(
    const h2sl::PCFG::mapVectorProductionRules& production_rules
  );

  /**
    This method checks the enforcement of BinarizedPCFG class invariants by checking:
    1. No rules are repeated
    2. The probability constraints for PCFGs hold

    @brief Enforce BinarizedPCFG invariants
    @returns              none
    @throws               Throws runtime error if there are violations
  **/
  void _check_invariants( void )const;

  /********************************************************************************/
  // The data members below are private in order to enforce class invariants.

  /**
    The original PCFG used for transformation into a CNF versions.
  **/
  h2sl::PCFG initial_pcfg;

  /**
    A set of terminal symbols (a string of a word's text). This is ordered to allow
    for alphabetical sorting of the symbols.
  **/
  std::set< std::string > terminals;

  /**
    A set of preterminal symbols (POS tags). This is ordered to allow for alphabetical
    sorting of the symbols.
  **/
  std::set< std::string > preterminals;

  /**
    A set of nonterminal symbols (phrase types and x nonterminals). This is ordered to
    allow for alphabetical sorting of the symbols.
  **/
  std::set< std::string > nonterminals;

  /**
    A container for the lexical rules and their associated probabilities. These rules
    produce a terminal (word) from a preterminal (POS tag). This should exactly match
    lexical_rules of initial_pcfg.
  **/
  mapVectorUnaryRules lexical_rules;

  /**
    A container for the unary production rules and their associated probabilities.
    These rules produce a nonterminal or preterminal from a nonterminal.
  **/
  mapVectorUnaryRules unary_production_rules;

  /**
    A container for the binary production rules and their associated probabilities.
    These rules produce a pair of nonterminals/preterminals from a nonterminal.
  **/
  mapVectorBinaryRules binary_production_rules;

  /**
    A container for the x production rules (binary). All x production rules have a
    probability of 1. 
  **/
  mapBinaryRules x_production_rules;

}; // class BinarizedPCFG

} // namespace h2sl
