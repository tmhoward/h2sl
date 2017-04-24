/**
 * @file    region.cc
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
 * The implementation of a class used to describe a region of space
 */

#include "h2sl/rule_object_type.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/region.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

Region::
Region( const string& spatialRelationType,
        const string& objectId ) : Grounding() {
  insert_prop< std::string >( _string_properties, "spatial_relation_type", spatialRelationType );
  insert_prop< std::string >( _string_properties, "object_id", objectId );
}

Region::
Region( xmlNodePtr root,
        World* world ) : Grounding() {
  insert_prop< std::string >( _string_properties, "spatial_relation_type", "na" );
  insert_prop< std::string >( _string_properties, "object_id", "na" );
  from_xml( root, world );
}

Region::
~Region() {

}

Region::
Region( const Region& other ) : Grounding( other ) {

}

Region&
Region::
operator=( const Region& other ) {
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

bool
Region::
operator==( const Region& other )const{
  if( spatial_relation_type() != other.spatial_relation_type() ){
    return false;
  } if( object_id() != other.object_id() ){
    return false;
  } else {
    return true;
  }
}

bool
Region::
operator!=( const Region& other )const{
  return !( *this == other );
}

Grounding*
Region::
dup( void )const{
  return new Region( *this );
}

string 
Region::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Region* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Region* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Region::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "spatial_relation_type", spatial_relation_type(), stringTypes );
  return;
}

void
Region::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Region::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const std::string& symbolType ){

  if( symbolDictionary.has_class_name( class_name() ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "binary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< string > >::const_iterator it_spatial_relation_type_types = symbolDictionary.string_types().find( "spatial_relation_type" );

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( it_spatial_relation_type_types != symbolDictionary.string_types().end() ){
        for( unsigned int i = 0; i < it_spatial_relation_type_types->second.size(); i++ ){
          if( it_spatial_relation_type_types->second[ i ] != "na" ){
            if( world != NULL ){
              for( map< string, Object* >::const_iterator it_world_object = world->objects().begin(); it_world_object != world->objects().end(); it_world_object++ ){
                it_search_spaces_symbol->second.second.push_back( new Region( it_spatial_relation_type_types->second[ i ], it_world_object->second->id() ) );
              }
            }
          }   
        }
      }
    }
  }

  return;
}

void
Region::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  map< string, Object* >::const_iterator it_object = world->objects().find( object_id() );
  assert( it_object != world->objects().end() );
  Rule_Object_Type rule_object_type( it_object->second->type() );
  insert_unique_grounding< Rule_Object_Type >( groundingSet, rule_object_type );
  Rule_Spatial_Relation rule_spatial_relation( spatial_relation_type() );
  insert_unique_grounding< Rule_Spatial_Relation >( groundingSet, rule_spatial_relation );
  return;
}

bool
Region::
equals( const Grounding& other )const{
  if( dynamic_cast< const Region* >( &other ) != NULL ){
    return ( *this == *static_cast< const Region* >( &other ) );
  } else {
    return false;
  }
}

void
Region::
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
Region::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "region" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "spatial_relation_type" ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "object_id" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "object_id" ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Region::
to_latex( void )const{
  stringstream tmp;
  tmp << "Region(spatial_relation_type=" << spatial_relation_type() << ",object=" << object_id() << ")";
  return tmp.str();
}


void
Region::
from_xml( const string& filename,
          World* world ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "region" ) ) == 0 ){
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
Region::
from_xml( xmlNodePtr root,
          World* world ){
  spatial_relation_type() = "na";
  object_id() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > region_keys = { "spatial_relation_type", "region_type", "object_id" };
    assert( check_keys( root, region_keys ) );
    
    pair< bool, string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ){
      spatial_relation_type() = spatial_relation_type_prop.second;
    }
    pair< bool, string > region_type_prop = has_prop< std::string >( root, "region_type" );
    if( region_type_prop.first ){
      spatial_relation_type() = region_type_prop.second;
    }
    pair< bool, string > object_id_prop = has_prop< std::string >( root, "object_id" );
    if( object_id_prop.first ){
      object_id() = object_id_prop.second;
    }
    for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
          Object object( l1, world );  
          object_id() = object.id();
        }
      }
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Region& other ) {
    out << "Region(";
    out << "spatial_relation_type=\"" << other.spatial_relation_type() << "\",";
    out << "object_id=\"" << other.object_id() << "\"";
    out << ")";
    return out;
  }
}
