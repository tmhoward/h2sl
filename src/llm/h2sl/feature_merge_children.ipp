/**
 * @file feature_merge_children.ipp
 *
 * @brief
 *
 * a template used to describe a feature that checks the value of a type field
 */

/**
 * Feature_Merge_Children class constructor
 */
template< class A, class B >
Feature_Merge_Children< A, B >::
Feature_Merge_Children( const bool& invert,
                                const bool& order,
                                const bool& same ) : Feature( invert ),
                                                              _order( order ),
                                                              _same( same ) {

}

/**
 * Feature_Merge_Children class constructor
 */
template< class A, class B >
Feature_Merge_Children< A, B >::
Feature_Merge_Children( xmlNodePtr root ) : Feature(),
                                                    _order( false ),
                                                    _same( false ) {
  from_xml( root );
}

/**
 * Feature_Merge_Children class copy constructor
 */
template< class A, class B >
Feature_Merge_Children< A, B >::
Feature_Merge_Children( const Feature_Merge_Children< A, B >& other ) : Feature( other ),
                                                                                    _order( other._order ),
                                                                                    _same( other._same ) {

}

/**
 * Feature_Merge_Children class destructor
 */
template< class A, class B >
Feature_Merge_Children< A, B >::
~Feature_Merge_Children() {

}

/** 
 * Feature_Merge_Children class assignment operator
 */
template< class A, class B >
Feature_Merge_Children< A, B >&
Feature_Merge_Children< A, B >::
operator=( const Feature_Merge_Children& other ){
  _invert = other._invert;
  _order = other._order;
  _same = other._same;
  return (*this);
}

template< class A, class B >
bool
Feature_Merge_Children< A, B >::
value( const std::string& cv,
        const Grounding* grounding,
        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

template< class A, class B >
bool
Feature_Merge_Children< A, B >::
value( const std::string& cv,
        const Grounding* grounding,
        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const A * a = dynamic_cast< const A* >( grounding );
  if( ( a != NULL ) && ( !children.empty() ) ){
    std::pair< const Phrase*, const A* > a_child( NULL, NULL );
    std::pair< const Phrase*, const B* > b_child( NULL, NULL );
    // enforce that children come from different phrases
    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        if( a_child.first == NULL ){
          if ( dynamic_cast< const A* >( children[ i ].second[ j ] ) != NULL ){
            a_child.first = children[ i ].first;
            a_child.second = static_cast< const A* >( children[ i ].second[ j ] );
          }
        }
      }
    }
    if( a_child.second == NULL ){
      return false;
    }
    for( unsigned int i = 0; i < children.size(); i++ ){
      if( ( !_same && ( children[ i ].first != a_child.first ) ) || ( _same && ( children[ i ].first == a_child.first ) ) ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( b_child.first == NULL ){
            if( dynamic_cast< const B* >( children[ i ].second[ j ] ) != NULL ){
              b_child.first = children[ i ].first;
              b_child.second = static_cast< const B* >( children[ i ].second[ j ] );
            }
          }
        }
      }
    }

    if( ( a_child.first != NULL ) && ( a_child.second != NULL ) && ( b_child.first != NULL ) && ( b_child.second != NULL ) ){
      if( _order ){
        if( ( !_same && ( a_child.first->min_word_order() > b_child.first->min_word_order() ) ) || _same ){
          if( *a == *a_child.second ){
            return !_invert;
          } else {
            return _invert;
          }
        }
      } else {
        if( ( !_same && ( a_child.first->min_word_order() < b_child.first->min_word_order() ) ) || _same ){
          if( *a == *a_child.second ){
            return !_invert;
          } else {
            return _invert;
          }
        }
      }
    }
  }
  return false;
}

/** 
 * imports the Feature_Merge_Children class from an XML file
 */
template< class A, class B >
void
Feature_Merge_Children< A, B >::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( std::string( "feature_merge_children_" + A::class_name() + "_" + B::class_name() ).c_str() ) ) == 0 ){
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
 * imports the Feature_Merge_Children class from an XML node pointer
 */
template< class A, class B >
void
Feature_Merge_Children< A, B >::
from_xml( xmlNodePtr root ){
  _invert = false;
  _order = false;
  _same = false;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      std::string invert_string = ( char* )( tmp );
      _invert = ( bool )( std::strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
 
    std::pair< bool, bool > order_prop = has_prop< bool >( root, "order" );
    if( order_prop.first ) {
      _order = order_prop.second;
    }

    std::pair< bool, bool > same_prop = has_prop< bool >( root, "same" );
    if( same_prop.first ) {
      _same = same_prop.second;
    }
  }
  return;
}

/**
 * exports the Feature_Merge_Children class to an XML file
 */
template< class A, class B >
void
Feature_Merge_Children< A, B >::
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
 * exports the Feature_Merge_Children class to an XML node pointer
 */
template< class A, class B >
void
Feature_Merge_Children< A, B >::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( std::string( "feature_merge_children_" + A::class_name() + "_" + B::class_name() ).c_str() ), NULL );
  std::stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  std::stringstream order_string;
  order_string << _order;
  xmlNewProp( node, ( const xmlChar* )( "order" ), ( const xmlChar* )( order_string.str().c_str() ) );
  std::stringstream same_string;
  same_string << _same;
  xmlNewProp( node, ( const xmlChar* )( "same" ), ( const xmlChar* )( same_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

/** 
 * Feature_Merge_Children class ostream operator
 */
template< class A, class B >
std::ostream&
operator<<( std::ostream& out,
            const Feature_Merge_Children< A, B >& other ){
  out << "Feature_Merge_Children:( invert:\"" << other.invert() << " order:\"" << other.order() << " same:\"" << other.same() << "\")";
  return out;
}
