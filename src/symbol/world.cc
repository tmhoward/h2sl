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
        const map< string, Object* >& objects ) : _time( time ),
                                              _objects( objects ),
                                              _sorted_objects() {
  initialise_sorted_object_collections();
}

World::
World( const string& filename ) : _time( 0 ),
                                  _objects(),
                                  _sorted_objects() {

  from_xml( filename );
  initialise_sorted_object_collections();
}

/**
 * Helper function to initilise the sorted object collections. 
 */
void
World::
initialise_sorted_object_collections( void ){
/*
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
*/
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
                              _sorted_objects( other._sorted_objects ) {
/*
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
*/
}

/** 
 * World class assignment operator
 */

World&
World::
operator=( const World& other ) {
  _time = other._time;
  _objects = other._objects;
  _sorted_objects = other._sorted_objects;
/*
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
*/
  return (*this);
}

bool
World::
operator==( const World& other )const{
  const double th = 0.00001;
  unsigned int loop_1_counter = 0;
  for( map< string, Object* >::const_iterator it_this_object = _objects.begin(); it_this_object != _objects.end(); ++it_this_object ){
    bool no_match = true;
    unsigned int loop_2_counter = 0;
    for( map< string, Object* >::const_iterator it_other_object = other.objects().begin(); it_other_object != other.objects().end(); ++it_other_object ){
      //compare the object id, type, and color properties
      if( *(it_this_object->second) == *(it_other_object->second) ){
        //compare the position and orienation values
        if( ( th > fabs( it_this_object->second->transform().position().x() - it_other_object->second->transform().position().x() ) ) &&
            ( th > fabs( it_this_object->second->transform().position().y() - it_other_object->second->transform().position().y() ) ) &&
            ( th > fabs( it_this_object->second->transform().position().z() - it_other_object->second->transform().position().z() ) ) &&
            ( th > fabs( it_this_object->second->transform().orientation().qv().x() - it_other_object->second->transform().orientation().qv().x() ) ) &&
            ( th > fabs( it_this_object->second->transform().orientation().qv().y() - it_other_object->second->transform().orientation().qv().y() ) ) &&
            ( th > fabs( it_this_object->second->transform().orientation().qv().z() - it_other_object->second->transform().orientation().qv().z() ) ) &&
            ( th > fabs( it_this_object->second->transform().orientation().qs() - it_other_object->second->transform().orientation().qs() ) ) ){
          no_match = false;
        }
      }
    }
    if( no_match ){ return false; }
  }
  return true;
}

bool
World::
operator!=( const World& other )const{
  return !( *this == other );
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
  for( map< string, Object* >::const_iterator it_world_object = _objects.begin(); it_world_object != _objects.end(); it_world_object++ ){
    if( it_world_object->second != NULL ){
      it_world_object->second->to_xml( doc, node, true );
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
  for( map< string, Object* >::iterator it_world_object = _objects.begin(); it_world_object != _objects.end(); it_world_object++ ){
    delete it_world_object->second;
    it_world_object->second = NULL;
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
          Object * object = new Object( l1, NULL );
          _objects.insert( pair< string, Object* >( object->id(), object ) );
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
                Object * object = new Object();
                // get body->id and set it to object->id()
                xmlChar * tmp = xmlGetProp( l2, ( const xmlChar* )( "id" ) );
                if( tmp != NULL ){
                  string id_string = ( char* )( tmp );
                  object->id() = id_string.c_str();
                  xmlFree( tmp );
                  id_string.clear();
                }

                //assign object->type() from object->id()
                if( !object->id().empty() ){
                  if( object->id().find( "baxter-left_hand" ) != std::string::npos ){
                    object->type() = std::string( "robot" );
                  } else if( object->id().find( "baxter-right_hand" ) != std::string::npos ){
                    object->type() = std::string( "robot" );
                  } else if( object->id().find( "cube-cube-base" ) != std::string::npos ){
                    object->type() = std::string( "block" );
                  } else if( object->id().find( "table-table-table" ) != std::string::npos ){
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
                  assert( false );
                  xmlFree( tmp );
                  rpy_string.clear();
                  tmp_string.clear();
                }
            
                _objects.insert( pair< string, Object* >( object->id(), object ) );
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
 * Transform objects with respect to object with id "reference_object_id"
 */
bool
World::
transform_objects( const string& reference_object_id ) {
  std::map< std::string, Object* >::iterator it = _objects.find( reference_object_id );
  if( it != _objects.end() ){
    Transform transform_to_apply = it->second->transform().inverse();
    for( map< std::string, Object* >::iterator it_objects = _objects.begin(); it_objects != _objects.end(); ++it_objects ){
      if ( it_objects->second != NULL ) {
		Transform& object_transform = it_objects->second->transform();
		object_transform = transform_to_apply * object_transform;
      }
    }
  } else {
	cout << "Could not find object " << reference_object_id << endl;
	return 0;
  }
  return 1;
}

/**
 * Sorting of objects according to types and spatial characteristics.
 * Object types are obtained by iterating over the object list.
 */

void
World::
sort_object_collections( void ){ 

  _sorted_objects.clear();

  vector< string > sorted_object_keys = { "min_x", "max_x", "min_y", "max_y", "min_z", "max_z", "min_abs_x", "max_abs_x", "min_abs_y", "max_abs_y", "min_distance", "max_distance", "min_center_distance", "max_center_distance" };

  for( vector< string >::const_iterator it_sorted_object_keys = sorted_object_keys.begin(); it_sorted_object_keys != sorted_object_keys.end(); it_sorted_object_keys++ ){
    pair< map< string, map< string, vector< Object* > > >::iterator, bool > key_insert = _sorted_objects.insert( pair< string, map< string, vector< Object* > > >( *it_sorted_object_keys, map< string, vector< Object* > >() ) );

    for( map< string, Object* >::const_iterator it_world_object = _objects.begin(); it_world_object != _objects.end(); it_world_object++ ){
      map< string, vector< Object* > >::iterator it_object_vectors = key_insert.first->second.find( it_world_object->second->type() );
      if( it_object_vectors == key_insert.first->second.end() ){
        key_insert.first->second.insert( pair< string, vector< Object* > >( it_world_object->second->type(), vector< Object* >() ) );
        it_object_vectors = key_insert.first->second.find( it_world_object->second->type() );
      }
      it_object_vectors->second.push_back( it_world_object->second );
    }

    for( map< string, vector< Object* > >::iterator it_object_vectors = key_insert.first->second.begin(); it_object_vectors != key_insert.first->second.end(); it_object_vectors++ ){
      if( key_insert.first->first == "min_x" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::min_x_sort );
      } else if ( key_insert.first->first == "max_x" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::max_x_sort );
      } else if ( key_insert.first->first == "min_y" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::min_y_sort );
      } else if ( key_insert.first->first == "max_y" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::max_y_sort );
      } else if ( key_insert.first->first == "min_z" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::min_z_sort );
      } else if ( key_insert.first->first == "max_z" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::max_z_sort );
      } else if ( key_insert.first->first == "min_abs_x" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::min_abs_x_sort );
      } else if ( key_insert.first->first == "max_abs_x" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::max_abs_x_sort );
      } else if ( key_insert.first->first == "min_abs_y" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::min_abs_y_sort );
      } else if ( key_insert.first->first == "max_abs_y" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::max_abs_y_sort );
      } else if ( key_insert.first->first == "min_distance" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::min_distance_sort );
      } else if ( key_insert.first->first == "max_distance" ){
        sort( it_object_vectors->second.begin(), it_object_vectors->second.end(), World::max_distance_sort );
      } else if ( key_insert.first->first == "min_center_distance" ){
        min_center_distance_sort_objects( it_object_vectors->second );
      } else if ( key_insert.first->first == "max_center_distance" ){
        max_center_distance_sort_objects( it_object_vectors->second );
      }
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
min_abs_x_sort( const Object* a,
              const Object* b ){
  return fabs( a->transform().position().x() ) < fabs( b->transform().position().x() );
}

bool
World::
max_abs_x_sort( const Object* a,
              const Object* b ){
  return fabs( a->transform().position().x() ) > fabs( b->transform().position().x() );
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

namespace h2sl {
  /** 
   * World class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const World& other ) {
    out << "time:\"" << other.time() << "\" ";
    out << "objects[" << other.objects().size() << "]:{";
    for( map< string, Object* >::const_iterator it_world_object = other.objects().begin(); it_world_object != other.objects().end(); it_world_object++ ){
      if( it_world_object->second != NULL ){
        out << "(" << *it_world_object->second << ")";
      } else {
        out << "(NULL)";
      }
      if( next( it_world_object ) != other.objects().end() ){
        out << ",";
      }
    }
    out << "}";
    return out;
  }
}

