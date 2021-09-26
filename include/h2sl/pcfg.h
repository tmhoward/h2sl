/**
 * @file pcfg.h
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

#include <tinyxml2.h>

#include "h2sl/sentence.h"

namespace h2sl{

/**
  Class to represent a Probabilistic Context-Free Grammar (PCFG).

  A PCFG is typically represented as a 4-tuple, G = (T,N,S,R), where:

    G: the grammar; it is stated that a grammar generates a language L
    T: a set of terminal symbols (e.g. text of words)
    N: a set of nonterminal symbols (e.g. part-of-speech tags or phrase-types)
    S: a special start nonterminal symbol
    R: a set of rules/productions X -> y
        X is a nonterminal symbol
        y is a string that can consist of symbols from T and/or N


  However, in practice it is useful to modify this representation by introducing a 
    lexicon that makes explicit a distinction between nonterminal part-of-speech
    symbols and nonterminal phrase-type symbols. So, the representation of a PCFG used
    by this class is a 7-tuple, G = (T,C,N,S,L,R,P), where:

    G: the pcfg; again, this is a "generative" grammar that generates a language L
    T: a set of terminal symbols (text of words)
    C: a set of preterminal symbols (part-of-speech tags (POS) / lexical categories)
    N: a set of nonterminal symbols (phrase-types)
    S: a special start symbol (often used to indicate a sentence)
    L: a lexicon; this is a set of rules of the form:
        W -> w
        W is an element of C
        w is an element of T
    R: a set of production rules of the form:
        X -> y
        X is an element of N
        y is a string that can consist of symbols from C and/or N
    P: a probability function that assigns a probability to the element of L and R
        P: R -> [0,1], L -> [0,1]
        Constraint: the sum of probabilities for a rule with a given lhs is 1
        Constraint: the sum of the probabilities of all generated sentences is 1

    Notes:
    P(t) - The probability of a tree t is the product of the probabilities of the rules
            used to generate it.
    P(s) - The probability of a string s is the sum of the probabilities of the trees
            that yield s.


  This class contains the following:
    terminals: a set containing the terminals T (text of words)

    preterminals: a set containing the preterminals C (POS tags)

    nonterminals: a set containing the nonterminals N (phrase-types)

    lexical_rules: a container to hold the rules for producing words from POS tags;
                    this container also holds the probabilities for each rule.

    production_rules: a container to hold the rules for producing phrase types from
                        a sequence of phrase types and POS tags; this container also
                        holds the probabilities for each rule.
  
**/

class PCFG{
public:
  /** Type alias for a LanguageVariable */
  using Phrase = LanguageVariable;

  /**
    A structure to contain the constituents of a lexical rule. Lexical rules are of the
    form W -> w, where W is a preterminal (POS tag) and w is a terminal (word text).

    lhs: the preterminal symbol W
    rhs: the terminal symbol w
    prob: the probability of the rule
    freq: the frequency of occurrence of the rule
  **/
  struct lexical_rule_t{
    std::string lhs;
    std::string rhs;
    double prob;
    int freq;
  };

  /**
    A structure to contain the constituents of a production rule. Production rules are
    of the form X -> y, where X is a single nonterminal (phrase-type) and y is an n-ary
    sequence of nonterminal and preterminal symbols.

    lhs: the nonterminal symbol X
    rhs: a variable length vector of nonterminal and preterminal symbols
    prob: the probability of the rule
    freq: the frequency of occurrence of the rule
  **/
  struct production_rule_t{
    std::string lhs;
    std::vector< std::string > rhs;
    double prob;
    int freq;
  };

  /**
    Type alias for a container of lexical rules. It is a map consisting of the rules
    organized by their lhs preterminal symbols. The map is ordered to allow for
    alphabetical sorting.

    key: the lhs preterminal symbol
    value: a vector of lexical_rule_t objects where the lhs matches the key
  **/
  using mapVectorLexicalRules = std::map<
                                  std::string,
                                  std::vector< lexical_rule_t >
                                >;

  /**
    Type alias for a container of production rules. It is a map consisting of the rules
    organized by their lhs nonterminal symbols. The map is ordered to allow for
    alphabetical sorting.

    key: the lhs nonterminal symbol
    value: a vector of production_rule_t objects where the lhs matches the key
  **/
  using mapVectorProductionRules = std::map<
                                    std::string,
                                    std::vector< production_rule_t >
                                   >;

