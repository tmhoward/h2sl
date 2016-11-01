/**
 * @file feature_region_abstract_container_type.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the region_abstract_container type
 */

#ifndef H2SL_FEATURE_REGION_ABSTRACT_CONTAINER_TYPE_H
#define H2SL_FEATURE_REGION_ABSTRACT_CONTAINER_TYPE_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Region_Abstract_Container_Type class definition
   */
  class Feature_Region_Abstract_Container_Type : public Feature {
  public:
    //Feature_Region_Abstract_Container_Type( const bool& invert = false, const unsigned int& region_abstractContainerType = 0 );
    Feature_Region_Abstract_Container_Type( const bool& invert = false, const std::string& region_abstract_container_type = "na" );
    Feature_Region_Abstract_Container_Type( const Feature_Region_Abstract_Container_Type& other );
    virtual ~Feature_Region_Abstract_Container_Type();
    Feature_Region_Abstract_Container_Type& operator=( const Feature_Region_Abstract_Container_Type& other );
 
    virtual bool value( const unsigned int& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    //unsigned int& region_abstract_container_type( void );
    //const unsigned int& region_abstract_container_type( void )const;
      
    std::string& region_abstract_container_type( void );
    const std::string& region_abstract_container_type( void )const;

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:
    //unsigned int _region_abstract_container_type;
      std::string _region_abstract_container_type;
      
  private:

  };

  /** 
   * Feature_Region_Abstract_Container_Type class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Region_Abstract_Container_Type& other );
}

#endif /* H2SL_FEATURE_REGION_ABSTRACT_CONTAINER_TYPE_H */
