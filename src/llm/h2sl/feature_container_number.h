/**
 * @file feature_container_number.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the container number
 */

#ifndef H2SL_FEATURE_CONTAINER_NUMBER_H
#define H2SL_FEATURE_CONTAINER_NUMBER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Container_Number class definition
   */
  class Feature_Container_Number : public Feature {
  public:
    Feature_Container_Number( const bool& invert = false, const unsigned int& abstractContainerNumber = 0 );
    Feature_Container_Number( const Feature_Container_Number& other );
    virtual ~Feature_Container_Number();
    Feature_Container_Number& operator=( const Feature_Container_Number& other );
 
    virtual bool value( const unsigned int& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    unsigned int& container_number( void );
    const unsigned int& container_number( void )const;

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:
    unsigned int _container_number;

  private:

  };

  /** 
   * Feature_Container_Number class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Container_Number& other );
}

#endif /* H2SL_FEATURE_CONTAINER_NUMBER_H */
