/**
 * @file index.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

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
Index( xmlNodePtr root ) : Grounding() {
  insert_prop< int >( _int_properties, "value", 0 );
  from_xml( root );
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

void
Index::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    vector< pair< unsigned int, Grounding* > >& searchSpaces,
                    const symbol_type_t& symbolType ){

  map< string, vector< int > >::const_iterator it_index_value_types = symbolDictionary.int_types().find( "index_value" );

  switch( symbolType ){
  case( SYMBOL_TYPE_CONCRETE ):
  case( SYMBOL_TYPE_ALL ):
    if( it_index_value_types != symbolDictionary.int_types().end() ){
      for( unsigned int i = 0; i < it_index_value_types->second.size(); i++ ){
        searchSpaces.push_back( pair< unsigned int, Grounding* >( 0, new Index( it_index_value_types->second[ i ] ) ) );
      }
    }
    break;
  case( SYMBOL_TYPE_ABSTRACT ):
  case( NUM_SYMBOL_TYPES ):
  default:
    break;
  }

  return;
}

/** 
 * imports the Index class from an XML file
 */
void
Index::
from_xml( const string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "index" ) ) == 0 ){
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
 * imports the Index class from an XML node pointer
 */
void
Index::
from_xml( xmlNodePtr root ){
  value() = 0;
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, int > value_prop = has_prop< int >( root, "value" );
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
