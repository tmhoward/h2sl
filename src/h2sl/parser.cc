/**
 * @file parser.cc
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

#include <sstream>
#include <h2sl/parser.h>
#include <h2sl/language-variable.h>

namespace h2sl{

///
/// Parser constuctor from a PCFG object
///
Parser::Parser( const h2sl::PCFG& pcfgArg ){
  // Create the binarized pcfg
  bpcfg = h2sl::BinarizedPCFG( pcfgArg );

  // Initialize the word_strings, parse_chart, and parse_trees members
  word_strings = std::vector<std::string>();
  parse_chart = Parser::vecVecVecChartElements();
  parse_trees = Parser::vecPairPhraseDouble();
}

///
/// Method to parse a provided input string ( e.g. sentence, instruction, etc.).
/// Uses a modified CKY parsing algorithm.
///
Parser::vecPairPhraseDouble
Parser::parse( const std::string& input, const int num_parses ){
  std::stringstream error_msg;
  // Clear the member data
  word_strings.clear();
  parse_chart.clear();
  parse_trees.clear();
  try{
    /******************************************************************************/
    /********************* Extract the words & initialize chart *******************/
    /******************************************************************************/
    // Extract individual words from the input string (assumes space-separated)
    std::stringstream ss(input);
    while( ss.good() ){
      std::string substr;
      std::getline( ss, substr, ' ');
      word_strings.push_back( substr );
    }

    // Initialize the chart's shape according to the number of words in the input
    for( size_t i = 0; i < word_strings.size(); i++ ){
      parse_chart.push_back( std::vector<std::vector< chart_element_t >>() );
      for( size_t j = i; j < word_strings.size(); j++ ){
        parse_chart[i].push_back( std::vector< chart_element_t >() );
      }
    }

    /******************************************************************************/
    /**************** Fill in first diagonal via lexicon & unary rules ************/
    /******************************************************************************/
    for( size_t i = 0; i < word_strings.size(); i++ ){
      /************************* Handle lexical rules *****************************/
      // For each word, check for a lexical rule that has it as a rhs
      bool found_lexical_category = false;
      for( const auto& v_lexical_rules : bpcfg.get_lexical_rules() ){
        for( const auto& lexical_rule : v_lexical_rules.second ){
          // Check if the word matches the rhs of the lexical rule
          if( word_strings[i] == lexical_rule.rhs ){
            found_lexical_category = true;
            // Create a chart element
            chart_element_t chart_element;
            chart_element.prob = lexical_rule.prob;
            chart_element.type = ChartElementType::Word;
            chart_element.symbol = lexical_rule.lhs;
            chart_element.word = std::make_pair( lexical_rule.rhs, i );
            // Ensure the back pointers are nullptr
            chart_element.bp_1 = nullptr;
            chart_element.bp_2 = nullptr;
            // Add the chart element to the parse chart
            parse_chart[i][0].push_back( chart_element );
          }
        }
      }
      if( !found_lexical_category ){
        error_msg << "No lexical category (POS) found for the word \""
                  << word_strings[i] << "\"";
        throw error_msg.str();
      }
      /************************* Handle unary production rules ********************/
      bool added_symbol = true;
      // set of rhs symbols used in a unary rule; prevents infinite loop
      std::set< std::string > used_unary_rhs;
      while( added_symbol ){
        added_symbol = false;
        // Temporary vector to store new chart elements; append to the parse chart
        auto unary_chart_elements = std::vector< chart_element_t >();
        for( const auto& chart_element : parse_chart[i][0] ){
          for( const auto& v_unary_rules : bpcfg.get_unary_production_rules() ){
            for( const auto& unary_rule : v_unary_rules.second ){
              // check that the rhs has not been used already
              if( used_unary_rhs.find( unary_rule.rhs ) != used_unary_rhs.end() )
                continue;
              // Check if the rhs matches the chart element symbol
              if( chart_element.symbol == unary_rule.rhs ){
                added_symbol = true;
                used_unary_rhs.insert( unary_rule.rhs );
                chart_element_t unary_chart_element;
                unary_chart_element.type = ChartElementType::UnaryPhrase;
                unary_chart_element.symbol = unary_rule.lhs;
                unary_chart_element.prob = chart_element.prob * unary_rule.prob;
                unary_chart_element.bp_1 =
                                  std::make_shared< chart_element_t>( chart_element );
                unary_chart_element.bp_2 = nullptr;
                unary_chart_elements.push_back( unary_chart_element );
              }
            }
          }
        }
        // Append the new chart elements to the vector
        parse_chart[i][0].insert( parse_chart[i][0].end(),
                            unary_chart_elements.begin(),
                            unary_chart_elements.end() );
      }
    } 
    /******************************************************************************/
    /************** Fill in the rest of the chart via production rules ************/
    /******************************************************************************/
    // Fill the chart along the diagonals; Start j at 1 since the first diagonal was
    //    filled above. Start i at 0 but only access the rows that are of length j or
    //    greater. So, the row for (i == 0) has length parse_chart.size(); the row at
    //    (i == parse_chart.size()) has length 1 (index 0).
    for( size_t j = 1; j < parse_chart.size(); j++ ){
      for( size_t i = 0; i < (parse_chart.size() -j); i++ ){
        // Create the indices for each slice. Each slice indicates which two cells in
        // the chart contain the rhs symbols for a possible rule. The number of slices
        // is equal to j.
        std::vector< slice_t > slices;
        for( size_t slice_count = 1; slice_count <= j; slice_count++ ){
          slice_t slice;
          slice.first_cell.first = i;
          slice.first_cell.second = slice_count - 1;
          slice.second_cell.first = (i + slice_count);
          slice.second_cell.second = (j - slice_count);
          slices.push_back( slice );
        }
        // For each slice, construct each possible rhs (pair of symbols)
        std::vector<std::pair<chart_element_t, chart_element_t>> v_rhs_chart_elements; 
        for( const auto& slice : slices ){
          auto& first_chart_elements = parse_chart[slice.first_cell.first]
                                                  [slice.first_cell.second];
          for( auto& first_chart_element : first_chart_elements ){
            auto& second_chart_elements = parse_chart[slice.second_cell.first]
                                                     [slice.second_cell.second];
            for( auto& second_chart_element : second_chart_elements ){
              auto rhs = std::make_pair( first_chart_element, second_chart_element );
              v_rhs_chart_elements.push_back( rhs );
            }
          }
        }
        /******************* Handle binary & x production rules *******************/
        // For each possible rhs, check if there is a production rule that matches
        for( const auto& rhs_chart_elements : v_rhs_chart_elements ){
          auto rhs = std::make_pair( rhs_chart_elements.first.symbol,
                                     rhs_chart_elements.second.symbol );
          // Check for a matching rhs in binary_production_rules
          for( const auto& v_binary_rules : bpcfg.get_binary_production_rules() ){
            for( const auto& binary_rule : v_binary_rules.second ){
              // Add a new chart element if there is a match
              if( rhs == binary_rule.rhs ){
                chart_element_t chart_element;
                chart_element.prob = binary_rule.prob
                                     * rhs_chart_elements.first.prob
                                     * rhs_chart_elements.second.prob;
                chart_element.type = ChartElementType::BinaryPhrase;
                chart_element.symbol = binary_rule.lhs;
                // Ensure the back pointers are nullptr
                chart_element.bp_1 =
                    std::make_shared<chart_element_t>(rhs_chart_elements.first);
                chart_element.bp_2 =
                    std::make_shared<chart_element_t>(rhs_chart_elements.second);
                // Add the chart element to the parse chart
                parse_chart[i][j].push_back( chart_element );
              }
            }
          }
          // Check for a matching rhs in x_production_rules
          for( const auto& x_production_rule : bpcfg.get_x_production_rules() ){
            // Add a new chart element if there is a match
            if( rhs == x_production_rule.second.rhs ){
              chart_element_t chart_element;
              chart_element.prob = x_production_rule.second.prob
                                   * rhs_chart_elements.first.prob
                                   * rhs_chart_elements.second.prob;
              chart_element.type = ChartElementType::XPhrase;
              chart_element.symbol = x_production_rule.second.lhs;
              // Ensure the back pointers are nullptr
              chart_element.bp_1 =
                  std::make_shared<chart_element_t>(rhs_chart_elements.first);
              chart_element.bp_2 =
                  std::make_shared<chart_element_t>(rhs_chart_elements.second);
              // Add the chart element to the parse chart
              parse_chart[i][j].push_back( chart_element );
            }
          }
        }
        /************************* Handle unary production rules ********************/
      // set of rhs symbols used in a unary rule; prevents infinite loop
        std::set< std::string > used_unary_rhs;
        bool added_symbol = true;
        while( added_symbol ){
          added_symbol = false;
          // Temporary vector to store new chart elements; append to the parse chart
          auto unary_chart_elements = std::vector< chart_element_t >();
          for( const auto& chart_element : parse_chart[i][j] ){
            if( chart_element.type == ChartElementType::Word ){
              error_msg << "Encounted a Word chart element not in the bottom diagonal";
              throw error_msg.str();
            }
            if( chart_element.type == ChartElementType::XPhrase )
              continue;
            for( const auto& v_unary_rules : bpcfg.get_unary_production_rules() ){
              for( const auto& unary_rule : v_unary_rules.second ){
                if( used_unary_rhs.find( unary_rule.rhs ) != used_unary_rhs.end() )
                  continue;
                // Check if the rhs matches the chart element symbol
                if( chart_element.symbol == unary_rule.rhs ){
                  added_symbol = true;
                  used_unary_rhs.insert( unary_rule.rhs );
                  chart_element_t unary_chart_element;
                  unary_chart_element.type = ChartElementType::UnaryPhrase;
                  unary_chart_element.symbol = unary_rule.lhs;
                  unary_chart_element.prob = chart_element.prob * unary_rule.prob;
                  unary_chart_element.bp_1 =
                                  std::make_shared< chart_element_t>( chart_element );
                  unary_chart_element.bp_2 = nullptr;
                  unary_chart_elements.push_back( unary_chart_element );
                }
              }
            }
          }
          // Append the new chart elements to the vector
          if( unary_chart_elements.size() > 0 ){
            parse_chart[i][j].insert( parse_chart[i][j].end(),
                                unary_chart_elements.begin(),
                                unary_chart_elements.end() );
          }
        }
      }
    }
    /******************************************************************************/
    /************** Sort the top of the chart & generate parse trees **************/
    /******************************************************************************/
    _sort_chart_elements( parse_chart.front().back() );
    parse_trees = _generate_parse_trees( num_parses );
  }
  catch( std::string msg ){
    // Prepend the class preamble
    msg.insert(0, ParserPreamble);
    throw std::runtime_error( msg );
  }
  
  return parse_trees;
}

