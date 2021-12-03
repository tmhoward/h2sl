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
// LanguageVariable class full member parameter constructor
//
LanguageVariable::
LanguageVariable( std::string const & type, std::string const & name, symbolsVector const & symbols,
                  std::vector<language_variable_connection_t> const & children, std::vector<Word> const & words,
                  rolesMap const & roles )
                  : type_( type ), name_( name ), symbols_( symbols ), children_( children ), o_words_( words ), o_roles_( roles ){}

//
// LanguageVariable class member parameter constructor; no roles parameter
//
LanguageVariable::
LanguageVariable( std::string const & type, std::string const & name, symbolsVector const & symbols,
                  std::vector<language_variable_connection_t> const & children, std::vector<Word> const & words )
                  : type_( type ), name_( name ), symbols_( symbols ), children_( children ),
                    o_words_( words ), o_roles_( std::nullopt ){}
//
// LanguageVariable class member parameter constructor; no roles or words parameters
//
LanguageVariable::
LanguageVariable( std::string const & type, std::string const & name, symbolsVector const & symbols,
                  std::vector<language_variable_connection_t> const & children )
                  : type_( type ), name_( name ), symbols_( symbols ), children_( children ),
                    o_words_( std::nullopt ), o_roles_( std::nullopt ){}

//
// LanguageVariable class constructor from XMLElement*
//
LanguageVariable::LanguageVariable( tinyxml2::XMLElement const * p_lv_element, SymbolSpace *const p_symbolspace )
    : type_(), name_(), symbols_(), children_(), o_words_(), o_roles_()
{
  from_xml( p_lv_element, p_symbolspace );
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
      lv_symbols.push_back( std::make_shared<Symbol>( symbol_msg ) );
    }
    // Import any words from the node
    auto lv_words = std::vector<Word>();
    for( const auto& word_msg : node.words ){
      lv_words.push_back( word_msg );
    }
    // Import any roles from the node
    auto lv_roles = rolesMap();
    for( const auto& role_msg : node.roles ){
      lv_roles.emplace( role_msg.name, role_msg.value );
    }
    // Attempt to add new node with found information
    auto const it_node = flattened_info.nodes.find( node.name );
    if( it_node != flattened_info.nodes.end() ){
      std::cout << "Warning: Duplicate variable name \"" << node.name << "\"" << std::endl;
      return std::nullopt;
    }

    auto p_lv_node = std::make_unique<LanguageVariable>( node.type, node.name, lv_symbols,
                                                       std::vector<language_variable_connection_t>(), lv_words, lv_roles );
    flattened_info.nodes.emplace( node.name, std::move(p_lv_node) );

    // Import child connections of the language variable
    for( const auto& connection_msg : node.children ){
      auto lv_edge = language_variable_edge_t();
      lv_edge.parent_name = node.name;
      lv_edge.child_name = connection_msg.child_name;
      if( connection_msg.label_exists ) lv_edge.label = connection_msg.label;
      flattened_info.edges.push_back( lv_edge );
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
  symbols_.clear();

  // Clear symbols from all children LanguageVariables
  for( auto& connection : children_ ){
    connection.child->clear_symbols();
  }
}

