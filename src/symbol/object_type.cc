/**
 * @file object_type.cc
 *
 * @brief
 *
 * a class that describes a object_type
 */

#include "h2sl/rule_object_type.h"
#include "h2sl/object_type.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Object_Type class constructor
 */
Object_Type::
Object_Type( const string& objectType ) : Grounding() {
  insert_prop< std::string >( _string_properties, "object_type", objectType );
}


Object_Type::
Object_Type( xmlNodePtr root, World* world ) : Grounding() {
    insert_prop< std::string >( _string_properties, "object_type", "na" );
    from_xml( root, world );
}


/**
 * Object_Type class copy constructor
 */
Object_Type::
Object_Type( const Object_Type& other ) : Grounding( other ) {

}

/**
 * Object_Type class destructor
 */
Object_Type::
~Object_Type() {

}

/** 
 * Object_Type class assignment operator
 */
Object_Type&
Object_Type::
operator=( const Object_Type& other ){
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/** 
 * Object_Type class equality operator
 */
bool
Object_Type::
operator==( const Object_Type& other )const{
  if ( type() != other.type() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Object_Type class inequality operator
 */
bool
Object_Type::
operator!=( const Object_Type& other )const{
  return !( *this == other );
}

/** 
 * Object_Type class dup operator
 */
Object_Type*
Object_Type::
dup( void )const{
  return new Object_Type( *this );
}

string
Object_Type::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Object_Type* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Object_Type* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Object_Type::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "object_type", type(), stringTypes );
  return;
}

void
Object_Type::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Object_Type::
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

    map< string, vector< string > >::const_iterator it_object_type_types = symbolDictionary.string_types().find( "object_type" );

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( it_object_type_types != symbolDictionary.string_types().end() ){
        for( unsigned int i = 0; i < it_object_type_types->second.size(); i++ ){
          it_search_spaces_symbol->second.second.push_back( new Object_Type( it_object_type_types->second[ i ] ) );
        }
      }
    }
  }

  return;
}

void
Object_Type::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Object_Type rule_object_type( type() );
  insert_unique_grounding< Rule_Object_Type >( groundingSet, rule_object_type );
  return;
}

bool
Object_Type::
equals( const Grounding& other )const{
  if( dynamic_cast< const Object_Type* >( &other ) != NULL ){
    return ( *this == *static_cast< const Object_Type* >( &other ) );
  } else {
    return false;
  }
}

/** 
 * imports the Object_Type class from an XML file
 */
void
Object_Type::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "object_type" ) ) == 0 ){
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
 * imports the Object_Type class from an XML node pointer
 */
void
Object_Type::
from_xml( xmlNodePtr root, World* world ){
  type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > object_keys = { "type", "object_type" };
    assert( check_keys( root, object_keys ) );

    pair< bool, string > object_type_prop = has_prop< std::string >( root, "object_type" );
    if( object_type_prop.first ){
      type() = object_type_prop.second;
    }
    object_type_prop = has_prop< std::string >( root, "type" );
    if( object_type_prop.first ){
      type() = object_type_prop.second;
    }
  }
  return;
}

/**
 * exports the Object_Type class to an XML file
 */
void
Object_Type::
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
 * exports the Object_Type class to an XML node pointer
 */
void
Object_Type::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "object_type" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "object_type").c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Object_Type::
to_latex( void )const{
  stringstream tmp;
  tmp << "Object\\_Type(" << type() << ")";
  return tmp.str();
}

namespace h2sl {
  /** 
   * Object_Type class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Object_Type& other ){
    out << "class:\"Object_Type\" ";
    out << "object_type:" << other.type() << " ";
    return out;
  }
}
