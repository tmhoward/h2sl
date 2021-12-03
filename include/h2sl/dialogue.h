/**
 * @file dialogue.h
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
#include <tinyxml2.h>
#include <jsoncpp/json/value.h>

#include "h2sl/sentence.h"
#include "h2sl/symbolspace.h"

namespace h2sl {
  /**
   * Dialogue class definition
   */
  class Dialogue {
  public:

  /**
    Default class constructor for h2sl::Dialogue
  **/
  Dialogue() = default;

  /**
      Dialogue class constructor from a vector of h2sl::Sentences.

      @brief Dialogue class constructor from a vector of sentences.
      @param[in]    sentences     vector of  sentences
      @returns                    none
      @throws                     no expected throws
  */
  explicit Dialogue( const std::vector< Sentence >& sentences );

  /**
      Dialogue class constructor for a single time, uid, and sentence.

      @brief Dialogue class constructor for a single sentence.
      @param[in]    sentence  sentence
      @returns              none
      @throws               no expected throws
  */
  explicit Dialogue( const Sentence& sentence );

  /**
    Dialogue class constructor from a filepath

    @brief Dialogue class constructor from a file
    @param[in]    filename    File from which to constructor the Dialogue
    @returns                  none
    @throws                   Can throw during parse
  **/
  explicit Dialogue( const std::string& filename );

  /**
    Default copy constructor for h2sl::Dialogue
  */
  Dialogue( const Dialogue& other ) = default;

  /**
    Default move constructor for h2sl::Dialogue
  */
  Dialogue( Dialogue&& other ) = default;

  /**
    Default destructor for h2sl::Dialogue
  */
  virtual ~Dialogue() = default;

  /**
    Default assignment operator for h2sl::Dialogue
  */
  Dialogue& operator=( const Dialogue& other ) = default;

  /**
    Default move assignment operator for h2sl::Dialgoue
  */
  Dialogue& operator=( Dialogue&& other ) = default;

  /**
    Method to convert the words member to a concatenated string. This recursively
    includes the words from all sentences.

    @brief Convert words to a concatenated string.
    @returns                  concatenated std::string of words
    @throws                   no expected throws
  */
  std::string words_to_std_string( void ) const;

  /**
    This method imports a Dialogue from a file without symbols.

    @brief This method imports a Dialogue from a file without symbols.
    @param[in]    filename  file to be imported from
    @returns                none
    @throws                 Throws runtime error on file loading or parsing error
  */
  virtual void from_file( const std::string& filename );

  /**
    This method imports a Dialogue from a file with symbols.

    @brief This method imports a Dialogue from a file with symbols.
    @param[in]    filename        file to be imported from
    @param[in]    p_symbolSpace   symbol space for loading the symbols
    @returns                      none
    @throws                       Throws runtime error on file loading or parsing error
  */
  virtual void from_file( const std::string& filename, SymbolSpace *const p_symbolspace );

  /**
      This method imports a Dialogue from an XML file without symbols.

      @brief This method imports a Dialogue from an XML file without symbols.
      @param[in]    filename  XML file to be imported from
      @returns                none
      @throws                 no expected throws
  */
  virtual void from_xml( const char* filename );

  /**
      This method imports a Dialogue from an XML file with symbols.

      @brief This method imports a Dialogue from an XML file with symbols.
      @param[in]    filename        XML file to be imported from
      @param[in]    p_symbolSpace   symbol space for loading the symbols
      @returns                      none
      @throws                       no expected throws
  */
  virtual void from_xml( const char* filename, SymbolSpace *const p_symbolSpace );

  /**
      This method imports a Dialogue from a tinyxml2 element.

      @brief This method imports a Dialogue from a tinyxml2 element without symbols.
      @param[in]    p_dialogue_element        current XMLElement* to an XML Dialogue element
      @returns      p_symbolspace             SymbolSpace object for inserting symbols
      @throws                                 no expected throws
  */
  virtual void from_xml( tinyxml2::XMLElement const * p_dialogue_element, SymbolSpace *const p_symbolspace );

  /**
      This method imports a Dialogue from a json formatted string.

      @brief This method imports a Dialogue from a json formatted string.
      @param[in]    json_string      json formatting string to read from
      @returns                       success/failure
      @throws                        no expected throws
  */
  virtual bool from_json( const std::string& json_string );

  /**
      This method imports a Dialogue from a json value.

      @brief This method imports a Dialogue from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
      This method exports a Dialogue to a file.

      @brief This method exports a Dialogue to a file.
      @param[in]    filename  file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_file( const std::string& filename )const;

  /**
      This method exports a Dialogue to an XML file.

      @brief This method exports a Dialogue to an XML file.
      @param[in]    filename  XML file to be exported to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( const std::string& filename )const;

  /**
      This method exports a Dialogue to a tinyxml2 document.

      @brief This method exports a Dialogue to a tinyxml2 document.
      @param[in]    document  tinyxml2 document to be written to
      @param[in]    root      root of tinyxml2 document to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const;

  /**
      This method exports a Dialogue to a json formatted string.

      @brief This method exports a Dialogue to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a Dialogue to a json value.

      @brief This method exports a Dialogue to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  /**
    These methods provide read-only and read-write access to the sentences_ member data
  **/
  inline std::vector< Sentence > const & sentences( void )const{ return sentences_; };
  inline std::vector< Sentence >& sentences( void ){ return sentences_; };


  protected:

  private:

  /**
    This method resets the member data to an initialized state.

    @brief This method resets the member data to an initialized state
    @return                       none
    @throws                       No expected throws
  **/
  void reset_( void );

  // std::vector containing the sentences of the dialogue
  std::vector< Sentence > sentences_ = std::vector< Sentence >();

  };

  /**
      Dialogue class ostream operator

      @brief Dialogue class ostream operator
      @param[in]    out       std::ostream input
      @param[in]    other     class instance
      @returns                none
      @throws                 no expected throws
   */
  std::ostream& operator<<( std::ostream& out, const Dialogue& other );
} // namespace h2sl
