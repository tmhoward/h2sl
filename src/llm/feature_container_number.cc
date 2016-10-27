/**
 * @file feature_container_number.cc
 *
 * @brief
 *
 * Class used to describe a feature of the container number
 */
#include <sstream>

#include "h2sl/feature_container_number.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Number class constructor
 */
Feature_Container_Number::
Feature_Container_Number( const bool& invert,
                          const unsigned int& abstractContainerNumber ) : Feature( invert ),
                                                              _container_number( abstractContainerNumber ) {

}

/**
 * Feature_Container_Number class copy constructor
 */
Feature_Container_Number::
Feature_Container_Number( const Feature_Container_Number& other ) : Feature( other ),
                                                              _container_number( other._container_number ){

}

/**
 * Feature_Container_Number class destructor
 */
Feature_Container_Number::
~Feature_Container_Number() {

}

/** 
 * Feature_Container_Number class assignment operator
 */
Feature_Container_Number&
Feature_Container_Number::
operator=( const Feature_Container_Number& other ){
  _invert = other._invert;
  _container_number = other._container_number;
  return (*this);
}

/**
 * returns the value of the fature
 */
bool
Feature_Container_Number::
value( const unsigned int& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world ){
  const Container* container = dynamic_cast< const Container* >( grounding );
  if( container != NULL ){
    if( container->container().size() == _container_number ){
      return !_invert;
    } else {
      return _invert;
    }
  }
  return false;
}

/** 
 * imports the Feature_Container_Number class from an XML file
 */
void
Feature_Container_Number::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_number" ) ) == 0 ){
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
 * imports the Feature_Container_Number class from an XML node pointer
 */
void
Feature_Container_Number::
from_xml( xmlNodePtr root ){
  _invert = false;
  _container_number = 0;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "container_number" ) );
    if( tmp != NULL){
      string container_number_string = ( const char* )( tmp );
      _container_number = strtol( container_number_string.c_str(), NULL, 10 );
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Container_Number class to an XML file
 */
void
Feature_Container_Number::
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
 * exports the Feature_Container_Number class to an XML node pointer
 */
void
Feature_Container_Number::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_number" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  stringstream container_number_string;
  container_number_string << _container_number;
  xmlNewProp( node, ( const xmlChar* )( "container_number" ), ( const xmlChar* )( container_number_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

unsigned int&
Feature_Container_Number::
container_number( void ){
  return _container_number;
}

const unsigned int&
Feature_Container_Number::
container_number( void )const{
  return _container_number;
}

namespace h2sl {
  /** 
   * Feature_Container_Number class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Number& other ){
    out << "class:\"Feature_Container_Number\" ";
    out << "container_number:\"" << other.container_number() << "\"";
    return out;
  }
}
