/**
 * @file    generate_grammar.cc
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
 * A grammar generation program
 */

#include <iostream>

#include "h2sl/grammar.h"
#include "grammar_generator_cmdline.h"

using namespace std;
using namespace h2sl;

void
scrape_phrases( const Phrase* phrase,
                vector< Grammar_Terminal >& terminals,
                vector< Grammar_Non_Terminal >& nonTerminals ){
  // check for unique terminals
  for( unsigned int i = 0; i < phrase->words().size(); i++ ){
    for( unsigned int j = 0; j < terminals.size(); j++ ){
      if( pos_t_to_std_string( phrase->words()[ i ].pos() ) == terminals[ j ].symbol() ){
        bool found_match = false;
        for( unsigned int k = 0; k < terminals[ j ].words().size(); k++ ){
          if( phrase->words()[ i ].text() == terminals[ j ].words()[ k ] ){
            found_match = true;
          }
        }
        if( !found_match ){
          terminals[ j ].words().push_back( phrase->words()[ i ].text() );
          cout << "adding word " << phrase->words()[ i ] << endl;
        }
        break;
      }
    }
  }

  // check for unique non-terminals 
  Grammar_Non_Terminal grammar_non_terminal( Phrase::phrase_type_t_to_std_string( phrase->type() ) );
  cout << "phrase:" << phrase->words_to_std_string() << endl;
  // add words  
  cout << "phrase->words().size():" << phrase->words().size() << endl;
  for( unsigned int i = 0; i < phrase->words().size(); i++ ){
//    grammar_non_terminal.elements().push_back( pos_t_to_std_string( phrase->words()[ i ].pos() ) + "[" + phrase->words()[ i ].text() + "]" );
    grammar_non_terminal.elements().push_back( pos_t_to_std_string( phrase->words()[ i ].pos() ) );
  }
  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    grammar_non_terminal.elements().push_back( Phrase::phrase_type_t_to_std_string( phrase->children()[ i ]->type() ) );
  }
  cout << "grammar_non_terminal:(" << grammar_non_terminal << ")" << endl;
  bool found_match = false;
  for( unsigned int i = 0; i < nonTerminals.size(); i++ ){
    if( grammar_non_terminal == nonTerminals[ i ] ){
      found_match = true;
    }
  }
  if( !found_match ){
    nonTerminals.push_back( grammar_non_terminal );
    cout << "adding non_terminal " << grammar_non_terminal << endl;
  }

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    scrape_phrases( phrase->children()[ i ], terminals, nonTerminals );
  }
  return;
}

int
main( int argc,
      char* argv[] ) {
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  vector< Phrase* > phrases( args.inputs_num, NULL );
  Grammar * grammar = new Grammar();

  if( grammar != NULL ){
    for( unsigned int i = 0; i < args.inputs_num; i++ ){
      World * world = new World( args.inputs[ i ] );
      if( world != NULL ){
        Phrase * phrase = new Phrase( args.inputs[ i ], world );
        if( phrase != NULL ){
          Grammar::scrape_phrases( phrase, grammar->terminals(), grammar->non_terminals() );
          delete phrase;
          phrase = NULL;
        }
        delete world;
        world = NULL;
      } else {
        return 1;
      }
    }

    cout << "grammar terminals[" << grammar->terminals().size() << "{" << endl;
    for( unsigned int i = 0; i < grammar->terminals().size(); i++ ){
      cout << "  " << grammar->terminals()[ i ] << endl;
    }   
    cout << "}" << endl;

    cout << "grammar non_terminals[" << grammar->non_terminals().size() << "{" << endl;
    for( unsigned int i = 0; i < grammar->non_terminals().size(); i++ ){
      cout << "  " << grammar->non_terminals()[ i ] << endl;
    }  
    cout << "}" << endl;

    if( args.output_given ){
      grammar->to_xml( args.output_arg );
    }
    
    delete grammar;
    grammar = NULL;
    return 0;
  } else {
    return 1;
  }
}
