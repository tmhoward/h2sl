/**
 * @file    feature_generator.cc
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
 * The implementation of a class for automatically generating features
 *   from a symbol dictionary
 */

#include "h2sl/feature_generator.h"

// groundings
#include "h2sl/spatial_relation.h"
#include "h2sl/constraint.h"
#include "h2sl/object_property.h"
#include "h2sl/object_type.h"
#include "h2sl/container_type.h"
#include "h2sl/object_color.h"
#include "h2sl/number.h"
#include "h2sl/index.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_abstract_container.h"
#include "h2sl/container.h"
#include "h2sl/region_container.h"
#include "h2sl/rule_object_type.h"
#include "h2sl/rule_object_color.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/rule_constraint_type.h"
#include "h2sl/rule_constraint_payload_type.h"
#include "h2sl/rule_constraint_reference_type.h"
#include "h2sl/rule_container_type.h"
#include "h2sl/rule_number.h"
#include "h2sl/rule_index.h"

// cv features
#include "h2sl/feature_cv.h"

// phrase features
#include "h2sl/feature_phrase_type_and_word.h"
#include "h2sl/feature_phrase_type_and_num_words.h"
#include "h2sl/feature_phrase_type_and_num_child_phrases.h"
#include "h2sl/feature_phrase_type_and_phrase_has_pos_tag.h"

// template features
#include "h2sl/feature_matches_child.h"
#include "h2sl/feature_matches_only_child.h"
#include "h2sl/feature_matches_ordered_child.h"
#include "h2sl/feature_merge_children.h"

// grounding features
#include "h2sl/feature_grounding_class_name.h"
#include "h2sl/feature_grounding_string_property_value.h"
#include "h2sl/feature_grounding_int_property_value.h"
#include "h2sl/feature_grounding_string_property_value_matches_child_string_property_value.h"
#include "h2sl/feature_grounding_string_property_value_missing_child_string_property.h"
#include "h2sl/feature_grounding_string_property_value_matches_child_string_properties.h"
#include "h2sl/feature_grounding_class_name.h"

// object features
#include "h2sl/feature_object_merge_abstract_container_spatial_relation.h"
#include "h2sl/feature_object_merge_object_property_container.h"
#include "h2sl/feature_object_merge_region_abstract_container_container.h"
#include "h2sl/feature_object_merge_object_type_spatial_relation.h"
#include "h2sl/feature_object_merge_object_type_region.h"
#include "h2sl/feature_object_merge_object_property_region_container.h"
#include "h2sl/feature_object_merge_object_property_spatial_relation.h"
#include "h2sl/feature_object_merge_object_type_region_container.h"
#include "h2sl/feature_object_matches_sorted_objects.h"
#include "h2sl/feature_object_matches_sorted_object.h"
#include "h2sl/feature_object_matches_child_container_object.h"
#include "h2sl/feature_object_matches_child_region_container_object.h"
#include "h2sl/feature_is_in_a_container.h"
#include "h2sl/feature_object_shape_abstract_container.h"
#include "h2sl/feature_object_merge_abstract_container_region_container.h"
#include "h2sl/feature_object_merge_container_pair.h"
#include "h2sl/feature_object_merge_object_region_container.h"

// region features
#include "h2sl/feature_region_object_matches_child.h"
#include "h2sl/feature_region_object_string_property_value.h"
#include "h2sl/feature_region_merge_spatial_relation_and_object.h"
#include "h2sl/feature_region_merge_spatial_relation_and_region.h"

// constraint features
#include "h2sl/feature_constraint_payload_string_property_value.h"
#include "h2sl/feature_constraint_reference_matches_child.h"
#include "h2sl/feature_constraint_object_relationship.h"

// object property features
#include "h2sl/feature_object_property_merge_index_object_property.h"

// container type features
#include "h2sl/feature_container_type_merge_container_type_number.h"

// number features
#include "h2sl/feature_number_merge_container_type_number.h"

// index features

// abstract container features
#include "h2sl/feature_abstract_container_number_equals_world_objects.h"

// region abstract container features


// container features
#include "h2sl/feature_container_object_type.h"
#include "h2sl/feature_container_object_color.h"
#include "h2sl/feature_container_number.h"
#include "h2sl/feature_container_number_equals_world_objects.h"
#include "h2sl/feature_container_type_merge_container_type_number.h"
#include "h2sl/feature_container_merge_abstract_container_spatial_relation.h"
#include "h2sl/feature_container_merge_abstract_container_container_type.h"
#include "h2sl/feature_container_merge_abstract_container_region.h"
#include "h2sl/feature_container_merge_abstract_container_region_container.h"
#include "h2sl/feature_container_merge_region_abstract_container_container.h"
#include "h2sl/feature_container_merge_container_type_container.h"
#include "h2sl/feature_container_merge_container_region.h"
#include "h2sl/feature_container_is_empty.h"
#include "h2sl/feature_container_matches_sorted_objects.h"
#include "h2sl/feature_container_container_type_matches_child_region_container_container_container_type.h"
#include "h2sl/feature_container_merge_container_pair.h"
#include "h2sl/feature_container_shape_abstract_container.h"

