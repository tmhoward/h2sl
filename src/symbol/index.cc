/**
 * @file index.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/rule_index.h"
#include "h2sl/index.h"

using namespace std;
using namespace h2sl;

/**
 * Index class constructor
 */
Index::
Index( const int& value ) : Grounding() {
  insert_prop< int >( _int_properties, "value", value );
}

Index::
Index( xmlNodePtr root, World* world ) : Grounding() {
  insert_prop< int >( _int_properties, "value", 0 );
  from_xml( root, world );
}


/**
 * Index class copy constructor
 */
Index::
Index( const Index& other ) : Grounding( other ) {

}

/**
 * Index class destructor
 */
Index::
~Index() {

}

/** 
 * Index class assignment operator
 */
Index&
Index::
operator=( const Index& other ){
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/** 
 * Index class equality operator
 */
bool
Index::
operator==( const Index& other )const{
  if ( value() != other.value() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Index class inequality operator
 */
bool
Index::
operator!=( const Index& other )const{
  return !( *this == other );
}

/** 
 * Index class dup operator
 */
Index*
Index::
dup( void )const{
  return new Index( *this );
}

string
Index::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Index* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Index* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Index::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< int >( "index", value(), intTypes );
  return;
}

void
Index::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Index::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const std::string& symbolType ){

  if( symbolDictionary.has_class_name( class_name() ) || symbolDictionary.has_class_name( "abstract_container" ) || symbolDictionary.has_class_name( "region_abstract_container" ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "binary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< int > >::const_iterator it_index_value_types = symbolDictionary.int_types().find( "index" );

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( it_index_value_types != symbolDictionary.int_types().end() ){
        for( unsigned int i = 0; i < it_index_value_types->second.size(); i++ ){
          it_search_spaces_symbol->second.second.push_back( new Index( it_index_value_types->second[ i ] ) );
        }
      }
    }
  }

  return;
}

void
Index::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Index rule_index( value() );
  insert_unique_grounding< Rule_Index >( groundingSet, rule_index );
  return;
}

bool
Index::
equals( const Grounding& other )const{
  if( dynamic_cast< const Index* >( &other ) != NULL ){
    return ( *this == *static_cast< const Index* >( &other ) );
  } else {
    return false;
  }
}

/** 
 * imports the Index class from an XML file
 */
void
Index::
from_xml( const string& filename, World* world ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "index" ) ) == 0 ){
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
 * imports the Index class from an XML node pointer
 */
void
Index::
from_xml( xmlNodePtr root, World* world ){
  value() = 0;
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > index_keys = { "value", "type" };
    assert( check_keys( root, index_keys ) );
    pair< bool, int > value_prop = has_prop< int >( root, "value" );
    if( value_prop.first ){
      value() = value_prop.second;
    }
    value_prop = has_prop< int >( root, "type" );
    if( value_prop.first ){
      value() = value_prop.second;
    }
  }
  return;
}

/**
 * exports the Index class to an XML file
 */
void
Index::
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
 * exports the Index class to an XML node pointer
 */
void
Index::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "index" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "value" ), ( const xmlChar* )( to_std_string( get_prop< int >( _int_properties, "value" ) ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Index::
to_latex( void )const{
  stringstream tmp;
  tmp << "Index(" << value() << ")";
  return tmp.str();
}

namespace h2sl {
  /** 
   * Index class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Index& other ){
    out << "Index(";
    out << "value=\"" << other.value() << "\"";
    out << ")";
    return out;
  }
}
