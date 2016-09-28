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
Object_Color( const Type& type ) : Grounding(),
                                      _type( type ) {

}

/**
 * Object_Color class copy constructor
 */
Object_Color::
Object_Color( const Object_Color& other ) : Grounding( other ),
                                _type( other._type ) {

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
  _type = other._type;
  return (*this);
}

/** 
 * Object_Color class equality operator
 */
bool
Object_Color::
operator==( const Object_Color& other )const{
  if ( _type != other._type ){
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
 * imports the Object_Color class from a LCM msg
 */
/*bool
Object_Color::
from_msg( const object_color_msg_t * msg ){
  return true;
}*/

/**
 * exports the Object_Color class to a LCM msg
 */
/*void
Object_Color::
to_msg( object_color_msg_t * msg )const{
  return;
}*/

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
  _type = Type::TYPE_UNKNOWN;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "type" ) );
    if( tmp != NULL ){
      string type_string = ( char* )( tmp );
      _type = Object_Color::type_from_std_string( type_string );
      xmlFree( tmp );
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
  xmlNewProp( node, ( const xmlChar* )( "type" ), ( const xmlChar* )( Object_Color::type_to_std_string( _type ).c_str() ) );
  xmlAddChild( root, node );
  return;
}


string
Object_Color::
type_to_std_string( const Type& type ){
  switch( type ){
  case( TYPE_RED ):
    return "red";
    break;
  case( TYPE_BLUE ):
    return "blue";
    break;
  case( TYPE_YELLOW ):
    return "yellow";
    break;
  case( TYPE_UNKNOWN ):
  case( TYPE_NUM_TYPES ):
  default:
    return "na";
  }
}

Object_Color::Type
Object_Color::
type_from_std_string( const std::string& arg){
  for ( unsigned int i = 0; i < Type::TYPE_NUM_TYPES; i++ ){
    if( type_to_std_string( ( Type )( i ) ) == arg ){
      return ( Type )( i );
    }
  }
  return Type::TYPE_UNKNOWN;
}


namespace h2sl {
  /** 
   * Object_Color class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Object_Color& other ){
    out << "class:\"Object_Color\" ";
    out << "type:" << Object_Color::type_to_std_string( other.type() );
    return out;
  }
}
