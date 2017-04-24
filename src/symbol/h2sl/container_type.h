/**
 * @file container_type.h
 *
 * @brief
 *
 * a class that describes the container_type symbol.
 */

#ifndef H2SL_CONTAINER_TYPE_H
#define H2SL_CONTAINER_TYPE_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/grounding.h"
#include "h2sl/common.h"
#include "h2sl/symbol_dictionary.h"

namespace h2sl {
    /**
     * Container_Type class definition
     */
    class Container_Type : public Grounding {
    public:
        Container_Type( const std::string& containerType = "na" );
        Container_Type( const Container_Type& other );
        Container_Type( xmlNodePtr root, World* world );
        virtual ~Container_Type();
        Container_Type& operator=( const Container_Type& other );
        bool operator==( const Container_Type& other )const;
        bool operator!=( const Container_Type& other )const;
        virtual Container_Type* dup( void )const;
        
        virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
        virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "container_type" ); };
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
        
        inline std::string& type( void ){ return get_prop< std::string >( _string_properties, "container_type" ); };
        inline const std::string& type( void )const{ return get_prop< std::string >( _string_properties, "container_type" ); };

        static std::string class_name( void ){ return "container_type"; };
        
    protected:

    private:
        
    };
    
    /** 
     * Container_Type class ostream operator
     */
    std::ostream& operator<<( std::ostream& out, const Container_Type& other );
}

#endif /* H2SL_CONTAINER_TYPE_H */
