/**
 * @file feature_max_distance_object_within.cc
 *
 * @brief
 *
 * Class used to describe a feature of the container number
 */
#include <sstream>
#include <assert.h>

#include "h2sl/feature_max_distance_object_within.h"
#include "h2sl/object.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Max_Distance_Object_Within class constructor
 */
Feature_Max_Distance_Object_Within::
Feature_Max_Distance_Object_Within( const bool& invert,
                          const unsigned int& number ) : Feature( invert ),
                                                              _number( number ) {

}

/**
 * Feature_Max_Distance_Object_Within class copy constructor
 */
Feature_Max_Distance_Object_Within::
Feature_Max_Distance_Object_Within( const Feature_Max_Distance_Object_Within& other ) : Feature( other ),
                                                              _number( other._number ){

}

/**
 * Feature_Max_Distance_Object_Within class destructor
 */
Feature_Max_Distance_Object_Within::
~Feature_Max_Distance_Object_Within() {

}

/** 
 * Feature_Max_Distance_Object_Within class assignment operator
 */
Feature_Max_Distance_Object_Within&
Feature_Max_Distance_Object_Within::
operator=( const Feature_Max_Distance_Object_Within& other ){
  _invert = other._invert;
  _number = other._number;
  return (*this);
}

/**
 * returns the value of the feature
 */
bool
Feature_Max_Distance_Object_Within::
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
Feature_Max_Distance_Object_Within::
value( const string& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world,
        const Grounding* context){
  const Object* object_grounding = dynamic_cast< const Object* >( grounding );
  if( object_grounding != NULL ){
      map< string, vector< Object* > >::const_iterator it = world->max_distance_sorted_objects().find( object_grounding->type() );
      if ( it != world->max_distance_sorted_objects().end() ) {
        for( unsigned int i = 0; i < _number; i++ ){
          if( _number < it->second.size() ){
           if( *object_grounding == *(it->second[i]) ){
              return !_invert;
            }
          }
        } // for loop
       return _invert;
      }
  }
  return false;
}

/** 
 * imports the Feature_Max_Distance_Object_Within class from an XML file
 */
void
Feature_Max_Distance_Object_Within::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_max_distance_object_within" ) ) == 0 ){
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
 * imports the Feature_Max_Distance_Object_Within class from an XML node pointer
 */
void
Feature_Max_Distance_Object_Within::
from_xml( xmlNodePtr root ){
  _invert = false;
  _number = 0;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "number" ) );
    if( tmp != NULL){
      string number_string = ( const char* )( tmp );
      _number = strtol( number_string.c_str(), NULL, 10 );
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Max_Distance_Object_Within class to an XML file
 */
void
Feature_Max_Distance_Object_Within::
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
 * exports the Feature_Max_Distance_Object_Within class to an XML node pointer
 */
void
Feature_Max_Distance_Object_Within::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_max_distance_object_within" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  stringstream number_string;
  number_string << _number;
  xmlNewProp( node, ( const xmlChar* )( "number" ), ( const xmlChar* )( number_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

unsigned int&
Feature_Max_Distance_Object_Within::
number( void ){
  return _number;
}

const unsigned int&
Feature_Max_Distance_Object_Within::
number( void )const{
  return _number;
}

namespace h2sl {
  /** 
   * Feature_Max_Distance_Object_Within class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Max_Distance_Object_Within& other ){
    out << "class:\"Feature_Max_Distance_Object_Within\" ";
    out << "number:\"" << other.number() << "\"";
    return out;
  }
}