///
/// Method to generate N parse trees from a populated chart parse
///
Parser::vecPairPhraseDouble
Parser::_generate_parse_trees( const int num_parses )
{
  std::stringstream error_msg;
  // Clear the parse_trees member
  parse_trees.clear();

  // Enforce that there is at least 1 solution
  if( parse_chart.front().back().size() == 0 ){
    error_msg << "No solutions found in the top of the chart";
    throw error_msg.str();
  }
  // Enforce that the top elements of the chart are sorted by probability
  double prob = parse_chart.front().back().front().prob;
  for( const auto& chart_element : parse_chart.front().back() ){
    if( chart_element.prob > prob ){
      error_msg << "The top of the chart was not sorted by probability";
      throw error_msg.str();
    }
    prob = chart_element.prob;
  }

  // Extract the top num_parses from the top of the chart
  auto top_parses_count = num_parses;
  if( top_parses_count > parse_chart.front().back().size() )
    top_parses_count = parse_chart.front().back().size();

  // Construct a parse tree (Phrase) for each top parse
  // Add the tree and its probability to parse_trees
  for( size_t i = 0; i < top_parses_count; i++ ){
    const auto& chart_element = parse_chart.front().back()[i];
    // Enforce the type of element
    if( chart_element.type == ChartElementType::Word ){
      error_msg << "A top chart element was of type Word";
      throw error_msg.str();
    }
    if( chart_element.type == ChartElementType::XPhrase ){
      continue;
      //error_msg << "A top chart element was of type XPhrase";
      //throw error_msg.str();
    }
    // Make the root phrase
    auto root = Phrase();
    root.type = chart_element.symbol;
 
    // Handle the first back pointer for the top chart element
    if( chart_element.bp_1 == nullptr ){
      error_msg << "A top chart element had a nullptr bp_1";
      throw error_msg.str();
    } else{
      _populate_phrase( root, *chart_element.bp_1 );
    }
    // Handle the second back pointer (can be nullptr)
    if( chart_element.bp_2 != nullptr )
      _populate_phrase( root, *chart_element.bp_2 );
    // Add the phrase to parse_trees
    parse_trees.push_back( std::make_pair( root, chart_element.prob ) );
  }

  return parse_trees;
}

