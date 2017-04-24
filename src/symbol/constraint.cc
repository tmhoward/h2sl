/**
 * @file    constraint.cc
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
 * The implementation of a class used to represent a constraint
 */

#include "h2sl/rule_constraint_type.h"
#include "h2sl/rule_constraint_payload_type.h"
#include "h2sl/rule_constraint_reference_type.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/constraint.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

Constraint::
Constraint( const string& constraintType,
            const string& payload,
            const string& reference,
            const string& referenceRelation ) : Grounding() {
  insert_prop< std::string >( _string_properties, "constraint_type", constraintType );
  insert_prop< std::string >( _string_properties, "payload", payload );
  insert_prop< std::string >( _string_properties, "reference", reference );
  insert_prop< std::string >( _string_properties, "reference_relation", referenceRelation );
}

Constraint::
Constraint( const string& constraintType,
            const string& payload,
            const string& payloadRelation,
            const string& reference,
            const string& referenceRelation ) : Grounding() {
  insert_prop< std::string >( _string_properties, "constraint_type", constraintType );
  insert_prop< std::string >( _string_properties, "payload", payload );
  insert_prop< std::string >( _string_properties, "payload_relation", payloadRelation );
  insert_prop< std::string >( _string_properties, "reference", reference );
  insert_prop< std::string >( _string_properties, "reference_relation", referenceRelation );
}

Constraint::
Constraint( xmlNodePtr root, World* world ) : Grounding() {
  insert_prop< std::string >( _string_properties, "constraint_type", "na" );
  insert_prop< std::string >( _string_properties, "payload", "na" );
  insert_prop< std::string >( _string_properties, "reference", "na" );
  insert_prop< std::string >( _string_properties, "reference_relation", "na" );
  from_xml( root, world );
}

Constraint::
~Constraint() {

}

Constraint::
Constraint( const Constraint& other ) : Grounding( other ) {

}

Constraint&
Constraint::
operator=( const Constraint& other ) {
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

bool
Constraint::
operator==( const Constraint& other )const{
  if( constraint_type() != other.constraint_type() ){
    return false;
  } else if ( payload() != other.payload() ){
    return false;
  } else if ( reference() != other.reference() ){
    return false; 
  } else if ( reference_relation() != other.reference_relation() ){
    return false; 
  } else {
    return true;
  }
}

bool
Constraint::
operator!= ( const Constraint& other )const{
  return !( *this == other );
}

Grounding*
Constraint::
dup( void )const{
  return new Constraint( *this );
}

string 
Constraint::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Constraint* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Constraint* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Constraint::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "constraint_type", constraint_type(), stringTypes );
  map< string, Object* >::const_iterator it_payload = world->objects().find( payload() );
  if( it_payload != world->objects().end() ){
    insert_unique< std::string >( "constraint_payload_type", it_payload->second->type(), stringTypes );
  }
  map< string, Object* >::const_iterator it_reference = world->objects().find( reference() );
  if( it_reference != world->objects().end() ){
    insert_unique< std::string >( "constraint_reference_type", it_reference->second->type(), stringTypes );
  }
  insert_unique< std::string >( "spatial_relation_type", reference_relation(), stringTypes );
  return;
}

