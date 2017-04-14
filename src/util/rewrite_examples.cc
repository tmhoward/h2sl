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
#include "h2sl/object_property.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_abstract_container.h"
#include "h2sl/object_color.h"
#include "h2sl/phrase.h"
#include "rewrite_examples_cmdline.h"

using namespace std;
using namespace h2sl;

void
rewrite_objects( Phrase* phrase, World* world ){
  cout << "function: " << "rewrite_objects" << endl;
  if( phrase->grounding_set() != NULL ){
    cout << "checking " << *( phrase->grounding_set() ) << endl;
    for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
      Object * object = dynamic_cast< Object* >( *it_grounding );
      if( object != NULL ){
        cout << "found object" << endl;
        map< string, Object* >::iterator it_object = world->objects().find( object->id() );
        assert( it_object != world->objects().end() );
        cout << "  reassigning grounding to object " << *( it_object->second ) << endl;
        *object = *(it_object->second);
      }
    }

    for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
      rewrite_objects( *it_child, world );
    }
  } else{
    cout << "phrase->grounding_set() was NULL, printing phrase: " << *( phrase ) << endl;
    assert( false );
  }
  return;
}

void
rewrite_containers( Phrase* phrase, World* world ){
  return;
}

void
replace_empty_regions_with_objects( Phrase* phrase, World* world ){
  cout << "function: " << "replace_empty_regions_with_objects" << endl;
  if( phrase->grounding_set() != NULL ){
    cout << "checking " << *( phrase->grounding_set() ) << endl;
    for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
      Region * region = dynamic_cast< Region* >( *it_grounding );
      if( region != NULL ){
        cout << "found region" << endl;
        if( region->spatial_relation_type() == "na" ){
          cout << "  found empty region" << endl;
          map< string, Object* >::iterator it_object = world->objects().find( region->object_id() );
          assert( it_object != world->objects().end() );
          cout << "  reassigning grounding to object " << *( it_object->second ) << endl;
          *it_grounding = it_object->second;
        }
      }
    }

    for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
      replace_empty_regions_with_objects( *it_child, world );
    }
  } else{
    cout << "phrase->grounding_set() was NULL, printing phrase: " << *( phrase ) << endl;
    assert( false );
  }
  return;
}

void
replace_objectless_regions_with_spatial_relations( Phrase* phrase, World* world ){
  cout << "function: " << "replace_objectless_regions_with_spatial_relations" << endl;
  if( phrase->grounding_set() ){
    cout << "checking " << *( phrase->grounding_set() ) << endl;
    for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
      Region * region = dynamic_cast< Region* >( *it_grounding );
      if( region != NULL ){
        cout << "found region" << endl;
        if( region->object_id() == "na" ){
          cout << "  found objectless region" << endl;
          Spatial_Relation * spatial_relation = new Spatial_Relation( region->spatial_relation_type() );
          cout << "  reassigning grounding to spatial relation " << *( spatial_relation ) << endl;
          *it_grounding = spatial_relation;
        }
      }
    }

    for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
      replace_objectless_regions_with_spatial_relations( *it_child, world );
    }
  } else{
    cout << "phrase->grounding_set() was NULL, printing phrase: " << *( phrase ) << endl;
    assert( false );
  }
  return;
}

void
add_spatial_relation_to_object_property( Phrase* phrase, World* world ){
  cout << "function: " << "add_spatial_relation_to_object_property" << endl;
  if( phrase->grounding_set() ){
    vector< Grounding* > new_groundings;
    cout << "checking " << *( phrase->grounding_set() ) << endl;
    for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
      Object_Property * object_property = dynamic_cast< Object_Property* >( *it_grounding );
      if( object_property != NULL ){
        cout << "found object_property" << endl;
        bool found_spatial_relation = false;
        for( vector< Grounding* >::iterator it_other_grounding = phrase->grounding_set()->groundings().begin(); it_other_grounding != phrase->grounding_set()->groundings().end(); it_other_grounding++ ){
          Spatial_Relation * spatial_relation = dynamic_cast< Spatial_Relation* >( *it_other_grounding );
          if( spatial_relation != NULL ){
            if( spatial_relation->spatial_relation_type() == object_property->relation_type() ){
              found_spatial_relation = true;
            }  
          } 
        }
        if( !found_spatial_relation ){
          new_groundings.push_back( new Spatial_Relation( object_property->relation_type() ) );
        }
      }
    }

    for( vector< Grounding* >::iterator it_new_grounding = new_groundings.begin(); it_new_grounding != new_groundings.end(); it_new_grounding++ ){
      phrase->grounding_set()->groundings().push_back( *it_new_grounding );
    }

    for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
      add_spatial_relation_to_object_property( *it_child, world );
    }
  } else{
    cout << "phrase->grounding_set() was NULL, printing phrase: " << *( phrase ) << endl;
    assert( false );
  }
  return;
}

