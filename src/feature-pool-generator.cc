/**
 * @file feature-pool-generator.cc
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
#include "h2sl/feature-symbol-has-child.h"
#include "h2sl/feature-symbol-matches-child.h"
#include "h2sl/feature-symbol-child-attribute-value.h"
#include "h2sl/feature-symbol-attribute-value-matches-child-attribute-value.h"
#include "h2sl/feature-symbol-attribute-value-matches-child-attribute-value-label.h"
#include "h2sl/feature-symbol-attribute-value-matches-lv-role.h"
#include "h2sl/feature-symbol-property-csv-contains-child-property-value.h"
#include "h2sl/feature-lv-type.h"
#include "h2sl/feature-lv-edge-label.h"
#include "h2sl/feature-lv-role-value.h"
#include "h2sl/feature-spatial-relation-children-satisfied.h"
#include "h2sl/feature-spatial-relation-children-satisfied-object-type.h"
#include "h2sl/feature-spatial-relation-children-contradicted.h"
#include "h2sl/feature-spatial-relation-position.h"
#include "h2sl/feature-spatial-relation-position-object-type.h"

int main( int argc, char* argv[] ){
  std::cout << "Start of the feature-pool-generator program." << std::endl;

  boost::program_options::options_description desc{"Options"};
  desc.add_options()("help,h", "Help screen")(
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
  std::cout << "Loading symbol dictionary from file..." << std::endl;
  auto sd = h2sl::SymbolDictionary();
  if( sd.from_file( vm["sd"].as<std::string>().c_str() ) ){
    std::cout << "Imported symbol dictionary " << sd << std::endl << std::endl;
  } else {
    std::cout << "Could not load symbol dictionary, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }

  // Load lexicon from the file
  std::cout << "Loading lexicon from file..." << std::endl;
  h2sl::Lexicon lx = h2sl::Lexicon();
  if( lx.from_file( vm["lexicon"].as<std::string>().c_str() ) ){
    std::cout << "Imported lexicon " << lx << std::endl << std::endl;
  } else {
    std::cout << "Could not load lexicon, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }

  // Generate CV related features
  std::cout << "Generating CV features..." << std::endl;
  feature_pool.constituent_feature_sets.push_back( std::vector< std::shared_ptr< h2sl::Feature > >() );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureCV >( "false" ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureCV >( "true" ) );

  // Generate language-related features using the Lexicon
  std::cout << "Generating language features..." << std::endl;
  feature_pool.constituent_feature_sets.push_back( std::vector< std::shared_ptr< h2sl::Feature > >() );
  auto speakers = std::unordered_set< std::string >(); // Track all speakers over all LVs
  for( const auto& info : lx.type_information ){
    // Create a feature for this type of LanguageVariable
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureLVType >( info.first ) );

/* These LV features shouldn't really provide much information and only allow overfitting
    // Create features for edge labels
    for( const auto& label : info.second.edge_labels ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureLVEdgeLabel >( info.first, label ) );
    }

    // Create features for words
    for( const auto& word : info.second.words ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureWord >( info.first, word ) );
    }
*/

    // Find any speakers from this LV type
/* We don't need to find all speakers when there's only one in the dataset
    auto speaker_map_it = info.second.roles.find( "speaker" );
    if( speaker_map_it != info.second.roles.end() ){
      for( const auto& speaker : speaker_map_it->second ){
        speakers.insert( speaker );
      }
    }
*/
  }

