/**
 * @file feature_object_matches_child.h
 * 
 * @brief
 * 
 * a template used to describe a feature that checks the value of a type field
 */

#ifndef H2SL_FEATURE_OBJECT_MATCHES_CHILD_H
#define H2SL_FEATURE_OBJECT_MATCHES_CHILD_H

#include <iostream>
#include <sstream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Object_Matches_Child class definition
   */
  template< class T, class O >
  class Feature_Object_Matches_Child : public Feature {
  public:
    Feature_Object_Matches_Child( const bool& invert = false );
    Feature_Object_Matches_Child( xmlNodePtr root );
    Feature_Object_Matches_Child( const Feature_Object_Matches_Child& other );
    virtual ~Feature_Object_Matches_Child();
    Feature_Object_Matches_Child& operator=( const Feature_Object_Matches_Child& other );

    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual inline const feature_type_t type( void )const{ return FEATURE_TYPE_GROUNDING; };

  protected:

  private:

  };

  /** 
   * Feature_Object_Matches_Child class ostream operator
   */
  template< class T, class O >
  std::ostream& operator<<( std::ostream& out, const Feature_Object_Matches_Child< T, O >& other );

  #include "h2sl/feature_object_matches_child.ipp"
}

#endif /* H2SL_FEATURE_OBJECT_MATCHES_CHILD_H */
