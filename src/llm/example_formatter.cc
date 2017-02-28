/**
 * @file    example_formatter.cc
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
 * A example formatter program
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "h2sl/world.h"
#include "h2sl/phrase.h"
#include "h2sl/parser_cyk.h"
#include "example_formatter_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  cout << "start of example formatter program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  World * world = new World();
  world->from_xml( args.world_arg );

  Parser< Phrase > * parser = new Parser_CYK< Phrase >(); 
  Grammar * grammar = new Grammar();
  grammar->from_xml( args.grammar_arg );

  vector< Phrase* > phrases;

  if( parser->parse( *grammar, args.command_arg, phrases ) ){
    cout << "successfully parsed " << args.command_arg << endl;
    string filename = args.output_arg;
    if( filename.find( ".xml" ) != string::npos ){
      if( phrases.size() == 1 ){
        xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
        xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
        xmlDocSetRootElement( doc, root );
        world->to_xml( doc, root );
        phrases[ 0 ]->to_xml( doc, root );
        cout << "writing example to " << filename << endl;
        xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
        xmlFreeDoc( doc );
      } else {
        boost::trim_if( filename, boost::is_any_of( ".xml" ) );
        for( unsigned int i = 0; i < phrases.size(); i++ ){
          if( phrases[ i ] != NULL ){
            stringstream tmp;
            tmp << filename << "_" << setw( 4 ) << setfill( '0' ) << i << ".xml";
            xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
            xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
            xmlDocSetRootElement( doc, root );
            world->to_xml( doc, root );
            phrases[ i ]->to_xml( doc, root );
            cout << "writing example " << i << " to " << tmp.str() << endl;
            xmlSaveFormatFileEnc( tmp.str().c_str(), doc, "UTF-8", 1 );
            xmlFreeDoc( doc );
          }
        }
      }
    }
  } else {
    cout << "failed to parse " << args.command_arg << endl;
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
  
  if( world != NULL ){
    delete world;
    world = NULL;
  }

  cout << "end of example formatter program" << endl;
  return 0;
}
