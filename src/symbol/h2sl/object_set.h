/**
 * @file    object_set.h
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
 * The interface for a class used to describe a set of objects
 */

#ifndef H2SL_OBJECT_SET_H
#define H2SL_OBJECT_SET_H

#include <iostream>
#include <vector>

#include "grounding.h"
#include "object.h"

namespace h2sl {
  class Object_Set: public Grounding {
  public:
    Object_Set( const std::vector< Object >& objects = std::vector< Object >() );
    virtual ~Object_Set();
    Object_Set( const Object_Set& other );
    Object_Set& operator=( const Object_Set& other );

    void clear( void );

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    inline std::vector< Object >& objects( void ){ return _objects; };
    inline const std::vector< Object >& objects( void )const{ return _objects; };

  protected:
    std::vector< Object > _objects;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Object_Set& other );
}

#endif /* H2SL_OBJECT_SET_H */
