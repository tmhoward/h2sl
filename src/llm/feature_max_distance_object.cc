/**
 * @file feature_max_distance_object.cc
 *
 * @brief
 *
 * Class used to describe a feature of the object number
 */
#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/world.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/feature_max_distance_object.h"
#include "h2sl/object.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Max_Distance_Object class constructor
 */
Feature_Max_Distance_Object::
Feature_Max_Distance_Object( const bool& invert ) : Feature( invert ) {

}

/**
 * Feature_Max_Distance_Object class copy constructor
 */
Feature_Max_Distance_Object::
Feature_Max_Distance_Object( const Feature_Max_Distance_Object& other ) : Feature( other ) {

}

/**
 * Feature_Max_Distance_Object class destructor
 */
Feature_Max_Distance_Object::
~Feature_Max_Distance_Object() {

}

/** 
 * Feature_Max_Distance_Object class assignment operator
 */
Feature_Max_Distance_Object&
Feature_Max_Distance_Object::
operator=( const Feature_Max_Distance_Object& other ){
  _invert = other._invert;
  return (*this);
}


/**
 * returns the value of the feature
 */
bool
Feature_Max_Distance_Object::
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
Feature_Max_Distance_Object::
value( const string& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world,
        const Grounding* context ) {
  const Object* object_grounding = dynamic_cast< const Object* >( grounding );
  if( object_grounding != NULL ){
      map< string, vector< Object* > >::const_iterator it = world->max_distance_sorted_objects().find( object_grounding->type() );
      if (it != world->max_distance_sorted_objects().end()) {
          if (*object_grounding == *(it->second.front())) {
              return !_invert;
          } else {
              return _invert;
          }
      } else {
          std::cout << "Error: object grounding not found in max_distance_sorted_objects" << std::endl;
          return false;
      }
  }
    return false;
}

/** 
 * imports the Feature_Max_Distance_Object class from an XML file
 */
void
Feature_Max_Distance_Object::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_max_distance_object" ) ) == 0 ){
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
 * imports the Feature_Max_Distance_Object class from an XML node pointer
 */
void
Feature_Max_Distance_Object::
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
 * exports the Feature_Max_Distance_Object class to an XML file
 */
void
Feature_Max_Distance_Object::
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
 * exports the Feature_Max_Distance_Object class to an XML node pointer
 */
void
Feature_Max_Distance_Object::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_max_distance_object" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Feature_Max_Distance_Object class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Max_Distance_Object& other ){
    out << "class:\"Feature_Max_Distance_Object\" ";
    return out;
  }
}
