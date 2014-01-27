/**
 * @file    feature_region.h
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
 * The interface for a class used to check for a match with a region's type
 */

#ifndef H2SL_FEATURE_REGION_H
#define H2SL_FEATURE_REGION_H

#include <iostream>

#include <h2sl/feature.h>

namespace h2sl {
  class Feature_Region: public Feature {
  public:
    Feature_Region( const bool& invert = false, const unsigned int& regionType = 0 );
    virtual ~Feature_Region();
    Feature_Region( const Feature_Region& other );
    Feature_Region& operator=( const Feature_Region& other );

    virtual bool value( const unsigned int& cv, const Grounding* grounding, const std::vector< Grounding* >& children, const Phrase* phrase );

    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( xmlNodePtr root );

    inline unsigned int& region_type( void ){ return _region_type; };
    inline const unsigned int& region_type( void )const{ return _region_type; };

  protected:
    unsigned int _region_type;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Feature_Region& other );
}

#endif /* H2SL_FEATURE_REGION_H */
