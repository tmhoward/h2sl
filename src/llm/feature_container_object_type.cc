/**
 * @file feature_container_object_type.cc
 *
 * @brief
 *
 * Class used to describe a feature of the container object_type
 */
#include <sstream>

#include "h2sl/feature_container_object_type.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Object_Type class constructor
 */
Feature_Container_Object_Type::
Feature_Container_Object_Type( const bool& invert,
                                                const string& objectType ) : Feature( invert ) {
  insert_prop< std::string >( _string_properties, "object_type", objectType );
}

/**
 * Feature_Container_Object_Type class constructor
 */
Feature_Container_Object_Type::
Feature_Container_Object_Type( xmlNodePtr root ) : Feature() {
  insert_prop< std::string >( _string_properties, "object_type", "na" );
  from_xml( root );
}

/**
 * Feature_Container_Object_Type class copy constructor
 */
Feature_Container_Object_Type::
Feature_Container_Object_Type( const Feature_Container_Object_Type& other ) : Feature( other ) {
  
}

/**
 * Feature_Container_Object_Type class destructor
 */
Feature_Container_Object_Type::
~Feature_Container_Object_Type() {

}

/** 
 * Feature_Container_Object_Type class assignment operator
 */
Feature_Container_Object_Type&
Feature_Container_Object_Type::
operator=( const Feature_Container_Object_Type& other ){
  _invert = other._invert;
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/**
 * returns the value of the fature
 */
bool
Feature_Container_Object_Type::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Object_Type::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Container* container = dynamic_cast< const Container* >( grounding );
  if( container != NULL ){
    if( container->container().size() > 0 ){
      for( vector< Grounding* >::const_iterator it_grounding = container->container().begin(); it_grounding != container->container().end(); it_grounding++ ){
        if( *it_grounding != NULL ){
          if( dynamic_cast< const Object* >( *it_grounding ) != NULL ){
            if( static_cast< const Object* >( *it_grounding )->type() != object_type() ){
              return _invert;
            }
          }
        } 
      }
      return !_invert;
    }
  }
  return false;
}

/**
 * imports the Feature_Container_Object_Type class from an XML file
 */
void
Feature_Container_Object_Type::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_object_type" ) ) == 0 ){
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
 * imports the Feature_Container_Object_Type class from an XML node pointer
 */
void
Feature_Container_Object_Type::
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
 * exports the Feature_Container_Object_Type class to an XML file
 */
void
Feature_Container_Object_Type::
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
 * exports the Feature_Container_Object_Type class to an XML node pointer
 */
void
Feature_Container_Object_Type::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_object_type" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( object_type().c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Feature_Container_Object_Type class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Object_Type& other ){
    out <<"class:\"Feature_Container_Object_Type\" invert:(" << other.invert() << ") object_type:(" << other.object_type() << ")";
    return out;
  }
}
