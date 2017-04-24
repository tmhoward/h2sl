/**
 * @file rule_constraint_payload_type.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/rule_constraint_payload_type.h"

using namespace std;
using namespace h2sl;

/**
 * Rule_Constraint_Payload_Type class constructor
 */
Rule_Constraint_Payload_Type::
Rule_Constraint_Payload_Type( const string& constraintPayloadType ) : Rule() {
  insert_prop< std::string >( _string_properties, "constraint_payload_type", constraintPayloadType );
}

Rule_Constraint_Payload_Type::
Rule_Constraint_Payload_Type( xmlNodePtr root, World* world ) : Rule() {
  insert_prop< std::string >( _string_properties, "constraint_payload_type", "na" );
  from_xml( root, world );
}

/**
 * Rule_Constraint_Payload_Type class copy constructor
 */
Rule_Constraint_Payload_Type::
Rule_Constraint_Payload_Type( const Rule_Constraint_Payload_Type& other ) : Rule( other ) {

}

/**
 * Rule_Constraint_Payload_Type class destructor
 */
Rule_Constraint_Payload_Type::
~Rule_Constraint_Payload_Type() {

}

/** 
 * Rule_Constraint_Payload_Type class assignment operator
 */
Rule_Constraint_Payload_Type&
Rule_Constraint_Payload_Type::
operator=( const Rule_Constraint_Payload_Type& other ){
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/** 
 * Rule_Constraint_Payload_Type class equality operator
 */
bool
Rule_Constraint_Payload_Type::
operator==( const Rule_Constraint_Payload_Type& other )const{
  if ( constraint_payload_type() != other.constraint_payload_type() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Rule_Constraint_Payload_Type class inequality operator
 */
bool
Rule_Constraint_Payload_Type::
operator!=( const Rule_Constraint_Payload_Type& other )const{
  return !( *this == other );
}

/** 
 * Rule_Constraint_Payload_Type class dup operator
 */
Rule_Constraint_Payload_Type*
Rule_Constraint_Payload_Type::
dup( void )const{
  return new Rule_Constraint_Payload_Type( *this );
}

string
Rule_Constraint_Payload_Type::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Rule_Constraint_Payload_Type* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Rule_Constraint_Payload_Type* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Rule_Constraint_Payload_Type::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( "constraint_payload_type", constraint_payload_type(), stringTypes );
  return;
}

void
Rule_Constraint_Payload_Type::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Rule_Constraint_Payload_Type::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const std::string& symbolType ){

  if( symbolDictionary.has_class_name( class_name() ) || symbolDictionary.has_class_name( "constraint" ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "binary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< string > >::const_iterator it_constraint_payload_type_types = symbolDictionary.string_types().find( "constraint_payload_type" );

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( it_constraint_payload_type_types != symbolDictionary.string_types().end() ){
        for( unsigned int i = 0; i < it_constraint_payload_type_types->second.size(); i++ ){
          it_search_spaces_symbol->second.second.push_back( new Rule_Constraint_Payload_Type( it_constraint_payload_type_types->second[ i ] ) );
        }
      }
    }
  }

  return;
}

/** 
 * imports the Rule_Constraint_Payload_Type class from an XML file
 */
void
Rule_Constraint_Payload_Type::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_constraint_payload_type" ) ) == 0 ){
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

void
Rule_Constraint_Payload_Type::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Constraint_Payload_Type rule_constraint_payload_type( constraint_payload_type() );
  insert_unique_grounding< Rule_Constraint_Payload_Type >( groundingSet, rule_constraint_payload_type );
  return;
}

bool
Rule_Constraint_Payload_Type::
equals( const Grounding& other )const{
  if( dynamic_cast< const Rule_Constraint_Payload_Type* >( &other ) != NULL ){
    return ( *this == *static_cast< const Rule_Constraint_Payload_Type* >( &other ) );
  } else {
    return false;
  }
}

/** 
 * imports the Rule_Constraint_Payload_Type class from an XML node pointer
 */
void
Rule_Constraint_Payload_Type::
from_xml( xmlNodePtr root, World* world ){
  constraint_payload_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > constraint_payload_type_keys = { "type", "constraint_payload_type" };
    assert( check_keys( root, constraint_payload_type_keys ) );
    pair< bool, string > constraint_payload_type_prop = has_prop< std::string >( root, "constraint_payload_type" );
    if( constraint_payload_type_prop.first ){
      constraint_payload_type() = constraint_payload_type_prop.second;
    }
    constraint_payload_type_prop = has_prop< std::string >( root, "type" );
    if( constraint_payload_type_prop.first ){
      constraint_payload_type() = constraint_payload_type_prop.second;
    }
  }
  return;
}

/**
 * exports the Rule_Constraint_Payload_Type class to an XML file
 */
void
Rule_Constraint_Payload_Type::
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
 * exports the Rule_Constraint_Payload_Type class to an XML node pointer
 */
void
Rule_Constraint_Payload_Type::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "rule_constraint_payload_type" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "constraint_payload_type" ), ( const xmlChar* )( get_prop< std::string >( _string_properties, "constraint_payload_type" ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Rule_Constraint_Payload_Type::
to_latex( void )const{
  stringstream tmp;
  tmp << "Rule_Constraint_Payload_Type(constraint_payload_type=" << constraint_payload_type() << ")";
  return tmp.str();
}

namespace h2sl {
  /** 
   * Rule_Constraint_Payload_Type class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Rule_Constraint_Payload_Type& other ){
    out << "Rule_Constraint_Payload_Type(";
    out << "constraint_payload_type=\"" << other.constraint_payload_type() << "\"";
    out << ")";
    return out;
  }
}
