/**
 * @file    feature_region_object_string_property_value.h
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
 * The interface for a class to check a region object's property value
 */

#ifndef H2SL_FEATURE_REGION_OBJECT_STRING_PROPERTY_VALUE_H
#define H2SL_FEATURE_REGION_OBJECT_STRING_PROPERTY_VALUE_H

#include <iostream>

#include <h2sl/feature_grounding_string_property_value.h>

namespace h2sl {
  class Feature_Region_Object_String_Property_Value : public Feature_Grounding_String_Property_Value {
  public:
    Feature_Region_Object_String_Property_Value( const bool& invert = false, const std::string& propertyKey = "na", const std::string& propertyValue = "na" );
    Feature_Region_Object_String_Property_Value( xmlNodePtr root );
    virtual ~Feature_Region_Object_String_Property_Value();
    Feature_Region_Object_String_Property_Value( const Feature_Region_Object_String_Property_Value& other );
    Feature_Region_Object_String_Property_Value& operator=( const Feature_Region_Object_String_Property_Value& other );

    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );

    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

  protected:

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Feature_Region_Object_String_Property_Value& other );
}

#endif /* H2SL_FEATURE_REGION_OBJECT_STRING_PROPERTY_VALUE_H */
