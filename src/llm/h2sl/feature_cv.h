/**
 * @file    feature_cv.h
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
 * The interface for a class that looks at the value of the CV
 */

#ifndef H2SL_FEATURE_CV_H
#define H2SL_FEATURE_CV_H

#include <iostream>

#include <h2sl/feature.h>

namespace h2sl {
  class Feature_CV: public Feature {
  public:
    Feature_CV( const bool& invert = false, const unsigned int& cv = 0 );
    virtual ~Feature_CV();
    Feature_CV( const Feature_CV& other );
    Feature_CV& operator=( const Feature_CV& other );

    virtual bool value( const unsigned int& cv, const Grounding* grounding, const std::vector< Grounding* >& children, const Phrase* phrase ); 

    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( xmlNodePtr root );

    inline unsigned int& cv( void ){ return _cv; };
    inline const unsigned int& cv( void )const{ return _cv; };

  protected:
    unsigned int _cv;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Feature_CV& other );
}

#endif /* H2SL_FEATURE_CV_H */
