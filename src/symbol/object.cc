/**
 * @file    object.cc
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
 * 02110-1301, USA. *
 * @section DESCRIPTION
 *
 * The implementation of a class used to describe an object
 */

#include "h2sl/common.h"
#include "h2sl/rule_object_type.h"
#include "h2sl/rule_object_color.h"
#include "h2sl/object.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

Object::
Object( const string& id,
        const string& objectType,
        const string& objectColor,
        const Transform& transform,
        const Vector3& linearVelocity,
        const Vector3& angularVelocity ) : Grounding(),   
                                            _transform( transform ),  
                                            _linear_velocity( linearVelocity ),
                                            _angular_velocity( angularVelocity ) {
   insert_prop< std::string >( _string_properties, "id", id ); 
   insert_prop< std::string >( _string_properties, "object_type", objectType ); 
   insert_prop< std::string >( _string_properties, "object_color", objectColor ); 
}

Object::
Object( xmlNodePtr root, World* world ) : Grounding(),
                            _transform(),
                            _linear_velocity(),
                            _angular_velocity() {
  insert_prop< std::string >( _string_properties, "id", "na" );
  insert_prop< std::string >( _string_properties, "object_type", "na" );
  insert_prop< std::string >( _string_properties, "object_color", "na" );
  from_xml( root, world );
}

Object::
~Object() {

}

Object::
Object( const Object& other ) : Grounding( other ),
                                _transform( other._transform ), 
                                _linear_velocity( other._linear_velocity ),
                                _angular_velocity( other._angular_velocity ) {

}

Object&
Object::
operator=( const Object& other ) {
  _string_properties = other._string_properties;
  _transform = other._transform;
  _linear_velocity = other._linear_velocity;
  _angular_velocity = other._angular_velocity;
  return (*this);
}

bool
Object::
operator==( const Object& other )const{
  if( id() != other.id() ){
    return false;
  } else if ( type() != other.type() ){
    return false;
  } else if ( color() != other.color() ){
    return false;
  } else {
    return true;
  }   
}

bool
Object::
operator!=( const Object& other )const{
  return !( *this == other );
} 
 
Object*
Object::
dup( void )const{
  return new Object( *this );
}

string 
Object::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Object* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Object* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv; 
}

void
Object::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "object_type", type(), stringTypes );
  insert_unique< std::string >( "object_color", color(), stringTypes );
  return;
}

void 
Object::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes, 
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void 
Object::
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

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( world != NULL ){
        for( map< string, Object* >::const_iterator it_world_object = world->objects().begin(); it_world_object != world->objects().end(); it_world_object++ ){
          it_search_spaces_symbol->second.second.push_back( it_world_object->second->dup() );
        }
      }
    }
  }

  return;
}

void
Object::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Object_Type rule_object_type( type() );
  insert_unique_grounding< Rule_Object_Type >( groundingSet, rule_object_type );
  Rule_Object_Color rule_object_color( color() );
  insert_unique_grounding< Rule_Object_Color >( groundingSet, rule_object_color );
  return;
}

bool
Object::
equals( const Grounding& other )const{
  if( dynamic_cast< const Object* >( &other ) != NULL ){
    return ( *this == *static_cast< const Object* >( &other ) );
  } else {
    return false;
  }
}
 
void
Object::
to_xml( const string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root, false );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

void
Object::
to_xml( const string& filename,
        const bool& writeAllProperties )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root, writeAllProperties );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

string
Object::
to_latex( void )const{
  stringstream tmp;
  tmp << "Object(id=" << id() << ")";
  return tmp.str();
}

void
Object::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  to_xml( doc, root, false );
  return;
}

