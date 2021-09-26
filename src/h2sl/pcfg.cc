/**
 * @file pcfg.cc
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

#include "h2sl/pcfg.h"
#include "h2sl/common.h"

namespace h2sl{

///
/// Method to load a PCFG from a file
///
void PCFG::from_file( const char* filename ){
  try{
    if( boost::algorithm::ends_with( filename, "xml" ) ){
      from_xml( filename );
    } else{
      std::stringstream error_msg;
      error_msg << "Erroneous file extension (\"" << filename << "\") in from_file";
      throw error_msg.str();
    }
  }
  catch( std::string msg ){
    // Prepend the class preamble
    msg.insert(0, PCFGPreamble);
    throw std::runtime_error( msg );
  }
  return;
}

///
/// Method to import a PCFG from an XML file
///
void PCFG::from_xml( const char* filename ){
  // Initialize an error_msg for exception handling
  std::stringstream error_msg;
  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename );
  if( doc.Error() ){
    error_msg << "Document error during LoadFile for \"" << filename << "\""
              << " with doc.Error(): " << doc.Error();
    throw error_msg.str();
  }

  // Extract the root element from the document object
  const auto* root = doc.RootElement();
  if( root == nullptr ){
    error_msg << "Document error during RootElement for \"" << filename << "\"";
    throw error_msg.str();
  }

  // Check for any PCFG child element of the extracted root element
  const auto* pcfg_element = root->FirstChildElement( XMLPCFGName );
  if( pcfg_element == nullptr ){
    error_msg << "No grammar element found as a child of the root element for \""
              << filename << "\"";
    throw error_msg.str();
  }

  from_xml( pcfg_element );
  return;
}

///
/// Method to import a PCFG from an XMLElement*
///
void PCFG::from_xml( const tinyxml2::XMLElement* pcfg_element ){
  // Initialize an error_msg for exception handling
  std::stringstream error_msg;

  // Clear the data members
  terminals.clear();
  preterminals.clear();
  nonterminals.clear();
  lexical_rules.clear();
  production_rules.clear();

  // Check that the element is a PCFG element
  if( pcfg_element == nullptr ){
    error_msg << "The XMLElement* was nullptr";
    throw error_msg.str();
  }
  if( std::strcmp( pcfg_element->Name(), XMLPCFGName ) != 0 ){
    error_msg << "The XMLElement* name (\"" << pcfg_element->Name()
              << "\") did not match the PCFG name (\""
              << XMLPCFGName << "\")";
    throw error_msg.str();
  }

  /********************************************************************************/
  /***************************** Import the terminals *****************************/
  /********************************************************************************/
  // Import the terminals from a csv attribute string
  const auto* terminals_element = pcfg_element->FirstChildElement(XMLTerminalsName);
  if( terminals_element == nullptr ){
    error_msg << "No XMLElement child found for the terminal symbols.";
    throw error_msg.str();
  }
  const auto*
  terminals_attribute = terminals_element->FindAttribute(XMLSymbolsAttributeName);
  if( terminals_attribute == nullptr ){
    error_msg << "No XMLAttribute found for the terminals symbols.";
    throw error_msg.str();
  }
  // Extract the csv string into a set and store as terminals data member
  std::string terminals_string = terminals_attribute->Value();
  terminals = std_string_set_from_std_string( terminals_string );

  /********************************************************************************/
  /***************************** Import the preterminals **************************/
  /********************************************************************************/
  // Import the preterminals from a csv attribute string
  const auto*
  preterminals_element = pcfg_element->FirstChildElement(XMLPreTerminalsName);
  if( preterminals_element == nullptr ){
    error_msg << "No XMLElement child found for the preterminal symbols.";
    throw error_msg.str();
  }
  const auto* preterminals_attribute =
                          preterminals_element->FindAttribute(XMLSymbolsAttributeName);
  if( preterminals_attribute == nullptr ){
    error_msg << "No XMLAttribute found for the preterminal symbols.";
    throw error_msg.str();
  }
  // Extract the csv string into a set and store as preterminals data member
  std::string preterminals_string = preterminals_attribute->Value();
  preterminals = std_string_set_from_std_string( preterminals_string );

  /********************************************************************************/
  /***************************** Import the nonterminals **************************/
  /********************************************************************************/
  // Import the nonterminals from a csv attribute string
  const auto*
  nonterminals_element = pcfg_element->FirstChildElement(XMLNonTerminalsName);
  if( nonterminals_element == nullptr ){
    error_msg << "No XMLElement child found for the nonterminal symbols.";
    throw error_msg.str();
  }
  const auto* nonterminals_attribute =
                          nonterminals_element->FindAttribute(XMLSymbolsAttributeName);
  if( nonterminals_attribute == nullptr ){
    error_msg << "No XMLAttribute found for the nonterminal symbols.";
    throw error_msg.str();
  }
  // Extract the csv string into a set and store as nonterminals data member
  std::string nonterminals_string = nonterminals_attribute->Value();
  nonterminals = std_string_set_from_std_string( nonterminals_string );

  /********************************************************************************/
  /***************************** Import the lexical rules *************************/
  /********************************************************************************/
  // Import the lexical rules from child elements; Each child is a single rule
  const auto*
  lexical_rules_element = pcfg_element->FirstChildElement(XMLLexicalRulesName);
  if( lexical_rules_element == nullptr ){
    error_msg << "No XMLElement child found for the lexical rules.";
    throw error_msg.str();
  }
  // Check for child lexical rule elements
  auto*
  lexical_rule_element = lexical_rules_element->FirstChildElement(XMLLexicalRuleName);
  if( lexical_rule_element == nullptr ){
    error_msg << "No XMLElement children found for the lexical rules.";
    throw error_msg.str();
  }

  // Iterate through each child element containing a lexical rule
  while( lexical_rule_element != nullptr ){
    // Create a lexical_rule_t object to contain the constituents of the rule
    lexical_rule_t lexical_rule;
    
    // Extract lhs, rhs, probability, and frequency from individual attributes
    const auto*
    lhs_attribute = lexical_rule_element->FindAttribute(XMLRuleLHSAttributeName);
    if( lhs_attribute == nullptr ){
      error_msg << "No \"" << XMLRuleLHSAttributeName << "\" XMLAttribute found for "
                << "a lexical rule.";
      throw error_msg.str(); 
    }
    lexical_rule.lhs = lhs_attribute->Value();

    const auto*
    rhs_attribute = lexical_rule_element->FindAttribute(XMLRuleRHSAttributeName);
    if( rhs_attribute == nullptr ){
      error_msg << "No \"" << XMLRuleRHSAttributeName << "\" XMLAttribute found for "
                << "a lexical rule.";
      throw error_msg.str();
    }
    lexical_rule.rhs = rhs_attribute->Value();

    const auto*
    prob_attribute = lexical_rule_element->FindAttribute(XMLRuleProbAttributeName);
    if( prob_attribute == nullptr ){
      error_msg << "No \"" << XMLRuleProbAttributeName << "\" XMLAttribute found for "
                << "a lexical rule.";
      throw error_msg.str();
    }
    lexical_rule.prob = prob_attribute->DoubleValue();

    const auto*
    freq_attribute = lexical_rule_element->FindAttribute(XMLRuleFreqAttributeName);
    if( freq_attribute == nullptr ){
      error_msg << "No \"" << XMLRuleFreqAttributeName << "\" XMLAttribute found for "
                << "a lexical rule.";
      throw error_msg.str();
    }
    lexical_rule.freq = freq_attribute->IntValue();

    // Insert the lexical rule onto lexical_rules
    std::vector< lexical_rule_t > vec_lexical_rules ( 1, lexical_rule );
    auto ret_lexical_rules = lexical_rules.insert(std::make_pair(
                                            lexical_rule.lhs,
                                            vec_lexical_rules ) );
    if( !ret_lexical_rules.second )
      ret_lexical_rules.first->second.push_back( lexical_rule );

    // Move to next lexical rule element
    lexical_rule_element = lexical_rule_element->NextSiblingElement();
  }

  /********************************************************************************/
  /***************************** Import the production rules **********************/
  /********************************************************************************/
  // Import the production rules from child elements; Each child is a single rule
  const auto*
  production_rules_element = pcfg_element->FirstChildElement(XMLProductionRulesName);
  if( production_rules_element == nullptr ){
    error_msg << "No XMLElement child found for the production rules.";
  throw error_msg.str();
  }

  // Check for child production rule elements
  auto* production_rule_element =
                  production_rules_element->FirstChildElement(XMLProductionRuleName);
  if( production_rule_element == nullptr ){
    error_msg << "No XMLElement children found for the production rules.";
    throw error_msg.str();
  }

  // Iterate through each child element containing a production rule
  while( production_rule_element != nullptr ){
    // Create a production_rule_t object to contain the consistuents of the rule
    production_rule_t production_rule;

    // Extract lhs, rhs, probability, and frequency from individual attributes
    const auto*
    lhs_attribute = production_rule_element->FindAttribute(XMLRuleLHSAttributeName);
    if( lhs_attribute == nullptr ){
      error_msg << "No \"" << XMLRuleLHSAttributeName << "\" XMLAttribute found for "
                << "a production rule.";
      throw error_msg.str();
    }
    production_rule.lhs = lhs_attribute->Value();

    const auto*
    rhs_attribute = production_rule_element->FindAttribute(XMLRuleRHSAttributeName);
    if( rhs_attribute == nullptr ){
      error_msg << "No \"" << XMLRuleRHSAttributeName << "\" XMLAttribute found for "
                << "a production rule.";
      throw error_msg.str();
    }
    std::string rhs_string = rhs_attribute->Value();
    production_rule.rhs = std_string_vector_from_std_string( rhs_string );

    const auto*
    prob_attribute = production_rule_element->FindAttribute(XMLRuleProbAttributeName);
    if( prob_attribute == nullptr ){
      error_msg << "No \"" << XMLRuleProbAttributeName << "\" XMLAttribute found for "
                << "a production rule.";
      throw error_msg.str();
    }
    production_rule.prob = prob_attribute->DoubleValue();

    const auto*
    freq_attribute = production_rule_element->FindAttribute(XMLRuleFreqAttributeName);
    if( freq_attribute == nullptr ){
      error_msg << "No \"" << XMLRuleFreqAttributeName << "\" XMLAttribute found for "
                << "a lexical rule.";
      throw error_msg.str();
    }
    production_rule.freq = freq_attribute->IntValue();

    // Insert the production rule onto production_rules
    std::vector< production_rule_t > vec_production_rules ( 1, production_rule );
    auto ret_production_rules = production_rules.insert(std::make_pair(
                                            production_rule.lhs,
                                            vec_production_rules ) );
    if( !ret_production_rules.second )
      ret_production_rules.first->second.push_back( production_rule );

    // Move to next production rule element
    production_rule_element = production_rule_element->NextSiblingElement();
  }

  // Check that the imported PCFG does not violate any class invariant constraints
  // This method will throw an error if something is violated
  _check_invariants();
  return;
}

