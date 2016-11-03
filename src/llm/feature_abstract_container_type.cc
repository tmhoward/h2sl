/**
 * @file feature_abstract_container_type.cc
 *
 * @brief
 *
 * Class used to describe a feature of the abstract_container type
 */
#include <sstream>

#include "h2sl/feature_abstract_container_type.h"
#include "h2sl/abstract_container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Abstract_Container_Type class constructor
 */
Feature_Abstract_Container_Type::
Feature_Abstract_Container_Type( const bool& invert,
                                const std::string& abstract_container_type ) : Feature( invert ),
                                                              _abstract_container_type( abstract_container_type ) {

}

/**
 * Feature_Abstract_Container_Type class copy constructor
 */
Feature_Abstract_Container_Type::
Feature_Abstract_Container_Type( const Feature_Abstract_Container_Type& other ) : Feature( other ),
                                                              _abstract_container_type( other._abstract_container_type ){

}

/**
 * Feature_Abstract_Container_Type class destructor
 */
Feature_Abstract_Container_Type::
~Feature_Abstract_Container_Type() {

}

/** 
 * Feature_Abstract_Container_Type class assignment operator
 */
Feature_Abstract_Container_Type&
Feature_Abstract_Container_Type::
operator=( const Feature_Abstract_Container_Type& other ){
  _invert = other._invert;
  _abstract_container_type = other._abstract_container_type;
  return (*this);
}

/**
 * returns the value of the fature
 */
bool
Feature_Abstract_Container_Type::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Abstract_Container_Type::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Abstract_Container* abstract_container = dynamic_cast< const Abstract_Container* >( grounding );
  if( abstract_container != NULL ){
    if( abstract_container->type() == _abstract_container_type ){
      return !_invert;
    } else{
      return _invert; 
    }
  }
  return false;
}

/** 
 * imports the Feature_Abstract_Container_Type class from an XML file
 */
void
Feature_Abstract_Container_Type::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_abstract_container_type" ) ) == 0 ){
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
 * imports the Feature_Abstract_Container_Type class from an XML node pointer
 */
void
Feature_Abstract_Container_Type::
from_xml( xmlNodePtr root ){
  _invert = false;
  _abstract_container_type = "na";
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "abstract_container_type" ) );
    if( tmp != NULL){
      string abstract_container_type_string = ( const char* )( tmp );
      _abstract_container_type = abstract_container_type_string;
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Abstract_Container_Type class to an XML file
 */
void
Feature_Abstract_Container_Type::
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
 * exports the Feature_Abstract_Container_Type class to an XML node pointer
 */
void
Feature_Abstract_Container_Type::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_abstract_container_type" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "abstract_container_type" ), ( const xmlChar* )( _abstract_container_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

std::string&
Feature_Abstract_Container_Type::
abstract_container_type( void ){
  return _abstract_container_type;
}

const std::string&
Feature_Abstract_Container_Type::
abstract_container_type( void )const{
  return _abstract_container_type;
}

namespace h2sl {
  /** 
   * Feature_Abstract_Container_Type class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Abstract_Container_Type& other ){
    out << "class:\"Feature_Abstract_Container_Type\" "; 
    out << "abstract_container_type:\"" << other.abstract_container_type() << "\"";
    return out;
  }
}
