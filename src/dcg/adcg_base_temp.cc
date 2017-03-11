/**
 * @file    adcg_base.cc
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
 * The implementation of a class used to represent a Distributed 
 *   Correspondence Graph
 */

#include <fstream>
#include <utility>
#include <assert.h>
#include <cmath>
#include <bitset>

#include "h2sl/grounding_set.h"

#include "h2sl/object.h"
#include "h2sl/world.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/container.h"
#include "h2sl/index.h"
#include "h2sl/number.h"
#include "h2sl/object_color.h"
#include "h2sl/object_property.h"
#include "h2sl/object_type.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_container.h"
#include "h2sl/region_abstract_container.h"

#include "h2sl/adcg_base.h"

using namespace std;
using namespace h2sl;

ADCG_Base::
ADCG_Base() : _search_spaces(),
        _correspondence_variables(),
        _solutions(),
        _root( NULL ) {

}

ADCG_Base::
~ADCG_Base() {

}

ADCG_Base::
ADCG_Base( const ADCG_Base& other ) : _search_spaces( other._search_spaces ),
                          _correspondence_variables( other._correspondence_variables ),
                          _solutions( other._solutions ),
                          _root( other._root ) {

}

ADCG_Base&
ADCG_Base::
operator=( const ADCG_Base& other ) {
  _search_spaces = other._search_spaces;
  _correspondence_variables = other._correspondence_variables;
  _solutions = other._solutions;
  _root = other._root;
  return (*this);
}

/*
 * fill search spaces
 */
