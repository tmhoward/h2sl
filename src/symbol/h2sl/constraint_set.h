/**
 * @file    constraint_set.h
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
 * The interface for a class used to describe a set of constraints
 */

#ifndef H2SL_CONSTRAINT_SET_H
#define H2SL_CONSTRAINT_SET_H

#include <iostream>
#include <vector>

#include "grounding.h"
#include "constraint.h"

namespace h2sl {
  class Constraint_Set: public Grounding {
  public:
    Constraint_Set( const std::vector< Constraint >& constraints = std::vector< Constraint >() );
    virtual ~Constraint_Set();
    Constraint_Set( const Constraint_Set& other );
    Constraint_Set& operator=( const Constraint_Set& other );
    virtual Grounding* dup( void )const;

    void clear( void );

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    inline std::vector< Constraint >& constraints( void ){ return _constraints; };
    inline const std::vector< Constraint >& constraints( void )const{ return _constraints; };

  protected:
    std::vector< Constraint > _constraints;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Constraint_Set& other );
}

#endif /* H2SL_CONSTRAINT_SET_H */
