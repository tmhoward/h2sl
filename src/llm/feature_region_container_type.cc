/**
 * @file feature_region_container_type.cc
 *
 * @brief
 *
 * a class used to describe a feature of the region_container type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/region_container.h"
#include "h2sl/feature_region_container_type.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Region_Container_Type class constructor
 */
Feature_Region_Container_Type::
Feature_Region_Container_Type( const bool& invert,
                              const std::string& region_container_type ) : Feature( invert ),
                                                          _region_container_type( region_container_type ) {

}

/**
 * Feature_Region_Container_Type class copy constructor
 */
Feature_Region_Container_Type::
Feature_Region_Container_Type( const Feature_Region_Container_Type& other ) : Feature( other ),
                                                          _region_container_type( other._region_container_type ){

}

/**
 * Feature_Region_Container_Type class destructor
 */
Feature_Region_Container_Type::
~Feature_Region_Container_Type() {

}

/** 
 * Feature_Region_Container_Type class assignment operator
 */
Feature_Region_Container_Type&
Feature_Region_Container_Type::
operator=( const Feature_Region_Container_Type& other ){
  _invert = other._invert;
  _region_container_type = other._region_container_type;
  return (*this);
}

bool
Feature_Region_Container_Type::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Region_Container_Type::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Region_Container* region_container = dynamic_cast< const Region_Container* >( grounding );
  if( region_container != NULL ){
    if( region_container->relation_type() == _region_container_type ){
      return !_invert;
    } else {
      return _invert;
    }
  }
  return false;
}

/** 
 * imports the Feature_Region_Container_Type class from an XML file
 */
void
Feature_Region_Container_Type::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_region_container_type" ) ) == 0 ){
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
 * imports the Feature_Region_Container_Type class from an XML node pointer
 */
void
Feature_Region_Container_Type::
from_xml( xmlNodePtr root ){
  _invert = false;
  _region_container_type = "na";
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "region_container_type" ) );
    if( tmp != NULL ){
      string region_container_type_string = ( const char* )( tmp );
      _region_container_type = region_container_type_string;
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Region_Container_Type class to an XML file
 */
void
Feature_Region_Container_Type::
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
 * exports the Feature_Region_Container_Type class to an XML node pointer
 */
void
Feature_Region_Container_Type::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_region_container_type" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "region_container_type" ), ( const xmlChar* )( _region_container_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

std::string&
Feature_Region_Container_Type::
region_container_type( void ){
  return _region_container_type;
}

const std::string&
Feature_Region_Container_Type::
region_container_type( void )const{
  return _region_container_type;
}


namespace h2sl {
  /** 
   * Feature_Region_Container_Type class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Region_Container_Type& other ){
    out << "class:\"Feature_Region_Container_Type\" ";
    out << "region_container_type:\"" << other.region_container_type() << "\"";
    return out;
  }
}