///
/// Method to export a PCFG to an XML file
///
void PCFG::to_xml( const char* filename )const{
  tinyxml2::XMLDocument doc;
  auto* root = doc.NewElement("root");
  doc.InsertEndChild( root );
  to_xml( doc, root );

  // Save the file
  tinyxml2::XMLError error_result = doc.SaveFile( filename );
  if( error_result != tinyxml2::XML_SUCCESS ){
    std::stringstream error_msg;
    error_msg << "Failed to save XML file, error state: " << error_result;
    throw error_msg.str();
  }
  return;
}


///
/// Method to export a PCFG to an XMLDocument object
///
void PCFG::to_xml( tinyxml2::XMLDocument& doc,
                   tinyxml2::XMLElement* root )const
{
  // Create the pcfg element and insert it as a child of root
  auto* pcfg_element = doc.NewElement( XMLPCFGName );
  root->InsertEndChild( pcfg_element );

  /********************************************************************************/
  /***************************** Export the terminals *****************************/
  /********************************************************************************/
  // Create the terminals element and insert it as a child of pcfg_element
  auto* terminals_element = doc.NewElement( XMLTerminalsName );
  pcfg_element->InsertEndChild( terminals_element );
  // Export the terminal symbols as a csv string attribute
  auto terminals_string = std_string_set_to_std_string( terminals );
  terminals_element->SetAttribute(XMLSymbolsAttributeName, terminals_string.c_str() );

  /********************************************************************************/
  /***************************** Export the preterminals **************************/
  /********************************************************************************/
  // Create the preterminals element and insert it as a child of pcfg_element
  auto* preterminals_element = doc.NewElement( XMLPreTerminalsName );
  pcfg_element->InsertEndChild( preterminals_element );
  // Export the preterminal symbols as a csv string attribute
  auto preterminals_string = std_string_set_to_std_string( preterminals );
  preterminals_element->SetAttribute(XMLSymbolsAttributeName,
                                     preterminals_string.c_str() );

  /********************************************************************************/
  /***************************** Export the nonterminals **************************/
  /********************************************************************************/
  // Create the nonterminals element and insert it as a child of pcfg_element
  auto* nonterminals_element = doc.NewElement( XMLNonTerminalsName );
  pcfg_element->InsertEndChild( nonterminals_element );
  // Export the nonterminal symbols as a csv string attribute
  auto nonterminals_string = std_string_set_to_std_string( nonterminals );
  nonterminals_element->SetAttribute(XMLSymbolsAttributeName,
                                     nonterminals_string.c_str() );

  /********************************************************************************/
  /***************************** Export the lexical rules *************************/
  /********************************************************************************/
  // Create the lexical rules element and insert it as a child of pcfg_element
  auto* lexical_rules_element = doc.NewElement( XMLLexicalRulesName );
  pcfg_element->InsertEndChild( lexical_rules_element );
  // For each lexical rule, create a new child element of lexical_rules_element
  for( const auto& v_lexical_rules : lexical_rules ){
    for( const auto& lexical_rule : v_lexical_rules.second ){
      auto* lexical_rule_element = doc.NewElement( XMLLexicalRuleName );
      lexical_rules_element->InsertEndChild( lexical_rule_element );
      // Add the lhs, rhs, and prob as attributes
      lexical_rule_element->SetAttribute( XMLRuleLHSAttributeName,
                                          lexical_rule.lhs.c_str() );
      lexical_rule_element->SetAttribute( XMLRuleRHSAttributeName,
                                          lexical_rule.rhs.c_str() );
      lexical_rule_element->SetAttribute( XMLRuleProbAttributeName,
                                          lexical_rule.prob );
      lexical_rule_element->SetAttribute( XMLRuleFreqAttributeName,
                                          lexical_rule.freq );
    }
  }

  /********************************************************************************/
  /***************************** Export the production rules **********************/
  /********************************************************************************/
  // Create the production rules element and insert it as a child of pcfg_element
  auto* production_rules_element = doc.NewElement( XMLProductionRulesName );
  pcfg_element->InsertEndChild( production_rules_element );
  // For each production rule, create a new child element of production_rules_element
  for( const auto& v_production_rules : production_rules ){
    for( const auto& production_rule : v_production_rules.second ){
      auto* production_rule_element = doc.NewElement( XMLProductionRuleName );
      production_rules_element->InsertEndChild( production_rule_element );
      // Add the lhs, rhs, and prob as attributes
      production_rule_element->SetAttribute( XMLRuleLHSAttributeName,
                                             production_rule.lhs.c_str() );
      auto rhs_string = std_string_vector_to_std_string( production_rule.rhs );
      production_rule_element->SetAttribute( XMLRuleRHSAttributeName,
                                             rhs_string.c_str() );
      production_rule_element->SetAttribute( XMLRuleProbAttributeName,
                                             production_rule.prob );
      production_rule_element->SetAttribute( XMLRuleFreqAttributeName,
                                             production_rule.freq );
    }
  }
  return;
}

