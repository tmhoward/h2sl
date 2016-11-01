/**
 * @file feature_region_abstract_container_object_type.cc
 *
 * @brief
 *
 * Class used to describe a feature of the region_abstract_container type
 */
#include <sstream>

#include "h2sl/feature_region_abstract_container_object_type.h"
#include "h2sl/region_abstract_container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Region_Abstract_Container_Object_Type class constructor
 */
Feature_Region_Abstract_Container_Object_Type::
Feature_Region_Abstract_Container_Object_Type( const bool& invert,
                                              const std::string& object_type ) : Feature( invert ),
                                                              _object_type( object_type ) {

}

/**
 * Feature_Region_Abstract_Container_Object_Type class copy constructor
 */
Feature_Region_Abstract_Container_Object_Type::
Feature_Region_Abstract_Container_Object_Type( const Feature_Region_Abstract_Container_Object_Type& other ) : Feature( other ),
                                                              _object_type( other._object_type ){

}

/**
 * Feature_Region_Abstract_Container_Object_Type class destructor
 */
Feature_Region_Abstract_Container_Object_Type::
~Feature_Region_Abstract_Container_Object_Type() {

}

/** 
 * Feature_Region_Abstract_Container_Object_Type class assignment operator
 */
Feature_Region_Abstract_Container_Object_Type&
Feature_Region_Abstract_Container_Object_Type::
operator=( const Feature_Region_Abstract_Container_Object_Type& other ){
  _invert = other._invert;
  _object_type = other._object_type;
  return (*this);
}

/**
 * returns the value of the fature
 */
bool
Feature_Region_Abstract_Container_Object_Type::
value( const unsigned int& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world ){
  const Region_Abstract_Container* region_abstract_container = dynamic_cast< const Region_Abstract_Container* >( grounding );
  if( region_abstract_container != NULL ){
    if( region_abstract_container->abstract_container().type() == _object_type ){
      return !_invert;
    }
  }
  return false;
}

/** 
 * imports the Feature_Region_Abstract_Container_Object_Type class from an XML file
 */
void
Feature_Region_Abstract_Container_Object_Type::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_region_abstract_container_object_type" ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
      xmlFreeDoc( doc );
    }
  }  
  return;
}

/** 
 * imports the Feature_Region_Abstract_Container_Object_Type class from an XML node pointer
 */
void
Feature_Region_Abstract_Container_Object_Type::
from_xml( xmlNodePtr root ){
  _invert = false;
  _object_type = "na";
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "object_type" ) );
    if( tmp != NULL){
      string object_type_string = ( const char* )( tmp );
      _object_type = object_type_string;
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Region_Abstract_Container_Object_Type class to an XML file
 */
void
Feature_Region_Abstract_Container_Object_Type::
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
 * exports the Feature_Region_Abstract_Container_Object_Type class to an XML node pointer
 */
void
Feature_Region_Abstract_Container_Object_Type::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_region_abstract_container_object_type" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( _object_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

std::string&
Feature_Region_Abstract_Container_Object_Type::
object_type( void ){
  return _object_type;
}

const std::string&
Feature_Region_Abstract_Container_Object_Type::
object_type( void )const{
  return _object_type;
}

namespace h2sl {
  /** 
   * Feature_Region_Abstract_Container_Object_Type class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Region_Abstract_Container_Object_Type& other ){
    out << "class:\"Feature_Region_Abstract_Container_Object_Type\" ";
    out << "object_type:\"" << other.object_type() << "\"";
    return out;
  }
}
