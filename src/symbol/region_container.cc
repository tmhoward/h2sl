/**
 * @file region_container.cc
 * 
 * @brief
 *
 * a class that describes the Region_Container class.
 */

#include <sstream>
#include "h2sl/region_container.h"

using namespace std;
using namespace h2sl;

/**
 * Region_Container class constructor. 
 */
Region_Container::
Region_Container( const string& spatial_relationType,
                 const Container& container ) : Grounding() ,
                                                    _container( container ) {
            insert_prop< std::string >( _properties, "spatial_relation_type", spatial_relationType);
}

/**
 * Region_Container class destructor. 
 */
Region_Container::
~Region_Container() {

}

Region_Container::
Region_Container( xmlNodePtr root ) : Grounding(),
                                       _container() {
    insert_prop< std::string >( _properties, "spatial_relation_type", "na" );
    from_xml( root );
}

/**
 * Region_Container class copy constructor. 
 */
Region_Container::
Region_Container( const Region_Container& other ) : Grounding( other ),
                                                        _container( other._container ) {

}

/**
 * Region_Container class assingnment operator. 
 */
Region_Container&
Region_Container::
operator=( const Region_Container& other ) {
  _properties = other._properties;
  _container = other._container;
  return (*this);
}

/**
 * Region_Container class equality operator. 
 */
bool
Region_Container::
operator==( const Region_Container& other )const{
  if ( relation_type() != other.relation_type() ) {
    return false;
  } else if ( _container != other._container ) {
    return false;
  } else {
    return true;
  }
}

/**
 * Region_Container class inequality operator. 
 */
bool
Region_Container::
operator!=( const Region_Container& other )const{
  return !( *this == other );
}

/**
 * Region_Container class dup operator. 
 */
Region_Container*
Region_Container::
dup( void )const{
  return new Region_Container( *this );
}

/**
 * exports the Region_Container class to an XML file. 
 */
void
Region_Container::
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
 * exports the Region_Container class to an XML node pointer. 
 */
void
Region_Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "region_container" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "spatial_relation_type" ).c_str() ) );
  _container.to_xml( doc, node );
  
  xmlAddChild( root, node );
  return;
}

/**
 * imports the Region_Container class from an XML file. 
 */
void
Region_Container::
from_xml( const string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  root = xmlDocGetRootElement( doc );
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "region_container" ) ) == 0 ){
          from_xml( l1 );
        }
      }
    }
  }
  xmlFreeDoc( doc );
  return;
}

/**
 * imports the Region_Container class from an XML node pointer. 
 */
void
Region_Container::
from_xml( xmlNodePtr root ){
  relation_type() = "na";
  _container = Container();

  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ) {
        relation_type() = spatial_relation_type_prop.second;
    }
    for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
            if( xmlStrcmp( l1->name, ( const xmlChar* )( "container" ) ) == 0 ){
                _container.from_xml( l1 );
            }
        }
    }

  }
  return;
}

namespace h2sl {

  /**
   * Region_Container class ostream operator
   */

  ostream&
  operator<<( ostream& out,
              const Region_Container& other ){
    out << "Region_Container(";
    out << "spatial_relation_type=\"" << other.relation_type() << "\",";
      out << "container=" << other.container();
    out << ")";
    return out;
  }
}


