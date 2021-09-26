/**
 * @file language-constraint-map.h
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

#include "h2sl/language-constraint.h"

namespace h2sl{

class LanguageConstraintMap{
public:

  /**
    Default constructor for h2sl::LanguageConstraintMap class
  **/
  LanguageConstraintMap() = default;

  /**
    Default copy constructor for h2sl::LanguageConstraintMap class
  **/
  LanguageConstraintMap( const LanguageConstraintMap& ) = default;

  /**
    Default move constructor for h2sl::LanguageConstraintMap class
  **/
  LanguageConstraintMap( LanguageConstraintMap&& ) = default;

  /**
    Default destructor for h2sl::LanguageConstraintMap class
  **/
  ~LanguageConstraintMap() = default;

  /**
    Default copy assignment operator for h2sl::LanguageConstraintMap class
  **/
  LanguageConstraintMap& operator=( const LanguageConstraintMap& ) = default;

  /**
    Default move assignment operator for h2sl::LanguageConstraintMap class
  **/
  LanguageConstraintMap& operator=( LanguageConstraintMap&& ) = default;

  /**                                                                                  
    This method evaluates the stored constraints against a language variable
                                                                                       
    @brief Evaluate the stored LanguageConstraint against an language model
    @param[in]    language_variable    LanguageVariable to evaluate
    @returns                           boolean flag indicating if any constraints failed
    @throws                            Throws runtime_error on parsing error
   */                                                                                  
  bool evaluate( const h2sl::LanguageVariable& language_variable )const;

  /**
    LanguageConstraintMap ostream operator. Prints to the ostream.

    @brief LanguageConstraintMap ostream operator
    @param[in]    out     ostream operator class reference
    @param[in]    other   LanguageConstraintMap reference to be printed to ostream
    @returns              reference to modified ostream object
    @throws               no expected throws
  **/
  friend std::ostream& operator<<( std::ostream& out, const LanguageConstraintMap& other );

  /** Constant to define the class preamble for exception handling **/
  static const char* LanguageConstraintMapPreamble;

  /** Constant to define the string used in XMLElement name for LanguageConstraintMap **/
  static const char* XMLLanguageConstraintMapName;

  /**
    Data member to hold a collection of unique LanguageConstraint objects
    key: unique string associated with the LanguageConstraint
    value: a unique LanguageConstraint object
  **/
  std::map< std::string, LanguageConstraint > language_constraints = std::map< std::string, LanguageConstraint >();
protected:
private:

}; // class LanguageConstraintSet

} // namespace h2sl