  /**
    Default constructor for h2sl::PCFG class
  **/
  PCFG() = default;

  /**
    Default copy constructor for h2sl::PCFG class
  **/
  PCFG( const PCFG& ) = default;

  /**
    Default move constructor for h2sl::PCFG class
  **/
  PCFG( PCFG&& ) = default;

  /**
    Default destructor for h2sl::PCFG class
  **/
  ~PCFG() = default;

  /**
    Default copy assignment operator for h2sl::PCFG class
  **/
  PCFG& operator=( const PCFG& ) = default;

  /**
    Default move assignment operator for h2sl::PCFG class
  **/
  PCFG& operator=( PCFG&& ) = default;

  /**
    This method imports a Probabilistic Context-Free Grammar from a file.

    @brief Import a PCFG from a file.
    @param[in]    filename    file from which to import
    @returns                  none
    @throws                   Throws runtime_error on parsing error
  **/
  void from_file( const char* filename );

  /**
    This method imports a Probabilistic Context-Free Grammar from an XML file.

    @brief Import a PCFG from an XML file.
    @overload
    @param[in]    filename    filename of the XML file from which to import
    @returns                  none
    @throws                   Throws runtime_error on XML parsing error
  **/
  void from_xml( const char* filename );

  /**
    This method imports a Probabilistic Context-Free Grammar from an XMLElement*.

    @brief Import a PCFG from a tinyxml2 XMLElement*
    @overload
    @param[in]    pcfg_element    current XMLElement* to a pcfg element
    @returns                      none
    @throws                       Throws runtime_error on XML parsing error
  **/
  void from_xml( const tinyxml2::XMLElement* pcfg_element );

  /**
    This method exports a Probabilistic Context-Free Grammar to an XML file

    @brief Export a PCFG to an XML file
    @overload
    @param[in]    filename    filename of the XML to which the pcfg is exported
    @returns                  none
    @throws                   Throws runtime_error on XML of File write error
  **/
  void to_xml( const char* filename)const;

  /**
    This method export a Probabilistic Context-Free Grammar to an XMLDocument object.
    The root XMLElement* takes the current pcfg as a child XMLELement.

    @brief Export a PCFG to an XMLDocument object
    @param[in]    doc   XMLDocument object for the current document
    @param[in]    root  XMLElement* to accept the PCFG as a child
    @returns            none
    @throws             Throws runtime_error on XML write error
  **/
  void to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root )const;

  /**
    This method determines the size of the current lexicon by counting each
    lexical_rule. Thus, terminal produced from multiple preterminals will be counted
    multiple times (i.e. a word can be take on different lexical categories).

    @brief Determine the number of rules in the current lexicon
    @returns    size    The size of the current lexicon (int)
    @throws             no expected throws
  **/
  int lexicon_size( void )const;

  /**
    This method appends to a pcfg by scraping components from h2sl::Sentences.

    @brief Populate a grammar by scraping h2sl::Sentences
    @param[in]    sentences   std::vector of h2sl::Sentence to be scraped
    @returns                  none
    @throws                   Throws runtime_error on scraping error
  **/
  void
  scrape_sentences( const std::vector<std::shared_ptr<h2sl::Sentence>>& sentences );

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
  inline const mapVectorLexicalRules&
  get_lexical_rules( void )const{ return lexical_rules; }

  /**
    This method provides read-only access to the production_rules member data
  **/
  inline const mapVectorProductionRules&
  get_production_rules( void )const{ return production_rules; }
  

  /**
    PCFG ostream operator. Prints the PCFG to the ostream

    @brief PCFG ostream operator
    @param[in]    out     ostream operator class reference
    @param[in]    other   PCFG reference to be printed to ostream
    @returns              reference to modified ostream object
    @throws               no expected throws
  **/
  // TODO: Commented out until actually implemented
  //friend std::ostream& operator<<( std::ostream& out, const PCFG& other );

  /** Constant to define the class preamble for exception handling */
  static const char* PCFGPreamble;

  /** Constant to define the string used in XMLElement Name for PCFG */
  static const char* XMLPCFGName;

  /** Constant to define the string used in XMLElement Name for terminals */
  static const char* XMLTerminalsName;

  /** Constant to define the string used in XMLElement Name for preterminals */
  static const char* XMLPreTerminalsName;

  /** Constant to define the string used in XMLElement Name for nonterminals */
  static const char* XMLNonTerminalsName;

  /** Constant to define the string used in XMLAttribute Name for symbols */
  static const char* XMLSymbolsAttributeName;

  /** Constant to define the string used in XMLElement Name for lexical rules */
  static const char* XMLLexicalRulesName;

  /** Constant to define the string used in XMLElement Name for a lexical rule */
  static const char* XMLLexicalRuleName;

  /** Constant to define the string used in XMLElement Name for production rules */
  static const char* XMLProductionRulesName;

  /** Constant to define the string used in XMLElement Name for a production rule */
  static const char* XMLProductionRuleName;

  /** Constant to define the string used in XMLAttribute Name for lhs of rules */
  static const char* XMLRuleLHSAttributeName;

  /** Constant to define the string used in XMLAttribute Name for rhs of rules */
  static const char* XMLRuleRHSAttributeName;

  /** Constant to define the string used in XMLAttribute Name for a rule probability */
  static const char* XMLRuleProbAttributeName;

  /** Constant to define the string used in XMLAttribute Name for a rule frequency */
  static const char* XMLRuleFreqAttributeName;

