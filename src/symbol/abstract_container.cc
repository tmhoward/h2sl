/**
 * @file abstract_container.cc
 *
 * @brief
 *
 * a class that describes an abstract container
 */
#include <sstream>
#include "h2sl_nsf_nri_mvli/abstract_container.h"

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

  if ( abstract_container_object_type() != other.abstract_container_object_type() ) {
    return false;
  } else if ( abstract_container_number_type() != other.abstract_container_number_type() ) {
    return false;
  } else if ( abstract_container_index_type() != other.abstract_container_index_type() ) {
    return false;
  } else if ( abstract_container_color_type() != other.abstract_container_color_type() )
    return false;
  }
  return true;
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
 * xmlNodePtr function before from_xml
**/
Abstract_Container::
Abstract_Container( xmlNodePtr root ) : Grounding () {

  insert_prop< strd::string >( _properties, "object_type", "na" );
  insert_prop< strd::string >( _properties, "number_type", "na" );
  insert_prop< strd::string >( _properties, "index_type", "na" );
  insert_prop< strd::string >( _properties, "object_color_type", "na" );
 
  from_xml( root );
  
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
  abstract_container_object_type() = "na";
  abstract_container_number_type() = "na";
  abstract_container_index_type() = "na";
  abstract_container_object_color_type() = "na";

  if ( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > object_type_prop = has_prop< std::string >( root, "object_type" );
    if( object_type_prop.first ) {
      abstract_container_object_type() = object_type_prop.second;
    }

    pair< bool, string > number_type_prop = has_prop< std::string >( root, "number_type" );
    if( number_type_prop.first ) {
      abstract_container_number_type() = number_type_prop.second;
    }

    pair< bool, string > index_type_prop = has_prop< std::string >( root, "index_type" );
    if( index_type_prop.first ) {
      abstract_container_index_type() = index_type_prop.second;
    }

    pair< bool, string > object_color_type_prop = has_prop< std::string >( root, "object_color_type" );
    if( object_color_type_prop.first ) {
      abstract_container_color_type() = object_color_type_prop.second;
    }

  }

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

  xmlNewProp( node, ( const xmlChar* )( "type" ), ( const xmlChar* )( Object_Type::type_to_std_string( ( Object_Type::Type )( _type ) ).c_str() ) );

  stringstream num_string;
  num_string << _num;
  xmlNewProp( node, ( const xmlChar* )( "num" ), ( const xmlChar* )( num_string.str().c_str() ) );

  xmlNewProp( node, ( const xmlChar* )( "index" ), ( const xmlChar* )( Index::type_to_std_string( ( Index::Type )( _index ) ).c_str() ) );

  xmlNewProp( node, ( const xmlChar* )( "color" ), ( const xmlChar* )( Object_Color::type_to_std_string( ( Object_Color::Type )( _color ) ).c_str() ) );

  xmlAddChild( root, node );
  return;
}

namespace h2sl_nsf_nri_mvli {
  /** 
   * abstract_container class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Abstract_Container& other ){
    out << "class:\"Abstract_Container\" ";
    out << "type:\"" << Object_Type::type_to_std_string( (Object_Type::Type) other.type() ) << "\" ";
    out << "num:\"" << Number::value_to_std_string( ( Number::Value ) other.num() ) << "\" ";
    out << "index:\"" << Index::type_to_std_string( ( Index::Type ) other.index() ) << "\" ";
    out << "color:\"" << Object_Color::type_to_std_string( (Object_Color::Type) other.color() ) << "\" ";
    return out;
  }
}