//
// Method to generate a unique key for the language variable
//
std::string LanguageVariable::key( void ) const{
  // TODO - Just use unique name
  std::stringstream out;
  out << type_ << "{";

  // Output words for this LanguageVariable if they exist (does not include words from children)
  if( o_words_ ){
    out << "words(";
    for( auto it_word = o_words_->cbegin(); it_word != o_words_->cend(); ++it_word ){
      out << *it_word;
      if( std::next(it_word) != o_words_->cend() )
        out << ",";
    }
    out << ")";
  }

  // Output roles for this LanguageVariable if they exist
  if( o_roles_ ){
    out << "roles(";
    bool first = true;
    for( std::pair<std::string,std::string> role_pair : *o_roles_ ){
      if( !first ){
        out << ",";
      }
      out << role_pair.first << ":" << role_pair.second;
      first = false;
    }
    out << ")";
  }

  // Output children
  if( children_.size() > 0 ){
    out << ",children(";
    for( auto it_child = children_.cbegin();
              it_child != children_.cend();
              ++it_child )
    { // Child may or may not have an edge label
      if ( it_child->label ) {
        out << "[" << *(it_child->label) << "]:";
      }
      out << it_child->child->key();
      if( std::next(it_child) != children_.cend() )
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
  for( auto const& child_connection : children_ ){
    std::vector<Word> child_words_vec = child_connection.child->all_words();
    all_words_vec.insert( all_words_vec.end(),
                          child_words_vec.begin(),
                          child_words_vec.end() );
  }
  if( o_words_ ){
    all_words_vec.insert( all_words_vec.end(), o_words_->begin(), o_words_->end() );
  }
  return all_words_vec;
}

//
// Method to import a LanguageVariable in either a hierarchical or flattened format
//
void LanguageVariable::from_xml( const char* filename ){
  from_xml( filename, nullptr );
  return;
}

//
// Method to import a LanguageVariable in either a hierarchical or flattened format
//
void LanguageVariable::from_xml( const char* filename, SymbolSpace *const p_symbolspace ){
  // Initialize an error_msg for exception handling
  std::stringstream error_msg;
  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename );
  if( doc.Error() ){
    error_msg << log_error(__FILE__, __LINE__) << "Document error during LoadFile for \""
              << filename << "\" with doc.Error(): " << doc.Error();
    throw std::runtime_error( error_msg.str() );
  }
  // Extract the root element from the document object
  auto const* p_root = doc.RootElement();
  if( p_root == nullptr ){
    error_msg << log_error(__FILE__, __LINE__) << "Document error during RootElement for \"" << filename << "\"";
    throw std::runtime_error( error_msg.str() );
  }
  // Check for any LanguageVariable child element of the root
  auto const* p_lv_element = p_root->FirstChildElement("language-variable");
  if( p_lv_element == nullptr ){
    error_msg << log_error(__FILE__, __LINE__) << "No language variale element found as a child of the root element for \""
              << filename << "\"";
    throw std::runtime_error( error_msg.str() );
  }
  from_xml( p_lv_element, p_symbolspace );
  return;
}

//
// Method to import a LanguageVariable in either a hierarchical or flattened format
//
void LanguageVariable::from_xml( tinyxml2::XMLElement const * p_lv_element, SymbolSpace *const p_symbolspace,
                                  std::string const & speaker )
{
  // Check that the element is a language variable element
  validate_lv_element_( p_lv_element );
  // Determine whether to use a flattened or hierarchical representation parse
  // Check whether the element has any child language-variable elements
  auto const *const p_child_lv_element = p_lv_element->FirstChildElement("language-variable");
  if( p_child_lv_element != nullptr ) hierarchical_from_xml( p_lv_element, p_symbolspace, speaker );
  else flattened_from_xml( p_lv_element, p_symbolspace, speaker );
  return;
}

//
// Method to import a Language Variable in hierarchical format from an XMLElement*
//
void LanguageVariable::hierarchical_from_xml( tinyxml2::XMLElement const* p_lv_element, SymbolSpace *const p_symbolspace,
                                              std::string const & speaker )
{
  try{
    // Initialize an error_msg for exception handling
    std::stringstream error_msg;
    // Clear the member data
    reset_();
    // Check that the element is a language variable element
    validate_lv_element_( p_lv_element );
    // Import the type and name attributes
    auto const *const p_type_attribute = p_lv_element->FindAttribute("type");
    if( p_type_attribute == nullptr ){
      error_msg << log_error(__FILE__, __LINE__) << "No \"type\" attribute found";
      throw std::runtime_error( error_msg.str() );
    }
    type_ = p_type_attribute->Value();
    auto const *const p_name_attribute = p_lv_element->FindAttribute("name");
    if( p_name_attribute == nullptr ){
      error_msg << log_error(__FILE__, __LINE__) << "No \"name\" attribute found";
      throw std::runtime_error( error_msg.str() );
    }
    name_ = p_name_attribute->Value();
    // Import child XMLElement Words as this LV's words; Otherwise, leave the member as nullopt
    auto const * p_word_element = p_lv_element->FirstChildElement("word");
    if( p_word_element != nullptr ) o_words_ = std::make_optional< std::vector<Word> >();
    while( p_word_element != nullptr ){
      Word word;
      if( !word.from_xml( p_word_element ) ){
        error_msg << log_error(__FILE__, __LINE__) << "Failure loading child word from xml";
        throw std::runtime_error( error_msg.str() );
      }
      o_words_.value().push_back( word );
      p_word_element = p_word_element->NextSiblingElement("word");
    }
    // Import roles from any other attributes
    auto const * p_role_attribute = p_lv_element->FirstAttribute();
    while( p_role_attribute != nullptr ){
      // Skip type/name/text attributes; these are not roles
      auto const & r_role_name = p_role_attribute->Name();
      if( (strcmp( r_role_name, "type" ) == 0) || (strcmp( r_role_name, "name" ) == 0) || (strcmp( r_role_name, "text" ) == 0) ){
        p_role_attribute = p_role_attribute->Next();
        continue;
      }
      if( o_roles_ == std::nullopt ) o_roles_ = std::make_optional< rolesMap >();
      o_roles_->emplace( p_role_attribute->Name(), p_role_attribute->Value() );
      p_role_attribute = p_role_attribute->Next();
    }
    // Add the speaker as a role if there were other roles & speaker is not empty
    if( o_roles_ != std::nullopt ){
      if( !speaker.empty() ) o_roles_->emplace( "speaker", speaker );
    }
    // Import child XMLElement Symbols as this LV's symbols; Insert it onto the symbolspace, if not nullptr
    auto const * p_symbol_element = p_lv_element->FirstChildElement("symbol");
    while( p_symbol_element != nullptr ){
      Symbol symbol;
      // Check that the Symbol is imported correctly
      if( !symbol.from_xml( p_symbol_element ) ){
        error_msg << log_error(__FILE__, __LINE__) << "Failed to load child symbol from xml";
        throw std::runtime_error( error_msg.str() );
      }
      if( p_symbolspace != nullptr ) symbols_.push_back( p_symbolspace->insert( symbol ) );
      else symbols_.push_back( std::make_shared<Symbol>( symbol ) );
      p_symbol_element = p_symbol_element->NextSiblingElement("symbol");
    }
    // Import child XMLElement language_variable_connection_t as this LanguageVariable's children
    auto const * p_child_connection_element = p_lv_element->FirstChildElement("child-connection");
    while( p_child_connection_element != nullptr ){
      language_variable_connection_t connection;
      connection.child = std::make_shared<LanguageVariable>();
      auto const * p_edge_label_attribute = p_child_connection_element->FindAttribute("label");
      if( p_edge_label_attribute != nullptr ) connection.label = p_edge_label_attribute->Value();
      auto const * p_child_lv_element = p_child_connection_element->FirstChildElement("language-variable");
      if( p_child_lv_element == nullptr ){
        error_msg << log_error(__FILE__, __LINE__) << "Failed to find child language variable";
        throw std::runtime_error( error_msg.str() );
      }
      connection.child->hierarchical_from_xml( p_child_lv_element, p_symbolspace, speaker );
      children_.push_back( connection );
      p_child_connection_element = p_child_connection_element->NextSiblingElement("child-connection");
    }
    // Allow importing of child XMLElement LV children directly
    auto const * p_child_lv_element = p_lv_element->FirstChildElement("language-variable");
    while( p_child_lv_element != nullptr ){
      language_variable_connection_t connection;
      connection.child = std::make_shared<LanguageVariable>(); // Leave edge label empty
      connection.child->hierarchical_from_xml( p_child_lv_element, p_symbolspace, speaker );
      children_.push_back( connection );
      p_child_lv_element = p_child_lv_element->NextSiblingElement("language-variable");
    }
  }
  catch( std::runtime_error& error ){
    reset_();
    throw error;
  }
  return;
}

//
// Method to import a LanguageVariable in flattened format from an XMLElement*
//
void LanguageVariable::flattened_from_xml( tinyxml2::XMLElement const * p_lv_element, SymbolSpace *const p_symbolspace,
                                            std::string const & speaker )
{
  try{
    // Initialize an error_msg for exception handling
    std::stringstream error_msg;
    // Clear the member data
    reset_();
    // Check that the element is a language variable element
    validate_lv_element_( p_lv_element );
    // Check that the element does not have child language-variable elements
    auto const *const p_child_lv_element = p_lv_element->FirstChildElement("language-variable");
    if( p_child_lv_element != nullptr ){
      error_msg << log_error(__FILE__, __LINE__) << "The XMLElement* had a child language-variable element. Not a flat representation.";
      throw std::runtime_error( error_msg.str() );
    }
    // Create flattened representation struct
    auto flattened_info = flattened_language_variable_t();

    // Loop through all sibling language-variable elements
    while( p_lv_element != nullptr ){
      // Import the type and name attributes
      auto const *const p_type_attribute = p_lv_element->FindAttribute("type");
      if( p_type_attribute == nullptr ){
        error_msg << log_error(__FILE__, __LINE__) << "No \"type\" attribute found";
        throw std::runtime_error( error_msg.str() );
      }
      auto const *const p_name_attribute = p_lv_element->FindAttribute("name");
      if( p_name_attribute == nullptr ){
        error_msg << log_error(__FILE__, __LINE__) << "No \"name\" attribute found";
        throw std::runtime_error( error_msg.str() );
      }

      // Import child XMLElement Symbols as this LanguageVariable's symbols
      auto lv_symbols = symbolsVector();
      auto const * p_symbol_element = p_lv_element->FirstChildElement("symbol");
      while( p_symbol_element != nullptr ){
        Symbol symbol;
        // Check that the Symbol is imported correctly
        if( !symbol.from_xml( p_symbol_element ) ){
          error_msg << log_error(__FILE__, __LINE__) << "Failed to load child symbol from xml";
          throw std::runtime_error( error_msg.str() );
        }
        if( p_symbolspace != nullptr ) lv_symbols.push_back( p_symbolspace->insert( symbol ) );
        else lv_symbols.push_back( std::make_shared<Symbol>( symbol ) );
        p_symbol_element = p_symbol_element->NextSiblingElement("symbol");
      }

      // Import child XMLElement Words
      auto lv_words = std::vector<Word>();
      auto const * p_word_element = p_lv_element->FirstChildElement("word");
      while( p_word_element != nullptr ){
        Word word;
        if( !word.from_xml( p_word_element ) ){
          error_msg << log_error(__FILE__, __LINE__) << "Failure loading child word from xml";
          throw std::runtime_error( error_msg.str() );
        }
        lv_words.push_back( word );
        p_word_element = p_word_element->NextSiblingElement("word");
      }

      // Import roles from any other attributes
      auto lv_roles = rolesMap();
      auto const * p_role_attribute  = p_lv_element->FirstAttribute();
      while( p_role_attribute != nullptr ){
        // Skip type/name/text attributes; these are not roles
        auto const & r_role_name = p_role_attribute->Name();
        if( (strcmp( r_role_name, "type" ) == 0) || (strcmp( r_role_name, "name" ) == 0) || (strcmp( r_role_name, "text" ) == 0) ){
          p_role_attribute = p_role_attribute->Next();
          continue;
        }
        lv_roles.emplace( p_role_attribute->Name(), p_role_attribute->Value() );
        p_role_attribute = p_role_attribute->Next();
      }
      // Add the speaker as a role if there were other roles & speaker is not empty
      if( !lv_roles.empty() ){
        if( !speaker.empty() ) lv_roles.emplace( "speaker", speaker );
      }
      // Attempt to add new node with found information
      auto const it_node = flattened_info.nodes.find( p_name_attribute->Value() );
      if( it_node != flattened_info.nodes.end() ){ // Duplicate variable name
        error_msg << log_error(__FILE__, __LINE__) << "Duplicate name attribute for \"" << p_name_attribute->Value() << "\"";
        throw std::runtime_error( error_msg.str() );
      }
      auto p_lv_node = std::make_unique<LanguageVariable>( p_type_attribute->Value(), p_name_attribute->Value(), lv_symbols,
                                       std::vector<language_variable_connection_t>(), lv_words, lv_roles );
      flattened_info.nodes.emplace( p_name_attribute->Value(), std::move(p_lv_node) );

      // Import child connections of the language variable
      auto const * p_connection_element = p_lv_element->FirstChildElement("connection");
      while( p_connection_element != nullptr ){
        auto const *const p_label_attribute = p_connection_element->FindAttribute("label");
        std::optional<std::string> o_connection_label = std::nullopt;
        if( p_label_attribute != nullptr ) o_connection_label = p_label_attribute->Value();
        auto const *const p_child_name_attribute = p_connection_element->FindAttribute("child-name");
        if( p_child_name_attribute == nullptr ){
          error_msg << log_error(__FILE__, __LINE__) << "Connection element did not have a name attribute";
          throw std::runtime_error( error_msg.str() );
        }
        language_variable_edge_t lv_edge;
        lv_edge.parent_name = p_name_attribute->Value();
        lv_edge.child_name = p_child_name_attribute->Value();
        lv_edge.label = o_connection_label;
        flattened_info.edges.push_back( lv_edge );
        p_connection_element = p_connection_element->NextSiblingElement("connection");
      }
      // Iterate to next language variable
      p_lv_element = p_lv_element->NextSiblingElement("language-variable");
    } // p_lv_element loop

    // Find the root of the imported LV information
    std::optional<std::string> o_root_name = find_root( flattened_info );
    if( o_root_name != std::nullopt ){
      auto o_this = construct_lv( *o_root_name, flattened_info );
      if( o_this != std::nullopt ) *this = *o_this;
      else{
        error_msg << log_error(__FILE__, __LINE__) << "The construct_lv() method returned nullopt, unable to load from flattened XML";
        throw std::runtime_error( error_msg.str() );
      }
    } else{
      error_msg << log_error(__FILE__, __LINE__) << "No LV root name found, unable to load from flattened XML";
      throw std::runtime_error( error_msg.str() );
    }
  }
  catch( std::runtime_error& error ){
    reset_();
    throw error;
  }
  return;
}

//
// This method validates that a purported language-variable XMLElement has the correct name value
//
void LanguageVariable::validate_lv_element_( tinyxml2::XMLElement const *const p_lv_element ){
  std::stringstream error_msg;
  // Check that the element is a language variable element
  if( p_lv_element == nullptr ){
    error_msg << log_error(__FILE__, __LINE__) << "The XMLElement* was nullptr";
    throw std::runtime_error( error_msg.str() );
  }
  if( strcmp( p_lv_element->Name(), "language-variable" ) != 0 ){
    error_msg << log_error(__FILE__, __LINE__) << "The XMLElement* name \"" << p_lv_element->Name() << "\" did not match"
              << " \"language-variable\"";
    throw std::runtime_error( error_msg.str() );
  }
  return;
}

//
// This method finds and returns the root variable of the imported information.
//
std::optional<std::string> LanguageVariable::find_root( const flattened_language_variable_t& info ){
  std::vector<std::string> root_names;
  for( const auto & [node_name, lv_node] : info.nodes ){
    // Skip any nodes which have a parent node
    bool had_parent = false;
    for( const auto& edge : info.edges ){
      if( node_name == edge.child_name ){ // This node is another's child
        had_parent = true;
        break; // Exits edges loop
      }
    }
    if( !had_parent ){
      root_names.push_back( node_name );
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
std::optional<LanguageVariable>
LanguageVariable::construct_lv( const std::string& var_name, const flattened_language_variable_t& info ){
  // First check that a LV exists for the given variable name
  auto const it_node = info.nodes.find( var_name );
  if( it_node == info.nodes.end() ){ // Node was not found!
    return std::nullopt;
  }

  // Otherwise we can construct and return a LanguageVariable
  auto o_lv = std::make_optional<LanguageVariable>( it_node->second->type(), var_name, it_node->second->symbols(),
                                                  std::vector<language_variable_connection_t>() );

  // Optionally assign LV words and roles if any were imported
  if( it_node->second->words() != std::nullopt ) o_lv->words() = it_node->second->words();
  if( it_node->second->roles() != std::nullopt ) o_lv->roles() = it_node->second->roles();

  // Now iterate over found edges. Any with a matching parent name are our children.
  for( const auto& edge : info.edges ){
    if( edge.parent_name == var_name ){ // Matching parent name => Create child LV
      std::optional<LanguageVariable> o_child_lv = construct_lv( edge.child_name, info );

      if( !o_child_lv ){ // Child construction failed; skip it
        std::cout << "Construction of child LV \"" << edge.child_name << "\" failed!" << std::endl;
      } else{ // There's actually a child LV => Create connection and add to parent LV
        h2sl::LanguageVariable::language_variable_connection_t connection;
        connection.label = edge.label; // Label is already std::optional<std::string>
        connection.child = std::make_shared<h2sl::LanguageVariable>( *o_child_lv );
        o_lv->children().push_back( connection );
      }
    }
  }
  return o_lv;
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
  reset_();
  type_ = root["type"].asString();
  name_ = root["name"].asString();

  // TODO - Import roles from JSON

  for( const auto& word_root : root["words"] ){
    if( !o_words_ ){
      o_words_ = std::make_optional< std::vector<Word> >();
    }
    h2sl::Word word;
    word.from_json( word_root );
    o_words_->push_back(word);
  }

  for( const auto& symbol_root : root["symbols"] ){
    auto symbol = std::make_shared<h2sl::Symbol>();
    symbol->from_json( symbol_root );
    symbols_.push_back(symbol);
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
    children_.push_back( connection );
  }

  return true;
}

//
// Method to export a LanguageVariable in hierarchical format to an XML file
//
void LanguageVariable::to_xml( const char* filename ) const{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertEndChild( root );
  to_xml( doc, root );

  // Save the file and store the error status
  tinyxml2::XMLError error_result = doc.SaveFile( filename );
  if( error_result != tinyxml2::XML_SUCCESS ){
    std::stringstream error_msg;
    error_msg << log_error(__FILE__, __LINE__) << "Error during SaveFile with XMLError: " << error_result;
    throw std::runtime_error( error_msg.str() );
  }
  return;
}

//
// Method to export a LanguageVariable in hierarchical format to an XMLDocument object
//
void LanguageVariable::to_xml( tinyxml2::XMLDocument& r_doc, tinyxml2::XMLElement* p_root) const{
  tinyxml2::XMLElement* language_variable_elem = r_doc.NewElement("language-variable");
  language_variable_elem->SetAttribute( "type", type_.c_str() );
  language_variable_elem->SetAttribute( "name", name_.c_str() );

  if( o_words_ ){
    language_variable_elem->SetAttribute( "text", words_to_std_string().c_str() );
  }

  if( o_roles_ ){
    for( auto it_roles = o_roles_->cbegin(); it_roles != o_roles_->cend(); ++it_roles ){
      language_variable_elem->SetAttribute( it_roles->first.c_str(), it_roles->second.c_str() );
    }
  }

  // Add the constructed element to the p_root
  p_root->InsertEndChild( language_variable_elem );

  // Export words, symbols, and children
  if( o_words_ ){
    for( auto const& word : *o_words_ ){
      word.to_xml( r_doc, language_variable_elem );
    }
  }
  for( auto const& symbol : symbols_ ){
    symbol->to_xml( r_doc, language_variable_elem );
  }
  for( auto const& child_connection : children_ ){
    tinyxml2::XMLElement* connection_elem = r_doc.NewElement("child-connection");
    if( child_connection.label ){
      connection_elem->SetAttribute( "label", child_connection.label->c_str() );
    }

    child_connection.child->to_xml( r_doc, connection_elem );

    // Add the constructed connection element to the language variable
    language_variable_elem->InsertEndChild( connection_elem );
  }

  return;
}

//
// Method to export a LanguageVariable in flattened format to an XML file
//
void LanguageVariable::to_flattened_xml( const char* filename )const{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertEndChild( root );
  to_flattened_xml( doc, root );

  // Save the file and store the error status
  tinyxml2::XMLError error_result = doc.SaveFile( filename );
  if( error_result != tinyxml2::XML_SUCCESS ){
    std::stringstream error_msg;
    error_msg << log_error(__FILE__, __LINE__) << "Error during SaveFile with XMLError: " << error_result;
    throw std::runtime_error( error_msg.str() );
  }
  return;
}

//
// Method to export a LanguageVariable in flattened format to an XMLDocument object
//
void LanguageVariable::to_flattened_xml( tinyxml2::XMLDocument& r_doc, tinyxml2::XMLElement* p_root )const{
  // First create a flattened representation of the LanguageVariable
  LanguageVariable::flattened_language_variable_t flattened_info = flatten();
  // Export each node as a sibling
  for( auto const & [node_name, p_lv_node] : flattened_info.nodes ){
    // Create the new XMLElement node & Add it to p_root
    tinyxml2::XMLElement *const p_lv_element = r_doc.NewElement("language-variable");
    p_root->InsertEndChild( p_lv_element );
    // Export the node member data
    p_lv_element->SetAttribute( "type", p_lv_node->type().c_str() );
    p_lv_element->SetAttribute( "name", p_lv_node->name().c_str() );
    if( p_lv_node->words() != std::nullopt ){
      p_lv_element->SetAttribute( "text", p_lv_node->words_to_std_string().c_str() );
      for( auto const & word : *(p_lv_node->words()) ){
        word.to_xml( r_doc, p_lv_element );
      }
    }
    if( p_lv_node->roles() != std::nullopt ){
      for( auto const & [name, value] : *(p_lv_node->roles()) ){
        p_lv_element->SetAttribute( name.c_str(), value.c_str() );
      }
    }
    for( auto const & symbol : p_lv_node->symbols() ){
      symbol->to_xml( r_doc, p_lv_element );
    }
    for( auto const & edge : flattened_info.edges ){
      // Only add child edges with the current node as a parent_name
      if( node_name != edge.parent_name ) continue;
      // Create the new XMLElement node for the connetion & add it to p_lv_element
      tinyxml2::XMLElement *const p_connection_element = r_doc.NewElement("connection");
      p_lv_element->InsertEndChild( p_connection_element );
      if( edge.label != std::nullopt ) p_connection_element->SetAttribute("label", (*edge.label).c_str() );
      p_connection_element->SetAttribute("child-name", edge.child_name.c_str() );
    }
  }
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
  root["type"] = type_;
  root["name"] = name_;
  root["children"] = Json::Value( Json::arrayValue );
  for( const auto& child_connection : children_ ){
    Json::Value child_connection_root;
    if( child_connection.label ){
      child_connection_root["label"] = *(child_connection.label);
    }
    child_connection.child->to_json( child_connection_root );
    root["children"].append( child_connection_root );
  }
  if( o_words_ ){
    root["words"] = Json::Value( Json::arrayValue );

    for( const auto& word : *o_words_ ){
      Json::Value word_root;
      word.to_json( word_root );
      root["words"].append( word_root );
    }
  }

  // TODO - Export roles
  root["symbols"] = Json::Value( Json::arrayValue );
  for( const auto& symbol : symbols_ ){
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
  for( const auto& [node_name, p_lv_node] : flattened_info.nodes ){
    LanguageVariableNodeMessage node_msg;
    node_msg.name = p_lv_node->name();
    node_msg.type = p_lv_node->type();

    // Create symbol messages for each Symbol
    for( const auto& symbol : p_lv_node->symbols() ){
      node_msg.symbols.push_back( symbol->to_msg() );
    }

    // Create word messages for each Word
    for( const auto& word : *(p_lv_node->words()) ){
      node_msg.words.push_back( word.to_msg() );
    }

    // Create role messages for each Role
    for( const auto& role : *(p_lv_node->roles()) ){
      PropertyMessage p;
      p.name = role.first;
      p.value = role.second;
      node_msg.roles.push_back( p );
    }

    // Find all child connections and add to the message
    for( const auto& edge : flattened_info.edges ){
      // Only add child edges with the correct parent name
      if( edge.parent_name != node_name ) continue;

      LanguageVariableConnectionMessage connection_msg;
      if( edge.label ){
        connection_msg.label = *(edge.label);
        connection_msg.label_exists = 1;
      } else{
        connection_msg.label_exists = 0;
      }
      connection_msg.child_name = edge.child_name;
      node_msg.children.push_back( connection_msg );
    }

    // Add this node to the LanguageVariableMessage
    msg.nodes.push_back( node_msg );
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
  auto node_it = info.nodes.find( name_ );
  if( node_it == info.nodes.end() ){ // This node is new!
    // Check if this LV has words
    std::vector<Word> lv_words = std::vector<Word>();
    if( o_words_ ){ lv_words = *o_words_; }

    // Check if this LV has roles
    auto lv_roles = rolesMap();
    if( o_roles_ ){ lv_roles = *o_roles_; }

    // Construct node object
    auto const & [ it_new, success ] = info.nodes.emplace( name_, std::make_unique<LanguageVariable>( *this ) );
    it_new->second->children().clear();
  }

  // Attempt to add child LVs to the flattened information
  for( const auto& connection : children_ ){
    // Add edge information and then process the child LV
    auto lv_edge = language_variable_edge_t();
    lv_edge.parent_name = name_;
    lv_edge.child_name = connection.child->name();
    lv_edge.label = connection.label;
    info.edges.push_back( lv_edge );
    connection.child->flatten_recursive( info );
  }
  return;
}

//
// Returns only the current language-variable absent of any children
//
LanguageVariable LanguageVariable::current( void )const{
  if( (o_words_ != std::nullopt) && (o_roles_ != std::nullopt) ){
    return LanguageVariable( type_, name_, symbols_, std::vector<language_variable_connection_t>(), *o_words_, *o_roles_ );
  } else if( o_words_ != std::nullopt ){
    return LanguageVariable( type_, name_, symbols_, std::vector<language_variable_connection_t>(), *o_words_ );
  } else{
    return LanguageVariable( type_, name_, symbols_, std::vector<language_variable_connection_t>() );
  }
}

//
// Reset the member data to initialization values
//
void LanguageVariable::reset_( void ){
  type_ = std::string();
  name_ = std::string();
  symbols_.clear();
  children_.clear();
  o_words_ = std::nullopt;
  o_roles_ = std::nullopt;
  return;
}

//
// LanguageVariable class ostream operator
//
std::ostream& operator<<( std::ostream& out, const LanguageVariable& other ){
  out << other.type() << "{";
  out << other.name();

  // Output words for this LanguageVariable (does not include words from children)
  if( other.words() ){
    out << ",\"" << other.words_to_std_string() << "\" ";
    out << "words(";
    for( auto it_word = other.words()->cbegin(); it_word != other.words()->cend(); ++it_word ){
      out << *it_word;
      if( std::next(it_word) != other.words()->cend() )
        out << ", ";
    }
    out << ")";
  }

  // Output roles for this LanguageVariable if they exist
  if( other.roles() ){
    out << ",roles(";
    bool first_role = true;
    for( std::pair<std::string,std::string> role_pair : *(other.roles()) ){
      if( !first_role ){
        out << ",";
      }
      out << role_pair.first << ":" << role_pair.second;
      first_role = false;
    }
    out << ")";
  }

  // Output symbols for this LanguageVariable
  if( other.symbols().size() > 0 ){
    out << ",symbols(";
    for( auto it_symbol = other.symbols().cbegin();
                it_symbol != other.symbols().cend();
                ++it_symbol )
    {
      if( *it_symbol != nullptr )
        out << **it_symbol;
      if( std::next(it_symbol) != other.symbols().cend() )
        out << ", ";
    }
    out << ")";
  }

  // Output children connections
  if( other.children().size() > 0 ){
    out << ",children(";
    for( auto it_child = other.children().cbegin();
              it_child != other.children().cend();
              ++it_child )
    { // Child may or may not have an edge label
      if ( it_child->label ) {
        out << "[" << *(it_child->label) << "]:";
      }
      out << (*it_child->child);
      if( std::next(it_child) != other.children().cend() )
        out << ",";
    }
    out << ")";
  }
  out << "}";
  return out;
}

} // namespace h2sl
