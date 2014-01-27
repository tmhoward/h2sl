/**
 * @file    gui_lcm.cc
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
 * The implementation of a class used for a graphical user interface with an LCM output
 */

#include "h2sl/gui_lcm.h"
#include "h2sl/constraint_set_t.hpp"

using namespace std;
using namespace lcm;
using namespace h2sl;

GUI_LCM::
GUI_LCM( Parser * parser,
          World * world,
          LLM * llm,
          DCG * dcg,
          const unsigned int& beamWidth,
          QWidget * parent ) : GUI( parser, world, llm, dcg, beamWidth, parent ),
                                _lcm( new LCM() ) {
}

GUI_LCM::
~GUI_LCM() {
  if( _lcm != NULL ){
    delete _lcm;
    _lcm = NULL;
  }
}

void 
GUI_LCM::
object_to_object_t( const Object& object, 
                    object_t& msg ){
  msg.name = object.name();
  msg.type = object.type();
  return;
}

void 
GUI_LCM::
region_to_region_t( const Region& region, 
                    region_t& msg ){
  msg.type = region.type();
  object_to_object_t( region.object(), msg.object );
  return;
}

void 
GUI_LCM::
constraint_to_constraint_t( const Constraint& constraint, 
                            constraint_t& msg ){
  msg.type = constraint.type();
  region_to_region_t( constraint.parent(), msg.parent );
  region_to_region_t( constraint.child(), msg.child );
  return;
}

void
GUI_LCM::
constraint_set_to_constraint_set_t( const Constraint_Set& constraintSet,
                                    constraint_set_t& msg ){
  msg.num_constraints = constraintSet.constraints().size();
  msg.constraints.resize( constraintSet.constraints().size() );
  for( unsigned int i = 0; i < constraintSet.constraints().size(); i++ ){
    constraint_to_constraint_t( constraintSet.constraints()[ i ], msg.constraints[ i ] );
  }
  return;
}

void 
GUI_LCM::
object_from_object_t( Object& object, 
                      const object_t& msg ){
  object.name() = msg.name;
  object.type() = msg.type;
  return;
}

void 
GUI_LCM::
region_from_region_t( Region& region, 
                      const region_t& msg ){
  region.type() = msg.type;
  object_from_object_t( region.object(), msg.object );
  return;
}

void 
GUI_LCM::
constraint_from_constraint_t( Constraint& constraint, 
                              const constraint_t& msg ){
  constraint.type() = msg.type;
  region_from_region_t( constraint.parent(), msg.parent );
  region_from_region_t( constraint.child(), msg.child );
  return;
}

void 
GUI_LCM::
constraint_set_from_constraint_set_t( Constraint_Set& constraintSet, 
                                      const constraint_set_t& msg ){
  constraintSet.constraints().resize( msg.constraints.size() );
  for( unsigned int i = 0; i < msg.constraints.size(); i++ ){
    constraint_from_constraint_t( constraintSet.constraints()[ i ], msg.constraints[ i ] );
  }
  return;
}

void
GUI_LCM::
_send_message( void ){
  if( !_lcm->good() ){
    cout << "LCM isn't initialized" << endl;
    return;
  } else {
    cout << "sending LCM message" << endl;
  }

  if( !_solutions.empty() ){
    Constraint_Set * constraint_set = dynamic_cast< Constraint_Set* >( _solutions.front().second.grounding() );
    if( constraint_set != NULL ){
      constraint_set_t msg;
      constraint_set_to_constraint_set_t( *constraint_set, msg );
      _lcm->publish( "CONSTRAINT_SET", &msg );
    }
  }

  return;
}


namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const GUI_LCM& other ) {
    return out;
  }
}
