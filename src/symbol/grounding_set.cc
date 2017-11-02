/**
 * @file    grounding_set.cc
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
 * The implementation of a class used to describe a set of groundings
 */

#include <assert.h>

#include "h2sl/cv.h"

#include "h2sl/rule_object_type.h"
#include "h2sl/rule_container_type.h"
#include "h2sl/rule_object_color.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/rule_constraint_type.h"
#include "h2sl/rule_constraint_payload_type.h"
#include "h2sl/rule_constraint_reference_type.h"
#include "h2sl/rule_index.h"
#include "h2sl/rule_number.h"
#include "h2sl/object.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/object_property.h"
#include "h2sl/object_type.h"
#include "h2sl/container_type.h"
#include "h2sl/object_color.h"
#include "h2sl/index.h"
#include "h2sl/number.h"
#include "h2sl/container.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_abstract_container.h"
#include "h2sl/region_container.h"
#include "h2sl/spatial_relation.h"
//#include "h2sl/action.h"

#include "h2sl/grounding_set.h"

using namespace std;
using namespace h2sl;

Grounding_Set::
Grounding_Set( const vector< Grounding* >& groundings ) : Grounding(),
                                                _groundings( groundings ) {

}

Grounding_Set::
Grounding_Set( xmlNodePtr root, World* world ) : Grounding(),
                                    _groundings() {
  from_xml( root, world );
}

Grounding_Set::
~Grounding_Set() {
  clear();
}

Grounding_Set::
Grounding_Set( const Grounding_Set& other ) : Grounding( other ),
                                        _groundings(){
  _groundings.resize( other._groundings.size(), NULL );
  for( unsigned int i = 0; i < other._groundings.size(); i++ ){
    _groundings[ i ] = other._groundings[ i ]->dup();
  }   
}

Grounding_Set&
Grounding_Set::
operator=( const Grounding_Set& other ) {
  clear();
  _groundings.resize( other._groundings.size(), NULL );
  for( unsigned int i = 0; i < other._groundings.size(); i++ ){
    _groundings[ i ] = other._groundings[ i ];
  }
  return (*this);
}

bool
Grounding_Set::
operator==( const Grounding_Set& other )const{
  for( unsigned int i = 0; i < _groundings.size(); i++ ){
    assert( dynamic_cast< Grounding* >( _groundings[ i ] ) != NULL );
  }
  for( unsigned int i = 0; i < other.groundings().size(); i++ ){
    assert( dynamic_cast< Grounding* >( other.groundings()[ i ] ) != NULL );
  }

  if( _groundings.size() != other.groundings().size() ){
    return false;
  }

  for( unsigned int i = 0; i < _groundings.size(); i++ ){
    bool found_match = false;
    for( unsigned int j = 0; j < other.groundings().size(); j++ ){
      if( *static_cast< const Grounding* >( _groundings[ i ] ) == *static_cast< const Grounding* >( other.groundings()[ j ] ) ){
        found_match = true;
      }
    }
    if( !found_match ){
      return false;
    }
  }
  return true;
}

bool
Grounding_Set::
operator!=( const Grounding_Set& other )const{
  return !( *this == other );
}

Grounding_Set*
Grounding_Set::
dup( void )const{
  return new Grounding_Set( *this );
}

void
Grounding_Set::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  for( vector< Grounding* >::const_iterator it_grounding = _groundings.begin(); it_grounding != _groundings.end(); it_grounding++ ){
    if( ( *it_grounding ) != NULL ){
      (*it_grounding)->scrape_grounding( world, stringTypes, intTypes );
    }
  }
  return;
}

void
Grounding_Set::
scrape_grounding( const World * world,
                  vector< string >& classNames, 
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  for( vector< Grounding* >::const_iterator it_grounding = _groundings.begin(); it_grounding != _groundings.end(); it_grounding++ ){
    if( ( *it_grounding ) != NULL ){
      (*it_grounding)->scrape_grounding( world, classNames, stringTypes, intTypes );
    }
  }
  return;
}

void
Grounding_Set::
clear( void ){
  _groundings.clear();
  return;
}

string
Grounding_Set::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Grounding_Set* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Grounding_Set* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

string
Grounding_Set::
evaluate_cv( const Grounding* grounding )const{
  return grounding->evaluate_cv( this );
}

