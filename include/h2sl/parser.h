/**
 * @file parser.h
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
#include <vector>

#include "h2sl/language-variable.h"
#include "h2sl/pcfg.h"
#include "h2sl/binarized-pcfg.h"

namespace h2sl{

/**
  Class to represent a constituency parser that utilizes a Binarized Probabilistic
  Context Free Grammar (only unary or binary production rules). The parsing algorithm
  is a modified probabilistic Cocke-Kasimi-Younger algorithm that allows unary rules.

  This class is constructed from an initial non-binarized PCFG that is maintained as
  a member of the class. During construction, the class also constructs a binarized
  version of the grammar.
**/

class Parser{
public:
  /** Type alias for a LanguageVariable */
  using Phrase = LanguageVariable;

  /** Enum type to specify the kind of chart element; useful for associated logic **/
  enum class ChartElementType{ Word, UnaryPhrase, BinaryPhrase, XPhrase };

  /**
    A Structure to contain the contents of a single producable constituent in the parse
    chart. It includes back pointers to the rule constituents B and C used to produce
    the symbol A. It also includes the probability of the current symbol A:
      Binary rule: P(A) = P(A -> B C) * P(B) * P(C)
      Unary rule: P(A) = P(A -> B) * P(B)


    prob: the probability of the parse up to the current symbol
    type: the type of the chart element
    symbol: the produced symbol for this part of the chart
    word: the word from which the chart element was produced (only chart elements
            produced via lexicl rules will have this as not empty). Also includes the
            order of the word in the input (sentence).
    bp_1: the back pointer for the first symbol of the rule used to generate symbol
    bp_2: the back pointer for the second symbol of the rule used to generate symbol
  **/
  struct chart_element_t{
    double prob;
    ChartElementType type;
    std::string symbol;
    std::pair< std::string, size_t > word;
    std::shared_ptr< chart_element_t > bp_1 = nullptr;
    std::shared_ptr< chart_element_t > bp_2 = nullptr;
  };

  /**
    Structure to contain the two chart cells that can provide constituents for the rhs
    of a rule.

    first_cell: the indices to access the first cell
    second_cell: the indices to access the second cell
  **/
  struct slice_t{
    std::pair<size_t, size_t> first_cell;
    std::pair<size_t, size_t> second_cell;
  };

  /**
    Type alias of the parse chart. The two outer vectors form the chart (also called a
    parse triangle); the inner-most vector contains all of the possible chart elements
    for that cell of the chart
  **/
  using vecVecVecChartElements = std::vector<
                                    std::vector<
                                      std::vector< chart_element_t >
                                    >
                                  >;

  /**
    Type alias for a vector of N parse trees (Phrase) with their associated
    probabilities.
  **/
  using vecPairPhraseDouble = std::vector< std::pair< Phrase, double > >;

  /**
    Do not allow compiler to generate a default constructor for h2sl::Parser class
  **/
  Parser() = delete;

  /**
    Argument constructor for h2sl::Parser class
  **/
  explicit Parser( const h2sl::PCFG& pcfgArg );

  /**
    Default copy constructor for h2sl::Parser class
  **/
  Parser( const Parser& ) = default;

  /**
    Default move constructor for h2sl::Parser class
  **/
  Parser( Parser&& ) = default;

  /**
    Default destructor for h2sl::Parser class
  **/
  ~Parser() = default;

  /**
    Default copy assignment operator for h2sl::Parser class
  **/
  Parser& operator=( const Parser& ) = default;

  /**
    Default move assignment operator for h2sl::Parser class
  **/
  Parser& operator=( Parser&& ) = default;


  /**
    This method parses an input string into a list of parse trees (Phrase). The
    parse trees have associated probabilities and are ordered accordingly.

    @brief Modified probabilistic CKY parsing algorithm to handle unary rules
    @param[in]    input       Provided input string to be parsed
    @param[in]    num_parses  The top N parses to return for the input
    @returns                  Vector of parse trees with their probabilities
    @throws                   Throws runtime error if parsing error
  **/
  vecPairPhraseDouble parse( const std::string& input, const int num_parses );

  /**
    This method provides read-only access to the bpcfg member data
  **/
  inline const h2sl::BinarizedPCFG&
  get_bpcfg( void )const{ return bpcfg; }

  /**
    This method provides read-only access to the word_strings member data
  **/
  inline const std::vector< std::string >&
  get_word_strings( void )const{ return word_strings; }

  /**
    This method provides read-only access to the parse_chart member data
  **/
  inline const vecVecVecChartElements&
  get_parse_chart( void )const{ return parse_chart; }

  /**
    This method provides read-only access to the parse_trees member data
  **/
  inline const vecPairPhraseDouble&
  get_parse_trees( void )const{ return parse_trees; }

  /** Constant to define the class preamble for exception handling */
  static const char* ParserPreamble;
protected:
private:

  /**
    This method generates the N most probable parse trees from a parse chart.
    Expected the chart to have the top elements sorted by probability.

    @brief Generate the N most probable parse trees given a parse chart
    @param[in]    num_parses      The number of most likely parse trees to generate
    @returns                      Vector of parse trees with their probabilities
    @throws                       Throws runtime error if issue with passed params
  **/
  vecPairPhraseDouble
  _generate_parse_trees( const int num_parses );

  /**
    This method populates an Phrase from a specific chart element. The input
    parent phrase is modified according to the type of the child chart element.
    If child_chart_element represents a phrase, then a new phrase is constructed and
    pushed onto the parent_phrase's children member. If child_chart_element
    represents a word, then a new word is constructed and pushed onto the parent_phrase
    words member.

    @brief Populate an Phrase for a single chart element
    @param[out]     r_parent_phrase       Phrase to be populated
    @param[in]      child_chart_element   The chart element for a child (word/phrase)
    @param[out]     r_phrase_name         The name to be incremented and assigned to the child phrase
    @returns                              none
    @throws                               Throws runtime error if tree generation issue
  **/
  void
  _populate_phrase( Phrase& r_parent_phrase, chart_element_t const & child_chart_element, char & r_phrase_name );

  /**
    This method sorts a vector of chart elements according to their probability. The
    front of the vector contains the highest probability element.

    @brief Sort vector of chart elements by probability
    @returns                      none
    @throws                       Throws runtime error if the vector is empty
  **/
  void _sort_chart_elements( std::vector< chart_element_t >& chart_elements );

  /**
    The BinarizedPCFG constructed from the provided PCFG. This is used during parsing.
  **/
  h2sl::BinarizedPCFG bpcfg;

  /**
    The data member structure to contain the strings of words for the sentence being
    parsed
  **/
  std::vector< std::string > word_strings;

  /**
    The data member structure to contain the parse chart
  **/
  vecVecVecChartElements parse_chart;

  /**
    The data member to hold the top N parse trees
  **/
  vecPairPhraseDouble parse_trees;


}; // class parser

} // namespace h2sl
