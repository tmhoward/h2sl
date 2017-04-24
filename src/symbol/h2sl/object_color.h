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



namespace h2sl {
    /**
     * Object_Color class definition
     */
    class Object_Color : public Grounding {
    public:
        Object_Color( const std::string& object_colorType = "na" );
        Object_Color( const Object_Color& other );
        Object_Color( xmlNodePtr root, World* world );
        virtual ~Object_Color();
        Object_Color& operator=( const Object_Color& other );
        bool operator==( const Object_Color& other )const;
        bool operator!=( const Object_Color& other )const;
        virtual Object_Color* dup( void )const;
       
        virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
        virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "object_color" ); };
        virtual void scrape_grounding( const World * world, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
        virtual void scrape_grounding( const World * world, std::vector< std::string >& classNames, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
        static void fill_search_space( const Symbol_Dictionary& symbolDictionary, const World* world, std::map< std::string, std::pair< std::string, std::vector< Grounding* > > >& searchSpaces, const std::string& symbolType ); 
        virtual void fill_rules( const World* world, Grounding_Set* groundingSet )const;
        virtual bool equals( const Grounding& other )const;    
 
        virtual void from_xml( const std::string& file, World* world );
        virtual void from_xml( xmlNodePtr root, World* world );
        virtual void to_xml( const std::string& file )const;
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
       
        virtual std::string to_latex( void )const;
 
        inline std::string& object_color_type( void ){ return get_prop< std::string >( _string_properties, "object_color_type" ); };
        inline const std::string& object_color_type( void )const{ return get_prop< std::string >( _string_properties, "object_color_type" ); };

        static std::string class_name( void ){ return "object_color"; };
        
    protected:

    private:
        
    };
    
    /** 
     * Object_Color class ostream operator
     */
    std::ostream& operator<<( std::ostream& out, const Object_Color& other );
}

#endif /* H2SL_OBJECT_COLOR_H */