void
Constraint::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Constraint::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const std::string& symbolType ){
  if( symbolDictionary.has_class_name( class_name() ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "ternary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< string > >::const_iterator it_constraint_type_types = symbolDictionary.string_types().find( "constraint_type" );
    map< string, vector< string > >::const_iterator it_constraint_payload_type_types = symbolDictionary.string_types().find( "constraint_payload_type" );
    map< string, vector< string > >::const_iterator it_constraint_reference_type_types = symbolDictionary.string_types().find( "constraint_reference_type" );
    map< string, vector< string > >::const_iterator it_spatial_relation_type_types = symbolDictionary.string_types().find( "spatial_relation_type" );

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( ( it_constraint_type_types != symbolDictionary.string_types().end() ) && ( it_constraint_payload_type_types != symbolDictionary.string_types().end() ) && ( it_constraint_reference_type_types != symbolDictionary.string_types().end() ) && ( it_spatial_relation_type_types != symbolDictionary.string_types().end() ) ){
        for( unsigned int i = 0; i < it_constraint_type_types->second.size(); i++ ){
          for( map< string, Object* >::const_iterator it_world_object_1 = world->objects().begin(); it_world_object_1 != world->objects().end(); it_world_object_1++ ){
            if( find( it_constraint_payload_type_types->second.begin(), it_constraint_payload_type_types->second.end(), it_world_object_1->second->type() ) != it_constraint_payload_type_types->second.end() ){
              for( unsigned int k = 0; k < it_spatial_relation_type_types->second.size(); k++ ){
                for( map< string, Object* >::const_iterator it_world_object_2 = world->objects().begin(); it_world_object_2 != world->objects().end(); it_world_object_2++ ){
                  if( find( it_constraint_reference_type_types->second.begin(), it_constraint_reference_type_types->second.end(), it_world_object_2->second->type() ) != it_constraint_reference_type_types->second.end() ){
                    if( it_world_object_1->second != it_world_object_2->second ){
                      it_search_spaces_symbol->second.second.push_back( new Constraint( it_constraint_type_types->second[ i ], it_world_object_1->second->id(), it_world_object_2->second->id(), it_spatial_relation_type_types->second[ k ] ) );
                    }
                  }
                }
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
Constraint::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Constraint_Type rule_constraint_type( constraint_type() );
  insert_unique_grounding< Rule_Constraint_Type >( groundingSet, rule_constraint_type );
  map< string, Object* >::const_iterator it_payload_object = world->objects().find( payload() );
  assert( it_payload_object != world->objects().end() );
  Rule_Constraint_Payload_Type rule_constraint_payload_type( it_payload_object->second->type() );
  insert_unique_grounding< Rule_Constraint_Payload_Type >( groundingSet, rule_constraint_payload_type );
  map< string, Object* >::const_iterator it_reference_object = world->objects().find( reference() );
  assert( it_reference_object != world->objects().end() );
  Rule_Constraint_Reference_Type rule_constraint_reference_type( it_reference_object->second->type() );
  insert_unique_grounding< Rule_Constraint_Reference_Type >( groundingSet, rule_constraint_reference_type );
  Rule_Spatial_Relation rule_spatial_relation( reference_relation() );
  insert_unique_grounding< Rule_Spatial_Relation >( groundingSet, rule_spatial_relation );
  return;
}

bool
Constraint::
equals( const Grounding& other )const{
  if( dynamic_cast< const Constraint* >( &other ) != NULL ){
    return ( *this == *static_cast< const Constraint* >( &other ) );
  } else {
    return false;
  }
}

void
Constraint::
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
Constraint::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "constraint" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "constraint_type" ), ( const xmlChar* )( constraint_type().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "payload" ), ( const xmlChar* )( payload().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "reference" ), ( const xmlChar* )( reference().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "reference_relation" ), ( const xmlChar* )( reference_relation().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Constraint::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "constraint" ) ) == 0 ){
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
Constraint::
from_xml( xmlNodePtr root, World* world ){
  constraint_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > constraint_keys = { "constraint_type", "payload", "reference", "reference_relation" };
    assert( check_keys( root, constraint_keys ) );
    pair< bool, string > constraint_type_prop = has_prop< std::string >( root, "constraint_type" );
    if( constraint_type_prop.first ){
      constraint_type() = constraint_type_prop.second;
    }
    pair< bool, string > payload_prop = has_prop< std::string >( root, "payload" );
    if( payload_prop.first ){
      payload() = payload_prop.second;
    }
    pair< bool, string > reference_prop = has_prop< std::string >( root, "reference" );
    if( reference_prop.first ){
      reference() = reference_prop.second;
    }
    pair< bool, string > reference_relation_prop = has_prop< std::string >( root, "reference_relation" );
    if( reference_relation_prop.first ){
      reference_relation() = reference_relation_prop.second;
    }
    for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
      if( matches_name( l1, "properties" ) ){
        for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
          if( matches_name( l2, "property" ) ){
            pair< bool, string > payload_prop = has_prop< std::string >( l2, "payload" );
            if( payload_prop.first ){
              payload() = payload_prop.second;
            }
            pair< bool, string > reference_prop = has_prop< std::string >( l2, "reference" );
            if( reference_prop.first ){
              reference() = reference_prop.second;
            }
            pair< bool, string > reference_relation_prop = has_prop< std::string >( l2, "reference_relation" );
            if( reference_relation_prop.first ){
              reference_relation() = reference_relation_prop.second;
            } 
          }
        }       
      }
    }
  }
  return;
}

string
Constraint::
to_latex( void )const{
  stringstream tmp;
  tmp << "Constraint(constraint\\_type=" << constraint_type() << ",payload=" << payload() << ",reference=" << reference() << ",reference\\_relation=" << reference_relation() << ")";
  return tmp.str();
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Constraint& other ) {
    out << "Constraint(";
    out << "type=\"" << other.constraint_type() << "\",";
    out << "payload=\"" << other.payload() << "\",";
    out << "reference=\"" << other.reference() << "\",";
    out << "reference_relation=\"" << other.reference_relation() << "\"";
    out << ")";
    return out;
  }
}
