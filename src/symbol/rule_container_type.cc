/**
 * @file rule_container_type.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/rule_container_type.h"

using namespace std;
using namespace h2sl;

/**
 * Rule_Container_Type class constructor
 */
Rule_Container_Type::
Rule_Container_Type( const string& containerType ) : Rule() {
  insert_prop< std::string >( _string_properties, "container_type", containerType );
}

Rule_Container_Type::
Rule_Container_Type( xmlNodePtr root, World* world ) : Rule() {
  insert_prop< std::string >( _string_properties, "container_type", "na" );
  from_xml( root, world );
}

/**
 * Rule_Container_Type class copy constructor
 */
Rule_Container_Type::
Rule_Container_Type( const Rule_Container_Type& other ) : Rule( other ) {

}

/**
 * Rule_Container_Type class destructor
 */
Rule_Container_Type::
~Rule_Container_Type() {

}

/** 
 * Rule_Container_Type class assignment operator
 */
Rule_Container_Type&
Rule_Container_Type::
operator=( const Rule_Container_Type& other ){
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/** 
 * Rule_Container_Type class equality operator
 */
bool
Rule_Container_Type::
operator==( const Rule_Container_Type& other )const{
  if ( container_type() != other.container_type() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Rule_Container_Type class inequality operator
 */
bool
Rule_Container_Type::
operator!=( const Rule_Container_Type& other )const{
  return !( *this == other );
}

/** 
 * Rule_Container_Type class dup operator
 */
Rule_Container_Type*
Rule_Container_Type::
dup( void )const{
  return new Rule_Container_Type( *this );
}

string
Rule_Container_Type::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Rule_Container_Type* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Rule_Container_Type* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Rule_Container_Type::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "container_type", container_type(), stringTypes );
  return;
}

void
Rule_Container_Type::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Rule_Container_Type::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const std::string& symbolType ){

  if( symbolDictionary.has_class_name( class_name() ) || symbolDictionary.has_class_name( "container_type" ) || symbolDictionary.has_class_name( "container" ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "binary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< string > >::const_iterator it_container_type_types = symbolDictionary.string_types().find( "container_type" );

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( it_container_type_types != symbolDictionary.string_types().end() ){
        for( unsigned int i = 0; i < it_container_type_types->second.size(); i++ ){
          it_search_spaces_symbol->second.second.push_back( new Rule_Container_Type( it_container_type_types->second[ i ] ) );
        }
      }
    }
  }

  return;
}

void
Rule_Container_Type::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Container_Type rule_container_type( container_type() );
  insert_unique_grounding< Rule_Container_Type >( groundingSet, rule_container_type );
  return;
}

bool
Rule_Container_Type::
equals( const Grounding& other )const{
  if( dynamic_cast< const Rule_Container_Type* >( &other ) != NULL ){
    return ( *this == *static_cast< const Rule_Container_Type* >( &other ) );
  } else {
    return false;
  }
}

/** 
 * imports the Rule_Container_Type class from an XML file
 */
void
Rule_Container_Type::
from_xml( const string& filename, World* world ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_container_type" ) ) == 0 ){
            from_xml( l1, world );
          }
        }
      }
      xmlFreeDoc( doc );
    } else {
      xmlFreeDoc( doc );
    }
  }
  return;
}

/** 
 * imports the Rule_Container_Type class from an XML node pointer
 */
void
Rule_Container_Type::
from_xml( xmlNodePtr root, World* world ){
  container_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > container_type_keys = { "type", "container_type" };
    assert( check_keys( root, container_type_keys ) );
    pair< bool, string > container_type_prop = has_prop< std::string >( root, "container_type" );
    if( container_type_prop.first ){
      container_type() = container_type_prop.second;
    }
    container_type_prop = has_prop< std::string >( root, "type" );
    if( container_type_prop.first ){
      container_type() = container_type_prop.second;
    }
  }
  return;
}

/**
 * exports the Rule_Container_Type class to an XML file
 */
void
Rule_Container_Type::
to_xml( const string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

/**
 * exports the Rule_Container_Type class to an XML node pointer
 */
void
Rule_Container_Type::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "rule_container_type" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "container_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "container_type" ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Rule_Container_Type::
to_latex( void )const{
  stringstream tmp;
  tmp << "Rule\\_Container\\_Type(container\\_type=" << container_type() << ")";
  return tmp.str();
}

namespace h2sl {
  /** 
   * Rule_Container_Type class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Rule_Container_Type& other ){
    out << "Rule_Container_Type(";
    out << "container_type=\"" << other.container_type() << "\"";
    out << ")";
    return out;
  }
}
