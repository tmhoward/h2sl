/**
 * @file    lcm_message_spy.cc
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
 * A console-based LCM message inspector program
 */

#include <iostream>
#include <lcm/lcm-cpp.hpp>

#include "h2sl/constraint_set.h"
#include "h2sl/constraint.h"
#include "h2sl/region.h"
#include "h2sl/object.h"
#include "h2sl/gui_lcm.h"
#include "h2sl/constraint_set_t.hpp"
#include "lcm_message_spy_cmdline.h"

using namespace std;
using namespace h2sl;

class LCM_Message_Handler {
  public:
    void on_lcm_message ( const lcm::ReceiveBuffer *rbuf,
                         const std::string &channel) {
        int decode_status = -1;

        // An h2sl_constraint_set_t message
        h2sl::constraint_set_t constraint_set_msg;
        decode_status = constraint_set_msg.decode (rbuf->data, 0, rbuf->data_size);
        if (decode_status >= 0) {
            Constraint_Set constraint_set;
            GUI_LCM::constraint_set_from_constraint_set_t( constraint_set, constraint_set_msg );
            cout << "constraint_set: " << constraint_set << endl;
        }
    }
};

int
main( int argc,
      char* argv[] ) {
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  lcm::LCM lcm;

  if( !lcm.good() ){
      cerr << "LCM is not initialized" << endl;
    return 1;
  }

  LCM_Message_Handler lcm_message_handler;
  lcm.subscribe( args.channel_arg, &LCM_Message_Handler::on_lcm_message, &lcm_message_handler );
    
  while( true ){
    lcm.handle();
  }
  
  cout << "end of listener demo program" << endl;

  return 0;
}
