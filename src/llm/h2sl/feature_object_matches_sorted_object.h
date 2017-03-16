/**
 * @file feature_object_matches_sorted_object.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the object index
 */

#ifndef H2SL_FEATURE_OBJECT_MATCHES_SORTED_OBJECT_H
#define H2SL_FEATURE_OBJECT_MATCHES_SORTED_OBJECT_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Object_Matches_Sorted_Object class definition
   */
  class Feature_Object_Matches_Sorted_Object : public Feature {
  public:
    Feature_Object_Matches_Sorted_Object( const bool& invert = false, const int& index = 0, const std::string& sortingKey = "na" );
    Feature_Object_Matches_Sorted_Object( xmlNodePtr root );
    Feature_Object_Matches_Sorted_Object( const Feature_Object_Matches_Sorted_Object& other );
    virtual ~Feature_Object_Matches_Sorted_Object();
    Feature_Object_Matches_Sorted_Object& operator=( const Feature_Object_Matches_Sorted_Object& other );

    virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );

    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline int& index( void ){ return get_prop< int >( _int_properties, "index" ); };
    inline const int& index( void )const{ return get_prop< int >( _int_properties, "index" ); };
    inline std::string& sorting_key( void ){ return get_prop< std::string >( _string_properties, "sorting_key" ); };
    inline const std::string& sorting_key( void )const{ return get_prop< std::string >( _string_properties, "sorting_key" ); };

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:

  private:

  };

  /** 
   * Feature_Object_Matches_Sorted_Object class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Object_Matches_Sorted_Object& other );
}

#endif /* H2SL_FEATURE_OBJECT_MATCHES_SORTED_OBJECT_H */


