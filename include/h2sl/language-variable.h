/**
 * @file language-variable.h
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
#include <vector>
#include <tuple>
#include <memory>
#include <optional>

#include <tinyxml2.h>
#include <jsoncpp/json/value.h>

#include "h2sl/word.h"
#include "h2sl/symbol.h"
#include "h2sl/symbolspace.h"
#include "h2sl/world.h"
#include "h2sl/LanguageVariableMessage.h"

namespace h2sl{

/**
  Class to represent a LanguageVariable
*/
class LanguageVariable{
  public:
  /** Type alias for a std::vector of Symbol std::shared_ptrs */
  using symbolsVector = std::vector< std::shared_ptr<Symbol> >;

  /** Type alias for a std::unordered_map of LanguageVariable roles */
  using rolesMap = std::unordered_map< std::string, std::string >;

  /** Structure for representing LanguageVariable child connections */
  struct language_variable_connection_t{
    language_variable_connection_t( void ) = default;
    language_variable_connection_t( std::optional<std::string> l, std::shared_ptr<LanguageVariable> c ) : label( l ), child( c ){}

    std::optional<std::string> label = std::nullopt;
    std::shared_ptr<LanguageVariable> child = nullptr;
  };

  /** Structure to contain information about an edge between two LV nodes */
  struct language_variable_edge_t{
    std::string parent_name = std::string();
    std::string child_name = std::string();
    std::optional<std::string> label = std::nullopt;
  };

  /** Structure for storing tree information from a flattened LV */
  struct flattened_language_variable_t{
    // Default flattened_language_variable_t constructor
    flattened_language_variable_t( void ) = default;

    /** Maps variable names to corresponding LanguageVariables */
    std::unordered_map< std::string, std::unique_ptr<LanguageVariable> >
    nodes = std::unordered_map< std::string, std::unique_ptr<LanguageVariable> >();

    /** Stores edge connections */
    std::vector<language_variable_edge_t> edges = std::vector<language_variable_edge_t>();
  };

  /**
    Default constructor for h2sl::LanguageVariable class
  **/
  LanguageVariable() = default;

  /**
    Parameter constructor for h2sl::LanguageVariable class. This method instantiates a
    LanguageVariable of the input type, its unique name, vector of Words composing
    its text, vector of Symbols with which it corresponds, unordered map of child
    LanguageVariables, and unordered map of its roles (other non-core information).

    @brief Default LanguageVariable class constructor.
    @param[in]    type        Type of the LanguageVariable
    @param[in]    name        Unique variable name of the LanguageVariable
    @param[in]    symbols     Vector of Symbols corresponding to the language variable
    @param[in]    children    Vector of child language_variable_connection_t
    @param[in]    words       Vector of Words composing the language variable
    @param[in]    roles       Map of additional roles of the language variable
    @returns                  none
    @throws                   no expected throws
  */
  LanguageVariable( std::string const & type, std::string const & name, symbolsVector const & symbols,
                    std::vector<language_variable_connection_t> const & children,
                    std::vector<Word> const & words, rolesMap const & roles);

  /**
    Parameter constructor for h2sl::LanguageVariable class. This method instantiates a
    LanguageVariable of the input type, its unique name, vector of Words composing
    its text, vector of Symbols with which it corresponds, unordered map of child
    LanguageVariables. The roles are set as nullopt.

    @brief Default LanguageVariable class constructor.
    @param[in]    type        Type of the LanguageVariable
    @param[in]    name        Unique variable name of the LanguageVariable
    @param[in]    symbols     Vector of Symbols corresponding to the language variable
    @param[in]    children    Vector of child language_variable_connection_t
    @param[in]    words       Vector of Words composing the language variable
    @returns                  none
    @throws                   no expected throws
  */
  LanguageVariable( std::string const & type, std::string const & name, symbolsVector const & symbols,
                    std::vector<language_variable_connection_t> const & children, std::vector<Word> const & words );