///
/// Method to determine the size of the lexicon
///
int PCFG::lexicon_size( void )const{
  int size = 0;
  // Iterate through each lexical category
  for( const auto& v_lexical_rules : lexical_rules ){
    size += v_lexical_rules.second.size();
  }
  return size;
}

///
/// Method to append to a grammar via components scraped from h2sl::Sentences
///
void PCFG::scrape_sentences(
  const std::vector<std::shared_ptr<h2sl::Sentence>>& sentences )
{
  try{
    // For each sentence, start a recursive call on the root Phrase
    for( const auto& sentence: sentences ){
      _scrape_phrase( sentence->child );
    }

    // Assign the rule probabilities for lexical_rules and production_rules
    _assign_rule_probabilities();

    // Check that the PCFG does not violate any class invariant constraints
    _check_invariants();
  }
  catch( std::string msg ){
    //Prepend the class preamble
    msg.insert(0, PCFGPreamble);
    throw std::runtime_error( msg );
  }
  return;
}

///
/// Method (private) to append to a grammar via components scraped from a Phrase
///
void PCFG::_scrape_phrase( const std::shared_ptr< Phrase >& phrase ){
  // Initialize an error message
  std::stringstream error_msg;

  // Scrape any child phrases first (recursive)
  for( const auto& connection : phrase->children ){
    _scrape_phrase( connection.child );
  }

  /********************************************************************************/
  /********** Scrape terminals, preterminals, & lexical rules from words **********/
  /********************************************************************************/
  // For each word, add the text to terminals & add the POS to preterminals
  // Check if the lexical rule exists; if no, add it; if yes, increment the freq
  for( const auto& word : phrase->words.value() ){
    terminals.insert( word.text );
    preterminals.insert( word.pos );
    // Check lexical_rules for an existing lhs (POS)
    auto it_lexical_rules = lexical_rules.find( word.pos );
    // If it does not exist, create one with a single lexical_rule_t element
    // Else, look for a matching rhs; if no match, add new rule; if match, increment
    if( it_lexical_rules == lexical_rules.end() ){
      std::vector<lexical_rule_t>
      v_lexical_rules ( 1, _make_lexical_rule_from_word( word ) );
      auto ret_lexical_rules = lexical_rules.insert(std::make_pair( word.pos,
                                                                  v_lexical_rules ));
      if( !ret_lexical_rules.second ){
        error_msg << "Error inserting new lexical rule.";
        throw error_msg.str();
      }
    } else{
      // Look for a matching rhs (word text)
      bool found_match = false;
      for( auto& lexical_rule : it_lexical_rules->second ){
        if( word.text == lexical_rule.rhs ){
          lexical_rule.freq++;
          found_match = true;
          break;
        }
      }
      if( !found_match )
        it_lexical_rules->second.push_back( _make_lexical_rule_from_word( word ) );
    }
  }

  /********************************************************************************/
  /******************************* Scrape nonterminals ****************************/
  /********************************************************************************/
  nonterminals.insert( phrase->type );

  /********************************************************************************/
  /**************************** Scrape production rules ***************************/
  /********************************************************************************/
  // Construct a production_rule_t object for the current phrase
  auto new_production_rule = _make_production_rule_from_phrase( *phrase );
  // Check production_rules for an existing lhs (phrase type)
  auto it_production_rules = production_rules.find( new_production_rule.lhs );

  // If it does not exist, create on with a single production_rule_t element
  // Else, look for a matching rhs; if no match, add new rule; if match, increment
  if( it_production_rules == production_rules.end() ){
    std::vector<production_rule_t>
    v_production_rules ( 1, new_production_rule );
    auto ret_production_rules = production_rules.insert(std::make_pair(
                                      new_production_rule.lhs, v_production_rules ));
    if( !ret_production_rules.second){
      error_msg << "Error inserting new production rule.";
      throw error_msg.str();
    }
  } else{
    // Look for a matching rhs (vector of nonterminals/preterminals)
    bool found_match = false;
    for( auto& production_rule : it_production_rules->second ){
      if( new_production_rule.rhs == production_rule.rhs ){
        production_rule.freq++;
        found_match = true;
        break;
      }
    }
    if( !found_match )
      it_production_rules->second.push_back( new_production_rule );
  }
  return;
}

