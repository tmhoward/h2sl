/**
 * @file rule_number.cc
 *
 * @brief
 *
 * a class used to describe a spatial relationship
 */

#include <assert.h>

#include "h2sl/rule_number.h"

using namespace std;
using namespace h2sl;

/**
 * Rule_Number class constructor
 */
Rule_Number::
Rule_Number( const int& numberValue ) : Rule() {
  insert_prop< int >( _int_properties, "number", numberValue );
}

Rule_Number::
Rule_Number( xmlNodePtr root, World* world ) : Rule() {
  insert_prop< int >( _int_properties, "number", 0 );
  from_xml( root, world );
}

/**
 * Rule_Number class copy constructor
 */
Rule_Number::
Rule_Number( const Rule_Number& other ) : Rule( other ) {

}

/**
 * Rule_Number class destructor
 */
Rule_Number::
~Rule_Number() {

}

/** 
 * Rule_Number class assignment operator
 */
Rule_Number&
Rule_Number::
operator=( const Rule_Number& other ){
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/** 
 * Rule_Number class equality operator
 */
bool
Rule_Number::
operator==( const Rule_Number& other )const{
  if ( number_value() != other.number_value() ){
    return false;
  } else {
    return true;
  }
}

/** 
 * Rule_Number class inequality operator
 */
bool
Rule_Number::
operator!=( const Rule_Number& other )const{
  return !( *this == other );
}

/** 
 * Rule_Number class dup operator
 */
Rule_Number*
Rule_Number::
dup( void )const{
  return new Rule_Number( *this );
}

string
Rule_Number::
evaluate_cv( const Grounding_Set* groundingSet )const{
  string cv = "false";
  for( unsigned int i = 0; i < groundingSet->groundings().size(); i++ ){
    if( dynamic_cast< const Rule_Number* >( groundingSet->groundings()[ i ] ) ){
      if( *this == *static_cast< const Rule_Number* >( groundingSet->groundings()[ i ] ) ){
        cv = "true";
      }
    }
  }
  return cv;
}

void
Rule_Number::
scrape_grounding( const World * world,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< int >( "number", number_value(), intTypes );
  return;
}

void
Rule_Number::
scrape_grounding( const World * world,
                  vector< string >& classNames,
                  map< string, vector< string > >& stringTypes,
                  map< string, vector< int > >& intTypes )const{
  insert_unique< std::string >( class_name(), classNames );
  scrape_grounding( world, stringTypes, intTypes );
  return;
}

void
Rule_Number::
fill_search_space( const Symbol_Dictionary& symbolDictionary,
                    const World* world,
                    map< string, pair< string, vector< Grounding* > > >& searchSpaces,
                    const std::string& symbolType ){
  if( symbolDictionary.has_class_name( "rule_number" ) || symbolDictionary.has_class_name( "number" ) ){
    map< string, pair< string, vector< Grounding* > > >::iterator it_search_spaces_symbol = searchSpaces.find( class_name() );
    if( it_search_spaces_symbol == searchSpaces.end() ){
      searchSpaces.insert( pair< string, pair< string, vector< Grounding* > > >( class_name(), pair< string, vector< Grounding* > >( "binary", vector< Grounding* >() ) ) );
      it_search_spaces_symbol = searchSpaces.find( class_name() );
    }

    map< string, vector< int > >::const_iterator it_number_value_values = symbolDictionary.int_types().find( "number" );

    if( ( symbolType == "concrete" ) || ( symbolType == "all" ) ){
      if( it_number_value_values != symbolDictionary.int_types().end() ){
        for( unsigned int i = 0; i < it_number_value_values->second.size(); i++ ){
          it_search_spaces_symbol->second.second.push_back( new Rule_Number( it_number_value_values->second[ i ] ) );
        }
      }
    }
  }

  return;
}

void
Rule_Number::
fill_rules( const World* world, Grounding_Set* groundingSet )const{
  Rule_Number rule_number( number_value() );
  insert_unique_grounding< Rule_Number >( groundingSet, rule_number );
  return;
}

bool
Rule_Number::
equals( const Grounding& other )const{
  if( dynamic_cast< const Rule_Number* >( &other ) != NULL ){
    return ( *this == *static_cast< const Rule_Number* >( &other ) );
  } else {
    return false;
  }
}

/** 
 * imports the Rule_Number class from an XML file
 */
void
Rule_Number::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "rule_number" ) ) == 0 ){
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
 * imports the Rule_Number class from an XML node pointer
 */
void
Rule_Number::
from_xml( xmlNodePtr root, World* world ){
  number_value() = 0;
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > number_keys = { "value", "number_value" };
    assert( check_keys( root, number_keys ) );
    pair< bool, int > number_value_prop = has_prop< int >( root, "number_value" );
    if( number_value_prop.first ){
      number_value() = number_value_prop.second;
    }
    number_value_prop = has_prop< int >( root, "value" );
    if( number_value_prop.first ){
      number_value() = number_value_prop.second;
    }
  }
  return;
}

/**
 * exports the Rule_Number class to an XML file
 */
void
Rule_Number::
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
 * exports the Rule_Number class to an XML node pointer
 */
void
Rule_Number::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "rule_number" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "number_value" ), ( const xmlChar* )( to_std_string( get_prop< int >( _int_properties, "number" ) ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

string
Rule_Number::
to_latex( void )const{
  stringstream tmp;
  tmp << "Rule_Number(value=" << number_value() << ")";
  return tmp.str();
}



namespace h2sl {
  /** 
   * Rule_Number class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Rule_Number& other ){
    out << "Rule_Number(";
    out << "number_value=\"" << other.number_value() << "\"";
    out << ")";
    return out;
  }
}
