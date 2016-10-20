/**
 * @file object_color.cc
 *
 * @brief
 *
 * a class that describes a object_color
 */

#include "h2sl/object_color.h"

using namespace std;
using namespace h2sl;

/**
 * Object_Color class constructor
 */
Object_Color::
Object_Color( const string& object_colorType ) : Grounding() {
  insert_prop< std::string >( _properties, "object_color_type", object_colorType );
}


Object_Color::
Object_Color( xmlNodePtr root ) : Grounding() {
    insert_prop< std::string >( _properties, "object_color_type", "na" );
    from_xml( root );
}

/**
 * Object_Color class copy constructor
 */
Object_Color::
Object_Color( const Object_Color& other ) : Grounding( other ) {

}

/**
 * Object_Color class destructor
 */
Object_Color::
~Object_Color() {

}

/** 
 * Object_Color class assignment operator
 */
Object_Color&
Object_Color::
operator=( const Object_Color& other ){
  _properties = other._properties;
  return (*this);
}

/** 
 * Object_Color class equality operator
 */
bool
Object_Color::
operator==( const Object_Color& other )const{
  if ( object_color_type() != other.object_color_type() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Object_Color class inequality operator
 */
bool
Object_Color::
operator!=( const Object_Color& other )const{
  return !( *this == other );
}

/** 
 * Object_Color class dup operator
 */
Object_Color*
Object_Color::
dup( void )const{
  return new Object_Color( *this );
}

/** 
 * imports the Object_Color class from an XML file
 */
void
Object_Color::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "object_color" ) ) == 0 ){
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
 * imports the Object_Color class from an XML node pointer
 */
void
Object_Color::
from_xml( xmlNodePtr root ){
  object_color_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
      pair< bool, string > object_color_type_prop = has_prop< std::string >( root, "object_color_type" );
      if( object_color_type_prop.first ){
        object_color_type() = object_color_type_prop.second;
      }
  }
  return;
}

/**
 * exports the Object_Color class to an XML file
 */
void
Object_Color::
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
 * exports the Object_Color class to an XML node pointer
 */
void
Object_Color::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "object_color" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "object_color_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "object_color_type").c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Object_Color class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Object_Color& other ){
    out << "Object_Color(";
    out << "object_color_type=\"" << other.object_color_type() << "\",";
    out << ")";  
    return out;
  }
}
