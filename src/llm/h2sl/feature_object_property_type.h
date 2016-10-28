/**
 * @file feature_object_property_type.h
 * 
 * @brief
 * 
 * a class used to describe a feature of the object type
 */

#ifndef H2SL_FEATURE_OBJECT_PROPERTY_TYPE_H
#define H2SL_FEATURE_OBJECT_PROPERTY_TYPE_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Object_Property_Type class definition
   */
  class Feature_Object_Property_Type : public Feature {
  public:
      Feature_Object_Property_Type( const bool& invert = false, const std::string& object_type = "na" );
    Feature_Object_Property_Type( const Feature_Object_Property_Type& other );
    virtual ~Feature_Object_Property_Type();
    Feature_Object_Property_Type& operator=( const Feature_Object_Property_Type& other );

    virtual bool value( const unsigned int& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    std::string& object_type( void );
    const std::string& object_type( void )const;

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:
    std::string _object_type;

  private:

  };

  /** 
   * Feature_Object_Property_Type class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Object_Property_Type& other );
}

#endif /* H2SL_FEATURE_OBJECT_PROPERTY_TYPE_H */
