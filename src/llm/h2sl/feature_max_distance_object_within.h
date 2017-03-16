/**
 * @file feature_max_distance_object_within.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the container number
 */

#ifndef H2SL_NSF_NRI_MVLI_FEATURE_MAX_DISTANCE_OBJECT_WITHIN_H
#define H2SL_NSF_NRI_MVLI_FEATURE_MAX_DISTANCE_OBJECT_WITHIN_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Max_Distance_Object_Within class definition
   */
  class Feature_Max_Distance_Object_Within : public Feature {
  public:
    Feature_Max_Distance_Object_Within( const bool& invert = false, const unsigned int& number = 0 );
    Feature_Max_Distance_Object_Within( const Feature_Max_Distance_Object_Within& other );
    virtual ~Feature_Max_Distance_Object_Within();
    Feature_Max_Distance_Object_Within& operator=( const Feature_Max_Distance_Object_Within& other );
 
    virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world, const Grounding* context );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    unsigned int& number( void );
    const unsigned int& number( void )const;

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:
    unsigned int _number;

  private:

  };

  /** 
   * Feature_Max_Distance_Object_Within class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Max_Distance_Object_Within& other );
}

#endif /* H2SL_FEATURE_MAX_DISTANCE_OBJECT_WITHIN_H */
