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
  if ( spatial_relation_type() != other.spatial_relation_type() ){
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
 * imports the Spatial_Relation class from a LCM msg
 */
/*bool
Spatial_Relation::
from_msg( const spatial_relation_msg_t * msg ){
  return true;
}*/

/**
 * exports the Spatial_Relation class to a LCM msg
 */
/*void
Spatial_Relation::
to_msg( spatial_relation_msg_t * msg )const{
  return;
}*/

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
  spatial_relation_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ){
      spatial_relation_type() = spatial_relation_type_prop.second;
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

/*string
Spatial_Relation::
type_to_std_string( const Type& type ){
  switch( type ){
  case( TYPE_FRONT ):
    return "front";
    break;
  case( TYPE_BACK ):
    return "back";
    break;
  case( TYPE_RIGHT ):
    return "right";
    break;
  case( TYPE_LEFT ):
    return "left";
    break;
  case( TYPE_CENTER ):
    return "center";
    break;
  case( TYPE_SIDE ):
    return "side";
    break;
  case( TYPE_NEAR ):
    return "near";
    break;
  case( TYPE_FAR ):
    return "far";
    break;
  case( TYPE_UNKNOWN ):
    return "na";
    break;
  default:
    assert( false );
    return "na";
  }
}

Spatial_Relation::Type
Spatial_Relation::
type_from_std_string( const string& arg ){
  for( unsigned int i = 0; i < Type::TYPE_NUM_TYPES; i++ ){
    if( type_to_std_string( ( Type ) ( i ) ) == arg ){
      return ( Type )( i );
    }
  }
  cout << "arg:\"" << arg << "\"" << endl;
  assert( false );
  return Type::TYPE_UNKNOWN;
}*/

namespace h2sl {
  /** 
   * Spatial_Relation class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Spatial_Relation& other ){
    out << "class:\"Spatial_Relation\" ";
    out << "spatial_relation_type:" << other.spatial_relation_type() << " ";
    return out;
  }
}
