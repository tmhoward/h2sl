/**
 * @file feature_object_number_matches_world_object_number.cc
 *
 * @brief
 *
 */
#include <sstream>

#include "h2sl/feature_object_number_matches_world_object_number.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Number_Matches_World_Object_Number class constructor
 */
Feature_Object_Number_Matches_World_Object_Number::
Feature_Object_Number_Matches_World_Object_Number( const bool& invert,
                          const unsigned int& objectNumber ) : Feature( invert ),
                                                              _object_number( objectNumber ) {

}

/**
 * Feature_Object_Number_Matches_World_Object_Number class copy constructor
 */
Feature_Object_Number_Matches_World_Object_Number::
Feature_Object_Number_Matches_World_Object_Number( const Feature_Object_Number_Matches_World_Object_Number& other ) : Feature( other ),
                                                              _object_number( other._object_number ){

}

/**
 * Feature_Object_Number_Matches_World_Object_Number class destructor
 */
Feature_Object_Number_Matches_World_Object_Number::
~Feature_Object_Number_Matches_World_Object_Number() {

}

/** 
 * Feature_Object_Number_Matches_World_Object_Number class assignment operator
 */
Feature_Object_Number_Matches_World_Object_Number&
Feature_Object_Number_Matches_World_Object_Number::
operator=( const Feature_Object_Number_Matches_World_Object_Number& other ){
  _invert = other._invert;
  _object_number = other._object_number;
  return (*this);
}

/**
 * returns the value of the feature
 */
bool

Feature_Object_Number_Matches_World_Object_Number::
value( const unsigned int& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ) {
    return value( cv, grounding, children, phrase, world, NULL );
}

/**
 * returns the value of the fature
 */
bool
Feature_Object_Number_Matches_World_Object_Number::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world, 
        const Grounding* context ) {
  const Object* object = dynamic_cast< const Object* >( grounding );
  if( object != NULL ) {
    map< string, vector< Object* >>::const_iterator it;
    it = world->min_x_sorted_objects().find( object->type() );
    if ( it != world->min_x_sorted_objects().end() ) {
      if ( _object_number == it->second.size() ) {
        return !_invert; 
      } else {
        return _invert;
      }
    }
  }    
  return false;
}

/** 
 * imports the Feature_Object_Number_Matches_World_Object_Number class from an XML file
 */
void
Feature_Object_Number_Matches_World_Object_Number::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_object_number_matches_world_object_number" ) ) == 0 ){
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
 * imports the Feature_Object_Number_Matches_World_Object_Number class from an XML node pointer
 */
void
Feature_Object_Number_Matches_World_Object_Number::
from_xml( xmlNodePtr root ){
  _invert = false;
  _object_number = 0;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "object_number" ) );
    if( tmp != NULL){
      string object_number_string = ( const char* )( tmp );
      _object_number = strtol( object_number_string.c_str(), NULL, 10 );
      xmlFree( tmp );
    }
  }
}


/**
 * exports the Feature_Object_Number_Matches_World_Object_Number class to an XML node pointer
 */
void
Feature_Object_Number_Matches_World_Object_Number::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_number_matches_world_object_number" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  stringstream object_number_string;
  object_number_string << _object_number;
  xmlNewProp( node, ( const xmlChar* )( "object_number" ), ( const xmlChar* )( object_number_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

unsigned int&
Feature_Object_Number_Matches_World_Object_Number::
object_number( void ){
  return _object_number;
}

const unsigned int&
Feature_Object_Number_Matches_World_Object_Number::
object_number( void )const{
  return _object_number;
}

namespace h2sl_nsf_nri_mvli {
  /** 
   * Feature_Object_Number_Matches_World_Object_Number class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Object_Number_Matches_World_Object_Number& other ){
    out << "class:\"Feature_Object_Number_Matches_World_Object_Number\" ";
    out << "object_number:\"" << other.object_number() << "\"";
    return out;
  }
}
                       



