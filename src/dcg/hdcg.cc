/**
 * @file    hdcg.cc
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
 * The implementation of a class used to represent a Hierarchial 
 *   Distributed Correspondence Graph
 */

#include <fstream>
#include <utility>

#include "h2sl/grounding_set.h"

#include "h2sl/hdcg.h"

using namespace std;
using namespace h2sl;

HDCG::
HDCG() : _dcg_rules(),
          _dcg_groundings() {

}

HDCG::
~HDCG() {

}

HDCG::
HDCG( const HDCG& other ) : _dcg_rules( other._dcg_rules ),
                            _dcg_groundings( other._dcg_groundings ) {

}

HDCG&
HDCG::
operator=( const HDCG& other ) {
  _dcg_rules = other._dcg_rules;
  _dcg_groundings = other._dcg_groundings;
  return (*this);
}

bool
HDCG::
leaf_search( const Phrase* phrase,
              const World* world,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  return leaf_search( phrase, world, NULL, llm, beamWidth, debug );
}
  
bool
HDCG::
leaf_search( const Phrase* phrase,
              const World* world,
              const Grounding* context,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  if( phrase != NULL ){
    // first infer a distribution of reduced symbol dictionary from the rules

    // now ground the phrase with the inferred distribution of symbol dictionaries

    return true;
  } else {
    return false;
  }
}

void
HDCG::
to_latex( const string& filename )const{
  ofstream outfile;
  outfile.open( filename.c_str() );
  outfile << "\\begin{tikzpicture}[textnode/.style={anchor=mid,font=\\tiny},nodeknown/.style={circle,draw=black!80,fill=black!10,minimum size=5mm,font=\\tiny,top color=white,bottom color=black!20},nodeunknown/.style={circle,draw=black!80,fill=white,minimum size=5mm,font=\\tiny},factor/.style={rectangle,draw=black!80,fill=black!80,minimum size=2mm,font=\\tiny,text=white},dot/.style={circle,draw=black!80,fill=black!80,minimum size=0.25mm,font=\\tiny}]" << endl;
  outfile << "\\end{tikzpicture}" << endl;
  outfile.close();
  return;
} 

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const HDCG& other ) {
    
    return out;
  }
}
