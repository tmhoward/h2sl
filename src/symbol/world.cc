/**
 * @file    world.cc
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
 * The implementation of a class used to represent the world
 */

#include <sstream>
#include <cmath>
#include <algorithm>

#include "h2sl/world.h"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace h2sl;

/**
 * World class constructor
 */

World::
World( const unsigned long long& time,
        const std::vector< Object* >& objects ) : _time( time ),
                                              _objects( objects ),
                                              _numeric_map(),
                                              _index_map() {
  initialise_sorted_object_collections();
  initialise_numeric_map(); 
  initialise_index_map(); 
}

/**
 * Helper function to initilise the sorted object collections. 
 */
void
World::
initialise_sorted_object_collections( void ){
  _min_x_sorted_objects = map< string, vector< Object* > >();
  _max_x_sorted_objects = map< string, vector< Object* > >();
  _min_y_sorted_objects = map< string, vector< Object* > >();
  _max_y_sorted_objects = map< string, vector< Object* > >();
  _min_abs_y_sorted_objects = map< string, vector< Object* > >();
  _max_abs_y_sorted_objects = map< string, vector< Object* > >();
  _min_z_sorted_objects = map< string, vector< Object* > >();
  _max_z_sorted_objects = map< string, vector< Object* > >();
  _min_distance_sorted_objects = map< string, vector< Object* > >();
  _max_distance_sorted_objects = map< string, vector< Object* > >();
  _min_center_distance_sorted_objects = map< string, vector< Object* > >();
  _max_center_distance_sorted_objects = map< string, vector< Object* > >();

}

/**
 * World class destructor 
 */

World::
~World() {

}

/**
 * World class copy constructor
 */

World::
World( const World& other ) : _time( other._time ),
                              _objects( other._objects ),
                              _min_x_sorted_objects( other._min_x_sorted_objects ),
                              _max_x_sorted_objects( other._max_x_sorted_objects ),
                              _min_y_sorted_objects( other._min_y_sorted_objects ),
                              _max_y_sorted_objects( other._max_y_sorted_objects ),
                              _min_abs_y_sorted_objects( other._min_abs_y_sorted_objects ),
                              _max_abs_y_sorted_objects( other._max_abs_y_sorted_objects ),
                              _min_z_sorted_objects( other._min_z_sorted_objects ),
                              _max_z_sorted_objects( other._max_z_sorted_objects ),
                              _min_distance_sorted_objects( other._min_distance_sorted_objects ),
                              _max_distance_sorted_objects( other._max_distance_sorted_objects ),
                              _min_center_distance_sorted_objects( other._min_center_distance_sorted_objects ),
                              _max_center_distance_sorted_objects( other._max_center_distance_sorted_objects ),
                              _numeric_map( other._numeric_map ),
                              _index_map( other._index_map ) {

}

/** 
 * World class assignment operator
 */

World&
World::
operator=( const World& other ) {
  _time = other._time;
  _objects = other._objects;
  _min_x_sorted_objects = other._min_x_sorted_objects;
  _max_x_sorted_objects = other._max_x_sorted_objects;
  _min_y_sorted_objects = other._min_y_sorted_objects;
  _max_y_sorted_objects = other._max_y_sorted_objects;
  _min_abs_y_sorted_objects = other._min_abs_y_sorted_objects;
  _max_abs_y_sorted_objects = other._max_abs_y_sorted_objects;
  _min_z_sorted_objects = other._min_z_sorted_objects;
  _max_z_sorted_objects = other._max_z_sorted_objects;
  _min_distance_sorted_objects = other._min_distance_sorted_objects;
  _max_distance_sorted_objects = other._max_distance_sorted_objects;
  _min_center_distance_sorted_objects = other._min_center_distance_sorted_objects;
  _max_center_distance_sorted_objects = other._max_center_distance_sorted_objects;
  _numeric_map = other._numeric_map;
  _index_map = other._index_map;
  return (*this);
}

/** 
 * World class copy constructor. 
 */

