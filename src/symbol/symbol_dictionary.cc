/**
 * @file    symbol_dictionary.cc
 * @author  Thomas M. Howard (tmhoward@csail.mit.edu)
 *          Matthew R. Walter (mwalter@csail.mit.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
 *
 * Copyright (C) 2014 by the Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * @section DESCRIPTION
 *
 * The implementation of a class used to represent a Distributed 
 *   Correspondence Graph
 */

#include "h2sl/common.h"
#include "h2sl/symbol_dictionary.h"

using namespace std;
using namespace h2sl;

/**
 * Symbol_Dictionary class constructor
 */
Symbol_Dictionary::
Symbol_Dictionary( const map< string, vector< string > >& classNames,
                    const map< string, vector< string > >& stringTypes,
                    const map< string, vector< int > >& intTypes ) : _class_names( classNames ),
                                                                      _string_types( stringTypes ),
                                                                      _int_types( intTypes ) {
}

/**
 * Symbol_Dictionary class constructor
 */
Symbol_Dictionary::
Symbol_Dictionary( const string& filename ) : _class_names(),
                                              _string_types(),
                                              _int_types() {
  from_xml( filename );
}

/**
 * Symbol_Dictionary class destructor 
 */
Symbol_Dictionary::
~Symbol_Dictionary() {

}

/**
 * Symbol_Dictionary copy constructor
 */
Symbol_Dictionary::
Symbol_Dictionary( const Symbol_Dictionary& other,
                    const string& symbolType ) : _class_names( other._class_names ),
                                                 _string_types(),
                                                 _int_types() {
  vector< string > string_properties_keys_concrete = { "object_type", "object_color", "spatial_relation_type", "constraint_payload_type", "constraint_reference_type", "constraint_type", "container_type" };
  vector< string > int_properties_keys_concrete = { "index", "number" };
  vector< string > string_properties_keys_abstract = {}; 
  vector< string > int_properties_keys_abstract = {};

  if( symbolType == "all" ){
    _string_types = other._string_types;
    _int_types = other._int_types;
  } else if ( symbolType == "concrete" ) {
    for( map< string, vector< string > >::const_iterator it_string_types = other.string_types().begin(); it_string_types != other.string_types().end(); it_string_types++ ){
      for( vector< string >::const_iterator it_string_types_keys_concrete = string_properties_keys_concrete.begin(); it_string_types_keys_concrete != string_properties_keys_concrete.end(); it_string_types_keys_concrete++ ){
        if( it_string_types->first == *it_string_types_keys_concrete ){
          _string_types.insert( pair< string, vector< string > >( it_string_types->first, it_string_types->second ) );
        }
      }
    }
    for( map< string, vector< int > >::const_iterator it_int_types = other.int_types().begin(); it_int_types != other.int_types().end(); it_int_types++ ){
      for( vector< string >::const_iterator it_int_types_keys_concrete = int_properties_keys_concrete.begin(); it_int_types_keys_concrete != int_properties_keys_concrete.end(); it_int_types_keys_concrete++ ){
        if( it_int_types->first == *it_int_types_keys_concrete ){
          _int_types.insert( pair< string, vector< int > >( it_int_types->first, it_int_types->second ) );
        }
      }
    }
  } else if ( symbolType == "abstract" ) {
    for( map< string, vector< string > >::const_iterator it_string_types = other.string_types().begin(); it_string_types != other.string_types().end(); it_string_types++ ){
      for( vector< string >::const_iterator it_string_types_keys_abstract = string_properties_keys_abstract.begin(); it_string_types_keys_abstract != string_properties_keys_abstract.end(); it_string_types_keys_abstract++ ){
        if( it_string_types->first == *it_string_types_keys_abstract ){
          _string_types.insert( pair< string, vector< string > >( it_string_types->first, it_string_types->second ) );
        }
      }
    }
    for( map< string, vector< int > >::const_iterator it_int_types = other.int_types().begin(); it_int_types != other.int_types().end(); it_int_types++ ){ 
      for( vector< string >::const_iterator it_int_types_keys_abstract = int_properties_keys_abstract.begin(); it_int_types_keys_abstract != int_properties_keys_abstract.end(); it_int_types_keys_abstract++ ){ 
        if( it_int_types->first == *it_int_types_keys_abstract ){ 
          _int_types.insert( pair< string, vector< int > >( it_int_types->first, it_int_types->second ) ); 
        }
      }
    }
  } else {
    assert( false );
  }
}

