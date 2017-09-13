/**
 * @file region_container.cc
 * 
 * @brief
 *
 * a class that describes the Region_Container class.
 */

#include <sstream>
#include <cmath>
#include "h2sl/rule_object_type.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/rule_container_type.h"
#include "h2sl/region_container.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Region_Container class constructor. 
 */
Region_Container::
Region_Container( const string& spatialRelationType,
                 const Container& container ) : Grounding() ,
                                                    _container( container ) {
            insert_prop< std::string >( _string_properties, "spatial_relation_type", spatialRelationType);
}

/**
 * Region_Container class destructor. 
 */
Region_Container::
~Region_Container() {

}

Region_Container::
Region_Container( xmlNodePtr root, World* world ) : Grounding(),
                                       _container() {
    insert_prop< std::string >( _string_properties, "spatial_relation_type", "na" );
    from_xml( root, world );
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

string
Region_Container::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Region_Container* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Region_Container* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Region_Container::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "spatial_relation_type", relation_type(), stringTypes );
  _container.scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Region_Container::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Region_Container::
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

    map< string, vector< string > >::const_iterator it_object_type_types = symbolDictionary.string_types().find( "object_type" );
    map< string, vector< string > >::const_iterator it_container_type_types = symbolDictionary.string_types().find( "container_type" );
    map< string, vector< string > >::const_iterator it_spatial_relation_type_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    map< string, vector< int > >::const_iterator it_number_types = symbolDictionary.int_types().find( "number" );

    if(  symbolType == "all" ){
      if( ( it_object_type_types != symbolDictionary.string_types().end() ) && ( it_spatial_relation_type_types != symbolDictionary.string_types().end() ) ){
        if( it_container_type_types != symbolDictionary.string_types().end() ){
          for( unsigned int k = 0; k < it_container_type_types->second.size(); k++ ){
            for( unsigned int l = 0; l < it_spatial_relation_type_types->second.size(); l++ ){
              it_search_spaces_symbol->second.second.push_back( new Region_Container( it_spatial_relation_type_types->second[ l ], Container( vector< Grounding* >(), it_container_type_types->second[ k ] ) ) );
            }
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
              if( it_container_type_types != symbolDictionary.string_types().end() ){
                for( unsigned int k = 0; k < it_container_type_types->second.size(); k++ ){
                  for( unsigned int l = 0; l < it_spatial_relation_type_types->second.size(); l++ ){
                    it_search_spaces_symbol->second.second.push_back( new Region_Container( it_spatial_relation_type_types->second[ l ], Container( container_objects, it_container_type_types->second[ k ] ) ) );
                  }
                }
              } else {
                for( unsigned int l = 0; l < it_spatial_relation_type_types->second.size(); l++ ){
                  it_search_spaces_symbol->second.second.push_back( new Region_Container( it_spatial_relation_type_types->second[ l ], Container( container_objects, "group" ) ) );
                }
              }
            }
          }
        }
      }
    } else if ( symbolType == "abstract" ) {
      if( ( it_object_type_types != symbolDictionary.string_types().end() ) && ( it_spatial_relation_type_types != symbolDictionary.string_types().end() ) ){
        for( unsigned int i = 0 ; i < it_object_type_types->second.size(); i++ ){
          vector< Object* > objects;
          for( map< string, Object* >::const_iterator it_world_object = world->objects().begin(); it_world_object != world->objects().end(); it_world_object++ ){
            if( it_world_object->second->type() == it_object_type_types->second[ i ] ){
              objects.push_back( it_world_object->second );
            }
          }

          vector< unsigned int > numbers;
          if( it_number_types != symbolDictionary.int_types().end() ){
            for( unsigned int k = 0; k < it_number_types->second.size(); k++ ){
              numbers.push_back( it_number_types->second[ k ] );
            }
          }

          if( numbers.empty() ){
            vector< Grounding* > container_objects;
            for( unsigned int k = 0; k < objects.size(); k++ ){
              container_objects.push_back( objects[ k ] );
            }
            if( it_container_type_types != symbolDictionary.string_types().end() ){
              for( unsigned int k = 0; k < it_container_type_types->second.size(); k++ ){
                for( unsigned int l = 0; l < it_spatial_relation_type_types->second.size(); l++ ){
                  it_search_spaces_symbol->second.second.push_back( new Region_Container( it_spatial_relation_type_types->second[ l ], Container( container_objects, it_container_type_types->second[ k ] ) ) );
                }
              }
            } else {
              for( unsigned int l = 0; l < it_spatial_relation_type_types->second.size(); l++ ){
                it_search_spaces_symbol->second.second.push_back( new Region_Container( it_spatial_relation_type_types->second[ l ], Container( container_objects, "group" ) ) );
              }
            }

          } else {
            const unsigned int num_sets = pow( 2, objects.size() );
            for( unsigned int j = 0; j < num_sets; j++ ){
              vector< Grounding* > container_objects;
              for( unsigned int k = 0; k < objects.size(); k++ ){
                int mask = 1 << k;
                if( mask & j ){
                  container_objects.push_back( dynamic_cast< Grounding* >( objects[ k ] ) );
                }
              }
              if( ( container_objects.size() > 1 ) && ( ( container_objects.size() == objects.size() ) || has_in_vector< unsigned int >( container_objects.size(), numbers ) ) ){
                if( it_container_type_types != symbolDictionary.string_types().end() ){
                  for( unsigned int k = 0; k < it_container_type_types->second.size(); k++ ){
                    for( unsigned int l = 0; l < it_spatial_relation_type_types->second.size(); l++ ){
                      it_search_spaces_symbol->second.second.push_back( new Region_Container( it_spatial_relation_type_types->second[ l ], Container( container_objects, it_container_type_types->second[ k ] ) ) );
                    }
                  }
                //}
                //if( it_container_type_types != symbolDictionary.string_types().end() ){
                //  for( unsigned int k = 0; k < it_container_type_types->second.size(); k++ ){
                //    for( unsigned int l = 0; l < it_spatial_relation_type_types->second.size(); l++ ){
                //      it_search_spaces_symbol->second.second.push_back( new Region_Container( it_spatial_relation_type_types->second[ l ], Container( container_objects, it_container_type_types->second[ k ] ) ) );
                //    }
                //  }
                } else {
                  for( unsigned int l = 0; l < it_spatial_relation_type_types->second.size(); l++ ){
                    it_search_spaces_symbol->second.second.push_back( new Region_Container( it_spatial_relation_type_types->second[ l ], Container( container_objects, "group" ) ) );
                  }
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
Region_Container::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Container_Type rule_container_type( _container.type() );
  insert_unique_grounding< Rule_Container_Type >( groundingSet, rule_container_type );
  Rule_Spatial_Relation rule_spatial_relation( relation_type() );
  insert_unique_grounding< Rule_Spatial_Relation >( groundingSet, rule_spatial_relation );
  for( vector< Grounding* >::const_iterator it_grounding = _container.container().begin(); it_grounding != _container.container().end(); it_grounding++ ){
    if( (*it_grounding)->matches_class_name( "object" ) ){
      Rule_Object_Type rule_object_type( static_cast< Object* >( *it_grounding )->type() );
      insert_unique_grounding< Rule_Object_Type >( groundingSet, rule_object_type );
    }
  }
  return;
}

bool
Region_Container::
equals( const Grounding& other )const{
  if( dynamic_cast< const Region_Container* >( &other ) != NULL ){
    return ( *this == *static_cast< const Region_Container* >( &other ) );
  } else {
    return false;
  }
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
  xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "spatial_relation_type" ).c_str() ) );
  _container.to_xml( doc, node );
  
  xmlAddChild( root, node );
  return;
}

string
Region_Container::
to_latex( void )const{
  stringstream tmp;
  tmp << "Region_Container(spatial\\_relation\\_type=" << relation_type() << "," << _container.to_latex() << ")";
  return tmp.str();
}

/**
 * imports the Region_Container class from an XML file. 
 */
void
Region_Container::
from_xml( const string& filename, World* world ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  root = xmlDocGetRootElement( doc );
  if( root->type == XML_ELEMENT_NODE ){
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "region_container" ) ) == 0 ){
          from_xml( l1, world );
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
from_xml( xmlNodePtr root, World* world ){
  relation_type() = "na";
  _container = Container();

  if( root->type == XML_ELEMENT_NODE ){
    vector< string > symbol_keys = { "type", "spatial_relation_type", "region_container_type" };
    assert( check_keys( root, symbol_keys ) );

    pair< bool, string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ) {
        relation_type() = spatial_relation_type_prop.second;
    }

    spatial_relation_type_prop = has_prop< std::string >( root, "region_container_type" );
    if( spatial_relation_type_prop.first ) {
        relation_type() = spatial_relation_type_prop.second;
    }

    spatial_relation_type_prop = has_prop< std::string >( root, "type" );
    if( spatial_relation_type_prop.first ) {
        relation_type() = spatial_relation_type_prop.second;
    }
  
    for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
            if( xmlStrcmp( l1->name, ( const xmlChar* )( "container" ) ) == 0 ){
                _container.from_xml( l1, world );
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
    out << "spatial_relation_type=\"" << other.relation_type() << "\",";
      out << "container=" << other.container();
    out << ")";
    return out;
  }
}