  /**
    Parameter constructor for h2sl::LanguageVariable class. This method instantiates a
    LanguageVariable of the input type, its unique name, its text, vector of Symbols with
    which it corresponds, unordered map of child LanguageVariables. The roles and words are
    set as nullopt.

    @brief Default LanguageVariable class constructor.
    @param[in]    type        Type of the LanguageVariable
    @param[in]    name        Unique variable name of the LanguageVariable
    @param[in]    symbols     Vector of Symbols corresponding to the language variable
    @param[in]    children    Vector of child language_variable_connection_t
    @returns                  none
    @throws                   no expected throws
  */
  LanguageVariable( std::string const & type, std::string const & name, symbolsVector const & symbols,
                    std::vector<language_variable_connection_t> const & children );

  /**
    LanguageVariable class constructor to instantiate a LanguageVariable from an XMLElement*.

    @brief LanguageVariable class constructor from XMLElement*.
    @overload
    @param[in]    p_lv_element    XMLElement* of the LanguageVariable to be imported
    @param[in]    p_symbolspace   SymbolSpace object to insert symbols
    @returns                      none
    @throws                       no expected throws
  */
  LanguageVariable( tinyxml2::XMLElement const * p_lv_element, SymbolSpace *const p_symbolspace );

  /**
    This method imports a LanguageVariable from a LanguageVariableMessage.

    @brief This method imports a LanguageVariable from a LanguageVariableMessage.
    @param[in]    msg           LanguageVariableMessage message
    @returns                    imported LanguageVariable; std::nullopt if there are errors
    @throws                     no expected throws
  */
  static std::optional<LanguageVariable> from_msg( const LanguageVariableMessage& msg );

  /**
    Default copy constructor for h2sl::LanguageVariable
  */
  LanguageVariable( const LanguageVariable& other ) = default;

  /**
    Default move constructor for h2sl::LanguageVariable
  */
  LanguageVariable( LanguageVariable&& other ) = default;

  /**
    Default destructor for h2sl::LanguageVariable
  */
  virtual ~LanguageVariable() = default;

  /**
    Default assignment operator for h2sl::LanguageVariable
  */
  LanguageVariable& operator=( const LanguageVariable& other ) = default;

  /**
    Default move assignment operator for h2sl::LanguageVariable
  */
  LanguageVariable& operator=( LanguageVariable&& other )= default;

  /**
    Method to clear all Symbols from this and all children language variables

    @brief Method to clear all Symbols from this and all children language variables
    @returns                  none
    @throws                   no expected throws
  */
  void clear_symbols( void );

  /**
    Method to generate a unique key for the language variable

    @brief Generate a unique key for the language variable
    @returns                  std::string with unique key for language variable
    @throws                   no expected throws
  */
  virtual std::string key( void ) const;

  /**
    Method to convert the words member to a concatenated string. This recursively
    includes the words from all child language variables.

    @brief Convert words to a concatenated string.
    @returns                  concatenated std::string of words
    @throws                   no expected throws
  */
  std::string words_to_std_string( void ) const;

  /**
    Method to return all Words in a vector. This recursively includes the Words
    from all child language variables.

    @brief Return all Words in a vector for this LanguageVariable and its children.
    @returns                  vector of Words from this and all child language variables
    @throws                   no expected throws
  */
  std::vector<Word> all_words( void ) const;

  /**
    This method imports a LanguageVariable in hierarchical or flattened format from an XML file.

    @brief This method imports a LanguageVariable in hierarchical or flattened format from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    none
    @throws                     Throws runtime_error on XML parsing error
  */
  void from_xml( const char* filename );

  /**
    This method imports a LanguageVariable in hierarchical or flattened format from an XML file.

    @brief This method imports a LanguageVariable in hierarchical or flattened format from an XML file.
    @overload
    @param[in]    filename        filename of the XML file from which to import
    @param[in]    p_symbolspace   SymbolSpace object for inserting symbols
    @returns                      none
    @throws                       Throws runtime_error on XML parsing error
  */
  void from_xml( const char* filename, SymbolSpace *const p_symbolspace );

  /**
    This method imports a LanguageVariable in hierarchical or flattened form from an XMLElement* with a symbol space.

    @brief Import a LanguageVariable class from an XMLElement* with a symbol space.
    @overload
    @param[in]    p_lv_element              current XMLElement* to an XML LanguageVariable element
    @param[in]    p_symbolspace             SymbolSpace object for inserting symbols
    @param[in]    speaker                   Speaker role value
    @returns                                none
    @throws                                 Throws runtime_error on XML parsing error
  */
  void from_xml( tinyxml2::XMLElement const * p_lv_element, SymbolSpace *const p_symbolspace,
                  std::string const & speaker = std::string() );

