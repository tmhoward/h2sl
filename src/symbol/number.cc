/**
 * @file number.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/number.h"

using namespace std;
using namespace h2sl;

/**
 * Number class constructor
 */
Number::
Number( const string& numberValue ) : Grounding(){
  insert_prop< std::string >( _properties, "number_value", numberValue );
}

Number::
Number( xmlNodePtr root ) : Grounding() {
    insert_prop< std::string >( _properties, "number_value", "na" );
    from_xml( root );
}

/**
 * Number class copy constructor
 */
Number::
Number( const Number& other ) : Grounding( other ){

}

/**
 * Number class destructor
 */
Number::
~Number() {

}

/** 
 * Number class assignment operator
 */
Number&
Number::
operator=( const Number& other ){
  _properties = other._properties;
  return (*this);
}

/** 
 * Number class equality operator
 */
bool
Number::
operator==( const Number& other )const{
  if ( number_value() != other.number_value() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Number class inequality operator
 */
bool
Number::
operator!=( const Number& other )const{
  return !( *this == other );
}

/** 
 * Number class dup operator
 */
Number*
Number::
dup( void )const{
  return new Number( *this );
}

/** 
 * imports the Number class from an XML file
 */
void
Number::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "number" ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
      xmlFreeDoc( doc );
    } else {
      xmlFreeDoc( doc );
    }
  }
  return;
}

/** 
 * imports the Number class from an XML node pointer
 */
void
Number::
from_xml( xmlNodePtr root ){
  number_value() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > number_value_prop = has_prop< std::string >( root, "number_value" );
    if( number_value_prop.first ){
      number_value() = number_value_prop.second;
    }
  }
  return;
}

/**
 * exports the Number class to an XML file
 */
void
Number::
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
 * exports the Number class to an XML node pointer
 */
void
Number::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "number" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "number_value" ), ( const xmlChar* )( get_prop< std::string >( _properties, "number_value").c_str()  ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Number class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Number& other ){
    out << "Number(";
    out << "number_value=\"" << other.number_value() << "\",";
    out << ")";
    return out;
  }
}
