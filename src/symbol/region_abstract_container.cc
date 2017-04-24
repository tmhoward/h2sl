/**
 * @file region_abstract_container.cc
 * 
 * @brief
 *
 * a class that describes the Region_Abstract_Container class.
 */

#include <sstream>
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/rule_object_type.h"
#include "h2sl/rule_number.h"
#include "h2sl/rule_index.h"
#include "h2sl/rule_object_color.h"
#include "h2sl/region_abstract_container.h"

using namespace std;
using namespace h2sl;

/**
 * Region_Abstract_Container class constructor. 
 */
Region_Abstract_Container::
Region_Abstract_Container( const string& region_abstract_containerType,
			    const string& objectType,
                            const int& number,
                            const int& index,
                            const string& objectColor ) : Grounding() {
  insert_prop< std::string >( _string_properties, "spatial_relation_type", region_abstract_containerType );
  insert_prop< std::string >( _string_properties, "object_type", objectType );
  insert_prop< int >( _int_properties, "number", number );
  insert_prop< int >( _int_properties, "index", index );
  insert_prop< std::string >( _string_properties, "object_color", objectColor );
}

/**
 * region abstract_container constructor from xml node ptr
 */

Region_Abstract_Container::
Region_Abstract_Container( xmlNodePtr root, World* world ) : Grounding() {
    insert_prop< std::string >( _string_properties, "spatial_relation_type", "na" );
    insert_prop< std::string >( _string_properties, "object_type", "na" );
    insert_prop< int >( _int_properties, "number", 0 );
    insert_prop< int >( _int_properties, "index", 0 );
    insert_prop< std::string >( _string_properties, "object_color", "na" );
    from_xml( root, world );
}

/**
 * Region_Abstract_Container class copy constructor. 
 */
Region_Abstract_Container::
Region_Abstract_Container( const Region_Abstract_Container& other ) : Grounding( other ) {

}

/**
 * Region_Abstract_Container class destructor.
 */
Region_Abstract_Container::
~Region_Abstract_Container() {

}

/**
 * Region_Abstract_Container class assingnment operator. 
 */
Region_Abstract_Container&
Region_Abstract_Container::
operator=( const Region_Abstract_Container& other ) {
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/**
 * Region_Abstract_Container class equality operator. 
 */
bool
Region_Abstract_Container::
operator==( const Region_Abstract_Container& other )const{
  if ( spatial_relation_type() != other.spatial_relation_type() ) {
    return false;
  } else if ( type() != other.type() ) {
    return false;
  } else if ( number() != other.number() ) {
    return false;
  } else if ( index() != other.index() ) {
    return false;
  } else if ( color() != other.color() ) {
    return false;
  } else {
    return true;
  }
}

/**
 * Region_Abstract_Container class inequality operator. 
 */
bool
Region_Abstract_Container::
operator!=( const Region_Abstract_Container& other )const{
  return !( *this == other );
}

/**
 * Region_Abstract_Container class dup operator. 
 */
Region_Abstract_Container*
Region_Abstract_Container::
dup( void )const{
  return new Region_Abstract_Container( *this );
}

string
Region_Abstract_Container::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Region_Abstract_Container* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Region_Abstract_Container* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Region_Abstract_Container::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "spatial_relation_type", spatial_relation_type(), stringTypes );
  insert_unique< std::string >( "object_type", type(), stringTypes );
  insert_unique< int >( "index", index(), intTypes );
  insert_unique< int >( "number", number(), intTypes );
  insert_unique< std::string >( "object_color", color(), stringTypes );
  return;
}

