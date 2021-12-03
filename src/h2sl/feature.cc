/**
 * @file feature.cc
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

#include <fstream>
#include <sstream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <boost/algorithm/string.hpp>

#include "h2sl/feature.h"

namespace h2sl {

///
/// FeatureType class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureType& other) {
  switch( other ){
  case( FeatureType::CorrespondenceVariable ):
    out << "cv";
    break;
  case( FeatureType::Language ):
    out << "language";
    break;
  case( FeatureType::StaticSymbol ):
    out << "static-symbol";
    break;
  case( FeatureType::DynamicSymbol ):
    out << "dynamic-symbol";
    break;
  default:
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureType Class Error] unknown type \"" << other << "\" in operator<<";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    break;
  }
  return out;
}

///
/// FeatureValue class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureValue& other) {
  switch( other ){
  case( FeatureValue::Unknown ):
    out << "unknown";
    break;
  case( FeatureValue::False ):
    out << "false";
    break;
  case( FeatureValue::True ):
    out << "true";
    break;
  default:
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureValue Class Error] unknown value \"" << other << "\" in operator<<";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    break;
  }
  return out;
}

///
/// Feature default class constructor
///
Feature::Feature( const FeatureType& typeArg,
                  const FeatureValue& valueArg,
                  const dependent_parameters_t& dependsOnArg ) : type( typeArg ),
                                                                 value( valueArg ),
                                                                 depends_on( dependsOnArg ){

}

///
/// This method prints out the values of a feature
///
void Feature::print( std::ostream& out )const{
  out << print_string();
  return;
}

///
/// This method generates an EnvironmentConstraint for a given object symbol and world
///
EnvironmentConstraintMap
Feature::generate_environment_constraints(const std::shared_ptr<Symbol>& object_symbol,
                                          const std::shared_ptr<World>& world )const
{
  // The base feature class does not have a constraint
  return EnvironmentConstraintMap();
}

///
///
///
std::vector<LanguageConstraint>
Feature::generate_language_constraints( const std::shared_ptr<LanguageVariable>& lv )const{
  // The base feature class does not have a constraint
  return std::vector<LanguageConstraint>();
}

///
/// Feature class ostream operator
///
std::ostream& operator<<(std::ostream& out, const Feature& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