bool
Grounding_Set::
contains_symbol_in_symbol_dictionary( const Symbol_Dictionary& symbolDictionary )const{
  for( vector< Grounding* >::const_iterator it_grounding = _groundings.begin(); it_grounding != _groundings.end(); it_grounding++ ){
    if( ( *it_grounding ) != NULL ){
      for( map< string, vector< string > >::const_iterator it_class_names = symbolDictionary.class_names().begin(); it_class_names != symbolDictionary.class_names().end(); it_class_names++ ){
        for( vector< string >::const_iterator it_class_name = it_class_names->second.begin(); it_class_name != it_class_names->second.end(); it_class_name++ ){
          if( (*it_grounding)->matches_class_name( *it_class_name ) ){
            return true;
          }
        }
      }
    }
  }
  return false;
}

void
Grounding_Set::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  for( vector< Grounding* >::const_iterator it_grounding = _groundings.begin(); it_grounding != _groundings.end(); it_grounding++ ){
    if( ( *it_grounding ) != NULL ){
      ( *it_grounding )->fill_rules( world, groundingSet );
    }
  }
  return;
}

bool
Grounding_Set::
equals( const Grounding& other )const{
  if( dynamic_cast< const Grounding_Set* >( &other ) != NULL ){
    return ( *this == *static_cast< const Grounding_Set* >( &other ) );
  } else {
    return false;
  }
}

void
Grounding_Set::
to_xml( const string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

void
Grounding_Set::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "grounding_set" ), NULL );
  for( unsigned int i = 0; i < _groundings.size(); i++ ){
    if( _groundings[ i ] != NULL ){
      _groundings[ i ]->to_xml( doc, node );
    }
  }
  xmlAddChild( root, node );
  return;
}

string 
Grounding_Set::
to_json_string( void )const {

return "";
}


string
Grounding_Set::
to_latex( void )const{
  stringstream tmp;
  return tmp.str();
}

void
Grounding_Set::
from_xml( const string& filename, World* world ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "grounding_set" ) ) == 0 ){
            from_xml( l1, world );
          }
        }
      }
    }
    xmlFreeDoc( doc );
  }
  return;
}

void
Grounding_Set::
from_xml( xmlNodePtr root, World* world ){
  clear();
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_object_type" ) ) == 0 ){
          _groundings.push_back( new Rule_Object_Type( l1, world ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_container_type" ) ) == 0 ){
          _groundings.push_back( new Rule_Container_Type( l1, world ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_object_color" ) ) == 0 ){
          _groundings.push_back( new Rule_Object_Color( l1, world ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_spatial_relation" ) ) == 0 ){
          _groundings.push_back( new Rule_Spatial_Relation( l1, world ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_constraint_type" ) ) == 0 ){
          _groundings.push_back( new Rule_Constraint_Type( l1, world ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_constraint_payload_type" ) ) == 0 ){
          _groundings.push_back( new Rule_Constraint_Payload_Type( l1, world ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_constraint_reference_type" ) ) == 0 ){
          _groundings.push_back( new Rule_Constraint_Reference_Type( l1, world ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_index" ) ) == 0 ){
          _groundings.push_back( new Rule_Index( l1, world ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_number" ) ) == 0 ){
          _groundings.push_back( new Rule_Number( l1, world ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
          _groundings.push_back( new Object( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "region" ) ) == 0 ){
          _groundings.push_back( new Region( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "constraint" ) ) == 0 ){
          _groundings.push_back( new Constraint( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "container" ) ) == 0 ){
          _groundings.push_back( new Container( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "container_type" ) ) == 0 ){
          _groundings.push_back( new Container_Type( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "region_container" ) ) == 0 ){
          _groundings.push_back( new Region_Container( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "index" ) ) == 0 ){
          _groundings.push_back( new Index( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "number" ) ) == 0 ){
          _groundings.push_back( new Number( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "object_color" ) ) == 0 ){
          _groundings.push_back( new Object_Color( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "object_type" ) ) == 0 ){
          _groundings.push_back( new Object_Type( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "spatial_relation" ) ) == 0 ){
          _groundings.push_back( new Spatial_Relation( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "object_property" ) ) == 0 ){
          _groundings.push_back( new Object_Property( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "abstract_container" ) ) == 0 ){
          _groundings.push_back( new Abstract_Container( l1, world ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "region_abstract_container" ) ) == 0 ){
          _groundings.push_back( new Region_Abstract_Container( l1, world ) );
        } else {
          cout << "could not identify symbol \"" << l1->name << "\" in Grounding_Set::from_xml" << endl;
          assert( false );
        }
      }
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Grounding_Set& other ) {
    out << "[" << other.groundings().size() << "]:{";
    for( unsigned int i = 0; i < other.groundings().size(); i++ ){
      if( other.groundings()[ i ] != NULL ){
        out << *other.groundings()[ i ];
      }
      if( i != ( other.groundings().size() - 1 ) ){
        out << ",";
      } 
    }
    out << "}";
    return out;
  }
}
