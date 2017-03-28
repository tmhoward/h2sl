/**
 * @file generate_feature_set_rules.cc
 *
 * @brief
 *
 * a template used to describe a feature that checks the value of a type field
 */

#include "h2sl/spatial_relation.h"
#include "h2sl/constraint.h"
#include "h2sl/object_property.h"
#include "h2sl/rule_object_type.h"
#include "h2sl/rule_object_color.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/rule_constraint_type.h"
#include "h2sl/rule_constraint_payload_type.h"
#include "h2sl/rule_constraint_reference_type.h"
#include "h2sl/rule_number.h"
#include "h2sl/rule_index.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_abstract_container.h"
#include "h2sl/container.h"
#include "h2sl/region_container.h"
#include "h2sl/symbol_dictionary.h"
#include "h2sl/grammar.h"
#include "h2sl/pos.h"
#include "h2sl/feature_set.h"
#include "h2sl/feature_product.h"
#include "h2sl/feature_type.h"
#include "h2sl/feature_cv.h"
#include "h2sl/feature_word.h"
#include "h2sl/feature_phrase_type_and_word.h"
#include "h2sl/feature_phrase_has_pos_tag.h"
#include "h2sl/feature_phrase_type_and_phrase_has_pos_tag.h"
#include "h2sl/feature_num_words.h"
#include "h2sl/feature_phrase_type_and_num_words.h"
#include "h2sl/feature_num_child_phrases.h"
#include "h2sl/feature_phrase_type_and_num_child_phrases.h"
#include "h2sl/feature_grounding_class_name.h"
#include "h2sl/feature_grounding_string_property_value.h"
#include "h2sl/feature_grounding_int_property_value.h"
#include "h2sl/feature_grounding_string_property_value_matches_child_string_property_value.h"
#include "h2sl/feature_matches_child.h"
#include "h2sl/feature_matches_only_child.h"
#include "h2sl/feature_object_merge_abstract_container_spatial_relation.h"
#include "h2sl/feature_object_merge_object_property_container.h"
#include "h2sl/feature_object_merge_object_property_region_container.h"
#include "h2sl/feature_object_matches_sorted_objects.h"
#include "h2sl/feature_object_matches_child_container_object.h"
#include "h2sl/feature_object_matches_child_region_container_object.h"
#include "h2sl/feature_region_object_matches_child.h"
#include "h2sl/feature_region_object_string_property_value.h"
#include "h2sl/feature_region_merge_spatial_relation_and_object.h"
#include "h2sl/feature_region_merge_spatial_relation_and_region.h"
#include "h2sl/feature_container_merge_abstract_container_spatial_relation.h"
#include "h2sl/feature_constraint_payload_string_property_value.h"
#include "h2sl/feature_constraint_reference_matches_child.h"
#include "h2sl/feature_container_is_empty.h"
#include "h2sl/feature_abstract_container_number_equals_world_objects.h"
#include "h2sl/feature_region_container_matches_child.h"
#include "generate_feature_set_rules_cmdline.h"

using namespace std;
using namespace h2sl;

