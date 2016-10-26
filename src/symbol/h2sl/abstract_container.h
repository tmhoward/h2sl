 /**
 * @file Abstract_Container.h
 * 
 * @brief
 * 
 * a class that describes an abstract container
 */

#ifndef H2SL_ABSTRACT_CONTAINER_H
#define H2SL_ABSTRACT_CONTAINER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/grounding.h"
#include "h2sl/common.h"

#include "h2sl/index.h"
#include "h2sl/number.h"
#include "h2sl/object_color.h"
#include "h2sl/object_type.h"

namespace h2sl {
  /**
   * Abstract_Container class definition
   */
  class Abstract_Container : public Grounding {
  public:
    Abstract_Container( const std::string& objectType,
                        const std::string& numberType,
                        const std::string& indexType,
                        const std::string& colorType );                   

    Abstract_Container( const Abstract_Container& other );
    Abstract_Container( xmlNodePtr root );
    virtual ~Abstract_Container();
    Abstract_Container& operator=( const Abstract_Container& other );
    bool operator==( const Abstract_Container& other )const;
    bool operator!=( const Abstract_Container& other )const;
    Abstract_Container* dup( void )const;

    inline std::string& type( void ){
              return get_prop< std::string >( _properties, "object_type" ); };
    inline const std::string& type( void )const{
              return get_prop< std::string >( _properties, "object_type" ); };
   
    inline std::string& number( void ){ 
              return get_prop< std::string >( _properties, "number_type" ); };
    inline const std::string& number( void )const{ 
              return get_prop< std::string >( _properties, "number_type" ); };
 
    inline std::string& index( void ){ 
              return get_prop< std::string >( _properties, "index_type" ); };
    inline const std::string& index( void )const{ 
              return get_prop< std::string >( _properties, "index_type" ); };
 
    inline std::string& color( void ){
              return get_prop< std::string >( _properties, "object_color_type" ); };
    inline const std::string& color( void )const{
              return get_prop< std::string >( _properties, "object_color_type" ); };
    
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );

  protected:

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Abstract_Container& other );
}

#endif /* H2SL_ABSTRACT_CONTAINER_H */

