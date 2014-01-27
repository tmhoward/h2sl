/**
 * @file    feature.h
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
 * The interface for a class used to describe a feature
 */

#ifndef H2SL_FEATURE_H
#define H2SL_FEATURE_H

#include <iostream>
#include <vector>
#include <libxml/tree.h>

#include <h2sl/grounding.h>
#include <h2sl/phrase.h>

namespace h2sl {
  class Feature {
  public:
    Feature( const bool& invert = false );
    virtual ~Feature();
    Feature( const Feature& other );
    Feature& operator=( const Feature& other );

    virtual bool value( const unsigned int& cv, const Grounding* grounding, const std::vector< Grounding* >& children, const Phrase* phrase ) = 0;

    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const = 0;

    virtual void from_xml( xmlNodePtr root ) = 0;
 
    inline bool& invert( void ){ return _invert; };
    inline const bool& invert( void )const{ return _invert; };
       
  protected:
    bool _invert;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Feature& other );
}

#endif /* H2SL_FEATURE_H */
