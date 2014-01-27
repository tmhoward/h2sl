/**
 * @file    world.h
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
 * The interface for a class used to represent the world
 */

#ifndef H2SL_WORLD_H
#define H2SL_WORLD_H

#include <iostream>
#include <vector>

#include "object_set.h"

namespace h2sl {
  class World {
  public:
    World( const unsigned long long& time = 0, const Object_Set& objectSet = Object_Set() );
    virtual ~World();
    World( const World& other );
    World& operator=( const World& other );

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    inline unsigned long long& time( void ){ return _time; };
    inline const unsigned long long& time( void )const{ return _time; };
    inline Object_Set& object_set( void ){ return _object_set; };
    inline const Object_Set& object_set( void )const{ return _object_set; };

  protected:
    unsigned long long _time;
    Object_Set _object_set;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const World& other );
}

#endif /* H2SL_WORLD_H */