void
ADCG_Base::
fill_search_spaces( const World* world ){
  for( unsigned int i = 0; i < _search_spaces.size(); i++ ){
    if( _search_spaces[ i ].second != NULL ){
      delete _search_spaces[ i ].second;
      _search_spaces[ i ].second = NULL;
    }
    _search_spaces.clear();
  }

  // Correspondence variables.
  for( unsigned int i = 0; i < _correspondence_variables.size(); i++ ){
    _correspondence_variables[ i ].clear();
  }
  _correspondence_variables.clear();

  std::vector< unsigned int > binary_cvs;
  binary_cvs.push_back( CV_FALSE );
  binary_cvs.push_back( CV_TRUE );

  std::vector< unsigned int > ternary_cvs;
  ternary_cvs.push_back( CV_FALSE );
  ternary_cvs.push_back( CV_TRUE );
  ternary_cvs.push_back( CV_INVERTED );

  _correspondence_variables.push_back( binary_cvs );
  _correspondence_variables.push_back( ternary_cvs );

  // ADCG_Base SYMBOLS

  // Constraints
  vector< std::string > constraint;
  constraint.push_back( "inside" );
  constraint.push_back( "outside" );

  // Constraints::payload
  vector< std::string > payload;
  payload.push_back( "generic-robot" ); 

  // AADCG_Base SYMBOLS 

  // Object_Type
  vector< std::string > object_type;
  object_type.push_back( "na" );
  object_type.push_back( "block" );
  object_type.push_back( "table" );
  object_type.push_back( "generic-robot" );
  object_type.push_back( "door" );
  //object_type.push_back( "ycb-can" );
  //object_type.push_back( "ycb-box" );
  //object_type.push_back( "ycb-fruit" );
 
  // Spatial_Relation
  vector< std::string > spatial_relation;
  spatial_relation.push_back( "na" );
  spatial_relation.push_back( "front" );
  spatial_relation.push_back( "back" );
  spatial_relation.push_back( "left" );
  spatial_relation.push_back( "center" );
  spatial_relation.push_back( "right" );
  spatial_relation.push_back( "side" );
  spatial_relation.push_back( "near" );
  spatial_relation.push_back( "far" );
  spatial_relation.push_back( "above" );
  spatial_relation.push_back( "below" );
  
  // Index
  // Note: not starting from "na"
  vector< std::string > index;
  index.push_back( "first" );
  index.push_back( "second" );
  index.push_back( "third" );
  index.push_back( "fourth" );
  index.push_back( "fifth" );
  
  // Number
  // Note: not starting from "na"
  vector< std::string > number;
  number.push_back( "one" );
  number.push_back( "two" );
  number.push_back( "three" );
  number.push_back( "four" );
  number.push_back( "five" );
  number.push_back( "six" );
  number.push_back( "seven" );
  number.push_back( "eight" );
  number.push_back( "nine" );
  number.push_back( "ten" );
  number.push_back( "eleven" );
  number.push_back( "twelve" );
 
  // Object_Color   
  vector< std::string > object_color;
  object_color.push_back( "na" );
  object_color.push_back( "red" );
  object_color.push_back( "blue" );
  object_color.push_back( "yellow" );

  // Region abstract container type.
  vector< std::string > region_abstract_container_type; 
  region_abstract_container.push_back( "front" );
  region_abstract_container.push_back( "back" );
  region_abstract_container.push_back( "left" );
  region_abstract_container.push_back( "center" );
  region_abstract_container.push_back( "right" );
  region_abstract_container.push_back( "side" );
  region_abstract_container.push_back( "near" );
  region_abstract_container.push_back( "far" );
  region_abstract_container.push_back( "above" );
  region_abstract_container.push_back( "below" );
   
  // Abstract symbols need to be initialized seperately in inference 
  vector< std::string > container_type;
  container.push_back( "group" );
  container.push_back( "row" );
  container.push_back( "column" );

  // Map of symbolic representation.
  _symbol_types.insert( pair< string, vector< string > >( string("object_type"), object_type ) );
  _symbol_types.insert( pair< string, vector< string > >( string("object_color"), object_color ) );
  _symbol_types.insert( pair< string, vector< string > >( string("number"), number ) );
  _symbol_types.insert( pair< string, vector< string > >( string("index"), index ) );
  _symbol_types.insert( pair< string, vector< string > >( string("spatial_relation"), spatial_relation ) );
  _symbol_types.insert( pair< string, vector< string > >( string("container"), container ) );
  _symbol_types.insert( pair< string, vector< string > >( string("constraint"), constraint ) );
  _symbol_types.insert( pair< string, vector< string > >( string("region_abstract_container"), region_abstract_container ) );

  // Objects
  for( unsigned int i = 0; i < world->objects().size(); i++ ){
    _search_spaces.push_back( pair< unsigned int, Grounding* >( 0, world->objects()[ i ]->dup() ) );
  }

  // Regions. Do not create an "na" region
  for( unsigned int i = 0; i < spatial_relation.size(); i++ ){
    if( spatial_relation[ i ] != "na" ){
      _search_spaces.push_back( pair< unsigned int, Grounding* >( 0, new Region( spatial_relation[ i ], Object() ) ) );
      for( unsigned int j = 0; j < world->objects().size(); j++ ){
        _search_spaces.push_back( pair< unsigned int, Grounding* >( 0, new Region( spatial_relation[ i ], *world->objects()[ j ] ) ) );
      }   
    }
  }

  // Constraints
  for( unsigned int i = 0; i < constraints.size(); i++ ) {
    for( unsigned int j = 0; j < payload.size(); j++ ) {
      for( unsigned int k = 0; j < spatial_relation.size(); k++ ) {
        for( unsigned int l = 0; l < world->objects().size(); l++ ) {
          for( unsigned int m = 0; m < spatial_relation.size(); m++ ) {
            _search_spaces.push_back( pair< unsigned int, Grounding* >( 0, new Constraint( constraints[ i ], 
  											   payload[ j ], spatial_relation[ k ], 
											   world->objects()[ l ]->name(), spatial_relation[ m ] ) ) );  
          }  
        } 
      }  
    } 
  }
  
  // Object_Type
  for (unsigned int i = 0; i < object_type.size(); i++) {
    _search_spaces.push_back( pair< unsigned int, Grounding* >(0, new Object_Type( object_type[ i ] ) ) );
  }

  // Spatial_Relation
  for (unsigned int i = 0; i < spatial_relation.size(); i++) {
    _search_spaces.push_back( pair< unsigned int, Grounding* >(0, new Spatial_Relation( spatial_relation[ i ] ) ) );
  }

  // Index 
  for (unsigned int i = 0; i < index.size(); i++) {
    _search_spaces.push_back( pair< unsigned int, Grounding* >(0, new Index( index[ i ] ) ) );
  }

  // Number
  for (unsigned int i = 0; i < number.size(); i++) {
    _search_spaces.push_back( pair< unsigned int, Grounding* >(0, new Number( number[ i ] ) ) );
  }
 
  // Object_Color
  for (unsigned int i = 0; i < object_color.size(); i++) {
    _search_spaces.push_back( pair< unsigned int, Grounding* >(0, new Object_Color( object_color[ i ] ) ) );
  }

  // Object_Property
  for( unsigned int i = 0; i < object_type.size(); i++ ){
    for( unsigned int j = 0; j < spatial_relation.size(); j++ ){
      for( unsigned int k = 0; k < index.size(); k++ ){
        _search_spaces.push_back( 
                  pair< unsigned int, Grounding* >( 0, new Object_Property( object_type[ i ] , spatial_relation[ j ], index[ k ] ) ) );
      }
    }
  }

  // Abstract Containers and Region Abstract Containers
  // Note: the index in abstract container is constrainted to be the first.
  // Region abstract containers are constrcuted with abstract containers associated 
  // with a type declared above.
  for( unsigned int i = 0; i < object_type.size() ; i++ ){
    for( unsigned int j = 0; j < number.size(); j++ ){
      for( unsigned int l = 0; l < object_color.size(); l++ ){
        _search_spaces.push_back( 
                 pair< unsigned int, Grounding* >( 0, new Abstract_Container( object_type[ i ], number[ j ], index[ 0 ], object_color[ l ] ) ) );
        for( unsigned int k = 0; k < region_abstract_container_type.size(); k++ ){
          _search_spaces.push_back( 
                   pair< unsigned int, Grounding* >( 0, new Region_Abstract_Container( region_abstract_container_type[ k ], 
								               Abstract_Container( object_type[ i ] , number[ j ], index[ 0 ], object_color[ l ] ) ) ) );
        }
      }
    }
  }

  // Container. 
  // Assign the container type with empty list of object groundings.   
  for( unsigned int i = 0; i < container_type.size(); i++ ){
    //_search_spaces.push_back( pair< unsigned int, Grounding* >( 0, new Container( vector< Object* >(), container_type[ i ] ) ) );
    _search_spaces.push_back( pair< unsigned int, Grounding* >( 0, new Container( vector< Grounding* >(), container_type[ i ] ) ) );
  }

  // Containers. Create the power set of objects. 
  // Create corresponding set of containers. 
  // Extract out the objects matching type from the world model.
  for( unsigned int i = 0 ; i < object_type.size(); i++ ){
    vector< Object* > objects;
    for( unsigned int j = 0; j < world->objects().size(); j++ ){
      if( world->objects()[ j ]->type() == object_type[ i ] ){
        objects.push_back( world->objects()[ j ] );
      }
    }
    // Determine the power set.
    const unsigned int num_sets = pow(2,( objects.size() ) );
    for( unsigned int j = 0; j < num_sets; j++ ){
      //vector< Object* > container_objects;
      vector< Grounding* > container_objects;

      for( unsigned int k = 0; k < objects.size(); k++ ){
        int mask = 1 << k;
        if( mask & j ){
          //container_objects.push_back( objects[ k ] );
          container_objects.push_back( dynamic_cast< Grounding* >( objects[ k ] ) );
        }
      }
      if( container_objects.size() > 1 ){
        // add container to search spaces
        for( unsigned int k = 0; k < container_type.size(); k++ ){
          _search_spaces.push_back( pair< unsigned int, Grounding* >( 0, new Container( container_objects, container_type[ k ] ) ) );
        }
        // add region container. 
        // Note: spatial relation starting from FRONT, not na. Hence 1.
        // Note: the container type is starting from group. Hence 0.
        for( unsigned int k = 1; k < spatial_relation.size(); k++ ){
          for( unsigned int l = 0; l < container_type.size() ; l++ ){
            _search_spaces.push_back( pair< unsigned int, Grounding* >( 0, new Region_Container( spatial_relation[ k ], 
 												 Container( container_objects, container_type[ l ] ) ) ) );
          }
        }
      }
    }
  }

  return;
}