///
/// Method to construct a lexical rule from an h2sl::Word
///
PCFG::lexical_rule_t
PCFG::_make_lexical_rule_from_word( const h2sl::Word& word )const{
  lexical_rule_t lexical_rule;
  lexical_rule.lhs = word.pos;
  lexical_rule.rhs = word.text;
  lexical_rule.prob = 1.0;
  lexical_rule.freq = 1;
  return lexical_rule;
}

///
/// Method to construct a production rule from an Phrase
///
PCFG::production_rule_t
PCFG::_make_production_rule_from_phrase( const Phrase& phrase )const{
  production_rule_t production_rule;
  production_rule.lhs = phrase.type;
  production_rule.rhs = std::vector< std::string >();
  production_rule.prob = 1.0;
  production_rule.freq = 1;
  
  // Extract the rhs symbols with their time values
  // Use the minimum time found for a word in each child phrase
  auto size = phrase.words->size() + phrase.children.size();
  std::vector< std::pair< std::string, double > > rhs_symbols;
  for( const auto& word : phrase.words.value() ){
    auto rhs_symbol = std::make_pair( word.pos, word.time );
    rhs_symbols.push_back( rhs_symbol );
  }
  for( const auto& connection : phrase.children ){
    auto child_time = _get_phrase_min_time( *(connection.child) );
    auto rhs_symbol = std::make_pair( connection.child->type, child_time);
    rhs_symbols.push_back( rhs_symbol );
  }
  // Sort the rhs symbols by time & incrementally add them to production_rule
  while( rhs_symbols.size() > 0 ){
    size_t index = 0;
    double min_time = DBL_MAX;
    for( size_t i = 0; i < rhs_symbols.size(); i++ ){
      if( rhs_symbols[i].second < min_time ){
        min_time = rhs_symbols[i].second;
        index = i;
      }
    }
    production_rule.rhs.push_back( rhs_symbols[index].first );
    rhs_symbols.erase( rhs_symbols.begin() + index );
  }
  return production_rule;
}