/* Skip speaker features
  if( speakers.size() > 0 ){
    std::cout << "Generating speaker-related features..." << std::endl;

    // Create features for all found speakers on LV "i" and "you" only
    for( const auto& info : lx.type_information ){
      if( info.first != "i" && info.first != "you" ) continue;
      for( const std::string& speaker : speakers ){
        feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureLVRoleValue >( info.first, "speaker", speaker ) );
      }
    }

    // Add a feature matching an object uid with a LV's speaker
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesLVRole >( "object", "uid", "speaker" ) );
  }
*/

  // Load Symbol related features using the Symbol Dictionary
  std::cout << "Generating symbol-related features..." << std::endl;
  feature_pool.constituent_feature_sets.push_back( std::vector< std::shared_ptr< h2sl::Feature > >() );

  // Features agnostic to the symbol type
  std::cout << "Generating symbol type features..." << std::endl;
  for(const auto& [symbol_type, info] : sd.dictionary ){
    // List the features that need to be templated for all of the symbol types in the symbol dictionary
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolType >( symbol_type ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolMatchesChild >( symbol_type ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolHasChild >( symbol_type ) );
  }

  // Features specific to the symbol type

  // Object related features
  std::cout << "Generating object-related features..." << std::endl;
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolType >( "object" ) );
  feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolMatchesChild >( "object" ) );
  auto it_object_type = sd.dictionary.find( "object_type" );
  if( it_object_type != sd.dictionary.end() ){
    auto it_prop = it_object_type->second->properties.find("object_type");
    for(const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "object", it_prop->first, property_value ) );
    }
  }

  // Create object_type symbol features for each known object type
  std::cout << "Generating object_type features..." << std::endl;
  it_object_type = sd.dictionary.find( "object_type" );
  if( it_object_type != sd.dictionary.end() ){
    auto it_prop = it_object_type->second->properties.find("object_type");
    for( const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_object_type->first, it_prop->first, property_value ) );
    }
  }

/*
  // Create object_quantifier features for each known quantifier
  auto it_object_quantifier = sd.dictionary.find( "object_quantifier" );
  if( it_object_quantifier != sd.dictionary.end() ){
    std::cout << "Generating object quantifier features..." << std::endl;

    auto it_prop = it_object_quantifier->second->properties.find( "quantifier" );
    for(const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "object_quantifier", it_prop->first, property_value ) );
    }

    // Add a feature to enable detection of "all" modifier on any LV
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolChildAttributeValue >( "object", "object_quantifier", "quantifier", "all" ) );
  }

  // Create object_color symbol and object property features for each known object color
  auto it_object_color = sd.dictionary.find( "object_color" );
  if( it_object_color != sd.dictionary.end() ){
    std::cout << "Generating object_color features..." << std::endl;

    auto it_prop = it_object_color->second->properties.find("object_color");
    for( const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_object_color->first, it_prop->first, property_value ) );
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "object", it_prop->first, property_value ) );
    }

    // Allow matching object's color with a child object_color symbol
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValueLabel >( "object", "object_color", "object_color", "object_color", "color" ) );
  }

  // Semantic property related features
  auto it_semantic_property = sd.dictionary.find( "object_semantic_property" );
  if( it_semantic_property != sd.dictionary.end() ){
    std::cout << "Generating semantic property features..." << std::endl;

    auto it_prop = it_semantic_property->second->properties.find("property");
    for( const auto& property_value : it_prop->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_semantic_property->first, it_prop->first, property_value ) );
    }
  }
*/

  // Action related features
  auto it_action = sd.dictionary.find( "action" );
  if( it_action != sd.dictionary.end() ){
    std::cout << "Generating action features..." << std::endl;

    auto it_action_type = it_action->second->properties.find("action_type");
    for( const auto& type : it_action_type->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_action->first, it_action_type->first, type ) );
    }

    auto it_agent = it_action->second->properties.find("agent");
    for( const auto& agent : it_agent->second ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_action->first, it_agent->first, agent ) );
    }

    // Enable capturing of payload from grounded child object symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "action", "payload", "object", "uid" ) );
    // Enable capturing of landmark from grounded child spatial_relation symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "action", "landmark", "spatial_relation", "landmark" ) );
    // Enable capturing of viewpoint from grounded child spatial_relation symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "action", "viewpoint", "spatial_relation", "viewpoint" ) );
    // Enable capturing of axis from grounded child spatial_relation symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "action", "axis", "spatial_relation", "axis" ) );
    // Enable capturing of quantity from grounded child distance symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "action", "quantity", "distance", "quantity" ) );
    // Enable capturing of unit from grounded child distance symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "action", "unit", "distance", "unit" ) );

    // Create edge-labeled payload selectors
    for( const auto& info : lx.type_information ){
      for( const auto& label : info.second.edge_labels ){
        feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValueLabel >( "action", "payload", "object", "uid", label ) );
      }
    }
  }

  // Distance related features
  auto it_distance = sd.dictionary.find( "distance" );
  if( it_distance != sd.dictionary.end() ){
    std::cout << "Generating distance features..." << std::endl;

    // Add indicators for all known units
    auto it_unit = it_distance->second->properties.find("unit");
    if( it_unit != it_distance->second->properties.end() ){
      std::cout << "Generating features checking for each unit..." << std::endl;
      for( const auto& u : it_unit->second ){
        feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "distance", "unit", u ) );
      }
    }
