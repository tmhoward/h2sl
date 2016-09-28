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
/*#include "h2sl_nsf_nri_mvli/spatial_relation.h"*/

/*#include "h2sl/number_msg_t.hpp"*/

namespace h2sl {
  /**
   * Number class definition
   */
  class Number : public Grounding {
  public:
    enum Value {
      VALUE_UNKNOWN,
      VALUE_ONE,
      VALUE_TWO,
      VALUE_THREE,
      VALUE_FOUR,
      VALUE_FIVE,
      VALUE_SIX,
      VALUE_SEVEN,
      VALUE_EIGHT,
      VALUE_NINE,
      VALUE_TEN,
      VALUE_ELEVEN,
      VALUE_TWELVE,
      VALUE_NUM_VALUES
    };

    Number( const Value& value = Number::VALUE_UNKNOWN );
    Number( const Number& other );
    virtual ~Number();
    Number& operator=( const Number& other );
    bool operator==( const Number& other )const;
    bool operator!=( const Number& other )const;
    virtual Number* dup( void )const;
 
    /*virtual bool from_msg( const number_msg_t * msg );
    virtual void to_msg( number_msg_t * msg )const;*/
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    static std::string value_to_std_string( const Value& value );
    static Number::Value value_from_std_string( const std::string& arg );

    inline Value& value( void ){ return _value; };
    inline const Value& value( void )const{ return _value; };

  protected:
    Value _value;

  private:

  };

  /** 
   * Number class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Number& other );
}

#endif /* H2SL_NUMBER_H */