/*
 * leaf search 
 */
bool
ADCG_Base::
leaf_search( const Phrase* phrase,
              const World* world,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  return leaf_search( phrase, world, NULL, llm, beamWidth, debug );
}

/*
 * leaf search with context
 */
 bool
ADCG_Base::
leaf_search( const Phrase* phrase,
              const World* world,
              const Grounding* context,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  for( unsigned int i = 0; i < _solutions.size(); i++ ){
    if( _solutions[ i ].second != NULL ){
      delete _solutions[ i ].second;
      _solutions[ i ].second = NULL;
    }
  }
  _solutions.clear();

  if( phrase != NULL ){
    fill_search_spaces( world );

    if( _root != NULL ){
      delete _root;
      _root = NULL;
    }

    _root = new Factor_Set( phrase->dup() );
    _fill_factors( _root, _root->phrase() );  

    Factor_Set * leaf = NULL;
    _find_leaf( _root, leaf );
    while( leaf != NULL ){
      leaf->search( _search_spaces,
                    _correspondence_variables,
                    _symbol_types,
                    world,
                    context,
                    llm,
                    beamWidth,
                    debug );
      leaf = NULL;
      _find_leaf( _root, leaf );
    }
  
    for( unsigned int i = 0; i < _root->solutions().size(); i++ ){
      _solutions.push_back( pair< double, Phrase* >( _root->solutions()[ i ].pygx, _root->phrase()->dup() ) );

      for( unsigned int j = 0; j < _solutions.back().second->children().size(); j++ ){
        if( _solutions.back().second->children()[ j ] != NULL ){
          delete _solutions.back().second->children()[ j ];
          _solutions.back().second->children()[ j ];
        }
      }
      _solutions.back().second->children().clear();
      if( _solutions.back().second->grounding() != NULL ){
        delete _solutions.back().second->grounding();
        _solutions.back().second->grounding() = NULL;
      }
      _fill_phrase( _root, _root->solutions()[ i ], _solutions.back().second );
    }

    return true;
  } else {
    return false;
  }
}

