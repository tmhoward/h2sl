/**
 * @file feature_merge_children.h
 * 
 * @brief
 * 
 * a template used to describe a feature that checks the value of a type field
 */

#ifndef H2SL_FEATURE_MERGE_CHILDREN_H
#define H2SL_FEATURE_MERGE_CHILDREN_H

#include <iostream>
#include <sstream>
#include <libxml/tree.h>

#include "h2sl/common.h"
#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Merge_Children class definition
   */
  template< class A, class B >
  class Feature_Merge_Children : public Feature {
  public:
    Feature_Merge_Children( const bool& invert = false, const bool& order = false, const bool& same = false );
    Feature_Merge_Children( xmlNodePtr root );
    Feature_Merge_Children( const Feature_Merge_Children& other );
    virtual ~Feature_Merge_Children();
    Feature_Merge_Children& operator=( const Feature_Merge_Children& other );

    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline bool& order( void ){ return _order; };
    inline const bool& order( void )const{ return _order; };
    inline bool& same( void ){ return _same; };
    inline const bool& same( void )const{ return _same; };

    virtual inline const feature_type_t type( void )const{ return FEATURE_TYPE_GROUNDING; };

  protected:
    bool _order;
    bool _same;

  private:

  };

  /** 
   * Feature_Merge_Children class ostream operator
   */
  template< class A, class B >
  std::ostream& operator<<( std::ostream& out, const Feature_Merge_Children< A, B >& other );

  #include "h2sl/feature_merge_children.ipp"
}

#endif /* H2SL_FEATURE_MERGE_CHILDREN_H */