World*
World::
dup( void )const{
  return new World( *this );
}

void
World::
to_xml( const string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

void
World::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "world" ), NULL );
  stringstream time_string;
  time_string << _time;
  xmlNewProp( node, ( const xmlChar* )( "time" ), ( const xmlChar* )( time_string.str().c_str() ) );
  for( unsigned int i = 0; i < _objects.size(); i++ ){
    if( _objects[ i ] != NULL ){
      _objects[ i ]->to_xml( doc, node );
    }
  }
  xmlAddChild( root, node );
  return;
}

void
World::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "world" ) ) == 0 ){
            from_xml( l1 );
          } else if ( xmlStrcmp( l1->name, ( const xmlChar* )( "models" ) ) == 0 ){
            convert_models( l1 );
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
 * imports the World class from an XML node pointer
 */
void
World::
from_xml( xmlNodePtr root ){
  _time = 0;
  for( unsigned int i = 0; i < _objects.size(); i++ ){
    delete _objects[ i ];
    _objects[ i ] = NULL;
  }
  _objects.clear();
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "time" ) );
    if( tmp != NULL ){
      string time_string = ( char* )( tmp );
      _time = strtol( time_string.c_str(), NULL, 10 );
      xmlFree( tmp );
    }
    for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
          _objects.push_back( new Object( l1 ) );
        }
      }
    }
  }
  // Call the sort object collections function.
  sort_object_collections();
  return;

}


/** 
 * Function for converting the object names from the simulator
 * to the object names in the data set. 
 * Commenting this out right now as the object types are not hard-coded now.
 * This is specific to the ealier adcg data set.
 */
void
World::
convert_models( xmlNodePtr root ){
  if( root->type == XML_ELEMENT_NODE ){
    for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "model" ) ) == 0 ){
          for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
            if( l2->type == XML_ELEMENT_NODE ){
              if( xmlStrcmp( l2->name, ( const xmlChar* )( "body" ) ) == 0 ){
                _objects.push_back( new Object() );
                Object * object = _objects.back();
                // modify object to match information contained in body xmlNode

                // get body->id and set it to object->name()
                xmlChar * tmp = xmlGetProp( l2, ( const xmlChar* )( "id" ) );
                if( tmp != NULL ){
                  string id_string = ( char* )( tmp );
                  object->name() = id_string.c_str();
                  xmlFree( tmp );
                  id_string.clear();
                }

                //assign object->type() from object->name()
                if( !object->name().empty() ){
                  if( object->name().find( "baxter-left_hand" ) != std::string::npos ){
                    object->type() = std::string( "robot" );
                  } else if( object->name().find( "baxter-right_hand" ) != std::string::npos ){
                    object->type() = std::string( "robot" );
                  } else if( object->name().find( "cube-cube-base" ) != std::string::npos ){
                    object->type() = std::string( "block" );
                  } else if( object->name().find( "table-table-table" ) != std::string::npos ){
                    object->type() = std::string( "table" );
                  } else{
                    object->type() = std::string( "na" );
                  }
                }

                //get object position 
                tmp = xmlGetProp( l2, ( const xmlChar* )( "xyz" ) );
                if( tmp != NULL ){
                  string xyz_string = ( char* )( tmp );
                  string tmp_string = xyz_string.c_str();
                  for( unsigned int i = 0; i < tmp_string.length(); ++i ){
                    if( tmp_string[ i ] == char( 32 ) ){
                      tmp_string[ i ] = char( 44 );
                    }
                  }

                  object->transform().position().from_std_string( tmp_string );
                  xmlFree( tmp );
                  xyz_string.clear();
                  tmp_string.clear();
                }

                //get object orietation 
                tmp = xmlGetProp( l2, ( const xmlChar* )( "rpy" ) );
                if( tmp != NULL ){
                  string rpy_string = ( char* )( tmp );
                  string tmp_string = rpy_string.c_str();
                  vector< string > data_strings;
                  boost::split( data_strings, tmp_string, boost::is_any_of( " " ) );
                  object->transform().orientation().from_rpy( strtof( data_strings[ 0 ].c_str(), NULL ), 
                                                              strtof( data_strings[ 1 ].c_str(), NULL ), 
                                                              strtof( data_strings[ 2 ].c_str(), NULL ) ) ;
                  xmlFree( tmp );
                  rpy_string.clear();
                  tmp_string.clear();
                }

                //get object color info
                //ToDo.
              }
            }
          }
        }
      }
    }
  }
  // Sort the object collections.
  sort_object_collections();
  return;
}


