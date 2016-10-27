/**
 * @file region_abstract_container.cc
 * 
 * @brief
 *
 * a class that describes the Region_Abstract_Container class.
 */

#include <sstream>
#include "h2sl_nsf_nri_mvli/region_abstract_container.h"

using namespace std;
using namespace h2sl_nsf_nri_mvli;

/**
 * Region_Abstract_Container class constructor. 
 */
Region_Abstract_Container::
Region_Abstract_Container( const unsigned int& type,
        const Abstract_Container& abstract_container ) : Grounding(),
				  _type( type ),
				  _abstract_container( abstract_container ) {

}

/**
 * Region_Abstract_Container class destructor. 
 */
Region_Abstract_Container::
~Region_Abstract_Container() {

}

/**
 * Region_Abstract_Container class copy constructor. 
 */
Region_Abstract_Container::
Region_Abstract_Container( const Region_Abstract_Container& other ) : Grounding( other ),
				_type( other._type ),
				_abstract_container( other._abstract_container ) {

}

/**
 * Region_Abstract_Container class assingnment operator. 
 */
Region_Abstract_Container&
Region_Abstract_Container::
operator=( const Region_Abstract_Container& other ) {
  _type = other._type;
  _abstract_container = other._abstract_container;
  return (*this);
}

/**
 * Region_Abstract_Container class equality operator. 
 */
bool
Region_Abstract_Container::
operator==( const Region_Abstract_Container& other )const{
  if ( _type != other._type ) {
    return false;
  } else if ( _abstract_container != other._abstract_container ) {
    return false;
  } else {
    return true;
  }
}

/**
 * Region_Abstract_Container class inequality operator. 
 */
bool
Region_Abstract_Container::
operator!=( const Region_Abstract_Container& other )const{
  return !( *this == other );
}

/**
 * Region_Abstract_Container class dup operator. 
 */
Region_Abstract_Container*
Region_Abstract_Container::
dup( void )const{
  return new Region_Abstract_Container( *this );
}

/**
 * exports the Region_Abstract_Container class to an XML file. 
 */
void
Region_Abstract_Container::
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
 * exports the Region_Abstract_Container class to an XML node pointer. 
 */
void
Region_Abstract_Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "region_abstract_container" ), NULL );
  stringstream type_string;
  type_string << type_to_std_string( (Type) _type );
  xmlNewProp( node, ( const xmlChar* )( "type" ), ( const xmlChar* )( type_string.str().c_str() ) );
  _abstract_container.to_xml( doc, node );
  xmlAddChild( root, node );
  return;
}

/**
 * imports the Region_Abstract_Container class from an XML file. 
 */
void
Region_Abstract_Container::
from_xml( const string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  root = xmlDocGetRootElement( doc );
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "region_abstract_container" ) ) == 0 ){
          from_xml( l1 );
        }
      }
    }
  }
  xmlFreeDoc( doc );
  return;
}

/**
 * imports the Region_Abstract_Container class from an XML node pointer. 
 */
void
Region_Abstract_Container::
from_xml( xmlNodePtr root ){
  _type = Type::REGION_ABSTRACT_CONTAINER_TYPE_UNKNOWN;
  _abstract_container = Abstract_Container();

  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "type" ) );
    if( tmp != NULL ){
      string type_string = ( char* )( tmp );
      _type = Region_Abstract_Container::type_from_std_string( type_string );
      xmlFree( tmp );
    }
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "abstract_container") ) == 0 ){
          _abstract_container.from_xml( l1 );
        }
      }
    }
  }
  return;
}

/**
 * Converts Region_Abstract_Container::Type to std::string
 */
string
Region_Abstract_Container::
type_to_std_string( const Type type ){
  switch( type ){
  case( REGION_ABSTRACT_CONTAINER_TYPE_FRONT ):
    return "front";
    break;
  case( REGION_ABSTRACT_CONTAINER_TYPE_BACK ):
    return "back";
    break;
  case( REGION_ABSTRACT_CONTAINER_TYPE_LEFT ):
    return "left";
    break;
  case( REGION_ABSTRACT_CONTAINER_TYPE_RIGHT ):
    return "right";
    break;
  case( REGION_ABSTRACT_CONTAINER_TYPE_NEAR ):
    return "near";
    break;
  case( REGION_ABSTRACT_CONTAINER_TYPE_FAR ):
    return "far";
    break;
  case( REGION_ABSTRACT_CONTAINER_TYPE_CENTER ):
    return "center";
    break;
  case( REGION_ABSTRACT_CONTAINER_TYPE_UNKNOWN ):
    return "na";
    break;
  case( REGION_ABSTRACT_CONTAINER_TYPE_NUM_TYPES ):
  default:
    return "na"; 
  }
}

/**
 * Converts string to Region_Abstract_Container::Type
 */

Region_Abstract_Container::Type
Region_Abstract_Container::
type_from_std_string( const std::string& arg ){
  for( unsigned int i = 0; i < Type::REGION_ABSTRACT_CONTAINER_TYPE_NUM_TYPES; i++ ){
    if( type_to_std_string( ( Type )( i ) ) == arg ){
      return ( Type )( i );
    }
  }
  return Type::REGION_ABSTRACT_CONTAINER_TYPE_UNKNOWN;
}

namespace h2sl_nsf_nri_mvli {

  /**
   * Region_Abstract_Container class ostream operator
   */

  ostream&
  operator<<( ostream& out,
              const Region_Abstract_Container& other ){
    out << "class:\"Region_Abstract_Container\" ";
    out << "type:\"" << Region_Abstract_Container::type_to_std_string( (Region_Abstract_Container::Type) other.type() ) << "\"";
    out << "abstract_container:{";
    out << other.abstract_container();
    out << "} ";
    return out;
  }
}


