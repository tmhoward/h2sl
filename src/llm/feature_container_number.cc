/**
 * @file feature_container_number.cc
 *
 * @brief
 *
 * Class used to describe a feature of the container number
 */
#include <sstream>

#include "h2sl/feature_container_number.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Number class constructor
 */
Feature_Container_Number::
Feature_Container_Number( const bool& invert,
                          const int& number ) : Feature( invert ) {
  insert_prop< int >( _int_properties, "number", number );
}

/**
 * Feature_Container_Number class constructor
 */
Feature_Container_Number::
Feature_Container_Number( xmlNodePtr root ) : Feature() {
  insert_prop< int >( _int_properties, "number", 0 );
  from_xml( root );
}

/**
 * Feature_Container_Number class copy constructor
 */
Feature_Container_Number::
Feature_Container_Number( const Feature_Container_Number& other ) : Feature( other ) {
  
}

/**
 * Feature_Container_Number class destructor
 */
Feature_Container_Number::
~Feature_Container_Number() {

}

/** 
 * Feature_Container_Number class assignment operator
 */
Feature_Container_Number&
Feature_Container_Number::
operator=( const Feature_Container_Number& other ){
  _invert = other._invert;
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/**
 * returns the value of the fature
 */
bool
Feature_Container_Number::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Number::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Container* container = dynamic_cast< const Container* >( grounding );
  if( container != NULL ){
    if( container->container().size() == number() ){
      return !_invert;
    } else {
      return _invert;
    }
  }
  return false;
}

/**
 * imports the Feature_Container_Number class from an XML file
 */
void
Feature_Container_Number::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_number" ) ) == 0 ){
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
 * imports the Feature_Container_Number class from an XML node pointer
 */
void
Feature_Container_Number::
from_xml( xmlNodePtr root ){
  _invert = false;
  number() = 0;
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "number" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }

    pair< bool, int > number_prop = has_prop< int >( root, "number" );
    if( number_prop.first ) {
      number() = number_prop.second;
    }
  }
}

/**
 * exports the Feature_Container_Number class to an XML file
 */
void
Feature_Container_Number::
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
 * exports the Feature_Container_Number class to an XML node pointer
 */
void
Feature_Container_Number::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_number" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "number" ), ( const xmlChar* )( to_std_string( number() ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Feature_Container_Number class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Number& other ){
    out <<"class:\"Feature_Container_Number\" invert:(" << other.invert() << ") number:(" << other.number() << ")";
    return out;
  }
}
