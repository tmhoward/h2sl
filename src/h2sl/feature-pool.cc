/**
 * @file feature-pool.cc
 * @author h2sl developers
 *
 * @version 2.0
 *
 * @section license
 *
 * this file is part of h2sl.
 *
 * this program is free software; you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation; either version 2 of the license, or (at
 * your option) any later version.
 *
 * this program is distributed in the hope that it will be useful, but
 * without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the gnu
 * general public license for more details.
 *
 * you should have received a copy of the gnu general public license
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the free
 * software foundation, inc., 51 franklin street, fifth floor, boston, ma
 * 02110-1301, usa.
 *
 * @section description
 */

#include <fstream>
#include <sstream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <boost/algorithm/string.hpp>

#include "h2sl/common.h"
#include "h2sl/feature-pool.h"
#include "h2sl/feature-cv.h"
#include "h2sl/feature-word.h"
#include "h2sl/feature-num-words.h"
#include "h2sl/feature-lv-type.h"
#include "h2sl/feature-lv-edge-label.h"
#include "h2sl/feature-lv-role-value.h"
#include "h2sl/feature-object-merge-object-type-region.h"
#include "h2sl/feature-object-merge-object-type-spatial-relation.h"
#include "h2sl/feature-object-position-order.h"
#include "h2sl/feature-object-less-position-order.h"
#include "h2sl/feature-object-is-unique.h"
#include "h2sl/feature-symbol-attribute-value.h"
#include "h2sl/feature-symbol-type.h"
#include "h2sl/feature-symbol-has-attribute.h"
#include "h2sl/feature-symbol-has-child.h"
#include "h2sl/feature-symbol-matches-child.h"
#include "h2sl/feature-symbol-child-attribute-value.h"
#include "h2sl/feature-symbol-attribute-value-matches-child-attribute-value.h"
#include "h2sl/feature-symbol-attribute-value-matches-child-attribute-value-label.h"
#include "h2sl/feature-symbol-attribute-value-matches-lv-role.h"
#include "h2sl/feature-symbol-property-csv-contains-child-property-value.h"
#include "h2sl/feature-spatial-relation-children-satisfied.h"
#include "h2sl/feature-spatial-relation-children-satisfied-object-type.h"
#include "h2sl/feature-spatial-relation-children-contradicted.h"
#include "h2sl/feature-spatial-relation-position.h"
#include "h2sl/feature-spatial-relation-position-object-type.h"
#include "h2sl/feature-spatial-relation-merge-object-spatial-relation.h"

