/**
 * @file symbol.h
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
#include <memory>
#include <unordered_map>

#include <tinyxml2.h>
#include <jsoncpp/json/value.h>

#include "h2sl/expressed-feature.h"
#include "h2sl/SymbolMessage.h"

namespace h2sl {

/**
    Class to represent a Symbol
*/
class Symbol{
  public:
  /** Type alias for an std::unordered_map of strings */
  using propertiesMap = std::unordered_map< std::string, std::string >;

  /**
    Default constructor for h2sl::Symbol class. This method instantiates
    a Symbol of the input type, properties, and child symbols.

    @brief Default Symbol class constructor.
    @param[in]    type        Type of the symbol
    @param[in]    properties  Unordered map of symbol properties as strings
    @param[in]    children    Vector of child symbols
    @returns                  none
    @throws                   no expected throws
  */
  Symbol( const std::string& type = "",
          const propertiesMap& properties = propertiesMap(),
          const std::vector<Symbol>& children = std::vector< Symbol >(),
          const std::map<std::string,std::vector<ExpressedFeature>>& expressedFeatures = std::map< std::string, std::vector<ExpressedFeature> >() );

  /**
    Default Symbol class copy constructor.
  */
  Symbol( const Symbol& other ) = default;

  /**
    Default Symbol class move constructor.
  */
  Symbol( Symbol&& other ) = default;

  /**
    Symbol class constructor to instantiate a Symbol from an XMLElement*.

    @brief Symbol class constructor from XMLElement*.
    @overload
    @param[in]    root        XMLElement* of the Symbol to be imported
    @returns                  none
    @throws                   no expected throws
  */
  Symbol( const tinyxml2::XMLElement* root );
  
  /**
    Class constructor for Symbol from SymbolMessage message.

    @brief Class constructor from SymbolMessage.
    @param[in]  msg   SymbolMessage message
    @returns          none
    @throws           no expected throws
  */
  Symbol( const SymbolMessage& msg );

  /**
    Default Symbol class destructor.
  */
  virtual ~Symbol() = default;

  /**
    Default Symbol class copy assignment operator.
  */
  Symbol& operator=( const Symbol& other ) = default;

  /**
    Default Symbol class move assignment operator.
  */
  Symbol& operator=( Symbol&& other ) = default;

  /**
    Symbol class equality operator to check equality between two Symbols.
    Compares the type, properties, and children.

    @brief Symbol class equality operator.
    @param[in]    other     Symbol reference to be compared
    @returns                boolean flag, true if the Symbols are equal
    @throws                 no expected throws
  */
  bool operator==( const Symbol& other ) const;

  /**
    Symbol class inequality operator to check inequality between two Symbols.
    Compares the type, properties, and children.

    @brief Symbol class inequality operator.
    @param[in]    other     Symbol reference to be compared
    @returns                boolean flag, true if the Symbol are not equal
    @throws                 no expected throws
  */
  bool operator!=( const Symbol& other) const;

  /**
    Method to generate a unique key for the symbol

    @brief Generate a unique key for the symbol
    @returns                  std::string with unique key for symbol
    @throws                   no expected throws
  */
  virtual std::string key( void ) const;

  /**
    This method imports a Symbol class from an XML file.

    @brief Import a Symbol class from an XML file.
    @overload
    @param[in]    filename    filename of the XML file from which to import
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool from_xml( const char* filename );

  /**
    This method imports a Symbol class from an XMLElement*.

    @brief Import a Symbol class from an XMLElement*.
    @overload
    @param[in]    symbol_elem   current XMLElement* to an XML Symbol element
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement* symbol_elem );

  /**
      This method imports a FeatureSpace from a json formatted file.

      @brief This method imports a FeatureSpace from a json formatted file.
      @param[in]    root      json formatting file to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const std::string& value );

  /**
      This method imports a FeatureSpace from a json formatted string.

      @brief This method imports a FeatureSpace from a json formatted string.
      @param[in]    root      json formatting string to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json_string( const std::string& value );

  /**
      This method imports a FeatureSpace from a json value.

      @brief This method imports a FeatureSpace from a json value.
      @param[in]    root      json value to read from
      @returns                success/failure
      @throws                 no expected throws
  */
  virtual bool from_json( const Json::Value& root );

  /**
    This method exports a Symbol class to an XML file.

    @brief Export a Symbol to an XML file.
    @overload
    @param[in]    filename    XML filename to which Symbol class is exported
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool to_xml( const char* filename ) const;

  /**
    This method exports a Symbol class to an XMLDocument object. The root
    XMLElement* takes the current Symbol as a child XMLElement.

    @brief Export a Symbol to an XMLDocument object.
    @overload
    @param[in]    doc       XMLDocument object for the current document
    @param[in]    root      XMLElement* to accept a Symbol as a child element
    @returns                none
    @throws                 no expected throws
  */
  void to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root ) const;

  /**
      This method exports a FeatureSpace to a json formatted file.

      @brief This method exports a FeatureSpace to a json formatted file.
      @param[in]    filename  filename to be written to in json format
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( const std::string& filename )const;

  /**
      This method exports a FeatureSpace to a json formatted string.

      @brief This method exports a FeatureSpace to a json formatted string
      @returns                json formatted string
      @throws                 no expected throws
  */
  virtual std::string to_json( void )const;

  /**
      This method exports a FeatureSpace to a json value.

      @brief This method exports a FeatureSpace to a json value.
      @param[in]    root      root of json value to be written to
      @returns                none
      @throws                 no expected throws
  */
  virtual void to_json( Json::Value& root )const;
  
  /**
      This method exports a Symbol to a SymbolMessage.

      @brief This method exports a Symbol to a SymbolMessage.
      @returns                    message version of the Symbol
      @throws                     no expected throws
  */
  SymbolMessage to_msg( void )const;

  /**
    Symbol class ostream operator. Prints the Symbol to the ostream.

    @brief Symbol class ostream operator.
    @param[in]    out       ostream operator class reference
    @param[in]    other     Symbol to be printed to the ostream
    @returns                reference to modified ostream operator
    @throws                 no expected throws
  */
  friend std::ostream& operator<<( std::ostream& out, const Symbol& other );

  /** std::string containing the type of the Symbol */
  std::string type = "";

  /** std::unordered_map of std::strings containing the properties of the Symbol */
  propertiesMap properties = propertiesMap();

  /** std::vector of child Symbol objects for this Symbol */
  std::vector<Symbol> children = std::vector< Symbol >();

  /** std::vector of expressed features for this symbol */
  std::map< std::string, std::vector< ExpressedFeature > > expressed_features;

  protected:
  private:

}; // class Symbol

} // namespace h2sl
