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

/*#include "h2sl_nsf_nri_mvli/object_color_msg_t.hpp"*/

namespace h2sl {
    /**
     * Object_Color class definition
     */
    class Object_Color : public Grounding {
    public:
        enum Type {
            TYPE_UNKNOWN,
            TYPE_RED,
            TYPE_BLUE,
            TYPE_YELLOW,
            TYPE_NUM_TYPES
        };
        
        Object_Color( const Type& type = Type::TYPE_UNKNOWN );
        Object_Color( const Object_Color& other );
        virtual ~Object_Color();
        Object_Color& operator=( const Object_Color& other );
        bool operator==( const Object_Color& other )const;
        bool operator!=( const Object_Color& other )const;
        virtual Object_Color* dup( void )const;
        
        /*virtual bool from_msg( const object_color_msg_t * msg );
        virtual void to_msg( object_color_msg_t * msg )const;*/
        
        virtual void from_xml( const std::string& file );
        virtual void from_xml( xmlNodePtr root );
        virtual void to_xml( const std::string& file )const;
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        static std::string type_to_std_string( const Type& type );
        static Type type_from_std_string( const std::string& arg );
        
        inline Type& type( void ){ return _type; };
        inline const Type& type( void )const{ return _type; };
        
    protected:
        Type _type;

    private:
        
    };
    
    /** 
     * Object_Color class ostream operator
     */
    std::ostream& operator<<( std::ostream& out, const Object_Color& other );
}

#endif /* H2SL_OBJECT_COLOR_H */
