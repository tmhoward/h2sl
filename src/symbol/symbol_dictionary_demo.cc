/**
 * @file    symbol_dictionary_demo.cc
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
 * A Symbol_Dictionary class demo program
 */

#include <iostream>
#include <iomanip>
#include "h2sl/world.h"
#include "h2sl/symbol_dictionary.h"
#include "h2sl/object_property.h"
#include "h2sl/object_type.h"
#include "h2sl/object_color.h"
#include "h2sl/object.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/container.h"
#include "h2sl/region_container.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_abstract_container.h"
#include "symbol_dictionary_demo_cmdline.h"

using namespace std;
using namespace h2sl;

template< class C >
void
print_search_spaces( Symbol_Dictionary& symbolDictionary, World* world, int symbol_threshold ){
  vector< pair< unsigned int, Grounding* > > search_spaces;
  C::fill_search_space( symbolDictionary, world, search_spaces, SYMBOL_TYPE_ALL );
  cout << C::class_name() << " search_spaces.size():" << search_spaces.size() << endl;
  if( search_spaces.size() < symbol_threshold ){
    for( unsigned int i = 0; i < search_spaces.size(); i++ ){
      cout << "  symbol[" << i << "]:(" << *search_spaces[ i ].second << ")" << endl;
    }
  }

  return;
} 

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of Symbol_Dictionary class demo program" << endl;
  
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary();
  if( symbol_dictionary != NULL ){
    if( args.input_given ){
      symbol_dictionary->from_xml( args.input_arg );
    } 

    cout << "symbol_dictionary:(" << *symbol_dictionary << ")" << endl;

    if( args.world_arg != NULL ){
      World * world = new World( args.world_arg );
      print_search_spaces< Object >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Object_Type >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Object_Color >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Object_Property >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Number >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Index >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Region >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Spatial_Relation >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Constraint >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Abstract_Container >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Region_Abstract_Container >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Container >( *symbol_dictionary, world, args.symbol_threshold_arg );
      print_search_spaces< Region_Container >( *symbol_dictionary, world, args.symbol_threshold_arg );
/*
      Object_Type::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );
      Object_Color::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );
      Object_Property::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );
      Number::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );
      Index::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );
      Region::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );
      Spatial_Relation::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );
      Constraint::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );
      Abstract_Container::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );
      Region_Abstract_Container::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );
      Container::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );
      Region_Container::fill_search_space( *symbol_dictionary, world, search_spaces, SYMBOL_TYPE_ALL );

      cout << "search_spaces.size(): " << search_spaces.size() << endl;
*/
      if( world != NULL ){
        delete world;
        world = NULL;
      }
    }

    if( args.output_given ){
      symbol_dictionary->to_xml( args.output_arg );
    }

    delete symbol_dictionary;
    symbol_dictionary = NULL;
  }

  cout << "end of Symbol_Dictionary class demo program" << endl;
  return status;
}
