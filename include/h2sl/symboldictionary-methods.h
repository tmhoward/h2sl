/**
 * @file symboldictionary-methods.h
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

#include "h2sl/language-variable.h"
#include "h2sl/sentence.h"
#include "h2sl/symboldictionary.h"

namespace h2sl{

  /** Type alias for an std::vector of std::pairs of World and Sentence */
  using vectorPairWorldSentence = std::vector<std::pair< std::shared_ptr<World>, std::shared_ptr<Sentence> >>;

  /**
    This method scrapes compositional information from Symbols contained in an 
    associated LanguageVariable class and updates the dictionary member accordingly.

    @brief Scrape compositional information from a LanguageVariable and its Symbols.
    @param[in]    symbolDictionary    Symboldictionary
    @param[in]    language_variable        LanguageVariable from which to scrape information
    @returns                    boolean flag; true if no errors
    @throws                     no expected throws 
  */
  bool symboldictionary_scrape_language_variable( SymbolDictionary& symbolDictionary, 
                                       const std::shared_ptr<LanguageVariable>& language_variable );

  /**
    This method scrapes compositional symbol information from all of the objects in the world.

    @brief Scrape symbol compositional information from all objects in the world.
    @param[in]    symbolDictionary    Symboldictionary
    @param[in]    world               World to scrape
    @returns                          boolean flag; true if no errors
    @throws                           no expected throws
  */
  bool symboldictionary_scrape_world( SymbolDictionary& symbolDictionary, 
                                      const std::shared_ptr<World>& world );

  /**
    This method scrapes compositional symbol information from multiple sentences
    and their associated Symbols, and semantic symbols associated with the objects in the world.
    This method leverages the scrape_language_variable and scrape_world function for each World-Sentence pair.

    @brief Scrape compositional information from a collection of Sentences and their Symbols.
    @param[in]    wsPairs       The World/Sentence pairs to scrape
    @returns                    boolean flag; true if no errors
    @throws                     no expected throws
  */
  bool symboldictionary_scrape_world_sentence_pairs( SymbolDictionary& symbolDictionary, 
                                                     vectorPairWorldSentence& wsPairs );

} // namespace h2sl