  /**
    This method imports a LanguageVariable class from an XMLElement* with a symbol space.

    @brief Import a LanguageVariable class from an XMLElement* with a symbol space.
    @overload
    @param[in]    p_lv_element              current XMLElement* to an XML LanguageVariable element
    @param[in]    p_symbolspace             SymbolSpace object for inserting symbols
    @param[in]    speaker                   Speaker role value
    @returns                                none
    @throws                                 Throws runtime_error on XML parsing error
  */
  void hierarchical_from_xml( tinyxml2::XMLElement const * p_lv_element, SymbolSpace *const p_symbolspace,
                              std::string const & speaker );

  /**
    This method imports a LanguageVariable in flattened form from an XMLElement*.

    @brief This method imports a LanguageVariable in flattened form from an XMLElement*.
    @overload
    @param[in]    p_lv_element    current XMLElement* to an XML LanguageVariable element
    @param[in]    p_symbolspace   SymbolSpace object for inserting symbols
    @param[in]    speaker         Speaker role value
    @returns                      none
    @throws                       Throws runtime_error on XML parsing error
  */
  void flattened_from_xml( tinyxml2::XMLElement const * p_lv_element, SymbolSpace *const symbolspace,
                            std::string const & speaker );

  /**
    This method finds and returns the root variable of the imported information.

    @brief This method finds and returns the root variable of the imported information.
    @returns      std::optional<std::string> containing root var if no errors
    @throws       no expected throws
  */
  static std::optional<std::string> find_root( const flattened_language_variable_t& info );

  /**
    This method constructs a LanguageVariable based on the imported information.
    In the case that no value has been mapped for the given variable name, std::nullopt is returned.

    @brief This method constructs a LanguageVariable based on the imported information.
    @param[in]    var_name      std::string containing LV variable name
    @param[in]    info          flattened_language_variable_t with imported information
    @returns      lv            std::optional<h2sl::LanguageVariable>; std::nullopt on error
    @throws                     no expected throws
  */
  static std::optional<LanguageVariable> construct_lv( const std::string& var_name, const flattened_language_variable_t& info );

  /**
      This method imports a LanguageVariable from a json formatted string.

      @brief This method imports a LanguageVariable from a json formatted string.
      @param[in]    json_string   json formatting string to read from
      @returns                    success/failure
      @throws                     no expected throws
  */
  virtual bool from_json( const std::string& json_string );

  /**
      This method imports a LanguageVariable from a json value.

      @brief This method imports a LanguageVariable from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
    This method exports a LanguageVariable in hierarchical format to an XML file.

    @brief This method exports a LanguageVariable in hierarchical format to an XML file.
    @overload
    @param[in]    filename    XML filename to which LanguageVariable class is exported
    @returns                  none
    @throws                   Throws runtime error on export error
  */
  void to_xml( const char* filename )const;

  /**
    This method exports a LanguageVariable class in hierarchical format to an XMLDocument object. The root
    XMLElement* takes the current LanguageVariable as a child XMLElement.

    @brief Export a LanguageVariable class to an XMLDocument object.
    @overload
    @param[in]    r_doc       XMLDocument object for the current document
    @param[in]    p_root      XMLElement* to accept a LanguageVariable as a child element
    @returns                  none
    @throws                   no expected throws
  */
  void to_xml( tinyxml2::XMLDocument& r_doc, tinyxml2::XMLElement* p_root )const;

  /**
    This method exports a LanguageVariable in flattened format to an XML file.

    @brief This method exports a LanguageVariable in flattened format to an XML file.
    @overload
    @param[in]    filename    XML filename to which LanguageVariable class is exported
    @return                   none
    @throws                   Throws runtime error on export error
  */
  void to_flattened_xml( const char* filename )const;

  /**
    This method exports a LanguageVariable class to an XMLDocument object using a flattened representation.

    @brief Export a LanguageVariable class to an XMLDocument as a flattened representation
    @param[in]    r_doc       XMLDocument object for the current document
    @param[in]    p_root      XMLElement* to accept a LanguageVariable as a child element
    @returns                  none
    @throws                   no expected throws
  */
  void to_flattened_xml( tinyxml2::XMLDocument& r_doc, tinyxml2::XMLElement* p_root )const;

