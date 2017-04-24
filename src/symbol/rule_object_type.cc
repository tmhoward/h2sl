/**
 * @file rule_object_type.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/rule_object_type.h"

using namespace std;
using namespace h2sl;

/**
 * Rule_Object_Type class constructor
 */
Rule_Object_Type::
Rule_Object_Type( const string& objectType ) : Rule() {
  insert_prop< std::string >( _string_properties, "object_type", objectType );
}

Rule_Object_Type::
Rule_Object_Type( xmlNodePtr root,
                  World* world ) : Rule() {
  insert_prop< std::string >( _string_properties, "object_type", "na" );
  from_xml( root, world );
}

/**
 * Rule_Object_Type class copy constructor
 */
Rule_Object_Type::
Rule_Object_Type( const Rule_Object_Type& other ) : Rule( other ) {

}

/**
 * Rule_Object_Type class destructor
 */
Rule_Object_Type::
~Rule_Object_Type() {

}

/** 
 * Rule_Object_Type class assignment operator
 */
Rule_Object_Type&
Rule_Object_Type::
operator=( const Rule_Object_Type& other ){
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/** 
 * Rule_Object_Type class equality operator
 */
bool
Rule_Object_Type::
operator==( const Rule_Object_Type& other )const{
  if ( object_type() != other.object_type() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Rule_Object_Type class inequality operator
 */
bool
Rule_Object_Type::
operator!=( const Rule_Object_Type& other )const{
  return !( *this == other );
}

/** 
 * Rule_Object_Type class dup operator
 */
Rule_Object_Type*
Rule_Object_Type::
dup( void )const{
  return new Rule_Object_Type( *this );
}

string
Rule_Object_Type::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Rule_Object_Type* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Rule_Object_Type* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Rule_Object_Type::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "object_type", object_type(), stringTypes );
  return;
}

void
Rule_Object_Type::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Rule_Object_Type::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const std::string& symbolType ){

  if( symbolDictionary.has_class_name( class_name() ) || symbolDictionary.has_class_name( "object_type" ) || symbolDictionary.has_class_name( "object" ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "binary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< string > >::const_iterator it_object_type_types = symbolDictionary.string_types().find( "object_type" );

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( it_object_type_types != symbolDictionary.string_types().end() ){
        for( unsigned int i = 0; i < it_object_type_types->second.size(); i++ ){
          it_search_spaces_symbol->second.second.push_back( new Rule_Object_Type( it_object_type_types->second[ i ] ) );
        }
      }
    }
  }

  return;
}

void
Rule_Object_Type::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Object_Type rule_object_type( object_type() );
  insert_unique_grounding< Rule_Object_Type >( groundingSet, rule_object_type );
  return;
}

bool
Rule_Object_Type::
equals( const Grounding& other )const{
  if( dynamic_cast< const Rule_Object_Type* >( &other ) != NULL ){
    return ( *this == *static_cast< const Rule_Object_Type* >( &other ) );
  } else {
    return false;
  }
}

/** 
 * imports the Rule_Object_Type class from an XML file
 */
void
Rule_Object_Type::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_object_type" ) ) == 0 ){
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
 * imports the Rule_Object_Type class from an XML node pointer
 */
void
Rule_Object_Type::
from_xml( xmlNodePtr root, World* world ){
  object_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > object_type_keys = { "type", "object_type" };
    assert( check_keys( root, object_type_keys ) );
    pair< bool, string > object_type_prop = has_prop< std::string >( root, "object_type" );
    if( object_type_prop.first ){
      object_type() = object_type_prop.second;
    }
    object_type_prop = has_prop< std::string >( root, "type" );
    if( object_type_prop.first ){
      object_type() = object_type_prop.second;
    }
  }
  return;
}

/**
 * exports the Rule_Object_Type class to an XML file
 */
void
Rule_Object_Type::
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
 * exports the Rule_Object_Type class to an XML node pointer
 */
void
Rule_Object_Type::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "rule_object_type" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "object_type" ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Rule_Object_Type::
to_latex( void )const{
  stringstream tmp;
  tmp << "Rule_Object_Type(object_type=" << object_type() << ")";
  return tmp.str();
}



namespace h2sl {
  /** 
   * Rule_Object_Type class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Rule_Object_Type& other ){
    out << "Rule_Object_Type(";
    out << "object_type=\"" << other.object_type() << "\"";
    out << ")";
    return out;
  }
}
