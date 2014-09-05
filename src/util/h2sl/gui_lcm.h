/**
 * @file    gui_lcm.h
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
 * The interface for a class used for a graphical user interface with an LCM output
 */

#ifndef H2SL_GUI_LCM_H
#define H2SL_GUI_LCM_H

#include <iostream>
#include <QThread>
#include <lcm/lcm-cpp.hpp>

#include "h2sl/gui.h"
#include "h2sl/grounding_set.h"
#include "h2sl/constraint.h"
#include "h2sl/region.h"
#include "h2sl/object.h"
#include "h2sl/world.h"
#include "h2sl/constraint_set_t.hpp"
#include "h2sl/constraint_t.hpp"
#include "h2sl/region_t.hpp"
#include "h2sl/object_t.hpp"
#include "h2sl/world_t.hpp"

namespace h2sl {
  class LCMThread : public QThread {
    Q_OBJECT
        
  public:
    LCMThread (lcm::LCM *lcm) {_lcm = lcm; };
    virtual ~LCMThread () {};

  protected:
    void run ();

    lcm::LCM * _lcm;
  };

  class GUI_LCM: public GUI {
    Q_OBJECT
  public:
    GUI_LCM( Grammar * grammar = NULL, Parser< Phrase > * parser = NULL, World * world = NULL, LLM * llm = NULL, DCG * dcg = NULL, const unsigned int& beamWidth = 4, const std::string& command = "", const std::string& worldChannel = "WORLD", QWidget * parent = NULL );
    virtual ~GUI_LCM();

    static void transform_to_transform_t( const Transform& transform, transform_t& msg );
    static void object_to_object_t( const Object& object, object_t& msg );
    static void world_to_world_t( const World& world, world_t& msg );
    static void region_to_region_t( const Region& region, region_t& msg );
    static void constraint_to_constraint_t( const Constraint& constraint, constraint_t& msg );
    static void grounding_set_to_constraint_set_t( const Grounding_Set& groundingSet, constraint_set_t& msg );

    static void transform_from_transform_t( Transform& transform, const transform_t& msg );
    static void object_from_object_t( Object& object, const object_t& msg );
    static void world_from_world_t( World& world, const world_t& msg );
    static void region_from_region_t( Region& region, const region_t& msg );
    static void constraint_from_constraint_t( Constraint& constraint, const constraint_t& msg );
    static void grounding_set_from_constraint_set_t( Grounding_Set& groundingSet, const constraint_set_t& msg );

  protected:
    virtual void _send_output( void );
    virtual void _receive_world( const lcm::ReceiveBuffer* rbuf, const std::string& channel, const world_t * msg );
    
    lcm::LCM * _lcm;
    LCMThread *_lcm_thread;

  };
  std::ostream& operator<<( std::ostream& out, const GUI_LCM& other );
}

#endif /* H2SL_GUI_LCM_H */
