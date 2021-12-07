/**
 * @file feature-pool-generator-cky-sr.cc
 * @author H2SL Developers
 *
 * @version 2.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
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
 */

#include <boost/program_options.hpp>
#include "h2sl/feature-pool.h"
#include "h2sl/lexicon.h"
#include "h2sl/world.h"

#include "h2sl/feature-cv.h"
#include "h2sl/feature-word.h"
#include "h2sl/feature-num-words.h"
#include "h2sl/feature-symbol-attribute-value.h"
#include "h2sl/feature-symbol-type.h"
#include "h2sl/feature-symbol-has-attribute.h"
#include "h2sl/feature-symbol-has-child.h"
#include "h2sl/feature-symbol-matches-child.h"
#include "h2sl/feature-symbol-attribute-value-matches-child-attribute-value.h"
#include "h2sl/feature-symbol-property-csv-contains-child-property-value.h"
#include "h2sl/feature-object-merge-object-type-region.h"
#include "h2sl/feature-object-merge-object-type-spatial-relation.h"
#include "h2sl/feature-object-position-order.h"
#include "h2sl/feature-object-less-position-order.h"
#include "h2sl/feature-object-is-unique.h"
#include "h2sl/feature-spatial-relation-children-satisfied.h"
#include "h2sl/feature-spatial-relation-children-satisfied-object-type.h"
#include "h2sl/feature-spatial-relation-children-contradicted.h"
#include "h2sl/feature-spatial-relation-position.h"
#include "h2sl/feature-spatial-relation-position-object-type.h"
#include "h2sl/feature-spatial-relation-merge-object-spatial-relation.h"


