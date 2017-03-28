/**
 * @file    rule_set.cc
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
 * The implementation of a class used to describe a set of rules
 */

#include <assert.h>

#include "h2sl/cv.h"

#include "h2sl/rule_object_type.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/rule_set.h"

using namespace std;
using namespace h2sl;

Rule_Set::
Rule_Set( const vector< Rule* >& rules ) : Rule(),
                                                _rules( rules ) {

}

Rule_Set::
Rule_Set( xmlNodePtr root, World* world ) : Rule(),
                                    _rules() {
  from_xml( root, world );
}

Rule_Set::
~Rule_Set() {
  clear();
}

Rule_Set::
Rule_Set( const Rule_Set& other ) : Rule( other ),
                                        _rules(){
  _rules.resize( other._rules.size(), NULL );
  for( unsigned int i = 0; i < other._rules.size(); i++ ){
    _rules[ i ] = other._rules[ i ];
  }   
}

Rule_Set&
Rule_Set::
operator=( const Rule_Set& other ) {
  clear();
  _rules.resize( other._rules.size(), NULL );
  for( unsigned int i = 0; i < other._rules.size(); i++ ){
    _rules[ i ] = other._rules[ i ];
  }
  return (*this);
}

Rule_Set*
Rule_Set::
dup( void )const{
  return new Rule_Set( *this );
}

void
Rule_Set::
scrape_grounding( const World * world,
                  vector< string >& classNames, 
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  for( vector< Rule* >::const_iterator it_rule = _rules.begin(); it_rule != _rules.end(); it_rule++ ){
    if( ( *it_rule ) != NULL ){
      (*it_rule)->scrape_grounding( world, classNames, stringTypes, intTypes );
    }
  }
  return;
}

void
Rule_Set::
clear( void ){
  _rules.clear();
  return;
}


unsigned int
Rule_Set::
evaluate_cv( const Grounding* rule )const{
  string cv = CV_UNKNOWN;
  if( dynamic_cast< const Rule_Object_Type* >( rule ) != NULL ){
    const Rule_Object_Type * object_type_rule = static_cast< const Rule_Object_Type* >( rule );
    cv = "false";
    for( unsigned int i = 0; i < _rules.size(); i++ ){
      if( dynamic_cast< const Rule_Object_Type* >( _rules[ i ] ) ){
        if( *object_type_rule == *static_cast< const Rule_Object_Type* >( _rules[ i ] ) ){
          cv = "true";
        }
      }
    }
  } else if( dynamic_cast< const Rule_Spatial_Relation* >( rule ) != NULL ){
    const Rule_Spatial_Relation * spatial_relation_rule = static_cast< const Rule_Spatial_Relation* >( rule );
    cv = "false";
    for( unsigned int i = 0; i < _rules.size(); i++ ){
      if( dynamic_cast< const Rule_Spatial_Relation* >( _rules[ i ] ) ){
        if( *spatial_relation_rule == *static_cast< const Rule_Spatial_Relation* >( _rules[ i ] ) ){
          cv = "true";
        }
      }
    }
  } else {
    cout << "rule:" << *rule << endl;
    assert( false );
  }

  return cv;
}




void
Rule_Set::
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
Rule_Set::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "rule_set" ), NULL );
  for( unsigned int i = 0; i < _rules.size(); i++ ){
    if( _rules[ i ] != NULL ){
      _rules[ i ]->to_xml( doc, node );
    }
  }
  xmlAddChild( root, node );
  return;
}

void
Rule_Set::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_set" ) ) == 0 ){
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
Rule_Set::
from_xml( xmlNodePtr root, World* world ){
  clear();
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_object_type" ) ) == 0 ){
          _rules.push_back( new Rule_Object_Type( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_spatial_relation" ) ) == 0 ){
          _rules.push_back( new Rule_Spatial_Relation( l1 ) );
        } else {
          cout << "could not identify symbol \"" << l1->name << "\"" << endl;
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
              const Rule_Set& other ) {
    out << "[" << other.rules().size() << "]:{";
    for( unsigned int i = 0; i < other.rules().size(); i++ ){
      if( other.rules()[ i ] != NULL ){
        out << *other.rules()[ i ];
      }
      if( i != ( other.rules().size() - 1 ) ){
        out << ",";
      } 
    }
    out << "}";
    return out;
  }
}