/**
 * Sorting of objects according to types and spatial characteristics.
 * Object types are obtained by iterating over the object list.
 */

void
World::
sort_object_collections( void ){ 

  // Clear the sorted object containers if they exist.
  for( unsigned int i = 0; i < _objects.size(); i++ ){
    
    if ( _min_x_sorted_objects.find( _objects[ i ]->type() ) != _min_x_sorted_objects.end() ) {
      _min_x_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _min_x_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }

     if ( _max_x_sorted_objects.find( _objects[ i ]->type() ) != _max_x_sorted_objects.end() ) {
      _max_x_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _max_x_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }

    if ( _min_y_sorted_objects.find( _objects[ i ]->type() ) != _min_y_sorted_objects.end() ) {
      _min_y_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _min_y_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }

    if ( _max_y_sorted_objects.find( _objects[ i ]->type() ) != _max_y_sorted_objects.end() ) {
      _max_y_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _max_y_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }

    if ( _min_abs_y_sorted_objects.find( _objects[ i ]->type() ) != _min_abs_y_sorted_objects.end() ) {
      _min_abs_y_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _min_abs_y_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }

    if ( _max_abs_y_sorted_objects.find( _objects[ i ]->type() ) != _max_abs_y_sorted_objects.end() ) {
      _max_abs_y_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _max_abs_y_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }

    if ( _min_z_sorted_objects.find( _objects[ i ]->type() ) != _min_z_sorted_objects.end() ) {
      _min_z_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _min_z_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }

    if ( _max_z_sorted_objects.find( _objects[ i ]->type() ) != _max_z_sorted_objects.end() ) {
      _max_z_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _max_z_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }
 
    if ( _min_distance_sorted_objects.find( _objects[ i ]->type() ) != _min_distance_sorted_objects.end() ) {
      _min_distance_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _min_distance_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }

    if ( _max_distance_sorted_objects.find( _objects[ i ]->type() ) != _max_distance_sorted_objects.end() ) {
      _max_distance_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _max_distance_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }

    if ( _min_center_distance_sorted_objects.find( _objects[ i ]->type() ) != _min_center_distance_sorted_objects.end() ) {
      _min_center_distance_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _min_center_distance_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }

    if ( _max_center_distance_sorted_objects.find( _objects[ i ]->type() ) != _max_center_distance_sorted_objects.end() ) {
      _max_center_distance_sorted_objects[ _objects[ i ]->type() ].clear(); 
    } else {
      _max_center_distance_sorted_objects.insert( pair< string, vector < Object* > >( _objects[ i ]->type(), vector< Object* >() ) );
    }
  } 

  // Fill in the object pointers according to the object type. 
  for( unsigned int i = 0; i < _objects.size(); i++ ){
    _min_x_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
    _max_x_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
    _min_y_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
    _max_y_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
    _min_abs_y_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
    _max_abs_y_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
    _min_z_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
    _max_z_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
    _min_distance_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
    _max_distance_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
    _min_center_distance_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
    _max_center_distance_sorted_objects[ _objects[ i ]->type() ].push_back( _objects[ i ] );
  }

 for( map< string, vector< Object* > >::iterator it = _min_x_sorted_objects.begin(); it != _min_x_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::min_x_sort);
   }
 }

 for( map< string, vector< Object* > >::iterator it = _max_x_sorted_objects.begin(); it != _max_x_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::max_x_sort);
   }
 }

 for( map< string, vector< Object* > >::iterator it = _min_y_sorted_objects.begin(); it != _min_y_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::min_y_sort);
   }
 }

 for( map< string, vector< Object* > >::iterator it = _max_y_sorted_objects.begin(); it != _max_y_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::max_y_sort);
   }
 }

 for( map< string, vector< Object* > >::iterator it = _min_abs_y_sorted_objects.begin(); it != _min_abs_y_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::min_abs_y_sort);
   }
 }

 for( map< string, vector< Object* > >::iterator it = _max_abs_y_sorted_objects.begin(); it != _max_abs_y_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::max_abs_y_sort);
   }
 }

 for( map< string, vector< Object* > >::iterator it = _min_z_sorted_objects.begin(); it != _min_z_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::min_z_sort);
   }
 }

 for( map< string, vector< Object* > >::iterator it = _max_z_sorted_objects.begin(); it != _max_z_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::max_z_sort);
   }
 }

 for( map< string, vector< Object* > >::iterator it = _min_distance_sorted_objects.begin(); it != _min_distance_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::min_distance_sort);
   }
 }

 for( map< string, vector< Object* > >::iterator it = _max_distance_sorted_objects.begin(); it != _max_distance_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::max_distance_sort);
   }
 }

 for( map< string, vector< Object* > >::iterator it = _min_center_distance_sorted_objects.begin(); it != _min_center_distance_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::min_distance_sort );
   }
 }

 for( map< string, vector< Object* > >::iterator it = _max_center_distance_sorted_objects.begin(); it != _max_center_distance_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     sort( it->second.begin(), it->second.end(), World::max_distance_sort );
   }
 }

 // Center adjusted sorting.
 for( map< string, vector< Object* > >::iterator it = _min_center_distance_sorted_objects.begin(); it != _min_center_distance_sorted_objects.end(); ++it ){
   if ( it->second.size() > 1){
     min_center_distance_sort_objects( it->second );
   }
 }

