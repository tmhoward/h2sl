/**
 * @file spatial_relation.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/spatial_relation.h"

using namespace std;
using namespace h2sl;

/**
 * Spatial_Relation class constructor
 */
Spatial_Relation::
Spatial_Relation( const string& spatial_relationType ) : Grounding() {
  insert_prop< std::string >( _properties, "spatial_relation_type", spatial_relationType );
}

Spatial_Relation::
Spatial_Relation( xmlNodePtr root ) : Grounding() {
    insert_prop< std::string >( _properties, "spatial_relation_type", "na" );
    from_xml( root );
}

/**
 * Spatial_Relation class copy constructor
 */
Spatial_Relation::
Spatial_Relation( const Spatial_Relation& other ) : Grounding( other ) {

}

/**
 * Spatial_Relation class destructor
 */
Spatial_Relation::
~Spatial_Relation() {

}

/** 
 * Spatial_Relation class assignment operator
 */
Spatial_Relation&
Spatial_Relation::
operator=( const Spatial_Relation& other ){
  _properties = other._properties;
  return (*this);
}

/** 
 * Spatial_Relation class equality operator
 */
bool
Spatial_Relation::
operator==( const Spatial_Relation& other )const{
  if ( relation_type() != other.relation_type() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Spatial_Relation class inequality operator
 */
bool
Spatial_Relation::
operator!=( const Spatial_Relation& other )const{
  return !( *this == other );
}

/** 
 * Spatial_Relation class dup operator
 */
Spatial_Relation*
Spatial_Relation::
dup( void )const{
  return new Spatial_Relation( *this );
}

/** 
 * imports the Spatial_Relation class from an XML file
 */
void
Spatial_Relation::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "spatial_relation" ) ) == 0 ){
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
 * imports the Spatial_Relation class from an XML node pointer
 */
void
Spatial_Relation::
from_xml( xmlNodePtr root ){
  relation_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ){
      relation_type() = spatial_relation_type_prop.second;
    }
  }
  return;
}

/**
 * exports the Spatial_Relation class to an XML file
 */
void
Spatial_Relation::
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
 * exports the Spatial_Relation class to an XML node pointer
 */
void
Spatial_Relation::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "spatial_relation" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "spatial_relation_type").c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Spatial_Relation class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Spatial_Relation& other ){
    out << "Spatial_Relation(";
    out << "spatial_relation_type=\"" << other.relation_type() << "\",";
    out << ")";
    return out;
  }
}
