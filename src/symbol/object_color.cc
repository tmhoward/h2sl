/**
 * @file object_color.cc
 *
 * @brief
 *
 * a class that describes a object_color
 */

#include "h2sl/rule_object_color.h"
#include "h2sl/object_color.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Object_Color class constructor
 */
Object_Color::
Object_Color( const string& object_colorType ) : Grounding() {
  insert_prop< std::string >( _string_properties, "object_color_type", object_colorType );
}


Object_Color::
Object_Color( xmlNodePtr root, World* world ) : Grounding() {
    insert_prop< std::string >( _string_properties, "object_color_type", "na" );
    from_xml( root, world );
}

/**
 * Object_Color class copy constructor
 */
Object_Color::
Object_Color( const Object_Color& other ) : Grounding( other ) {

}

/**
 * Object_Color class destructor
 */
Object_Color::
~Object_Color() {

}

/** 
 * Object_Color class assignment operator
 */
Object_Color&
Object_Color::
operator=( const Object_Color& other ){
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/** 
 * Object_Color class equality operator
 */
bool
Object_Color::
operator==( const Object_Color& other )const{
  if ( object_color_type() != other.object_color_type() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Object_Color class inequality operator
 */
bool
Object_Color::
operator!=( const Object_Color& other )const{
  return !( *this == other );
}

/** 
 * Object_Color class dup operator
 */
Object_Color*
Object_Color::
dup( void )const{
  return new Object_Color( *this );
}

string
Object_Color::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Object_Color* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Object_Color* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Object_Color::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "object_color", object_color_type(), stringTypes );
  return;
}

void
Object_Color::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Object_Color::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const std::string& symbolType ){
  if( symbolDictionary.has_class_name( class_name() ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "binary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< string > >::const_iterator it_object_color_types = symbolDictionary.string_types().find( "object_color" );

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( it_object_color_types != symbolDictionary.string_types().end() ){
        for( unsigned int i = 0; i < it_object_color_types->second.size(); i++ ){
          it_search_spaces_symbol->second.second.push_back( new Object_Color( it_object_color_types->second[ i ] ) );
        }
      }
    } 
  }

  return;
}

void
Object_Color::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Object_Color rule_object_color( object_color_type() );
  insert_unique_grounding< Rule_Object_Color >( groundingSet, rule_object_color );
  return;
}

bool
Object_Color::
equals( const Grounding& other )const{
  if( dynamic_cast< const Object_Color* >( &other ) != NULL ){
    return ( *this == *static_cast< const Object_Color* >( &other ) );
  } else {
    return false;
  }
}

/** 
 * imports the Object_Color class from an XML file
 */
void
Object_Color::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "object_color" ) ) == 0 ){
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
 * imports the Object_Color class from an XML node pointer
 */
void
Object_Color::
from_xml( xmlNodePtr root, World* world ){
  object_color_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
      pair< bool, string > object_color_type_prop = has_prop< std::string >( root, "object_color_type" );
      if( object_color_type_prop.first ){
        object_color_type() = object_color_type_prop.second;
      }
  }
  return;
}

/**
 * exports the Object_Color class to an XML file
 */
void
Object_Color::
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
 * exports the Object_Color class to an XML node pointer
 */
void
Object_Color::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "object_color" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "object_color_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "object_color_type").c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Object_Color::
to_latex( void )const{
  stringstream tmp;
  tmp << "Object\\_Color(" << object_color_type() << ")";
  return tmp.str();
}

namespace h2sl {
  /** 
   * Object_Color class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Object_Color& other ){
    out << "Object_Color(";
    out << "object_color_type=\"" << other.object_color_type() << "\",";
    out << ")";  
    return out;
  }
}
