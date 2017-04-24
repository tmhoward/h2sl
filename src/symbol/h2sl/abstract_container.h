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
    Abstract_Container( const std::string& objectType = "na",
                        const int& number = 0,
                        const int& index = 0,
                        const std::string& colorType = "na" );                   

    Abstract_Container( const Abstract_Container& other );
    Abstract_Container( xmlNodePtr root, World* world );
    virtual ~Abstract_Container();
    Abstract_Container& operator=( const Abstract_Container& other );
    bool operator==( const Abstract_Container& other )const;
    bool operator!=( const Abstract_Container& other )const;
    Abstract_Container* dup( void )const;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "abstract_container" ); };
    virtual void scrape_grounding( const World * world, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    virtual void scrape_grounding( const World * world, std::vector< std::string >& classNames, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    static void fill_search_space( const Symbol_Dictionary& symbolDictionary, const World* world, std::map< std::string, std::pair< std::string, std::vector< Grounding* > > >& searchSpaces, const std::string& symbolType );
    virtual void fill_rules( const World* world, Grounding_Set* groundingSet )const;
    virtual bool equals( const Grounding& other )const;

    inline std::string& type( void ){
              return get_prop< std::string >( _string_properties, "object_type" ); };
    inline const std::string& type( void )const{
              return get_prop< std::string >( _string_properties, "object_type" ); };
   
    inline int& number( void ){ 
              return get_prop< int >( _int_properties, "number" ); };
    inline const int& number( void )const{ 
              return get_prop< int >( _int_properties, "number" ); };
 
    inline int& index( void ){ 
              return get_prop< int >( _int_properties, "index" ); };
    inline const int& index( void )const{ 
              return get_prop< int >( _int_properties, "index" ); };
 
    inline std::string& color( void ){
              return get_prop< std::string >( _string_properties, "object_color" ); };
    inline const std::string& color( void )const{
              return get_prop< std::string >( _string_properties, "object_color" ); };
    
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual std::string to_latex( void )const;

    virtual void from_xml( const std::string& file, World* world );
    virtual void from_xml( xmlNodePtr root, World* world );

    static std::string class_name( void ){ return "abstract_container"; };

  protected:

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Abstract_Container& other );
}

#endif /* H2SL_ABSTRACT_CONTAINER_H */

