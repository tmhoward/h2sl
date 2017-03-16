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

#include "h2sl/grounding.h"
#include "h2sl/phrase.h"
#include "h2sl/world.h"

namespace h2sl {
  typedef enum {
    FEATURE_TYPE_UNKNOWN,
    FEATURE_TYPE_LANGUAGE,
    FEATURE_TYPE_GROUNDING,
    FEATURE_TYPE_CORRESPONDENCE,  
    NUM_FEATURE_TYPES
  } feature_type_t;

  class Feature {
  public:
    Feature( const bool& invert = false, const std::map< std::string, std::string >& stringProperties = std::map< std::string, std::string >(), const std::map< std::string, int >& intProperties = std::map< std::string, int >() );
    virtual ~Feature();
    Feature( const Feature& other );
    Feature& operator=( const Feature& other );

    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context ) = 0;

    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const = 0;

    virtual void from_xml( xmlNodePtr root ) = 0;
 
    inline bool& invert( void ){ return _invert; };
    inline const bool& invert( void )const{ return _invert; };
    inline std::map< std::string, std::string >& string_properties( void ){ return _string_properties; };
    inline const std::map< std::string, std::string >& string_properties( void )const{ return _string_properties; };
    inline std::map< std::string, int >& int_properties( void ){ return _int_properties; };
    inline const std::map< std::string, int >& int_properties( void )const{ return _int_properties; };
    virtual const feature_type_t type( void )const = 0;
    
  protected:
    bool _invert;
    std::map< std::string, std::string > _string_properties;
    std::map< std::string, int > _int_properties;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Feature& other );
}

#endif /* H2SL_FEATURE_H */
