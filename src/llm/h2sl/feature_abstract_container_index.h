/**
 * @file feature_abstract_container_index.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the abstract_container index
 */

#ifndef H2SL_FEATURE_ABSTRACT_CONTAINER_INDEX_H
#define H2SL_FEATURE_ABSTRACT_CONTAINER_INDEX_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Abstract_Container_Index class definition
   */
  class Feature_Abstract_Container_Index : public Feature {
  public:
    Feature_Abstract_Container_Index( const bool& invert = false, const std::string& abstract_container_index = "na" );
    Feature_Abstract_Container_Index( const Feature_Abstract_Container_Index& other );
    virtual ~Feature_Abstract_Container_Index();
    Feature_Abstract_Container_Index& operator=( const Feature_Abstract_Container_Index& other );
 
    virtual bool value( const unsigned int& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const unsigned int& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
    //virtual bool value( const unsigned int& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    std::string& abstract_container_index( void );
    const std::string& abstract_container_index( void )const;

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:
    std::string _abstract_container_index;

  private:

  };

  /** 
   * Feature_Abstract_Container_Index class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Abstract_Container_Index& other );
}

#endif /* H2SL_FEATURE_ABSTRACT_CONTAINER_INDEX_H */
