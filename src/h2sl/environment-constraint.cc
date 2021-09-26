/**
 * @file environment-constraint.cc
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

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>

#include "h2sl/environment-constraint.h"

namespace h2sl{


///
/// Parameterized constructor for h2sl::EnvironmentConstraint
///
EnvironmentConstraint::
EnvironmentConstraint( const operand_t& lhsArg,
                       const operand_t& rhsArg,
                       const ConstraintOperator& constraintOperatorArg )
                       : _lhs( lhsArg ), _rhs( rhsArg ), _constraint_operator( constraintOperatorArg ) {}

///
/// Equality operator for h2sl::EnvironmentConstraint
///
bool EnvironmentConstraint::operator==( const EnvironmentConstraint& other )const{
  if( _lhs.uid != other.lhs().uid )
    return false;
  if( _lhs.property != other.lhs().property )
    return false;
  if( _rhs.uid != other.rhs().uid )
    return false;
  if( _rhs.property != other.rhs().property )
    return false;
  if( _constraint_operator != other.constraint_operator() )
    return false;
  return true;
}

///
/// Method to evaluate the environment constraint given a world
///
bool EnvironmentConstraint::evaluate( const h2sl::World& world )const{
  // Find the objects in the world via their uid
  auto it_lhs_object = world.objects.find( _lhs.uid );
  if( it_lhs_object == world.objects.end() ){
    std::stringstream error_msg;
    error_msg << EnvironmentConstraintPreamble << "Could not find an object in the  world matching lhs uid \"" << _lhs.uid << "\".";
    throw std::runtime_error( error_msg.str() );
  }
  auto it_rhs_object = world.objects.find( _rhs.uid );
  if( it_rhs_object == world.objects.end() ){
    std::stringstream error_msg;
    error_msg << EnvironmentConstraintPreamble << "Could not find an object in the world matching rhs uid \"" << _rhs.uid << "\".";
    throw std::runtime_error( error_msg.str() );
  }

  // If the property of interest is the CENTER distance, transform the world
  // TODO

  // Extract and store the values for the object properties of interest
  auto lhs_property = _get_object_property( it_lhs_object->second, _lhs.property );
  auto rhs_property = _get_object_property( it_rhs_object->second, _rhs.property );

  // Compare the properties according to the constraint operator
  switch( _constraint_operator ){
    case EnvironmentConstraint::ConstraintOperator::LESS_THAN:
      if( lhs_property < rhs_property )
        return true;
      break;
    case EnvironmentConstraint::ConstraintOperator::LESS_THAN_OR_EQUAL:
      if( lhs_property <= rhs_property )
        return true;
      break;
    case EnvironmentConstraint::ConstraintOperator::EQUAL:
      if( lhs_property == rhs_property )
        return true;
      break;
    case EnvironmentConstraint::ConstraintOperator::NOT_EQUAL:
      if( lhs_property != rhs_property )
        return true;
      break;
    case EnvironmentConstraint::ConstraintOperator::GREATER_THAN:
      if( lhs_property > rhs_property )
        return true;
      break;
    case EnvironmentConstraint::ConstraintOperator::GREATER_THAN_OR_EQUAL:
      if( lhs_property >= rhs_property )
        return true;
      break;
    default:
      throw std::runtime_error("The stored _constraint_operator did not match an expected ConstraintOperator type.");
      break;
  }  
  return false;
}

///
/// Method to import a EnvironmentConstraint from a file
///
void EnvironmentConstraint::from_file( const char* filename ){
  try{
    if( boost::algorithm::ends_with( filename, "xml" ) ){
      from_xml( filename );
    } else{
      std::stringstream error_msg;
      error_msg << "Erroneous file extension (\"" << filename << "\") if from_file";
      throw error_msg.str();
    }

  }
  catch( std::string msg ){
    // Prepend the class preamble
    msg.insert(0, EnvironmentConstraintPreamble);
    throw std::runtime_error( msg );
  }
  return;
}

///
/// Method to import a EnvironmentConstraint from an XML file
///
void EnvironmentConstraint::from_xml( const char* filename ){
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

  // Check for any EnvironmentConstraint child element of the root element
  const auto*
  environment_constraint_element = root->FirstChildElement( XMLEnvironmentConstraintName );
  if( environment_constraint_element == nullptr ){
    error_msg << "No environment constraint element found as a child of the "
              << "root element for \"" << filename << "\"";
    throw error_msg.str();
  }

  from_xml( environment_constraint_element );
  return;
}

///
/// Method to import a EnvironmentConstraint from an XMLElement*
///
void EnvironmentConstraint::from_xml( const tinyxml2::XMLElement* environment_constraint_element ){
  // Initialize an error_msg for exception handling
  std::stringstream error_msg;

  // Clear any data members
  _lhs = operand_t();
  _rhs = operand_t();

  // Check that the element is an EnvironmentConstraint element
  if( environment_constraint_element == nullptr ){
    error_msg << "The XMLElement* was nullptr";
    throw error_msg.str();
  }
  if( environment_constraint_element->Name() != XMLEnvironmentConstraintName ){
    error_msg << "The XMLElement* name (\"" << environment_constraint_element->Name()
              << "\") did not match the EnvironmentConstraint name (\""
              << XMLEnvironmentConstraintName << "\")";
    throw error_msg.str();
  }

  // Load the data members
  //TODO

  return;
}

///
/// Method to export a EnvironmentConstraint to an XML file
///
void EnvironmentConstraint::to_xml( const char* filename )const{
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
/// Method to export a EnvironmentConstraint to an XMLDocument object
///
void EnvironmentConstraint::to_xml( tinyxml2::XMLDocument& doc,
                                 tinyxml2::XMLElement* root )const
{
  // Export any data members
  // TODO
  return;
}

///
/// Method to generate a concatenated string of the environment constraint
///
std::string EnvironmentConstraint::to_string( void )const{
  std::stringstream constraint_ss;
  constraint_ss << _lhs.uid << "." << object_property_to_string( _lhs.property );
  constraint_ss << constraint_operator_to_string( _constraint_operator );
  constraint_ss << _rhs.uid << "." << object_property_to_string( _rhs.property );
  return constraint_ss.str();
}

///
/// EnvironmentConstraint class ostream operator
///

std::ostream& operator<<( std::ostream& out, const EnvironmentConstraint& other ){
  out << "EnvironmentConstraint(";
  out << "lhs(uid:\"" << other.lhs().uid << "\",";
  out << "property:\"" << EnvironmentConstraint::object_property_to_string( other.lhs().property ) << "\"),";
  out << "constraint_operator(\"" << EnvironmentConstraint::constraint_operator_to_string( other.constraint_operator() ) << "\"),";
  out << "rhs(uid:\"" << other.rhs().uid << "\",";
  out << "property:\"" << EnvironmentConstraint::object_property_to_string( other.rhs().property ) << "\")";
  out << ")";
  return out;
}

///
/// Method to extract a property from an object
///
double EnvironmentConstraint::
_get_object_property( const h2sl::Object& object,
                      const EnvironmentConstraint::ObjectProperty& property )
{
  // initialize the return value
  double ret_property_value = 0.0;
  switch( property ){
    case EnvironmentConstraint::ObjectProperty::POSITION_X:
      ret_property_value = object.state_history.back().pose->x();
      break;
    case EnvironmentConstraint::ObjectProperty::POSITION_Y:
      ret_property_value = object.state_history.back().pose->y();
      break;
    case EnvironmentConstraint::ObjectProperty::POSITION_Z:
      ret_property_value = object.state_history.back().pose->z();
      break;
    case EnvironmentConstraint::ObjectProperty::ABS_POSITION_X:
      ret_property_value = fabs( object.state_history.back().pose->x() );
      break;
    case EnvironmentConstraint::ObjectProperty::ABS_POSITION_Y:
      ret_property_value = fabs( object.state_history.back().pose->y() );
      break;
    case EnvironmentConstraint::ObjectProperty::ABS_POSITION_Z:
      ret_property_value = fabs( object.state_history.back().pose->z() );
      break;
    case EnvironmentConstraint::ObjectProperty::EUCLIDEAN:
      ret_property_value = object.state_history.back().pose->position.norm();
      break;
    case EnvironmentConstraint::ObjectProperty::CENTER:
      ret_property_value = object.state_history.back().pose->position.norm();
      break;
    default:
      throw std::runtime_error("Passed ObjectProperty for _get_object_property did not match an expected ObjectProperty type");
      break;
  }
  return ret_property_value;
}


///
/// Constant used for exception preabmles
///
const char* EnvironmentConstraint::
EnvironmentConstraintPreamble = "[EnvironmentConstraint Class Error] ";

///
/// Constant used for XMLElement name for EnvironmentConstraint
///
const char* EnvironmentConstraint::
XMLEnvironmentConstraintName = "environment-constraint";

} // namespace h2sl
