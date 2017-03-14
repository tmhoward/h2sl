/**
 * @file rule_object_color.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/rule_object_color.h"

using namespace std;
using namespace h2sl;

/**
 * Rule_Object_Color class constructor
 */
Rule_Object_Color::
Rule_Object_Color( const string& objectType ) : Rule() {
  insert_prop< std::string >( _string_properties, "object_color", objectType );
}

Rule_Object_Color::
Rule_Object_Color( xmlNodePtr root ) : Rule() {
  insert_prop< std::string >( _string_properties, "object_color", "na" );
  from_xml( root );
}

/**
 * Rule_Object_Color class copy constructor
 */
Rule_Object_Color::
Rule_Object_Color( const Rule_Object_Color& other ) : Rule( other ) {

}

/**
 * Rule_Object_Color class destructor
 */
Rule_Object_Color::
~Rule_Object_Color() {

}

/** 
 * Rule_Object_Color class assignment operator
 */
Rule_Object_Color&
Rule_Object_Color::
operator=( const Rule_Object_Color& other ){
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/** 
 * Rule_Object_Color class equality operator
 */
bool
Rule_Object_Color::
operator==( const Rule_Object_Color& other )const{
  if ( object_color() != other.object_color() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Rule_Object_Color class inequality operator
 */
bool
Rule_Object_Color::
operator!=( const Rule_Object_Color& other )const{
  return !( *this == other );
}

/** 
 * Rule_Object_Color class dup operator
 */
Rule_Object_Color*
Rule_Object_Color::
dup( void )const{
  return new Rule_Object_Color( *this );
}

void
Rule_Object_Color::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "object_color", object_color(), stringTypes );
  return;
}

void
Rule_Object_Color::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Rule_Object_Color::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const symbol_type_t& symbolType ){

  if( symbolDictionary.has_class_name( "object_color" ) || symbolDictionary.has_class_name( "object" ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "binary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< string > >::const_iterator it_object_color_types = symbolDictionary.string_types().find( "object_color" );

    switch( symbolType ){
    case( SYMBOL_TYPE_CONCRETE ):
    case( SYMBOL_TYPE_ALL ):
      if( it_object_color_types != symbolDictionary.string_types().end() ){
        for( unsigned int i = 0; i < it_object_color_types->second.size(); i++ ){
          it_search_spaces_symbol->second.second.push_back( new Rule_Object_Color( it_object_color_types->second[ i ] ) );
        }
      }
      break;
    case( SYMBOL_TYPE_ABSTRACT ):
    case( NUM_SYMBOL_TYPES ):
    default:
      break;
    }
  }

  return;
}

/** 
 * imports the Rule_Object_Color class from an XML file
 */
void
Rule_Object_Color::
from_xml( const string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_object_color" ) ) == 0 ){
            from_xml( l1 );
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
 * imports the Rule_Object_Color class from an XML node pointer
 */
void
Rule_Object_Color::
from_xml( xmlNodePtr root ){
  object_color() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > object_color_keys = { "type", "object_color" };
    assert( check_keys( root, object_color_keys ) );
    pair< bool, string > object_color_prop = has_prop< std::string >( root, "object_color" );
    if( object_color_prop.first ){
      object_color() = object_color_prop.second;
    }
    object_color_prop = has_prop< std::string >( root, "type" );
    if( object_color_prop.first ){
      object_color() = object_color_prop.second;
    }
  }
  return;
}

/**
 * exports the Rule_Object_Color class to an XML file
 */
void
Rule_Object_Color::
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
 * exports the Rule_Object_Color class to an XML node pointer
 */
void
Rule_Object_Color::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "rule_object_color" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "object_color" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "object_color" ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Rule_Object_Color class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Rule_Object_Color& other ){
    out << "Rule_Object_Color(";
    out << "object_color=\"" << other.object_color() << "\"";
    out << ")";
    return out;
  }
}
