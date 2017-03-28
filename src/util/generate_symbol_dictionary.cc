/**
 * @file generate_symbol_dictionary.cc
 *
 * @brief
 *
 * a template used to describe a feature that checks the value of a type field
 */

#include "h2sl/constraint.h"
#include "h2sl/symbol_dictionary.h"
#include "h2sl/grammar.h"
#include "h2sl/pos.h"
#include "generate_symbol_dictionary_cmdline.h"

using namespace std;
using namespace h2sl;

/**
 * generate symbol dictionary
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of generate symbol dictionary program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }  

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary();
  for( unsigned int i = 0; i < NUM_PHRASE_TYPES; i++ ){
    symbol_dictionary->class_names().insert( pair< string, vector< string > >( Phrase::phrase_type_t_to_std_string( ( phrase_type_t )( i ) ), vector< string >() ) );
  }

  for( unsigned int i = 0; i < args.inputs_num; i++ ){
    World * world = new World();
    if( world != NULL ){
      world->from_xml( args.inputs[ i ] );
    }   

    Phrase * phrase = new Phrase();
    if( ( phrase != NULL ) && ( world != NULL ) ){
      phrase->from_xml( args.inputs[ i ], world );
      phrase->scrape_groundings( world, symbol_dictionary->class_names(), symbol_dictionary->string_types(), symbol_dictionary->int_types() );
      delete phrase;
      phrase = NULL;
    } else {
      return 1;
    }

    if( world != NULL ){
      delete world;
      world = NULL;
    }
  } 

  cout << "symbol_dictionary:(" << *symbol_dictionary << ")" << endl;

  if( args.output_given ){
    symbol_dictionary->to_xml( args.output_arg );
  }

  if( symbol_dictionary != NULL ){
    delete symbol_dictionary;
    symbol_dictionary = NULL;
  }

  cout << "end of generate symbol dictionary program" << endl;
  return 0;
} 
