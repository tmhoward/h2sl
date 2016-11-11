/**
 * @file container.cc
 *
 * @brief
 *
 * a class that describes a container
 */

#include <sstream>
#include <cmath>
#include <assert.h>

#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Container class constructor
 */
Container::
Container( const vector< Grounding* >& groundings,
        const string& containerType) : Grounding(),
                                    _groundings( groundings ) {
  insert_prop< std::string >( _properties, "container_type", containerType );
}

 /**
 * Container class constructor using xml
 */
Container::
Container( xmlNodePtr root ) : Grounding(),
_groundings() {
    insert_prop< std::string >( _properties, "container_type", "group" );
    from_xml( root );
}

/**
 * Container class copy constructor
 */
Container::
Container( const Container& other ) : Grounding( other ),
  				      _groundings( other._groundings ) {
  
}

/**
 * Container class destructor
 */
Container::
~Container() {

}

/**
 * Container class assignment operator
 */
Container&
Container::
operator=( const Container& other ){
  _groundings = other._groundings;
  _properties = other._properties;
  return (*this);
}

/**
 * Container class equality operator
 */
bool
Container::
operator==( const Container& other )const{ 
  if( type() != other.type() ){
     return false;
   } else if( _groundings.size() != other.container().size() ){
     return false;
   } else {
     for( unsigned int i = 0; i < _groundings.size(); i++ ){
       bool found_match = false;
       for( unsigned int j = 0; j < other.container().size(); j++ ){
         if( *_groundings[ i ] == *other.container()[ j ] ){
           found_match = true;
         }
       }
       if( !found_match ){
         return false;
       }
     }
   }
   return true;
}

/**
 * Container class inequality operator
 */
bool
Container::
operator!=( const Container& other )const{
  return !( *this == other );
}


/**
 * Container class dup operator
 */
Container*
Container::
dup( void )const{
  return new Container( *this );
}

bool
Container::
has_object( const Object* object )const{
  if( object != NULL ){
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Object* >( _groundings[ i ] ) != NULL ){
        if( *static_cast< const Object* >( _groundings[ i ] ) == *object ){
          return true;
        }
      } else if ( dynamic_cast< const Container* >( _groundings[ i ] ) != NULL ){
        // search through child groundings container
        if( static_cast< const Container* >( _groundings[ i ] )->has_object( object ) ){
          return true;
        }
      }
    }
  }
  return false;
}

double
Container::
min_distance_to_object( const Object& object )const{
    double min_distance = 0.0;
    for( unsigned int i = 0; i < _groundings.size(); i++ ){
      if( dynamic_cast< const Object* >( _groundings[ i ] ) != NULL ){
        if( *static_cast< const Object* >( _groundings[ i ] ) == object ){
          double distance = h2sl::Vector3::distance( static_cast< const Object* >( _groundings[ i ] )->transform().position(), object.transform().position() );
          if( ( i == 0 ) || ( distance < min_distance ) ){
            min_distance = distance;
          }
        }
      }
    }
    return min_distance;
  }


h2sl::Vector3
Container::
mean_position( void )const{
  assert( !_groundings.empty() );
  h2sl::Vector3 tmp;
  for( unsigned int i = 0; i < _groundings.size(); i++ ){
    if( dynamic_cast< const Object* >( _groundings[ i ] ) != NULL ){
      tmp += static_cast< const Object* >( _groundings[ i ] )->transform().position();
    }
  }
  tmp *= ( 1.0 / ( double )( _groundings.size() ) );
  return tmp;
}

h2sl::Vector3
Container::
variance_position( void )const{
  assert( !_groundings.empty() );
  h2sl::Vector3 tmp;
  h2sl::Vector3 mean = mean_position();
  for( unsigned int i = 0; i < _groundings.size(); i++ ){
    if( dynamic_cast< const Object* >( _groundings[ i ] ) != NULL ){
      tmp.x() += pow( static_cast< const Object* >( _groundings[ i ] )->transform().position().x() - mean.x(), 2.0 );
      tmp.y() += pow( static_cast< const Object* >( _groundings[ i ] )->transform().position().y() - mean.y(), 2.0 );
      tmp.z() += pow( static_cast< const Object* >( _groundings[ i ] )->transform().position().z() - mean.z(), 2.0 );
    }
  }
  tmp *= ( 1.0 / ( double )( _groundings.size() ) );
  return tmp;
}