/*
    // Add indicators for all known quantities
    auto it_quantity = it_distance->second->properties.find("quantity");
    if( it_quantity != it_distance->second->properties.end() ){
      std::cout << "Generating features checking for each quantity..." << std::endl;
      for( const auto& q : it_quantity->second ){
        feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "distance", "quantity", q ) );
      }
    }
*/
    auto it_distance_unit = sd.dictionary.find( "distance_unit" );
    if( it_distance_unit != sd.dictionary.end() ){
      std::cout << "Generating distance unit features..." << std::endl;

      // Add indicators for all known units
      auto it_unit = it_distance_unit->second->properties.find("unit");
      if( it_unit != it_distance_unit->second->properties.end() ){
        std::cout << "Generating features checking for each unit..." << std::endl;
        for( const auto& u : it_unit->second ){
          feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "distance_unit", "unit", u ) );
        }
      }
    }

    // Allow capturing of units from grounded child symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "distance", "unit", "distance_unit", "unit" ) );

    // Add a feature matching a quantity with a LV's quant role
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesLVRole >( "distance", "quantity", "quant" ) );
  } // End distance features

  // Spatial relation related features
  auto it_spatial_relation = sd.dictionary.find( "spatial_relation" );
  if( it_spatial_relation != sd.dictionary.end() ){
    std::cout << "Generating spatial relation features..." << std::endl;

    // Generate default features for binary spatial relations
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationChildrenSatisfied >( ) );
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationChildrenContradicted >( ) );
    it_object_type = sd.dictionary.find( "object_type" );
    if( it_object_type != sd.dictionary.end() ){
      auto it_prop = it_object_type->second->properties.find("object_type");
      for( const auto& property_value : it_prop->second ){
        feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationChildrenSatisfiedObjectType>( property_value ) );
      }
    }

    // Generate default features for ordinal spatial relations
    for( int position_index = 0; position_index < 3; position_index++ ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationPosition >( position_index ) );

      // Create object-type-specific position features as well
      it_object_type = sd.dictionary.find( "object_type" );
      if( it_object_type != sd.dictionary.end() ){
        auto it_prop = it_object_type->second->properties.find("object_type");
        for(const auto& property_value : it_prop->second ){
          feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSpatialRelationPositionObjectType >( position_index, property_value ) );
        }
      }
    }

    // Allow capturing of landmarks from grounded child symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "spatial_relation", "landmark", "object", "uid" ) );
    // Add the inverted feature to provide useful negative information
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "spatial_relation", "landmark", "object", "uid", true ) );

    // Create edge-labeled landmark selectors
    std::cout << "Generating edge-labeled landmark features..." << std::endl;
    auto spatial_relation_edge_labels = std::unordered_set<std::string>();
    for( const auto& info : lx.type_information ){
      if( info.second.symbol_types.find( "spatial_relation" ) == info.second.symbol_types.end() ){
        continue;
      }
      for( const auto& label : info.second.edge_labels ){
        spatial_relation_edge_labels.insert( label );
      }
    }

    // Add unique features for each
    for( const auto& label : spatial_relation_edge_labels ){
      feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValueLabel >( "spatial_relation", "landmark", "object", "uid", label ) );
    }

    // Add indicators for all known viewpoints
    auto it_viewpoint = it_spatial_relation->second->properties.find("viewpoint");
    if( it_viewpoint != it_spatial_relation->second->properties.end() ){
      std::cout << "Generating features checking for each viewpoint..." << std::endl;
      for( const auto& vp : it_viewpoint->second ){
        feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( "spatial_relation", "viewpoint", vp ) );
      }
    }

