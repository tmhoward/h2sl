/**
 * @file symboldictionary.h
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

#include "h2sl/symbolcompositionalinfo.h"
#include "h2sl/world.h"

namespace h2sl{

/**
    Class to represent a SymbolDictionary
*/
class SymbolDictionary{
  public:

  /**
    Default constructor for h2sl::SymbolDictionary
  */
  SymbolDictionary() = default;

  /**
    SymbolDictionary class constructor to instantiate a SymbolDictionary from an
    XMLElement*.

    @brief SymbolDictionary class constructor from XMLElement*
    @overload
    @param[in]    root        XMLElement* of the SymbolDictionary to be imported
    @returns                  none
    @throws                   no expected throws
  */
  explicit SymbolDictionary( const tinyxml2::XMLElement* root );

  /**
    SymbolDictionary class constructor to instantiate a SymbolDictionary from a
    filename.

    @brief SymbolDictionary class constructor from filename
    @overload
    @param[in]    filename    filename of the SymbolDictionary to be imported
    @returns                  none
    @throws                   no expected throws
  */
  explicit SymbolDictionary( const char* filename );
 
  /**
    Default copy constructor for h2sl::SymbolDictionary
  */
  SymbolDictionary( const SymbolDictionary& other ) = default;

  /**
    Default move constructor for h2sl::SymbolDictionary
  */
  SymbolDictionary( SymbolDictionary&& other ) = default;

 
  /**
    Default destructor for h2sl::SymbolDictionary
  */
  virtual ~SymbolDictionary() = default;

  /**
    Default assignment operator for h2sl::SymbolDictionary
  */
  SymbolDictionary& operator=( const SymbolDictionary& other ) = default;

  /**
    Default move assignment operator for h2sl::SymbolDictionary
  */
  SymbolDictionary& operator=( SymbolDictionary&& other ) = default;

  /**
    This method imports a SymbolDictionary from a file.

    @brief This method imports a SymbolDictionary from a file.
    @param[in]    filename  file to be imported from
    @returns                success/failure
    @throws                 no expected throws
  */
  virtual bool from_file( const char* filename );

  /**
    This method imports a SymbolDictionary class from an XML file.

    @brief Import a SymbolDictionary class from an XML file.
    @overload
    @param[in]    filename      filename of the XML file from which to import
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool from_xml( const char* filename );

  /**
    This method imports a SymbolDictionary class from an XMLElement*.

    @brief Import a SymbolDictionary class from an XMLELement*.
    @overload
    @param[in]    symdict_elem    current XMLElement* to an XML SymbolDictionary element
    @returns                      boolean flag; true if there are no errors
    @throws                       no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement* symdict_elem );

  /**
    This method exports a SymbolDictionary class to an XML file.

    @brief Export a SymbolDictionary to an XML File.
    @overload
    @param[in]    filename      XML filename to which SymbolDictionary is exported
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool to_xml( const char* filename ) const;

  /**
    This method exports a SymbolDictionary class to an XMLDocument object. The 
    root XMLElement* takes the current SymbolDictionary as a child XMLElement.

    @brief Export a SymbolDictionary to an XMLDocument object.
    @overload
    @param[in]    doc       XMLDocument object for the current document
    @param[in]    root      XMLElement* to accept a SymbolDictionary as a child element
    @returns                none
    @throws                 no expected throws
  */
  void to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root ) const;

  /**
    SymbolDictionary class ostream operator. Prints the SymbolDictionary to the ostream.

    @brief SymbolDictionary class ostream operator.
    @param[in]    out       ostream operator class reference.
    @param[in]    other     SymbolDictionary reference to be printed to ostream
    @returns                reference to modified ostream operator
    @throws                 no expected throws
  */
  friend std::ostream& operator<<( std::ostream& out, const SymbolDictionary& other );


  /**Constant to define the string used in XMLElement Name for SymbolDictionary */
  static const char* XMLSymbolDictionaryName;

  /**
    An unordered_map of SymbolCompositionalInfo shared_ptrs with key values of
    the Symbol type
  */
  SymbolCompositionalInfo::mapSymbolCompositionalInfo dictionary = 
                        SymbolCompositionalInfo::mapSymbolCompositionalInfo();
}; // class SymbolDictionary

} // namespace h2sl
