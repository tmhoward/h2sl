/**
 * @file feature_object_property_type.cc
 *
 * @brief
 *
 * a class used to describe a feature of the object type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/object.h"
#include "h2sl/object_property.h"
#include "h2sl/feature_object_property_type.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Property_Type class constructor
 */
Feature_Object_Property_Type::
Feature_Object_Property_Type( const bool& invert,
                             const std::string& object_type ) : Feature( invert ),
                                                          _object_type( object_type ) {

}

/**
 * Feature_Object_Property_Type class copy constructor
 */
Feature_Object_Property_Type::
Feature_Object_Property_Type( const Feature_Object_Property_Type& other ) : Feature( other ),
                                                          _object_type( other._object_type ){

}

/**
 * Feature_Object_Property_Type class destructor
 */
Feature_Object_Property_Type::
~Feature_Object_Property_Type() {

}

/** 
 * Feature_Object_Property_Type class assignment operator
 */
Feature_Object_Property_Type&
Feature_Object_Property_Type::
operator=( const Feature_Object_Property_Type& other ){
  _invert = other._invert;
  _object_type = other._object_type;
  return (*this);
}

bool
Feature_Object_Property_Type::
value( const unsigned int& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world ){
  const Object_Property* object_property = dynamic_cast< const Object_Property* >( grounding );
  if( object_property != NULL ){
    if( object_property != NULL ){
      if( object_property->type() == _object_type ){
        return !_invert;
      }
    } else {
      return _invert;
    }
  }
  return false;
}

/** 
 * imports the Feature_Object_Property_Type class from an XML file
 */
void
Feature_Object_Property_Type::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_object_property_type" ) ) == 0 ){
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
 * imports the Feature_Object_Property_Type class from an XML node pointer
 */
void
Feature_Object_Property_Type::
from_xml( xmlNodePtr root ){
  _invert = false;
  _object_type = "na";
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "object_type" ) );
    if( tmp != NULL ){
      string object_type_string = ( const char* )( tmp );
      _object_type = object_type_string;
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Object_Property_Type class to an XML file
 */
void
Feature_Object_Property_Type::
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
 * exports the Feature_Object_Property_Type class to an XML node pointer
 */
void
Feature_Object_Property_Type::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_property_type" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( _object_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

std::string&
Feature_Object_Property_Type::
object_type( void ){
  return _object_type;
}

const std::string&
Feature_Object_Property_Type::
object_type( void )const{
  return _object_type;
}


namespace h2sl {
  /** 
   * Feature_Object_Property_Type class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Object_Property_Type& other ){
    out << "class:\"Feature_Object_Property_Type\" ";
    out << "object_type:\"" << other.object_type() << "\"";
    return out;
  }
}
