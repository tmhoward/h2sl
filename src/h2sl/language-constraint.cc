/**
 * @file language-constraint.cc
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

#include "h2sl/language-constraint.h"

namespace h2sl{

///
/// Parameterized constructor for h2sl::LanguageConstraint
///
LanguageConstraint::
LanguageConstraint( const std::string& unigramArg,
                    const ConstraintOperator& constraintOperatorArg )
                    : _unigram( unigramArg ),
                      _constraint_operator( constraintOperatorArg) {}

///
/// Equality operator for h2sl::LanguageConstraint
///
bool LanguageConstraint::operator==( const LanguageConstraint& other )const{
  if( _unigram != other.unigram() )
    return false;
  if( _constraint_operator != other.constraint_operator() )
    return false;
  return true;
}

///
/// Method to evaluate the language constraint given a language variable
///
bool LanguageConstraint::evaluate( const h2sl::LanguageVariable& language_variable )const{
  // Compare the content of phrase according to the constraint operator
  switch( _constraint_operator ){
    case LanguageConstraint::ConstraintOperator::HAS_WORD:
      if( language_variable.words() ){
        for( const auto& word : language_variable.words().value() ){
          if( word.text == _unigram )
            return true;
        }
      }
      break;
    case LanguageConstraint::ConstraintOperator::NO_WORDS:
      if( !language_variable.words() ){
        return true; // TODO
      }
      if( language_variable.words()->size() == 0 )
        return true;
    default:
      throw std::runtime_error("The stored _constraint_operator did not match an expected ConstraintOperator type.");
      break;
  }
  return false;
}

///
/// Method to import a LanguageConstraint from a file
///
void LanguageConstraint::from_file( const char* filename ){
  try{
    if( boost::algorithm::ends_with( filename, "xml" ) ){

    } else{
      std::stringstream error_msg;
      error_msg << "Erroneous file extension (\"" << filename << "\") if from_file";
      throw error_msg.str();
    }

  }
  catch( std::string msg ){
    // Prepend the class preamble
    msg.insert(0, LanguageConstraintPreamble);
    throw std::runtime_error( msg );
  }
  return;
}

///
/// Method to import a LanguageConstraint from an XML file
///
void LanguageConstraint::from_xml( const char* filename ){
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

  // Check for any LanguageConstraint child element of the root element
  const auto*
  language_constraint_element = root->FirstChildElement( XMLLanguageConstraintName );
  if( language_constraint_element == nullptr ){
    error_msg << "No grammar element found as a child of the root element for \""
              << filename << "\"";
    throw error_msg.str();
  }

  from_xml( language_constraint_element );
  return;
}

///
/// Method to import a LanguageConstraint from an XMLElement*
///
void LanguageConstraint::from_xml( const tinyxml2::XMLElement* language_constraint_element ){
  // Initialize an error_msg for exception handling
  std::stringstream error_msg;

  // Clear any data members
  // TODO

  // Check that the element in a LanguageConstraint element
  if( language_constraint_element == nullptr ){
    error_msg << "The XMLElement* was nullptr";
    throw error_msg.str();
  }
  if( language_constraint_element->Name() != XMLLanguageConstraintName ){
    error_msg << "The XMLElement* name (\"" << language_constraint_element->Name()
              << "\") did not match the LanguageConstraint name (\""
              << XMLLanguageConstraintName << "\")";
    throw error_msg.str();
  }

  // Load any data members
  // TODO

  return;
}

///
/// Method to export a LanguageConstraint to an XML file
///
void LanguageConstraint::to_xml( const char* filename )const{
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
/// Method to export a LanguageConstraint to an XMLDocument object
///
void LanguageConstraint::to_xml( tinyxml2::XMLDocument& doc,
                                 tinyxml2::XMLElement* root )const
{
  // Export any data members
  // TODO
  return;
}

///
/// LanguageConstraint class ostream operator
///
std::ostream& operator<<( std::ostream& out, const LanguageConstraint& other ){
  out << "LanguageConstraint( ";
  // TODO
  out << ")";
  return out;
}

///
/// Constant used for exception preabmles
///
const char* LanguageConstraint::
LanguageConstraintPreamble = "[LanguageConstraint Class Error] ";

///
/// Constant used for XMLElement name for LanguageConstraint
///
const char* LanguageConstraint::
XMLLanguageConstraintName = "language-constraint";

} // namespace h2sl