void
add_object_color_to_abstract_container( Phrase* phrase, World* world ){
  cout << "function: " << "add_object_color_to_abstract_container" << endl;
  if( phrase->grounding_set() ){
    cout << "checking " << *( phrase->grounding_set() ) << endl;
    vector< Grounding* > new_groundings;

    for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
      Abstract_Container * abstract_container = dynamic_cast< Abstract_Container* >( *it_grounding );
      if( abstract_container != NULL ){
        cout << "found abstract_container" << endl;
        bool found_object_color = false;
        for( vector< Grounding* >::iterator it_other_grounding = phrase->grounding_set()->groundings().begin(); it_other_grounding != phrase->grounding_set()->groundings().end(); it_other_grounding++ ){
          cout << "checking " << **( it_other_grounding ) << endl;
          Object_Color * object_color = dynamic_cast< Object_Color* >( *it_other_grounding );
          if( object_color != NULL ){
            if( object_color->object_color_type() == abstract_container->color() ){
              found_object_color = true;
            }
          }
        }
        if( !found_object_color ){
          new_groundings.push_back( new Object_Color( abstract_container->color() ) );
        }
      }
    }

    for( vector< Grounding* >::iterator it_new_grounding = new_groundings.begin(); it_new_grounding != new_groundings.end(); it_new_grounding++ ){
      phrase->grounding_set()->groundings().push_back( *it_new_grounding );
    }

    for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
      add_object_color_to_abstract_container( *it_child, world );
    }
  } else{
    cout << "phrase->grounding_set() was NULL, printing phrase: " << *( phrase ) << endl;
    assert( false );
  }
  return;
}

void
add_object_type_to_abstract_container( Phrase* phrase, World* world ){
  cout << "function: " << "add_object_type_to_abstract_container" << endl;
  if( phrase->grounding_set() ){
    vector< Grounding* > new_groundings;

    cout << "checking " << *( phrase->grounding_set() ) << endl;
    for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
      Abstract_Container * abstract_container = dynamic_cast< Abstract_Container* >( *it_grounding );
      if( abstract_container != NULL ){
        cout << "found abstract_container" << endl;
        bool found_object_type = false;
        for( vector< Grounding* >::iterator it_other_grounding = phrase->grounding_set()->groundings().begin(); it_other_grounding != phrase->grounding_set()->groundings().end(); it_other_grounding++ ){
          Object_Type * object_type = dynamic_cast< Object_Type* >( *it_other_grounding );
          if( object_type != NULL ){
            if( object_type->type() == abstract_container->type() ){
              found_object_type = true;
            }
          }
        }
        if( !found_object_type ){
          new_groundings.push_back( new Object_Type( abstract_container->type() ) );
        }
      }
    }

    for( vector< Grounding* >::iterator it_new_grounding = new_groundings.begin(); it_new_grounding != new_groundings.end(); it_new_grounding++ ){
      phrase->grounding_set()->groundings().push_back( *it_new_grounding );
    }

    for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
      add_object_type_to_abstract_container( *it_child, world );
    }
  } else{
    cout << "phrase->grounding_set() was NULL, printing phrase: " << *( phrase ) << endl;
    assert( false );
  }
  return;
}

