/**
 * @file feature_type.h
 * 
 * @brief
 * 
 * a template used to describe a feature that checks the value of a type field
 */

#ifndef H2SL_FEATURE_TYPE_H
#define H2SL_FEATURE_TYPE_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Type class definition
   */
  template< class T >
  class Feature_Type : public Feature {
  public:
    Feature_Type( const bool& invert = false, const unsigned int& symbolType = 0 );
    Feature_Type( xmlNodePtr root );
    Feature_Type( const Feature_Type& other );
    virtual ~Feature_Type();
    Feature_Type& operator=( const Feature_Type& other );

    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline unsigned int& symbol_type( void ){ return _symbol_type; };
    inline const unsigned int& symbol_type( void )const{ return _symbol_type; };

    virtual inline const feature_type_t type( void )const{ return FEATURE_TYPE_GROUNDING; };

  protected:
    unsigned int _symbol_type;

  private:

  };

  /** 
   * Feature_Type class ostream operator
   */
  template< class T >
  std::ostream& operator<<( std::ostream& out, const Feature_Type< T >& other );

  #include "h2sl/feature_type.ipp"
}

#endif /* H2SL_FEATURE_TYPE_H */
