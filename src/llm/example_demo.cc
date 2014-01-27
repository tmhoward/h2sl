/**
 * @file    example_demo.cc
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
 * A LLM class demo program
 */

#include <iostream>

#include "h2sl/world.h"
#include "h2sl/phrase.h"
#include "h2sl/parser.h"
#include "example_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  cout << "start of LLM class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  World * world = new World();
  world->from_xml( args.world_arg );

  Parser * parser = new Parser(); 
  parser->grammar().from_xml( args.grammar_arg );

  Phrase * phrase = new Phrase();
  if( parser->parse( args.command_arg, *phrase ) ){
    cout << "successfully parsed " << args.command_arg << endl;
  } else {
    cout << "failed to parse " << args.command_arg << endl;
  }

  cout << "writing output to " << args.output_arg << endl;
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  world->to_xml( doc, root );
  phrase->to_xml( doc, root );
  xmlSaveFormatFileEnc( args.output_arg, doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
 
  if( phrase != NULL ){
    delete phrase;
    phrase = NULL;
  }

  if( parser != NULL ){
    delete parser;
    parser = NULL;
  }
  
  if( world != NULL ){
    delete world;
    world = NULL;
  }

  cout << "end of LLM class demo program" << endl;
  return 0;
}
