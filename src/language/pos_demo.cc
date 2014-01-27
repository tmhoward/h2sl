/**
 * @file    pos_demo.cc
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
 * A POS class demo program
 */

#include <iostream>
#include <h2sl/pos.h>

using namespace std;
using namespace h2sl;

int
main( int argc,
      char* argv[] ) {
  int status = 0;
  cout << "start of POS class demo program" << endl;

  cout << "pos_t_to_std_string( POS_JJ ): " << pos_t_to_std_string( POS_JJ ) << endl;
  cout << "pos_t_to_std_string( POS_NN ): " << pos_t_to_std_string( POS_NN ) << endl;
  cout << "pos_t_to_std_string( POS_TO ): " << pos_t_to_std_string( POS_TO ) << endl;
  cout << "pos_t_to_std_string( POS_VB ): " << pos_t_to_std_string( POS_VB ) << endl;

  cout << "pos_t_from_std_string( \"JJ\" ): " << pos_t_from_std_string( "JJ" ) << endl;
  cout << "pos_t_from_std_string( \"NN\" ): " << pos_t_from_std_string( "NN" ) << endl;
  cout << "pos_t_from_std_string( \"TO\" ): " << pos_t_from_std_string( "TO" ) << endl;
  cout << "pos_t_from_std_string( \"VB\" ): " << pos_t_from_std_string( "VB" ) << endl;

  cout << "end of POS class demo program" << endl;
  return status;
}