/**
 * generate feature set
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of generate feature set for rule-based symbols program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }  

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary();
  symbol_dictionary->from_xml( args.symbol_dictionary_arg );  

  map< string, vector< Grammar_Terminal > > grammar_terminals;
  map< string, vector< Grammar_Non_Terminal > > grammar_non_terminals;

  Grammar * grammar = new Grammar();
  if( args.grammar_given ){
    grammar->from_xml( args.grammar_arg );
  } else {
    for( unsigned int i = 0; i < args.inputs_num; i++ ){
      World * world = new World( args.inputs[ i ] );
      if( world != NULL ){
        Phrase * phrase = new Phrase( args.inputs[ i ], world );
        if( phrase != NULL ){
          Grammar::scrape_phrases( phrase, grammar_terminals, grammar_non_terminals );
          delete phrase;
          phrase = NULL;
        }
        delete world;
        world = NULL;
      } else {
        return 1;
      }
    }
  } 

  cout << "grammar:(" << *grammar << ")" << endl;

  Feature_Set * feature_set = new Feature_Set();

  for( map< string, vector< Grammar_Terminal > >::const_iterator it_grammar_terminals = grammar_terminals.begin(); it_grammar_terminals != grammar_terminals.end(); it_grammar_terminals++ ){
  cout << "adding feature products for \"" << it_grammar_terminals->first << "\"" << endl;
  cout << "found " << it_grammar_terminals->second.size() << " words" << endl;

  feature_set->feature_products().push_back( new Feature_Product() );
  // cv features
  feature_set->feature_products().back()->feature_groups().push_back( vector< Feature* >() );
  cout << "adding cv features" << endl;
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_CV( false, "false" ) ); 
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_CV( false, "true" ) ); 
  // world features
  feature_set->feature_products().back()->feature_groups().push_back( vector< Feature* >() );
  cout << "adding word features" << endl;
  for( unsigned int i = 0; i < it_grammar_terminals->second.size(); i++ ){
    if( pos_t_from_std_string( it_grammar_terminals->second[ i ].symbol() ) != POS_UNKNOWN ){
      for( unsigned int j = 0; j < it_grammar_terminals->second[ i ].words().size(); j++ ){
        Word word( pos_t_from_std_string( it_grammar_terminals->second[ i ].symbol() ), it_grammar_terminals->second[ i ].words()[ j ] );
        cout << "  adding feature with word " << word << endl;
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Word( false, word, Phrase::phrase_type_t_from_std_string( it_grammar_terminals->first ) ) );
      }
    }
  }
  cout << "done adding words" << endl;
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Phrase_Has_POS_Tag( false, POS_CD, Phrase::phrase_type_t_from_std_string( it_grammar_terminals->first ) ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Phrase_Has_POS_Tag( true, POS_CD, Phrase::phrase_type_t_from_std_string( it_grammar_terminals->first ) ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Phrase_Has_POS_Tag( false, POS_POS, Phrase::phrase_type_t_from_std_string( it_grammar_terminals->first ) ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Phrase_Has_POS_Tag( true, POS_POS, Phrase::phrase_type_t_from_std_string( it_grammar_terminals->first ) ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Num_Words( false, 0, Phrase::phrase_type_t_from_std_string( it_grammar_terminals->first ) ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Num_Words( true, 0, Phrase::phrase_type_t_from_std_string( it_grammar_terminals->first ) ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Num_Child_Phrases( false, 0, Phrase::phrase_type_t_from_std_string( it_grammar_terminals->first ) ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Phrase_Type_And_Num_Child_Phrases( true, 0, Phrase::phrase_type_t_from_std_string( it_grammar_terminals->first ) ) );

  feature_set->feature_products().back()->feature_groups().push_back( vector< Feature* >() );

  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_constraint_type" ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_constraint_payload_type" ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_constraint_reference_type" ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_spatial_relation" ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_object_type" ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_object_color" ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_number" ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "rule_index" ) );

  map< string, vector< string > >::const_iterator it_class_names = symbol_dictionary->class_names().find( it_grammar_terminals->first );
  if( find( it_class_names->second.begin(), it_class_names->second.end(), "object" ) != it_class_names->second.end() ){
    cout << "adding object features" << endl;
    // object symbols
    map< string, vector< string > >::const_iterator it_object_types = symbol_dictionary->string_types().find( "object_type" );
    if( it_object_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object", "object_type", *it_object_type ) ); 
      }
    }
    map< string, vector< string > >::const_iterator it_object_colors = symbol_dictionary->string_types().find( "object_color" );
    if( it_object_colors != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_object_color = it_object_colors->second.begin(); it_object_color != it_object_colors->second.end(); it_object_color++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object", "object_color", *it_object_color ) );
      }
    }
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbol_dictionary->string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_z" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_z" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_abs_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_abs_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_abs_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_abs_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_distance" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_distance" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_center_distance" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_center_distance" ) );
      }
    }
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_x" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_x" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_y" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_y" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_z" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_z" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_abs_x" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_abs_x" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_abs_y" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_abs_y" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_distance" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_distance" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "min_center_distance" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Container( false, "max_center_distance" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "min_x" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "max_x" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "min_y" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "max_y" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "min_z" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "max_z" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "min_abs_x" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "max_abs_x" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "min_abs_y" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "max_abs_y" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "min_distance" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "max_distance" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "min_center_distance" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Merge_Object_Property_Region_Container( false, "max_center_distance" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object >( false ) ); 
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object >( true ) ); 

    map< string, vector< int > >::const_iterator it_numbers = symbol_dictionary->int_types().find( "number" );
    if( it_numbers != symbol_dictionary->int_types().end() ){
      for( vector< int >::const_iterator it_number = it_numbers->second.begin(); it_number != it_numbers->second.end(); it_number++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_z" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_z" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_abs_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_abs_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_abs_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_abs_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_distance" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_distance" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "min_center_distance" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Sorted_Objects( false, *it_number, "max_center_distance" ) );
      }
    }
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Child_Container_Object( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Child_Container_Object( true ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Child_Region_Container_Object( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Object_Matches_Child_Region_Container_Object( true ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "object", "object_type", "object", "object_type" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "object", "object_type", "object", "object_type" ) );
  }
 
  cout << "done generating objects" << endl;
 
  if( find( it_class_names->second.begin(), it_class_names->second.end(), "region" ) != it_class_names->second.end() ){
    cout << "adding region symbols" << endl;  
    // region symbols
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbol_dictionary->string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "region", "spatial_relation_type", *it_spatial_relation_type ) );
      }
    }
    map< string, vector< string > >::const_iterator it_object_types = symbol_dictionary->string_types().find( "object_type" );
    if( it_object_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Region_Object_String_Property_Value( false, "object_type", *it_object_type ) );
      }
    }
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region >( true ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Region_Object_Matches_Child( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Object( true ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Object( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Region( true ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Region( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "region", "spatial_relation_type", "region", "spatial_relation_type" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "region", "spatial_relation_type", "region", "spatial_relation_type" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "region", "object_id", "region", "object_id" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "region", "object_id", "region", "object_id" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "region", "spatial_relation_type", "spatial_relation", "spatial_relation_type" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "region", "spatial_relation_type", "spatial_relation", "spatial_relation_type" ) );
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "constraint" ) != it_class_names->second.end() ){
    cout << "adding constraint symbols" << endl;  
    // constraint symbols
    map< string, vector< string > >::const_iterator it_constraint_types = symbol_dictionary->string_types().find( "constraint_type" );
    if( it_constraint_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_constraint_type = it_constraint_types->second.begin(); it_constraint_type != it_constraint_types->second.end(); it_constraint_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "constraint", "constraint_type", *it_constraint_type ) );
      }
    }
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbol_dictionary->string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "constraint", "reference_relation", *it_spatial_relation_type ) );
      } 
    }
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Constraint >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Constraint >( true ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "constraint", "reference", "object", "name" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "constraint", "reference", "object", "name" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "constraint", "reference_relation", "region", "spatial_relation_type" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "constraint", "reference_relation", "region", "spatial_relation_type" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "constraint", "reference", "region", "object_id" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "constraint", "reference", "region", "object_id" ) );
    map< string, vector< string > >::const_iterator it_constraint_payload_type_types = symbol_dictionary->string_types().find( "constraint_payload_type" );
    for( vector< string >::const_iterator it_constraint_payload_type = it_constraint_payload_type_types->second.begin(); it_constraint_payload_type != it_constraint_payload_type_types->second.end(); it_constraint_payload_type++ ){
      feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Constraint_Payload_String_Property_Value( false, "object_type", *it_constraint_payload_type ) );
    }
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_spatial_relation" ) != it_class_names->second.end() ){
    cout << "adding rule_spatial_relation symbols" << endl;  
    // spatial relation symbols
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbol_dictionary->string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_spatial_relation", "spatial_relation_type", *it_spatial_relation_type ) );
      }
    }
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Spatial_Relation >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Spatial_Relation >( true ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Rule_Spatial_Relation >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Only_Child< Rule_Spatial_Relation >( true ) );
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_object_type" ) != it_class_names->second.end() ){
    cout << "adding rule_object_type symbols" << endl;
    // object_type symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Object_Type >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Object_Type >( true ) );
    map< string, vector< string > >::const_iterator it_object_types = symbol_dictionary->string_types().find( "object_type" );
    if( it_object_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_object_type", "object_type", *it_object_type ) );
      }
    }
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_object_color" ) != it_class_names->second.end() ){
    cout << "adding rule_object_color symbols" << endl;
    // object_color symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Object_Color >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Object_Color >( true ) );
    map< string, vector< string > >::const_iterator it_object_colors = symbol_dictionary->string_types().find( "object_color" );
    if( it_object_colors != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_object_color = it_object_colors->second.begin(); it_object_color != it_object_colors->second.end(); it_object_color++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_object_color", "object_color_type", *it_object_color ) );
      }
    }
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_constraint_type" ) != it_class_names->second.end() ){
    cout << "adding rule_constraint_type symbols" << endl;
    // constraint_type symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Type >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Type >( true ) );
    map< string, vector< string > >::const_iterator it_constraint_types = symbol_dictionary->string_types().find( "constraint_type" );
    if( it_constraint_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_constraint_type = it_constraint_types->second.begin(); it_constraint_type != it_constraint_types->second.end(); it_constraint_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_constraint_type", "constraint_type", *it_constraint_type ) );
      }
    }
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_constraint_payload_type" ) != it_class_names->second.end() ){
    cout << "adding rule_constraint_payload_type symbols" << endl;
    // constraint_payload_type symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Payload_Type >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Payload_Type >( true ) );
    map< string, vector< string > >::const_iterator it_constraint_payload_types = symbol_dictionary->string_types().find( "constraint_payload_type" );
    if( it_constraint_payload_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_constraint_payload_type = it_constraint_payload_types->second.begin(); it_constraint_payload_type != it_constraint_payload_types->second.end(); it_constraint_payload_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_constraint_payload_type", "constraint_payload_type", *it_constraint_payload_type ) ); 
      }
    }
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "rule_constraint_reference_type" ) != it_class_names->second.end() ){
    cout << "adding rule_constraint_reference_type symbols" << endl;
    // constraint_reference_type symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Reference_Type >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Rule_Constraint_Reference_Type >( true ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "rule_constraint_reference_type", "constraint_reference_type", "rule_object_type", "object_type" ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( true, "rule_constraint_reference_type", "constraint_reference_type", "rule_object_type", "object_type" ) );
    map< string, vector< string > >::const_iterator it_constraint_reference_types = symbol_dictionary->string_types().find( "constraint_reference_type" );
    if( it_constraint_reference_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_constraint_reference_type = it_constraint_reference_types->second.begin(); it_constraint_reference_type != it_constraint_reference_types->second.end(); it_constraint_reference_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "rule_constraint_reference_type", "constraint_reference_type", *it_constraint_reference_type ) );
      }
    }
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "object_property" ) != it_class_names->second.end() ){
    cout << "adding object_property symbols" << endl;
    // object_property symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object_Property >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object_Property >( true ) );
    map< string, vector< string > >::const_iterator it_object_types = symbol_dictionary->string_types().find( "object_type" );
    if( it_object_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object_property", "object_type", *it_object_type ) );
      }
    }
    map< string, vector< string > >::const_iterator it_object_colors = symbol_dictionary->string_types().find( "object_color" );
    if( it_object_colors != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_object_color = it_object_colors->second.begin(); it_object_color != it_object_colors->second.end(); it_object_color++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object_property", "object_color", *it_object_color ) );
      }
    }
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "index" ) != it_class_names->second.end() ){
    cout << "adding index symbols" << endl;
    // index symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Index >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Index >( true ) );
    map< string, vector< int > >::const_iterator it_indices = symbol_dictionary->int_types().find( "index" );
    if( it_indices != symbol_dictionary->int_types().end() ){
      for( vector< int >::const_iterator it_index = it_indices->second.begin(); it_index != it_indices->second.end(); it_index++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "index", "value", *it_index ) );
      }
    }
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "number" ) != it_class_names->second.end() ){
    cout << "adding number symbols" << endl;
    // number symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Number >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Number >( true ) );
    map< string, vector< int > >::const_iterator it_numbers = symbol_dictionary->int_types().find( "number" );  
    if( it_numbers != symbol_dictionary->int_types().end() ){
      for( vector< int >::const_iterator it_number = it_numbers->second.begin(); it_number != it_numbers->second.end(); it_number++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "number", "value", *it_number ) ); 
      }
    }
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "abstract_container" ) != it_class_names->second.end() ){
    cout << "adding abstract_container symbols" << endl;
    // abstract_container symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Abstract_Container >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Abstract_Container >( true ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Abstract_Container_Number_Equals_World_Objects( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Abstract_Container_Number_Equals_World_Objects( true ) );
    map< string, vector< string > >::const_iterator it_object_types = symbol_dictionary->string_types().find( "object_type" );
    if( it_object_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "abstract_container", "object_type", *it_object_type ) );
      }
    }
    map< string, vector< int > >::const_iterator it_indices = symbol_dictionary->int_types().find( "index" );
    if( it_indices != symbol_dictionary->int_types().end() ){
      for( vector< int >::const_iterator it_index = it_indices->second.begin(); it_index != it_indices->second.end(); it_index++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "abstract_container", "index", *it_index ) );
      }
    }
    map< string, vector< int > >::const_iterator it_numbers = symbol_dictionary->int_types().find( "number" );
    if( it_numbers != symbol_dictionary->int_types().end() ){
      for( vector< int >::const_iterator it_number = it_numbers->second.begin(); it_number != it_numbers->second.end(); it_number++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Int_Property_Value( false, "abstract_container", "number", *it_number ) );
      }
    }
    map< string, vector< string > >::const_iterator it_object_colors = symbol_dictionary->string_types().find( "object_color" );
    if( it_object_colors != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_object_color = it_object_colors->second.begin(); it_object_color != it_object_colors->second.end(); it_object_color++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "abstract_container", "object_color", *it_object_color ) );
      }
    }
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "region_abstract_container" ) != it_class_names->second.end() ){
    cout << "adding region_abstract_container symbols" << endl;
    // region_abstract_container symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region_Abstract_Container >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region_Abstract_Container >( true ) );
  }

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "container" ) != it_class_names->second.end() ){
    cout << "adding container symbols" << endl;  
    // container symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Container >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Container >( true ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Is_Empty( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Is_Empty( true ) );
    map< string, vector< string > >::const_iterator it_container_types = symbol_dictionary->string_types().find( "container_type" );
    if( it_container_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_container_type = it_container_types->second.begin(); it_container_type != it_container_types->second.end(); it_container_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "container", "container_type", *it_container_type ) );
      }
    }
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbol_dictionary->string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_z" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_z" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_abs_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_abs_x" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_abs_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_abs_y" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_distance" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_distance" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "min_center_distance" ) );
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Container_Merge_Abstract_Container_Spatial_Relation( false, *it_spatial_relation_type, "max_center_distance" ) );
      }
    }
  }  

  if( find( it_class_names->second.begin(), it_class_names->second.end(), "region_container" ) != it_class_names->second.end() ){
    cout << "adding region_container symbols" << endl;  
    // region_container symbols
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region_Container >( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region_Container >( true ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Region_Container_Matches_Child( false ) );
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Region_Container_Matches_Child( true ) );
    map< string, vector< string > >::const_iterator it_spatial_relation_types = symbol_dictionary->string_types().find( "spatial_relation_type" );
    if( it_spatial_relation_types != symbol_dictionary->string_types().end() ){
      for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "region_container", "region_container_type", *it_spatial_relation_type ) );
      } 
    }
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value_Matches_Child_String_Property_Value( false, "region_container", "region_container_type", "spatial_relation", "spatial_relation_type" ) );
  }

  }

  if( args.output_given ){
    feature_set->to_xml( args.output_arg );
  }

  if( feature_set != NULL ){
    delete feature_set;
    feature_set = NULL;
  }

  if( grammar != NULL ){
    delete grammar;
    grammar = NULL;
  }

  if( symbol_dictionary != NULL ){
    delete symbol_dictionary;
    symbol_dictionary = NULL;
  }

  cout << "end of generate feature set for rule-based symbols program" << endl;
  return 0;
} 