namespace h2sl {

///
/// FeaturePool constructor from xml
///
FeaturePool::FeaturePool( const tinyxml2::XMLElement* root ) : constituent_feature_sets(), symbol_features(), language_variable_features() {
  from_xml( root );
}

///
/// Method to evaluate expressed features
///
std::map< std::string, FeaturePool::indices_t >
FeaturePool::evaluate( const std::shared_ptr<std::vector< std::shared_ptr<std::string> > >& cvs,
                        const std::shared_ptr<h2sl::Symbol>& symbol,
                        const std::shared_ptr<h2sl::LanguageVariable>& lv,
                        const std::shared_ptr<h2sl::WorldDCG>& world,
                        const bool& debug )
{
  // Initialize the return structure to hold the indices for features evaluated to be True
  // map: (key) CV Value as string, (value) structure of indices
  auto true_feature_indices = std::map< std::string, FeaturePool::indices_t >();

  // Enforce that there are CV values to use
  if( cvs->empty() )
    return true_feature_indices;

  // Enforce that this feature_set's constituent_feature_sets is of size 3 (a general solution likely exists)
  if( constituent_feature_sets.size() != 3 ){
    std::stringstream error_msg;
    error_msg << "[FeatureSet Class Error] The constituent_feature_sets member is not size 3, unable to evaluate.";
    throw std::runtime_error( error_msg.str() );
  }

  // Initialize the indices for each CV value
  for( const auto& p_cv : *cvs ){
    // Add a new indices_t element for each CV value
    auto [ it, result ]  = true_feature_indices.emplace( *p_cv, FeaturePool::indices_t() );
    // Resize the constituent_feature_set_indices vector to reflect the constituent_feature_set member (a vector for each set)
    it->second.constituent_feature_set_indices.resize( constituent_feature_sets.size() );
  }

  if( debug ){
    std::cout << "Now evaluating non-CV constituent features..." << std::endl;
  }

  // Evaluate the individual constituent features and store their indices into the constituent_feature_sets member
  // Wait to evaluate the CV features - prevents redundant evaluation of feature evaluations that are independent of the CV value
  // i is the index into the particular set of constituent features
  for( size_t i = 0; i < constituent_feature_sets.size(); ++i ){
    // j is the index for a particular constituent features in the i'th set
    for( size_t j = 0; j < constituent_feature_sets[i].size(); ++j ){
      // Do not evaluate the CV features at this point, see above comment.
      if( constituent_feature_sets[i][j]->type == FeatureType::CorrespondenceVariable ){
        if( debug ){
          std::cout << "Skipping " << constituent_feature_sets[i][j]->print_string()
                    << " at index [" << i << "][" << j << "]" << std::endl;
        }
      } else{
        // Evaluate the feature using the Factor's language variable, world, and symbol
        // Use any CV value since any CV dependent features were skipped (see above)
        constituent_feature_sets[i][j]->evaluate( *((*cvs).begin()), lv, world, symbol ); // TODO - Dependency struct?
        if( debug ){
          std::cout << "Evaluated " << constituent_feature_sets[i][j]->value << " for "
                    << constituent_feature_sets[i][j]->print_string() << " at index " << j << std::endl;
        }
        // Handle features that are evaluated to TRUE; Store the constituent feature set index for all CV values
        if( constituent_feature_sets[i][j]->value == FeatureValue::True ){
          if( debug ){
            std::cout << "Storing evaluated True feature for " << constituent_feature_sets[i][j]->print_string()
                      << " at index " << j << std::endl;
          }
          for( auto& kv_true_feature_indices : true_feature_indices ){
            // i is the particular set of constituent features, j is the index into that set for this feature
            kv_true_feature_indices.second.constituent_feature_set_indices[i].push_back( j );
          }
        }
      }
    }
  }

  if( debug ){
    std::cout << "Now evaluating CV constituent features..." << std::endl;
  }

  // Evaluate the CV features for each CV Value - Do not re-evaluate the other features
  // i is the index into the particular set of constituent features
  for( size_t i = 0; i < constituent_feature_sets.size(); ++i ){
    // j is the index for a particular constituent features in the i'th set
    for( size_t j = 0; j < constituent_feature_sets[i].size(); ++j ){
      // Only handle CV features
      if( constituent_feature_sets[i][j]->type == FeatureType::CorrespondenceVariable ){
        // Evaluate the feature for all CV values
        for( const auto& p_cv : *cvs ){
          constituent_feature_sets[i][j]->evaluate( p_cv, lv, world, symbol );
          // Store the index if the feature is True
          if( constituent_feature_sets[i][j]->value == FeatureValue::True ){
            auto it_true_feature_indices = true_feature_indices.find( *p_cv );
            if( it_true_feature_indices != true_feature_indices.end() ){
              it_true_feature_indices->second.constituent_feature_set_indices[i].push_back( j );
            } else{
              // Throw an error if there is no associated indices_t element for the CV value
              std::stringstream error_msg;
              error_msg << "[FeatureSet Class Error] Could not locate " << *p_cv << " cv in true_feature_indices in evaluate";
              throw std::runtime_error( error_msg.str() );
            }
          }
        }
      }
    }
  }

  // Debug printouts to show the content of the stored indices so far
  if( debug ){
    std::cout << "true_feature_indices[" << true_feature_indices.size() << "]:{";
    for( const auto& kv_true_feature_indices : true_feature_indices ){
      auto& r_constituent_feature_set_indices = kv_true_feature_indices.second.constituent_feature_set_indices;
      std::cout << "[" << r_constituent_feature_set_indices.size() << "]:{";
      for( const auto& v_indices : r_constituent_feature_set_indices ){
        std::cout << "[" << v_indices.size() << "]:{";
        for( const auto& index : v_indices ){
          std::cout << index << ",";
        }
        std::cout << "}";
      }
      std::cout << "}";
    }
    std::cout << "}" << std::endl;

    std::cout << "constituent_feature_sets dimensions[3] = {"
              << constituent_feature_sets[0].size() << ","
              << constituent_feature_sets[1].size() << ","
              << constituent_feature_sets[2].size() << "}" << std::endl;
  }

  if( debug ){
    std::cout << "Now computing weight vector indices..." << std::endl;
  }

  // Convert and store the constituent_feature_set_indices to the feature_indices (indices into the weight vector)
  for( auto& [ key, indices ] : true_feature_indices ){
    // Reserve memory for the feature_indices
    size_t reserve_size = indices.constituent_feature_set_indices[0].size()
                          * indices.constituent_feature_set_indices[1].size()
                          * indices.constituent_feature_set_indices[2].size();
    indices.feature_indices.reserve( reserve_size );

    // Hardcode a solution for 3 constituent feature sets
    for( size_t cv_index = 0; cv_index < indices.constituent_feature_set_indices[0].size(); ++cv_index ){
    for( size_t lang_index = 0; lang_index < indices.constituent_feature_set_indices[1].size(); ++lang_index ){
    for( size_t symbol_index = 0; symbol_index < indices.constituent_feature_set_indices[2].size(); ++symbol_index ){
      // Construct a local feature_indices_t object to store the weight index and the constituent feature sets indices
      auto feature_index = FeaturePool::feature_indices_t();
      feature_index.cv() = indices.constituent_feature_set_indices[0][cv_index];
      feature_index.language() = indices.constituent_feature_set_indices[1][lang_index];
      feature_index.symbol() = indices.constituent_feature_set_indices[2][symbol_index];
      feature_index.weight = convert_to_weight_index( feature_index.cv(), feature_index.language(), feature_index.symbol() );
      // Add the feature_index to indices
      indices.feature_indices.push_back( feature_index );
    } } }
  }

  // Add debug printouts for the weight indices
  if( debug ){
    std::cout << "true_feature_indices[" << true_feature_indices.size() << "]:{";
    for( const auto& kv_true_feature_indices : true_feature_indices ){
      auto& r_feature_indices = kv_true_feature_indices.second.feature_indices;
      std::cout << "[" << r_feature_indices.size() << "]:{";
      for( const auto& feature_index : r_feature_indices ){
        std::cout << feature_index.weight << ",";
      }
      std::cout << "}";
    }
    std::cout << "}" << std::endl;
  }

  return true_feature_indices;
}

///
/// Method to return the total number of features in the feature pool
///
unsigned int FeaturePool::num_constituent_features( void )const{
  unsigned int tmp = 0;
  if( constituent_feature_sets.size() > 0 ){
    tmp = 1;
  }
  for( const auto& constituent_feature_set : constituent_feature_sets ){
    tmp *= constituent_feature_set.size();
  }
  return tmp;
}

///
/// Method to load from a file
///
bool FeaturePool::from_file( const std::string& filename ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return from_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return from_json( filename );
  } else {
    std::cerr << "[FeaturePool Class Error] Erroneous file suffix (\"" << filename << "\") in from_file";
    return false;
  }
}

