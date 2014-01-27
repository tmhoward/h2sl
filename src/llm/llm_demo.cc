/**
 * @file    llm_class_demo.cc
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
 * A LLM class demo program
 */

#include <iostream>

#include "h2sl/llm.h"
#include "llm_demo_cmdline.h"

using namespace std;
using namespace h2sl;

void
train( LLM * llm ){
  
  return;
}

int
main( int argc,
      char* argv[] ) {
  cout << "start of LLM class demo program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit(1);
  }

  LLM * llm = new LLM();

  if( args.feature_set_given ){
    llm->feature_set().from_xml( args.feature_set_arg );
    cout << "read " << llm->feature_set().size() << " features" << endl;
  }

  if( args.llm_given ){
    llm->from_xml( args.llm_arg );
  }

  if( args.training_given ){
    cout << "training model" << endl;
  }

  if( args.output_given ){
    cout << "writing llm to " << args.output_arg << endl;
    llm->to_xml( args.output_arg );
  }

  if( llm != NULL ){
    delete llm;
    llm = NULL;
  }

  cout << "end of LLM class demo program" << endl;
  return 0;
}