void
add_object_color_to_region_abstract_container( Phrase* phrase, World* world ){
  cout << "function: " << "add_object_color_to_region_abstract_container" << endl;
  if( phrase->grounding_set() ){
    vector< Grounding* > new_groundings;

    cout << "checking " << *( phrase->grounding_set() ) << endl;
    for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
      cout << "checking " << **( it_grounding ) << endl;
      Region_Abstract_Container * region_abstract_container = dynamic_cast< Region_Abstract_Container* >( *it_grounding );
      if( region_abstract_container != NULL ){
        cout << "found region_abstract_container" << endl;
        bool found_object_color = false;
        for( vector< Grounding* >::iterator it_other_grounding = phrase->grounding_set()->groundings().begin(); it_other_grounding != phrase->grounding_set()->groundings().end(); it_other_grounding++ ){
          cout << "checking " << **( it_other_grounding ) << endl;
          Object_Color * object_color = dynamic_cast< Object_Color* >( *it_other_grounding );
          if( object_color != NULL ){
            cout << "found object_color" << endl;
            if( object_color->object_color_type() == region_abstract_container->color() ){
              found_object_color = true;
            }
          }
        }
        if( !found_object_color ){
          cout << "adding object_color" << endl;
          new_groundings.push_back( new Object_Color( region_abstract_container->color() ) );
          cout << "done adding object_color" << endl;
        }
      }
    }

    for( vector< Grounding* >::iterator it_new_grounding = new_groundings.begin(); it_new_grounding != new_groundings.end(); it_new_grounding++ ){
      phrase->grounding_set()->groundings().push_back( *it_new_grounding );
    }

    for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
      cout << "adding object_color_to_region_abstract_container " << **( it_child ) << endl;
      add_object_color_to_region_abstract_container( *it_child, world );
    }
  } else{
    cout << "phrase->grounding_set() was NULL, printing phrase: " << *( phrase ) << endl;
    assert( false );
  }
  return;
}

void
add_object_type_to_region_abstract_container( Phrase* phrase, World* world ){
  cout << "function: " << "add_object_type_to_abstract_container" << endl;
  if( phrase->grounding_set() ){
    vector< Grounding* > new_groundings;
  
    cout << "checking " << *( phrase->grounding_set() ) << endl;
    for( vector< Grounding* >::iterator it_grounding = phrase->grounding_set()->groundings().begin(); it_grounding != phrase->grounding_set()->groundings().end(); it_grounding++ ){
      Region_Abstract_Container * region_abstract_container = dynamic_cast< Region_Abstract_Container* >( *it_grounding );
      if( region_abstract_container != NULL ){
        cout << "found region_abstract_container" << endl;
        bool found_object_type = false;
        for( vector< Grounding* >::iterator it_other_grounding = phrase->grounding_set()->groundings().begin(); it_other_grounding != phrase->grounding_set()->groundings().end(); it_other_grounding++ ){
          Object_Type * object_type = dynamic_cast< Object_Type* >( *it_other_grounding );
          if( object_type != NULL ){
            if( object_type->type() == region_abstract_container->type() ){
              found_object_type = true;
            }
          }
        }
        if( !found_object_type ){
          new_groundings.push_back( new Object_Type( region_abstract_container->type() ) );
        }
      }
    }

    for( vector< Grounding* >::iterator it_new_grounding = new_groundings.begin(); it_new_grounding != new_groundings.end(); it_new_grounding++ ){
      phrase->grounding_set()->groundings().push_back( *it_new_grounding );
    }

    for( vector< Phrase* >::iterator it_child = phrase->children().begin(); it_child != phrase->children().end(); it_child++ ){
      add_object_type_to_region_abstract_container( *it_child, world );
    }
  } else{
    cout << "phrase->grounding_set() was NULL, printing phrase: " << *( phrase ) << endl;
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
    phrase->from_xml( args.inputs[ i ], world );

    replace_empty_regions_with_objects( phrase, world );
    replace_objectless_regions_with_spatial_relations( phrase, world );
    add_spatial_relation_to_object_property( phrase, world );
    add_object_color_to_abstract_container( phrase, world );
    add_object_type_to_abstract_container( phrase, world );
    add_object_color_to_region_abstract_container( phrase, world );
    add_object_type_to_region_abstract_container( phrase, world );

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
