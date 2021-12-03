/**
 * @file feature-pool-generator-cky.cc
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

#include <boost/program_options.hpp>
#include "h2sl/feature-pool.h"
#include "h2sl/lexicon.h"
#include "h2sl/world.h"

#include "h2sl/feature-cv.h"
#include "h2sl/feature-word.h"
#include "h2sl/feature-num-words.h"
#include "h2sl/feature-symbol-attribute-value.h"
#include "h2sl/feature-symbol-type.h"
#include "h2sl/feature-symbol-matches-child.h"
#include "h2sl/feature-symbol-attribute-value-matches-child-attribute-value.h"
#include "h2sl/feature-symbol-property-csv-contains-child-property-value.h"
#include "h2sl/feature-object-merge-object-type-region.h"
#include "h2sl/feature-object-merge-object-type-spatial-relation.h"
#include "h2sl/feature-object-position-order.h"
#include "h2sl/feature-object-less-position-order.h"


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

  // Features for object position
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MIN_X_AXIS, 0 ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MAX_X_AXIS, 0 ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MIN_X_AXIS, 1 ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MAX_X_AXIS, 1 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MIN_ABS_X_AXIS, 0 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MAX_ABS_X_AXIS, 0 ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MIN_Y_AXIS, 0 ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MAX_Y_AXIS, 0 ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MIN_Y_AXIS, 1 ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MAX_Y_AXIS, 1 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MIN_ABS_Y_AXIS, 0 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MAX_ABS_Y_AXIS, 0 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MIN_Z_AXIS, 0 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MAX_Z_AXIS, 0 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MIN_ABS_Z_AXIS, 0 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MAX_ABS_Z_AXIS, 0 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MIN_EUCLIDEAN_DISTANCE, 0 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MAX_EUCLIDEAN_DISTANCE, 0 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MIN_CENTER_DISTANCE, 0 ) );
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectPositionOrder >( h2sl::World::SortKey::MAX_CENTER_DISTANCE, 0 ) );

  // Features for object less position order
  /*
   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MIN_X_AXIS, 0 ) );
   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MAX_X_AXIS, 0 ) );
   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MIN_X_AXIS, 1 ) );
   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MAX_X_AXIS, 1 ) );
   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MIN_X_AXIS, 2 ) );
   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MAX_X_AXIS, 2 ) );

   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MIN_Y_AXIS, 0 ) );
   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MAX_Y_AXIS, 0 ) );
   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MIN_Y_AXIS, 1 ) );
   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MAX_Y_AXIS, 1 ) );
   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MIN_Y_AXIS, 2 ) );
   feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectLessPositionOrder >( h2sl::World::SortKey::MAX_Y_AXIS, 2 ) );
  */


  // Features agnostic to the symbol type
  for(const auto& symbol_type : sd.dictionary ){
    // List the features that need to be tempalated for all of the symbols types in the symbol dictionary
    // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolType >( symbol_type.first ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolMatchesChild >( symbol_type.first ) );
  }

  // Features specific to the symbol type

  // Object related features
  // feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolType >( "object" ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolMatchesChild >( "object" ) );
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

  // Create object_color symbol and object property features for each known object color
  auto it_object_color = sd.dictionary.find( "object_color" );
  if( it_object_color != sd.dictionary.end() ){
    auto it_prop = it_object_color->second->properties.find("object_color");
    for( const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_object_color->first, it_prop->first, property_value, true ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_object_color->first, it_prop->first, property_value, false ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "object", it_prop->first, property_value, true ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "object", it_prop->first, property_value, false ) );
    }
  }

  // Spatial Relation related features
  auto it_spatial_relation = sd.dictionary.find( "spatial_relation" );
  if( it_spatial_relation != sd.dictionary.end() ){
    auto it_prop = it_spatial_relation->second->properties.find("spatial_relation_type");
    for(const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_spatial_relation->first, it_prop->first, property_value ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectMergeObjectTypeSpatialRelation >( property_value ) );
    }
  }

  // Region related features
  auto it_region = sd.dictionary.find( "region" );
  if( it_region != sd.dictionary.end() ){
    auto it_prop = it_region->second->properties.find("spatial_relation_type");
    for(const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_region->first, it_prop->first, property_value ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureObjectMergeObjectTypeRegion >( property_value ) );
    }
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "region", "object", "object", "uid" ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "region", "spatial_relation_type", "spatial_relation", "spatial_relation_type" ) );
  }

  // Relation related features
  auto it_relation = sd.dictionary.find( "relation" );
  if( it_relation != sd.dictionary.end() ){
    auto it_prop = it_relation->second->properties.find("relation_type");
    for(const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_relation->first, it_prop->first, property_value ) );
    }
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "relation", "relation_type", "relation", "relation_type" ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "relation", "regions", "object", "uid" ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolPropertyCSVContainsChildPropertyValue >( "relation", "regions", "object", "uid" ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolPropertyCSVContainsChildPropertyValue >( "relation", "regions", "relation", "regions" ) );
  }

  // Action related features
  auto it_action = sd.dictionary.find( "action" );
  if( it_action != sd.dictionary.end() ){
    auto it_prop = it_action->second->properties.find("action_type");
    for(const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_action->first, it_prop->first, property_value ) );
    }
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "action", "goal", "object", "uid" ) );
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
