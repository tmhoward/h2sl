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
#include <map>

#include "h2sl/object.h"

namespace h2sl {
  class World {
  public:
    World( const unsigned long long& time = 0, const std::map< std::string, Object* >& objects = std::map< std::string, Object* >() );
    World( const std::string& filename );
    virtual ~World();
    World( const World& other );
    World& operator=( const World& other );
    bool operator==( const World& other )const;
    bool operator!=( const World& other )const;
    virtual World* dup( void )const;

    // xml read and write functions
    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );
  
    // Obtain world model objects
    inline std::map< std::string, Object* >& objects( void ){ return _objects; };
    inline const std::map< std::string, Object* >& objects( void )const{ return _objects; };

    // Sorting of objects based on spatial characteristics. 
    virtual void sort_object_collections(void);

    // Min and max sorting function for objects along x axis.
    static bool min_x_sort( const Object* a, const Object* b );
    static bool max_x_sort( const Object* a, const Object* b );

    // Array sorting for min and max x sorted objects. 
    static void min_x_sort_objects( std::vector< Object* >& objects );
    static void max_x_sort_objects( std::vector< Object* >& objects );

    // Min and max sorting function for objects along y axis.
    static bool min_y_sort( const Object* a, const Object* b );
    static bool max_y_sort( const Object* a, const Object* b );

    // Array sorting for min and max y sorted objects. 
    static void min_y_sort_objects( std::vector< Object* >& objects );
    static void max_y_sort_objects( std::vector< Object* >& objects );
   
    // Sorting according to the absolute y coordinate.
    static bool min_abs_x_sort( const Object* a, const Object* b );
    static bool max_abs_x_sort( const Object* a, const Object* b );
    static bool min_abs_y_sort( const Object* a, const Object* b );
    static bool max_abs_y_sort( const Object* a, const Object* b );
 
    // Min and max sorting function for objects along z axis.
    static bool min_z_sort( const Object* a, const Object* b );
    static bool max_z_sort( const Object* a, const Object* b );

    // Min and max sorting function for objects with distance from origin.
    static bool min_distance_sort( const Object* a, const Object* b );
    static bool max_distance_sort( const Object* a, const Object* b );

    // Array sorting for min and max distance sorted objects. 
    static void min_distance_sort_objects( std::vector< Object* >& objects );
    static void max_distance_sort_objects( std::vector< Object* >& objects );

    // Array sorting for min and max distance from center sorted objects. 
    static void min_center_distance_sort_objects( std::vector< Object* >& objects );
    static void max_center_distance_sort_objects( std::vector< Object* >& objects );
     
    inline const std::map< std::string, std::map< std::string, std::vector< Object* > > >& sorted_objects( void )const{ return _sorted_objects; };
    
    // Time stamp for the world model
    inline unsigned long long& time( void ){ return _time; };
    inline const unsigned long long& time( void )const{ return _time; };
    
    // Convert model format.
    virtual void convert_models( xmlNodePtr root );

    // Transform objects with respect to object with id "reference_object_id"
    virtual bool transform_objects( const std::string& referece_object_id );

  protected:
   // World model time instant.
   unsigned long long _time;

   // World model objects.
   std::map< std::string, Object* > _objects;

    // Sorted world model objects.
    std::map< std::string, std::map< std::string, std::vector< Object* > > > _sorted_objects;

    // Initialise sorted object collections
    void initialise_sorted_object_collections( void );
    
  private:

  };
  std::ostream& operator<<( std::ostream& out, const World& other );
}

#endif /* H2SL_WORLD_H */