int main( int argc, char* argv[] ){
  std::cout << "Start of the feature-pool-generator program." << std::endl;

  boost::program_options::options_description desc{"Options"};
  desc.add_options()("help,h", "Help screen")(
      "input,i", boost::program_options::value<std::string>(), "Input file")(
      "sd,s", boost::program_options::value<std::string>(), "symbol dictionary file")(
      "lexicon,l", boost::program_options::value<std::string>(), "lexicon file")(
      "output,o", boost::program_options::value<std::string>(), "Output file");

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  // Create an object of FeaturePool
  h2sl::FeaturePool feature_pool;

  // Load feature-pool from the symbol dictionary and the lexicon
  std::cout << "Loading features using lexicon and symbol dictionary" << std::endl;

  // Exit if symbol dictionary is not provided
  if( not vm.count("sd") ){
    std::cout << "Symbol dictionary not provided, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }

  // Exit if Lexicon is not provided
  if( not vm.count("lexicon") ){
    std::cout << "Lexicon not provided, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }

  // Load symbol dictionary from the file
  auto sd = h2sl::SymbolDictionary();
  if( sd.from_file( vm["sd"].as<std::string>().c_str() ) ){
    std::cout << "Imported symbol dictionary " << sd << std::endl << std::endl;
  } else {
    std::cout << "Could not load symbol dictionary, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }

  // Load lexicon from the file
  h2sl::Lexicon lx = h2sl::Lexicon();
  if( lx.from_file( vm["lexicon"].as<std::string>().c_str() ) ){
    std::cout << "Imported lexicon " << lx << std::endl << std::endl;
  } else {
    std::cout << "Could not load lexicon, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }


  // Generate CV related features
  feature_pool.constituent_feature_sets.push_back( std::vector< std::shared_ptr< h2sl::Feature > >() );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureCV >( "false" ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureCV >( "true" ) );

  // Generate language related features using the Lexicon
  feature_pool.constituent_feature_sets.push_back( std::vector< std::shared_ptr< h2sl::Feature > >() );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureNumWords >( 0 ) );
  for( const auto& [lv_type, info] : lx.type_information ){
    for( const auto& [pos, text] : info.words ){
      for( const auto& word_text : text ){
        feature_pool.constituent_feature_sets.back().push_back( std::make_shared<h2sl::FeatureWord>( pos, word_text ) );
      }
    }
  }

  // Load Symbol related features using the Symbol Dictionary
  feature_pool.constituent_feature_sets.push_back( std::vector< std::shared_ptr< h2sl::Feature > >() );

  // Features agnostic to the symbol type
  for(const auto& [symbol_type, info] : sd.dictionary ){
    // List the features that need to be tempalated for all of the symbols types in the symbol dictionary
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolType >( symbol_type ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolMatchesChild >( symbol_type ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolHasChild >( symbol_type ) );
  }

  // Features specific to the symbol type

  // Object related features
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolType >( "object" ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolMatchesChild >( "object" ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectIsUnique>(true) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectIsUnique>(false) );
  auto it_object_type = sd.dictionary.find( "object_type" );
  if( it_object_type != sd.dictionary.end() ){
    auto it_prop = it_object_type->second->properties.find("object_type");
    for(const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "object", it_prop->first, property_value, true ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "object", it_prop->first, property_value, false ) );
    }
  }

  // Object_Type related features
  it_object_type = sd.dictionary.find( "object_type" );
  if( it_object_type != sd.dictionary.end() ){
    auto it_prop = it_object_type->second->properties.find("object_type");
    for(const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_object_type->first, it_prop->first, property_value, true ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_object_type->first, it_prop->first, property_value, false ) );
    }
  }

  // Spatial Relation related features
  auto it_spatial_relation = sd.dictionary.find("spatial_relation");
  if( it_spatial_relation != sd.dictionary.end() ){
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationMergeObjectSpatialRelation >( true ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationMergeObjectSpatialRelation >( false ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolHasChild >( "spatial_relation", true ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared<h2sl::FeatureSymbolHasAttribute>( "spatial_relation", "landmark", false) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared<h2sl::FeatureSymbolHasAttribute>( "spatial_relation", "landmark", true) );
    // Symbol Attribute Features
    auto it_prop = it_spatial_relation->second->properties.find("viewpoint");
    for( auto const & property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "spatial_relation", it_prop->first, property_value, true ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "spatial_relation", it_prop->first, property_value, false ) );
    }
    it_prop = it_spatial_relation->second->properties.find("axis");
    for( auto const & property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "spatial_relation", it_prop->first, property_value, true ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "spatial_relation", it_prop->first, property_value, false ) );
    }
    it_prop = it_spatial_relation->second->properties.find("extrema");
    for( auto const & property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "spatial_relation", it_prop->first, property_value, true ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "spatial_relation", it_prop->first, property_value, false ) );
    }
    // Generate default features for binary spatial relations
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationChildrenSatisfied >( ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationChildrenContradicted >( ) );
    it_object_type = sd.dictionary.find( "object_type" );
    if( it_object_type != sd.dictionary.end() ){
      auto it_prop = it_object_type->second->properties.find("object_type");
      for( const auto& property_value : it_prop->second ){
//        feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationChildrenSatisfiedObjectType>( property_value ) );
      }
    }

    // Generate default features for ordinal spatial relations
    for( int position_index = 0; position_index < 3; position_index++ ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationPosition >( position_index ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationPositionObjectType >( position_index ) );
    }

    // Compare the axis property to children
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "spatial_relation", "axis", "spatial_relation", "axis" ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "spatial_relation", "axis", "spatial_relation", "axis", true ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "spatial_relation", "extrema", "spatial_relation", "extrema" ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "spatial_relation", "extrema", "spatial_relation", "extrema", true ) );
    // Allow capturing of landmarks from grounded child symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "spatial_relation", "landmark", "object", "uid" ) );
    // Add the inverted feature to provide useful negative information
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "spatial_relation", "landmark", "object", "uid", true ) );
  }

  // Action related features
  auto it_action = sd.dictionary.find( "action" );
  if( it_action != sd.dictionary.end() ){
    auto it_prop = it_action->second->properties.find("action_type");
    for(const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_action->first, it_prop->first, property_value ) );
    }
    auto it_agent = it_action->second->properties.find("agent");
    for( const auto& agent : it_agent->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_action->first, it_agent->first, agent ) );
    }
    // Enable capturing of payload from grounded child object symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "action", "payload", "object", "uid" ) );
  }

  std::cout << "Success!" << std::endl;
  std::cout << "Loaded the feature_pool: " << feature_pool << std::endl;

  if (vm.count("output")) {
    std::cout << "writing feature_pool to \"" << vm["output"].as<std::string>() << "\"" << std::endl;
    feature_pool.to_file(vm["output"].as<std::string>());
  }

  std::cout << "End of the feature-pool-generator program." << std::endl;
  return EXIT_SUCCESS;
}
