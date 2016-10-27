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
Index( const string& indexType ) : Grounding() {
  insert_prop< std::string >( _properties, "index_type", indexType );
}

Index::
Index( xmlNodePtr root ) : Grounding() {
    insert_prop< std::string >( _properties, "index_type", "na" );
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
  _properties = other._properties;
  return (*this);
}

/** 
 * Index class equality operator
 */
bool
Index::
operator==( const Index& other )const{
  if ( index_type() != other.index_type() ){
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
      xmlNodePtr l1 = NULL;
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
  index_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > index_type_prop = has_prop< std::string >( root, "index_type" );
    if( index_type_prop.first ){
      index_type() = index_type_prop.second;
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
    xmlNewProp( node, ( const xmlChar* )( "index_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "index_type").c_str() ) );
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
    out << "index_type=\"" << other.index_type() << "\",";
    out << ")";
    return out;
  }
}
