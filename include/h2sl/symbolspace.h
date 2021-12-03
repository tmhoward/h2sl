/**
 * @file symbolspace.h
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
#include <unordered_map>
#include <vector>
#include <memory>

#include <tinyxml2.h>

#include "h2sl/symbol.h"
#include "h2sl/world.h"
#include "h2sl/symboldictionary.h"

namespace h2sl{

/**
    Class to represent a SymbolSpace
*/
class SymbolSpace{
  public:
  /**
    Type alias for an std::vector of Symbol std::shared_ptrs of the same Symbol
    type but different instantiated properties, e.g. object_type(ball) and
    object_type(block). The alias name reads "Vector of Symbols of the same Type".
  */
  using symbolsVectorType = std::vector< std::shared_ptr<Symbol> >;

  /**
    Type alias for an std::unordered_map of symbolsVectorType. The key is the
    Symbol type for a given symbolsVectorType. The value is the symbolsVectorType.
    The alias name reads "Map of Vectors of Symbols of the same Type".
  */
  using symbolsMapVectorType = std::unordered_map<std::string, symbolsVectorType>;

  /**
    Default constructor for h2sl::SymbolSpace class. This method instantiates
    a SymbolSpace with an input symbolsMapVectorType.

    @brief Default SymbolSpace class constructor.
    @param[in]    symbolsMap    Unordered map of vectors of Symbols keyed by type.
    @returns                    none
    @throws                     no expected throws

  */
  explicit SymbolSpace( const symbolsMapVectorType& symbolsMap = symbolsMapVectorType() );

  /**
    Default SymbolSpace class copy constructor.
  */
  SymbolSpace( const SymbolSpace& other ) = default;

  /**
    Default SymbolSpace class move constructor.
  */
  SymbolSpace( SymbolSpace&& other ) = default;

  /**
    SymbolSpace class constructor to instantiate a SymbolSpace from an XMLElement*.

    @brief  Symbolspace class constructor from XMLElement*.
    @overload
    @param[in]    root          XMLElement* of the SymbolSpace to be imported
    @returns                    none
    @throws                     no expected throws
  */
  SymbolSpace( const tinyxml2::XMLElement* root );

  /**
    SymbolSpace class constructor to instantiate a SymbolSpace from a SymbolDictionary and a World

    @brief  SymbolSpace class constructor to instantiate a SymbolSpace from a SymbolDictionary and a World
    @param[in]    symbolDictionary  SymbolDictionary input
    @param[in]    world             World input
    @returns                        none
    @throws                         no expected throws
  */
  SymbolSpace( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  /**
    Default SymbolSpace class destructor.
  */
  virtual ~SymbolSpace() = default;

  /**
    Default SymbolSpace class copy assignment operator.
  */
  SymbolSpace& operator=( const SymbolSpace& other ) = default;

  /**
    Default SymbolSpace class move assignment operator.
  */
  SymbolSpace& operator=( SymbolSpace&& other ) = default;

  /**
    Method to fill a SymbolSpace from a SymbolDictionary and a World

    @brief  Method to fill a SymbolSpace from a SymbolDictionary and a World
    @param[in]    symbolDictionary  SymbolDictionary input
    @param[in]    world             World input
    @returns      bool              return false when missing symbol dictionary entry
    @throws                         no expected throws
  */
  virtual bool fill( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_objects( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_object_types( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_object_colors( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_object_quantifiers( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_object_semantic_property_types( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_spatial_relation_extrema( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_spatial_relation_axis( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_spatial_relations( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_distances( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_distance_units( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_regions( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_relations( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  virtual bool fill_actions( const std::shared_ptr<SymbolDictionary>& symbolDictionary, const std::shared_ptr<World>& world );

  /**
    This method inserts a Symbol into the symbolsMap to store it as a shared_ptr.
    If an identical Symbol already exists, it is not inserted but the
    shared_ptr is still returned.

    @brief Insert a new Symbol into the symbolsMap.
    @param[in]    symbol    Symbol to be inserted
    @returns                std::shared_ptr to the inserted Symbol
    @throws                 no expected throws
  */
  std::shared_ptr< Symbol > insert( const Symbol& symbol );

  /**
    This method searches for a provided Symbol in the symbolsMap and returns the
    associated shared_ptr. If not found, it returns a nullptr value.

    @brief Search for a provided symbol in symbolsMap.
    @param[in]    symbol    Symbol to be searched for
    @returns                std::shared_ptr to the provided Symbol, nullptr if not found
    @throws                 no expected throws
  */
  std::shared_ptr< Symbol > find_symbol( const Symbol& symbol );

  /**
    This method imports a SymbolSpace class from an XML file.

    @brief Import a SymbolSpace class from an XML file.
    @overload
    @param[in]    filename    filename of the XML file from which to import
    @returns                  boolean flag; true if there are no errors
    @throws                   no expected throws
  */
  bool from_xml( const char* filename );

  /**
    This method imports a SymbolSpace class from an XMLElement*.

    @brief Import a SymbolSpace class from an XMLElement*.
    @overload
    @param[in]    symSpace_elem   current XMLElement* to an XML SymbolSpace element
    @returns                      boolean flag; true if there are no errors
    @throws                       no expected throws
  */
  bool from_xml( const tinyxml2::XMLElement* symSpace_elem );

  /**
    This method exports a SymbolSpace class to an XML file.

    @brief Export a SymbolSpace to an XML file.
    @overload
    @param[in]    filename      XML filename to which SymbolSpace is exported
    @returns                    boolean flag; true if there are no errors
    @throws                     no expected throws
  */
  bool to_xml( const char* filename ) const;

  /**
    This method exports a SymbolSpace class to an XMLDocument object. The root
    XMLElement* takes the current SymbolSpace as a child XMLElement.

    @brief Export a SymbolSpace to an XMLDocument object.
    @overload
    @param[in]    doc       XMLDocument object for the current document
    @param[in]    root      XMLElement* to accept a SymbolSpace as a child element
    @returns                none
    @throws                 no expected throws
  */
  void to_xml( tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root ) const;

  /**
    SymbolSpace class ostream operator. Prints the SymbolSpace to the ostream.

    @brief SymbolSpace class ostream operator.
    @param[in]    out       ostream operator class reference
    @param[in]    other     SymbolSpace reference to be printed to the ostream
    @returns                reference to modifed ostream operator
    @throws                 no expected throws
  */
  friend std::ostream& operator<<( std::ostream& out, const SymbolSpace& other );

  /** An unordered_map of vectors of Symbol shared_ptrs */
  symbolsMapVectorType symbolsMap;

  protected:
  private:

}; // class SymbolSpace

} // namespace h2sl
