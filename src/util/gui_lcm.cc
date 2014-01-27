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
using namespace h2sl;

GUI_LCM::
GUI_LCM( Parser * parser,
          World * world,
          LLM * llm,
          DCG * dcg,
          const unsigned int& beamWidth,
          QWidget * parent ) : GUI( parser, world, llm, dcg, beamWidth, parent ) {
    lcm::LCM lcm = lcm::LCM();

//  connect( _line_edit_sentence, SIGNAL( returnPressed() ), this, SLOT( _line_edit_sentence_return_pressed() ) );
}

GUI_LCM::
~GUI_LCM() {

}

void
GUI_LCM::
_send_message( void ){
  h2sl::xml_string_t xml_msg;
  if(!lcm.good())
      cout << "LCM isn't initialized" << endl;

  xml_msg.xml = _line_edit_sentence->text().toLower().toStdString();
  lcm.publish ("GUI_XML_STRING", &xml_msg);

  h2sl::constraint_set_t cmsg;
  cmsg.num_constraints = 2;
  h2sl::constraint_t c1;
  c1.type = 10;
  c1.parent.type = 1;
  c1.parent.object.type = 2;
  c1.parent.object.name = "hello";
  c1.child.object.type = 3;
  c1.child.object.name = "matt";
  h2sl::constraint_t c2;
  c2.type = 20;
  c2.parent.type = 1;
  c2.parent.object.type = 2;
  c2.parent.object.name = "hello";
  c2.child.object.type = 3;
  c2.child.object.name = "matt";
  cmsg.constraints.resize (cmsg.num_constraints);
  cmsg.constraints[0] = c1;
  cmsg.constraints[1] = c2;
  lcm.publish ("CONSTRAINT_SET", &cmsg);
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const GUI_LCM& other ) {
    return out;
  }

}
