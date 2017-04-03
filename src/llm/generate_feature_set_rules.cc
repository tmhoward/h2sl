/**
 * @file generate_feature_set_groundings.cc
 *
 * @brief
 *
 * a template used to describe a feature that checks the value of a type field
 */

#include "h2sl/feature_generator.h"
#include "generate_feature_set_groundings_cmdline.h"

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
  symbol_dictionary->from_xml( args.sd_arg );  

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

  feature_generator_add_grounding_features( grammar_terminals, grammar_non_terminals, *symbol_dictionary, *feature_set );

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
