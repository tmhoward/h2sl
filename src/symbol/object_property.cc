/**
 * @file object_property.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/rule_spatial_relation.h"
#include "h2sl/rule_object_type.h"
#include "h2sl/rule_index.h"
#include "h2sl/object_property.h"

using namespace std;
using namespace h2sl;

/**
 * Object_Property class constructor
 */
Object_Property::
Object_Property( const string& objectType,
                  const string& spatial_relationType,
                  const int& index ) : Grounding() {
    insert_prop< std::string >( _string_properties, "object_type", objectType );
    insert_prop< std::string >( _string_properties, "spatial_relation_type", spatial_relationType );
    insert_prop< int >( _int_properties, "index", index );

}

Object_Property::
Object_Property( xmlNodePtr root, World* world ) : Grounding() {
    insert_prop< std::string >( _string_properties, "object_type", "na" );
    insert_prop< std::string >( _string_properties, "spatial_relation_type", "na" );
    insert_prop< int >( _int_properties, "index", 0 );
    from_xml( root, world );
}


/**
 * Object_Property class copy constructor
 */
Object_Property::
Object_Property( const Object_Property& other ) : Grounding( other ) {

}

/**
 * Object_Property class destructor
 */
Object_Property::
~Object_Property() {

}

/** 
 * Object_Property class assignment operator
 */
Object_Property&
Object_Property::
operator=( const Object_Property& other ){
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/** 
 * Object_Property class equality operator
 */
bool
Object_Property::
operator==( const Object_Property& other )const{
  if ( type() != other.type() ){
    return false;
  } else if ( relation_type() != other.relation_type() ){
    return false;
  } else if ( index() != other.index() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Object_Property class inequality operator
 */
bool
Object_Property::
operator!=( const Object_Property& other )const{
  return !( *this == other );
}

/** 
 * Object_Property class dup operator
 */
Object_Property*
Object_Property::
dup( void )const{
  return new Object_Property( *this );
}

string
Object_Property::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Object_Property* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Object_Property* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Object_Property::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "object_type", type(), stringTypes );
  insert_unique< std::string >( "spatial_relation_type", relation_type(), stringTypes );
  insert_unique< int >( "index", index(), intTypes );
  return;
}

void
Object_Property::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Object_Property::
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
    map< string, vector< string > >::const_iterator it_spatial_relation_type_types = symbolDictionary.string_types().find( "spatial_relation_type" );
    map< string, vector< int > >::const_iterator it_index_values = symbolDictionary.int_types().find( "index" );

    if( ( symbolType == "abstract" ) || ( symbolType == "all" ) ){
      if( ( it_object_type_types != symbolDictionary.string_types().end() ) && ( it_spatial_relation_type_types != symbolDictionary.string_types().end() ) && ( it_index_values != symbolDictionary.int_types().end() ) ){
        for( unsigned int i = 0; i < it_object_type_types->second.size(); i++ ){
          for( unsigned int j = 0; j < it_spatial_relation_type_types->second.size(); j++ ){
            for( unsigned int k = 0; k < it_index_values->second.size(); k++ ){
              it_search_spaces_symbol->second.second.push_back( new Object_Property( it_object_type_types->second[ i ], it_spatial_relation_type_types->second[ j ], it_index_values->second[ k ] ) );
            }
          }
        }
      }
    }
  }

  return;
}

void
Object_Property::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Spatial_Relation rule_spatial_relation( relation_type() );
  insert_unique_grounding< Rule_Spatial_Relation >( groundingSet, rule_spatial_relation );
  Rule_Object_Type rule_object_type( type() );
  insert_unique_grounding< Rule_Object_Type >( groundingSet, rule_object_type );
  Rule_Index rule_index( index() );
  insert_unique_grounding< Rule_Index >( groundingSet, rule_index );
  return;
}

bool
Object_Property::
equals( const Grounding& other )const{
  if( dynamic_cast< const Object_Property* >( &other ) != NULL ){
    return ( *this == *static_cast< const Object_Property* >( &other ) );
  } else {
    return false;
  }
}

/** 
 * imports the Object_Property class from an XML file
 */
void
Object_Property::
from_xml( const string& filename, World* world ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "object_property" ) ) == 0 ){
            from_xml( l1, world );
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
 * imports the Object_Property class from an XML node pointer
 */
void
Object_Property::
from_xml( xmlNodePtr root, World* world ){
  type() = "na";
  relation_type() = "na";
  index() = 0;
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > object_property_keys = { "object_type", "relation_type", "spatial_relation_type", "index" };
    assert( check_keys( root, object_property_keys ) );
    pair< bool, string > object_type_prop = has_prop< std::string >( root, "object_type" );
    if( object_type_prop.first ){
      type() = object_type_prop.second;
    }
    pair< bool, string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ){
      relation_type() = spatial_relation_type_prop.second;
    }
    spatial_relation_type_prop = has_prop< std::string >( root, "relation_type" );
    if( spatial_relation_type_prop.first ){
      relation_type() = spatial_relation_type_prop.second;
    }
    pair< bool, int > index_prop = has_prop< int >( root, "index" );
    if( index_prop.first ){
      index() = index_prop.second;
    }
  }
  return;
}

/**
 * exports the Object_Property class to an XML file
 */
void
Object_Property::
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
 * exports the Object_Property class to an XML node pointer
 */
void
Object_Property::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "object_property" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "object_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "object_type").c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "spatial_relation_type").c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "index" ), ( const xmlChar* )( to_std_string( get_prop< int >( _int_properties, "index" ) ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Object_Property::
to_latex( void )const{
  stringstream tmp;
  tmp << "Object\\_Property(" << type() << "," << relation_type() << "," << index() << ")";
  return tmp.str();
}

namespace h2sl {
  /** 
   * Object_Property class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Object_Property& other ){
    out << "Object_Property(";
    out << "object_type=\"" << other.type() << "\",";
    out << "spatial_relation_type=\"" << other.relation_type() << "\",";
    out << "index=\"" << other.index() << "\",";
    out << ")";
    return out;
  }
}
