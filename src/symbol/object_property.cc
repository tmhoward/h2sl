/**
 * @file object_property.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/object_property.h"

using namespace std;
using namespace h2sl;

/**
 * Object_Property class constructor
 */
Object_Property::
Object_Property( const string& objectType,
                  const string& spatial_relationType,
                  const string& indexType ) : Grounding() {
    insert_prop< std::string >( _properties, "object_type", objectType );
    insert_prop< std::string >( _properties, "spatial_relation_type", spatial_relationType );
    insert_prop< std::string >( _properties, "index_type", indexType );

}

Object_Property::
Object_Property( xmlNodePtr root ) : Grounding() {
    insert_prop< std::string >( _properties, "object_type", "na" );
    insert_prop< std::string >( _properties, "spatial_relation_type", "na" );
    insert_prop< std::string >( _properties, "index_type", "na" );
    from_xml( root );
}


/**
 * Object_Property class copy constructor
 */
Object_Property::
Object_Property( const Object_Property& other ) : Grounding( other ) {

}

/**
 * Object_Property class destructor
 */
Object_Property::
~Object_Property() {

}

/** 
 * Object_Property class assignment operator
 */
Object_Property&
Object_Property::
operator=( const Object_Property& other ){
  _properties = other._properties;
  return (*this);
}

/** 
 * Object_Property class equality operator
 */
bool
Object_Property::
operator==( const Object_Property& other )const{
  if ( object_type() != other.object_type() ){
    return false;
  } else if ( spatial_relation_type() != other.spatial_relation_type() ){
    return false;
  } else if ( index_type() != other.index_type() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Object_Property class inequality operator
 */
bool
Object_Property::
operator!=( const Object_Property& other )const{
  return !( *this == other );
}

/** 
 * Object_Property class dup operator
 */
Object_Property*
Object_Property::
dup( void )const{
  return new Object_Property( *this );
}

/** 
 * imports the Object_Property class from an XML file
 */
void
Object_Property::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "object_property" ) ) == 0 ){
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
 * imports the Object_Property class from an XML node pointer
 */
void
Object_Property::
from_xml( xmlNodePtr root ){
  object_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > object_type_prop = has_prop< std::string >( root, "object_type" );
    if( object_type_prop.first ){
      object_type() = object_type_prop.second;
    }
    pair< bool, string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ){
      spatial_relation_type() = spatial_relation_type_prop.second;
    }
    pair< bool, string > index_type_prop = has_prop< std::string >( root, "index_type" );
    if( index_type_prop.first ){
      index_type() = index_type_prop.second;
    }
  }
  return;
}

/**
 * exports the Object_Property class to an XML file
 */
void
Object_Property::
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
 * exports the Object_Property class to an XML node pointer
 */
void
Object_Property::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "object_property" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "object_type").c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "spatial_relation_type").c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "index_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "index_type").c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Object_Property class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Object_Property& other ){
    out << "Object_Property(";
    out << "object_type=\"" << other.object_type() << "\",";
    out << "spatial_relation_type=\"" << other.spatial_relation_type() << "\",";
    out << "index_type=\"" << other.index_type() << "\",";
    out << ")";
    return out;
  }
}
