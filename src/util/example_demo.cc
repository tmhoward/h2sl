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
 * A program that loads and rewrites the examples
 */

#include <iostream>

#include "h2sl/common.h"
#include "h2sl/world.h"
#include "h2sl/phrase.h"
#include "example_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  cout << "loading \"" << args.input_arg << "\"" << endl;

  h2sl::World * world = new World();
  world->from_xml( args.input_arg );

  h2sl::Phrase * phrase = new Phrase();
  phrase->from_xml( args.input_arg, world );

  string instruction = extract_instruction( args.input_arg );
    
  cout << "instruction:\"" << instruction << "\"" << endl;    

  if( args.output_given ){

    xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
    xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
    xmlDocSetRootElement( doc, root );
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "instruction" ), NULL );
    xmlNewProp( node, ( const xmlChar* )( "text" ), ( const xmlChar* )( instruction.c_str() ) );
    xmlAddChild( root, node ); 
    world->to_xml( doc, root );
    phrase->to_xml( doc, root );
    cout << "writing example to " << args.output_arg << endl;
    xmlSaveFormatFileEnc( args.output_arg, doc, "UTF-8", 1 );
    xmlFreeDoc( doc );
  }

  if( phrase != NULL ){
    delete phrase;
    phrase = NULL;
  }
    
  if( world != NULL ){
    delete world;
    world = NULL;
  }

  return 0;
}