  /**
      This method exports a LanguageVariable to a json formatted string.

      @brief This method exports a LanguageVariable to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a LanguageVariable to a json value.

      @brief This method exports a LanguageVariable to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;

  /**
      This method exports a LanguageVariable to a LanguageVariableMessage.

      @brief This method exports a LanguageVariable to a LanguageVariableMessage.
      @returns                message version of the LanguageVariable
      @throws                 no expected throws
  */
  LanguageVariableMessage to_msg( void )const;

  /**
    This method returns a flattened version of the LanguageVariable.

    @brief This method returns a flattened version of the LanguageVariable.
    @returns                  flattened_language_variable_t version of the LanguageVariable
  */
  flattened_language_variable_t flatten( void )const;

  /**
    Recursive helper method for flatten() method.

    @brief Recursive helper method for flatten() method.
    @param[in]    info        flattened_language_variable_t storing found LV information
    @returns      none
    @throws       no expected throws
  */
  void flatten_recursive( flattened_language_variable_t& info )const;

  /**
    This method returns only the current language variable.

    @brief This method returns only the current language variable.
    @returns      copy of the current language variable
    @throws       no expected throws
  */
  LanguageVariable current( void )const;

  /**
    These methods provide read-only and read-write access to the type member data
  **/
  inline std::string const & type( void )const{ return type_; };
  inline std::string& type( void ){ return type_; };

  /**
    These methods provide read-only and read-write access to the name member data
  **/
  inline std::string const & name( void )const{ return name_; };
  inline std::string& name( void ){ return name_; };

  /**
    These methods provide read-only and read-write access to the symbols member data
  **/
  inline symbolsVector const & symbols( void )const{ return symbols_; };
  inline symbolsVector& symbols( void ){ return symbols_; };

  /**
    These methods provide read-only and read-write access to the children member data
  **/
  inline std::vector<language_variable_connection_t> const & children( void )const{ return children_; };
  inline std::vector<language_variable_connection_t>& children( void ){ return children_; };

  /**
    These methods provide read-only and read-write access to the o_words member data
  **/
  inline std::optional< std::vector<Word> > const & words( void )const{ return o_words_; };
  inline std::optional< std::vector<Word> >& words( void ){ return o_words_; };

  /**
    These methods provide read-only and read-write access to the o_roles member data
  **/
  inline std::optional< rolesMap > const & roles( void )const{ return o_roles_; };
  inline std::optional< rolesMap >& roles( void ){ return o_roles_; };

  /**
    LanguageVariable class ostream operator. Prints the LanguageVariable to the ostream.

    @brief LanguageVariable class ostream operator.
    @param[in]    out       ostream operator class reference
    @param[in]    other     LanguageVariable to be printed to the ostream
    @returns                referece to modified ostream operator
    @throws                 no expected throws
  */
  friend std::ostream& operator<<( std::ostream& out, const LanguageVariable& other );

  protected:
  private:
  /**
    This method validates that a purported language-variable XMLElement* (1) is not nullptr and (2) has the expected name.

    @brief This method validates a language-variable XMLElement
    @param[in]    p_lv_element    XMLElement* to validate
    @returns                      none
    @throws                       Throws runtime_error if invalid
  **/
  void validate_lv_element_( tinyxml2::XMLElement const *const p_lv_element );

  /**
    This method resets the member data to an initialized state.

    @brief This method resets the member data to an initialized state
    @return                       none
    @throws                       No expected throws
  **/
  void reset_( void );

  // The type of the language variable
  std::string type_ = std::string();

  // The name of the language variable
  std::string name_ = std::string();

  // std::vector containing std::shared_ptrs to Symbol objects corresponding to the LanguageVariable
  symbolsVector symbols_ = symbolsVector();

  // std::vector containing the child LanguageVariables for the LanguageVariable
  std::vector<language_variable_connection_t> children_ = std::vector<language_variable_connection_t>();

  // Optional std::vector containing the Words of the LanguageVariable
  std::optional< std::vector<Word> > o_words_ = std::nullopt;

  // Optional std::unordered_map containing any non-core roles of the LanguageVariable
  std::optional< rolesMap > o_roles_ = std::nullopt;
}; // class LanguageVariable

} // namespace h2sl