/*
 * to_latex 
 */

void
ADCG_Base::
to_latex( const string& filename )const{
  ofstream outfile;
  outfile.open( filename.c_str() );
  outfile << "\\begin{tikzpicture}[textnode/.style={anchor=mid,font=\\tiny},nodeknown/.style={circle,draw=black!80,fill=black!10,minimum size=5mm,font=\\tiny,top color=white,bottom color=black!20},nodeunknown/.style={circle,draw=black!80,fill=white,minimum size=5mm,font=\\tiny},factor/.style={rectangle,draw=black!80,fill=black!80,minimum size=2mm,font=\\tiny,text=white},dot/.style={circle,draw=black!80,fill=black!80,minimum size=0.25mm,font=\\tiny}]" << endl;
  outfile << "\\end{tikzpicture}" << endl;
  outfile.close();
  return;
} 

/*
 * Helper function: find leaf 
 */
void
ADCG_Base::
_find_leaf( Factor_Set* node, 
            Factor_Set*& leaf ){ 
  if( node->solutions().empty() ){
    bool all_children_known = true;
    for( unsigned int i = 0; i < node->children().size(); i++ ){
      if( node->children()[ i ]->solutions().empty() ){
        all_children_known = false;
      } 
    } 
    if( all_children_known ){
      leaf = node;
    }
  }

  for( unsigned int i = 0; i < node->children().size(); i++ ){
    _find_leaf( node->children()[ i ], leaf );
  }
  return;
}

/*
 * Helper function: fill phrase 
 */
void
ADCG_Base::
_fill_phrase( Factor_Set* node,
              Factor_Set_Solution& solution,
              Phrase* phrase ){
  phrase->grounding() = new Grounding_Set();
  for( unsigned int i = 0; i < solution.groundings.size(); i++ ){
    dynamic_cast< Grounding_Set* >( phrase->grounding() )->groundings().push_back( solution.groundings[ i ] );
  }
  for( unsigned int i = 0; i < node->children().size(); i++ ){
    phrase->children().push_back( node->children()[ i ]->phrase()->dup() );
    for( unsigned int j = 0; j < phrase->children().back()->children().size(); j++ ){
      if( phrase->children().back()->children()[ j ] != NULL ){
        delete phrase->children().back()->children()[ j ];
        phrase->children().back()->children()[ j ];
      }
    }
    phrase->children().back()->children().clear();
    if( phrase->children().back()->grounding() != NULL ){
      delete phrase->children().back()->grounding();
      phrase->children().back()->grounding() = NULL;
    }

    _fill_phrase( node->children()[ i ],
                  node->children()[ i ]->solutions()[ solution.children[ i ] ],
                  phrase->children().back() );

  }
  return;
}

