/**
 * @file feature_abstract_container_color.cc
 *
 * @brief
 *
 * Class used to describe a feature of the abstract_container color
 */
#include <sstream>

#include "h2sl/feature_abstract_container_color.h"
#include "h2sl/abstract_container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Abstract_Container_Color class constructor
 */
Feature_Abstract_Container_Color::
Feature_Abstract_Container_Color( const bool& invert,
                          const std::string& abstract_container_color ) : Feature( invert ),
                                                              _abstract_container_color( abstract_container_color ) {

}

/**
 * Feature_Abstract_Container_Color class copy constructor
 */
Feature_Abstract_Container_Color::
Feature_Abstract_Container_Color( const Feature_Abstract_Container_Color& other ) : Feature( other ),
                                                              _abstract_container_color( other._abstract_container_color ){

}

/**
 * Feature_Abstract_Container_Color class destructor
 */
Feature_Abstract_Container_Color::
~Feature_Abstract_Container_Color() {

}

/** 
 * Feature_Abstract_Container_Color class assignment operator
 */
Feature_Abstract_Container_Color&
Feature_Abstract_Container_Color::
operator=( const Feature_Abstract_Container_Color& other ){
  _invert = other._invert;
  _abstract_container_color = other._abstract_container_color;
  return (*this);
}

/**
 * returns the value of the fature
 */
bool
Feature_Abstract_Container_Color::
value( const unsigned int& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world ){
  const Abstract_Container* abstract_container = dynamic_cast< const Abstract_Container* >( grounding );
  if( abstract_container != NULL ){
    if( abstract_container->color() == _abstract_container_color ){
      return !_invert;
    }
  }
  return false;
}

/** 
 * imports the Feature_Abstract_Container_Color class from an XML file
 */
void
Feature_Abstract_Container_Color::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_abstract_container_color" ) ) == 0 ){
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
 * imports the Feature_Abstract_Container_Color class from an XML node pointer
 */
void
Feature_Abstract_Container_Color::
from_xml( xmlNodePtr root ){
  _invert = false;
  _abstract_container_color = "na";
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "abstract_container_color" ) );
    if( tmp != NULL){
      string abstract_container_color_string = ( const char* )( tmp );
      _abstract_container_color = abstract_container_color_string;
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Abstract_Container_Color class to an XML file
 */
void
Feature_Abstract_Container_Color::
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
 * exports the Feature_Abstract_Container_Color class to an XML node pointer
 */
void
Feature_Abstract_Container_Color::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_abstract_container_color" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "abstract_container_color" ), ( const xmlChar* )( _abstract_container_color.c_str() ) );
  xmlAddChild( root, node );
  return;
}

std::string&
Feature_Abstract_Container_Color::
abstract_container_color( void ){
  return _abstract_container_color;
}

const std::string&
Feature_Abstract_Container_Color::
abstract_container_color( void )const{
  return _abstract_container_color;
}

namespace h2sl {
  /** 
   * Feature_Abstract_Container_Color class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Abstract_Container_Color& other ){
    out << "class:\"Feature_Abstract_Container_Color\" "; 
    out << "abstract_container_color:\"" << other.abstract_container_color() << "\"";
    return out;
  }
}
