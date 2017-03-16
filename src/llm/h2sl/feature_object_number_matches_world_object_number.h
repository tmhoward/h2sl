/**
 * @file feature_object_number_matches_world_object_number.h
 * 
 * @brief
 * 
 */

#ifndef H2SL_FEATURE_OBJECT_NUMBER_MATCHES_WORLD_OBJECT_NUMBER_H
#define H2SL_FEATURE_OBJECT_NUMBER_MATCHES_WORLD_OBJECT_NUMBER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Object_Number_Matches_World_Object_Number class definition
   */
  class Feature_Object_Number_Matches_World_Object_Number : public Feature {
  public:
    Feature_Object_Number_Matches_World_Object_Number( const bool& invert = false, const unsigned int& objectNumber = 0 );
    Feature_Object_Number_Matches_World_Object_Number( const Feature_Object_Number_Matches_World_Object_Number& other );
    virtual ~Feature_Object_Number_Matches_World_Object_Number();
    Feature_Object_Number_Matches_World_Object_Number& operator=( const Feature_Object_Number_Matches_World_Object_Number& other );

    virtual bool value( const std::string& cv, 
                        const Grounding* grounding, 
                        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, 
                        const h2sl::Phrase* phrase, 
                        const World* world );

    virtual bool value( const std::string& cv, 
                        const Grounding* grounding, 
                        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children,
                        const Phrase* phrase, 
                        const World* world, 
                        const Grounding* context );

    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline unsigned int& object_number( void ){ return _object_number; };
    inline const unsigned int& object_number( void )const{ return _object_number; };

    virtual inline const feature_type_t type( void )const{ return FEATURE_TYPE_GROUNDING; };

  protected:
    unsigned int _object_number;

  private:

  };

  /** 
   * Feature_Object_Number_Matches_World_Object_Number class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Object_Number_Matches_World_Object_Number& other );
}

#endif /* H2SL_FEATURE_OBJECT_NUMBER_MATCHES_WORLD_OBJECT_NUMBER_H */