/**
 * Symbol_Dictionary assignment operator. 
 */
Symbol_Dictionary&
Symbol_Dictionary::
operator=( const Symbol_Dictionary& other ) {
  _class_names = other._class_names;
  _string_types = other._string_types;
  _int_types = other._int_types;
  return (*this);
}

bool
Symbol_Dictionary::
operator==( const Symbol_Dictionary& other )const{
  if( class_names() != other.class_names() ){
    cout << "class names did not match" << endl;
    return false;
  } else if ( string_types().size() != other.string_types().size() ){
    cout << "string types size did not match" << endl;
    return false;
  } else if ( int_types().size() != other.int_types().size() ){
    cout << "int types size did not match" << endl;
    return false;
  } else {
    // check the string types for matches
    for( map< string, vector< string > >::const_iterator it_string_types = string_types().begin(); it_string_types != string_types().end(); it_string_types++ ){
      for( vector< string >::const_iterator it_string_type = it_string_types->second.begin(); it_string_type != it_string_types->second.end(); it_string_type++ ){
        bool found_match = false;
        // search the other map for an equivalent value
        for( map< string, vector< string > >::const_iterator it_other_string_types = other.string_types().begin(); it_other_string_types != other.string_types().end(); it_other_string_types++ ){
          if( it_string_types->first == it_other_string_types->first ){
            for( vector< string >::const_iterator it_other_string_type = it_other_string_types->second.begin(); it_other_string_type != it_other_string_types->second.end(); it_other_string_type++ ){
              if( (*it_string_type) == (*it_other_string_type ) ){
                found_match = true;
              }
            }
          }
        }
        if( !found_match ){
          cout << "did not find match for " << it_string_types->first << ":" << *it_string_type << endl;
          return false;
        }
      } 
    }
    // check the int types for matches
    for( map< string, vector< int > >::const_iterator it_int_types = int_types().begin(); it_int_types != int_types().end(); it_int_types++ ){
      for( vector< int >::const_iterator it_int_type = it_int_types->second.begin(); it_int_type != it_int_types->second.end(); it_int_type++ ){
        bool found_match = false;
        // search the other map for an equivalent value
        for( map< string, vector< int > >::const_iterator it_other_int_types = other.int_types().begin(); it_other_int_types != other.int_types().end(); it_other_int_types++ ){
          if( it_int_types->first == it_other_int_types->first ){
            for( vector< int >::const_iterator it_other_int_type = it_other_int_types->second.begin(); it_other_int_type != it_other_int_types->second.end(); it_other_int_type++ ){
              if( (*it_int_type) == (*it_other_int_type ) ){
                found_match = true;
              }
            }
          }
        }
        if( !found_match ){
          cout << "did not find match for " << it_int_types->first << ":" << *it_int_type << endl;
          return false;
        }
      }
    }
    return true;
  }
}

bool
Symbol_Dictionary::
operator!=( const Symbol_Dictionary& other )const{
  return !( *this == other );
}

/**
 * looks for a class name
 */
bool
Symbol_Dictionary::
has_class_name( const string& className )const{
  for( map< string, vector< string > >::const_iterator it_class_names = _class_names.begin(); it_class_names != _class_names.end(); it_class_names++ ){
    for( vector< string >::const_iterator it_class_name = it_class_names->second.begin(); it_class_name != it_class_names->second.end(); it_class_name++ ){
      if( *it_class_name == className ){
        return true;
      }
    }
  }
  return false;
}

/** 
 * imports the DCG class from an XML file
 */
bool
Symbol_Dictionary::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "symbol_dictionary" ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
      xmlFreeDoc( doc );
      return true;
    } else {
      xmlFreeDoc( doc );
      return false;
    }
  } else {
    return false;
  }
}

/** 
 * imports the class from an XML node pointer
 */