///
/// Method to get the earliest time associated with a word in a phrase
///
double
PCFG::_get_phrase_min_time( const Phrase& phrase )const{
  // Initialize to a value that is necessarily larger than any found
  double min_time = DBL_MAX;
  // Recursively find the minimum value for child phrases
  for( const auto& connection : phrase.children ){
    auto child_min_time = _get_phrase_min_time( *(connection.child) );
    if( child_min_time < min_time )
      min_time = child_min_time;
  }
  // If the current phrase has words, compare their time
  for( const auto& word : phrase.words.value() ){
    if( word.time < min_time )
      min_time = word.time;
  }
  return min_time;
}

///
/// Method to assign probabilities to both lexical and production rules
///
void PCFG::_assign_rule_probabilities( void ){
  // Assign lexical rule probabilities
  // For each rule with the same lhs, sum their frequencies and assign probability
  for( auto& v_lexical_rules : lexical_rules ){
    int denom = 0;
    for( const auto& lexical_rule : v_lexical_rules.second ){
      denom += lexical_rule.freq;
    }
    if( denom == 0 ){
      std::stringstream error_msg;
      error_msg << "Divide by 0 assigning probabilities for lexical rules with lhs \""
                << v_lexical_rules.first << "\"";
      throw error_msg.str();
    }
    for( auto& lexical_rule : v_lexical_rules.second ){
      lexical_rule.prob = ( lexical_rule.freq / (double) denom );
    }
  }

  // Assign production rule probabilities
  // For each rule with the same lhs, sum their frequencies and assign probability
  for( auto& v_production_rules : production_rules ){
    int denom = 0;
    for( const auto& production_rule : v_production_rules.second ){
      denom += production_rule.freq;
    }
    if( denom == 0 ){
      std::stringstream error_msg;
      error_msg << "Divide by 0 assigning probabilities for production rules with "
                << "lhs \"" << v_production_rules.first << "\"";
      throw error_msg.str();
    }
    for( auto& production_rule : v_production_rules.second ){
      production_rule.prob = ( production_rule.freq / (double) denom );
    }
  }
  return;
}

