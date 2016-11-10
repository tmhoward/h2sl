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
  cout << "start of ADCG class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }

  World * world = new World();
  world->from_xml( args.world_arg );
  if( world != NULL ){
//    cout << "world:(" << *static_cast< World* >( world ) << ")" << endl;
  }

  Feature_Set * feature_set = new Feature_Set();
  LLM * llm = new LLM( feature_set );
  if( args.llm_given ){
    llm->from_xml( args.llm_arg );
  }

  //int64_t start_time = current_time();

  ADCG* adcg = new ADCG();

  adcg->fill_search_spaces( world );
  if( adcg != NULL ){
//    cout << "adcg:(" << *adcg << ")" << endl;
  }

  if( args.command_given && args.grammar_given ){
    // use the instruction given at the command line
    vector< Phrase* > phrases;
    Parser< Phrase > * parser = new Parser_CYK< Phrase >();
    Grammar * grammar = new Grammar();
    grammar->from_xml( args.grammar_arg );
    cout << "using the instruction in the cmd line, parsing \"" << args.command_arg << "\"" << endl;
    if( parser->parse( *grammar, args.command_arg, phrases ) ){
      for( unsigned int i = 0; i < phrases.size(); i++ ){
        if( phrases[ i ] != NULL ){
          //cout << "phrases[" << i << "]:" << *phrases[ i ] << endl;
          adcg->leaf_search( phrases[ i ], world, llm, args.beam_width_arg, true );


          if( args.output_given ){
            string filename = args.output_arg;
            if( filename.find( ".xml" ) != string::npos ){
              if( phrases.size() == 1 ){
                xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
                xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
                xmlDocSetRootElement( doc, root );
                xmlNodePtr instruction_node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "instruction" ), NULL );
                xmlNewProp( instruction_node, ( const xmlChar* )( "text" ), ( const xmlChar* )( args.command_arg ) );
                xmlAddChild( root, instruction_node );
                world->to_xml( doc, root );
                adcg->solutions().front().second->to_xml( doc, root );
                xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
                xmlFreeDoc( doc );
              } else {
                boost::trim_if( filename, boost::is_any_of( ".xml" ) );
                stringstream tmp;
                tmp << filename << "_" << setw( 4 ) << setfill( '0' ) << i << ".xml";
                xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
                xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
                xmlDocSetRootElement( doc, root );
                xmlNodePtr instruction_node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "instruction" ), NULL );
                xmlNewProp( instruction_node, ( const xmlChar* )( "text" ), ( const xmlChar* )( args.command_arg ) );
                xmlAddChild( root, instruction_node );
                world->to_xml( doc, root );
                adcg->solutions().front().second->to_xml( doc, root );
                xmlSaveFormatFileEnc( tmp.str().c_str(), doc, "UTF-8", 1 );
                xmlFreeDoc( doc );
              }
            }
          }
        }
      }
    }
  } else {
    // get the instruction from the xml file
    cout << "getting instruction from xml file, not parsing. \"" << endl;
    Phrase * phrase = new Phrase();
    phrase->from_xml( args.world_arg );

    cout << "phrase:" << *phrase << endl;
    adcg->leaf_search( phrase, world, llm, args.beam_width_arg, true );
//    cout << "fixed_search_space size:" << adcg->search_spaces().size() << endl;
    if( args.output_given ){
      string filename = args.output_arg;
      if( filename.find( ".xml" ) != string::npos ){
        xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
        xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
        xmlDocSetRootElement( doc, root );
        xmlNodePtr instruction_node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "instruction" ), NULL );
        xmlNewProp( instruction_node, ( const xmlChar* )( "text" ), ( const xmlChar* )( args.command_arg ) );
        xmlAddChild( root, instruction_node );
        world->to_xml( doc, root );
        adcg->solutions().front().second->to_xml( doc, root );
        xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
        xmlFreeDoc( doc );
      }
      delete_ptr< Phrase >( phrase );
    }
  }

//  int64_t end_time = current_time();
//  cout << "ADCG time taken: " << microseconds_to_seconds(end_time-start_time) << " seconds" << endl;

  if( adcg != NULL ){
    delete adcg;
    adcg = NULL;
  }

  cout << "end of ADCG class demo program" << endl;
  return 0;
}
            

