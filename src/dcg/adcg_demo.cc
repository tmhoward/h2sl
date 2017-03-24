/**
 * @file adcg_demo.cc
 *
 * @brief
 *
 * a class to represent a adcg
 */


#include "h2sl/grammar.h"
#include "h2sl/parser_cyk.h"

#include "h2sl/feature_set.h"
#include "h2sl/world.h"
#include "h2sl/phrase.h"
#include "h2sl/adcg.h"
#include "adcg_demo_cmdline.h"
#include "h2sl/common.h"


using namespace std;
using namespace h2sl;

/**
 * ADCG class demo
 */
int
main( int argc,
      char* argv[] ){
  int status = 0;
  cout << "start of ADCG class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }

  Parser< Phrase > * parser = new Parser_CYK< Phrase >();
  Grammar * grammar = new Grammar();
  grammar->from_xml( args.grammar_arg );

  vector< Phrase* > phrases;

  Grounding * context = NULL;

  World * world = new World();
  if( args.world_given ){
    world->from_xml( args.world_arg );
  }

  Feature_Set * feature_set = new Feature_Set();
  LLM * llm = new LLM( feature_set );
  if( args.llm_given ){
    llm->from_xml( args.llm_arg );
  }

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary( args.symbol_dictionary_arg );
 
  Search_Space * search_space = new Search_Space();

  ADCG * adcg = new ADCG();
 
  struct timeval start_time;
  gettimeofday( &start_time, NULL );
 
  search_space->fill_groundings( *symbol_dictionary, world );

  struct timeval end_time;
  gettimeofday( &end_time, NULL );

  cout << "finished fill_seach_space for adcg in " << diff_time( start_time, end_time ) << " seconds" << endl;

  cout << endl << "search_spaces.size(): " << search_space->grounding_pairs().size() << endl << endl;

  cout << "parsing \"" << args.command_arg << "\"" << endl;
  if( parser->parse( *grammar, args.command_arg, phrases ) ){
    for( unsigned int i = 0; i < phrases.size(); i++ ){
      if( phrases[ i ] != NULL ){
        cout << "phrases[" << i << "]:" << *phrases[ i ] << endl;

        gettimeofday( &start_time, NULL );

        adcg->leaf_search( phrases[ i ], *symbol_dictionary, search_space, world, context, llm, args.beam_width_arg, ( bool )( args.debug_arg ) );

        gettimeofday( &end_time, NULL );

        cout << "finished the adcg search in " << diff_time( start_time, end_time ) << " seconds" << endl;
        for( unsigned int j = 0; j < adcg->solutions().size(); j++ ){
          cout << "  solutions[" << j << "]:" << *adcg->solutions()[ j ].second << " (" << adcg->solutions()[ j ].first << ")" << endl;
        }

        if( !adcg->solutions().empty() ){
          if( args.output_given ){
            string filename = args.output_arg;
            if( filename.find( ".xml" ) != string::npos ){
              if( phrases.size() == 1 ){
                adcg->solutions().front().second->to_xml( filename );
              } else {
                boost::trim_if( filename, boost::is_any_of( ".xml" ) );
                stringstream tmp;
                tmp << filename << "_" << setw( 4 ) << setfill( '0' ) << i << ".xml";
                adcg->solutions().front().second->to_xml( tmp.str() );
              }
            }
          }

          if( args.latex_output_given ){
            string filename = args.latex_output_arg;
            if( filename.find( ".tex" ) != string::npos ){
              if( phrases.size() == 1 ){
                adcg->to_latex( filename );
              } else {
                boost::trim_if( filename, boost::is_any_of( ".tex" ) );
                stringstream tmp;
                tmp << filename << "_" << setw( 4 ) << setfill( '0' ) << i << ".tex";
                adcg->to_latex( tmp.str() );
              }
            }
          }
        }
      cout << "after adcg inference printing out the phrase:" << endl;
      cout << *phrases[ i ] << endl;
      }
    }
  }

  if( adcg != NULL ){
    delete adcg;
    adcg = NULL;
  }

  if( llm != NULL ){
    delete llm;
    llm = NULL;
  }

  if( feature_set != NULL ){
    delete feature_set;
    feature_set = NULL;
  }

  if( world != NULL ){
    delete world;
    world = NULL;
  }

  for( unsigned int i = 0; i < phrases.size(); i++ ){
    if( phrases[ i ] != NULL ){
      delete phrases[ i ];
      phrases[ i ] = NULL;
    }
  }

  if( grammar != NULL ){
    delete grammar;
    grammar = NULL;
  }

  if( parser != NULL ){
    delete parser;
    parser = NULL;
  }

  cout << "end of ADCG class demo program" << endl;
  return status;
}
            

