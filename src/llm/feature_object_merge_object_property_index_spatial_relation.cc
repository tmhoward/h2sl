/**
 * @file feature_object_merge_object_property_index_spatial_relation.cc
 *
 * @brief
 *
 * Class used to describe a feature spatial_relation matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_merge_object_property_index_spatial_relation.h"
//#include "h2sl/object_property.h"
#include "h2sl/object_property.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Merge_Object_Property_Index_Spatial_Relation class constructor
 */
Feature_Object_Merge_Object_Property_Index_Spatial_Relation::
Feature_Object_Merge_Object_Property_Index_Spatial_Relation( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Object_Merge_Object_Property_Index_Spatial_Relation class destructor
 */
Feature_Object_Merge_Object_Property_Index_Spatial_Relation::
~Feature_Object_Merge_Object_Property_Index_Spatial_Relation() {
    
}

/**
 * Feature_Object_Merge_Object_Property_Index_Spatial_Relation class copy constructor
 */
Feature_Object_Merge_Object_Property_Index_Spatial_Relation::
Feature_Object_Merge_Object_Property_Index_Spatial_Relation( const Feature_Object_Merge_Object_Property_Index_Spatial_Relation& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Merge_Object_Property_Index_Spatial_Relation class assignment operator
 */
Feature_Object_Merge_Object_Property_Index_Spatial_Relation&
Feature_Object_Merge_Object_Property_Index_Spatial_Relation::
operator=( const Feature_Object_Merge_Object_Property_Index_Spatial_Relation& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Merge_Object_Property_Index_Spatial_Relation::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Merge_Object_Property_Index_Spatial_Relation::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const Grounding* context){
    const Object * object = dynamic_cast< const Object* >( grounding );
    if( ( object != NULL ) && ( !children.empty() ) ){
        pair< const Phrase*, const Object_Property* > object_property_child( NULL, NULL );
        pair< const Phrase*, const Spatial_Relation* > spatial_relation_child( NULL, NULL );
        
        for( unsigned int i = 0; i < children.size(); i++ ){
            for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
                if ( dynamic_cast< const Spatial_Relation* >( children[ i ].second[ j ] ) != NULL ){
                    spatial_relation_child.first = children[ i ].first;
                    spatial_relation_child.second = static_cast< const Spatial_Relation* >( children[ i ].second[ j ] );
                }
            }
        }
        if( spatial_relation_child.second == NULL ){
          return false;
        }
        for( unsigned int i = 0; i < children.size(); i++ ){
            if( children[ i ].first != spatial_relation_child.first ){
                for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
                    if( dynamic_cast< const Object_Property* >( children[ i ].second[ j ] ) != NULL ){
                        object_property_child.first = children[ i ].first;
                       object_property_child.second = static_cast< const Object_Property* >( children[ i ].second[ j ] );
                    }
                }
            }
        }
       
        if( ( object_property_child.first != NULL ) && ( object_property_child.second != NULL ) && 
            ( spatial_relation_child.first != NULL ) && ( spatial_relation_child.second != NULL ) ){
            if( ( object_property_child.first->min_word_order() < spatial_relation_child.first->min_word_order() ) ){
                if( spatial_relation_child.second->relation_type() == string( "center" ) ){
                    map< string, vector< Object* > >::const_iterator it1;
                    it1 = world->min_center_distance_sorted_objects().find( object_property_child.second->type() );

//                    map<string, unsigned int>::const_iterator it2;
//                    it2 = world->index_map().find( object_property_child.second->index() );
                    
                    if( object_property_child.second->index() < it1->second.size() ){
                            if( *object == *it1->second[ object_property_child.second->index() ] ){
                                return !_invert;
                            }
                    }

                } else if ( spatial_relation_child.second->relation_type() == string( "far" ) ){
                    map< string, vector< Object* > >::const_iterator it1;
                    it1 = world->max_distance_sorted_objects().find( object_property_child.second->type() );

//                    map<string, unsigned int>::const_iterator it2;
//                    it2 = world->index_map().find( object_property_child.second->index() );
                    
                    if( object_property_child.second->index() < it1->second.size() ){
                            if( *object == *it1->second[ object_property_child.second->index() ] ){
                                return !_invert;
                            }
                    }

                } else if ( spatial_relation_child.second->relation_type() == string( "near" ) ){
                    map< string, vector< Object* > >::const_iterator it1;
                    it1 = world->min_distance_sorted_objects().find( object_property_child.second->type() );

//                    map<string, unsigned int>::const_iterator it2;
//                    it2 = world->index_map().find( object_property_child.second->index() );
                    
                    if( object_property_child.second->index() < it1->second.size() ){
                          if( *object == *it1->second[ object_property_child.second->index() ] ){
                              return !_invert;
                          }
                    }

                } else if ( spatial_relation_child.second->relation_type() == string( "left" ) ){
                    map< string, vector< Object* > >::const_iterator it1 = world->min_y_sorted_objects().find( object_property_child.second->type() );
                    assert( it1 != world->min_y_sorted_objects().end() );

//                    map<string, unsigned int>::const_iterator it2 = world->index_map().find( object_property_child.second->index() );
//                    assert( it2 != world->index_map().end() );
 
                    if( object_property_child.second->index() < it1->second.size() ){
                          if( *object == *it1->second[ object_property_child.second->index() ] ){
                              return !_invert;
                          }
                    }

                } else if ( spatial_relation_child.second->relation_type() == string( "right" ) ){
                    map< string, vector< Object* > >::const_iterator it1 = world->max_y_sorted_objects().find( object_property_child.second->type() );
                    assert( it1 != world->max_y_sorted_objects().end() );                  
  
//                    map<string, unsigned int>::const_iterator it2 = world->index_map().find( object_property_child.second->index() );
//                    assert( it2 != world->index_map().end() );                   
 
                    if( object_property_child.second->index() < it1->second.size() ){
                        if( *object == *it1->second[ object_property_child.second->index() ] ){
                          return !_invert;
                        }
                    }
                } else if ( spatial_relation_child.second->relation_type() == string( "back") ){
                    map< string, vector< Object* > >::const_iterator it1;
                    it1 = world->max_x_sorted_objects().find( object_property_child.second->type() );

//                    map<string, unsigned int>::const_iterator it2;
//                    it2 = world->index_map().find( object_property_child.second->index() );

                    if( object_property_child.second->index() < it1->second.size() ){
                            if( *object == *it1->second[ object_property_child.second->index() ] ){
                                return !_invert;
                            }
                    }

                } else if ( spatial_relation_child.second->relation_type() == string( "front" ) ){
                    map< string, vector< Object* > >::const_iterator it1;
                    it1 = world->min_x_sorted_objects().find( object_property_child.second->type() );

//                    map<string, unsigned int>::const_iterator it2;
//                    it2 = world->index_map().find( object_property_child.second->index() );
                    
                    if( object_property_child.second->index() < it1->second.size() ){
                            if( *object == *it1->second[ object_property_child.second->index() ] ){
                                return !_invert;
                            }
                    }
               }
               return _invert;
           }
       }
   }
    return false;
}

/**
 * exports the Feature_Object_Merge_Object_Property_Index_Spatial_Relation class to an XML file
 */
void
Feature_Object_Merge_Object_Property_Index_Spatial_Relation::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_merge_object_property_index_spatial_relation" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Object_Merge_Object_Property_Index_Spatial_Relation class from an XML file
 */
void
Feature_Object_Merge_Object_Property_Index_Spatial_Relation::
from_xml( xmlNodePtr root ){
    _invert = false;
    if( root->type == XML_ELEMENT_NODE ){
        xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
        if( tmp != NULL ){
            string invert_string = ( char* )( tmp );
            _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
            xmlFree( tmp );
        }
    }
    return;
}

namespace h2sl {
    /**
     * Feature_Object_Merge_Object_Property_Index_Spatial_Relation class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Merge_Object_Property_Index_Spatial_Relation& other ) {
        out << "class:\"Feature_Object_Merge_Object_Property_Index_Spatial_Relation\" ";
        return out;
    }
    
}