/*
 * Helper function: fill factors 
 */
void
ADCG_Base::
_fill_factors( Factor_Set* node,
                const Phrase* phrase, 
                const bool& fill ){
  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    node->children().push_back( new Factor_Set( phrase->children()[ i ] ) );
    _fill_factors( node->children().back(), phrase->children()[ i ] );
  } 
  return;
}

/*
 * scrape examples function.
 */ 
void
ADCG_Base::
scrape_examples( const string& filename,
                  const Phrase* phrase,
                  const World* world,
                  const vector< pair< unsigned int, Grounding* > >& searchSpaces,
                  const vector< vector< unsigned int > >& correspondenceVariables,
                  vector< pair< unsigned int, LLM_X > >& examples ){
  const Grounding_Set * grounding_set = dynamic_cast< const Grounding_Set* >( phrase->grounding() );
  for( unsigned int i = 0; i < searchSpaces.size(); i++ ){

    bool add_example = true;
    if( grounding_set->evaluate_cv( searchSpaces[ i ].second ) == h2sl::CV_FALSE ){
      if( dynamic_cast< Container* >( searchSpaces[ i ].second ) ){
//        add_example = ( ( rand() % 2 ) == 0 );
      } else if ( dynamic_cast< Region_Container* >( searchSpaces[ i ].second ) ){
        add_example = ( ( rand() % 5 ) == 0 );
      }
    }

    if( add_example ){
      assert( phrase->grounding() != NULL );
//    cout << "comparing " << *static_cast< const Grounding* >( searchSpaces[ i ].second ) << endl;
      examples.push_back( pair< unsigned int, h2sl::LLM_X >( grounding_set->evaluate_cv( searchSpaces[ i ].second ), h2sl::LLM_X( searchSpaces[ i ].second, phrase, world, correspondenceVariables[ searchSpaces[ i ].first ], vector< h2sl::Feature* >(), filename ) ) );
      for( unsigned int j = 0; j < phrase->children().size(); j++ ){
        examples.back().second.children().push_back( pair< const h2sl::Phrase*, vector< h2sl::Grounding* > >( phrase->children()[ j ], vector< h2sl::Grounding* >() ) );
        Grounding_Set * child_grounding_set = dynamic_cast< Grounding_Set* >( phrase->children()[ j ]->grounding() );
        if( child_grounding_set ){
          for( unsigned int k = 0; k < child_grounding_set->groundings().size(); k++ ){
            examples.back().second.children().back().second.push_back( child_grounding_set->groundings()[ k ] );
          }
        }
      }
    }
  }

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    scrape_examples( filename, dynamic_cast< Phrase* >( phrase->children()[ i ] ), world, searchSpaces, correspondenceVariables, examples );
  }
  return;
}

/** 
 * imports the DCG class from an XML file
 */
bool
ADCG_Base::
from_xml( const string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "adcg_base" ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
      xmlFreeDoc( doc );
      return true;
    } else {
      xmlFreeDoc( doc );
      return false;
    }
  } else {
    return false;
  }
}

/** 
 * imports the class from an XML node pointer
 */
bool
ADCG_Base::
from_xml( xmlNodePtr root ){
  if( root->type == XML_ELEMENT_NODE ){
    return true;
  } else {
    return false;
  }
}

/**
 * exports the class to an XML file
 */
void
ADCG_Base::
to_xml( const string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

/**
 * exports the DCG class to an XML node pointer
 */
void
ADCG_Base::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "adcg_base" ), NULL );
  xmlAddChild( root, node );
  return;
}


namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const ADCG_Base& other ) {
    
    return out;
  }
}
