/**
 * @file feature-spatial-relation-merge-object-spatial-relation.cc
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

#include <fstream>
#include <sstream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <boost/algorithm/string.hpp>

#include "h2sl/feature-spatial-relation-merge-object-spatial-relation.h"

namespace h2sl {

///
/// FeatureSpatialRelationMergeObjectSpatialRelation default class constructor
///
FeatureSpatialRelationMergeObjectSpatialRelation::FeatureSpatialRelationMergeObjectSpatialRelation( const bool invertArg )
    : Feature( FeatureType::DynamicSymbol, FeatureValue::Unknown, Feature::dependent_parameters_t( false, false, true, true, false) ),
      invert( invertArg ){}

///
/// FeatureSpatialRelationMergeObjectSpatialRelation class constructor from an XMLElement
///
FeatureSpatialRelationMergeObjectSpatialRelation::FeatureSpatialRelationMergeObjectSpatialRelation( const tinyxml2::XMLElement* root ) : Feature(), invert() {
  from_xml( root );
}

///
/// This method generates a copy of a feature
///
std::shared_ptr<Feature> FeatureSpatialRelationMergeObjectSpatialRelation::dup( void )const{
  return std::make_shared<FeatureSpatialRelationMergeObjectSpatialRelation>(*this);
}

///
/// Method to load from a tinyxml2 element
///
bool FeatureSpatialRelationMergeObjectSpatialRelation::from_xml( const tinyxml2::XMLElement* root ){
  type = FeatureType::DynamicSymbol;
  value = FeatureValue::Unknown;
  depends_on = Feature::dependent_parameters_t( false, false, true, true, false );
  invert = false;

  // Check that the element is a feature-spatial-relation-merge-object-spatial-relation element
  if( root == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSpatialRelationMergeObjectSpatialRelation Class Error] XMLElement is nullptr in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to see if the class name is feature-spatial-relation-merge-object-spatial-relation
  const tinyxml2::XMLAttribute* class_attr = root->FindAttribute("class");
  if( class_attr == nullptr ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSpatialRelationMergeObjectSpatialRelation Class Error] could not find class attribute in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // check to make sure the class name is correct
  if( std::strcmp(class_attr->Value(), "feature-spatial-relation-merge-object-spatial-relation") != 0 ){
    try {
      std::stringstream error_msg;
      error_msg << "[FeatureSpatialRelationMergeObjectSpatialRelation Class Error] Erroneous class name (\"" << root->Name() << "\") in from_xml";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
    return false;
  }

  // Read the invert attribute and set it to invert
  const auto* invert_attr = root->FindAttribute("invert");
  if( invert_attr == nullptr ){
    throw std::runtime_error("[FeatureSymbolAttributeValue Class Error] Missing \"invert\" attribute in from_xml()");
  }
  invert = invert_attr->BoolValue();

  return true;
}

FeatureValue
FeatureSpatialRelationMergeObjectSpatialRelation::evaluate( const std::shared_ptr< std::string >& cv,
                                                        const std::shared_ptr<LanguageVariable>& lv,
                                                        const std::shared_ptr<WorldDCG>& world,
                                                        const std::shared_ptr<Symbol>& symbol )
{
  value = FeatureValue::False;

  // Enforce that the passed symbol is a spatial relation
  if( symbol->type != "spatial_relation" ) return value;

  // Enforce that there is a child object and child spatial relation
  // Further enforce that the symbols come from different language variables via break statements
  std::shared_ptr<h2sl::Symbol> child_object_symbol = nullptr;
  std::shared_ptr<h2sl::Symbol> child_spatial_relation_symbol = nullptr;
  for( const auto& connection : lv->children() ){
    for( const auto& child_symbol : connection.child->symbols() ){
      if( child_symbol->type == "object" ){
        child_object_symbol = child_symbol;
        break;
      }
      if( child_symbol->type == "spatial_relation" ){
        child_spatial_relation_symbol = child_symbol;
        break;
      }
    }
  }
  if( (child_object_symbol == nullptr) || (child_spatial_relation_symbol == nullptr) ) return value;
  // Enforce that the feature's spatial_relation's axis matches the child spatial_relation's axis
  auto it_symbol_axis = symbol->properties.find("axis");
  if( it_symbol_axis == symbol->properties.end() ) return value;
  auto it_child_spatial_relation_symbol_axis = child_spatial_relation_symbol->properties.find("axis");
  if( it_child_spatial_relation_symbol_axis == child_spatial_relation_symbol->properties.end() ) return value;
  if( it_symbol_axis->second != it_child_spatial_relation_symbol_axis->second ){
    value = (!invert) ? FeatureValue::False : FeatureValue::True;
    return value;
  }
  // Enforce that the feature's spatial_relation's viewpoint matches the child spatial_relation's viewpoint
  auto it_symbol_viewpoint = symbol->properties.find("viewpoint");
  if( it_symbol_viewpoint == symbol->properties.end() ) return value;
  auto it_child_spatial_relation_symbol_viewpoint = child_spatial_relation_symbol->properties.find("viewpoint");
  if( it_child_spatial_relation_symbol_viewpoint == child_spatial_relation_symbol->properties.end() ) return value;
  if( it_symbol_viewpoint->second != it_child_spatial_relation_symbol_viewpoint->second ){
    value = (!invert) ? FeatureValue::False : FeatureValue::True;
    return value;
  }
  // Enforce that the feature's spatial_relation's landmark matches the uid of the child object
  auto it_child_object_symbol_uid = child_object_symbol->properties.find("uid");
  if( it_child_object_symbol_uid == child_object_symbol->properties.end() ) return value;
  auto it_symbol_landmark = symbol->properties.find("landmark");
  if( it_symbol_landmark == symbol->properties.end() ) return value;
  if( it_symbol_landmark->second != it_child_object_symbol_uid->second ){
    value = (!invert) ? FeatureValue::False : FeatureValue::True;
    return value;
  }

  value = (!invert) ? FeatureValue::True : FeatureValue::False;
  return value;
}

///
/// This method prints out the values of a feature
///
std::string FeatureSpatialRelationMergeObjectSpatialRelation::print_string( const bool& printValue )const{
  std::stringstream out;
  if( printValue ){
    out << "feature-spatial-relation-merge-object-spatial-relation(type=\"" << type << "\",value=\"" << "\",invert= " << invert << "\")";
  } else {
    out << "feature-spatial-relation-merge-object-spatial-relation(type=\"" << type << "\"," << "\",invert= " << invert << "\")";
  }
  return out.str();
}

///
/// Method to generate a unique key for the feature
///
std::string FeatureSpatialRelationMergeObjectSpatialRelation::key( void ) const{
  std::stringstream out;
  out << "feature-spatial-relation-merge-object-spatial-relation(type=\"" << type << "\"," << "\",invert= " << invert << "\")";
  return out.str();
}

///
/// Method to load from a json formatted string
///
bool FeatureSpatialRelationMergeObjectSpatialRelation::from_json_string( const std::string& valueArg ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( valueArg, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FeatureSpatialRelationMergeObjectSpatialRelation::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a tinyxml2 document
///
void FeatureSpatialRelationMergeObjectSpatialRelation::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* feature_spatial_relation_merge_object_spatial_relation_elem = document.NewElement( "feature" );
  feature_spatial_relation_merge_object_spatial_relation_elem->SetAttribute( "class", "feature-spatial-relation-merge-object-spatial-relation" );
  feature_spatial_relation_merge_object_spatial_relation_elem->SetAttribute( "invert", invert );
  root->InsertEndChild( feature_spatial_relation_merge_object_spatial_relation_elem );
}

///
/// Method to write to a json formatted string
///
std::string FeatureSpatialRelationMergeObjectSpatialRelation::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FeatureSpatialRelationMergeObjectSpatialRelation::to_json( Json::Value& root )const{
  return;
}

///
/// FeatureSpatialRelationMergeObjectSpatialRelation class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FeatureSpatialRelationMergeObjectSpatialRelation& other) {
  other.print( out );
  return out;
}

} // namespace h2sl