///
/// Method to generate a parse tree for a provided chart element
///
void
Parser::_populate_phrase( Phrase& parent_phrase,
                          const chart_element_t& child_chart_element )
{
  std::stringstream error_msg;
  // Handle the chart element according to its type
  switch( child_chart_element.type ){
    // The chart element describes a word, add it to the parent phrase
    case ChartElementType::Word:{
      h2sl::Word word;
      word.pos = child_chart_element.symbol;
      word.text = child_chart_element.word.first;
      word.time = child_chart_element.word.second;
      if( !parent_phrase.words ){
        parent_phrase.words = std::make_optional<std::vector<Word>>();
      }
      parent_phrase.words->push_back( word );
      break;
    }
    // The chart element describes a UnaryPhrase, add it to the parent phrase and call
    //    _populate_phrase for it and the first back pointer
    case ChartElementType::UnaryPhrase:{
      if( child_chart_element.bp_1 == nullptr ){
        error_msg << "UnaryPhrase chart element had nullptr bp_1";
        throw error_msg.str();
      }
      Phrase::language_variable_connection_t connection;
      connection.child = std::make_shared<Phrase>();
      connection.child->type = child_chart_element.symbol;
      parent_phrase.children.push_back( connection );
      _populate_phrase( *(connection.child), *child_chart_element.bp_1 );
      break;
    }
    // The chart element describes a BinaryPhrase, add it to the parent phrase and call
    //    _populate_phrase for it and the both back pointers
    case ChartElementType::BinaryPhrase:{
      if( (child_chart_element.bp_1 == nullptr)
          || (child_chart_element.bp_2 == nullptr ) )
      {
        error_msg << "BinaryPhrase chart element had a nullptr back pointer";
        throw error_msg.str();
      }
      Phrase::language_variable_connection_t connection;
      connection.child = std::make_shared<Phrase>();
      connection.child->type = child_chart_element.symbol;
      parent_phrase.children.push_back( connection );
      _populate_phrase( *(connection.child), *child_chart_element.bp_1 );
      _populate_phrase( *(connection.child), *child_chart_element.bp_2 );
      break;
    }
    // The chart element described an XPhrase; pass the parent phrase and both 
    //    back pointers to _populate_phrase
    case ChartElementType::XPhrase:{
      if( (child_chart_element.bp_1 == nullptr)
          || (child_chart_element.bp_2 == nullptr ) )
      {
        error_msg << "XPhrase chart element had a nullptr back pointer";
        throw error_msg.str();
      }
      _populate_phrase( parent_phrase, *child_chart_element.bp_1 );
      _populate_phrase( parent_phrase, *child_chart_element.bp_2 );
      break;
    }
    default:{
      error_msg << "A chart element had an unknown type";
      throw error_msg.str();
    }
  }
  return;
}

///
/// Method to sort chart elements by probabilty
///
void Parser::_sort_chart_elements( std::vector< chart_element_t >& chart_elements ){
  // empty vector of chart elements to incrementally fill in order
  std::vector< chart_element_t > ordered_chart_elements;
  while( chart_elements.size() > 0 ){
    size_t index = 0;
    double top_prob = 0.0;
    for( size_t i = 0; i < chart_elements.size(); i++ ){
      if( chart_elements[i].prob > top_prob ){
        top_prob = chart_elements[i].prob;
        index = i;
      }
    }
    ordered_chart_elements.push_back( chart_elements[index] );
    chart_elements.erase( chart_elements.begin() + index );
  }

  // Replace the now empty chart_elements with ordered_chart_elements
  chart_elements = ordered_chart_elements;
  return;
}

///
/// Constant used for exception preamble
///
const char* Parser::
ParserPreamble = "[Parser Class Error] ";

} // namespace h2sl
