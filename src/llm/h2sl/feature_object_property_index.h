/**
 * @file feature_object_property_index.h
 * 
 * @brief
 * 
 * a class used to describe a feature of the object type
 */

#ifndef H2SL_FEATURE_OBJECT_PROPERTY_INDEX_H
#define H2SL_FEATURE_OBJECT_PROPERTY_INDEX_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Object_Property_Index class definition
   */
  class Feature_Object_Property_Index : public Feature {
  public:
    //Feature_Object_Property_Index( const bool& invert = false, const unsigned int& index = 0 );
    Feature_Object_Property_Index( const bool& invert = false, const std::string& index = "na" );
    
    Feature_Object_Property_Index( const Feature_Object_Property_Index& other );
    virtual ~Feature_Object_Property_Index();
    Feature_Object_Property_Index& operator=( const Feature_Object_Property_Index& other );

    virtual bool value( const unsigned int& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const unsigned int& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    //unsigned int& index( void );
    //const unsigned int& index( void )const;

    std::string& index( void );
    const std::string& index( void )const;

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:
    //unsigned int _index;
    std::string _index;

  private:

  };

  /** 
   * Feature_Object_Property_Index class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Object_Property_Index& other );
}

#endif /* H2SL_FEATURE_OBJECT_PROPERTY_INDEX_H */
