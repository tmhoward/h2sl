/**
 * @file language-constraint.h
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

#include "h2sl/common.h"
#include "h2sl/language-variable.h"

namespace h2sl{

/**

**/

class LanguageConstraint{
public:

  /**
    An enum class to provide types of the operator of a langauge constraint. These are
    operator types for language comparison and return true/false.
  **/
  enum class ConstraintOperator{ HAS_WORD,
                                 NO_WORDS };

  /* Methods to import/export ConstraintOperator types as string */
  inline static std::string
  constraint_operator_to_string( const ConstraintOperator& constraint_operator ){
    std::string ret_string;
    switch( constraint_operator ){
      case ConstraintOperator::HAS_WORD:
        ret_string = "has_word";
        break;
      case ConstraintOperator::NO_WORDS:
        ret_string = "no_words";
        break;
      default:
        throw std::runtime_error("Passed constraint_operator did not match a ConstraintOperator type");
        break;
    }
    return ret_string;
  }

  inline static LanguageConstraint::ConstraintOperator
  string_to_constraint_operator( const std::string& constraint_operator_string ){
    if( constraint_operator_string == "has_word" )
      return ConstraintOperator::HAS_WORD;
    if( constraint_operator_string == "no_words" )
      return ConstraintOperator::NO_WORDS;
    throw std::runtime_error("Passed constraint_operator did not match a ConstraintOperator type");
  }

  /**
    Default constructor for h2sl::LanguageConstraint class
  **/
  LanguageConstraint() = delete;

  /**
    Parameterized constructor for h2sl::LanguageConstraint class

    @brief Parameterized argument constructor for LanguageConstraint
    @param[in]    unigramArg            string of the expected word
    @param[in]    constraintOperatorArg operator of the constraint
    @returns                            none
    @throws                             no expected throws
  **/
  LanguageConstraint( const std::string& unigramArg,
                      const ConstraintOperator& constraintOperatorArg );

  /**
    Default copy constructor for h2sl::LanguageConstraint class
  **/
  LanguageConstraint( const LanguageConstraint& ) = default;

  /**
    Default move constructor for h2sl::LanguageConstraint class
  **/
  LanguageConstraint( LanguageConstraint&& ) = default;

  /**
    Default destructor for h2sl::LanguageConstraint class
  **/
  ~LanguageConstraint() = default;

  /**
    Default copy assignment operator for h2sl::LanguageConstraint class
  **/
  LanguageConstraint& operator=( const LanguageConstraint& ) = default;

  /**
    Default move assignment operator for h2sl::LanguageConstraint class
  **/
  LanguageConstraint& operator=( LanguageConstraint&& ) = default;

  /**
    Equality operator for h2sl::LanguageConstraint class
  **/
  bool operator==( const LanguageConstraint& other )const;

  /**
    This method evaluates the LanguageConstraint against a language variable

    @brief Evaluates the LanguageConstraint against a language variable object
    @param[in]    language_variable     language variable object
    @returns                            boolean flag indicating the state of the constraint
    @throws                             Throws runtime_error if enum types are violated
  **/
  bool evaluate( const h2sl::LanguageVariable& language_variable )const;

  /**
    This method imports a LanguageConstraint from a file

    @brief Import a LanguageConstraint from a file.
    @param[in]    filename    filename of the file from which to import
    @returns                  none
    @throws                   Throws runtime_error on parsing error
  **/
  void from_file( const char* filename );

  /**
    This method imports a LanguageConstraint from an XML file

    @brief Import a LanguageConstraint from an XML file.
    @overload
    @param[in]    filename    filename of the XML file from which to import
    @returns                  none
    @throws                   Throws runtime_error on XML parsing error
  **/
  void from_xml( const char* filename );

  /**
    This method imports a LanguageConstraint from an XMLElement*.

    @brief Import a LanguageConstraint from a tinyxml2 XMLElement*
    @overload
    @param[in]  language_constraint_element   XMLElement* to language constraint element
    @returns                                  none
    @throws                                   Throws runtime_error on XML parsing error
  **/
  void from_xml( const tinyxml2::XMLElement* language_constraint_element );

  /**
    This method export a LanguageConstraint to an XML file

    @brief Export a LanguageConstraint to an XML file
    @overload
    @param[in]    filename    filename of the XML to which the language constraint is exported
    @returns                  none
    @throws                   Throws runtime_error on XML file write error
  **/
  void to_xml( const char* filename )const;

  /**
    This method exports a LanguageConstraint to an XMLDocument object.
    The root XMLElement* takes the current language constraint as a child XMLElement.

    @brief Export a LanguageConstraint to an XMLDocument object
    @param[in]    doc   XMLDocument object for the current document
    @param[in]    root  XMLElement* to accept the LanguageConstraint as a child
    @returns            none
    @throws             Throws runtime_error on XML write error
  **/
  void to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root )const;

  /**
    LanguageConstraint ostream operator. Prints the LanguageConstraint to ostream.

    @brief LanguageConstraint ostream operator
    @param[in]    out     ostream operator class reference
    @param[in]    other   LanguageConstraint reference to be printed to ostream
    @returns              reference to modified ostream object
    @throws               no expected throws
  **/
  friend std::ostream& operator<<( std::ostream& out, const LanguageConstraint& other );

  /**
    This method provides read-only access to the _unigram data member

    @brief Provide read-only access to _unigram
    @returns    const reference to _unigram
    @throws     no expected throws
  **/
  inline const std::string&
  unigram( void )const{ return _unigram; }

  /**
    This method provides read-only access to the _constraint_operator data member

    @brief Provide read-only access to _constraint_operator
    @return   const reference to _constraint_operator
    @throws   no expected throws
  **/
  inline const ConstraintOperator&
  constraint_operator( void )const{ return _constraint_operator; }

  /** Constant to define the class preamble for exception handling */
  static const char* LanguageConstraintPreamble;

  /** Constant to define the string used in XMLElement name for LanguageConstraint */
  static const char* XMLLanguageConstraintName;

protected:
private:
  /**
    Data members to define a language constraint.
    _unigram is an expected word to be present in the language variable.
    _constraint_operator is an enum type that specifies the comparison operation to perform.
  **/
  std::string _unigram;
  ConstraintOperator _constraint_operator;

}; // class LanguageConstraint

} // h2sl namespace
