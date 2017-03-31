/**
 * @file feature_container_number_equals_world_objects.cc
 *
 * @brief
 *
 * Class used to describe a feature of the container number_equals_world_objects
 */
#include <sstream>

#include "h2sl/feature_container_number_equals_world_objects.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Number_Equals_World_Objects class constructor
 */
Feature_Container_Number_Equals_World_Objects::
Feature_Container_Number_Equals_World_Objects( const bool& invert,
                                                const string& objectType ) : Feature( invert ) {
  insert_prop< std::string >( _string_properties, "object_type", objectType );
}

/**
 * Feature_Container_Number_Equals_World_Objects class constructor
 */
Feature_Container_Number_Equals_World_Objects::
Feature_Container_Number_Equals_World_Objects( xmlNodePtr root ) : Feature() {
  insert_prop< std::string >( _string_properties, "object_type", "na" );
  from_xml( root );
}

/**
 * Feature_Container_Number_Equals_World_Objects class copy constructor
 */
Feature_Container_Number_Equals_World_Objects::
Feature_Container_Number_Equals_World_Objects( const Feature_Container_Number_Equals_World_Objects& other ) : Feature( other ) {
  
}

/**
 * Feature_Container_Number_Equals_World_Objects class destructor
 */
Feature_Container_Number_Equals_World_Objects::
~Feature_Container_Number_Equals_World_Objects() {

}

/** 
 * Feature_Container_Number_Equals_World_Objects class assignment operator
 */
Feature_Container_Number_Equals_World_Objects&
Feature_Container_Number_Equals_World_Objects::
operator=( const Feature_Container_Number_Equals_World_Objects& other ){
  _invert = other._invert;
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/**
 * returns the value of the fature
 */
bool
Feature_Container_Number_Equals_World_Objects::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Number_Equals_World_Objects::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Container* container = dynamic_cast< const Container* >( grounding );
  if( container != NULL ){
    if( container->container().size() > 0 ){
      vector< Object* > objects;
      for( map< string, Object* >::const_iterator it_world_object = world->objects().begin(); it_world_object != world->objects().end(); it_world_object++ ){
        if( it_world_object->second->type() == object_type() ) {
          objects.push_back( it_world_object->second );
        }
      }
      if ( container->container().size() == objects.size() ) {
        return !_invert;
      } else {
        return _invert;
      }
    }
  }
  return false;
}

/**
 * imports the Feature_Container_Number_Equals_World_Objects class from an XML file
 */
void
Feature_Container_Number_Equals_World_Objects::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_number_equals_world_objects" ) ) == 0 ){
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
 * imports the Feature_Container_Number_Equals_World_Objects class from an XML node pointer
 */
void
Feature_Container_Number_Equals_World_Objects::
from_xml( xmlNodePtr root ){
  _invert = false;
  object_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "object_type" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }

    pair< bool, std::string > object_type_prop = has_prop< std::string >( root, "object_type" );
    if( object_type_prop.first ) {
      object_type() = object_type_prop.second;
    }
  }
}

/**
 * exports the Feature_Container_Number_Equals_World_Objects class to an XML file
 */
void
Feature_Container_Number_Equals_World_Objects::
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
 * exports the Feature_Container_Number_Equals_World_Objects class to an XML node pointer
 */
void
Feature_Container_Number_Equals_World_Objects::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_number_equals_world_objects" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( object_type().c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Feature_Container_Number_Equals_World_Objects class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Number_Equals_World_Objects& other ){
    out <<"class:\"Feature_Container_Number_Equals_World_Objects\" ";
    return out;
  }
}
