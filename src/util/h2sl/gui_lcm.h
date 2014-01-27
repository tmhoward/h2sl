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
#include <lcm/lcm-cpp.hpp>

#include "h2sl/gui.h"
#include "h2sl/xml_string_t.hpp"

namespace h2sl {
  class GUI_LCM: public GUI {
    Q_OBJECT
  public:
    GUI_LCM( Parser * parser = NULL, World * world = NULL, LLM * llm = NULL, DCG * dcg = NULL, const unsigned int& beamWidth = 4, QWidget * parent = NULL );
    virtual ~GUI_LCM();

  protected:
    virtual void _send_message( void );

  private:
    lcm::LCM lcm;

  };
  std::ostream& operator<<( std::ostream& out, const GUI_LCM& other );
}

#endif /* H2SL_GUI_LCM_H */