protected:

private:

  /**
    This method appends to a pcfg by scraping components from an Phrase.

    @brief Append to a grammar by scraping the components from an Phrase
    @param[in]    phrase    Phrase to be scraped for grammar components
    @returns                none
    @throws                 Throws a runtime error if there are no rhs symbols
  **/
  void _scrape_phrase( const std::shared_ptr<Phrase>& phrase );

  /**
    This method constructs a lexical rule from an h2sl::Word. It assigns a probability
    of 1.0 and a frequency of 1.

    @brief Construct a lexical_rule_t object from an h2sl::Word
    @param[in]    word      h2sl::Word from which to construct the lexical_rule_t
    @returns                a lexical_rule_t object
    @throws                 no expected throws
  **/
  lexical_rule_t _make_lexical_rule_from_word( const h2sl::Word& word )const;

  /**
    This method constructs a production rule from an Phrase. It assigns a
    probability of 1.0 and a frequency of 1.

    @brief Construct a production_rule_t object from an Phrase
    @param[in]    phrase    Phrase from which to construct the production_rule_t
    @returns                a production_rule_t object
    @throws                 no expected throws
  **/
  production_rule_t _make_production_rule_from_phrase(const Phrase& phrase)const;

  /**
    This method gets the minimum time of words associated with an input phrase.

    @brief Get the minimum time of all words in a phrase
    @param[in]    phrase    Phrase from which to find the minimum time
    @returns                the minimum time (double)
    @throws                 no expected throws
  **/
  double _get_phrase_min_time( const Phrase& phrase )const;

  /**
    This method assign the probabilities for both lexical and production rules. It
    assumes that a treebank has been entirely parsed for rules and their frequenceies.

    @brief Assign probabilities to both lexical_rules and production_rules
    @returns                none
    @throws                 Throws a runtime error for missing elements of a rule
  **/
  void _assign_rule_probabilities( void );

  /**
    This method checks the enforcement of PCFG class invariants by checking:
    1. No rules are repeated
    2. The probability constraints listed at the head of this file

    @brief Enforce PCFG invariants
    @returns                none
    @throws                 Throws a runtime error if there are violations
  **/
  void _check_invariants( void )const;

  /********************************************************************************/
  // The data members below are private in order to enforce class invariants.

  /**
    A set of terminal symbols (a string of a word's text). This is ordered to allow
    for alphabetical sorting of the symbols.
  **/
  std::set< std::string > terminals = std::set< std::string >();

  /**
    A set of preterminal symbols (POS tag). This is ordered to allow for alphabetical
    sorting of the symbols.
  **/
  std::set< std::string > preterminals = std::set< std::string >();

  /**
    A set of nonterminal symbols (phrase-types). This is ordered to allow for
    alphabetical sorting of the symbols.
  **/
  std::set< std::string > nonterminals = std::set< std::string >();

  /**
    A container for the lexical rules and their associated probabilities. These rules
    produce a terminal (word) from a preterminal (POS tag).
  **/
  mapVectorLexicalRules lexical_rules = mapVectorLexicalRules();

  /**
    A container for the production rules and their associated probabilities. These
    rules produce a nonterminal (phrase-type) from a sequence of nonterminal and 
    preterminal (POS tag) symbols.
  **/
  mapVectorProductionRules production_rules = mapVectorProductionRules();

}; // class PCFG

} // namespace h2sl