for( map< string, vector< Object* > >::iterator it = _max_center_distance_sorted_objects.begin(); it != _max_center_distance_sorted_objects.end(); ++it ){
  if ( it->second.size() > 1){
    max_center_distance_sort_objects( it->second );
  }
}

  return;
}


/**
 * Compares two objects. Used in sorting.
 * Returns minimum  according to the x-coordinate. 
 */
bool
World::
min_x_sort( const Object* a,
              const Object* b ){
  return a->transform().position().x() < b->transform().position().x();
}

bool
World::
max_x_sort( const Object* a,
              const Object* b ){
  return a->transform().position().x() > b->transform().position().x();
}

void
World::
min_x_sort_objects( vector< Object* >& objects ){
  std::sort( objects.begin(), objects.end(), World::min_x_sort );
  return;
}

void
World::
max_x_sort_objects( vector< Object* >& objects ){
  std::sort( objects.begin(), objects.end(), World::max_x_sort );
  return;
}

bool
World::
min_y_sort( const Object* a,
              const Object* b ){
  return a->transform().position().y() < b->transform().position().y();
}

bool
World::
max_y_sort( const Object* a,
              const Object* b ){
  return a->transform().position().y() > b->transform().position().y();
}

void
World::
min_y_sort_objects( vector< Object* >& objects ){
  std::sort( objects.begin(), objects.end(), World::min_y_sort );
  return;
}

void
World::
max_y_sort_objects( vector< Object* >& objects ){
  std::sort( objects.begin(), objects.end(), World::max_y_sort );
  return;
}

bool
World::
min_abs_y_sort( const Object* a,
              const Object* b ){
  return fabs( a->transform().position().y() ) < fabs( b->transform().position().y() );
}

bool
World::
max_abs_y_sort( const Object* a,
              const Object* b ){
  return fabs( a->transform().position().y() ) > fabs( b->transform().position().y() );
}

bool
World::
min_z_sort( const Object* a,
              const Object* b ){
  return a->transform().position().z() < b->transform().position().z();
}