///
/// Method to load from an XML file
///
bool FeaturePool::from_xml( const std::string& filename ) {
  tinyxml2::XMLDocument document;
  document.LoadFile(filename.c_str());
  const tinyxml2::XMLElement *root = document.FirstChildElement("root");
  if (root != nullptr) {
    const tinyxml2::XMLElement *child = root->FirstChildElement();
    from_xml(child);
  }
  return document.ErrorID();
}

///
/// Method to load from a tinyxml2 element
///
bool FeaturePool::from_xml( const tinyxml2::XMLElement* tmp ){
  // clear all current constituent_feature_sets
  constituent_feature_sets.clear();

  // Import the feature vectors
  const tinyxml2::XMLElement* constituent_feature_set_elem = tmp->FirstChildElement("constituent-feature-set");
  while( constituent_feature_set_elem != nullptr ){
    constituent_feature_sets.push_back( std::vector< std::shared_ptr< h2sl::Feature > >() );

    const tinyxml2::XMLElement* feature_elem = constituent_feature_set_elem->FirstChildElement("feature");
    while( feature_elem != nullptr ){
      // Read the class attribute to determine what feature to load
      const tinyxml2::XMLAttribute* class_attr = feature_elem->FindAttribute("class");
      if( class_attr == nullptr ){
        // could not find the class attribute, return false
        try {
          std::stringstream error_msg;
          error_msg << "[FeaturePool Class Error] \"class\" attribute missing from feature in from_xml";
          throw std::runtime_error( error_msg.str() );
        } catch ( const char* msg ){
          std::cerr << msg << std::endl;
        }

        return false;
      }
      std::string class_name = class_attr->Value();
      if( class_name == "feature-cv" ){
        // load a FeatureCV if the class name is feature-cv
        constituent_feature_sets.back().push_back( std::make_shared<FeatureCV>( feature_elem ) );
      } else if ( class_name == "feature-word" ){
        // load a FeatureWord if the class name is feature-word
        constituent_feature_sets.back().push_back( std::make_shared<FeatureWord>( feature_elem ) );
      } else if ( class_name == "feature-num-words" ){
        // load a FeatureNumWords if the class name is feature-num-words
        constituent_feature_sets.back().push_back( std::make_shared<FeatureNumWords>( feature_elem ) );
      } else if ( class_name == "feature-lv-type" ){
        // load a FeatureLVType if the class name is feature-lv-type
        constituent_feature_sets.back().push_back( std::make_shared<FeatureLVType>( feature_elem ) );
      } else if ( class_name == "feature-lv-edge-label" ){
        // load a FeatureLVEdgeLabel if the class name is feature-lv-edge-label
        constituent_feature_sets.back().push_back( std::make_shared<FeatureLVEdgeLabel>( feature_elem ) );
      } else if ( class_name == "feature-lv-role-value" ){
        // load a FeatureLVRoleValue if the class name is feature-lv-role-value
        constituent_feature_sets.back().push_back( std::make_shared<FeatureLVRoleValue>( feature_elem ) );
      } else if ( class_name == "feature-object-merge-object-type-region" ){
        // load a FeatureObjectMergeObjectTypeRegion if the class name is feature-object-merge-object-type-region
        constituent_feature_sets.back().push_back( std::make_shared<FeatureObjectMergeObjectTypeRegion>( feature_elem ) );
      } else if ( class_name == "feature-object-merge-object-type-spatial-relation" ){
        // load a FeatureObjectMergeObjectTypeSpatialRelation if the class name is feature-object-merge-object-type-spatial-relation
        constituent_feature_sets.back().push_back( std::make_shared<FeatureObjectMergeObjectTypeSpatialRelation>( feature_elem ) );
      } else if ( class_name == "feature-spatial-relation-children-satisfied" ){
        // load a FeatureSpatialRelationChildrenSatisfied
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSpatialRelationChildrenSatisfied>( feature_elem ) );
      } else if ( class_name == "feature-spatial-relation-children-satisfied-object-type" ){
        // load a FeatureSpatialRelationChildrenSatisfiedObjectType
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSpatialRelationChildrenSatisfiedObjectType>( feature_elem ) );
      } else if ( class_name == "feature-spatial-relation-children-contradicted" ){
        // load a FeatureSpatialRelationChildrenContradicted
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSpatialRelationChildrenContradicted>( feature_elem ) );
      } else if ( class_name == "feature-spatial-relation-position" ){
        // load a FeatureSpatialRelationPosition
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSpatialRelationPosition>( feature_elem ) );
      } else if ( class_name == "feature-spatial-relation-position-object-type" ){
        // load a FeatureSpatialRelationPositionObjectType
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSpatialRelationPositionObjectType>( feature_elem ) );
      } else if ( class_name == "feature-spatial-relation-merge-object-spatial-relation" ){
        // load a FeatureSpatialRelationMergeObjectSpatialRelation
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSpatialRelationMergeObjectSpatialRelation>( feature_elem ) );
      } else if ( class_name == "feature-object-position-order" ){
        // load a FeatureObjectPositionOrder if the class name is feature-object-position-order
        constituent_feature_sets.back().push_back( std::make_shared<FeatureObjectPositionOrder>( feature_elem ) );
      } else if ( class_name == "feature-object-less-position-order" ){
        // load a FeatureObjectLessPositionOrder if the class name is feature-object-position-order
        constituent_feature_sets.back().push_back( std::make_shared<FeatureObjectLessPositionOrder>( feature_elem ) );
      } else if ( class_name == "feature-object-is-unique" ){
        // load a FeatureObjectIsUnique if the class name is feature-object-is-unique
        constituent_feature_sets.back().push_back( std::make_shared<FeatureObjectIsUnique>( feature_elem ) );
      } else if ( class_name == "feature-symbol-attribute-value" ){
        // load a FeatureSymbolAttributeValue if the class name is feature-symbol-attribute-value
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSymbolAttributeValue>( feature_elem ) );
      } else if ( class_name == "feature-symbol-type" ){
        // load a FeatureSymbolType if the class name is feature-symbol-type
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSymbolType>( feature_elem ) );
      } else if ( class_name == "feature-symbol-has-attribute" ){
        // load a FeatureSymbolHasAttribute if the class name is feature-symbol-has-attribute
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSymbolHasAttribute>( feature_elem ) );
      } else if ( class_name == "feature-symbol-has-child" ){
        // load a FeatureSymbolHasChild if the class name is feature-symbol-has-child
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSymbolHasChild>( feature_elem ) );
      } else if ( class_name == "feature-symbol-matches-child" ){
        // load a FeatureSymbolMatchesChild if the class name is feature-symbol-matches-child
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSymbolMatchesChild>( feature_elem ) );
      } else if ( class_name == "feature-symbol-child-attribute-value" ){
        // load a FeatureSymbolChildAttributeValue if the class name is feature-symbol-child-attribute-value
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSymbolChildAttributeValue>( feature_elem ) );
      } else if ( class_name == "feature-symbol-attribute-value-matches-child-attribute-value" ){
        // load a FeatureSymbolAttributeValueMatchesChildAttributeValue if the class name is feature-symbol-attribute-value-matches-child-attribute-value
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSymbolAttributeValueMatchesChildAttributeValue>( feature_elem ) );
      } else if ( class_name == "feature-symbol-attribute-value-matches-child-attribute-value-label" ){
        // load a FeatureSymbolAttributeValueMatchesChildAttributeValueLabel if the class name is feature-symbol-attribute-value-matches-child-attribute-value-label
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSymbolAttributeValueMatchesChildAttributeValueLabel>( feature_elem ) );
      } else if ( class_name == "feature-symbol-attribute-value-matches-lv-role" ){
        // load a FeatureSymbolAttributeValueMatchesLVRole if the class name is feature-symbol-attribute-value-matches-lv-role
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSymbolAttributeValueMatchesLVRole>( feature_elem ) );
      } else if ( class_name == "feature-symbol-property-csv-contains-child-property-value" ){
        // load a FeatureSymbolPropertyCSVContainsChildPropertyValue if the class name is feature-symbol-property-csv-contains-child-property-value
        constituent_feature_sets.back().push_back( std::make_shared<FeatureSymbolPropertyCSVContainsChildPropertyValue>( feature_elem ) );
      } else {
        // did not find the class name, return false
        try {
          std::stringstream error_msg;
          error_msg << "[FeaturePool Class Error] \"class\" attribute value (" << class_name << ") not known in from_xml";
          throw std::runtime_error( error_msg.str() );
        } catch ( const char* msg ){
          std::cerr << msg << std::endl;
        }

        return false;
      }
      // find the next feature in the feature vector
      feature_elem = feature_elem->NextSiblingElement("feature");
    }
    // find the next feature vector in the pool of features
    constituent_feature_set_elem = constituent_feature_set_elem->NextSiblingElement("constituent-feature-set");
  }

  return true;
}