double
Container::
deviation( const Object& object )const{
  pair< Object*, Object* > bounds = this->bounds();
  h2sl::Vector3 p0 = object.transform().position();
  h2sl::Vector3 p1 = bounds.first->transform().position();
  h2sl::Vector3 p2 = bounds.second->transform().position();
  return fabs( ( p2.y() - p1.y() ) * p0.x() - ( p2.x() - p1.x() ) * p0.y() + p2.x() * p1.y() - p2.y() * p1.x() ) / sqrt( pow( p2.y() - p1.y(), 2.0 ) + pow( p2.x() - p1.x(), 2.0 ) );
}

double
Container::
max_deviation( void )const{
  double max_deviation = 0.0;
  pair< Object*, Object* > bounds = this->bounds();
  for( unsigned int i = 0; i < _groundings.size(); i++ ){
    if( dynamic_cast< const Object* >( _groundings[ i ] ) != NULL ){
      if( ( static_cast< const Object* >( _groundings[ i ] ) != bounds.first ) && ( static_cast< const Object* >( _groundings[ i ] ) != bounds.second ) ){
        double deviation = this->deviation( *static_cast< const Object* >( _groundings[ i ] ) );
        if( deviation > max_deviation ){
          max_deviation = deviation;
        }
      }
    }
  }
  return max_deviation;
}

pair< Object*, Object* >
Container::
bounds( void )const{
  assert( _groundings.size() > 1 );
  // return the farthest pair of objects
  double max_distance = 0.0;
  pair< Object*, Object* > tmp( NULL, NULL );
  for( unsigned int i = 0; i < _groundings.size(); i++ ){
    for( unsigned int j = i; j < _groundings.size(); j++ ){
      if( dynamic_cast< const Object* >( _groundings[ i ] ) != NULL && dynamic_cast< const Object* >( _groundings[ j ] ) != NULL ){
        if( i != j ){
          double distance = h2sl::Vector3::distance( static_cast< const Object* >( _groundings[ i ] )->transform().position(), static_cast< const Object* >( _groundings[ j ] )->transform().position() );
          if( ( distance > max_distance ) || ( ( tmp.first == NULL ) && ( tmp.second == NULL ) ) ){
            max_distance = distance;
              tmp.first = static_cast< Object* >( _groundings[ i ] );
            tmp.second = static_cast< Object* >( _groundings[ j ] );
          }
        }
      }
    }
  }
  return tmp;
}

h2sl::Vector3
Container::
principal_axis( void )const{
  pair< Object*, Object* > bounds = this->bounds();
  assert( bounds.first != NULL );
  assert( bounds.second != NULL );

  h2sl::Vector3 tmp = bounds.first->transform().position() - bounds.second->transform().position();
  tmp *= 1.0 / tmp.norm();

  return tmp;
}

/**
 * exports the Container class to an XML file
 */
void
Container::
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
 * exports the Container class to an XML node pointer
 */
void
Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( const xmlChar* )( "container" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "container_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "container_type").c_str() ) );
  for( unsigned int i = 0; i < _groundings.size(); i++ ){
    if( _groundings[ i ] != NULL ){
      _groundings[ i ]->to_xml( doc, node );
    }
  }
  xmlAddChild( root, node );
  return;
}

/**
 * imports the Container class from an XML file
 */
void
Container::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "container" ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
    }
    xmlFreeDoc( doc );
  }
  return;
}

/**
 * imports the Container class from an XML node pointer
 */
void
Container::
from_xml( xmlNodePtr root ){
  type() = "group";
  for (unsigned int i = 0; i < _groundings.size(); i++){
    delete _groundings[ i ];
    _groundings[ i ] = NULL;
  }
  _groundings.clear();
  
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > type_prop = has_prop< std::string >( root, "container_type" );
    if( type_prop.first ){
      type() = type_prop.second;
    }
    
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
          _groundings.push_back( new Object() );
          _groundings.back()->from_xml( l1 );
        }
      }
    }
  }
  return;
}

namespace h2sl {
  /**
   * Container class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Container& other ){
   out << "Container(";
   out << "container_type=\"" << other.type() << "\",";
   out << "objects=" << other.container().size();
   for( unsigned int i = 0; i < other.container().size(); i++ ){
      if( other.container()[ i ] != NULL ){
        out << "(" << *other.container()[ i ] << ")";
      }
      if( i != ( other.container().size() - 1 ) ){
        out << ",";
      }
    }
    out << ")";
    return out;
  }
}
