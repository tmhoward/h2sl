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
/*#include "h2sl_nsf_nri_mvli/abstract_container_msg_t.hpp"*/

#include "h2sl/grounding.h"
#include "h2sl/index.h"
#include "h2sl/object.h"

namespace h2sl {
  /**
   * Abstract_Container class definition
   */
  class Abstract_Container : public Grounding {
  public:
    Abstract_Container( const std::string& objectType = "na", const unsigned int& num = 0, const std::string& indexType = "na", const std::string& object_colorType = "na");
    Abstract_Container( const Abstract_Container& other );
    virtual ~Abstract_Container();
    Abstract_Container& operator=( const Abstract_Container& other );
    bool operator==( const Abstract_Container& other )const;
    bool operator!=( const Abstract_Container& other )const;
    Abstract_Container* dup( void )const;
  
    /*unsigned int& type( void );
    const unsigned int& type( void )const;
      
    unsigned int& num( void );
    const unsigned int& num( void )const;
      
    unsigned int& index( void );
    const unsigned int& index( void )const;
      
    unsigned int& color( void );
    const unsigned int& color( void )const;*/
 
    /*virtual bool from_msg( const abstract_container_msg_t * msg );
    virtual void to_msg( abstract_container_msg_t * msg )const;*/

    /*static std::string index_to_std_string( const Index& index );
    static Index index_from_std_string( const std::string& arg );*/
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

      
    inline std::string& object_type( void ){ return get_prop< std::string >( _properties, "object_type" );};
    inline const std::string& object_type( void )const{ return get_prop< std::string >( _properties, "object_type" ); };

    inline std::string& index_type( void ){ return get_prop< std::string >( _properties, "index_type" ); };
    inline const std::string& index_type( void )const{ return get_prop< std::string >( _properties, "index_type" ); };
      
    inline std::string& object_color_type( void ){ return get_prop< std::string >( _properties, "object_color_type" ); };
    inline const std::string& object_color_type( void )const{ return get_prop< std::string >( _properties, "object_color_type" ); };
      
  protected:
    /*unsigned int _type;*/
    unsigned int _num;
    /*unsigned int _index;
    unsigned int _color;*/
  private:

  };

  /** 
   * abstract_container class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Abstract_Container& other );
}

#endif /* H2SL_NSF_NRI_MVLI_ABSTRACT_CONTAINER_H */
