/**
 * @file number.h
 * 
 * @brief
 * 
 * a class used to describe a spatial relationship
 */

#ifndef H2SL_NUMBER_H
#define H2SL_NUMBER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/grounding.h"
#include "h2sl/object.h"


namespace h2sl {
  /**
   * Number class definition
   */
  class Number : public Grounding {
  public:

    Number( const std::string& numberValue = "na" );
    Number( const Number& other );
    Number( xmlNodePtr root );
    virtual ~Number();
    Number& operator=( const Number& other );
    bool operator==( const Number& other )const;
    bool operator!=( const Number& other )const;
    virtual Number* dup( void )const;
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline std::string& number_value( void ){ return get_prop< std::string >( _properties, "number_value" ); };
    inline const std::string& number_value( void )const{ return get_prop< std::string >( _properties, "number_value" ); };

  protected:

  private:

  };

  /** 
   * Number class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Number& other );
}

#endif /* H2SL_NUMBER_H */
