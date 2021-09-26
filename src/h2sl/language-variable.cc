/**
 * @file language-variable.cc
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

#include <algorithm>

#include <fstream>
#include <sstream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

#include "h2sl/language-variable.h"

namespace h2sl{

//
// LanguageVariable class default constructor
//
LanguageVariable::LanguageVariable( const std::string& typeArg,
                const std::string& nameArg,
                const symbolsVector& symbolsArg,
                const std::vector<language_variable_connection_t>& childrenArg,
                const std::optional< std::vector<Word> >& wordsArg,
                const std::optional< rolesMap >& rolesArg ) : type( typeArg ),
                                                              name( nameArg ),
                                                              symbols( symbolsArg ),
                                                              children( childrenArg ),
                                                              words( wordsArg ),
                                                              roles( rolesArg ){}

//
// LanguageVariable class constructor from XMLElement*
//
LanguageVariable::LanguageVariable( const tinyxml2::XMLElement* root, std::shared_ptr<SymbolSpace>& symbolSpace )
    : type(), name(), symbols(), children(), words(), roles()
{
  from_xml( root, symbolSpace );
}

//
// This method imports a LanguageVariable in flattened format from an XML file.
//
std::optional<LanguageVariable> LanguageVariable::flattened_from_xml( const char* filename ){
  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename );

  // Check for loading errors
  if( doc.Error() ){
    std::cout << "Error loading document \"" << filename << "\"" << std::endl;
    return std::nullopt;
  }

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr ){
    std::cout << "No root element found in file \"" << filename << "\"" << std::endl;
    return std::nullopt;
  }

  // Check for at least one LanguageVariable child element of the root
  const tinyxml2::XMLElement* language_variable_elem = root->FirstChildElement("language-variable");
  if( language_variable_elem == nullptr ) return std::nullopt;

  return flattened_from_xml( root );
}

//
// This method imports a LanguageVariable in flattened format from an XML file.
//
std::optional<LanguageVariable> LanguageVariable::flattened_from_xml( const char* filename,
        std::shared_ptr<SymbolSpace>& symbolspace ){
  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename );

  // Check for loading errors
  if( doc.Error() ){
    std::cout << "Error loading document \"" << filename << "\"" << std::endl;
    return std::nullopt;
  }

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr ){
    std::cout << "No root element found in file \"" << filename << "\"" << std::endl;
    return std::nullopt;
  }

  // Check for at least one LanguageVariable child element of the root
  const tinyxml2::XMLElement* language_variable_elem = root->FirstChildElement("language-variable");
  if( language_variable_elem == nullptr ) return std::nullopt;

  return flattened_from_xml( root, symbolspace );
}

//
// This method imports a LanguageVariable from a LanguageVariableMessage.
//
std::optional<LanguageVariable> LanguageVariable::from_msg( const LanguageVariableMessage& msg ){
  // Create struct representation of flattened LV
  auto flattened_info = flattened_language_variable_t();

  // Import each node in the message
  for( const auto& node : msg.nodes ){

    // Import child Symbols as this LanguageVariable's symbols
    auto lv_symbols = symbolsVector(); 
    
    for( const auto& symbol_msg : node.symbols ){
      lv_symbols.emplace_back( std::make_shared<Symbol>( symbol_msg ) ); // Construct and add Symbol to the vector
    }

    // Import any words from the node
    auto lv_words = std::vector<Word>();
    
    for( const auto& word_msg : node.words ){
      lv_words.emplace_back( word_msg );
    }

    // Import any roles from the node
    auto lv_roles = rolesMap();
    
    for( const auto& role_msg : node.roles ){
      lv_roles.emplace( role_msg.name, role_msg.value );
    }
 
    // Attempt to add new node with found information
    auto node_it = flattened_info.nodes.find( node.name );
    if( node_it != flattened_info.nodes.end() ){ // Duplicate variable name
      std::cout << "Warning: Duplicate variable name \"" << node.name << "\"" << std::endl;
      return std::nullopt;
    }
    
    auto node_info = language_variable_node_t( node.type, node.name, lv_symbols, lv_words, lv_roles );
    flattened_info.nodes.emplace( node.name, node_info );
    
    // Import child connections of the language variable
    for( const auto& connection_msg : node.children ){
      std::optional< std::string > label = std::nullopt;
      if( connection_msg.label_exists ){
        label = std::make_optional<std::string>( connection_msg.label );
      } // Otherwise no label on the connection
      flattened_info.edges.emplace_back( node.name, label, connection_msg.child_name );
    }
  } // End node loop

  // Find the root of the imported LV information
  std::optional<std::string> root_name = find_root( flattened_info );
  if( root_name ){ // Construct and return the LanguageVariable
    return construct_lv( *root_name, flattened_info );
  } else{ // No root LV was found
    return std::nullopt;
  }
}

//
// Method to clear all Symbols from this and all children language variables
//
void LanguageVariable::clear_symbols( void ){
  symbols.clear();

  // Clear symbols from all children LanguageVariables
  for( auto& connection : children ){
    connection.child->clear_symbols();
  }
}

//
// Method to generate a unique key for the language variable
//
std::string LanguageVariable::key( void ) const{
  // TODO - Just use unique name
  std::stringstream out;
  out << type << "{";

  // Output words for this LanguageVariable if they exist (does not include words from children)
  if( words ){
    out << "words(";
    for( auto it_word = words->cbegin(); it_word != words->cend(); ++it_word ){
      out << *it_word;
      if( std::next(it_word) != words->cend() )
        out << ",";
    }
    out << ")";
  }

  // Output roles for this LanguageVariable if they exist  
  if( roles ){
    out << "roles(";
    bool first = true;
    for( std::pair<std::string,std::string> role_pair : roles.value() ){
      if( !first ){
        out << ",";
      }
      out << role_pair.first << ":" << role_pair.second;
      first = false;
    }
    out << ")";
  }

  // Output children 
  if( children.size() > 0 ){
    out << ",children(";
    for( auto it_child = children.cbegin();
              it_child != children.cend();
              ++it_child )
    { // Child may or may not have an edge label
      if ( it_child->label ) {
        out << "[" << it_child->label.value() << "]:";
      }
      out << it_child->child->key();
      if( std::next(it_child) != children.cend() )
        out << ",";
    }
    out << ")";
  }
  out << "}";
  return out.str();
}

//
// Method to recursively convert the words member to a string, including children
//
std::string LanguageVariable::words_to_std_string( void ) const{
  std::string all_words_str;
  auto all_words_vec = all_words();
  std::sort( all_words_vec.begin(), all_words_vec.end(), word_time_sort );
  for( auto it_word = all_words_vec.cbegin(); 
      it_word != all_words_vec.cend(); ++it_word )
  {
    all_words_str = all_words_str + it_word->text;
    if( std::next(it_word) != all_words_vec.cend() )
      all_words_str = all_words_str + " ";
  }
  return all_words_str;
}

//
// Method to recursively return all Words in a vector, including from children
//
std::vector<Word> LanguageVariable::all_words( void ) const{
  std::vector<Word> all_words_vec;
  for( auto const& child_connection : children ){
    std::vector<Word> child_words_vec = child_connection.child->all_words();
    all_words_vec.insert( all_words_vec.end(),
                          child_words_vec.begin(),
                          child_words_vec.end() );
  }
  if( words ){
    all_words_vec.insert( all_words_vec.end(), words->begin(), words->end() );
  }
  return all_words_vec;
}

//
// Method to import a LanguageVariable in hierarchical format from an XML file
//
bool LanguageVariable::from_xml( const char* filename ){
  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename );

  // Check for loading errors
  if( doc.Error() ){
    std::cout << "Error loading document \"" << filename << "\"" << std::endl;
    return false;
  }

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr ){
    std::cout << "No root element found in file \"" << filename << "\"" << std::endl;
    return false;
  }

  // Check for any LanguageVariable child element of the root
  const tinyxml2::XMLElement* language_variable_elem = root->FirstChildElement("language-variable");
  if( language_variable_elem == nullptr ) return false;

  return from_xml( language_variable_elem );
}

//
// Method to import a LanguageVariable in hierarchical format from an XML file
//
bool LanguageVariable::from_xml( const char* filename, std::shared_ptr<SymbolSpace>& symbolspace ){
  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename );

  // Check for loading errors
  if( doc.Error() ){
    std::cout << "Error loading document \"" << filename << "\"" << std::endl;
    return false;
  }

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr ){
    std::cout << "No root element found in file \"" << filename << "\"" << std::endl;
    return false;
  }

  // Check for any LanguageVariable child element of the root
  const tinyxml2::XMLElement* language_variable_elem = root->FirstChildElement("language-variable");
  if( language_variable_elem == nullptr ) return false;

  return from_xml( language_variable_elem, symbolspace );
}

//
// Method to import a LanguageVariable in hierarchical format from an XMLElement*
//
bool LanguageVariable::from_xml( const tinyxml2::XMLElement* language_variable_elem ){
  type = "";
  name = "";
  symbols.clear();
  children.clear();
  words = std::nullopt;
  roles = std::nullopt;

  // Check that the element is a language variable element
  if( language_variable_elem == nullptr ) return false;
  if( std::strcmp(language_variable_elem->Name(), "language-variable") != 0 ) return false;

  // Read the type attribute and set it to this LanguageVariable's type
  const tinyxml2::XMLAttribute* type_attr = language_variable_elem->FindAttribute("type");
  if( type_attr == nullptr ){
    std::cout << "failed to find language-variable type" << std::endl;
    return false;
  }
  type = type_attr->Value();
  
  // Read the name attribute and set it to this LanguageVariable's name
  const tinyxml2::XMLAttribute* name_attr = language_variable_elem->FindAttribute("name");
  if( name_attr == nullptr ){
    std::cout << "failed to find language-variable name" << std::endl;
    return false;
  }
  name = name_attr->Value();

  // Import child XMLElement Words as this LanguageVariable's words
  const tinyxml2::XMLElement* word_elem = language_variable_elem->FirstChildElement("word");
  if( word_elem != nullptr ){
    words = std::make_optional< std::vector<Word> >(); // Initialize words vector
  }
  while( word_elem != nullptr ){
    Word word = Word();

    // Check that the Word is imported correctly
    if( !word.from_xml( word_elem ) ){
      std::cout << "failed to load word" << std::endl;
      return false;
    }
    words->push_back( word );
    word_elem = word_elem->NextSiblingElement("word");
  }

  // Import roles from any other attributes
  const tinyxml2::XMLAttribute* role_attr = language_variable_elem->FirstAttribute();
  while( role_attr != nullptr ){
    // Skip type/name/text attributes; these are not roles
    if( std::strcmp( role_attr->Name(), "type" ) == 0 || std::strcmp( role_attr->Name(), "name" ) == 0 ||
        std::strcmp( role_attr->Name(), "text" ) == 0 ){
      role_attr = role_attr->Next();
      continue;
    }
    
    // Insert the role
    if( !roles ){
      roles = std::make_optional<rolesMap>();
    }
    roles->emplace( role_attr->Name(), role_attr->Value() );
    role_attr = role_attr->Next();
  }

  // Import child XMLElement Symbols as this LanguageVariable's symbols
  const tinyxml2::XMLElement* symbol_elem = language_variable_elem->FirstChildElement("symbol");
  while( symbol_elem != nullptr ){
    Symbol symbol = Symbol();

    // Check that the Symbol is imported correctly
    if( !symbol.from_xml( symbol_elem ) )
      return false;
    symbols.push_back( std::make_shared<Symbol>(symbol) );
    symbol_elem = symbol_elem->NextSiblingElement("symbol");
  }

  // Import child XMLElement language_variable_connection_t as this LanguageVariable's children
  const tinyxml2::XMLElement* child_connection_elem =
                              language_variable_elem->FirstChildElement("child-connection");
  while( child_connection_elem != nullptr ){
    language_variable_connection_t connection;
    connection.child = std::make_shared<LanguageVariable>();
    
    const tinyxml2::XMLAttribute* edge_label_attr = child_connection_elem->FindAttribute("label");
    if( edge_label_attr != nullptr ){
      connection.label = edge_label_attr->Value();
    }
    
    const tinyxml2::XMLElement* child_language_variable_elem = child_connection_elem->FirstChildElement("language-variable");
    if( child_language_variable_elem == nullptr ){
      std::cout << "failed to find child language variable" << std::endl;
      return false;
    }
    
    // Check that the child LanguageVariable is imported correctly
    if( !connection.child->from_xml( child_language_variable_elem ) )
      return false;
    
    children.push_back( connection );
    child_connection_elem = child_connection_elem->NextSiblingElement("child-connection");
  }

  // Allow import of child XMLElement LanguageVariable children directly
  const tinyxml2::XMLElement* child_elem = language_variable_elem->FirstChildElement("language-variable");
  while( child_elem != nullptr ){
    language_variable_connection_t connection;
    connection.child = std::make_shared<LanguageVariable>(); // We leave the edge label empty

    // Check that the child LanguageVariable is imported correctly
    if( !connection.child->from_xml( child_elem ) )
      return false;

    children.push_back( connection );
    child_elem = child_elem->NextSiblingElement("language-variable");
  }

  return true;
}

//
// Method to import a LanguageVariable in hierarchical format from an XMLElement*
//
bool LanguageVariable::from_xml( const tinyxml2::XMLElement* language_variable_elem,
                       std::shared_ptr<SymbolSpace>& symbolspace ){
  type = "";
  name = "";
  symbols.clear();
  children.clear();
  words = std::nullopt;
  roles = std::nullopt;

  // Check that the element is a language variable element
  if( language_variable_elem == nullptr ) return false;
  if( std::strcmp(language_variable_elem->Name(), "language-variable") != 0 ) return false;

  // Read the type attribute and set it to this LanguageVariable's type
  const tinyxml2::XMLAttribute* type_attr = language_variable_elem->FindAttribute("type");
  if( type_attr == nullptr ){
    std::cout << "failed to find language-variable type" << std::endl;
    return false;
  }
  type = type_attr->Value();
  
  // Read the name attribute and set it to this LanguageVariable's name
  const tinyxml2::XMLAttribute* name_attr = language_variable_elem->FindAttribute("name");
  if( name_attr == nullptr ){
    std::cout << "failed to find language-variable name" << std::endl;
    return false;
  }
  name = name_attr->Value();

  // Import child XMLElement Words as this LanguageVariable's words
  const tinyxml2::XMLElement* word_elem = language_variable_elem->FirstChildElement("word");
  if( word_elem != nullptr ){
    words = std::make_optional< std::vector<Word> >(); // Initialize words vector
  }
  while( word_elem != nullptr ){
    Word word = Word();

    // Check that the Word is imported correctly
    if( !word.from_xml( word_elem ) ){
      std::cout << "failed to load word" << std::endl;
      return false;
    }
    words->push_back( word );
    word_elem = word_elem->NextSiblingElement("word");
  }

  // Import roles from any other attributes
  const tinyxml2::XMLAttribute* role_attr = language_variable_elem->FirstAttribute();
  while( role_attr != nullptr ){
    // Skip the type/name/text attributes; these are not roles
    if( std::strcmp( role_attr->Name(), "type" ) == 0 || std::strcmp( role_attr->Name(), "name" ) == 0 ||
        std::strcmp( role_attr->Name(), "text" ) == 0 ){
      role_attr = role_attr->Next();
      continue;
    }
    
    // Insert the role
    if( !roles ){
      roles = std::make_optional<rolesMap>();
    }
    roles->emplace( role_attr->Name(), role_attr->Value() );
    role_attr = role_attr->Next();
  }

  // Import child XMLElement Symbols as this LanguageVariable's symbols
  const tinyxml2::XMLElement* symbol_elem = language_variable_elem->FirstChildElement("symbol");
  while( symbol_elem != nullptr ){
    Symbol symbol = Symbol();

    // Check that the Symbol is imported correctly
    if( !symbol.from_xml( symbol_elem ) )
      return false;
    symbols.push_back( symbolspace->insert(symbol) );
    symbol_elem = symbol_elem->NextSiblingElement("symbol");
  }

  // Import child XMLElement language_variable_connection_t as this LanguageVariable's children
  const tinyxml2::XMLElement* child_connection_elem =
                              language_variable_elem->FirstChildElement("child-connection");
  while( child_connection_elem != nullptr ){
    language_variable_connection_t connection;
    connection.child = std::make_shared<LanguageVariable>();
    
    const tinyxml2::XMLAttribute* edge_label_attr = child_connection_elem->FindAttribute("label");
    if( edge_label_attr != nullptr ){
      connection.label = edge_label_attr->Value();
    }
    
    const tinyxml2::XMLElement* child_language_variable_elem = child_connection_elem->FirstChildElement("language-variable");
    if( child_language_variable_elem == nullptr ){
      std::cout << "failed to find child language variable" << std::endl;
      return false;
    }
    
    // Check that the child LanguageVariable is imported correctly
    if( !connection.child->from_xml( child_language_variable_elem, symbolspace ) )
      return false;
    
    children.push_back( connection );
    child_connection_elem = child_connection_elem->NextSiblingElement("child-connection");
  }

  // Allow import of child XMLElement LanguageVariable children directly
  const tinyxml2::XMLElement* child_elem = language_variable_elem->FirstChildElement("language-variable");
  while( child_elem != nullptr ){
    language_variable_connection_t connection;
    connection.child = std::make_shared<LanguageVariable>(); // We leave the edge label empty

    // Check that the child LanguageVariable is imported correctly
    if( !connection.child->from_xml( child_elem, symbolspace ) )
      return false;

    children.push_back( connection );
    child_elem = child_elem->NextSiblingElement("language-variable");
  }

  return true;
}

//
// Method to import a LanguageVariable in flattened format from an XMLElement*
//
std::optional<LanguageVariable> LanguageVariable::flattened_from_xml( const tinyxml2::XMLElement* root_elem ){
  // Create flattened representation struct
  auto flattened_info = flattened_language_variable_t();

  // Check that the element is a root element
  if( root_elem == nullptr ) return std::nullopt;
  if( std::strcmp(root_elem->Name(), "root") != 0 ) return std::nullopt;
  
  // Import speaker uid if one has been annotated
  std::optional<std::string> speaker = std::nullopt;
  const tinyxml2::XMLElement* instruction_elem = root_elem->FirstChildElement("instruction");
  if( instruction_elem != nullptr ){
    const tinyxml2::XMLAttribute* speaker_attr = instruction_elem->FindAttribute("uid");
    if( speaker_attr != nullptr ){
      speaker = std::make_optional<std::string>( speaker_attr->Value() );
    }
  }

  // Import child truncated LanguageVariable items into nodes and edges
  const tinyxml2::XMLElement* lv_elem = root_elem->FirstChildElement("language-variable");
  while( lv_elem != nullptr ){
    const tinyxml2::XMLAttribute* name_attr = lv_elem->FindAttribute("name");
    if( name_attr == nullptr ){
      std::cout << "Failed to find language-variable name" << std::endl;
      return std::nullopt;
    }

    const tinyxml2::XMLAttribute* type_attr = lv_elem->FindAttribute("type");
    if( type_attr == nullptr ){
      std::cout << "Failed to find language-variable type" << std::endl;
      return std::nullopt;
    }

    // Import child XMLElement Symbols as this LanguageVariable's symbols
    auto lv_symbols = symbolsVector(); 
    
    const tinyxml2::XMLElement* symbol_elem = lv_elem->FirstChildElement("symbol");
    while( symbol_elem != nullptr ){
      h2sl::Symbol symbol = h2sl::Symbol();

      // Check that the Symbol is imported correctly
      if( !symbol.from_xml( symbol_elem ) ){
        return std::nullopt;
      }

      lv_symbols.push_back( std::make_shared<Symbol>(symbol) );
      symbol_elem = symbol_elem->NextSiblingElement("symbol");
    }

    // TODO - Import words!
    auto lv_words = std::vector<Word>();

    // Import roles from any other attributes
    auto lv_roles = rolesMap();

    const tinyxml2::XMLAttribute* role_attr = lv_elem->FirstAttribute();
    while( role_attr != nullptr ){
      // Skip the attributes type/name/text; these are not roles
      if( std::strcmp( role_attr->Name(), "type" ) == 0 || std::strcmp( role_attr->Name(), "name" ) == 0 ||
          std::strcmp( role_attr->Name(), "text" ) == 0 ){
        role_attr = role_attr->Next();
        continue;
      }
      lv_roles.emplace( role_attr->Name(), role_attr->Value() );
      role_attr = role_attr->Next();
    }

    // Add the speaker as a role if one was imported
    if( speaker ){
      lv_roles.emplace( "speaker", speaker.value() );
    }

    // Attempt to add new node with found information
    auto node_it = flattened_info.nodes.find( name_attr->Value() );
    if( node_it != flattened_info.nodes.end() ){ // Duplicate variable name
      std::cout << "Warning: Duplicate variable name \"" << name_attr->Value() << "\"" << std::endl;
      return std::nullopt;
    }
    
    auto node_info = language_variable_node_t( type_attr->Value(), name_attr->Value(),
      lv_symbols, lv_words, lv_roles );
    flattened_info.nodes.emplace( name_attr->Value(), node_info );

    // Import child connections of the language variable
    const tinyxml2::XMLElement* connection_elem = lv_elem->FirstChildElement("connection");
    while( connection_elem != nullptr ){
      const tinyxml2::XMLAttribute* label_attr = connection_elem->FindAttribute("label");
      std::optional<std::string> connection_label = std::nullopt;
      if( label_attr != nullptr ){
        connection_label = std::make_optional< std::string >( label_attr->Value() );
      }

      const tinyxml2::XMLAttribute* child_name_attr = connection_elem->FindAttribute("child-name");
      if( child_name_attr == nullptr ){
        std::cout << "Failed to find child variable name" << std::endl;
        return std::nullopt;
      }

      // Store the edge as ( parent_name, optional edge_label, child_name )
      flattened_info.edges.emplace_back( name_attr->Value(), connection_label, child_name_attr->Value() );

      // Iterate to next child connection
      connection_elem = connection_elem->NextSiblingElement("connection");
    } // connection_elem loop
 
    // Iterate to next language variable
    lv_elem = lv_elem->NextSiblingElement("language-variable");
  } // lv_elem loop

  // Find the root of the imported LV information
  std::optional<std::string> root_name = find_root( flattened_info );
  if( root_name ){ // Construct and return the LanguageVariable
    return construct_lv( *root_name, flattened_info );
  } else{ // No root LV was found
    return std::nullopt;
  }
}

//
// Method to import a LanguageVariable in flattened format from an XMLElement*
//
std::optional<LanguageVariable> LanguageVariable::flattened_from_xml( const tinyxml2::XMLElement* root_elem,
        std::shared_ptr<SymbolSpace>& symbolspace ){
  // Create flattened representation struct
  auto flattened_info = flattened_language_variable_t();

  // Check that the element is a root element
  if( root_elem == nullptr ) return std::nullopt;
  if( std::strcmp(root_elem->Name(), "root") != 0 ) return std::nullopt;
  
  // Import speaker uid if one has been annotated
  std::optional<std::string> speaker = std::nullopt;
  const tinyxml2::XMLElement* instruction_elem = root_elem->FirstChildElement("instruction");
  if( instruction_elem != nullptr ){
    const tinyxml2::XMLAttribute* speaker_attr = instruction_elem->FindAttribute("uid");
    if( speaker_attr != nullptr ){
      speaker = std::make_optional<std::string>( speaker_attr->Value() );
    }
  }

  // Import child truncated LanguageVariable items into nodes and edges
  const tinyxml2::XMLElement* lv_elem = root_elem->FirstChildElement("language-variable");
  while( lv_elem != nullptr ){
    const tinyxml2::XMLAttribute* name_attr = lv_elem->FindAttribute("name");
    if( name_attr == nullptr ){
      std::cout << "Failed to find language-variable name" << std::endl;
      return std::nullopt;
    }

    const tinyxml2::XMLAttribute* type_attr = lv_elem->FindAttribute("type");
    if( type_attr == nullptr ){
      std::cout << "Failed to find language-variable type" << std::endl;
      return std::nullopt;
    }

    // Import child XMLElement Symbols as this LanguageVariable's symbols
    auto lv_symbols = symbolsVector();
    
    const tinyxml2::XMLElement* symbol_elem = lv_elem->FirstChildElement("symbol");
    while( symbol_elem != nullptr ){
      h2sl::Symbol symbol = h2sl::Symbol();

      // Check that the Symbol is imported correctly
      if( !symbol.from_xml( symbol_elem ) ){
        return std::nullopt;
      }

      // Add symbol to the LV's symbols vector
      if( symbolspace != nullptr ){
        lv_symbols.push_back( symbolspace->insert(symbol) );
      } else{
        lv_symbols.push_back( std::make_shared<Symbol>(symbol) );
      }
      symbol_elem = symbol_elem->NextSiblingElement("symbol");
    }

    // TODO - Import words!
    auto lv_words = std::vector<Word>();

    // Import roles from any other attributes
    auto lv_roles = rolesMap();

    const tinyxml2::XMLAttribute* role_attr = lv_elem->FirstAttribute();
    while( role_attr != nullptr ){
      // Skip the attributes type/name/text; these are not roles
      if( std::strcmp( role_attr->Name(), "type" ) == 0 || std::strcmp( role_attr->Name(), "name" ) == 0 ||
          std::strcmp( role_attr->Name(), "text" ) == 0 ){
        role_attr = role_attr->Next();
        continue;
      }
      lv_roles.emplace( role_attr->Name(), role_attr->Value() );
      role_attr = role_attr->Next();
    }
    
    // Add the speaker as a role if one was imported
    if( speaker ){
      lv_roles.emplace( "speaker", speaker.value() );
    }

    // Attempt to add new node with found information
    auto node_it = flattened_info.nodes.find( name_attr->Value() );
    if( node_it != flattened_info.nodes.end() ){ // Duplicate variable name
      std::cout << "Warning: Duplicate variable name \"" << name_attr->Value() << "\"" << std::endl;
      return std::nullopt;
    }

    auto node_info = language_variable_node_t( type_attr->Value(), name_attr->Value(),
      lv_symbols, lv_words, lv_roles );
    flattened_info.nodes.emplace( name_attr->Value(), node_info );

    // Import child connections of the language variable
    const tinyxml2::XMLElement* connection_elem = lv_elem->FirstChildElement("connection");
    while( connection_elem != nullptr ){
      const tinyxml2::XMLAttribute* label_attr = connection_elem->FindAttribute("label");
      std::optional<std::string> connection_label = std::nullopt;
      if( label_attr != nullptr ){
        connection_label = std::make_optional< std::string >( label_attr->Value() );
      }

      const tinyxml2::XMLAttribute* child_name_attr = connection_elem->FindAttribute("child-name");
      if( child_name_attr == nullptr ){
        std::cout << "Failed to find child variable name" << std::endl;
        return std::nullopt;
      }

      // Store the edge as ( parent_name, optional edge_label, child_name )
      flattened_info.edges.emplace_back( name_attr->Value(), connection_label, child_name_attr->Value() );

      // Iterate to next child connection
      connection_elem = connection_elem->NextSiblingElement("connection");
    } // connection_elem loop

    // Iterate to next language variable
    lv_elem = lv_elem->NextSiblingElement("language-variable");
  } // lv_elem loop

  // Find the root of the imported LV information
  std::optional<std::string> root_name = find_root( flattened_info );
  if( root_name ){ // Construct and return the LanguageVariable
    return construct_lv( *root_name, flattened_info );
  } else{ // No root LV was found
    return std::nullopt;
  }
}

//
// This method finds and returns the root variable of the imported information.
//
std::optional<std::string> LanguageVariable::find_root( const flattened_language_variable_t& info ){
  std::vector<std::string> root_names;
  for( const auto& node : info.nodes ){
    // Skip any nodes which have a parent node
    bool had_parent = false;
    for( const auto& edge : info.edges ){
      if( node.first == std::get<2>(edge) ){ // This node is another's child
        had_parent = true;
        break; // Exits edges loop
      }
    }

    if( !had_parent ){
      root_names.push_back( node.first );
    }
  }

  // Check if we had a unique root, return accordingly
  if( root_names.size() == 0 ){
    std::cout << "No root node found; all nodes were part of some cycle!" << std::endl;
    return std::nullopt;
  } else if( root_names.size() > 1 ){
    std::cout << "Multiple root nodes found; invalid xml file!" << std::endl;
    return std::nullopt;
  } // Otherwise we have one root variable; this is what we return

  return std::make_optional<std::string>( root_names.at(0) );
}

//
// This method constructs a LanguageVariable based on the imported information.
//
std::optional<LanguageVariable> LanguageVariable::construct_lv( const std::string& var_name,
    const flattened_language_variable_t& info ){
  
  // First check that a LV exists for the given variable name
  auto node_it = info.nodes.find( var_name );
  if( node_it == info.nodes.end() ){ // Node was not found!
    return std::nullopt;
  }

  // Otherwise we can construct and return a LanguageVariable
  auto lv = std::make_optional<LanguageVariable>( node_it->second.type, var_name, node_it->second.symbols );
  
  // Optionally assign LV words and roles if any were imported
  if( node_it->second.words.size() > 0 ){
    lv->words = std::make_optional< std::vector<Word> >( node_it->second.words );
  }

  if( node_it->second.roles.size() > 0 ){
    lv->roles = std::make_optional< rolesMap >( node_it->second.roles );
  }

  // Now iterate over found edges. Any with a matching parent name are our children.
  for( const auto& edge : info.edges ){
    if( std::get<0>(edge) == var_name ){ // Matching parent name => Create child LV
      std::optional<LanguageVariable> child_lv = construct_lv( std::get<2>(edge), info );

      if( !child_lv ){ // Child construction failed; skip it
        std::cout << "Construction of child LV \"" << std::get<2>(edge) << "\" failed!" << std::endl;
      } else{ // There's actually a child LV => Create connection and add to parent LV
        h2sl::LanguageVariable::language_variable_connection_t connection;
        connection.label = std::get<1>(edge); // Label is already std::optional<std::string>
        connection.child = std::make_shared<h2sl::LanguageVariable>( *child_lv );
        lv->children.push_back( connection );
      }
    }
  }

  return lv;
}

///
/// Method to load from a json formatted string
///
bool LanguageVariable::from_json( const std::string& json_string ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_string, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool LanguageVariable::from_json( const Json::Value& root ){
  type = "";
  name = "";
  symbols.clear();
  children.clear();
  words = std::nullopt;
  roles = std::nullopt;

  type = root["type"].asString();
  name = root["name"].asString();

  // TODO - Import roles from JSON

  for( const auto& word_root : root["words"] ){
    if( !words ){
      words = std::make_optional< std::vector<Word> >();
    }
    h2sl::Word word;
    word.from_json( word_root );
    words->push_back(word);
  }

  for( const auto& symbol_root : root["symbols"] ){
    auto symbol = std::make_shared<h2sl::Symbol>();
    symbol->from_json( symbol_root );
    symbols.push_back(symbol);
  }
  
  Json::Value children_array = root["children"];
  for( const auto& child : children_array ){
    language_variable_connection_t connection;
    connection.child = std::make_shared<LanguageVariable>();

    Json::Value edge_label_root = child["label"];
    if( !edge_label_root.isNull() ){
      connection.label = edge_label_root.asString();
    }

    Json::Value child_lv = child["language-variable"];
    connection.child->from_json( child_lv );
    children.push_back( connection );
  }

  return true;
}

//
// Method to export a LanguageVariable in hierarchical format to an XML file
//
bool LanguageVariable::to_xml( const char* filename ) const{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertEndChild( root );
  to_xml( doc, root );

  // Save the file and store the error status
  tinyxml2::XMLError error_result = doc.SaveFile( filename );
  if( error_result != tinyxml2::XML_SUCCESS )
    return false;
  return true;
}

//
// Method to export a LanguageVariable in hierarchical format to an XMLDocument object
//
void LanguageVariable::to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root) const{
  tinyxml2::XMLElement* language_variable_elem = doc.NewElement("language-variable");
  language_variable_elem->SetAttribute( "type", type.c_str() );
  language_variable_elem->SetAttribute( "name", name.c_str() );
  
  if( words ){
    language_variable_elem->SetAttribute( "text", words_to_std_string().c_str() );
  }

  if( roles ){
    for( auto it_roles = roles->cbegin(); it_roles != roles->cend(); ++it_roles ){
      language_variable_elem->SetAttribute( it_roles->first.c_str(), it_roles->second.c_str() );
    }
  }

  // Add the constructed element to the root
  root->InsertEndChild( language_variable_elem );

  // Export words, symbols, and children
  if( words ){
    for( auto const& word : *words ){
      word.to_xml( doc, language_variable_elem ); 
    }
  }
  for( auto const& symbol : symbols ){
    symbol->to_xml( doc, language_variable_elem );
  }
  for( auto const& child_connection : children ){
    tinyxml2::XMLElement* connection_elem = doc.NewElement("child-connection");
    if( child_connection.label ){
      connection_elem->SetAttribute( "label", child_connection.label->c_str() );
    }

    child_connection.child->to_xml( doc, connection_elem );

    // Add the constructed connection element to the language variable
    language_variable_elem->InsertEndChild( connection_elem );
  }

  return;
}

///
/// Method to write to a json formatted string
///
std::string LanguageVariable::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void LanguageVariable::to_json( Json::Value& root )const{
  
  root["type"] = type;
  root["name"] = name;
 
  root["children"] = Json::Value( Json::arrayValue );

  for( const auto& child_connection : children ){
    Json::Value child_connection_root;
    if( child_connection.label ){
      child_connection_root["label"] = child_connection.label.value();
    }
    child_connection.child->to_json( child_connection_root );
    root["children"].append( child_connection_root );
  }

  if( words ){
    root["words"] = Json::Value( Json::arrayValue ); 

    for( const auto& word : *words ){
      Json::Value word_root;
      word.to_json( word_root );
      root["words"].append( word_root );
    }
  }

  // TODO - Export roles

  root["symbols"] = Json::Value( Json::arrayValue ); 

  for( const auto& symbol : symbols ){
    Json::Value symbol_root;
    symbol->to_json( symbol_root );
    root["symbols"].append( symbol_root );
  }

  return;
}

//
// This method exports a LanguageVariable to a LanguageVariableMessage.
//
LanguageVariableMessage LanguageVariable::to_msg( void )const{
  // First create a flattened representation of the LanguageVariable
  LanguageVariable::flattened_language_variable_t flattened_info = flatten();

  // Populate the message using the found LV nodes
  LanguageVariableMessage msg;
  for( const auto& node_it : flattened_info.nodes ){
    LanguageVariableNodeMessage node_msg;
    node_msg.name = node_it.second.name;
    node_msg.type = node_it.second.type;

    // Create symbol messages for each Symbol
    for( const auto& symbol : node_it.second.symbols ){
      node_msg.symbols.emplace_back( symbol->to_msg() );
    }

    // Create word messages for each Word
    for( const auto& word : node_it.second.words ){
      node_msg.words.emplace_back( word.to_msg() );
    }

    // Create role messages for each Role
    for( const auto& role : node_it.second.roles ){
      PropertyMessage p;
      p.name = role.first;
      p.value = role.second;
      node_msg.roles.emplace_back( p );
    }

    // Find all child connections and add to the message
    for( const auto& edge : flattened_info.edges ){
      // Only add child edges with the correct parent name
      if( std::get<0>(edge) != node_it.first ) continue;

      LanguageVariableConnectionMessage connection_msg;
      std::optional<std::string> label = std::get<1>(edge);
      if( label ){
        connection_msg.label = *label;
        connection_msg.label_exists = 1;
      } else{
        connection_msg.label_exists = 0;
      }
      connection_msg.child_name = std::get<2>(edge);
      node_msg.children.emplace_back( connection_msg );
    }

    // Add this node to the LanguageVariableMessage
    msg.nodes.emplace_back( node_msg );
  }

  return msg;
}

//
// This method returns a flattened version of the LanguageVariable.
//
LanguageVariable::flattened_language_variable_t LanguageVariable::flatten( void )const{
  // Create the information struct that we'll pass down to our children
  flattened_language_variable_t info = flattened_language_variable_t();
  flatten_recursive( info ); // Populate the struct with this LV's information
  return info;
}

//
// Recursive helper method for flatten() method.
//
void LanguageVariable::flatten_recursive( flattened_language_variable_t& info )const{
  // Check if this LV has yet been added
  auto node_it = info.nodes.find( name );
  if( node_it == info.nodes.end() ){ // This node is new!
    // Check if this LV has words
    std::vector<Word> lv_words = std::vector<Word>();
    if( words ){ lv_words = *words; }

    // Check if this LV has roles
    auto lv_roles = rolesMap();
    if( roles ){ lv_roles = *roles; }
    
    // Construct node object
    auto lv_info = language_variable_node_t( type, name, symbols, lv_words, lv_roles );
    info.nodes.emplace( name, lv_info );
  }

  // Attempt to add child LVs to the flattened information
  for( const auto& connection : children ){
    // Add edge information and then process the child LV
    info.edges.emplace_back( name, connection.label, connection.child->name );
    connection.child->flatten_recursive( info );
  }

  return;
}

//
// Returns only the current language-variable absent of any children
//
LanguageVariable LanguageVariable::current( void )const{
  return LanguageVariable( type, name, symbols, std::vector<language_variable_connection_t>(), words, roles );
}

//
// LanguageVariable class ostream operator
//
std::ostream& operator<<( std::ostream& out, const LanguageVariable& other ){
  out << other.type << "{";
  out << other.name;

  // Output words for this LanguageVariable (does not include words from children)
  if( other.words ){
    out << ",\"" << other.words_to_std_string() << "\" ";
    out << "words(";
    for( auto it_word = other.words->cbegin(); it_word != other.words->cend(); ++it_word ){
      out << *it_word;
      if( std::next(it_word) != other.words->cend() )
        out << ", ";
    }
    out << ")";
  }

  // Output roles for this LanguageVariable if they exist  
  if( other.roles ){
    out << ",roles(";
    bool first_role = true;
    for( std::pair<std::string,std::string> role_pair : other.roles.value() ){
      if( !first_role ){
        out << ",";
      }
      out << role_pair.first << ":" << role_pair.second;
      first_role = false;
    }
    out << ")";
  }

  // Output symbols for this LanguageVariable
  if( other.symbols.size() > 0 ){
    out << ",symbols(";
    for( auto it_symbol = other.symbols.cbegin();
                it_symbol != other.symbols.cend();
                ++it_symbol )
    {
      if( *it_symbol != nullptr )
        out << **it_symbol;
      if( std::next(it_symbol) != other.symbols.cend() )
        out << ", ";
    }
    out << ")";
  }

  // Output children connections
  if( other.children.size() > 0 ){
    out << ",children(";
    for( auto it_child = other.children.cbegin();
              it_child != other.children.cend();
              ++it_child )
    { // Child may or may not have an edge label
      if ( it_child->label ) {
        out << "[" << it_child->label.value() << "]:";
      }
      out << (*it_child->child);
      if( std::next(it_child) != other.children.cend() )
        out << ",";
    }
    out << ")";
  }
  out << "}";
  return out;
}

} // namespace h2sl
