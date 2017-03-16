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
            const string& cv ) : Feature( invert ),
                                        _cv( cv ) {

}

Feature_CV::
Feature_CV( xmlNodePtr root ) : Feature(),
                                _cv( "" ) {
  from_xml( root );
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
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,   
        const World* world,
        const Grounding* context ){
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
  _cv = "false";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "cv" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }

    pair< bool, string > cv_prop = has_prop< std::string >( root, "cv" );
    if( cv_prop.first ){
      _cv = cv_prop.second;
    }
  }
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Feature_CV& other ) {
    out << "Feature_CV:(invert:(" << other.invert() << ") cv:(" << other.cv() << "))";
    return out;
  }

}
