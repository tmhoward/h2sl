/**
 * @file object_type.h
 *
 * @brief
 *
 * a class that describes the object_type symbol.
 */

#ifndef H2SL_OBJECT_COLOR_H
#define H2SL_OBJECT_COLOR_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/grounding.h"
#include "h2sl/common.h"

namespace h2sl {
    /**
     * Object_Type class definition
     */
    class Object_Type : public Grounding {
    public:
        Object_Type( const std::string& objectType = "na" );
        Object_Type( const Object_Type& other );
        Object_Type( xmlNodePtr root );
        virtual ~Object_Type();
        Object_Type& operator=( const Object_Type& other );
        bool operator==( const Object_Type& other )const;
        bool operator!=( const Object_Type& other )const;
        virtual Object_Type* dup( void )const;
        
        virtual void from_xml( const std::string& file );
        virtual void from_xml( xmlNodePtr root );
        virtual void to_xml( const std::string& file )const;
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        inline std::string& object_type( void ){ return get_prop< std::string >( _properties, "object_type" ); };
        inline const std::string& object_type( void )const{ return get_prop< std::string >( _properties, "object_type" ); };
        
    protected:

    private:
        
    };
    
    /** 
     * Object_Type class ostream operator
     */
    std::ostream& operator<<( std::ostream& out, const Object_Type& other );
}

#endif /* H2SL_OBJECT_TYPE_H */
