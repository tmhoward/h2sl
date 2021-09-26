/**
 * @file lexicon.h
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
#include <unordered_map>
#include <set>

#include <tinyxml2.h>

#include "h2sl/sentence.h"

namespace h2sl{

/**
    Class to represent a Lexicon, which aggregates language-related structural information from examples.
*/
class Lexicon{
public:
  /**
    Type alias for a set of strings.
  **/
  using setStrings = std::set<std::string>;

  /**
    Type alias for an unordered map from strings to sets of strings.
  **/
  using mapSetStrings = std::unordered_map<std::string,setStrings>;
  
  /**
    Structure to contain known information about some type of LanguageVariable
    words: the set of words associated with the LanguageVariable type
    edge_labels: the set of edge labels from this LanguageVariable type to children
    symbol_types: the set of symbol types associated with the LanguageVariable type
    roles: the map of roles and their values associated with the LanguageVariable type
  **/
  struct lv_type_information_t{
    lv_type_information_t( void ) : words(), edge_labels(){};
    mapSetStrings words; 
    setStrings edge_labels;
    setStrings symbol_types;
    mapSetStrings roles; 
  };
  
  /** Type alias to contain a LanguageVariable type and associated information structs **/
  using mapTypeInformation = std::unordered_map< std::string, lv_type_information_t >;

  /**
    Default Lexicon constructor.
  */
  Lexicon( void ) = default;
  
  /**
    Lexicon class copy constructor.

    @brief Lexicon class copy constructor.
    @overload   
    @param[in]    other         Lexicon to be copied
    @return                     none
    @throws                     no expected throws
  */
  Lexicon( const Lexicon& other ) = default;

  /**
    Lexicon class move constructor.

    @brief Lexicon class move constructor.
    @overload
    @param[in]    other         Lexicon reference to be moved
    @returns                    none
    @throws                     no expected throws
  */
  Lexicon( Lexicon&& other ) = default;

  /**
    Lexicon class constructor to instantiate a Lexicon
    from an XMLElement*.

    @brief Lexicon class constructor from XMLelement*.
    @overload
    @param[in]    root        XMLElement* of the Lexicon to be imported
    @returns                  none
    @throws                   no expected throws
  */
  Lexicon( const tinyxml2::XMLElement* root );

  /**
    Lexicon class default destructor.

    @brief Lexicon class default destructor.
    @returns                  none
    @throws                   no throws expected
  */
  virtual ~Lexicon() = default;

  /**
    Lexicon class assignment operator.

    @brief Assignment operator from another Lexicon.
    @param[in]    other       Lexicon to be copied
    @returns                  reference to this instance of Lexicon class
    @throws                   no expected throws
  */
  Lexicon& operator=( const Lexicon& other ) = default;

  /**
    Lexicon class move assignment operator.

    @brief Move and assign operator from another Lexicon 
    @param[in]    other       Lexicon to be moved
    @returns                  reference to this instance of Lexicon class
    @throws                   no expected throws
  */
  Lexicon& operator=( Lexicon&& other ) = default;

  /**
    This method scrapes information from Sentences into the lexicon.

    @brief Scrape information from Sentences.
    @param[in]    sentences     std::vector of h2sl::Sentence
    @retuns                     boolean flag; true if no errors
    @throws                     no expected throws
  */
  bool scrape_sentences( const std::vector<std::shared_ptr<h2sl::Sentence>>& sentences );

  /**
    This method imports a Lexicon from a file.

    @brief This method imports a Lexicon from a file.
    @param[in]    filename  file to be imported from
    @returns                success/failure
    @throws                 no expected throws
  */
  virtual bool from_file( const char* filename );

  /**
    This method imports a Lexicon class from an XML file.

    @brief Import a Lexicon class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const char* filename );

  /**
    This method imports a Lexicon class from an XMLElement*.

    @brief Import a Lexicon class from an XMLElement*.
    @overload   
    @param[in]    lexicon_elem        current XMLElement* to an XML Lexicon element
    @returns                          boolean flag; true if there are no errors
    @throws                           no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement* lexicon_elem );

  /**
    This method exports a Lexicon class to an XML file.

    @brief Export a Lexicon to an XML file.
    @overload
    @param[in]    filename      XML filename to which Lexicon is exported
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool to_xml( const char* filename ) const;

  /**
    This method exports a Lexicon class to an XMLDocument object.
    The root XMLElement* takes the current Lexicon as a child XMLElement.

    @brief Export a SymbolCompostionalInfo to an XMLDocument object.
    @overload
    @param[in]    doc         XMLDocument object for the current document
    @param[in]    root        XMLElement* to accept a Lexicon as a child element
    @returns                  none
    @throws                   no expected throws
  */
  void to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root ) const;

  /**
    Lexicon class ostream operator. Prints the Lexicon
    to the ostream.

    @brief Lexicon class ostream operator.
    @param[in]    out       ostream operator class reference.
    @param[in]    other     Lexicon reference to be printed to ostream
    @returns                reference to modified ostream operator
    @throws                 no expected throws
  */
  friend std::ostream& operator<<( std::ostream& out, const Lexicon& other );

  /** Constant to define the string used in XMLElement Name for Lexicon */
  static const char* XMLLexiconName;

  /** A container to hold the LV types and associated words/edge labels */
  mapTypeInformation type_information = mapTypeInformation();

  /** A variable representing number of unique words in the lexicon */
  int word_count = 0;

private:

  /**
    This method scrapes information from a LanguageVariable into the lexicon.

    @brief Scrape information from a LanguageVariable.
    @param[in]    language_variable     std::shared_ptr to h2sl::LanguageVariable
    @returns                            boolean flag; true if no errors
    @throws                             no expected throws
  */
  bool _scrape_language_variable( const std::shared_ptr<h2sl::LanguageVariable>& language_variable );

}; // class Lexicon

}  // namespace h2sl
