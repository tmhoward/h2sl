/**
 * @file symbolcompositionalinfo.h
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

#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <tinyxml2.h>

#include "h2sl/world.h"
#include "h2sl/symbol.h"

namespace h2sl{

/**
    Class to represent SymbolCompositionalInfo
*/
class SymbolCompositionalInfo{
  public:
  /**
    Type alias to contain a Symbol property name and associated possible 
    property values. The name of the property is the std::unordered_map key
    and the value is an unordered_set of possible property values.
  */
  using mapSetSymbolProperties = 
            std::unordered_map< std::string, std::unordered_set<std::string> >;

  /**
    Type alias for an std::unordered_map of SymbolCompositionalInfo objects. The
    keys are Symbol types and the values are SymbolCompositionalInfo objects for
    the Symbol type.
  */
  using mapSymbolCompositionalInfo = 
    std::unordered_map< std::string, std::shared_ptr<SymbolCompositionalInfo> >;

  /**
    Default constructor for h2sl::SymbolCompositionalInfo class.

    @brief Default SymbolCompositionalInfo class
    @param[in]    symbol_type                Symbol type whose compositional info is contained
    @param[in]    language_variable_types    All of the LanguageVariable types associated with symbol_type
    @param[in]    properties                 All property types and their values for symbol_type
    @param[in[    children                   SymbolCompositionalInfo for child Symbols of symbol_type
    @return                                  none
    @throws                                  no expected throws
  */
  SymbolCompositionalInfo( const std::string& symbol_type = "",
      const std::unordered_set<std::string>& language_variable_types = 
                                              std::unordered_set<std::string>(),
      const mapSetSymbolProperties& properties = mapSetSymbolProperties(),
      const mapSymbolCompositionalInfo& children = mapSymbolCompositionalInfo() );
  
  /**
    Default copy constructor for SymbolCompositionalInfo.
  */
  SymbolCompositionalInfo( const SymbolCompositionalInfo& other ) = default;

  /**
    Default move constructor for SymbolCompositionalInfo.
  */
  SymbolCompositionalInfo( SymbolCompositionalInfo&& other ) = default;

  /**
    SymbolCompositionalInfo class constructor to instantiate a SymbolCompositionalInfo
    from an XMLElement*.

    @brief SymbolCompositionalInfo class constructor from XMLelement*.
    @overload
    @param[in]    root        XMLElement* of the SymbolCompositionalInfo to be imported
    @returns                  none
    @throws                   no expected throws
  */
  SymbolCompositionalInfo( const tinyxml2::XMLElement* root );

  /**
    Default SymbolCompositionalInfo destructor.
  */
  virtual ~SymbolCompositionalInfo() = default;

  /**
    Default SymbolCompositionalInfo copy assignment operator.
  */
  SymbolCompositionalInfo& operator=( const SymbolCompositionalInfo& other ) = default;

  /**
    Default SymbolCompositionalInfo move assignment operator
  */
  SymbolCompositionalInfo& operator=( SymbolCompositionalInfo&& other ) = default;

  /**
    This method scrapes compositional symbol information from a Symbol.

    @brief Scrape compositional information from a Symbol.
    @param[in]    lv_type       type of LanguageVariable the Symbol is associated with
    @param[in]    symbol        Symbol to be scraped for compositional info
    @retuns                     boolean flag; true if no errors
    @throws                     no expected throws
  */
  bool scrape_symbol( const std::string& lv_type, Symbol& symbol );

  /**
    This method imports a SymbolCompositionalInfo class from an XML file.

    @brief Import a SymbolCompositionalInfo class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const char* filename );

  /**
    This method imports a SymbolCompositionalInfo class from an XMLElement*.

    @brief Import a SymbolCompositionalInfo class from an XMLElement*.
    @overload   
    @param[in]    symcompinfo_elem    current XMLElement* to an XML 
                                      SymbolCompositionalInfo element
    @returns                          boolean flag; true if there are no errors
    @throws                           no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement* symcompinfo_elem );

  /**
    This method exports a SymbolCompositionalInfo class to an XML file.

    @brief Export a SymbolCompositionalInfo to an XML file.
    @overload
    @param[in]    filename      XML filename to which SymbolCompositionalInfo is exported
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool to_xml( const char* filename ) const;

  /**
    This method exports a SymbolCompositionalInfo class to an XMLDocument object.
    The root XMLElement* takes the current SymbolCompositionalInfo as a child XMLElement.

    @brief Export a SymbolCompostionalInfo to an XMLDocument object.
    @overload
    @param[in]    doc         XMLDocument object for the current document
    @param[in]    root        XMLElement* to accept a SymbolCompositionalInfo 
                              as a child element
    @returns                  none
    @throws                   no expected throws
  */
  void to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root ) const;

  /**
    SymbolCompositionalInfo class ostream operator. Prints the SymbolCompositionalInfo
    to the ostream.

    @brief SymbolCompositionalInfo class ostream operator.
    @param[in]    out       ostream operator class reference.
    @param[in]    other     SymbolCompositionalInfo reference to be printed to ostream
    @returns                reference to modified ostream operator
    @throws                 no expected throws
  */
  friend std::ostream& operator<<( std::ostream& out, 
                                  const SymbolCompositionalInfo& other );

  /** Constant to define the string used in XMLElement Name for SymbolCompositionaLInfo */
  static const char* XMLSymbolCompositionalInfoName;

  /** Constant to define the string used in XMLAttribute Name for symbol_type */
  static const char* XMLSymbolTypeName;

  /** Constant to define the string used in XMLAttribute Name for language_variable_types */
  static const char* XMLLanguageVariableTypesName;

  /** Constant to define the string used in XMLElement Name for properties */
  static const char* XMLPropertiesName;

  /** A string to contain the Symbol type associated with this SymbolCompositionalInfo */
  std::string symbol_type = "";

  /** 
    An unordered_map of strings containing all LanguageVariable types associated with this
    SymbolCompositionalInfo's symbol_type.
  */
  std::unordered_set<std::string> language_variable_types = std::unordered_set<std::string>();

  /** A container to hold the property names and associated values for this symbol_type */
  mapSetSymbolProperties properties = mapSetSymbolProperties();

  /** 
    An unordered_map to hold SymbolCompositionalInfo for any child Symbols of
    this symbol_type. The map keys are the Symbol type for the child Symbol and
    the map values are the associated SymbolCompositionalInfo.
  */
  mapSymbolCompositionalInfo children = mapSymbolCompositionalInfo();

  protected:
  private:

}; // class SymbolCompositionalInfo

} // namespace h2sl
