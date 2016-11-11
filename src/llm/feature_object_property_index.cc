/**
 * @file feature_object_property_index.cc
 *
 * @brief
 *
 * a class used to describe a feature of the object type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/object_property.h"
#include "h2sl/feature_object_property_index.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Property_Index class constructor
 */
//Feature_Object_Property_Index::
//Feature_Object_Property_Index( const bool& invert,
//                      const unsigned int& index ) : Feature( invert ),
//                                                          _index( index ) {
//
//}

Feature_Object_Property_Index::
Feature_Object_Property_Index( const bool& invert,
                      const std::string& index ) : Feature( invert ),
                                                          _index( index ) {

}

/**
 * Feature_Object_Property_Index class copy constructor
 */
Feature_Object_Property_Index::
Feature_Object_Property_Index( const Feature_Object_Property_Index& other ) : Feature( other ),
                                                          _index( other._index ){

}

/**
 * Feature_Object_Property_Index class destructor
 */
Feature_Object_Property_Index::
~Feature_Object_Property_Index() {

}

/** 
 * Feature_Object_Property_Index class assignment operator
 */
Feature_Object_Property_Index&
Feature_Object_Property_Index::
operator=( const Feature_Object_Property_Index& other ){
  _invert = other._invert;
  _index = other._index;
  return (*this);
}

bool
Feature_Object_Property_Index::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Property_Index::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Object_Property* object_property = dynamic_cast< const Object_Property* >( grounding );
  if( object_property != NULL ){
    if( object_property->index() == _index ){
      return !_invert;
    } else {
      return _invert;
    }
  }
  return false;
}

/** 
 * imports the Feature_Object_Property_Index class from an XML file
 */
void
Feature_Object_Property_Index::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_object_property_index" ) ) == 0 ){
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
 * imports the Feature_Object_Property_Index class from an XML node pointer
 */
void
Feature_Object_Property_Index::
from_xml( xmlNodePtr root ){
  _invert = false;
  _index = "na";
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "index" ) );
    if( tmp != NULL ){
      string index_string = ( const char* )( tmp );
      _index =  index_string; 
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Object_Property_Index class to an XML file
 */
void
Feature_Object_Property_Index::
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
 * exports the Feature_Object_Property_Index class to an XML node pointer
 */
void
Feature_Object_Property_Index::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_property_index" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "index" ), ( const xmlChar* )( _index.c_str() ) );
  xmlAddChild( root, node );
  return;
}

std::string&
Feature_Object_Property_Index::
index( void ){
  return _index;
}

const std::string&
Feature_Object_Property_Index::
index( void )const{
  return _index;
}


namespace h2sl {
  /** 
   * Feature_Object_Property_Index class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Object_Property_Index& other ){
    out << "class:\"Feature_Object_Property_Index\" ";
    out << "index:\"" << other.index() << "\"";
    return out;
  }
}
