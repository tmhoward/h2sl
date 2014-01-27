/**
 * @file    constraint.h
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
 * The interface for a class used to represent a constraint
 */

#ifndef H2SL_CONSTRAINT_H
#define H2SL_CONSTRAINT_H

#include <iostream>

#include "h2sl/grounding.h"
#include "h2sl/region.h"

namespace h2sl {
  typedef enum {
    CONSTRAINT_TYPE_UNKNOWN,
    CONSTRAINT_TYPE_INSIDE,
    CONSTRAINT_TYPE_OUTSIDE,
    NUM_CONSTRAINT_TYPES
  } constraint_type_t;

  class Constraint: public Grounding {
  public:
    Constraint( const unsigned int& type = 0, const Region& parent = Region(), const Region& child = Region() );
    Constraint( const constraint_type_t& type, const Region&, const Region& child );
    virtual ~Constraint();
    Constraint( const Constraint& other );
    Constraint& operator=( const Constraint& other );
    bool operator==( const Constraint& other )const;
    bool operator!=( const Constraint& other )const;
    virtual Grounding* dup( void )const;

    static std::string type_to_std_string( const unsigned int& type );
    static unsigned int type_from_std_string( const std::string& type );

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    inline unsigned int& type( void ){ return _type; };
    inline const unsigned int& type( void )const{ return _type; };
    inline Region& parent( void ){ return _parent; };
    inline const Region& parent( void )const{ return _parent; };
    inline Region& child( void ){ return _child; };
    inline const Region& child( void )const{ return _child; };

  protected:
    unsigned int _type;
    Region _parent;
    Region _child;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Constraint& other );
}

#endif /* H2SL_CONSTRAINT_H */
