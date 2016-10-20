/**
 * @file object_color.h
 *
 * @brief
 *
 * a class that describes a object_color
 */

#ifndef H2SL_OBJECT_COLOR_H
#define H2SL_OBJECT_COLOR_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/transform.h"

#include "h2sl/grounding.h"

#include "h2sl/object.h"

/*#include "h2sl_nsf_nri_mvli/object_color_msg_t.hpp"*/

namespace h2sl {
    /**
     * Object_Color class definition
     */
    class Object_Color : public Grounding {
    public:
        Object_Color( const std::string& object_colorType = "na" );
        Object_Color( const Object_Color& other );
        Object_Color( xmlNodePtr root );
        virtual ~Object_Color();
        Object_Color& operator=( const Object_Color& other );
        bool operator==( const Object_Color& other )const;
        bool operator!=( const Object_Color& other )const;
        virtual Object_Color* dup( void )const;
        
        virtual void from_xml( const std::string& file );
        virtual void from_xml( xmlNodePtr root );
        virtual void to_xml( const std::string& file )const;
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        inline std::string& object_color_type( void ){ return get_prop< std::string >( _properties, "object_color_type" ); };
        inline const std::string& object_color_type( void )const{ return get_prop< std::string >( _properties, "object_color_type" ); };
        
    protected:
        /*Type _type;*/

    private:
        
    };
    
    /** 
     * Object_Color class ostream operator
     */
    std::ostream& operator<<( std::ostream& out, const Object_Color& other );
}

#endif /* H2SL_OBJECT_COLOR_H */
