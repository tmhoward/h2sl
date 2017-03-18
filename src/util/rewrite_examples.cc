/**
 * @file    rewrite_examples.cc
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
#include "h2sl/region.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/phrase.h"
#include "rewrite_examples_cmdline.h"

using namespace std;
using namespace h2sl;

void
replace_empty_regions_with_objects( Phrase* phrase, World* world ){
  cout << "function: " << "replace_empty_regions_with_objects" << endl;
  if( phrase->grounding_set() != NULL ){
    cout << "checking " << *phrase->grounding_set() << endl;
    for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
      Region * region = dynamic_cast< Region* >( *it_grounding );
      if( region != NULL ){
        cout << "found region" << endl;
        if( region->spatial_relation_type() == "na" ){
          cout << "  found empty region" << endl;
          map< string, Object* >::iterator it_object = world->objects().find( region->object_id() );
          assert( it_object != world->objects().end() );
          cout << "  reassigning grounding to object " << *it_object->second << endl;
          *it_grounding = it_object->second;
        }
      }
    }

    for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
      replace_empty_regions_with_objects( *it_child, world );
    }
  } else{
    cout << "phrase->grounding_set() was NULL, printing phrase: " << *phrase << endl;
    assert( false );
  }
  return;
}

void
replace_objectless_regions_with_spatial_relations( Phrase* phrase, World* world ){
  cout << "function: " << "replace_objectless_regions_with_spatial_relations" << endl;
  if( phrase->grounding_set() ){
    cout << "checking " << *phrase->grounding_set() << endl;
    for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
      Region * region = dynamic_cast< Region* >( *it_grounding );
      if( region != NULL ){
        cout << "found region" << endl;
        if( region->object_id() == "na" ){
          cout << "  found objectless region" << endl;
          Spatial_Relation * spatial_relation = new Spatial_Relation( region->spatial_relation_type() );
          cout << "  reassigning grounding to spatial relation " << *spatial_relation << endl;
          *it_grounding = spatial_relation;
        }
      }
    }

    for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
      replace_objectless_regions_with_spatial_relations( *it_child, world );
    }
  } else{
    cout << "phrase->grounding_set() was NULL, printing phrase: " << *phrase << endl;
    assert( false );
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
    phrase->from_xml( args.inputs[ i ] );

    replace_empty_regions_with_objects( phrase, world );
    replace_objectless_regions_with_spatial_relations( phrase, world );

    string instruction = extract_instruction( args.inputs[ i ] );
    
    cout << "instruction:\"" << instruction << "\"" << endl;    

    xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
    xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
    xmlDocSetRootElement( doc, root );
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "instruction" ), NULL );
    xmlNewProp( node, ( const xmlChar* )( "text" ), ( const xmlChar* )( instruction.c_str() ) );
    xmlAddChild( root, node ); 
    world->to_xml( doc, root );
    phrase->to_xml( doc, root );
    cout << "writing example to " << args.inputs[ i ] << endl;
    xmlSaveFormatFileEnc( args.inputs[ i ], doc, "UTF-8", 1 );
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