void
Object::
to_xml( xmlDocPtr doc,
        xmlNodePtr root,
        const bool& writeAllProperties )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "object" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "id" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "id" ).c_str() ) );
  if( writeAllProperties ){
    xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "object_type" ).c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "object_color" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "object_color" ).c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "position" ), ( const xmlChar* )( _transform.position().to_std_string().c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "orientation" ), ( const xmlChar* )( _transform.orientation().to_std_string().c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "linear_velocity" ), ( const xmlChar* )( _linear_velocity.to_std_string().c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "angular_velocity" ), ( const xmlChar* )( _angular_velocity.to_std_string().c_str() ) );
  }
  xmlAddChild( root, node );
  return;
}

void
Object::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
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
Object::
from_xml( xmlNodePtr root,
          World* world ){
  id() = "na";
  type() = "na";
  color() = "na";
  transform() = Transform();
  linear_velocity() = Vector3();
  angular_velocity() = Vector3();

  if( root->type == XML_ELEMENT_NODE ){
    vector< string > object_keys = { "id", "name", "type", "object_type", "color", "object_color", "position", "orientation", "linear_velocity", "angular_velocity" };
    assert( check_keys( root, object_keys ) );
  
    pair< bool, string > id_prop = has_prop< std::string >( root, "id" );
    if( id_prop.first ){
      id() = id_prop.second;
    }
    pair< bool, string > name_prop = has_prop< std::string >( root, "name" );
    if( name_prop.first ){
      id() = name_prop.second;
    }

    if( world != NULL ){
      // load data from world model
      map< string, Object* >::iterator it_object = world->objects().find( id() );
      assert( it_object != world->objects().end() );
      *this = *it_object->second;
    } else {
      // load directly from entry
      pair< bool, string > type_prop = has_prop< std::string >( root, "type" );
      if( type_prop.first ){
        type() = type_prop.second;
      } 
      type_prop = has_prop< std::string >( root, "object_type" );
      if( type_prop.first ){
        type() = type_prop.second;
      }
      pair< bool, string > color_prop = has_prop< std::string >( root, "object_color" );
      if( color_prop.first ){
        color() = color_prop.second;
      }
      color_prop = has_prop< std::string >( root, "color" );
      if( color_prop.first ){
        color() = color_prop.second;
      }
      pair< bool, string > position_prop = has_prop< std::string >( root, "position" );
      if( position_prop.first ){
        _transform.position().from_std_string( position_prop.second );
      }
      pair< bool, string > orientation_prop = has_prop< std::string >( root, "orientation" );
      if( orientation_prop.first ){
        _transform.orientation().from_std_string( orientation_prop.second );
      }
      pair< bool, string > linear_velocity_prop = has_prop< std::string >( root, "linear_velocity" );
      if( linear_velocity_prop.first ){
        _linear_velocity.from_std_string( linear_velocity_prop.second );
      }
      pair< bool, string > angular_velocity_prop = has_prop< std::string >( root, "angular_velocity" );
      if( angular_velocity_prop.first ){
        _angular_velocity.from_std_string( angular_velocity_prop.second );
      }
    }
  }
  return;
}


namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Object& other ) {
    out << "Object(";
    out << "id=\"" << other.id() << "\",";
    out << "object_type=\"" << other.type() << "\",";
    out << "object_color=\"" << other.color() << "\",";
    out << "position=" << other.transform().position() << ",";
    out << "orientation=" << other.transform().orientation() << ",";
    if( other.linear_velocity().norm() > 0.0 ){
      out << ",linear_velocity=" << other.linear_velocity();
    }
    if( other.angular_velocity().norm() > 0.0 ){
      out << ",angular_velocity=" << other.angular_velocity();
    }
    out << ")";
    return out;
  }
    
  ostream&
  operator<<( ostream& out,
              const vector< Object* >& other ) {
    out << "[" << other.size() << "]:{";
    for( unsigned int i = 0; i < other.size(); i++ ){
      if( other[ i ] != NULL ){
        out << *other[ i ];
      } else {
        out << "NULL";
      }
      if( i != ( other.size() - 1 ) ){
        out << ",";
      }
    }
    out << "}";
    return out;
  }
}
