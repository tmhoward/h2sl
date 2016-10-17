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
Container( const vector< Object* >& container,
        const string& containerType) : Grounding(),
                                    _container( container ) {
  insert_prop< std::string >( _properties, "container_type", containerType );
}

/**
 * Container class copy constructor
 */
Container::
Container( const Container& other ) : Grounding(), 
  				      _container( other._container ) {
  
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
  _container = other._container;
  _properties = other._properties;
  return (*this);
}

/**
 * Container class equality operator
 */
bool
Container::
operator==( const Container& other )const{ 
  if( container_type() != other.container_type() ){
     return false;
   } else if( _container.size() != other.container().size() ){
     return false;
   } else {
     for( unsigned int i = 0; i < _container.size(); i++ ){
       bool found_match = false;
       for( unsigned int j = 0; j < other.container().size(); j++ ){
         if( *_container[ i ] == *other.container()[ j ] ){
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
  for( unsigned int i = 0; i < _container.size(); i++ ){
    if( _container[ i ] == object ){
      return true;
    }
  }
  return false;
}

double
Container::
min_distance_to_object( const Object& object )const{
  assert( !_container.empty() );
  double min_distance = 0.0;
  for( unsigned int i = 0; i < _container.size(); i++ ){
    double distance = h2sl::Vector3::distance( _container[ i ]->transform().position(), object.transform().position() );
    if( ( i == 0 ) || ( distance < min_distance ) ){
      min_distance = distance;
    }
  }
  return min_distance;
}

h2sl::Vector3
Container::
mean_position( void )const{
  assert( !_container.empty() );
  h2sl::Vector3 tmp;
  for( unsigned int i = 0; i < _container.size(); i++ ){
    tmp += _container[ i ]->transform().position();
  }
  tmp *= ( 1.0 / ( double )( _container.size() ) );
  return tmp;
}

h2sl::Vector3
Container::
variance_position( void )const{
  assert( !_container.empty() );
  h2sl::Vector3 tmp;
  h2sl::Vector3 mean = mean_position();
  for( unsigned int i = 0; i < _container.size(); i++ ){
    tmp.x() += pow( _container[ i ]->transform().position().x() - mean.x(), 2.0 );
    tmp.y() += pow( _container[ i ]->transform().position().y() - mean.y(), 2.0 );
    tmp.z() += pow( _container[ i ]->transform().position().z() - mean.z(), 2.0 );
  }
  tmp *= ( 1.0 / ( double )( _container.size() ) );
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
  for( unsigned int i = 0; i < _container.size(); i++ ){
    if( ( _container[ i ] != bounds.first ) && ( _container[ i ] != bounds.second ) ){
      double deviation = this->deviation( *_container[ i ] );
      if( deviation > max_deviation ){
        max_deviation = deviation;
      }
    }
  }
  return max_deviation;
}

pair< Object*, Object* >
Container::
bounds( void )const{
  assert( _container.size() > 1 );
  // return the farthest pair of objects
  double max_distance = 0.0;
  pair< Object*, Object* > tmp( NULL, NULL );
  for( unsigned int i = 0; i < _container.size(); i++ ){
    for( unsigned int j = i; j < _container.size(); j++ ){
      if( i != j ){
        double distance = h2sl::Vector3::distance( _container[ i ]->transform().position(), _container[ j ]->transform().position() );
        if( ( distance > max_distance ) || ( ( tmp.first == NULL ) && ( tmp.second == NULL ) ) ){
          max_distance = distance;
          tmp.first = _container[ i ];
          tmp.second = _container[ j ];
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
  /*stringstream type_string;
  type_string << type_to_std_string( (Type) _type );*/
  xmlNewProp( node, ( const xmlChar* )( "container_type" ), ( const xmlChar* )( get_prop< std::string >( _properties, "container_type").c_str() ) );
  for( unsigned int i = 0; i < _container.size(); i++ ){
    if( _container[ i ] != NULL ){
      _container[ i ]->to_xml( doc, node );
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
  container_type() = "group";
  for (unsigned int i = 0; i < _container.size(); i++){
    delete _container[ i ];
    _container[ i ] = NULL;
  }
  _container.clear();
  
  if( root->type == XML_ELEMENT_NODE ){
    pair< bool, string > container_type_prop = has_prop< std::string >( root, "container_type" );
    if( container_type_prop.first ){
      container_type() = container_type_prop.second;
    }
    
    xmlNodePtr l1 = NULL;
    for( l1 = root->children; l1; l1 = l1->next ){
      if( l1->type == XML_ELEMENT_NODE ){
        if( xmlStrcmp( l1->name, ( const xmlChar* )( "object" ) ) == 0 ){
          _container.push_back( new Object() );
          _container.back()->from_xml( l1 );
        }
      }
    }
  }
  return;
}


/*string
Container::
type_to_std_string( const Type type ){
  switch( type ){
  case( TYPE_ROW ):
    return "row";
    break;
  case( TYPE_COLUMN ):
    return "column";
    break;
  case( TYPE_TOWER ):
    return "tower";
    break;
  case( TYPE_GROUP ):
  case( TYPE_NUM_TYPES ):
  default:
    return "group";
  }
}

Container::Type
Container::
type_from_std_string( const std::string& arg){
  for ( unsigned int i = 0; i < Type::TYPE_NUM_TYPES; i++ ){
    if( type_to_std_string( ( Type )( i ) ) == arg ){
      return ( Type )( i );
    }
  }
  return Type::TYPE_GROUP;
}*/


namespace h2sl {
  /**
   * Container class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Container& other ){
   out << "class:\"Container\" ";
   out << "container_type:" << other.container_type() << " ";
   out << "objects[" << other.container().size() << "]:{";    
   for( unsigned int i = 0; i < other.container().size(); i++ ){
      if( other.container()[ i ] != NULL ){
        out << "(" << *other.container()[ i ] << ")";
//        out << other.container()[ i ]->name();
//        out << other.container()[ i ];
      }
      if( i != ( other.container().size() - 1 ) ){
        out << ",";
      }
    }
    out << "} ";
    return out;
  }
}