// region container features
#include "h2sl/feature_region_container_matches_child.h"
#include "h2sl/feature_region_container_merge_container_spatial_relation.h"

// rule_object_type features
#include "h2sl/feature_rule_object_type_merge_rule_object_types.h"
#include "h2sl/feature_rule_object_type_merge_rule_container_type_rule_object_type.h"

// rule_object_color features
#include "h2sl/feature_rule_object_color_merge_rule_object_type_rule_spatial_relation.h"

// rule_spatial_relation features
#include "h2sl/feature_rule_spatial_relation_merge_rule_spatial_relations.h"
#include "h2sl/feature_rule_spatial_relation_merge_rule_spatial_relation_rule_container_type.h"

// rule_container_type features
#include "h2sl/feature_rule_container_type_merge_rule_container_types.h"
#include "h2sl/feature_rule_container_type_merge_rule_object_types.h"
#include "h2sl/feature_rule_container_type_equals_world_objects.h"

// rule_index features
#include "h2sl/feature_rule_index_merge_rule_container_type_rule_index.h"

using namespace std;

namespace h2sl {

  void
  feature_generator_add_grounding_features( const map< string, vector< Grammar_Terminal > >& grammarTerminals,
                                            const map< string, vector< Grammar_Non_Terminal > >& grammarNonTerminals,
                                            const Symbol_Dictionary& symbolDictionary, 
                                            Feature_Set& featureSet ){
    for( map< string, vector< Grammar_Terminal > >::const_iterator it_grammar_terminals = grammarTerminals.begin(); it_grammar_terminals != grammarTerminals.end(); it_grammar_terminals++ ){
      cout << "checking phrase " << it_grammar_terminals->first << endl;
  
      featureSet.feature_products().push_back( new Feature_Product() );
      feature_generator_add_cv_features( featureSet );

      feature_generator_add_word_features( Phrase::phrase_type_t_from_std_string( it_grammar_terminals->first ), it_grammar_terminals->second, symbolDictionary, featureSet );

      featureSet.feature_products().back()->feature_groups().push_back( vector< Feature* >() );
      
      map< string, vector< string > >::const_iterator it_class_names = symbolDictionary.class_names().find( it_grammar_terminals->first );
    
      feature_generator_add_grounding_class_name_features( featureSet );

      if( find( it_class_names->second.begin(), it_class_names->second.end(), "object" ) != it_class_names->second.end() ){ 
        feature_generator_add_object_features( symbolDictionary, featureSet );
      }  
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "region" ) != it_class_names->second.end() ){
        feature_generator_add_region_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "constraint" ) != it_class_names->second.end() ){
        feature_generator_add_constraint_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "spatial_relation" ) != it_class_names->second.end() ){
        feature_generator_add_spatial_relation_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "object_type" ) != it_class_names->second.end() ){
        feature_generator_add_object_type_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "object_color" ) != it_class_names->second.end() ){
        feature_generator_add_object_color_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "object_property" ) != it_class_names->second.end() ){
        feature_generator_add_object_property_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "container_type" ) != it_class_names->second.end() ){
        feature_generator_add_container_type_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "index" ) != it_class_names->second.end() ){
        feature_generator_add_index_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "number" ) != it_class_names->second.end() ){
        feature_generator_add_number_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "abstract_container" ) != it_class_names->second.end() ){
        feature_generator_add_abstract_container_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "region_abstract_container" ) != it_class_names->second.end() ){
        feature_generator_add_region_abstract_container_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "container" ) != it_class_names->second.end() ){
        feature_generator_add_container_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "region_container" ) != it_class_names->second.end() ){
        feature_generator_add_region_container_features( symbolDictionary, featureSet );
      }
    }
    return;
  }

  void
  feature_generator_add_rule_features( const map< string, vector< Grammar_Terminal > >& grammarTerminals,
                                        const map< string, vector< Grammar_Non_Terminal > >& grammarNonTerminals,
                                        const Symbol_Dictionary& symbolDictionary,
                                        Feature_Set& featureSet ){
    for( map< string, vector< Grammar_Terminal > >::const_iterator it_grammar_terminals = grammarTerminals.begin(); it_grammar_terminals != grammarTerminals.end(); it_grammar_terminals++ ){
      cout << "checking phrase " << it_grammar_terminals->first << endl;

      featureSet.feature_products().push_back( new Feature_Product() );
      feature_generator_add_cv_features( featureSet );

      feature_generator_add_word_features( Phrase::phrase_type_t_from_std_string( it_grammar_terminals->first ), it_grammar_terminals->second, symbolDictionary, featureSet );

      featureSet.feature_products().back()->feature_groups().push_back( vector< Feature* >() );

      map< string, vector< string > >::const_iterator it_class_names = symbolDictionary.class_names().find( it_grammar_terminals->first );

      feature_generator_add_rule_class_name_features( featureSet );


      if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_spatial_relation" ) != it_class_names->second.end() ){
        feature_generator_add_rule_spatial_relation_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_object_type" ) != it_class_names->second.end() ){
        feature_generator_add_rule_object_type_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_object_color" ) != it_class_names->second.end() ){
        feature_generator_add_rule_object_color_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_constraint_type" ) != it_class_names->second.end() ){
        feature_generator_add_rule_constraint_type_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_constraint_payload_type" ) != it_class_names->second.end() ){
        feature_generator_add_rule_constraint_payload_type_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_constraint_reference_type" ) != it_class_names->second.end() ){
        feature_generator_add_rule_constraint_reference_type_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_container_type" ) != it_class_names->second.end() ){
        feature_generator_add_rule_container_type_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_index" ) != it_class_names->second.end() ){
        feature_generator_add_rule_index_features( symbolDictionary, featureSet );
      }
      if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_number" ) != it_class_names->second.end() ){
        feature_generator_add_rule_number_features( symbolDictionary, featureSet );
      }
    }
    return;
  }

  void
  feature_generator_add_cv_features( Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().push_back( vector< Feature* >() );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_CV( false, "false" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_CV( false, "true" ) );
    return;
  }

  void
  feature_generator_add_grounding_class_name_features( Feature_Set& featureSet ){   
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "object" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "region" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "constraint" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "spatial_relation" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "object_property" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "object_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "container_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "object_color" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "number" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "index" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "abstract_container" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "region_abstract_container" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "container" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "region_container" ) );
    return;
  }
  
  void
  feature_generator_add_rule_class_name_features( Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_constraint_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_constraint_payload_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_constraint_reference_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_container_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_spatial_relation" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_object_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_object_color" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_number" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_index" ) );
    return;
  }

  void
  feature_generator_add_word_features( const phrase_type_t& phraseType,
                                        const vector< Grammar_Terminal >& grammarTerminals,
                                        const Symbol_Dictionary& symbolDictionary,
                                        Feature_Set& featureSet ){
    // world features
    featureSet.feature_products().back()->feature_groups().push_back( vector< Feature* >() );
    for( unsigned int i = 0; i < grammarTerminals.size(); i++ ){
      if( pos_t_from_std_string( grammarTerminals[ i ].symbol() ) != POS_UNKNOWN ){
        for( unsigned int j = 0; j < grammarTerminals[ i ].words().size(); j++ ){
          Word word( pos_t_from_std_string( grammarTerminals[ i ].symbol() ), grammarTerminals[ i ].words()[ j ] );
          featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Word( false, word, phraseType ) );
        }
      }
    }
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Phrase_Has_POS_Tag( false, POS_CD, phraseType ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Phrase_Has_POS_Tag( false, POS_JJ, phraseType ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Phrase_Has_POS_Tag( false, POS_JJS, phraseType ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Phrase_Has_POS_Tag( false, POS_POS, phraseType ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Num_Words( false, 0, phraseType ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Num_Child_Phrases( false, 0, phraseType ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Num_Child_Phrases( false, 1, phraseType ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Num_Child_Phrases( false, 2, phraseType ) );
    return;
  }

  void
  feature_generator_add_object_features( const Symbol_Dictionary& symbolDictionary,
                                          Feature_Set& featureSet ){
    // object symbols
    map< string, vector< string > >::const_iterator it_object_types = symbolDictionary.string_types().find( "object_type" );
    if( it_object_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object", "object_type", *it_object_type ) ); 
      }
    }
    map< string, vector< string > >::const_iterator it_object_colors = symbolDictionary.string_types().find( "object_color" );
    if( it_object_colors != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_color = it_object_colors->second.begin(); it_object_color != it_object_colors->second.end(); it_object_color++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object", "object_color", *it_object_color ) );
      }
    }
    map< string, vector< string > >::const_iterator it_container_types = symbolDictionary.string_types().find( "container_type" );
    if( it_container_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_container_type = it_container_types->second.begin(); it_container_type != it_container_types->second.end(); it_container_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Is_In_A_Container( false, *it_container_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Is_In_A_Container( true, *it_container_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Shape_Abstract_Container( false, *it_container_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Shape_Abstract_Container( true, *it_container_type ) );
      }
    }
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "min_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "min_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "max_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "max_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "min_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "min_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "max_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "max_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "min_abs_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "min_abs_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "max_abs_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "max_abs_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "min_abs_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "min_abs_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "max_abs_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "max_abs_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "min_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "min_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "max_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "max_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "min_center_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "min_center_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( false, "max_center_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Container_Pair( true, "max_center_distance" ) );
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_center_distance" ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_center_distance" ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( true, *it_spatial_relation_type, "max_center_distance" ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( false, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Spatial_Relation( true, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( false, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region( true, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( true, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "max_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( false, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "max_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Spatial_Relation( true, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "max_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, false, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "max_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( false, true, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "max_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, false, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "max_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Region_Abstract_Container_Container( true, true, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( false, "max_center_distance", *it_spatial_relation_type ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "min_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "max_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "min_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "max_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "min_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "max_abs_x", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "min_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "max_abs_y", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "min_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "max_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "min_center_distance", *it_spatial_relation_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Type_Region_Container( true, "max_center_distance", *it_spatial_relation_type ) );
      }
    }
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object >( false ) ); 
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object >( true ) ); 

    map< string, vector< int > >::const_iterator it_indices = symbolDictionary.int_types().find( "index" );
    if( it_indices != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_index = it_indices->second.begin(); it_index != it_indices->second.end(); it_index++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_index, "max_center_distance" ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_index, "max_center_distance" ) );
      }
    }

    map< string, vector< int > >::const_iterator it_numbers = symbolDictionary.int_types().find( "number" );
    if( it_numbers != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_number = it_numbers->second.begin(); it_number != it_numbers->second.end(); it_number++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_center_distance" ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( true, *it_number, "max_center_distance" ) );
      }
    }
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Child_Container_Object( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Child_Container_Object( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Region_Container( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Region_Container( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Child_Region_Container_Object( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Child_Region_Container_Object( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Object, Container >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Object, Container >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Object, Container >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Object, Container >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Object, Region_Container >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Object, Region_Container >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Object, Region_Container >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Object, Region_Container >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "object", "object_type", "object", "object_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "object", "object_type", "object", "object_type" ) );
  return;
  }

  void
  feature_generator_add_region_features( const Symbol_Dictionary& symbolDictionary,
                                          Feature_Set& featureSet ){
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "region", "spatial_relation_type", *it_spatial_relation_type ) );
      }
    }
    map< string, vector< string > >::const_iterator it_object_types = symbolDictionary.string_types().find( "object_type" );
    if( it_object_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Object_String_Property_Value( false, "object_type", *it_object_type ) );
      }
    }
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Object_Matches_Child( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Object( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Object( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Region( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Region( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "region", "spatial_relation_type", "region", "spatial_relation_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "region", "spatial_relation_type", "region", "spatial_relation_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "region", "object_id", "region", "object_id" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "region", "object_id", "region", "object_id" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "region", "spatial_relation_type", "spatial_relation", "spatial_relation_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "region", "spatial_relation_type", "spatial_relation", "spatial_relation_type" ) );
  return;
}

void
feature_generator_add_constraint_features( const Symbol_Dictionary& symbolDictionary,
                                            Feature_Set& featureSet ){
  map< string, vector< string > >::const_iterator it_constraint_types = symbolDictionary.string_types().find( "constraint_type" );
    if( it_constraint_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_constraint_type = it_constraint_types->second.begin(); it_constraint_type != it_constraint_types->second.end(); it_constraint_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "constraint", "constraint_type", *it_constraint_type ) );
      }
    }
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "constraint", "reference_relation", *it_spatial_relation_type ) );
      } 
    }
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Constraint >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Constraint >( true ) );
    map< string, vector< string > >::const_iterator it_object_types = symbolDictionary.string_types().find( "object_type" );
    if( it_object_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Constraint_Object_Relationship( false, "min_distance", *it_object_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Constraint_Object_Relationship( false, "max_distance",  *it_object_type ) );
      }
    }
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "constraint", "payload", "object", "id" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "constraint", "payload", "object", "id" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "constraint", "reference", "object", "id" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "constraint", "reference", "object", "id" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "constraint", "reference_relation", "region", "spatial_relation_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "constraint", "reference_relation", "region", "spatial_relation_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "constraint", "reference", "region", "object_id" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "constraint", "reference", "region", "object_id" ) );
    map< string, vector< string > >::const_iterator it_constraint_payload_type_types = symbolDictionary.string_types().find( "constraint_payload_type" );
    for( vector< string >::const_iterator it_constraint_payload_type = it_constraint_payload_type_types->second.begin(); it_constraint_payload_type != it_constraint_payload_type_types->second.end(); it_constraint_payload_type++ ){
      featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Constraint_Payload_String_Property_Value( false, "object_type", *it_constraint_payload_type ) );
    }
    return;
  }

  void
  feature_generator_add_spatial_relation_features( const Symbol_Dictionary& symbolDictionary,
                                                   Feature_Set& featureSet ){
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "spatial_relation", "spatial_relation_type", *it_spatial_relation_type ) );
      }
    }
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Spatial_Relation >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Spatial_Relation >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Spatial_Relation >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Spatial_Relation >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "spatial_relation", "spatial_relation_type", "spatial_relation", "spatial_relation_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "spatial_relation", "spatial_relation_type", "spatial_relation", "spatial_relation_type" ) );
  return;
}

  void
  feature_generator_add_object_type_features( const Symbol_Dictionary& symbolDictionary,
                                                  Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object_Type >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object_Type >( true ) );
    map< string, vector< string > >::const_iterator it_object_types = symbolDictionary.string_types().find( "object_type" );
    if( it_object_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object_type", "object_type", *it_object_type ) );
      }
    }
    return;
  }

  void
  feature_generator_add_object_color_features( const Symbol_Dictionary& symbolDictionary,
                                                  Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object_Color >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object_Color >( true ) );
    map< string, vector< string > >::const_iterator it_object_colors = symbolDictionary.string_types().find( "object_color" );
    if( it_object_colors != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_color = it_object_colors->second.begin(); it_object_color != it_object_colors->second.end(); it_object_color++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object_color", "object_color_type", *it_object_color ) );
      }
    }
    return;
  }

  void
  feature_generator_add_object_property_features( const Symbol_Dictionary& symbolDictionary,
                                                  Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object_Property >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object_Property >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Property_Merge_Index_Object_Property( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Object_Property_Merge_Index_Object_Property( true ) );
    map< string, vector< string > >::const_iterator it_object_types = symbolDictionary.string_types().find( "object_type" );
    if( it_object_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object_property", "object_type", *it_object_type ) );
      }
    }
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object_property", "spatial_relation_type", *it_spatial_relation_type ) );
      }
    }
    map< string, vector< int > >::const_iterator it_indices = symbolDictionary.int_types().find( "index" );
    if( it_indices != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_index = it_indices->second.begin(); it_index != it_indices->second.end(); it_index++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "object_property", "index", *it_index ) );
      }
    }
    return;
  }

  void
  feature_generator_add_container_type_features( const Symbol_Dictionary& symbolDictionary,
                                                  Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Container_Type >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Container_Type >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Type_Merge_Container_Type_Number( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Type_Merge_Container_Type_Number( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Number_Merge_Container_Type_Number( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Number_Merge_Container_Type_Number( true ) );
    map< string, vector< string > >::const_iterator it_container_types = symbolDictionary.string_types().find( "container_type" );
    if( it_container_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_container_type = it_container_types->second.begin(); it_container_type != it_container_types->second.end(); it_container_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "container_type", "container_type", *it_container_type ) );
      }
    }
    return;
  }

  void
  feature_generator_add_index_features( const Symbol_Dictionary& symbolDictionary,
                                                  Feature_Set& featureSet ){
  featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Index >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Index >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Index >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Index >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Index, Region_Container >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Index, Region_Container >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Index, Region_Container >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Index, Region_Container >( true, true ) );
    map< string, vector< int > >::const_iterator it_indices = symbolDictionary.int_types().find( "index" );
    if( it_indices != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_index = it_indices->second.begin(); it_index != it_indices->second.end(); it_index++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "index", "value", *it_index ) );
      }
    }
    return;
  }

  void
  feature_generator_add_number_features( const Symbol_Dictionary& symbolDictionary,
                                                  Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Number >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Number >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Number >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Number >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Number, Region_Container >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Number, Region_Container >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Number, Region_Container >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Number, Region_Container >( true, true ) );
    map< string, vector< int > >::const_iterator it_numbers = symbolDictionary.int_types().find( "number" );  
    if( it_numbers != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_number = it_numbers->second.begin(); it_number != it_numbers->second.end(); it_number++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "number", "value", *it_number ) ); 
      }
    }
    return;
  }

  void
  feature_generator_add_abstract_container_features( const Symbol_Dictionary& symbolDictionary,
                                                  Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Abstract_Container >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Abstract_Container >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Abstract_Container >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Abstract_Container >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Object >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Object >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Object >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Object >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Region_Container >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Region_Container >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Region_Container >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Region_Container >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Region >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Region >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Region >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Abstract_Container, Region >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Abstract_Container_Number_Equals_World_Objects( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Abstract_Container_Number_Equals_World_Objects( true ) );
    map< string, vector< string > >::const_iterator it_object_types = symbolDictionary.string_types().find( "object_type" );
    if( it_object_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "abstract_container", "object_type", *it_object_type ) );
      }
    }
    map< string, vector< int > >::const_iterator it_indices = symbolDictionary.int_types().find( "index" );
    if( it_indices != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_index = it_indices->second.begin(); it_index != it_indices->second.end(); it_index++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "abstract_container", "index", *it_index ) );
      }
    }
    map< string, vector< int > >::const_iterator it_numbers = symbolDictionary.int_types().find( "number" );
    if( it_numbers != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_number = it_numbers->second.begin(); it_number != it_numbers->second.end(); it_number++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "abstract_container", "number", *it_number ) );
      }
    }
    map< string, vector< string > >::const_iterator it_object_colors = symbolDictionary.string_types().find( "object_color" );
    if( it_object_colors != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_color = it_object_colors->second.begin(); it_object_color != it_object_colors->second.end(); it_object_color++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "abstract_container", "object_color", *it_object_color ) );
      }
    }
    return;
  }


  void
  feature_generator_add_region_abstract_container_features( const Symbol_Dictionary& symbolDictionary,
                                                  Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region_Abstract_Container >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region_Abstract_Container >( true ) );
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "region_abstract_container", "spatial_relation_type", *it_spatial_relation_type ) );
      }
    }
    map< string, vector< string > >::const_iterator it_object_types = symbolDictionary.string_types().find( "object_type" );
    if( it_object_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "region_abstract_container", "object_type", *it_object_type ) );
      }
    }
    map< string, vector< int > >::const_iterator it_indices = symbolDictionary.int_types().find( "index" );
    if( it_indices != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_index = it_indices->second.begin(); it_index != it_indices->second.end(); it_index++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "region_abstract_container", "index", *it_index ) );
      }
    }
    map< string, vector< int > >::const_iterator it_numbers = symbolDictionary.int_types().find( "number" );
    if( it_numbers != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_number = it_numbers->second.begin(); it_number != it_numbers->second.end(); it_number++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "region_abstract_container", "number", *it_number ) );
      }
    }
    map< string, vector< string > >::const_iterator it_object_colors = symbolDictionary.string_types().find( "object_color" );
    if( it_object_colors != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_color = it_object_colors->second.begin(); it_object_color != it_object_colors->second.end(); it_object_color++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "region_abstract_container", "object_color", *it_object_color ) );
      }
    }
    return;
  }

  void
  feature_generator_add_container_features( const Symbol_Dictionary& symbolDictionary,
                                                  Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Container >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Container >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Is_Empty( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Is_Empty( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Type_Container( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Type_Container( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Type_Container( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Type_Container( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Region( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Region( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Container_Type( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Container_Type( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Container_Type_Matches_Child_Region_Container_Container_Container_Type( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "min_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "min_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "max_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "max_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "min_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "min_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "max_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "max_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "min_abs_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "min_abs_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "max_abs_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "max_abs_x" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "min_abs_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "min_abs_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "max_abs_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "max_abs_y" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "min_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "min_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "max_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "max_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "min_center_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "min_center_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( false, "max_center_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Container_Pair( true, "max_center_distance" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Container, Spatial_Relation >( false, false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Container, Spatial_Relation >( true, false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Container, Spatial_Relation >( false, true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Container, Spatial_Relation >( true, true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Container, Spatial_Relation >( false, false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Container, Spatial_Relation >( true, false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Container, Spatial_Relation >( false, true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Container, Spatial_Relation >( true, true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "container", "container_type", "container_type", "container_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "container", "container_type", "container_type", "container_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "container", "container_type", "container", "container_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "container", "container_type", "container", "container_type" ) );
    map< string, vector< string > >::const_iterator it_container_types = symbolDictionary.string_types().find( "container_type" );
    if( it_container_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_container_type = it_container_types->second.begin(); it_container_type != it_container_types->second.end(); it_container_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "container", "container_type", *it_container_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Shape_Abstract_Container( false, *it_container_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Shape_Abstract_Container( true, *it_container_type ) );
      }
    }

    map< string, vector< string > >::const_iterator it_object_types = symbolDictionary.string_types().find( "object_type" );
    if( it_object_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Object_Type( false, *it_object_type ) );
      }
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Number_Equals_World_Objects( false, *it_object_type ) );
      }
    }
    map< string, vector< string > >::const_iterator it_object_colors = symbolDictionary.string_types().find( "object_color" );
    if( it_object_colors != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_color = it_object_colors->second.begin(); it_object_color != it_object_colors->second.end(); it_object_color++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Object_Color( false, *it_object_color ) );
      }
    }
    map< string, vector< int > >::const_iterator it_numbers = symbolDictionary.int_types().find( "number" );
    if( it_numbers != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_number = it_numbers->second.begin(); it_number != it_numbers->second.end(); it_number++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Number( false, *it_number ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Matches_Sorted_Objects( false, *it_number, "max_center_distance" ) );
      }
    }
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_center_distance" ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region( false, *it_spatial_relation_type, "max_center_distance" ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Region_Container( false, *it_spatial_relation_type, "max_center_distance" ) );
        //
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "min_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "max_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "min_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "max_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "min_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "max_abs_x" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "min_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "max_abs_y" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "min_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "max_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "min_center_distance" ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Region_Abstract_Container_Container( false, *it_spatial_relation_type, "max_center_distance" ) ); 
      }
    }
    return;
  }

  void
  feature_generator_add_region_container_features( const Symbol_Dictionary& symbolDictionary,
                                                  Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region_Container >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region_Container >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Container_Matches_Child( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Container_Matches_Child( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Container_Merge_Container_Spatial_Relation( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Container_Merge_Container_Spatial_Relation( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Container_Merge_Container_Spatial_Relation( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Region_Container_Merge_Container_Spatial_Relation( true, false ) );
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "region_container", "spatial_relation_type", *it_spatial_relation_type ) );
      }
    }
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "region_container", "spatial_relation_type", "spatial_relation", "spatial_relation_type" ) );
    return;
  }

  void 
  feature_generator_add_rule_spatial_relation_features( const Symbol_Dictionary& symbolDictionary,
                                                        Feature_Set& featureSet ){
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_spatial_relation", "spatial_relation_type", *it_spatial_relation_type ) );
      }
    }
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Missing_Child_String_Property( false, "rule_spatial_relation", "spatial_relation_type", "na", "spatial_relation_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Missing_Child_String_Property( true, "rule_spatial_relation", "spatial_relation_type", "na", "spatial_relation_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Missing_Child_String_Property( false, "rule_spatial_relation", "spatial_relation_type", "na", "index" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Missing_Child_String_Property( true, "rule_spatial_relation", "spatial_relation_type", "na", "index" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Spatial_Relation >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Spatial_Relation >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Rule_Spatial_Relation >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Rule_Spatial_Relation >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Ordered_Child< Rule_Spatial_Relation >( false, 0 ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Ordered_Child< Rule_Spatial_Relation >( true, 0 ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Ordered_Child< Rule_Spatial_Relation >( false, 1 ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Ordered_Child< Rule_Spatial_Relation >( true, 1 ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type( true ) );
    return;
  }

  void
  feature_generator_add_rule_object_type_features( const Symbol_Dictionary& symbolDictionary,
                                                    Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Object_Type >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Object_Type >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Object_Type_Merge_Rule_Object_Types( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Object_Type_Merge_Rule_Object_Types( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Object_Type_Merge_Rule_Object_Types( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Object_Type_Merge_Rule_Object_Types( true, true ) );
//    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type( false ) );
//    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Type, Rule_Spatial_Relation >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Type, Rule_Spatial_Relation >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Type, Rule_Spatial_Relation >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Type, Rule_Spatial_Relation >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Type, Rule_Container_Type >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Type, Rule_Container_Type >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Type, Rule_Container_Type >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Type, Rule_Container_Type >( true, true ) );
    map< string, vector< string > >::const_iterator it_object_types = symbolDictionary.string_types().find( "object_type" );
    if( it_object_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_object_type", "object_type", *it_object_type ) );
      }
    }
    return;
  }
    
  void
  feature_generator_add_rule_object_color_features( const Symbol_Dictionary& symbolDictionary,
                                                    Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Object_Color >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Object_Color >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Rule_Object_Color >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Rule_Object_Color >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Object_Color_Merge_Rule_Object_Type_Rule_Spatial_Relation( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Object_Color_Merge_Rule_Object_Type_Rule_Spatial_Relation( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Color, Rule_Spatial_Relation >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Color, Rule_Spatial_Relation >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Color, Rule_Spatial_Relation >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Color, Rule_Spatial_Relation >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Color, Rule_Container_Type >( false, false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Color, Rule_Container_Type >( true, false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Color, Rule_Container_Type >( false, true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Object_Color, Rule_Container_Type >( true, true, true ) );
    map< string, vector< string > >::const_iterator it_object_colors = symbolDictionary.string_types().find( "object_color" );
    if( it_object_colors != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_object_color = it_object_colors->second.begin(); it_object_color != it_object_colors->second.end(); it_object_color++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_object_color", "object_color", *it_object_color ) );
      }
    }
    return;
  }

  void
  feature_generator_add_rule_constraint_type_features( const Symbol_Dictionary& symbolDictionary,
                                                        Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Type >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Type >( true ) );
    map< string, vector< string > >::const_iterator it_constraint_types = symbolDictionary.string_types().find( "constraint_type" );
    if( it_constraint_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_constraint_type = it_constraint_types->second.begin(); it_constraint_type != it_constraint_types->second.end(); it_constraint_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_constraint_type", "constraint_type", *it_constraint_type ) );
      }
    }
    return;
  }

  void
  feature_generator_add_rule_constraint_payload_type_features( const Symbol_Dictionary& symbolDictionary,
                                                                Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Payload_Type >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Payload_Type >( true ) );
    map< string, vector< string > >::const_iterator it_constraint_payload_types = symbolDictionary.string_types().find( "constraint_payload_type" );
    if( it_constraint_payload_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_constraint_payload_type = it_constraint_payload_types->second.begin(); it_constraint_payload_type != it_constraint_payload_types->second.end(); it_constraint_payload_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_constraint_payload_type", "constraint_payload_type", *it_constraint_payload_type ) );
      }
    }
    return;
  }

  void
  feature_generator_add_rule_constraint_reference_type_features( const Symbol_Dictionary& symbolDictionary,
                                                                  Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Reference_Type >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Reference_Type >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "rule_constraint_reference_type", "constraint_reference_type", "rule_object_type", "object_type" ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "rule_constraint_reference_type", "constraint_reference_type", "rule_object_type", "object_type" ) );
    map< string, vector< string > >::const_iterator it_constraint_reference_types = symbolDictionary.string_types().find( "constraint_reference_type" );
    if( it_constraint_reference_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_constraint_reference_type = it_constraint_reference_types->second.begin(); it_constraint_reference_type != it_constraint_reference_types->second.end(); it_constraint_reference_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_constraint_reference_type", "constraint_reference_type", *it_constraint_reference_type ) );
      }
    }
    return;
  } 

  void
  feature_generator_add_rule_container_type_features( const Symbol_Dictionary& symbolDictionary,
                                                                  Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Container_Types( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Container_Types( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Container_Types( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Container_Types( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Object_Types( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Object_Types( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Container_Type >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Container_Type >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Rule_Container_Type >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Rule_Container_Type >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Container_Types( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Container_Types( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Container_Types( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Container_Type_Merge_Rule_Container_Types( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Ordered_Child< Rule_Container_Type >( false, 0 ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Ordered_Child< Rule_Container_Type >( true, 0 ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Ordered_Child< Rule_Container_Type >( false, 1 ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Ordered_Child< Rule_Container_Type >( true, 1 ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Spatial_Relation >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Spatial_Relation >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Spatial_Relation >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Spatial_Relation >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Index >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Index >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Index >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Index >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Number >( false, false ) ); 
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Number >( true, false ) ); 
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Number >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Container_Type, Rule_Number >( true, true ) );
    map< string, vector< string > >::const_iterator it_container_types = symbolDictionary.string_types().find( "container_type" );
    if( it_container_types != symbolDictionary.string_types().end() ){
      for( vector< string >::const_iterator it_container_type = it_container_types->second.begin(); it_container_type != it_container_types->second.end(); it_container_type++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_container_type", "container_type", *it_container_type ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Properties( false, "rule_container_type", "container_type", *it_container_type, vector< string >( 1, "number" ) ) );
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Properties( true, "rule_container_type", "container_type", *it_container_type, vector< string >( 1, "number" ) ) );
      }
    }

    map< string, vector< string > >::const_iterator it_object_types = symbolDictionary.string_types().find( "object_type" );
    map< string, vector< int > >::const_iterator it_numbers = symbolDictionary.int_types().find( "number" );
    if( ( it_object_types != symbolDictionary.string_types().end() ) && ( it_numbers != symbolDictionary.int_types().end() ) ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        for( vector< int >::const_iterator it_number = it_numbers->second.begin(); it_number != it_numbers->second.end(); it_number++ ){
          featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Rule_Container_Type_Equals_World_Objects( false, *it_object_type, *it_number ) );
        }
      }
    }

    return;
  }

  void
  feature_generator_add_rule_index_features( const Symbol_Dictionary& symbolDictionary,
                                             Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Index >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Index >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Spatial_Relation >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Spatial_Relation >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Spatial_Relation >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Spatial_Relation >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Object_Type >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Object_Type >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Object_Type >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Object_Type >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Container_Type >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Container_Type >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Container_Type >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Index, Rule_Container_Type >( true, true ) );

    map< string, vector< int > >::const_iterator it_indices = symbolDictionary.int_types().find( "index" );
    if( it_indices != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_index = it_indices->second.begin(); it_index != it_indices->second.end(); it_index++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "rule_index", "index", *it_index ) );
      }
    }
    return;
  }

  void
  feature_generator_add_rule_number_features( const Symbol_Dictionary& symbolDictionary,
                                              Feature_Set& featureSet ){
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Number >( false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Number >( true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Spatial_Relation >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Spatial_Relation >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Spatial_Relation >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Spatial_Relation >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Object_Type >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Object_Type >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Object_Type >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Object_Type >( true, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Container_Type >( false, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Container_Type >( true, false ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Container_Type >( false, true ) );
    featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Merge_Children< Rule_Number, Rule_Container_Type >( true, true ) );

    map< string, vector< int > >::const_iterator it_numbers = symbolDictionary.int_types().find( "number" );
    if( it_numbers != symbolDictionary.int_types().end() ){
      for( vector< int >::const_iterator it_number = it_numbers->second.begin(); it_number != it_numbers->second.end(); it_number++ ){
        featureSet.feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "rule_number", "number", *it_number ) );
      }
    }
    return;
  }


}