///
/// Method to load from a json formatted file
///
bool FeaturePool::from_json( const std::string& filename ){
  std::ifstream json_file( filename );
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_file, root );
  return from_json( root );
}

///
/// Method to load from a json formatted string
///
bool FeaturePool::from_json_string( const std::string& value ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( value, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeaturePool::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a file
///
void FeaturePool::to_file( const std::string& filename )const {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return to_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return to_json( filename );
  } else {
    try {
      std::stringstream error_msg;
      error_msg << "[FeaturePool Class Error] Erroneous file suffix (\"" << filename << "\") in to_file";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
  }
  return;
}

///
/// Method to write to an XML file
///
void FeaturePool::to_xml( const std::string& filename )const {
  tinyxml2::XMLDocument document;
  tinyxml2::XMLElement * root = document.NewElement( "root" );
  document.InsertEndChild( root );
  to_xml( document, root );
  document.SaveFile( filename.c_str() );
  return;
}

///
/// Method to write to a tinyxml2 document
///
void FeaturePool::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_pool_elem = document.NewElement( "feature-pool" );
  for( const auto& constituent_feature_set : constituent_feature_sets ){
    tinyxml2::XMLElement* constituent_feature_set_elem = document.NewElement( "constituent-feature-set" );
    for( const auto& feature : constituent_feature_set ){
      feature->to_xml( document, constituent_feature_set_elem);
    }
    feature_pool_elem->InsertEndChild( constituent_feature_set_elem );
  }
  root->InsertEndChild( feature_pool_elem );
}

///
/// Method to write to a json file
///
void FeaturePool::to_json( const std::string& filename )const{
  std::ofstream outfile;
  outfile.open( filename );
  outfile << to_json();
  outfile.close();
  return;
}

///
/// Method to write to a json formatted string
///
std::string FeaturePool::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeaturePool::to_json( Json::Value& root )const{
  return;
}

///
/// FeaturePool class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeaturePool& other) {
  out << "constituent_feature_sets[" << other.constituent_feature_sets.size() << "]:{" << std::endl;
  for( const auto& constituent_feature_set : other.constituent_feature_sets ){
    out << "  constituent_feature_set[" << constituent_feature_set.size() << "]:{" << std::endl;
    for( const auto& feature : constituent_feature_set ){
      out << "    " << feature->print_string() << std::endl;
    }
    out << "  }" << std::endl;
  }
  out << "}";
  return out;
}

} // namespace h2sl
