/**
 * @file feature_abstract_container_number_equals_world_objects.cc
 *
 * @brief
 *
 * Class used to describe a feature of the abstract_container number_equals_world_objects
 */
#include <sstream>

#include "h2sl/world.h"
#include "h2sl/feature_abstract_container_number_equals_world_objects.h"
#include "h2sl/abstract_container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Abstract_Container_Number_Equals_World_Objects class constructor
 */
Feature_Abstract_Container_Number_Equals_World_Objects::
Feature_Abstract_Container_Number_Equals_World_Objects( const bool& invert ) : Feature( invert ) {

}

Feature_Abstract_Container_Number_Equals_World_Objects::
Feature_Abstract_Container_Number_Equals_World_Objects( xmlNodePtr root ) : Feature() {
  from_xml( root );
}

/**
 * Feature_Abstract_Container_Number_Equals_World_Objects class copy constructor
 */
Feature_Abstract_Container_Number_Equals_World_Objects::
Feature_Abstract_Container_Number_Equals_World_Objects( const Feature_Abstract_Container_Number_Equals_World_Objects& other ) : Feature( other ) {

}

/**
 * Feature_Abstract_Container_Number_Equals_World_Objects class destructor
 */
Feature_Abstract_Container_Number_Equals_World_Objects::
~Feature_Abstract_Container_Number_Equals_World_Objects() {

}

/** 
 * Feature_Abstract_Container_Number_Equals_World_Objects class assignment operator
 */
Feature_Abstract_Container_Number_Equals_World_Objects&
Feature_Abstract_Container_Number_Equals_World_Objects::
operator=( const Feature_Abstract_Container_Number_Equals_World_Objects& other ){
  _invert = other._invert;
  return (*this);
}


/**
 * returns the value of the fature
 */
bool
Feature_Abstract_Container_Number_Equals_World_Objects::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

/**
 * returns the value of the fature
 */
bool
Feature_Abstract_Container_Number_Equals_World_Objects::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world, 
        const Grounding* context ){
  const Abstract_Container* abstract_container = dynamic_cast< const Abstract_Container* >( grounding );
  if( abstract_container != NULL ){
    assert( !world->sorted_objects().empty() );
    map< string, vector< Object* > >::const_iterator it_sorted_objects = world->sorted_objects().begin()->second.find( abstract_container->type() );
    if ( it_sorted_objects != world->sorted_objects().begin()->second.end() ) {
      if ( abstract_container->number() == it_sorted_objects->second.size() ) {
        return !_invert;
      } else {
        return _invert;
      }
    }
  }
  return false;
}

/** 
 * imports the Feature_Abstract_Container_Number_Equals_World_Objects class from an XML file
 */
void
Feature_Abstract_Container_Number_Equals_World_Objects::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_abstract_container_number_equals_world_objects" ) ) == 0 ){
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
 * imports the Feature_Abstract_Container_Number_Equals_World_Objects class from an XML node pointer
 */
void
Feature_Abstract_Container_Number_Equals_World_Objects::
from_xml( xmlNodePtr root ){
  _invert = false;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Abstract_Container_Number_Equals_World_Objects class to an XML file
 */
void
Feature_Abstract_Container_Number_Equals_World_Objects::
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
 * exports the Feature_Abstract_Container_Number_Equals_World_Objects class to an XML node pointer
 */
void
Feature_Abstract_Container_Number_Equals_World_Objects::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_abstract_container_number_equals_world_objects" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Feature_Abstract_Container_Number_Equals_World_Objects class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Abstract_Container_Number_Equals_World_Objects& other ){
    out <<"class:\"Feature_Abstract_Container_Number_Equals_World_Objects\" ";
    return out;
  }
}