bool
Symbol_Dictionary::
from_xml( xmlNodePtr root ){
  _class_names.clear();
  _string_types.clear();
  _int_types.clear();
  if( root->type == XML_ELEMENT_NODE ){
    for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "class_names" ) ) == 0 ){
          for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
            if( l2->type == XML_ELEMENT_NODE ){
              string name = ( char* )( l2->name );
              pair< bool, string > class_names_prop = has_prop< std::string >( l2, "class_names" );
              if( class_names_prop.first ){
                _class_names.insert( pair< string, vector< string > >( name, std_vector_from_std_string< string >( class_names_prop.second ) ) );
              } 
            }
          } 
        } else if( xmlStrcmp( l1->name, ( const xmlChar* )( "string_property" ) ) == 0 ){
          pair< bool, string > key_prop = has_prop< std::string >( l1, "key" );
          pair< bool, string > value_prop = has_prop< std::string >( l1, "value" );
          if( key_prop.first && value_prop.first ){
            _string_types.insert( pair< string, vector< string > >( key_prop.second, std_vector_from_std_string< string >( value_prop.second ) ) );
          }
        } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "int_property" ) ) == 0 ){
          pair< bool, string > key_prop = has_prop< std::string >( l1, "key" );
          pair< bool, string > value_prop = has_prop< std::string >( l1, "value" );
          if( key_prop.first && value_prop.first ){
            _int_types.insert( pair< string, vector< int > >( key_prop.second, std_vector_from_std_string< int >( value_prop.second ) ) );
          }
        }
      }
    }
    return true;
  } else {
    return false;
  }
}

/**
 * exports the class to an XML file
 */
void
Symbol_Dictionary::
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
 * exports the DCG class to an XML node pointer
 */
void
Symbol_Dictionary::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "symbol_dictionary" ), NULL );
  xmlNodePtr class_names_node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "class_names" ), NULL );
  xmlAddChild( node, class_names_node );
  for( map< string, vector< string > >::const_iterator it_class_names = _class_names.begin(); it_class_names != _class_names.end(); it_class_names++ ){
    xmlNodePtr phrase_class_names_node = xmlNewDocNode( doc, NULL, ( xmlChar* )( it_class_names->first.c_str() ), NULL );
    xmlNewProp( phrase_class_names_node, ( const xmlChar* )( "class_names" ), ( const xmlChar* )( std_vector_to_std_string( it_class_names->second, false ).c_str() ) );
    xmlAddChild( class_names_node, phrase_class_names_node );
  }
  for( map< string, vector< string > >::const_iterator it = _string_types.begin(); it != _string_types.end(); it++ ){
    xmlNodePtr string_property_node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "string_property" ), NULL );
    xmlNewProp( string_property_node, ( const xmlChar* )( "key" ), ( const xmlChar* )( it->first.c_str() ) );        
    stringstream tmp_string;
    for( vector< string >::const_iterator it_value = it->second.begin(); it_value != it->second.end(); it_value++ ){
      tmp_string << *it_value;
      if( next( it_value ) != it->second.end() ){
        tmp_string << ",";
      }
    } 
    xmlNewProp( string_property_node, ( const xmlChar* )( "value" ), ( const xmlChar* )( tmp_string.str().c_str() ) );        
    xmlAddChild( node, string_property_node ); 
  }
  for( map< string, vector< int > >::const_iterator it = _int_types.begin(); it != _int_types.end(); it++ ){
    xmlNodePtr int_property_node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "int_property" ), NULL );
    xmlNewProp( int_property_node, ( const xmlChar* )( "key" ), ( const xmlChar* )( it->first.c_str() ) );
    stringstream tmp_string;
    for( vector< int >::const_iterator it_value = it->second.begin(); it_value != it->second.end(); it_value++ ){
      tmp_string << *it_value;
      if( next( it_value ) != it->second.end() ){
        tmp_string << ",";
      }
    }
    xmlNewProp( int_property_node, ( const xmlChar* )( "value" ), ( const xmlChar* )( tmp_string.str().c_str() ) );
    xmlAddChild( node, int_property_node );
  }
  xmlAddChild( root, node );
  return;
}


namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Symbol_Dictionary& other ) {
//    out << "class_names" << std_vector_to_std_string< string >( other.class_names() ) << ",";
    for( map< string, vector< string > >::const_iterator it = other.string_types().begin(); it != other.string_types().end(); it++ ){
      out << "string_property[" << it->first << "]" << std_vector_to_std_string< string >( it->second );
      if( ( next( it ) != other.string_types().end() ) || ( !other.int_types().empty() ) ){
        out << ",";
      }
    }    
    for( map< string, vector< int > >::const_iterator it = other.int_types().begin(); it != other.int_types().end(); it++ ){
      out << "int_property[" << it->first << "]" << std_vector_to_std_string< int >( it->second );
      if( next( it ) != other.int_types().end() ){
        out << ","; 
      }
    }
    return out;
  }
}
