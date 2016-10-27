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

World::
World( const unsigned long long& time,
        const std::vector< Object* >& objects ) : _time( time ),
                                              _objects( objects ) {

}

World::
~World() {

}

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
                              _max_center_distance_sorted_objects( other._max_center_distance_sorted_objects ) {

}

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
  return (*this);
}

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
          }
        }
      }
    }
    xmlFreeDoc( doc );
  }
  return;
}

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
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
          _objects.push_back( new Object( l1 ) );
        }
      }
    }
  }
  return;
}

void
World::
sort( void ){
  /*
  // IN PROGRESS

  for( unsigned int i = 0; i < _min_x_sorted_objects.size(); i++ ){
    _min_x_sorted_objects[ i ].clear();
  }
  for( unsigned int i = 0; i < _max_x_sorted_objects.size(); i++ ){
    _max_x_sorted_objects[ i ].clear();
  }
  for( unsigned int i = 0; i < _min_y_sorted_objects.size(); i++ ){
    _min_y_sorted_objects[ i ].clear();
  }
  for( unsigned int i = 0; i < _max_y_sorted_objects.size(); i++ ){
    _max_y_sorted_objects[ i ].clear();
  }
  for( unsigned int i = 0; i < _min_abs_y_sorted_objects.size(); i++ ){
    _min_abs_y_sorted_objects[ i ].clear();
  }
  for( unsigned int i = 0; i < _max_abs_y_sorted_objects.size(); i++ ){
    _max_abs_y_sorted_objects[ i ].clear();
  }
  for( unsigned int i = 0; i < _min_z_sorted_objects.size(); i++ ){
    _min_z_sorted_objects[ i ].clear();
  }
  for( unsigned int i = 0; i < _max_z_sorted_objects.size(); i++ ){
    _max_z_sorted_objects[ i ].clear();
  }
  for( unsigned int i = 0; i < _min_distance_sorted_objects.size(); i++ ){
    _min_distance_sorted_objects[ i ].clear();
  }
  for( unsigned int i = 0; i < _max_distance_sorted_objects.size(); i++ ){
    _max_distance_sorted_objects[ i ].clear();
  }
  for( unsigned int i = 0; i < _min_center_distance_sorted_objects.size(); i++ ){
    _min_center_distance_sorted_objects[ i ].clear();
  }
  for( unsigned int i = 0; i < _max_center_distance_sorted_objects.size(); i++ ){
    _max_center_distance_sorted_objects[ i ].clear();
  }

 for( unsigned int i = 0; i < _objects.size(); i++ ){
    _min_x_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
    _max_x_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
    _min_y_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
    _max_y_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
    _min_abs_y_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
    _max_abs_y_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
    _min_z_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
    _max_z_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
    _min_distance_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
    _max_distance_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
    _min_center_distance_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
    _max_center_distance_sorted_objects[ _objects[ i ]->object_type() ].push_back( _objects[ i ] );
  }

  for( unsigned int i = 0; i < _min_x_sorted_objects.size(); i++ ){
    if( _min_x_sorted_objects[ i ].size() > 1 ){
      std::sort( _min_x_sorted_objects[ i ].begin(), _min_x_sorted_objects[ i ].end(), World::min_x_sort );
    }
  }

  for( unsigned int i = 0; i < _max_x_sorted_objects.size(); i++ ){
    if( _max_x_sorted_objects[ i ].size() > 1 ){
      std::sort( _max_x_sorted_objects[ i ].begin(), _max_x_sorted_objects[ i ].end(), World::max_x_sort );
    }
  }

  for( unsigned int i = 0; i < _min_y_sorted_objects.size(); i++ ){
    if( _min_y_sorted_objects[ i ].size() > 1 ){
      std::sort( _min_y_sorted_objects[ i ].begin(), _min_y_sorted_objects[ i ].end(), World::min_y_sort );
    }
  }

  for( unsigned int i = 0; i < _max_y_sorted_objects.size(); i++ ){
    if( _max_y_sorted_objects[ i ].size() > 1 ){
      std::sort( _max_y_sorted_objects[ i ].begin(), _max_y_sorted_objects[ i ].end(), World::max_y_sort );
    }
  }

  for( unsigned int i = 0; i < _min_abs_y_sorted_objects.size(); i++ ){
    if( _min_abs_y_sorted_objects[ i ].size() > 1 ){
      std::sort( _min_abs_y_sorted_objects[ i ].begin(), _min_abs_y_sorted_objects[ i ].end(), World::min_abs_y_sort );
    }
  }

  for( unsigned int i = 0; i < _max_abs_y_sorted_objects.size(); i++ ){
    if( _max_abs_y_sorted_objects[ i ].size() > 1 ){
      std::sort( _max_abs_y_sorted_objects[ i ].begin(), _max_abs_y_sorted_objects[ i ].end(), World::max_abs_y_sort );
    }
  }

  for( unsigned int i = 0; i < _min_z_sorted_objects.size(); i++ ){
    if( _min_z_sorted_objects[ i ].size() > 1 ){
      std::sort( _min_z_sorted_objects[ i ].begin(), _min_z_sorted_objects[ i ].end(), World::min_z_sort );
    }
  }

  for( unsigned int i = 0; i < _max_z_sorted_objects.size(); i++ ){
    if( _max_z_sorted_objects[ i ].size() > 1 ){
      std::sort( _max_z_sorted_objects[ i ].begin(), _max_z_sorted_objects[ i ].end(), World::max_z_sort );
    }
  }

  for( unsigned int i = 0; i < _min_distance_sorted_objects.size(); i++ ){
    if( _min_distance_sorted_objects[ i ].size() > 1 ){
      std::sort( _min_distance_sorted_objects[ i ].begin(), _min_distance_sorted_objects[ i ].end(), World::min_distance_sort );
    }
  }

  for( unsigned int i = 0; i < _max_distance_sorted_objects.size(); i++ ){
    if( _max_distance_sorted_objects[ i ].size() > 1 ){
      std::sort( _max_distance_sorted_objects[ i ].begin(), _max_distance_sorted_objects[ i ].end(), World::max_distance_sort );
    }
  }

  for( unsigned int i = 0; i < _min_center_distance_sorted_objects.size(); i++ ){
    if( _min_center_distance_sorted_objects[ i ].size() > 1 ){
      std::sort( _min_center_distance_sorted_objects[ i ].begin(), _min_center_distance_sorted_objects[ i ].end(), World::min_distance_sort );
    }
  }

  for( unsigned int i = 0; i < _min_center_distance_sorted_objects.size(); i++ ){
    if( _min_center_distance_sorted_objects[ i ].size() > 1 ){
      min_center_distance_sort_objects( _min_center_distance_sorted_objects[ i ] );
    }
  }

  for( unsigned int i = 0; i < _max_center_distance_sorted_objects.size(); i++ ){
    if( _max_center_distance_sorted_objects[ i ].size() > 1 ){
      max_center_distance_sort_objects( _max_center_distance_sorted_objects[ i ] );
    }
  }
*/

  /*
  // Determining the containers is not required right now.
  vector< vector< Object* > > object_vectors( Object_Type::TYPE_NUM_TYPES );

  // group like objects into their respective containers
  for( unsigned int i = 0; i < _objects.size(); i++ ){
    object_vectors[ _nsf_nri_mvli_objects[ i ]->type() ].push_back( static_cast< Object* >( _nsf_nri_mvli_objects[ i ] ) );
  }

  for( unsigned int i = 0; i < object_vectors.size(); i++ ){
    if( object_vectors[ i ].size() > 2 ){
      for( unsigned int j = 0; j < object_vectors[ i ].size(); j++ ){
        Container container;
        container.container().push_back( object_vectors[ i ][ j ] );

        bool object_added = true;
        while( object_added ){
          object_added = false;
          for( unsigned int k = 0; k < object_vectors[ i ].size(); k++ ){
            if( k != j ){
              double min_distance = container.min_distance_to_object( *object_vectors[ i ][ k ] );
              if( min_distance < 0.2 ){
                if( !container.has_object( object_vectors[ i ][ k ] ) ){
                  container.container().push_back( object_vectors[ i ][ k ] );
                  object_added = true;
                }
              }
            }
          }
        }

        if( container.container().size() > 1 ){
        //  h2sl::Vector3 variance_position = container.variance_position();
        //  cout << "    variance_position:" << variance_position << endl;
        //  if( ( variance_position.x() < 0.002 ) && ( variance_position.y() > 0.002 ) ){
        //     container.type() = Container::Type::TYPE_ROW;
        //  } else if( ( variance_position.y() < 0.002 ) && ( variance_position.x() > 0.002 ) ){
        //    container.type() = Container::Type::TYPE_COLUMN;
        //  } else {
        //    container.type() = Container::Type::TYPE_GROUP;
        //  }
          // determine container shape
          bool new_container = true;
          for( unsigned int k = 0; k < _containers.size(); k++ ){
            if( _containers[ k ] == container ){
              new_container = false;
            }
          }

          if( new_container ){
//            cout << "  adding container:(" << container << ")" << endl;
            pair< Object*, Object* > bounds = container.bounds();
//            cout << "bounds:(" << *bounds.first << "," << *bounds.second << ")" << endl;
//            cout << "principal_axis:" << container.principal_axis() << endl;
            _containers.push_back( container );
          }

        }

      }
    }
  }
  */

  return;
}




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


namespace h2sl {
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
