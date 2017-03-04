/**
 * @file region_container.cc
 * 
 * @brief
 *
 * a class that describes the Region_Container class.
 */

#include <sstream>
#include <cmath>
#include "h2sl/region_container.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Region_Container class constructor. 
 */
Region_Container::
Region_Container( const string& region_containerType,
                 const Container& container ) : Grounding() ,
                                                    _container( container ) {
            insert_prop< std::string >( _string_properties, "region_container_type", region_containerType);
}

/**
 * Region_Container class destructor. 
 */
Region_Container::
~Region_Container() {

}

Region_Container::
Region_Container( xmlNodePtr root ) : Grounding(),
                                       _container() {
    insert_prop< std::string >( _string_properties, "region_container_type", "na" );
    from_xml( root );
}

/**
 * Region_Container class copy constructor. 
 */
Region_Container::
Region_Container( const Region_Container& other ) : Grounding( other ),
                                                        _container( other._container ) {

}

/**
 * Region_Container class assingnment operator. 
 */
Region_Container&
Region_Container::
operator=( const Region_Container& other ) {
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  _container = other._container;
  return (*this);
}

/**
 * Region_Container class equality operator. 
 */
bool
Region_Container::
operator==( const Region_Container& other )const{
  if ( relation_type() != other.relation_type() ) {
    return false;
  } else if ( _container != other._container ) {
    return false;
  } else {
    return true;
  }
}

/**
 * Region_Container class inequality operator. 
 */
bool
Region_Container::
operator!=( const Region_Container& other )const{
  return !( *this == other );
}

/**
 * Region_Container class dup operator. 
 */
Region_Container*
Region_Container::
dup( void )const{
  return new Region_Container( *this );
}

void
Region_Container::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  insert_unique< std::string >( "spatial_relation_type", relation_type(), stringTypes );
  return;
}

void
Region_Container::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< unsigned int, vector< Grounding* > > >& searchSpaces,
                    const symbol_type_t& symbolType ){

  if( symbolDictionary.has_class_name( class_name() ) ){
    map< string, pair< unsigned int, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< unsigned int, vector< Grounding* > > >( class_name(), pair< unsigned int, vector< Grounding* > >( 0, vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< string > >::const_iterator it_object_type_types = symbolDictionary.string_types().find( "object_type" );
    map< string, vector< string > >::const_iterator it_container_type_types = symbolDictionary.string_types().find( "container_type" );
    map< string, vector< string > >::const_iterator it_spatial_relation_type_types = symbolDictionary.string_types().find( "spatial_relation_type" );

    switch( symbolType ){
    case( SYMBOL_TYPE_ABSTRACT ):
    case( SYMBOL_TYPE_ALL ):
      if( ( it_object_type_types != symbolDictionary.string_types().end() ) && ( it_container_type_types != symbolDictionary.string_types().end() ) && ( it_spatial_relation_type_types != symbolDictionary.string_types().end() ) ){
        for( unsigned int k = 0; k < it_container_type_types->second.size(); k++ ){
          for( unsigned int l = 0; l < it_spatial_relation_type_types->second.size(); l++ ){
            it_search_spaces_symbol->second.second.push_back( new Region_Container( it_spatial_relation_type_types->second[ l ], Container( vector< Grounding* >(), it_container_type_types->second[ k ] ) ) );
          }
        }
  
        for( unsigned int i = 0 ; i < it_object_type_types->second.size(); i++ ){
          vector< Object* > objects;
            for( map< string, Object* >::const_iterator it_world_object = world->objects().begin(); it_world_object != world->objects().end(); it_world_object++ ){
            if( it_world_object->second->type() == it_object_type_types->second[ i ] ){
              objects.push_back( it_world_object->second );
            }
          }
          const unsigned int num_sets = pow( 2, objects.size() );
          for( unsigned int j = 0; j < num_sets; j++ ){
            vector< Grounding* > container_objects;
            for( unsigned int k = 0; k < objects.size(); k++ ){
              int mask = 1 << k;
              if( mask & j ){
                container_objects.push_back( dynamic_cast< Grounding* >( objects[ k ] ) );
              }
            }
            if( container_objects.size() > 1 ){
              for( unsigned int k = 0; k < it_container_type_types->second.size(); k++ ){
                for( unsigned int l = 0; l < it_spatial_relation_type_types->second.size(); l++ ){
                  it_search_spaces_symbol->second.second.push_back( new Region_Container( it_spatial_relation_type_types->second[ l ], Container( container_objects, it_container_type_types->second[ k ] ) ) );
                }
              }
            }
          }
        }
      }
      break;
    case( SYMBOL_TYPE_CONCRETE ):
    case( NUM_SYMBOL_TYPES ):
    default:
      break;
    }
  }

  return;
}

/**
 * exports the Region_Container class to an XML file. 
 */
void
Region_Container::
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
 * exports the Region_Container class to an XML node pointer. 
 */
void
Region_Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "region_container" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "region_container_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "region_container_type" ).c_str() ) );
  _container.to_xml( doc, node );
  
  xmlAddChild( root, node );
  return;
}

/**
 * imports the Region_Container class from an XML file. 
 */
void
Region_Container::
from_xml( const string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  root = xmlDocGetRootElement( doc );
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "region_container" ) ) == 0 ){
          from_xml( l1 );
        }
      }
    }
  }
  xmlFreeDoc( doc );
  return;
}

/**
 * imports the Region_Container class from an XML node pointer. 
 */
void
Region_Container::
from_xml( xmlNodePtr root ){
  relation_type() = "na";
  _container = Container();

  if( root->type == XML_ELEMENT_NODE ){
    vector< string > symbol_keys = { "type", "region_container_type" };
    assert( check_keys( root, symbol_keys ) );

    pair< bool, string > region_container_type_prop = has_prop< std::string >( root, "region_container_type" );
    if( region_container_type_prop.first ) {
        relation_type() = region_container_type_prop.second;
    }
    
    region_container_type_prop = has_prop< std::string >( root, "type" );
    if( region_container_type_prop.first ) {
        relation_type() = region_container_type_prop.second;
    }
  
    for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
            if( xmlStrcmp( l1->name, ( const xmlChar* )( "container" ) ) == 0 ){
                _container.from_xml( l1 );
            }
        }
    }

  }
  return;
}

namespace h2sl {

  /**
   * Region_Container class ostream operator
   */

  ostream&
  operator<<( ostream& out,
              const Region_Container& other ){
    out << "Region_Container(";
    out << "region_container_type=\"" << other.relation_type() << "\",";
      out << "container=" << other.container();
    out << ")";
    return out;
  }
}


