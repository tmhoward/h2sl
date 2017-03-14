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
#include "h2sl/rule_object_color.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/rule_constraint_type.h"
#include "h2sl/rule_constraint_payload_type.h"
#include "h2sl/rule_constraint_reference_type.h"
#include "h2sl/object.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/object_property.h"
#include "h2sl/object_type.h"
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
Grounding_Set( xmlNodePtr root ) : Grounding(),
                                    _groundings() {
  from_xml( root );
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
    _groundings[ i ] = other._groundings[ i ];
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
  cout << "scraping " << *this << endl;
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
  cout << "scraping " << *this << endl;
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


unsigned int
Grounding_Set::
evaluate_cv( const Grounding* grounding )const{
  unsigned int cv = CV_UNKNOWN;
  if( dynamic_cast< const Rule_Object_Type* >( grounding ) != NULL ){
    const Rule_Object_Type * rule_object_type_grounding = static_cast< const Rule_Object_Type* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Rule_Object_Type* >( _groundings[ i ] ) ){
        if( *rule_object_type_grounding == *static_cast< const Rule_Object_Type* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Rule_Object_Color* >( grounding ) != NULL ){
    const Rule_Object_Color * rule_object_color_grounding = static_cast< const Rule_Object_Color* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Rule_Object_Color* >( _groundings[ i ] ) ){
        if( *rule_object_color_grounding == *static_cast< const Rule_Object_Color* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Rule_Spatial_Relation* >( grounding ) != NULL ){
    const Rule_Spatial_Relation * rule_spatial_relation_grounding = static_cast< const Rule_Spatial_Relation* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Rule_Spatial_Relation* >( _groundings[ i ] ) ){
        if( *rule_spatial_relation_grounding == *static_cast< const Rule_Spatial_Relation* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Rule_Constraint_Type* >( grounding ) != NULL ){
    const Rule_Constraint_Type * rule_constraint_type_grounding = static_cast< const Rule_Constraint_Type* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Rule_Constraint_Type* >( _groundings[ i ] ) ){
        if( *rule_constraint_type_grounding == *static_cast< const Rule_Constraint_Type* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Rule_Constraint_Payload_Type* >( grounding ) != NULL ){
    const Rule_Constraint_Payload_Type * rule_constraint_payload_type_grounding = static_cast< const Rule_Constraint_Payload_Type* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Rule_Constraint_Payload_Type* >( _groundings[ i ] ) ){
        if( *rule_constraint_payload_type_grounding == *static_cast< const Rule_Constraint_Payload_Type* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Rule_Constraint_Reference_Type* >( grounding ) != NULL ){
    const Rule_Constraint_Reference_Type * rule_constraint_reference_type_grounding = static_cast< const Rule_Constraint_Reference_Type* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Rule_Constraint_Reference_Type* >( _groundings[ i ] ) ){
        if( *rule_constraint_reference_type_grounding == *static_cast< const Rule_Constraint_Reference_Type* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Object* >( grounding ) != NULL ){
    const Object * object_grounding = static_cast< const Object* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Object* >( _groundings[ i ] ) ){
        if( *object_grounding == *static_cast< const Object* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Region* >( grounding ) != NULL ){
    const Region * region_grounding = static_cast< const Region* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Region* >( _groundings[ i ] ) ){
        if( *region_grounding == *static_cast< const Region* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Constraint* >( grounding ) != NULL ){
    const Constraint * constraint_grounding = static_cast< const Constraint* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Constraint* >( _groundings[ i ] ) ){
        if( *constraint_grounding == *static_cast< const Constraint* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Object_Property* >( grounding ) != NULL ){
    const Object_Property * object_property_grounding = static_cast< const Object_Property* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Object_Property* >( _groundings[ i ] ) ){
        if( *object_property_grounding == *static_cast< const Object_Property* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Object_Type* >( grounding ) != NULL ){
    const Object_Type * object_type_grounding = static_cast< const Object_Type* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Object_Type* >( _groundings[ i ] ) ){
        if( *object_type_grounding == *static_cast< const Object_Type* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Object_Color* >( grounding ) != NULL ){
    const Object_Color * object_color_grounding = static_cast< const Object_Color* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Object_Color* >( _groundings[ i ] ) ){
        if( *object_color_grounding == *static_cast< const Object_Color* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Index* >( grounding ) != NULL ){
    const Index * index_grounding = static_cast< const Index* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Index* >( _groundings[ i ] ) ){
        if( *index_grounding == *static_cast< const Index* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Number* >( grounding ) != NULL ){
    const Number * number_grounding = static_cast< const Number* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Number* >( _groundings[ i ] ) ){
        if( *number_grounding == *static_cast< const Number* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if ( dynamic_cast< const Spatial_Relation* >( grounding ) != NULL ){
    const Spatial_Relation* spatial_relation_grounding = static_cast< const Spatial_Relation* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Spatial_Relation* >( _groundings[ i ] ) ){
        if( *spatial_relation_grounding == *static_cast< const Spatial_Relation* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if ( dynamic_cast< const Container* >( grounding ) != NULL ){
    const Container* container_grounding = static_cast< const Container* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Container* >( _groundings[ i ] ) ){
        if( *container_grounding == *static_cast< const Container* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if ( dynamic_cast< const Abstract_Container* >( grounding ) != NULL ){
    const Abstract_Container* abstract_container_grounding = static_cast< const Abstract_Container* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Abstract_Container* >( _groundings[ i ] ) ){
        if( *abstract_container_grounding == *static_cast< const Abstract_Container* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if ( dynamic_cast< const Region_Abstract_Container* >( grounding ) != NULL ){
    const Region_Abstract_Container* region_abstract_container_grounding = static_cast< const Region_Abstract_Container* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Region_Abstract_Container* >( _groundings[ i ] ) ){
        if( *region_abstract_container_grounding == *static_cast< const Region_Abstract_Container* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else if( dynamic_cast< const Region_Container* >( grounding ) != NULL ){
    const Region_Container* region_container_grounding = static_cast< const Region_Container* >( grounding );
    cv = CV_FALSE;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Region_Container* >( _groundings[ i ] ) ){
        if( *region_container_grounding == *static_cast< const Region_Container* >( _groundings[ i ] ) ){
          cv = CV_TRUE;
        }
      }
    }
  } else {
    cout << "grounding:" << *grounding << endl;
    assert( false );
  }

  return cv;
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

void
Grounding_Set::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "grounding_set" ) ) == 0 ){
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
Grounding_Set::
from_xml( xmlNodePtr root ){
  clear();
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_object_type" ) ) == 0 ){
          _groundings.push_back( new Rule_Object_Type( l1 ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_object_color" ) ) == 0 ){
          _groundings.push_back( new Rule_Object_Color( l1 ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_spatial_relation" ) ) == 0 ){
          _groundings.push_back( new Rule_Spatial_Relation( l1 ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_constraint_type" ) ) == 0 ){
          _groundings.push_back( new Rule_Constraint_Type( l1 ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_constraint_payload_type" ) ) == 0 ){
          _groundings.push_back( new Rule_Constraint_Payload_Type( l1 ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_constraint_reference_type" ) ) == 0 ){
          _groundings.push_back( new Rule_Constraint_Reference_Type( l1 ) );
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
          _groundings.push_back( new Object( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "region" ) ) == 0 ){
          _groundings.push_back( new Region( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "constraint" ) ) == 0 ){
          _groundings.push_back( new Constraint( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "container" ) ) == 0 ){
          _groundings.push_back( new Container( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "region_container" ) ) == 0 ){
          _groundings.push_back( new Region_Container( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "index" ) ) == 0 ){
          _groundings.push_back( new Index( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "number" ) ) == 0 ){
          _groundings.push_back( new Number( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "object_color" ) ) == 0 ){
          _groundings.push_back( new Object_Color( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "object_type" ) ) == 0 ){
          _groundings.push_back( new Object_Type( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "spatial_relation" ) ) == 0 ){
          _groundings.push_back( new Spatial_Relation( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "object_property" ) ) == 0 ){
          _groundings.push_back( new Object_Property( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "abstract_container" ) ) == 0 ){
          _groundings.push_back( new Abstract_Container( l1 ) );
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "region_abstract_container" ) ) == 0 ){
          _groundings.push_back( new Region_Abstract_Container( l1 ) );
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
