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
#include "h2sl/object.h"

using namespace std;
using namespace h2sl;

Object::
Object( const string& name,
        const string& objectType,
        const Transform& transform,
        const Vector3& linearVelocity,
        const Vector3& angularVelocity ) : Grounding(),   
                                            _transform( transform ),  
                                            _linear_velocity( linearVelocity ),
                                            _angular_velocity( angularVelocity ) {
   insert_prop< std::string >( _properties, "name", name ); 
   insert_prop< std::string >( _properties, "object_type", objectType ); 
}

Object::
Object( xmlNodePtr root ) : Grounding(),
                            _transform(),
                            _linear_velocity(),
                            _angular_velocity() {
  insert_prop< std::string >( _properties, "name", "na" );
  insert_prop< std::string >( _properties, "object_type", "na" );
  from_xml( root );
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
  _properties = other._properties;
  _transform = other._transform;
  _linear_velocity = other._linear_velocity;
  _angular_velocity = other._angular_velocity;
  return (*this);
}

bool
Object::
operator==( const Object& other )const{
  if( name() != other.name() ){
    return false;
  } else if ( type() != other.type() ){
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
 
void
Object::
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
Object::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "object" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "name" ), ( const xmlChar* )( get_prop< std::string >( _properties, "name" ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "object_type" ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "position" ), ( const xmlChar* )( _transform.position().to_std_string().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "orientation" ), ( const xmlChar* )( _transform.orientation().to_std_string().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "linear_velocity" ), ( const xmlChar* )( _linear_velocity.to_std_string().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "angular_velocity" ), ( const xmlChar* )( _angular_velocity.to_std_string().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Object::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
            from_xml( l1 );
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
from_xml( xmlNodePtr root ){
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > name_prop = has_prop< std::string >( root, "name" );
    if( name_prop.first ){
      name() = name_prop.second;
    }
    pair< bool, string > type_prop = has_prop< std::string >( root, "object_type" );
    if( type_prop.first ){
      type() = type_prop.second;
    }
  }
  return;
}


namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Object& other ) {
    out << "Object(";
    out << "name=\"" << other.name() << "\",";
    out << "object_type=\"" << other.type() << "\"";
    if( other.transform().position().norm() > 0.0 ){
      out << ",position=" << other.transform().position();
    }
    if( !other.transform().orientation().is_identity() ){
      out << ",orientation=" << other.transform().orientation();
    }
    if( other.linear_velocity().norm() > 0.0 ){
      out << ",linear_velocity=" << other.linear_velocity();
    }
    if( other.angular_velocity().norm() > 0.0 ){
      out << ",angular_velocity=" << other.angular_velocity();
    }
    out << ")";
    return out;
  }
}
