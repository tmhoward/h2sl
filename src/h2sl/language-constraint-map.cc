/**
 * @file language-constraint-map.cc
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

#include "h2sl/language-constraint-map.h"

namespace h2sl{

///
/// Method to evaluate the stored constraints against a language variable
///
bool LanguageConstraintMap::
evaluate( const h2sl::LanguageVariable& language_variable )const{
  for( auto const& kv_language_constraint : language_constraints ){
    if( !kv_language_constraint.second.evaluate( language_variable ) ){
      std::cout << "The language constraint \"" << kv_language_constraint.first
                << "\" was evaluated as false." << std::endl;
      return false;
    }
  }
  return true;
}

///
/// LanguageConstraintMap class ostream operator
///
std::ostream& operator<<( std::ostream& out, const LanguageConstraintMap& other ){
  out << "LanguageConstraintMap:\n";
  for( const auto& kv_language_constraint : other.language_constraints ){
    out << kv_language_constraint.second << "\n";
  }
  return out;
}

///
/// Constant used for exception preambles
///
const char* LanguageConstraintMap::
LanguageConstraintMapPreamble = "[LanguageConstraintMap Class Error] ";

///
/// Constant used for XMLElement name for LanguageConstraintMap
///
const char* LanguageConstraintMap::
XMLLanguageConstraintMapName = "language-constraint-map";
} // namespace h2sl
