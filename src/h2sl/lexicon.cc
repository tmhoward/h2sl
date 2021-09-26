/**
 * @file lexicon.cc
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

#include "h2sl/common.h"
#include "h2sl/lexicon.h"

namespace h2sl{

//
// Lexicon class constructor from XMLElement*
//
Lexicon::Lexicon( const tinyxml2::XMLElement* root ) : type_information()
{
  from_xml( root );
}

//
// This method scrapes information from a LanguageVariable into the lexicon.
//
bool Lexicon::_scrape_language_variable( const std::shared_ptr<h2sl::LanguageVariable>& language_variable )
{
  // Make a recursive call for each child language variable
  for( const auto& connection : language_variable->children ){
    _scrape_language_variable( connection.child );
  }

  // Try inserting an entry for the current LV type into the map if it does not exist
  const auto& [it_type_info, type_success] = type_information.emplace( language_variable->type, lv_type_information_t() );

  // Iterate through words in this LanguageVariable
  if( language_variable->words ){
    for( const auto& word : language_variable->words.value() ){
      // Try to insert an unordered map for the POS type
      const auto& [it_pos_words, pos_success] = it_type_info->second.words.emplace( word.pos, Lexicon::setStrings() );
      // Try to insert the current word text into the word set
      const auto& [it_word, word_success] = it_pos_words->second.emplace( word.text );
      if( word_success ){
        word_count++;
      }
    }
  }

  // Iterate through edge labels under this LanguageVariable
  for( const auto& connection : language_variable->children ){
    // Try to insert the current edge label if it exists
    if( connection.label ){
      it_type_info->second.edge_labels.emplace( connection.label.value() );
    }
  }

  // Iterate through symbols under this LanguageVariable
  for( const auto& symbol : language_variable->symbols ){
    // Try to insert the current symbol type
    it_type_info->second.symbol_types.emplace( symbol->type );
  }

  // Iterate through roles of this LanguageVariable
  if( language_variable->roles ){
    for( const auto& role : language_variable->roles.value() ){
      // Try inserting a new set for this role into the LV type's roles map in case it does not exist
      auto [it_roles, role_success] = it_type_info->second.roles.emplace( role.first, setStrings() );
      it_roles->second.emplace( role.second );
    }
  }
  return true;
}


//
// This method scrapes information from Sentences into the lexicon.
//
bool Lexicon::scrape_sentences( const std::vector<std::shared_ptr<h2sl::Sentence>>& sentences )
{
  for( const auto& sentence : sentences ){
    if( !_scrape_language_variable( sentence->child ) ){
      return false;
    }
  }
  return true;
}

//
// Method to load from a file
//
bool Lexicon::from_file( const char* filename ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return from_xml( filename );
  } else {
    std::cerr << "[Lexicon Class Error] Erroneous file suffix (\"" << filename << "\") in from_file";
    return false;
  }
}

//
// Method to import a Lexicon from an XML file
//
bool Lexicon::from_xml( const char* filename ){
  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename );

  // Check for loading errors
  if( doc.Error() )
    return false;

  // Extract the root element from the document object
  const auto* root = doc.RootElement();
  if( root == nullptr )
    return false;

  // Check for any Lexicon child element of the root
  const auto* lexicon_elem = root->FirstChildElement( XMLLexiconName );
  if( lexicon_elem == nullptr )
    return false;

  return from_xml( lexicon_elem );
}

//
// Method to import a Lexicon from an XMLElement*
//
bool Lexicon::from_xml( const tinyxml2::XMLElement* lexicon_elem ){
  type_information.clear();
  word_count = 0;

  // Check that the element is a lexicon element
  if( lexicon_elem == nullptr )
    return false;
  if( std::strcmp(lexicon_elem->Name(), XMLLexiconName) != 0 )
    return false;

  // Check for any child LanguageVariable type XMLElement*
  const tinyxml2::XMLElement* lv_type_elem = lexicon_elem->FirstChildElement("language-variable-type");
  if( lv_type_elem == nullptr )
    return false;

  while( lv_type_elem != nullptr ){

    // Import the LV type from the XMLAttribute
    const tinyxml2::XMLAttribute* type_attr = lv_type_elem->FindAttribute("type");
    if( type_attr == nullptr )
      return false;
    std::string lv_type = type_attr->Value();
    lv_type_information_t info;

    // Import the words from the XMLAttribute if any exist
    const auto* words_element = lv_type_elem->FirstChildElement("words");
    if( words_element != nullptr ){
      // Import the words via the POS attribute and comma separated string text attribute
      const auto* pos_attr = words_element->FirstAttribute();
      // Enforce that a word element has both part-of-speech and text attributes
      if( pos_attr == nullptr ) return false;
      while( pos_attr != nullptr ){
        info.words.emplace( pos_attr->Name(), std_string_set_from_std_string( pos_attr->Value() ) );
        pos_attr = pos_attr->Next();
      }
    }

    // Import the comma separated string of edge labels from the XMLAttribute if it exists
    const tinyxml2::XMLAttribute* labels_attr = lv_type_elem->FindAttribute("edge-labels");
    if( labels_attr != nullptr ){
      std::string edge_labels_string = labels_attr->Value();
      info.edge_labels = std_string_set_from_std_string(edge_labels_string);
    }

    // Import the comma separated string of symbol types from the XMLAttribute if it exists
    const tinyxml2::XMLAttribute* symbol_types_attr = lv_type_elem->FindAttribute("symbol-types");
    if( symbol_types_attr != nullptr ){
      std::string symbol_types_string = symbol_types_attr->Value();
      info.symbol_types = std_string_set_from_std_string(symbol_types_string);
    }

    // Import the roles from the XMLAttribute if any exist
    const tinyxml2::XMLElement* roles_elem = lv_type_elem->FirstChildElement("roles");
    if( roles_elem != nullptr ){
      // Import all roles_elem attributes to this LV's role information
      const tinyxml2::XMLAttribute* role_attr = roles_elem->FirstAttribute();
      while( role_attr != nullptr ){
        info.roles.emplace( role_attr->Name(), std_string_set_from_std_string( role_attr->Value() ) );
        role_attr = role_attr->Next();
      }
    }

    // Attempt to add the created lv_type_information_t to the type information map 
    auto result = type_information.emplace( lv_type, info );
    if( !result.second ){
      return false;
    }

    lv_type_elem = lv_type_elem->NextSiblingElement("language-variable-type");
  }

  return true;
}

//
// Method to export a Lexicon to an XML file
//
bool Lexicon::to_xml( const char* filename ) const{
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
// Method to export a Lexicon to an XMLDocument object
//
void Lexicon::to_xml( tinyxml2::XMLDocument& doc, 
                      tinyxml2::XMLElement* root) const
{
  tinyxml2::XMLElement* lexicon_elem = doc.NewElement( XMLLexiconName );
  root->InsertEndChild( lexicon_elem );
  
  // Create a language-variable-type element for each type in the Lexicon
  for( const auto& [lv_type, info] : type_information ){

    tinyxml2::XMLElement* lv_type_elem = doc.NewElement( "language-variable-type" );

    // Export language variable type as an XMLAttribute
    lv_type_elem->SetAttribute( "type", lv_type.c_str() );
   
    // If there are words, create a corresponding child XMLElement
    // Get and export each POS and set of word texts
    if( !info.words.empty() ){
      auto words_element = doc.NewElement("words");
      for( const auto& [pos, text] : info.words ){
        auto text_string = std_string_set_to_std_string( text );
        words_element->SetAttribute( pos.c_str(), text_string.c_str() );
      }
      lv_type_elem->InsertEndChild( words_element );
    }

    // If there are edge labels, get and export their string.
    if( !info.edge_labels.empty() ){
      std::vector<std::string> edge_labels;
      for( const auto& label : info.edge_labels ){
        edge_labels.push_back(label);
      }

      // Export edge labels string as an XMLAttribute
      auto edge_labels_string = std_string_vector_to_std_string( edge_labels );
      lv_type_elem->SetAttribute( "edge-labels", edge_labels_string.c_str() );
    }

    // If there are symbol types, get and export their string.
    if( !info.symbol_types.empty() ){
      std::vector<std::string> symbol_types;
      for( const auto& type : info.symbol_types ){
        symbol_types.push_back( type );
      }

      // Export symbol types string as an XMLAttribute
      auto symbol_types_string = std_string_vector_to_std_string( symbol_types );
      lv_type_elem->SetAttribute( "symbol-types", symbol_types_string.c_str() );
    }

    // If there are roles, create a corresponding child XML element
    // Then for each role, get and export its string.
    if( !info.roles.empty() ){
      tinyxml2::XMLElement* roles_elem = doc.NewElement( "roles" );

      // Export each role as an XMLAttribute
      for( const auto& it_roles : info.roles ){
        std::vector<std::string> role_values;
        for( const auto& role : it_roles.second ){
          role_values.push_back( role );
        }

        // Export roles string as an XMLAttribute
        auto roles_string = std_string_vector_to_std_string( role_values );
        roles_elem->SetAttribute( it_roles.first.c_str(), roles_string.c_str() );
      }
      lv_type_elem->InsertEndChild( roles_elem );
    }
    
    // Add LV type element to the lexicon
    lexicon_elem->InsertEndChild( lv_type_elem );
  }

  return;
}

//
// Lexicon class ostream operator
//
std::ostream& operator<<( std::ostream& out, const Lexicon& other ){
  out << "Lexicon( " << "cardinality: " << other.word_count << std::endl;
  for( const auto& [lv_type, info] : other.type_information ){
    out << lv_type << " :\n";
    // Print words if there are any
    if( !info.words.empty() ){
      unsigned int i = 0;
      out << "\twords{";
      for( const auto& [pos, text] : info.words ){
        out << pos << "[";
        unsigned int j = 0;
        for( const auto& word_text : text ){
          out << word_text;
          j++;
          if( j < text.size() ) out << ",";
        }
        out << "]";
        i++;
        if( i < info.words.size() ){
          out << ",";
        }
      }
      out << "}" << std::endl;
    }

    // Print edge labels if there are any
    if( !info.edge_labels.empty() ){
      unsigned int i = 0;
      out << "\tedge_labels{";
      for( const auto& label : info.edge_labels ){
        out << label;
        i++;
        if( i < info.edge_labels.size() ){
          out << ",";
        }
      }
      out << "}" << std::endl;
    }
  
    // Print symbol types if there are any
    if( !info.symbol_types.empty() ){
      unsigned int i = 0;
      out << "\tsymbol_types{";
      for( const auto& type : info.symbol_types ){
        out << type;
        i++;
        if( i < info.symbol_types.size() ){
          out << ",";
        }
      }
      out << "}" << std::endl;
    }

    // Print roles if there are any
    if( !info.roles.empty() ){
      unsigned int i = 0;
      out << "\troles{";
      for( const auto& it_roles : info.roles ){
        out << it_roles.first << "[";
        unsigned int j = 0;
        for( const auto& role : it_roles.second ){
          out << role;
          j++;
          if( j < it_roles.second.size() ){
            out << ",";
          }
        }
        out << "]";
        i++;
        if( i < info.roles.size() ){
          out << ",";
        }
      }
      out << "}" << std::endl;
    }
  }
  out << ")";
  return out;
}

//
// Constant used for XMLElement Name for Lexicon element
//
const char* Lexicon::XMLLexiconName = "lexicon";

} // namespace h2sl
