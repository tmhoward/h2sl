/**
 * @file feature_min_x_object.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the object number
 */

#ifndef H2SL_FEATURE_CONTAINER_MATCHES_SORTED_OBJECTS_H
#define H2SL_FEATURE_CONTAINER_MATCHES_SORTED_OBJECTS_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Container_Matches_Sorted_Objects class definition
   */
  class Feature_Container_Matches_Sorted_Objects : public Feature {
  public:
    Feature_Container_Matches_Sorted_Objects( const bool& invert = false, const int& number = 0, const std::string& sortingKey = "na" );
    Feature_Container_Matches_Sorted_Objects( xmlNodePtr root );
    Feature_Container_Matches_Sorted_Objects( const Feature_Container_Matches_Sorted_Objects& other );
    virtual ~Feature_Container_Matches_Sorted_Objects();
    Feature_Container_Matches_Sorted_Objects& operator=( const Feature_Container_Matches_Sorted_Objects& other );

    virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );

    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline int& number( void ){ return get_prop< int >( _int_properties, "number" ); };
    inline const int& number( void )const{ return get_prop< int >( _int_properties, "number" ); };
    inline std::string& sorting_key( void ){ return get_prop< std::string >( _string_properties, "sorting_key" ); };
    inline const std::string& sorting_key( void )const{ return get_prop< std::string >( _string_properties, "sorting_key" ); };

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:

  private:

  };

  /** 
   * Feature_Container_Matches_Sorted_Objects class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Container_Matches_Sorted_Objects& other );
}

#endif /* H2SL_FEATURE_CONTAINER_MATCHES_SORTED_OBJECTS_H */


