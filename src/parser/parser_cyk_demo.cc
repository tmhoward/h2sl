/**
 * @file    parser_cyk_demo.cc
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
 * A Parser_CYK class demo program
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include "h2sl/phrase.h"
#include "h2sl/parser_cyk.h"
#include "parser_cyk_demo_cmdline.h"

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Parser_CYK class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  Parser_CYK< Phrase >* parser_cyk = new Parser_CYK< Phrase >();
  Grammar * grammar = new Grammar();
  grammar->from_xml( args.grammar_arg );

  vector< Phrase* > phrases;
  if( parser_cyk->parse( *grammar, args.command_arg, phrases, ( bool )( args.debug_arg ) ) ){
    cout << "successfully parsed \"" << args.command_arg << "\"" << endl;
  } else {
    cout << "failed to parse \"" << args.command_arg << "\"" << endl;
  }

  if( args.output_given ){
    string filename = args.output_arg;
    if( filename.find( ".xml") != string::npos ){
      if( !phrases.empty() ){
        if( phrases.size() == 1 ){
          cout << "writing parsed phrase to " << filename << endl;
          phrases.front()->to_xml( filename );
        } else {
          boost::trim_if( filename, boost::is_any_of( ".xml" ) );
          for( unsigned int i = 0; i < phrases.size(); i++ ){
            stringstream tmp;
            tmp << filename << "_" << setw( 4 ) << setfill( '0' ) << i << ".xml";
            cout << "writing parsed phrase to " << tmp.str() << endl;
            phrases[ i ]->to_xml( tmp.str() );
          }
        }
      } else {
        cout << "cannot write phrase to \"" << args.output_arg << "\", phrases empty" << endl;
      }
    }
  }

  if( grammar != NULL ){
    delete grammar;
    grammar = NULL;
  }
  
  if( parser_cyk != NULL ){
    delete parser_cyk;
    parser_cyk = NULL;
  }

  cout << "end of Parser_CYK class demo program" << endl;
  return status;
}
