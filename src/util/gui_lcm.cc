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
#include "h2sl/constraint_t.hpp"

using namespace std;
using namespace lcm;
using namespace h2sl;

GUI_LCM::
GUI_LCM( Grammar * grammar,
          Parser< Phrase > * parser,
          World * world,
          LLM * llm,
          DCG * dcg,
          const unsigned int& beamWidth,
          const string& command,
          const string& worldChannel,
          QWidget * parent ) : GUI( grammar, parser, world, llm, dcg, beamWidth, command, parent ),
                               _lcm( new LCM()) {
  if( _lcm->good() ){
    cout << "subscribing to world" << endl;
    _lcm->subscribe( worldChannel, &GUI_LCM::_receive_world, this );
    
    _lcm_thread = new LCMThread (_lcm);
    _lcm_thread->start();
  } else
      cerr << "Error initializing LCM instance" << endl;
}

GUI_LCM::
~GUI_LCM() {
  if( _lcm != NULL ){
    delete _lcm;
    _lcm = NULL;
  }
  delete _lcm_thread;
}

void
GUI_LCM::
transform_to_transform_t( const Transform& transform,
                          transform_t& msg ){
  for( unsigned int i = 0; i < 3; i++ ){
    msg.position[ i ] = transform.position()[ i ];
  }
  for( unsigned int i = 0; i < 3; i++ ){
    msg.orientation[ i ] = transform.orientation().qv()[ i ];
  }
  msg.orientation[ 3 ] = transform.orientation().qs();
}

void 
GUI_LCM::
object_to_object_t( const Object& object, 
                    object_t& msg ){
  msg.name = object.name();
  msg.type = object.type();
  transform_to_transform_t( object.transform(), msg.transform );
  return;
}

void
GUI_LCM::
world_to_world_t( const World& world,
                  world_t& msg ){
  msg.utime = world.time();
  msg.num_objects = world.objects().size();
  msg.objects.resize( world.objects().size() );
  for( unsigned int i = 0; i < world.objects().size(); i++ ){
    object_to_object_t( *world.objects()[ i ], msg.objects[ i ] );
  }
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
grounding_set_to_constraint_set_t( const Grounding_Set& groundingSet,
                                    constraint_set_t& msg ){
  for( unsigned int i = 0; i < groundingSet.groundings().size(); i++ ){
    if( dynamic_cast< Constraint* >( groundingSet.groundings()[ i ] ) != NULL ){
      msg.constraints.push_back( constraint_t() );
      constraint_to_constraint_t( *static_cast< Constraint* >( groundingSet.groundings()[ i ] ), msg.constraints.back() );
    }
  }
  msg.num_constraints = msg.constraints.size();
  return;
}

void
GUI_LCM::
transform_from_transform_t( Transform& transform,
                            const transform_t& msg ){
  transform.position() = Vector3( msg.position[ 0 ],
                                  msg.position[ 1 ],
                                  msg.position[ 2 ] );
  transform.orientation().qv() = Vector3( msg.orientation[ 0 ], 
                                          msg.orientation[ 1 ],
                                          msg.orientation[ 2 ] );
  transform.orientation().qs() = msg.orientation[ 3 ];
  transform.orientation().normalize();
  return;
}

void 
GUI_LCM::
object_from_object_t( Object& object, 
                      const object_t& msg ){
  object.name() = msg.name;
  object.type() = msg.type;
  transform_from_transform_t( object.transform(), msg.transform );
  return;
}

void
GUI_LCM::
world_from_world_t( World& world,
                    const world_t& msg ){
  for( unsigned int i = 0; i < world.objects().size(); i++ ){
    if( world.objects()[ i ] != NULL ){
      delete world.objects()[ i ];
      world.objects()[ i ] = NULL;
    }
  }
  world.objects().clear();
  world.time() = msg.utime;
  world.objects().resize( msg.objects.size() );
  for( unsigned int i = 0; i < msg.objects.size(); i++ ){
    world.objects()[ i ] = new Object();
    object_from_object_t( *world.objects()[ i ], msg.objects[ i ] );
  }
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
grounding_set_from_constraint_set_t( Grounding_Set& groundingSet, 
                                      const constraint_set_t& msg ){
  groundingSet.groundings().resize( msg.constraints.size() );
  for( unsigned int i = 0; i < msg.constraints.size(); i++ ){
    groundingSet.groundings()[ i ] = new Constraint();
    constraint_from_constraint_t( *static_cast< Constraint* >( groundingSet.groundings()[ i ] ), msg.constraints[ i ] );
  }
  return;
}

void
GUI_LCM::
_send_output( void ){
  if( !_lcm->good() ){
    cout << "LCM isn't initialized" << endl;
    return;
  } else {
    cout << "sending LCM message" << endl;
  }

  Grounding_Set * grounding_set = dynamic_cast< Grounding_Set* >( _dcg->solutions().front().second->grounding() );
//  Grounding_Set * grounding_set = dynamic_cast< Grounding_Set* >( _dcg->root()->solutions().front().groundings.front() );
  if( grounding_set != NULL ){
    constraint_set_t msg;
    grounding_set_to_constraint_set_t( *grounding_set, msg );
    _lcm->publish( "CONSTRAINT_SET", &msg );
  }
  
  return;
}

void 
GUI_LCM::
_receive_world( const ReceiveBuffer* buf, 
                const string& channel, 
                const world_t * msg ){
  if( _world != NULL ){
    cout << "loading world" << endl;
    world_from_world_t( *_world, *msg );
    cout << "finished loading world" << endl;
    cout << *_world << endl;
    update_world();
  }
  return;
}


void
LCMThread::
run () {
    while (_lcm->handle() == 0);
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const GUI_LCM& other ) {
    return out;
  }
}