/*
    // Create a feature matching a spatial_relation viewpoint with a LV's speaker
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesLVRole >( "spatial_relation", "viewpoint", "speaker" ) );

    // Add a feature matching child viewpoint object connected via :poss label
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValueLabel >( "spatial_relation", "viewpoint", "object", "uid", "poss" ) );
*/

    // Add indicators for all known axes
    auto it_axis = it_spatial_relation->second->properties.find("axis");
    if( it_axis != it_spatial_relation->second->properties.end() ){
      std::cout << "Generating features checking for each axis..." << std::endl;
      for( const auto& axis : it_axis->second ){
        feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_spatial_relation->first, it_axis->first, axis ) );
      }
    }

    // Add indicators for all known spatial_relation_axis symbols
    auto it_axis_symbol = sd.dictionary.find( "spatial_relation_axis" );
    if( it_axis_symbol != sd.dictionary.end() ){
      auto it_axis_type = it_axis_symbol->second->properties.find("axis");
      if( it_axis_type != it_axis_symbol->second->properties.end() ){
        std::cout << "Generating features checking for each spatial_relation_axis..." << std::endl;
        for( const auto& axis_type : it_axis_type->second ){
          feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_axis_symbol->first, it_axis_type->first, axis_type ) );
        }
      }
    }

    // Allow capturing of axis attribute from grounded child axis symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "spatial_relation", "axis", "spatial_relation_axis", "axis" ) );

    // Add indicators for all known extrema attributes
    auto it_extrema = it_spatial_relation->second->properties.find("extrema");
    if( it_extrema != it_spatial_relation->second->properties.end() ){
      std::cout << "Generating features checking for each extrema..." << std::endl;
      for( const auto& extrema : it_extrema->second ){
        feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_spatial_relation->first, it_extrema->first, extrema ) );
      }
    }

    // Add indicators for all known spatial_relation_extrema symbols
    auto it_extrema_symbol = sd.dictionary.find( "spatial_relation_extrema" );
    if( it_extrema_symbol != sd.dictionary.end() ){
      auto it_extrema_type = it_extrema_symbol->second->properties.find("extrema");
      if( it_extrema_type != it_extrema_symbol->second->properties.end() ){
        std::cout << "Generating features checking for each spatial_relation_extrema..." << std::endl;
        for( const auto& extrema_type : it_extrema_type->second ){
          feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValue >( it_extrema_symbol->first, it_extrema_type->first, extrema_type ) );
        }
      }
    }

    // Allow capturing of extrema from grounded child symbols
    feature_pool.constituent_feature_sets.back().push_back( std::make_shared< h2sl::FeatureSymbolAttributeValueMatchesChildAttributeValue >( "spatial_relation", "extrema", "spatial_relation_extrema", "extrema" ) );
  } // End spatial relation features

  std::cout << "Success!" << std::endl;
  std::cout << "Loaded the feature_pool: " << feature_pool << std::endl;

  if (vm.count("output")) {
    std::cout << "writing feature_pool to \"" << vm["output"].as<std::string>() << "\"" << std::endl;
    feature_pool.to_file(vm["output"].as<std::string>());
  }

  std::cout << "End of the feature-pool-generator program." << std::endl;
  return EXIT_SUCCESS;
}
