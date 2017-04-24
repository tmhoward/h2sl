/**
 * @file    rule.cc
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
 * The implementation of a class used to describe a rule
 */

#include <assert.h>
#include "h2sl/rule.h"
#include "h2sl/rule_object_type.h"
#include "h2sl/rule_object_color.h"
#include "h2sl/rule_spatial_relation.h"

using namespace std;
using namespace h2sl;

Rule::
Rule( const std::map< std::string, std::string >& stringProperties,
      const std::map< std::string, int >& intProperties ) : Grounding( stringProperties, intProperties ) {

} 

Rule::
~Rule() {

}

Rule::
Rule( const Rule& other ) : Grounding( other ) {

}

Rule&
Rule::
operator=( const Rule& other ) {
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

bool 
Rule::
operator==( const Rule& other )const{
  return equals( other );
}

bool 
Rule::
operator!=( const Rule& other )const{
  return !( *this == other );
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Rule& other ) {
    if( dynamic_cast< const Rule_Object_Type* >( &other ) != NULL ){
      out << *static_cast< const Rule_Object_Type* >( &other );
    } else if( dynamic_cast< const Rule_Object_Color* >( &other ) != NULL ){
      out << *static_cast< const Rule_Object_Color* >( &other );
    } else if( dynamic_cast< const Rule_Spatial_Relation* >( &other ) != NULL ){
      out << *static_cast< const Rule_Spatial_Relation* >( &other );
    }
    return out;
  }
}