///
/// Method to check the enforcement of PCFG class invariants
///   1. No rules are repeated
///   2. The probabilities are appropriately constrained
void PCFG::_check_invariants( void )const{
  std::stringstream error_msg;

  /********************************************************************************/
  /**************************** Check for redundant rules *************************/
  /********************************************************************************/
  // Check for repeated lexical_rules
  for( const auto& v_lexical_rules : lexical_rules ){
    for( size_t i = 0; i < v_lexical_rules.second.size(); i++ ){
      for( size_t j = i+1; j < v_lexical_rules.second.size(); j++ ){
        if( v_lexical_rules.second[i].rhs == v_lexical_rules.second[j].rhs ){
          error_msg << "Found redunant lexical rule with lhs \""
                    << v_lexical_rules.second[i].lhs << "\" and rhs \""
                    << v_lexical_rules.second[i].rhs;
          throw error_msg.str();
        }
      }
    }
  }
  // Check for repeated production_rules
  for( const auto& v_production_rules : production_rules ){
    for( size_t i = 0; i < v_production_rules.second.size(); i++ ){
      for( size_t j = i+1; j < v_production_rules.second.size(); j++ ){
        if( v_production_rules.second[i].rhs == v_production_rules.second[j].rhs ){
          error_msg << "Found redunant production rule with lhs \""
                    << v_production_rules.second[i].lhs << "\" and rhs \"";
          for( const auto& symbol : v_production_rules.second[j].rhs ){
            error_msg << symbol << " ";
          }
          error_msg << "\"";
          throw error_msg.str();
        }
      }
    }
  }

  /********************************************************************************/
  /**************** Check that probabilities for a given lhs sum to 1.0 ***********/
  /********************************************************************************/
  int epsilon = 5;
  // Check the probabilities constraint for lexical_rules
  for( const auto& v_lexical_rules : lexical_rules ){
    double sum = 0.0;
    for( const auto& lexical_rule : v_lexical_rules.second ){
      sum += lexical_rule.prob;
    }
    if( !(fabs(1.0 - sum) < (epsilon * DBL_EPSILON * fabs(1.0 + sum))) ){
      error_msg << "The sum of probabilities for lexical rules with the lhs \""
                << v_lexical_rules.first << "\" did NOT sum to 1.0.";
      throw error_msg.str();
    }
  }
  // Check the probabilities constraint for production_rules
  for( const auto& v_production_rules : production_rules ){
    double sum = 0.0;
    for( const auto& production_rule : v_production_rules.second ){
      sum += production_rule.prob;
    }
    if( !(fabs(1.0 - sum) < (epsilon * DBL_EPSILON * fabs(1.0 + sum))) ){
      error_msg << "The sum of probabilities for production rules with the lhs \""
                << v_production_rules.first << "\" did NOT sum to 1.0.";
      throw error_msg.str();
    }
  }
  return;
}

