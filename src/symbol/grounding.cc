/**
 * @file    grounding.cc
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
 * The implementation of a class used to describe a grounding
 */

#include <assert.h>
#include "h2sl/grounding.h"
#include "h2sl/object.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/grounding_set.h"
#include "h2sl/abstract_container.h"
#include "h2sl/container.h"
#include "h2sl/index.h"
#include "h2sl/number.h"
#include "h2sl/object_color.h"
#include "h2sl/object_property.h"
#include "h2sl/object_type.h"
#include "h2sl/spatial_relation.h"

using namespace std;
using namespace h2sl;

Grounding::
Grounding( const std::map< std::string, std::string >& properties ) : _properties( properties ) {

} 

Grounding::
~Grounding() {

}

Grounding::
Grounding( const Grounding& other ) : _properties( other._properties ){

}

Grounding&
Grounding::
operator=( const Grounding& other ) {
  _properties = other._properties;
  return (*this);
}

bool 
Grounding::
operator==( const Grounding& other )const{
  return _equals( other );
}

bool 
Grounding::
operator!=( const Grounding& other )const{
  return !( *this == other );
}

Grounding*
Grounding::
dup( void )const{
  return new Grounding( *this );
}

void 
Grounding::
to_xml( const string& filename )const{
  return;
}

void 
Grounding::
to_xml( xmlDocPtr doc, 
        xmlNodePtr root )const{
  return;
}

void 
Grounding::
from_xml( const string& filename ){
  return;
}

void 
Grounding::
from_xml( xmlNodePtr root ){
  return;
}

bool
Grounding::
_equals( const Grounding& other )const{
  return true;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Grounding& other ) {
    if( dynamic_cast< const Grounding_Set* >( &other ) != NULL ){
      out << *static_cast< const Grounding_Set* >( &other ); 
    } else if( dynamic_cast< const Object* >( &other ) != NULL ){
      out << *static_cast< const Object* >( &other );
    } else if( dynamic_cast< const Region* >( &other ) != NULL ){
      out << *static_cast< const Region* >( &other );
    } else if( dynamic_cast< const Constraint* >( &other ) != NULL ){
      out << *static_cast< const Constraint* >( &other );
    } else if( dynamic_cast< const Abstract_Container* >( &other ) != NULL ){
      out << *static_cast< const Abstract_Container* >( &other );
    } else if( dynamic_cast< const Container* >( &other ) != NULL ){
      out << *static_cast< const Container* >( &other );
    } else if( dynamic_cast< const Index* >( &other ) != NULL ){
      out << *static_cast< const Index* >( &other );
    } else if( dynamic_cast< const Number* >( &other ) != NULL ){
      out << *static_cast< const Number* >( &other );
    } else if( dynamic_cast< const Object_Color* >( &other ) != NULL ){
      out << *static_cast< const Object_Color* >( &other );
    } else if( dynamic_cast< const Object_Property* >( &other ) != NULL ){
      out << *static_cast< const Object_Property* >( &other );
    } else if( dynamic_cast< const Object_Type* >( &other ) != NULL ){
      out << *static_cast< const Object_Type* >( &other );
    } else if( dynamic_cast< const Spatial_Relation* >( &other ) != NULL ){
      out << *static_cast< const Spatial_Relation* >( &other );
    }
    return out;
  }
}
