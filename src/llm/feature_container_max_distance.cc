/**
 * @file feature_container_max_distance.cc
 *
 * @brief
 *
 * Class used to describe a feature of the container number
 */
#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/world.h"
#include "h2sl/feature_container_max_distance.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Max_Distance class constructor
 */
Feature_Container_Max_Distance::
Feature_Container_Max_Distance( const bool& invert ) : Feature( invert ) {

}

/**
 * Feature_Container_Max_Distance class copy constructor
 */
Feature_Container_Max_Distance::
Feature_Container_Max_Distance( const Feature_Container_Max_Distance& other ) : Feature( other ) {

}

/**
 * Feature_Container_Max_Distance class destructor
 */
Feature_Container_Max_Distance::
~Feature_Container_Max_Distance() {

}

/** 
 * Feature_Container_Max_Distance class assignment operator
 */
Feature_Container_Max_Distance&
Feature_Container_Max_Distance::
operator=( const Feature_Container_Max_Distance& other ){
  _invert = other._invert;
  return (*this);
}


/**
 * returns the value of the feature
 */
bool
Feature_Container_Max_Distance::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

/**
 * returns the value of the feature
 */
bool
Feature_Container_Max_Distance::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Container * container = dynamic_cast< const Container* >( grounding );

  if( container == NULL ) {
    return false;
  }

  if ( container->container().size() <= 1 ) {
    return false;
  }

  for ( unsigned int i = 0; i < container->container().size(); i++ ) {
    if ( dynamic_cast< Object* >(container->container()[ i ]) == NULL ) {
      return false;
    }
  }

  for ( unsigned int i = 0; i < container->container().size(); i++ ) {
    map< string, vector< Object* > >::const_iterator it; 
    it = world->max_distance_sorted_objects().find( static_cast< Object* >(container->container()[ i ])->type() );
    if (it != world->max_distance_sorted_objects().end() ) {
      for ( unsigned int j = 0; j < it->second.size(); j++ ) {
        if ( *(static_cast< Object* >( container->container()[ i ])) == *(it->second[ j ]) ) {
          if ( j >= container->container().size() ) {
            return _invert; 
          } else {
            return !_invert;
          } 
        }
      }
    }
  }
  return false;
}


/** 
 * imports the Feature_Container_Max_Distance class from an XML file
 */
void
Feature_Container_Max_Distance::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_max_distance" ) ) == 0 ){
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
 * imports the Feature_Container_Max_Distance class from an XML node pointer
 */
void
Feature_Container_Max_Distance::
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
 * exports the Feature_Container_Max_Distance class to an XML file
 */
void
Feature_Container_Max_Distance::
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
 * exports the Feature_Container_Max_Distance class to an XML node pointer
 */
void
Feature_Container_Max_Distance::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_max_distance" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Feature_Container_Max_Distance class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Max_Distance& other ){
    out << "class:\"Feature_Max_Distance_Container\" ";
    return out;
  }
}
