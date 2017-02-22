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
Symbol_Dictionary() : _string_types(),
                      _int_types() {

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
Symbol_Dictionary( const Symbol_Dictionary& other ) : _string_types( other._string_types ),
                                                      _int_types( other._int_types ) {

}

/**
 * Symbol_Dictionary assignment operator. 
 */
Symbol_Dictionary&
Symbol_Dictionary::
operator=( const Symbol_Dictionary& other ) {
  _string_types = other._string_types;
  _int_types = other._int_types;
  return (*this);
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
  _string_types.clear();
  _int_types.clear();
  if( root->type == XML_ELEMENT_NODE ){
    for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "string_property" ) ) == 0 ){
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
  for( map< string, vector< string > >::const_iterator it = _string_types.begin(); it != _string_types.end(); it++ ){
    xmlNodePtr string_property_node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "string_property" ), NULL );
    string tmp = std_vector_to_std_string< string >( it->second );
    cout << "writing \"" << tmp << "\"" << endl;
    xmlAddChild( node, string_property_node ); 
  }
  xmlAddChild( root, node );
  return;
}


namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const Symbol_Dictionary& other ) {
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
