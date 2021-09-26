/**
 * @file sentence.h
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

#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <tinyxml2.h>
#include <jsoncpp/json/value.h>

#include "h2sl/language-variable.h"

namespace h2sl{

/**
    Class to represent a Sentence
*/
class Sentence {
  public:
  /**
    Default constructor for h2sl::Sentence class
  **/
  Sentence() = default;

  /**
    Parameter constructor for h2sl::Sentence class without symbols. This method instantiates a unique identified (uid) of the source
    and the child language variable of the sentence.

    @brief Parameter Sentence class constructor without symbols.
    @param[in]    uidArg    Unique ID of the source
    @param[in]    childArg  Child language variable of the sentence
    @returns                none
    @throws                 no expected throws
  **/
  Sentence( const std::string& uidArg, const std::shared_ptr<LanguageVariable>& childArg );

  /**
    Parameter constructor for h2sl::Sentence class with symbols. This method instantiates a
    unique identifier of the source, a vector of Symbols with which it 
    corresponds, and the child language variable.

    @brief Parameter Sentence class constructor with symbols.
    @param[in]    uidArg      Unique ID of the source
    @param[in]    childArg    Child language variable
    @param[in]    symbolsArg  Vector of Symbols corresponding to the sentence
    @returns                  none
    @throws                   no expected throws
  */
  Sentence( const std::string& uidArg,
            const std::shared_ptr<LanguageVariable>& childArg,
            const LanguageVariable::symbolsVector& symbolsArg );

  /**
    Sentence class constructor to instantiate a Sentence from an XMLElement*.

    @brief Sentence class constructor from XMLElement*.
    @overload
    @param[in]    root          XMLElement* of the Sentence to be imported
    @param[in]    symbolspace   SymbolSpace object to insert symbols
    @returns                    none
    @throws                     no expected throws
  */
  Sentence( const tinyxml2::XMLElement* root, std::shared_ptr<SymbolSpace>& symbolspace );

  /**
    Default copy constructor for h2sl::Sentence
  */
  Sentence( const Sentence& other ) = default;

  /**
    Default move constructor for h2sl::Sentence
  */
  Sentence( Sentence&& other ) = default;

  /**
    Default destructor for h2sl::Sentence
  */
  virtual ~Sentence() = default;

  /**
    Default assignment operator for h2sl::Sentence
  */
  Sentence& operator=( const Sentence& other ) = default;

  /**
    Default move assignment operator for h2sl::Sentence
  */
  Sentence& operator=( Sentence&& other ) = default;

  /**
    Method to convert the words member to a concatenated string. This recursively
    includes the words from all child language variables.

    @brief Convert words to a concatenated string.
    @returns                  concatenated std::string of words
    @throws                   no expected throws
  */
  std::string words_to_std_string( void ) const;

  /**
    Method to return all Words in a vector. This recursively includes the Words
    from all child language variables.

    @brief Return all Words in a vector for this Sentence and its children.
    @returns                  vector of Words from this and all child language variables
    @throws                   no expected throws
  */
  std::vector<Word> all_words( void ) const;

  /**
    This method imports a Sentence class from an XML file without symbols.

    @brief Import a Sentence class from an XML file without symbols.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const char* filename );

  /**
    This method imports a Sentence class from an XML file with symbols.

    @brief Import a Sentence class from an XML file with symbols.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @param[in]    symbolspace   SymbolSpace object for inserting symbols
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const char* filename, std::shared_ptr<SymbolSpace>& symbolspace );

  /**
    This method imports a Sentence class from an XMLElement* without symbols.

    @brief Import a Sentence class from an XMLElement* without symbols.
    @overload
    @param[in]      sentence_elem   current XMLElement* to an XML Sentence element
    @returns                        boolean flag; true if there are no errors
    @throws                         no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement* sentence_elem );

  /**
    This method imports a Sentence class from an XMLElement* with symbols.

    @brief Import a Sentence class from an XMLElement* with symbols.
    @overload
    @param[in]      sentence_elem   current XMLElement* to an XML Sentence element
    @param[in]      symbolspace     SymbolSpace object for inserting symbols
    @returns                        boolean flag; true if there are no errors
    @throws                         no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement* sentence_elem, std::shared_ptr<SymbolSpace>& symbolspace );

  /**
      This method imports a Sentence from a json formatted string.

      @brief This method imports a Sentence from a json formatted string.
      @param[in]    json_string     json formatting string to read from
      @returns                      success/failure
      @throws                       no expected throws
  */
  virtual bool from_json( const std::string& json_string );

  /**
      This method imports a Sentence from a json value.

      @brief This method imports a Sentence from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
    This method export a Sentence class to an XML file.

    @brief Export a Sentence to an XML file.
    @overload
    @param[in]    filename    XML filename to which Sentence class is exported
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool to_xml( const char* filename ) const;

  /**
    This method exports a Sentence class to an XMLDocument object. The root
    XMLElement* takes the current Sentence as a child XMLElement.

    @brief Export a Sentence class to an XMLDocument object.
    @overload
    @param[in]    doc       XMLDocument object for the current document
    @param[in]    root      XMLElement* to accept a Sentence as a child element
    @returns                none
    @throws                 no expected throws
  */
  void to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root ) const;

  /**
      This method exports a Sentence to a json formatted string.

      @brief This method exports a Sentence to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a Sentence to a json value.

      @brief This method exports a Sentence to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  /**
    Sentence class ostream operator. Prints the Sentence to the ostream.

    @brief Sentence class ostream operator.
    @param[in]    out       ostream operator class reference
    @param[in]    other     Sentence to be printed to the ostream
    @returns                referece to modified ostream operator
    @throws                 no expected throws
  */
  friend std::ostream& operator<<( std::ostream& out, const Sentence& other );

  /** std::string containing the uid of the source */
  std::string uid = "";
 
  /** child language variable */
  std::shared_ptr<LanguageVariable> child = std::make_shared<LanguageVariable>();
  
  /** 
    std::vector containing std::shared_ptrs of Symbol objects corresponding to
    this Sentence instance.
  */
  LanguageVariable::symbolsVector symbols = LanguageVariable::symbolsVector();
 
  protected:
  private:

}; // class Sentence

} // namespace h2sl
