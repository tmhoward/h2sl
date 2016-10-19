/**
 * @file abstract_container.cc
 *
 * @brief
 *
 * a class that describes an abstract container
 */
#include <sstream>
#include "h2sl/abstract_container.h"

using namespace std;
using namespace h2sl;

/**
 * abstract_container class constructor
 */
Abstract_Container::
Abstract_Container( const string& objectType,
                    const string& numberType,
                    const string& indexType,
                    const string& colorType ) : Grounding() {
  insert_prop< std::string >( _properties, "object_type", objectType );
  insert_prop< std::string >( _properties, "number_type", numberType );
  insert_prop< std::string >( _properties, "index_type", indexType );
  insert_prop< std::string >( _properties, "object_color_type", colorType );
}

/**
 * abstract_container constructor from xml node ptr
 */
Abstract_Container::
Abstract_Container( xmlNodePtr root ) {
  insert_prop< std::string >( _properties, "object_type", "na" );
  insert_prop< std::string >( _properties, "number_type", "na");
  insert_prop< std::string >( _properties, "index_type", "na" );
  insert_prop< std::string >( _properties, "object_color_type", "na" );

  from_xml( root );
}


/**
 * abstract_container class copy constructor
 */
Abstract_Container::
Abstract_Container( const Abstract_Container& other ) : Grounding() {

}

/**
 * abstract_container class destructor
 */
Abstract_Container::
~Abstract_Container() {

}

/** 
 * abstract_container class assignment operator
 */
Abstract_Container&
Abstract_Container::
operator=( const Abstract_Container& other ){
  _properties = other._properties;
  return (*this);
}


/**
  * Abstract_Container class equality operator
 */
bool
Abstract_Container::
operator==( const Abstract_Container& other )const{
  if ( object_type() != other.object_type() ) {
    return false;
  } else if ( number_type() != other.number_type() ) {
    return false;
  } else if ( index_type() != other.index_type() ) {
    return false;
  } else if ( object_color_type() != other.object_color_type() ) {
    return false;
  } else {
  return true;
  }
}

/**
 * Abstract_Container class inequality operator
 */
bool
Abstract_Container::
operator!=( const Abstract_Container& other )const{
  return !( *this == other );
}

/**
 * Abstract_Container class duplicator operator
 */
Abstract_Container*
Abstract_Container::
dup( void )const{
  return new Abstract_Container( *this );
}


/** 
 * imports the abstract_container class from an XML file
 */
void
Abstract_Container::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "abstract_container" ) ) == 0 ){
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
 * imports the abstract_container class from an XML node pointer
 *
 */

void
Abstract_Container::
from_xml( xmlNodePtr root ){
  object_type() = "na";
  number_type() = "na";
  index_type() = "na";
  object_color_type() = "na";

  if ( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > object_type_prop = has_prop< std::string >( root, "object_type" );
    if( object_type_prop.first ) {
      object_type() = object_type_prop.second;
    }

    pair< bool, string > number_type_prop = has_prop< std::string >( root, "number_type" );
    if( number_type_prop.first ) {
      number_type() = number_type_prop.second;
    }

    pair< bool, string > index_type_prop = has_prop< std::string >( root, "index_type" );
    if( index_type_prop.first ) {
      index_type() = index_type_prop.second;
    }

    pair< bool, string > object_color_type_prop = has_prop< std::string >( root, "object_color_type" );
    if( object_color_type_prop.first ) {
      object_color_type() = object_color_type_prop.second;
    }
  }
  return; 
}

/**
 * exports the abstract_container class to an XML file
 */
void
Abstract_Container::
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
 * exports the abstract_container class to an XML node pointer
 */
void
Abstract_Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "abstract_container" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "object_type" ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "number_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "number_type" ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "index_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "index_type" ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "object_color_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "object_color_type" ).c_str() ) );

  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * abstract_container class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Abstract_Container& other ){
    out << "Abstract_Container(";
    out << "object_type=\"" << other.object_type() << "\",";
    out << "number_type=\"" << other.number_type() << "\",";
    out << "index_type=\"" << other.index_type()  << "\",";
    out << "object_color_type=\"" << other.object_color_type()  << "\",";
    out << ")";
   return out;
  }
}

