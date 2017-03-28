/**
 * @file    gui_demo.cc
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
 * A GUI class demo program
 */

#include <iostream>

#include <QtGui/QApplication>

#include "h2sl/parser_cyk.h"
#include "h2sl/gui.h"
#include "gui_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }
  
  cout << "start of GUI class demo program" << endl;

  QApplication app( argc, argv );

  Parser< Phrase > * parser = new Parser_CYK< Phrase >();
  Grammar * grammar = new Grammar();
  grammar->from_xml( args.grammar_arg );

  Grounding * context = NULL;

  World * world = new World();
  world->from_xml( args.world_arg );

  Feature_Set * feature_set = new Feature_Set();

  LLM * llm = new LLM( feature_set );
  if( args.llm_given ){
    llm->from_xml( args.llm_arg );
  }

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary( args.symbol_dictionary_groundings_arg );

  Search_Space * search_space = new Search_Space();
  search_space->fill_groundings( *symbol_dictionary, world );

  DCG * dcg = new DCG();

  GUI gui( grammar, parser, symbol_dictionary, search_space, world, context, llm, dcg, args.beam_width_arg );

  if( args.phrase_given ){
    Phrase * phrase = new Phrase();
    phrase->from_xml( args.phrase_arg, world );
  
    if( phrase != NULL ){
      delete phrase;
      phrase = NULL;
    }
  }

  gui.show(); 

  return app.exec();
}
