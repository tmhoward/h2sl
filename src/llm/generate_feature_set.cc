/**
 * @file generate_feature_set.cc
 *
 * @brief
 *
 * a template used to describe a feature that checks the value of a type field
 */

#include "h2sl/spatial_relation.h"
#include "h2sl/constraint.h"
#include "h2sl/symbol_dictionary.h"
#include "h2sl/grammar.h"
#include "h2sl/pos.h"
#include "h2sl/feature_set.h"
#include "h2sl/feature_product.h"
#include "h2sl/feature_type.h"
#include "h2sl/feature_cv.h"
#include "h2sl/feature_word.h"
#include "h2sl/feature_num_words.h"
#include "h2sl/feature_num_child_phrases.h"
#include "h2sl/feature_grounding_class_name.h"
#include "h2sl/feature_grounding_string_property_value.h"
#include "h2sl/feature_grounding_int_property_value.h"
#include "h2sl/feature_grounding_string_property_value_matches_child_string_property_value.h"
#include "h2sl/feature_matches_child.h"
#include "h2sl/feature_region_object_matches_child.h"
#include "h2sl/feature_region_merge_spatial_relation_and_object.h"
#include "h2sl/feature_constraint_payload_string_property_value.h"
#include "h2sl/feature_constraint_reference_matches_child.h"
#include "generate_feature_set_cmdline.h"

using namespace std;
using namespace h2sl;

/**
 * generate feature set
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of generate feature set program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }  

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary();
  symbol_dictionary->from_xml( args.symbol_dictionary_arg );  

  Grammar * grammar = new Grammar();
  if( args.grammar_given ){
    grammar->from_xml( args.grammar_arg );
  } else {
    for( unsigned int i = 0; i < args.inputs_num; i++ ){
      Phrase * phrase = new Phrase();
      if( phrase != NULL ){
        phrase->from_xml( args.inputs[ i ] );
        Grammar::scrape_phrases( phrase, grammar->terminals(), grammar->non_terminals() );
        delete phrase;
        phrase = NULL;
      } else {
        return 1;
      }
    }
  } 

  cout << "grammar:(" << *grammar << ")" << endl;

  Feature_Set * feature_set = new Feature_Set();

  feature_set->feature_products().push_back( new Feature_Product() );
  // cv features
  feature_set->feature_products().back()->feature_groups().push_back( vector< Feature* >() );
  cout << "adding cv features" << endl;
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_CV( false, 1 ) ); 
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_CV( false, 2 ) ); 
  // world features
  feature_set->feature_products().back()->feature_groups().push_back( vector< Feature* >() );
  cout << "adding word features" << endl;
  for( unsigned int i = 0; i < grammar->terminals().size(); i++ ){
    if( pos_t_from_std_string( grammar->terminals()[ i ].symbol() ) != POS_UNKNOWN ){
      for( unsigned int j = 0; j < grammar->terminals()[ i ].words().size(); j++ ){
        Word word( pos_t_from_std_string( grammar->terminals()[ i ].symbol() ), grammar->terminals()[ i ].words()[ j ] );
        cout << "  adding feature with word " << word << endl;
        feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Word( false, word ) );
      }
    }
  }
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Num_Words( false, 0 ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Num_Words( true, 0 ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Num_Child_Phrases( false, 0 ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Num_Child_Phrases( true, 0 ) );

  feature_set->feature_products().back()->feature_groups().push_back( vector< Feature* >() );
  cout << "adding symbol features" << endl;
  // object symbols
  map< string, vector< string > >::const_iterator it_object_types = symbol_dictionary->string_types().find( "object_type" );
  for( vector< string >::const_iterator it_object_type = it_object_types->second.begin(); it_object_type != it_object_types->second.end(); it_object_type++ ){
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object", "object_type", *it_object_type ) ); 
  }
  map< string, vector< string > >::const_iterator it_object_colors = symbol_dictionary->string_types().find( "object_color" );
  for( vector< string >::const_iterator it_object_color = it_object_colors->second.begin(); it_object_color != it_object_colors->second.end(); it_object_color++ ){
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "object", "object_color", *it_object_color ) );
  }
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "object" ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object >( false ) ); 
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Object >( true ) ); 
  
  // region symbols
  map< string, vector< string > >::const_iterator it_spatial_relation_types = symbol_dictionary->string_types().find( "spatial_relation_type" );
  for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "region", "spatial_relation_type", *it_spatial_relation_type ) );
  }
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "region" ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region >( false ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Region >( true ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Region_Object_Matches_Child( false ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Object( true ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Region_Merge_Spatial_Relation_And_Object( false ) );
 
  // constraint symbols
  map< string, vector< string > >::const_iterator it_constraint_types = symbol_dictionary->string_types().find( "constraint_type" );
  for( vector< string >::const_iterator it_constraint_type = it_constraint_types->second.begin(); it_constraint_type != it_constraint_types->second.end(); it_constraint_type++ ){
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "constraint", "constraint_type", *it_constraint_type ) );
  }
  for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "constraint", "reference_relation", *it_spatial_relation_type ) );
  }
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "constraint" ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Constraint >( false ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Constraint >( true ) );
//  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Constraint_Reference_Matches_Child( false ) );
//  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Constraint_Reference_Matches_Child( true ) );
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

  // spatial relation symbols
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_Class_Name( false, "spatial_relation" ) );
  for( vector< string >::const_iterator it_spatial_relation_type = it_spatial_relation_types->second.begin(); it_spatial_relation_type != it_spatial_relation_types->second.end(); it_spatial_relation_type++ ){
    feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Grounding_String_Property_Value( false, "spatial_relation", "spatial_relation_type", *it_spatial_relation_type ) );
  }
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Spatial_Relation >( false ) );
  feature_set->feature_products().back()->feature_groups().back().push_back( new Feature_Matches_Child< Spatial_Relation >( true ) );

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

  cout << "end of generate feature set program" << endl;
  return 0;
} 
