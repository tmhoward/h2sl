/**
 * @file environment-constraint.h
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
#include "h2sl/world.h"

namespace h2sl{

/**

**/

class EnvironmentConstraint{
public:

  /**
    An enum class to provide types of the operator of an environment constraint. These
    are standard boolean comparison operators.
  **/
  enum class ConstraintOperator{ LESS_THAN,
                                 LESS_THAN_OR_EQUAL,
                                 EQUAL,
                                 NOT_EQUAL,
                                 GREATER_THAN,
                                 GREATER_THAN_OR_EQUAL };

  /* Methods to import/export ConstraintOperator types as strings */
  inline static std::string
  constraint_operator_to_string( const ConstraintOperator& constraint_operator ){
    std::string ret_string;
    switch( constraint_operator ){
      case ConstraintOperator::LESS_THAN:
        ret_string = "<";
        break;
      case ConstraintOperator::LESS_THAN_OR_EQUAL:
        ret_string = "<=";
        break;
      case ConstraintOperator::EQUAL:
        ret_string = "==";
        break;
      case ConstraintOperator::NOT_EQUAL:
        ret_string = "!=";
        break;
      case ConstraintOperator::GREATER_THAN:
        ret_string = ">";
        break;
      case ConstraintOperator::GREATER_THAN_OR_EQUAL:
        ret_string = ">=";
        break;
      default:
        throw std::runtime_error("Passed constraint_operator did not match a ConstraintOperator type");
        break;
    }
    return ret_string;
  }

  inline static EnvironmentConstraint::ConstraintOperator
  string_to_constraint_operator( const std::string& constraint_operator_string ){
    if( constraint_operator_string == "<" )
      return ConstraintOperator::LESS_THAN;
    if( constraint_operator_string == "<=" )
      return ConstraintOperator::LESS_THAN_OR_EQUAL;
    if( constraint_operator_string == "==" )
      return ConstraintOperator::EQUAL;
    if( constraint_operator_string == "!=" )
      return ConstraintOperator::NOT_EQUAL;
    if( constraint_operator_string == ">" )
      return ConstraintOperator::GREATER_THAN;
    if( constraint_operator_string == ">=" )
      return ConstraintOperator::GREATER_THAN_OR_EQUAL;
    throw std::runtime_error("Passed constraint_operator_string did not match a ConstraintOperator type");
  }

  /**
    An enum class to provide types for an object's property to be used by the
    environment constraint.
  **/
  enum class ObjectProperty{ POSITION_X,
                             POSITION_Y,
                             POSITION_Z,
                             ABS_POSITION_X,
                             ABS_POSITION_Y,
                             ABS_POSITION_Z,
                             EUCLIDEAN,
                             CENTER };

  /** Methods to export/import ObjectProperty types as strings */
  inline static std::string object_property_to_string(const ObjectProperty& property){
    std::string ret_string;
    switch( property ){
      case ObjectProperty::POSITION_X:
        ret_string = "x";
        break;
      case ObjectProperty::POSITION_Y:
        ret_string = "y";
        break;
      case ObjectProperty::POSITION_Z:
        ret_string = "z";
        break;
      case ObjectProperty::ABS_POSITION_X:
        ret_string = "abs_x";
        break;
      case ObjectProperty::ABS_POSITION_Y:
        ret_string = "abs_y";
        break;
      case ObjectProperty::ABS_POSITION_Z:
        ret_string = "abs_z";
        break;
      case ObjectProperty::EUCLIDEAN:
        ret_string = "euclidean";
        break;
      case ObjectProperty::CENTER:
        ret_string = "center";
        break;
      default:
        throw std::runtime_error("Passed ObjectProperty did not match an ObjectProperty type");
        break;
    }
    return ret_string;
  }

  inline static EnvironmentConstraint::ObjectProperty
  string_to_object_property( const std::string& property_string ){
    if( property_string == "x" )
      return ObjectProperty::POSITION_X;
    if( property_string == "y" )
      return ObjectProperty::POSITION_Y;
    if( property_string == "z" )
      return ObjectProperty::POSITION_Z;
    if( property_string == "abs_x" )
      return ObjectProperty::ABS_POSITION_X;
    if( property_string == "abs_y" )
      return ObjectProperty::ABS_POSITION_Y;
    if( property_string == "abs_z" )
      return ObjectProperty::ABS_POSITION_Z;
    if( property_string == "euclidean" )
      return ObjectProperty::EUCLIDEAN;
    if( property_string == "center" )
      return ObjectProperty::CENTER;
    throw std::runtime_error("Passed property string did not match an ObjectProperty type");
  }

  /**
    A structure to contain the constituents of an operand of a boolean comparison
    for the environment constraint class.
  **/
  struct operand_t{
    std::string uid;
    ObjectProperty property;
  };

  /**
    Do not allow default constructor for h2sl::EnvironmentConstraint class
  **/
  EnvironmentConstraint() = delete;

  /**
    Parameterized constructor for h2sl::EnvironmentConstraint class

    @brief Parameterized argument constructor for EnvironmentConstraint
    @param[in]    rhsArg                  rhs operand of the constraint
    @param[in]    lhsArg                  lhs operand of the constraint
    @param[in]    constraintOperatorArg   operator of the constraint
    @returns                              none
    @throws                               no throws expected
  **/
  EnvironmentConstraint( const operand_t& lhsArg,
                         const operand_t& rhsArg,
                         const ConstraintOperator& constraintOperatorArg );

