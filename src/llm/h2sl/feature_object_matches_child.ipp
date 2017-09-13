/**
 * @file feature_object_matches_child.ipp
 *
 * @brief
 *
 * a template used to describe a feature that checks the value of a type field
 */

/**
 * Feature_Object_Matches_Child class constructor
 */
template< class T, class O >
Feature_Object_Matches_Child< T, O >::
Feature_Object_Matches_Child( const bool& invert ) : Feature( invert ) {

}

/**
 * Feature_Object_Matches_Child class constructor
 */
template< class T, class O >
Feature_Object_Matches_Child< T, O >::
Feature_Object_Matches_Child( xmlNodePtr root ) : Feature() {
  from_xml( root );
}

/**
 * Feature_Object_Matches_Child class copy constructor
 */
template< class T, class O >
Feature_Object_Matches_Child< T, O >::
Feature_Object_Matches_Child( const Feature_Object_Matches_Child< T, O >& other ) : Feature( other ) {

}

/**
 * Feature_Object_Matches_Child class destructor
 */
template< class T, class O >
Feature_Object_Matches_Child< T, O >::
~Feature_Object_Matches_Child() {

}

/** 
 * Feature_Object_Matches_Child class assignment operator
 */
template< class T, class O >
Feature_Object_Matches_Child< T, O >&
Feature_Object_Matches_Child< T, O >::
operator=( const Feature_Object_Matches_Child& other ){
  _invert = other._invert;
  return (*this);
}

template< class T, class O >
bool
Feature_Object_Matches_Child< T, O >::
value( const std::string& cv,
        const Grounding* grounding,
        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

template< class T, class O >
bool
Feature_Object_Matches_Child< T, O >::
value( const std::string& cv,
        const Grounding* grounding,
        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const T * symbol = dynamic_cast< const T* >( grounding );
  if( symbol != NULL ){
    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        const O * child = dynamic_cast< const O* >( children[ i ].second[ j ] );
        if( child != NULL ){
          typename std::map< std::string, O* >::const_iterator it_object = world->objects().find( symbol->object_id() );
          //assert( it_object != world->objects().end() );
          if( it_object != world->objects().end() ){
            if( *(it_object->second) == *child ){
              return !_invert;
            }
          }
        }
      }
    }
    if( !children.empty() ){
      return _invert;
    }
  }
  return false;
}

/** 
 * imports the Feature_Object_Matches_Child class from an XML file
 */
template< class T, class O >
void
Feature_Object_Matches_Child< T, O >::
from_xml( const std::string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( std::string( "feature_object_matches_child_" + T::class_name() ).c_str() ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
      xmlFreeDoc( doc );
    }
  }
  return;
}

/** 
 * imports the Feature_Object_Matches_Child class from an XML node pointer
 */
template< class T, class O >
void
Feature_Object_Matches_Child< T, O >::
from_xml( xmlNodePtr root ){
  _invert = false;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      std::string invert_string = ( char* )( tmp );
      _invert = ( bool )( std::strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
  }
  return;
}

/**
 * exports the Feature_Object_Matches_Child class to an XML file
 */
template< class T, class O >
void
Feature_Object_Matches_Child< T, O >::
to_xml( const std::string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

/**
 * exports the Feature_Object_Matches_Child class to an XML node pointer
 */
template< class T, class O >
void
Feature_Object_Matches_Child< T, O >::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( std::string( "feature_object_matches_child_" + T::class_name() ).c_str() ), NULL );
  std::stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

/** 
 * Feature_Object_Matches_Child class ostream operator
 */
template< class T, class O >
std::ostream&
operator<<( std::ostream& out,
            const Feature_Object_Matches_Child< T, O >& other ){
  out << "Feature_Object_Matches_Child:( invert:\"" << other.invert() << "\")";
  return out;
}