bool
World::
max_z_sort( const Object* a,
              const Object* b ){
  return a->transform().position().z() > b->transform().position().z();
}

bool
World::
min_distance_sort( const Object* a,
                    const Object* b ){
  return a->transform().position().norm() < b->transform().position().norm();
}

bool
World::
max_distance_sort( const Object* a,
                    const Object* b ){
  return a->transform().position().norm() > b->transform().position().norm();
}

void
World::
min_distance_sort_objects( vector< Object* >& objects ){
  std::sort( objects.begin(), objects.end(), World::min_distance_sort );
  return;
}

void
World::
max_distance_sort_objects( vector< Object* >& objects ){
  std::sort( objects.begin(), objects.end(), World::max_distance_sort );
  return;
}


void
World::
min_center_distance_sort_objects( vector< Object* >& objects ){
  // find the center of the objects
  h2sl::Transform center;
  for( unsigned int i = 0; i < objects.size(); i++ ){
    center.position() += objects[ i ]->transform().position();
  }
  center.position() /= ( double )( objects.size() );

  // set transform relative to the center
  for( unsigned int i = 0; i < objects.size(); i++ ){
    objects[ i ]->transform() = center.inverse() * objects[ i ]->transform();
  }

  // sort the objects
  std::sort( objects.begin(), objects.end(), World::min_distance_sort );

  // restore the original transforms
  for( unsigned int i = 0; i < objects.size(); i++ ){
    objects[ i ]->transform() = center * objects[ i ]->transform();
  }
  return;
}

void
World::
max_center_distance_sort_objects( vector< Object* >& objects ){
  // find the center of the objects
  h2sl::Transform center;
  for( unsigned int i = 0; i < objects.size(); i++ ){
    center.position() += objects[ i ]->transform().position();
  }
  center.position() /= ( double )( objects.size() );

  // set transform relative to the center
  for( unsigned int i = 0; i < objects.size(); i++ ){
    objects[ i ]->transform() = center.inverse() * objects[ i ]->transform();
  }

  // sort the objects
  std::sort( objects.begin(), objects.end(), World::max_distance_sort );

  // restore the original transforms
  for( unsigned int i = 0; i < objects.size(); i++ ){
    objects[ i ]->transform() = center * objects[ i ]->transform();
  }
  return;
}

/*
 * Initialise the numeric map.
 */
void 
World::
initialise_numeric_map( void ) {
// Values are used as indices. Hence subtracting one.
  _numeric_map.clear();
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "one",0 )  );
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "two",1 )  );
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "three",2 )  );
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "four",3 )  );
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "five",4 )  );
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "six",5 )  );
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "seven",6 )  );
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "eight",7 )  );
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "nine",8 )  );
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "ten",9 )  );
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "eleven",10 )  );
  _numeric_map.insert( std::pair< std::string, unsigned int > ( "twelve",11 )  );
}

/*
 * Initialise the index map.
 */
void
World::
initialise_index_map( void ) {
  _index_map.clear();
// Values are used as indices. Hence subtracting one.
    _index_map.insert( std::pair< std::string, unsigned int > ( "first",0 )  );
    _index_map.insert( std::pair< std::string, unsigned int > ( "second",1 )  );
    _index_map.insert( std::pair< std::string, unsigned int > ( "third",2 )  );
    _index_map.insert( std::pair< std::string, unsigned int > ( "fourth",3 )  );
    _index_map.insert( std::pair< std::string, unsigned int > ( "fifth",4 )  );
}


namespace h2sl {
  /** 
   * World class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const World& other ) {
    out << "time:\"" << other.time() << "\" ";
    out << "objects[" << other.objects().size() << "]:{";
    for( unsigned int i = 0; i < other.objects().size(); i++ ){
      out << "(" << *other.objects()[ i ] << ")";
      if( i != ( other.objects().size() - 1 ) ){
        out << ",";
      }
    }
    out << "}";
    return out;
  }
}
