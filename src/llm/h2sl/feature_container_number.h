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
    Feature_Container_Number( const bool& invert = false, const int& number = 0 );
    Feature_Container_Number( xmlNodePtr root );
    Feature_Container_Number( const Feature_Container_Number& other );
    virtual ~Feature_Container_Number();
    Feature_Container_Number& operator=( const Feature_Container_Number& other );
 
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline int& number( void ){ return get_prop< int >( _int_properties, "number" ); };
    inline const int& number( void )const{ return get_prop< int >( _int_properties, "number" ); };

    virtual inline const feature_type_t type( void )const{ return FEATURE_TYPE_GROUNDING; };

  protected:

  private:

  };

  /** 
   * Feature_Container_Number class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Container_Number& other );
}

#endif /* H2SL_FEATURE_CONTAINER_NUMBER_H */