///
/// PCFG class ostream operator
///
/*
std::ostream& operator<<( std::ostream& out, const PCFG& other ){
  out << "PCFG( ";
  // TODO
  out << ")";
  return out;
}
*/

///
/// Constant used for exception preambles
///
const char* PCFG::
PCFGPreamble = "[PCFG Class Error] ";

///
/// Constant used for XMLElement name for PCFG
///
const char* PCFG::
XMLPCFGName = "pcfg";

///
/// Constant used for XMLELement name for terminals
///
const char* PCFG::
XMLTerminalsName = "terminals";

///
/// Constant used for XMLElement name for preterminals
///
const char* PCFG::
XMLPreTerminalsName = "preterminals";

///
/// Constant used for XMLElement name for nonterminals
///
const char* PCFG::
XMLNonTerminalsName = "nonterminals";

///
/// Constant used for XMLAttribute name for symbols
///
const char* PCFG::
XMLSymbolsAttributeName = "symbols";

///
/// Constant used for XMLElement name for lexical rules
///
const char* PCFG::
XMLLexicalRulesName = "lexical-rules";

///
/// Constant used for XMLElement name for a single lexical rule
///
const char* PCFG::
XMLLexicalRuleName = "lexical-rule";

///
/// Constant used for XMLElement name for production rules
///
const char* PCFG::
XMLProductionRulesName = "production-rules";

///
/// Constant used for XMLElement name for a single production rule
///
const char* PCFG::
XMLProductionRuleName = "production-rule";

///
/// Constant used for XMLAttribute name for the lhs of rules
///
const char* PCFG::
XMLRuleLHSAttributeName = "lhs";

///
/// Constant used for XMLAttribute name for the rhs of rules
///
const char* PCFG::
XMLRuleRHSAttributeName = "rhs";

///
/// Constant used for XMLAttibute name for the probability of rules
///
const char* PCFG::
XMLRuleProbAttributeName = "prob";

///
/// Constant used for XMLAttribute name for the frequency of rules
///
const char* PCFG::
XMLRuleFreqAttributeName = "freq";

} // namespace h2sl