void
Region_Abstract_Container::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Region_Abstract_Container::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const std::string& symbolType ){

  if( symbolDictionary.has_class_name( class_name() ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "binary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< string > >::const_iterator it_spatial_relation_type_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    map< string, vector< string > >::const_iterator it_object_type_types = symbolDictionary.string_types().find( "object_type" );
    map< string, vector< int > >::const_iterator it_number_value_types = symbolDictionary.int_types().find( "number" );
    map< string, vector< int > >::const_iterator it_index_value_types = symbolDictionary.int_types().find( "index" );
    map< string, vector< string > >::const_iterator it_object_colors = symbolDictionary.string_types().find( "object_color" );

    if( ( symbolType == "abstract" ) || ( symbolType == "all" ) ){
      if( ( it_spatial_relation_type_types != symbolDictionary.string_types().end() ) && ( it_object_type_types != symbolDictionary.string_types().end() ) && ( it_number_value_types != symbolDictionary.int_types().end() ) && ( it_index_value_types != symbolDictionary.int_types().end() ) && ( it_object_colors != symbolDictionary.string_types().end() ) ){
        for( unsigned int i = 0; i < it_spatial_relation_type_types->second.size(); i++ ){
          for( unsigned int j = 0; j < it_object_type_types->second.size(); j++ ){
            for( unsigned int k = 0; k < it_number_value_types->second.size(); k++ ){
              for( unsigned int l = 0; l < it_index_value_types->second.size(); l++ ){
                for( unsigned int m = 0; m < it_object_colors->second.size(); m++ ){
                  it_search_spaces_symbol->second.second.push_back( new Region_Abstract_Container( it_spatial_relation_type_types->second[ i ], it_object_type_types->second[ j ], it_number_value_types->second[ k ], it_index_value_types->second[ l ], it_object_colors->second[ m ] ) );
                }
              }
            }
          }
        }
      }
    }
  }

  return;
}

void
Region_Abstract_Container::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Spatial_Relation rule_spatial_relation( spatial_relation_type() );
  insert_unique_grounding< Rule_Spatial_Relation >( groundingSet, rule_spatial_relation );
  Rule_Object_Type rule_object_type( type() );
  insert_unique_grounding< Rule_Object_Type >( groundingSet, rule_object_type );
  Rule_Number rule_number( number() );
  insert_unique_grounding< Rule_Number >( groundingSet, rule_number );
  Rule_Index rule_index( index() );
  insert_unique_grounding< Rule_Index >( groundingSet, rule_index );
  Rule_Object_Color rule_object_color( color() );
  insert_unique_grounding< Rule_Object_Color >( groundingSet, rule_object_color );
  return;
}

bool
Region_Abstract_Container::
equals( const Grounding& other )const{
  if( dynamic_cast< const Region_Abstract_Container* >( &other ) != NULL ){
    return ( *this == *static_cast< const Region_Abstract_Container* >( &other ) );
  } else {
    return false;
  }
}

/**
 * exports the Region_Abstract_Container class to an XML file. 
 */
void
Region_Abstract_Container::
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
 * exports the Region_Abstract_Container class to an XML node pointer. 
 */
void
Region_Abstract_Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "region_abstract_container" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "spatial_relation_type" ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "object_type" ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "number" ), ( const xmlChar* )( to_std_string( get_prop< int >( _int_properties, "number" ) ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "index" ), ( const xmlChar* )( to_std_string( get_prop< int >( _int_properties, "index" ) ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "object_color" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "object_color" ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Region_Abstract_Container::
to_latex( void )const{
  stringstream tmp;
  tmp << "Region\\_Abstract\\_Container(spatial\\_relation\\_type=" << spatial_relation_type() << "object\\_type=" << type() << ",number=" << number() << ",index=" << index() << "object\\_color=" << color() << ")";
  return tmp.str();
}

/**
 * imports the Region_Abstract_Container class from an XML file. 
 */
void
Region_Abstract_Container::
from_xml( const string& filename, World* world ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  root = xmlDocGetRootElement( doc );
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "region_abstract_container" ) ) == 0 ){
          from_xml( l1, world );
        }
      }
    }
  }
  xmlFreeDoc( doc );
  return;
}

/**
 * imports the Region_Abstract_Container class from an XML node pointer. 
 */
void
Region_Abstract_Container::
from_xml( xmlNodePtr root, World* world ){
  spatial_relation_type() = "na";
  type() = "na";
  number() = 0;
  index() = 0;
  color() = "na";

  if( root->type == XML_ELEMENT_NODE ){
    vector< string > region_abstract_container_keys = { "spatial_relation_type", "object_type", "number", "index", "object_color", "object_color_type" };
    assert( check_keys( root, region_abstract_container_keys ) );

    pair< bool, string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ){
      spatial_relation_type() = spatial_relation_type_prop.second;
    }
    pair< bool, string > type_prop = has_prop< std::string >( root, "object_type" );
    if( type_prop.first ) {
      type() = type_prop.second;
    }
    pair< bool, int > number_prop = has_prop< int >( root, "number" );
    if( number_prop.first ) {
      number() = number_prop.second;
    }
    pair< bool, int > index_prop = has_prop< int >( root, "index" );
    if( index_prop.first ) {
      index() = index_prop.second;
    }
    pair< bool, string > color_prop = has_prop< std::string >( root, "object_color" );
    if( color_prop.first ) {
      color() = color_prop.second;
    }
    color_prop = has_prop< std::string >( root, "object_color_type" );
    if( color_prop.first ) {
      color() = color_prop.second;
    }
  }
  return;
}

namespace h2sl {

  /**
   * Region_Abstract_Container class ostream operator
   */

  ostream&
  operator<<( ostream& out,
              const Region_Abstract_Container& other ){
    out << "Region_Abstract_Container(";
    out << "spatial_relation_type=\"" << other.spatial_relation_type() << "\",";
    out << "object_type=\"" << other.type() << "\",";
    out << "number=\"" << other.number() << "\",";
    out << "index=\"" << other.index()  << "\",";
    out << "object_color=\"" << other.color()  << "\",";
    out << "} ";
    return out;
  }
}


