/**
 * @file rule_index.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/rule_index.h"

using namespace std;
using namespace h2sl;

/**
 * Rule_Index class constructor
 */
Rule_Index::
Rule_Index( const int& indexValue ) : Rule() {
  insert_prop< int >( _int_properties, "index", indexValue );
}

Rule_Index::
Rule_Index( xmlNodePtr root, World* world ) : Rule() {
  insert_prop< int >( _int_properties, "index", 0 );
  from_xml( root, world );
}

/**
 * Rule_Index class copy constructor
 */
Rule_Index::
Rule_Index( const Rule_Index& other ) : Rule( other ) {

}

/**
 * Rule_Index class destructor
 */
Rule_Index::
~Rule_Index() {

}

/** 
 * Rule_Index class assignment operator
 */
Rule_Index&
Rule_Index::
operator=( const Rule_Index& other ){
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/** 
 * Rule_Index class equality operator
 */
bool
Rule_Index::
operator==( const Rule_Index& other )const{
  if ( index_value() != other.index_value() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Rule_Index class inequality operator
 */
bool
Rule_Index::
operator!=( const Rule_Index& other )const{
  return !( *this == other );
}

/** 
 * Rule_Index class dup operator
 */
Rule_Index*
Rule_Index::
dup( void )const{
  return new Rule_Index( *this );
}

string
Rule_Index::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Rule_Index* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Rule_Index* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Rule_Index::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< int >( "index", index_value(), intTypes );
  return;
}

void
Rule_Index::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Rule_Index::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const std::string& symbolType ){
  if( symbolDictionary.has_class_name( "rule_index" ) || symbolDictionary.has_class_name( "index" ) || symbolDictionary.has_class_name( "abstract_container" ) || symbolDictionary.has_class_name( "region_abstract_container" ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "binary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< int > >::const_iterator it_index_value_values = symbolDictionary.int_types().find( "index" );

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( it_index_value_values != symbolDictionary.int_types().end() ){
        for( unsigned int i = 0; i < it_index_value_values->second.size(); i++ ){
          it_search_spaces_symbol->second.second.push_back( new Rule_Index( it_index_value_values->second[ i ] ) );
        }
      }
    }
  }

  return;
}

void
Rule_Index::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Index rule_index( index_value() );
  insert_unique_grounding< Rule_Index >( groundingSet, rule_index );
  return;
}

bool
Rule_Index::
equals( const Grounding& other )const{
  if( dynamic_cast< const Rule_Index* >( &other ) != NULL ){
    return ( *this == *static_cast< const Rule_Index* >( &other ) );
  } else {
    return false;
  }
}

/** 
 * imports the Rule_Index class from an XML file
 */
void
Rule_Index::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_index" ) ) == 0 ){
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
 * imports the Rule_Index class from an XML node pointer
 */
void
Rule_Index::
from_xml( xmlNodePtr root, World* world ){
  index_value() = 0;
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > index_keys = { "value", "index" };
    assert( check_keys( root, index_keys ) );
    pair< bool, int > index_value_prop = has_prop< int >( root, "index" );
    if( index_value_prop.first ){
      index_value() = index_value_prop.second;
    }
    index_value_prop = has_prop< int >( root, "value" );
    if( index_value_prop.first ){
      index_value() = index_value_prop.second;
    }
  }
  return;
}

/**
 * exports the Rule_Index class to an XML file
 */
void
Rule_Index::
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
 * exports the Rule_Index class to an XML node pointer
 */
void
Rule_Index::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "rule_index" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "index" ), ( const xmlChar* )( to_std_string( get_prop< int >( _int_properties, "index" ) ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Rule_Index::
to_latex( void )const{
  stringstream tmp;
  tmp << "Rule_Index(value=" << index_value() << ")";
  return tmp.str();
}

namespace h2sl {
  /** 
   * Rule_Index class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Rule_Index& other ){
    out << "Rule_Index(";
    out << "index=\"" << other.index_value() << "\"";
    out << ")";
    return out;
  }
}
