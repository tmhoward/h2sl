/**
 * @file feature_matches_child.h
 * 
 * @brief
 * 
 * a template used to describe a feature that checks the value of a type field
 */

#ifndef H2SL_FEATURE_MATCHES_CHILD_H
#define H2SL_FEATURE_MATCHES_CHILD_H

#include <iostream>
#include <sstream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Matches_Child class definition
   */
  template< class T >
  class Feature_Matches_Child : public Feature {
  public:
    Feature_Matches_Child( const bool& invert = false );
    Feature_Matches_Child( xmlNodePtr root );
    Feature_Matches_Child( const Feature_Matches_Child& other );
    virtual ~Feature_Matches_Child();
    Feature_Matches_Child& operator=( const Feature_Matches_Child& other );

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
   * Feature_Matches_Child class ostream operator
   */
  template< class T >
  std::ostream& operator<<( std::ostream& out, const Feature_Matches_Child< T >& other );

  #include "h2sl/feature_matches_child.ipp"
}

#endif /* H2SL_FEATURE_MATCHES_CHILD_H */
