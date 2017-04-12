/**
 * @file    feature_generator.h
 * @author  Thomas M. Howard (tmhoward@csail.mit.edu)
 *          Matthew R. Walter (mwalter@csail.mit.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
 *
 * Copyright (C) 2014 by the Massachusetts Institute of Technology
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
 *
 * The interface for a class used to generate features from a symbol
 *   dictionary
 */

#ifndef H2SL_FEATURE_GENERATOR_H
#define H2SL_FEATURE_GENERATOR_H

#include <vector>
#include <map>

#include "h2sl/grammar.h"
#include "h2sl/feature_set.h"
#include "h2sl/symbol_dictionary.h"

namespace h2sl {
  void feature_generator_add_grounding_features( const std::map< std::string, std::vector< Grammar_Terminal > >& grammarTerminals,
                                                  const std::map< std::string, std::vector< Grammar_Non_Terminal > >& grammarNonTerminals,
                                                  const Symbol_Dictionary& symbolDictionary, 
                                                  Feature_Set& featureSet );
  void feature_generator_add_rule_features( const std::map< std::string, std::vector< Grammar_Terminal > >& grammarTerminals,
                                            const std::map< std::string, std::vector< Grammar_Non_Terminal > >& grammarNonTerminals,
                                            const Symbol_Dictionary& symbolDictionary,
                                            Feature_Set& featureSet );
  void feature_generator_add_cv_features( Feature_Set& featureSet );
  void feature_generator_add_word_features( const phrase_type_t& phraseType,
                                            const std::vector< Grammar_Terminal >& grammarTerminals,
                                            const Symbol_Dictionary& symbolDictionary,
                                            Feature_Set& featureSet );
  void feature_generator_add_grounding_class_name_features( Feature_Set& featureSet );
  void feature_generator_add_rule_class_name_features( Feature_Set& featureSet );
  void feature_generator_add_object_features( const Symbol_Dictionary& symbolDictionary, 
                                              Feature_Set& featureSet );
  void feature_generator_add_region_features( const Symbol_Dictionary& symbolDictionary, 
                                              Feature_Set& featureSet );
  void feature_generator_add_constraint_features( const Symbol_Dictionary& symbolDictionary, 
                                                  Feature_Set& featureSet );
  void feature_generator_add_spatial_relation_features( const Symbol_Dictionary& symbolDictionary, 
                                                        Feature_Set& featureSet );
  void feature_generator_add_object_type_features( const Symbol_Dictionary& symbolDictionary, 
                                                        Feature_Set& featureSet );
  void feature_generator_add_object_color_features( const Symbol_Dictionary& symbolDictionary, 
                                                        Feature_Set& featureSet );
  void feature_generator_add_object_property_features( const Symbol_Dictionary& symbolDictionary, 
                                                        Feature_Set& featureSet );
  void feature_generator_add_container_type_features( const Symbol_Dictionary& symbolDictionary, 
                                                        Feature_Set& featureSet );
  void feature_generator_add_index_features( const Symbol_Dictionary& symbolDictionary, 
                                                        Feature_Set& featureSet );
  void feature_generator_add_number_features( const Symbol_Dictionary& symbolDictionary, 
                                                        Feature_Set& featureSet );
  void feature_generator_add_abstract_container_features( const Symbol_Dictionary& symbolDictionary, 
                                                        Feature_Set& featureSet );
  void feature_generator_add_region_abstract_container_features( const Symbol_Dictionary& symbolDictionary, 
                                                        Feature_Set& featureSet );
  void feature_generator_add_container_features( const Symbol_Dictionary& symbolDictionary, 
                                                        Feature_Set& featureSet );
  void feature_generator_add_region_container_features( const Symbol_Dictionary& symbolDictionary, 
                                                        Feature_Set& featureSet );
  void feature_generator_add_rule_spatial_relation_features( const Symbol_Dictionary& symbolDictionary, 
                                                              Feature_Set& featureSet );
  void feature_generator_add_rule_object_type_features( const Symbol_Dictionary& symbolDictionary, 
                                                              Feature_Set& featureSet );
  void feature_generator_add_rule_object_color_features( const Symbol_Dictionary& symbolDictionary, 
                                                              Feature_Set& featureSet );
  void feature_generator_add_rule_constraint_type_features( const Symbol_Dictionary& symbolDictionary, 
                                                              Feature_Set& featureSet );
  void feature_generator_add_rule_constraint_payload_type_features( const Symbol_Dictionary& symbolDictionary, 
                                                              Feature_Set& featureSet );
  void feature_generator_add_rule_constraint_reference_type_features( const Symbol_Dictionary& symbolDictionary, 
                                                              Feature_Set& featureSet );
  void feature_generator_add_rule_container_type_features( const Symbol_Dictionary& symbolDictionary, 
                                                              Feature_Set& featureSet );
  void feature_generator_add_rule_index_features( const Symbol_Dictionary& symbolDictionary, 
                                                              Feature_Set& featureSet );
  void feature_generator_add_rule_number_features( const Symbol_Dictionary& symbolDictionary, 
                                                              Feature_Set& featureSet );

}

#endif /* H2SL_FEATURE_GENERATOR_H */
