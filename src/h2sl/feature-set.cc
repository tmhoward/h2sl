/**
 * @file feature-set.cc
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

#include "h2sl/feature-set.h"

namespace h2sl {

std::map< std::string, FeaturePool::indices_t >
FeatureSet::evaluate( const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvs,
                      const std::shared_ptr< Symbol >& symbol,
                      const std::shared_ptr< LanguageVariable >& lv,
                      const std::shared_ptr< WorldDCG >& world,
                      std::map< std::string, std::vector< ExpressedFeature > >& expressedFeatures,
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

  if( debug ){
    std::cout << "Evaluating the FeatureSet for: \n\tsymbol " << *symbol << "\n\tLV " << *lv << "\n" << std::endl;
  }

  // Initialize the indices for each CV value
  for( const auto& p_cv : *cvs ){
    // Add a new indices_t element for each CV value
    auto [ it, result ]  = true_feature_indices.emplace( *p_cv, FeaturePool::indices_t() );
    // Resize the constituent_feature_set_indices vector to reflect the constituent_feature_set member (a vector for each set)
    it->second.constituent_feature_set_indices.resize( constituent_feature_sets.size() );
    // Add a new element to the ExpressedFeature map
    expressedFeatures.emplace( *p_cv, std::vector< ExpressedFeature >() );
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
        continue;
      }
      // Evaluate DynamicSymbol features and features that are currently unevaluated
      if( (constituent_feature_sets[i][j]->type == FeatureType::DynamicSymbol)
          || ( constituent_feature_sets[i][j]->value == FeatureValue::Unknown ) )
      {
        // Evaluate the feature using the Factor's language variable, world, and symbol
        // Use any CV value since any CV dependent features were skipped (see above)
        constituent_feature_sets[i][j]->evaluate( *((*cvs).begin()), lv, world, symbol );
        if( debug ){
          std::cout << "Evaluated " << constituent_feature_sets[i][j]->value << " for "
                    << constituent_feature_sets[i][j]->print_string() << " at index " << j << std::endl;
        }
      }
      // Handle features that are evaluated to TRUE; Store the constituent feature set index for all CV values
      if( constituent_feature_sets[i][j]->value == FeatureValue::True ){
        if( debug){
          std::cout << "Storing evaluated True feature for " << constituent_feature_sets[i][j]->print_string()
                    << " at index " << j << std::endl;
        }
        for( auto& kv_true_feature_indices : true_feature_indices ){
          // i is the particular set of constituent features, j is the index into that set for this feature
          kv_true_feature_indices.second.constituent_feature_set_indices[i].push_back( j );
        }
        continue;
      }
      // Provide debugging printout for False features
      if( constituent_feature_sets[i][j]->value == FeatureValue::False ){
        if( debug ){
          std::cout << "Not storing evaluated False feature for " << constituent_feature_sets[i][j]->print_string()
                    << " at index " << j << std::endl;
        }
        continue;
      }
      // Throw an error if none of the above conditions were met
      std::stringstream error_msg;
      error_msg << "[FeatureSet Class Error] missing case for feature " << constituent_feature_sets[i][j]->print_string( true ) << " in evaluate";
      throw std::runtime_error( error_msg.str() );
    }
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



  for( auto& [ key, indices ]  : true_feature_indices ){
    auto it_expressed_features = expressedFeatures.find( key );
    if( it_expressed_features == expressedFeatures.end() ){
      std::stringstream error_msg;
      error_msg << "[FeatureSet Class Error] Could not locate \"" << key << "\" cv in expressedFreatures." << std::endl;
      throw std::runtime_error( error_msg.str() );
    }

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

      // Create a new expressed feature
      it_expressed_features->second.push_back( ExpressedFeature() );
      // Add the constituent features
      it_expressed_features->second.back().features.push_back(
                                          constituent_feature_sets[0][indices.constituent_feature_set_indices[0][cv_index]] );
      it_expressed_features->second.back().features.push_back(
                                          constituent_feature_sets[1][indices.constituent_feature_set_indices[1][lang_index]] );
      it_expressed_features->second.back().features.push_back(
                                          constituent_feature_sets[2][indices.constituent_feature_set_indices[2][symbol_index]] );
      // Add the weight index
      it_expressed_features->second.back().index = feature_index.weight;
      // Add the constituent feature set indices
      it_expressed_features->second.back().jkl_indices.emplace("j",cv_index);
      it_expressed_features->second.back().jkl_indices.emplace("k",lang_index);
      it_expressed_features->second.back().jkl_indices.emplace("l",symbol_index);
      // Add the language variable shared_ptr
      it_expressed_features->second.back().language_variable = lv;
      // Add the symbol shared_ptr
      it_expressed_features->second.back().symbol = symbol;
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

unsigned int FeatureSet::num_constituent_features( void )const{
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
bool FeatureSet::from_file( const std::string& filename ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return from_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return from_json( filename );
  } else {
    std::cerr << "[FeatureSet Class Error] Erroneous file suffix (\"" << filename << "\") in from_file";
    return false;
  }
}

///
/// Method to load from an XML file
///
bool FeatureSet::from_xml( const std::string& filename ) {
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
bool FeatureSet::from_xml( const tinyxml2::XMLElement* tmp ){
  return true;
}

///
/// Method to load from a json formatted file
///
bool FeatureSet::from_json( const std::string& filename ){
  std::ifstream json_file( filename );
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_file, root );
  return from_json( root );
}

///
/// Method to load from a json formatted string
///
bool FeatureSet::from_json_string( const std::string& value ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( value, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureSet::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a file
///
void FeatureSet::to_file( const std::string& filename )const {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return to_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return to_json( filename );
  } else {
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSet Class Error] Erroneous file suffix (\"" << filename << "\") in to_file";
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
void FeatureSet::to_xml( const std::string& filename )const {
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
void FeatureSet::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* tmp = document.NewElement( "feature-set" );
  root->InsertEndChild( tmp );
}

///
/// Method to write to a json file
///
void FeatureSet::to_json( const std::string& filename )const{
  std::ofstream outfile;
  outfile.open( filename );
  outfile << to_json();
  outfile.close();
  return;
}

///
/// Method to write to a json formatted string
///
std::string FeatureSet::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureSet::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureSet class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureSet& other) {
  out << "feature_vectors[" << other.constituent_feature_sets.size() << "]:{";
  for( auto it_feature_vector = other.constituent_feature_sets.cbegin(); it_feature_vector != other.constituent_feature_sets.cend(); it_feature_vector++ ){
    out << "features[" << it_feature_vector->size() << "]:{";
    for( auto it_feature = it_feature_vector->cbegin(); it_feature != it_feature_vector->cend(); it_feature++ ){
      out << *(*it_feature) << "(" << *it_feature << ")";
      if( std::next( it_feature ) != it_feature_vector->cend() ){
        out << ",";
      }
    }
    out << "}";
    if( std::next( it_feature_vector ) != other.constituent_feature_sets.cend() ){
      out << ",";
    }
  }
  out << "}";
  return out;
}

} // namespace h2sl