  /**
    Default copy constructor for h2sl::EnvironmentConstraint class
  **/
  EnvironmentConstraint( const EnvironmentConstraint& ) = default;

  /**
    Default move constructor for h2sl::EnvironmentConstraint class
  **/
  EnvironmentConstraint( EnvironmentConstraint&& ) = default;

  /**
    Default destructor for h2sl::EnvironmentConstraint class
  **/
  ~EnvironmentConstraint() = default;

  /**
    Default copy assignment operator for h2sl::EnvironmentConstraint class
  **/
  EnvironmentConstraint& operator=( const EnvironmentConstraint& ) = default;

  /**
    Default move assignment operator for h2sl::EnvironmentConstraint class
  **/
  EnvironmentConstraint& operator=( EnvironmentConstraint&& ) = default;

  /**
    Equality operator for h2sl::EnvironmentConstraint class
  **/
  bool operator==( const EnvironmentConstraint& other )const;

  /**
    This method evaluates the EnvironmentConstraint against an environment model

    @brief Evaluates the EnvironmentConstraint against an environment model
    @param[in]    world     world model
    @returns                boolean flag indicating the state of the constraint
    @throws                 Throws runtime_error if objects are not found in the world
   */
  bool evaluate( const h2sl::World& world )const;

  /**
    This method imports an EnvironmentConstraint from a file

    @brief Import an EnvironmentConstraint from a file.
    @param[in]    filename    filename of the file from which to import
    @returns                  none
    @throws                   Throws runtime_error on parsing error
  **/
  void from_file( const char* filename );

  /**
    This method imports an EnvironmentConstraint from an XML file

    @brief Import an EnvironmentConstraint from an XML file.
    @overload
    @param[in]    filename    filename of the XML file from which to import
    @returns                  none
    @throws                   Throws runtime_error on XML parsing error
  **/
  void from_xml( const char* filename );

  /**
    This method imports an EnvironmentConstraint from an XMLElement*.

    @brief Import an EnvironmentConstraint from a tinyxml2 XMLElement*
    @overload
    @param[in]  environment_constraint_element   XMLElement* to environment constraint element
    @returns                                     none
    @throws                                      Throws runtime_error on XML parsing error
  **/
  void from_xml( const tinyxml2::XMLElement* environment_constraint_element );

  /**
    This method exports an EnvironmentConstraint to an XML file

    @brief Export an EnvironmentConstraint to an XML file
    @overload
    @param[in]    filename    filename of the XML to which the environment constraint is exported
    @returns                  none
    @throws                   Throws runtime_error on XML of file write error
  **/
  void to_xml( const char* filename )const;

  /**
    This method exports an EnvironmentConstraint to an XMLDocument object.
    The root XMLElement* takes the current environment constraint as a child XMLElement.

    @brief Export an EnvironmentConstraint to an XMLDocument object
    @param[in]    doc   XMLDocument object for the current document
    @param[in]    root  XMLElement* to accept the EnvironmentConstraint as a child
    @returns            none
    @throws             Throws runtime_error on XML write error
  **/
  void to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root )const;

  /**
    This method generates a concatenated string from the environment constraint.

    @brief Generate a concatenated string of the environment constraint
    @returns          A string representing the environment constraint
    @throws           No expected throws
  **/
  std::string to_string( void )const;

  /**
    EnvironmentConstraint ostream operator. Prints the EnvironmentConstraint to ostream.

    @brief EnvironmentConstraint ostream operator
    @param[in]    out     ostream operator class reference
    @param[in]    other   EnvironmentConstraint reference to be printed to ostream
    @returns              reference to modified ostream object
    @throws               no expected throws
  **/
  friend std::ostream& operator<<( std::ostream& out, const EnvironmentConstraint& other );

  /**
    This method provides read-only access to the _lhs data member

    @brief Provide read-only access to _lhs
    @returns    const reference to _lhs
    @throws     no expected throws
  **/
  inline const operand_t&
  lhs( void )const{ return _lhs; }

  /**
    This method provides read-only access to the _rhs data member

    @brief Provide read-only access to _rhs
    @returns    const reference to _rhs
    @throws     no expected throws
  **/
  inline const operand_t&
  rhs( void )const{ return _rhs; }

  /**
    This method provides read-only access to the _constraint_operator data member

    @brief Provide read-only access to _constraint_operator
    @returns    const reference to _constraint_operator
    @throws     no expected throws
  **/
  inline const ConstraintOperator&
  constraint_operator( void )const{ return _constraint_operator; }

  /** Constant to define the class preamble for exception handling */
  static const char* EnvironmentConstraintPreamble;

  /** Constant to define the string used in XMLElement name for EnvironmentConstraint */
  static const char* XMLEnvironmentConstraintName;


protected:
private:
  /**
    Data members to define a constraint. lhs and rhs are operands of the constraint
    that inform the uid and the property of the object to be compared.
    constraint_operator is an enum type that specifies what comparison operator to use.

    These members are made private to enforce class invariance.
  **/
  operand_t _lhs;
  operand_t _rhs;
  ConstraintOperator _constraint_operator;

  /*
    Method to extract the relevant property from a passed object
  */
  static double
  _get_object_property( const h2sl::Object& object,
                        const EnvironmentConstraint::ObjectProperty& property );

}; // class EnvironmentConstraint

} // h2sl namespace
