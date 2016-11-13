/**
 * @file region_abstract_container.cc
 * 
 * @brief
 *
 * a class that describes the Region_Abstract_Container class.
 */

#include <sstream>
#include "h2sl/region_abstract_container.h"

using namespace std;
using namespace h2sl;

/**
 * Region_Abstract_Container class constructor. 
 */
Region_Abstract_Container::
Region_Abstract_Container( const string& region_abstract_containerType,
			   const Abstract_Container& abstract_container ) : Grounding(),
                                 _abstract_container ( abstract_container ) {
     insert_prop< std::string >( _properties, "region_abstract_container_type", region_abstract_containerType );
}

/**
 * region abstract_container constructor from xml node ptr
 */

Region_Abstract_Container::
Region_Abstract_Container( xmlNodePtr root ) : Grounding(),
                                               _abstract_container() {
    insert_prop< std::string >( _properties, "region_abstract_container_type", "na" );
    from_xml( root );
}

/**
 * Region_Abstract_Container class copy constructor. 
 */
Region_Abstract_Container::
Region_Abstract_Container( const Region_Abstract_Container& other ) : Grounding( other ),
                                                                      _abstract_container( other._abstract_container ) {

}

/**
 * Region_Abstract_Container class destructor.
 */
Region_Abstract_Container::
~Region_Abstract_Container() {

}

/**
 * Region_Abstract_Container class assingnment operator. 
 */
Region_Abstract_Container&
Region_Abstract_Container::
operator=( const Region_Abstract_Container& other ) {
  _properties = other._properties;
  _abstract_container = other._abstract_container; 
  return (*this);
}

/**
 * Region_Abstract_Container class equality operator. 
 */
bool
Region_Abstract_Container::
operator==( const Region_Abstract_Container& other )const{
  if ( region_abstract_container_type() != other.region_abstract_container_type() ) {
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
  xmlNewProp( node, ( const xmlChar* )( "region_abstract_container_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "region_abstract_container_type" ).c_str() ) );
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
  region_abstract_container_type() = "na";
  _abstract_container = Abstract_Container();

  if( root->type == XML_ELEMENT_NODE ){
      pair< bool, string > region_abstract_container_type_prop = has_prop< std::string >( root, "region_abstract_container_type" );
      if( region_abstract_container_type_prop.first ){
          region_abstract_container_type() = region_abstract_container_type_prop.second;
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

namespace h2sl {

  /**
   * Region_Abstract_Container class ostream operator
   */

  ostream&
  operator<<( ostream& out,
              const Region_Abstract_Container& other ){
    out << "Region_Abstract_Container(";
    out << "region_abstract_container_type=\"" << other.region_abstract_container_type() << "\",";
    out << "abstract_container=" << other.abstract_container();
    out << "} ";
    return out;
  }
}


