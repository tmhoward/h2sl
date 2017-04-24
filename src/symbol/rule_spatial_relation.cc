/**
 * @file rule_spatial_relation.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/rule_spatial_relation.h"

using namespace std;
using namespace h2sl;

/**
 * Rule_Spatial_Relation class constructor
 */
Rule_Spatial_Relation::
Rule_Spatial_Relation( const string& spatialRelationType ) : Rule() {
  insert_prop< std::string >( _string_properties, "spatial_relation_type", spatialRelationType );
}

Rule_Spatial_Relation::
Rule_Spatial_Relation( xmlNodePtr root, World* world ) : Rule() {
  insert_prop< std::string >( _string_properties, "spatial_relation_type", "na" );
  from_xml( root, world );
}

/**
 * Rule_Spatial_Relation class copy constructor
 */
Rule_Spatial_Relation::
Rule_Spatial_Relation( const Rule_Spatial_Relation& other ) : Rule( other ) {

}

/**
 * Rule_Spatial_Relation class destructor
 */
Rule_Spatial_Relation::
~Rule_Spatial_Relation() {

}

/** 
 * Rule_Spatial_Relation class assignment operator
 */
Rule_Spatial_Relation&
Rule_Spatial_Relation::
operator=( const Rule_Spatial_Relation& other ){
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/** 
 * Rule_Spatial_Relation class equality operator
 */
bool
Rule_Spatial_Relation::
operator==( const Rule_Spatial_Relation& other )const{
  if ( spatial_relation_type() != other.spatial_relation_type() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Rule_Spatial_Relation class inequality operator
 */
bool
Rule_Spatial_Relation::
operator!=( const Rule_Spatial_Relation& other )const{
  return !( *this == other );
}

/** 
 * Rule_Spatial_Relation class dup operator
 */
Rule_Spatial_Relation*
Rule_Spatial_Relation::
dup( void )const{
  return new Rule_Spatial_Relation( *this );
}

string
Rule_Spatial_Relation::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Rule_Spatial_Relation* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Rule_Spatial_Relation* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Rule_Spatial_Relation::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "spatial_relation_type", spatial_relation_type(), stringTypes );
  return;
}

void
Rule_Spatial_Relation::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Rule_Spatial_Relation::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const std::string& symbolType ){
  if( symbolDictionary.has_class_name( class_name() ) || symbolDictionary.has_class_name( "spatial_relation" ) || symbolDictionary.has_class_name( "region" ) || symbolDictionary.has_class_name( "constraint" ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "binary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< string > >::const_iterator it_spatial_relation_type_types = symbolDictionary.string_types().find( "spatial_relation_type" );

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( it_spatial_relation_type_types != symbolDictionary.string_types().end() ){
        for( unsigned int i = 0; i < it_spatial_relation_type_types->second.size(); i++ ){
          it_search_spaces_symbol->second.second.push_back( new Rule_Spatial_Relation( it_spatial_relation_type_types->second[ i ] ) );
        }
      }
    }
  }

  return;
}

void
Rule_Spatial_Relation::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Spatial_Relation rule_spatial_relation( spatial_relation_type() );
  insert_unique_grounding< Rule_Spatial_Relation >( groundingSet, rule_spatial_relation );
  return;
}

bool
Rule_Spatial_Relation::
equals( const Grounding& other )const{
  if( dynamic_cast< const Rule_Spatial_Relation* >( &other ) != NULL ){
    return ( *this == *static_cast< const Rule_Spatial_Relation* >( &other ) );
  } else {
    return false;
  }
}

/** 
 * imports the Rule_Spatial_Relation class from an XML file
 */
void
Rule_Spatial_Relation::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_spatial_relation" ) ) == 0 ){
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
 * imports the Rule_Spatial_Relation class from an XML node pointer
 */
void
Rule_Spatial_Relation::
from_xml( xmlNodePtr root, World* world ){
  spatial_relation_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > spatial_relation_keys = { "type", "spatial_relation_type" };
    assert( check_keys( root, spatial_relation_keys ) );
    pair< bool, string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ){
      spatial_relation_type() = spatial_relation_type_prop.second;
    }
    spatial_relation_type_prop = has_prop< std::string >( root, "type" );
    if( spatial_relation_type_prop.first ){
      spatial_relation_type() = spatial_relation_type_prop.second;
    }
  }
  return;
}

/**
 * exports the Rule_Spatial_Relation class to an XML file
 */
void
Rule_Spatial_Relation::
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
 * exports the Rule_Spatial_Relation class to an XML node pointer
 */
void
Rule_Spatial_Relation::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "rule_spatial_relation" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "spatial_relation_type").c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Rule_Spatial_Relation::
to_latex( void )const{
  stringstream tmp;
  tmp << "Rule\\_Spatial\\_Relation(spatial\\_relation\\_type=" << spatial_relation_type() << ")";
  return tmp.str();
}

namespace h2sl {
  /** 
   * Rule_Spatial_Relation class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Rule_Spatial_Relation& other ){
    out << "Rule_Spatial_Relation(";
    out << "spatial_relation_type=\"" << other.spatial_relation_type() << "\"";
    out << ")";
    return out;
  }
}
