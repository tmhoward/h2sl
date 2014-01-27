/**
 * @file    object.h
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
 * The interface for a class used to describe an object
 */

#ifndef H2SL_OBJECT_H
#define H2SL_OBJECT_H

#include <iostream>
#include <libxml/tree.h>

#include "grounding.h"

namespace h2sl {
  typedef enum {
    OBJECT_TYPE_UNKNOWN,
    OBJECT_TYPE_ROBOT,
    OBJECT_TYPE_BOX,
    OBJECT_TYPE_TABLE,
    OBJECT_TYPE_CHAIR,
    OBJECT_TYPE_COUCH,
    OBJECT_TYPE_SINK,
    OBJECT_TYPE_MUG,
    OBJECT_TYPE_BOTTLE,
    OBJECT_TYPE_CAN,
    OBJECT_TYPE_FRIDGE,
    OBJECT_TYPE_HAMMER,
    OBJECT_TYPE_SCREWDRIVER,
    OBJECT_TYPE_PIPE,
    OBJECT_TYPE_NEWSPAPER,
    OBJECT_TYPE_MARKER,
    OBJECT_TYPE_TUBE,
    OBJECT_TYPE_TAPE,
    OBJECT_TYPE_BONE,
    OBJECT_TYPE_KITCHEN,
    OBJECT_TYPE_BATHROOM,
    OBJECT_TYPE_PANTRY,
    OBJECT_TYPE_BUILDING,
    OBJECT_TYPE_LIVINGROOM,
    OBJECT_TYPE_BEDROOM,
    OBJECT_TYPE_STUDY,
    OBJECT_TYPE_HALLWAY,
    NUM_OBJECT_TYPES
  } object_type_t;

  class Object: public Grounding {
  public:
    Object( const std::string& name = "na", const unsigned int& type = 0 );
    Object( const std::string& name, const object_type_t& type );
    virtual ~Object();
    Object( const Object& other );
    Object& operator=( const Object& other );
    bool operator==( const Object& other )const;
    bool operator!=( const Object& other )const;
    virtual Grounding* dup( void )const;

    static std::string type_to_std_string( const unsigned int& type );
    static unsigned int type_from_std_string( const std::string& type );

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    inline std::string& name( void ){ return _name; };
    inline const std::string& name( void )const{ return _name; };
    inline unsigned int& type( void ){ return _type; };
    inline const unsigned int& type( void )const{ return _type; };

  protected:
    std::string _name;
    unsigned int _type;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Object& other );
}

#endif /* H2SL_OBJECT_H */
