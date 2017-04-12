/**
 * @file    generate_rule_examples.cc
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
 * A program that generates rule examples
 */

#include <iostream>
#include <boost/algorithm/string.hpp>

#include "h2sl/common.h"
#include "h2sl/world.h"
#include "h2sl/region.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/phrase.h"
#include "generate_rule_examples_cmdline.h"

using namespace std;
using namespace h2sl;

void
replace_groundings_with_rules( Phrase* phrase, 
                                World* world ){
  Grounding_Set * grounding_set_rules = new Grounding_Set();
  for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
    (*it_grounding)->fill_rules( world, grounding_set_rules );
  }

  phrase->grounding_set() = grounding_set_rules;

  for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
    replace_groundings_with_rules( *it_child, world );
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

  for( unsigned int i = 0; i < args.inputs_num; i++ ){
    cout << "loading \"" << args.inputs[ i ] << "\"" << endl;

    h2sl::World * world = new World();
    world->from_xml( args.inputs[ i ] );

    h2sl::Phrase * phrase = new Phrase();
    phrase->from_xml( args.inputs[ i ], world );

    cout << "loading \"" << args.inputs[ i ] << "\"" << endl;

    string instruction = extract_instruction( args.inputs[ i ] );
    
    cout << "instruction:\"" << instruction << "\"" << endl;    
    
    replace_groundings_with_rules( phrase, world );

    xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
    xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
    xmlDocSetRootElement( doc, root );
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "instruction" ), NULL );
    xmlNewProp( node, ( const xmlChar* )( "text" ), ( const xmlChar* )( instruction.c_str() ) );
    xmlAddChild( root, node ); 
    world->to_xml( doc, root );
    phrase->to_xml( doc, root );
    string old_filename = args.inputs[ i ];
    string new_filename;
    vector< string > tmp_strings;
    boost::split( tmp_strings, old_filename, boost::is_any_of( "/" ) );
    assert( !tmp_strings.empty() );
    tmp_strings.back() = "rule_" + tmp_strings.back();
    stringstream tmp;
    for( unsigned int i = 0; i < tmp_strings.size(); i++ ){
      tmp << tmp_strings[ i ];
      if( i != ( tmp_strings.size() - 1 ) ){
        tmp << "/";
      }
    }
    new_filename = tmp.str();
    cout << "writing example to " << new_filename << endl;
    xmlSaveFormatFileEnc( new_filename.c_str(), doc, "UTF-8", 1 );
    xmlFreeDoc( doc );

    if( phrase != NULL ){
      delete phrase;
      phrase = NULL;
    }
    
    if( world != NULL ){
      delete world;
      world = NULL;
    }
  }

  return 0;
}
