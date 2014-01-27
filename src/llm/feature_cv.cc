/**
 * @file    feature_cv.cc
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
 * 02110-1301, USA. *
 * @section DESCRIPTION
 *
 * The implementation of a class that looks at the value of the CV
 */

#include <sstream>

#include <h2sl/feature_cv.h>

using namespace std;
using namespace h2sl;

Feature_CV::
Feature_CV( const bool& invert,
            const unsigned int& cv ) : Feature( invert ),
                                        _cv( cv ) {

}

Feature_CV::
~Feature_CV() {

}

Feature_CV::
Feature_CV( const Feature_CV& other ) : Feature( other ),
                                        _cv( other._cv ){

}

Feature_CV&
Feature_CV::
operator=( const Feature_CV& other ) {
  _invert = other._invert;
  _cv = other._cv;
  return (*this);
}

bool
Feature_CV::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< Grounding* >& children,
        const Phrase* phrase ){
  if( _invert ){
    return ( cv != _cv );
  } else {
    return ( cv == _cv );
  }
}

void
Feature_CV::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_cv" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  stringstream cv_string;
  cv_string << _cv;
  xmlNewProp( node, ( const xmlChar* )( "cv" ), ( const xmlChar* )( cv_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_CV::
from_xml( xmlNodePtr root ){
  _invert = false;
  _cv = 0;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "cv" ) );
    if( tmp != NULL ){
      string cv_string = ( char* )( tmp );
      _cv = ( unsigned int )( strtol( cv_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_CV& other ) {
    return out;
  }

}
