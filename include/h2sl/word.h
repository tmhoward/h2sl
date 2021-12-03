/**
 * @file word.h
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

#include "h2sl/WordMessage.h"

#include <tinyxml2.h>
#include <jsoncpp/json/value.h>

namespace h2sl {

/**
    Class to represent a Word
*/
class Word{
  public:
  /**
    Default constructor for Word.
  **/
  Word() = default;

  /**
    Default class constructor for h2sl::Word class. This method
    instantiates a Word of the input part-of-speech, text, and time.

    @brief Default Word class constructor.
    @param[in]    pos         Part-of-speech tag for the Word
    @param[in]    text        Text of the Word
    @param[in]    time       Order of the Word in the sentence. Starts at 0.
    @returns                  none
    @throws                   no expected throws
  */
  Word( const std::string& pos, const std::string& text, const double time );

  /**
    Default Word class copy constructor.
  */
  Word( const Word& other ) = default;

  /**
    Default Word class move constructor.
  */
  Word( Word&& other ) = default;

  /**
    Word class equality operator to check equality between two Words.

    @brief Word class equality operator.
    @param[in]    other     Word reference to be compared
    @returns                boolean flag, true if the Words are equal
    @throws                 no expected throws
  */
  bool operator==( const Word& other ) const;

  /**
    Word class inequality operator to check inequality between two Words.

    @brief Word class inequality operator.
    @param[in]    other     Word reference to be compared
    @returns                boolean flag, true if the Word are not equal
    @throws                 no expected throws
  */
  bool operator!=( const Word& other) const;

  /**
    Word class constructor to instantiate a Word from an XMLElement*.

    @brief Word class constructor from XMLElement*.
    @overload
    @param[in]    root        XMLElement* of the Word to be imported
    @returns                  none
    @throws                   no expected throws
  */
  Word( const tinyxml2::XMLElement* root );

  /**
    Class constructor for Word from WordMessage message.

    @brief Class constructor from WordMessage.
    @param[in]  msg   WordMessage message
    @returns          none
    @throws           no expected throws
  */
  Word( const WordMessage& msg );

  /**
    Default Word class destructor.
  */
  virtual ~Word() = default;

  /**
    Default Word class copy assignment operator.
  */
  Word& operator=( const Word& other ) = default;

  /**
    Default Word class move assignment operator.
  */
  Word& operator=( Word&& other ) = default;

  /**
    This method imports a Word class from an XML file.

    @brief Import a Word class from an XML file.
    @overload
    @param[in]    filename    filename of the XML file from which to import
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool from_xml( const char* filename );

  /**
    This method imports a Word class from an XMLElement*.

    @brief Import a Word class from an XMLElement*.
    @overload
    @param[in]    word_elem   current XMLElement* to an XML Word element
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement* word_elem );

  /**
      This method imports a Word from a json formatted string.

      @brief This method imports a Word from a json formatted string.
      @param[in]    json_string     json formatting string to read from
      @returns                      success/failure
      @throws                       no expected throws
  */
  virtual bool from_json( const std::string& json_string );

  /**
      This method imports a Word from a json value.

      @brief This method imports a Word from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
    This method exports a Word class to an XML file.

    @brief Export a Word to an XML file.
    @overload
    @param[in]    filename    XML filename to which Word class is exported
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool to_xml( const char* filename ) const;

  /**
    This method exports a Word class to an XMLDocument object. The root
    XMLElement* takes the current Word as a child XMLElement.

    @brief Export a Word to an XMLDocument object.
    @overload
    @param[in]    doc         XMLDocument object for the current document
    @param[in]    root        XMLElement* to accept a Word as a child element
    @returns                  none
    @throws                   no expected throws
  */
  void to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root ) const;

  /**
      This method exports a Word to a json formatted string.

      @brief This method exports a Word to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a Word to a json value.

      @brief This method exports a Word to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  /**
      This method exports a Word to a WordMessage.

      @brief This method exports a Word to a WordMessage.
      @returns                message version of the Word
      @throws                 no expected throws
  */
  WordMessage to_msg( void )const;

  /**
    Word class ostream operator. Prints the Word class to the ostream.

    @brief Word class ostream operator
    @param[in]    out         ostream operator class reference
    @param[in]    other       Word to be printed to ostream
    @returns                  reference to modified ostream operator
    @throws                   no expected throws
  */
  friend std::ostream& operator<<( std::ostream& out, const Word& other );

  /** std::string containing the part-of-speech for the Word */
  std::string pos = "";

  /** std::string containing the text for the Word */
  std::string text = "";

  /** double indicating the time of the Word in the associated sentence*/
  double time = 0.0;

  protected:
  private:

}; // class Word

inline bool word_time_sort( const Word& a, const Word& b ){
  return a.time < b.time;
}

} // namespace h2sl
