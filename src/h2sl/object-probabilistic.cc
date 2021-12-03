/**
 * @file object-probabilistic.cc
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

#include "h2sl/common.h"
#include "h2sl/object-probabilistic.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

using namespace std;

namespace h2sl{

//
// ObjectProbabilistic class constructor
//
ObjectProbabilistic::ObjectProbabilistic( const string& uidArg,
                const ObjectState::vectorObjectState& stateHistoryArg,
                const mapPropertyDistributions& propertyDistributionsArg ) :

                uid(uidArg), state_history( stateHistoryArg ),
                property_distributions(propertyDistributionsArg) {
  if( !property_distributions_valid() ){
    std::cout << "ObjectProbabilistic constructed with invalid distributions!" << std::endl;
  }
}

//
// Class constructor for ObjectProbabilistic with static object state
//
ObjectProbabilistic::ObjectProbabilistic(const std::string& uidArg,
               const ObjectState& objectStateArg,
               const mapPropertyDistributions& propertyDistributionsArg ) :

               uid(uidArg), state_history(),
               property_distributions(propertyDistributionsArg) {

  state_history.push_back( objectStateArg );
  if( !property_distributions_valid() ){
    std::cout << "ObjectProbabilistic constructed with invalid distributions!" << std::endl;
  }
}

//
// ObjectProbabilistic XML constructor
//
ObjectProbabilistic::ObjectProbabilistic( const tinyxml2::XMLElement *root ){
  if( !from_xml( root ) ){
    std::cout << "XML constructor failed. Line: " << __LINE__ << " in " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }
}

//
// Checks whether the ObjectProbabilistic's semantic property distributions are valid.
//
bool ObjectProbabilistic::property_distributions_valid( void )const{
  // Check that each probability distribution is valid by summing the probabilities of its properties
  for( const auto& prop_dist : property_distributions ){
    double sum = 0.0;
    for( const auto& prop : prop_dist.second ){
      sum += prop.second;
    }
    if( fabs( sum - 1.0 ) > 0.0001 ){ // Check if doubles are 'close enough'
      std::cout << "Sum for property " << prop_dist.first << " was " << sum << std::endl;
      return false;
    }
  }
  return true;
}

//
// Imports the ObjectProbabilistic class from an XML file
//
bool ObjectProbabilistic::from_xml( const string& filename ){

  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename.c_str() );

  // Check for loading errors
  if( doc.Error() )
    return false;

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr )
    return false;

  // Check for any ObjectProbabilistic child element of the root
  const tinyxml2::XMLElement* object_elem = root->FirstChildElement("object_probabilistic");
  if( object_elem == nullptr )
    return false;

  return from_xml( object_elem );
}

//
// Imports the ObjectProbabilistic class from an tinyxml2::XMLElement pointer
//
bool ObjectProbabilistic::from_xml(const tinyxml2::XMLElement * object_elem) {

  uid = "";
  state_history.clear();
  property_distributions.clear();

  // Check that the element is a object element
  if( object_elem == nullptr )
    return false;

  if( std::strcmp(object_elem->Name(), "object_probabilistic") != 0 )
    return false;

  // Read the attributes
  const auto* uid_attribute = object_elem->FindAttribute("uid");

  // Signal false if uid attribute is missing
  if(uid_attribute == nullptr){
    std::cout << "failed to find uid" << std::endl;
    return false;
  }
  uid = uid_attribute->Value();

  // Loop over attributes of the xml_element to fill in deterministic semantic property distributions
  auto object_property_attribute = object_elem->FirstAttribute();
  while( object_property_attribute != nullptr ){
    if( strcmp(object_property_attribute->Name(), "uid") != 0 ){
      // Create a distribution with probability 1.0 that the property is true.
      auto new_dist = std::map<std::string, double>();
      new_dist.emplace( "true", 1.0 ); // We ignore the implicit false case
      property_distributions.emplace( object_property_attribute->Name(), new_dist );
    }
    object_property_attribute = object_property_attribute->Next();
  }

  // Read state_history
  // The state history should have one or more entries based on the task requirements.
  auto state_history_element = object_elem->FirstChildElement("state_history");
  if( state_history_element != nullptr ){

    auto object_state_element = state_history_element->FirstChildElement("object_state");
    while( object_state_element != nullptr ){

      h2sl::ObjectState object_state = h2sl::ObjectState( object_state_element );
      state_history.push_back( object_state );

      object_state_element = object_state_element->NextSiblingElement("object_state");
    }
  }

  // Read the semantic property distributions, which may or may not be included.
  auto distributions_elem = object_elem->FirstChildElement("semantic_property_distributions");
  if( distributions_elem != nullptr ){
    for( auto distribution_elem = distributions_elem->FirstChildElement();
         distribution_elem != nullptr;
	 distribution_elem = distribution_elem->NextSiblingElement() ){
      if( strcmp( distribution_elem->Name(), "semantic_property_distribution" ) == 0 ){
	const auto* key_attr = distribution_elem->FindAttribute("key");
	if( key_attr == nullptr ){
          continue; // Skip distribution if key is missing
	}

        // Push an empty map for the distribution into property_distributions
	auto new_distribution_map = std::map< std::string, double >();
        auto property_distribution_ret = property_distributions.insert({ key_attr->Value(), new_distribution_map });
	if( !property_distribution_ret.second ){
	  std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
	  continue; // Skip to next distribution
	}

	// Separate each distribution into its property options and probabilities
        for( auto property_elem = distribution_elem->FirstChildElement();
	     property_elem != nullptr;
	     property_elem = property_elem->NextSiblingElement() ){
	  // Insert each property value into the distribution
	  if( strcmp( property_elem->Name(), "semantic_property" ) == 0 ){
	    // Find property's key and value attributes
	    const auto* property_key_attr = property_elem->FindAttribute("key");
	    const auto* property_value_attr = property_elem->FindAttribute("value");
            if( property_key_attr == nullptr || property_value_attr == nullptr ){
              continue; // Skip to next property
	    }

	    // Insert into distribution
            auto property_ret = property_distributions.at( key_attr->Value() ).insert({
	        property_key_attr->Value(), property_value_attr->DoubleValue() });
	    if( !property_ret.second ){
	      std::cout << "failed on line " << __LINE__ << " in " << __FILE__ << std::endl;
            }
	  } // semantic_property if
	} // property for loop
      } // semantic_property_distribution if
    } // distribution for loop
  } // semantic_property_distributions if

  // Check validity of imported property distributions
  if( !property_distributions_valid() ){
    std::cout << "Imported probability distribution did not sum to 1.0" << std::endl;
    return false;
  }

  return true;
}

//
// Method to load from a json formatted string
//
bool ObjectProbabilistic::from_json( const std::string& json_string ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_string, root );
  return from_json( root );
}

//
// Method to load from a json value
//
bool ObjectProbabilistic::from_json( const Json::Value& root ){

  if(root["class"].asString() != "object_probabilistic")
    return false;

  uid = "";
  state_history.clear();
  property_distributions.clear();

  if( root.isMember("uid") ) {
    uid = root["uid"].asString();
  }

  // Read state_history, which may have one or more entries based on the task requirements.
  if(root.isMember("state_history")){
    Json::Value state_history_array = root["state_history"];
    for( const auto& state_history_element : state_history_array ){
      h2sl::ObjectState object_state = h2sl::ObjectState();
      object_state.from_json( state_history_element );
      state_history.push_back(object_state);
    }
  }

  // TODO - Code for property distribution not yet added

  return true;
}

//
// Exports the ObjectProbabilistic class to an XML file
//
bool ObjectProbabilistic::to_xml(const string& filename,
                    const int object_states_count) const {

  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertEndChild( root );
  to_xml( doc, root, object_states_count );

  // Save the file and store the error status
  tinyxml2::XMLError error_result = doc.SaveFile( filename.c_str() );
  if( error_result != tinyxml2::XML_SUCCESS )
    return false;
  return true;
}

//
// Exports the ObjectProbabilistic class to an XML node pointer
//
void ObjectProbabilistic::to_xml(tinyxml2::XMLDocument& document,
                    tinyxml2::XMLElement *root,
                    const int object_states_count) const {

  auto object_elem = document.NewElement("object_probabilistic");

  // Write uid
  object_elem->SetAttribute("uid", uid.c_str());

  // Write the state_history
  auto state_history_elem = document.NewElement("state_history");
  // Write ObjectState from state_history as a child to the state_history node
  // object_states_count indicates how many latest object states to be exported
  int counter = 0;
  for( auto object_state = state_history.rbegin(); object_state != state_history.rend(); ++object_state){
    if(counter >= object_states_count)
      break;
    object_state->to_xml( document, state_history_elem );
    counter++;
  }
  object_elem->InsertEndChild(state_history_elem);

  // Write semantic property distributions
  if( property_distributions.size() != 0 ){
    auto distributions_elem = document.NewElement("semantic_property_distributions");
    for( const auto& prop_dist : property_distributions ){
      auto distribution_elem = document.NewElement("semantic_property_distribution");
      distribution_elem->SetAttribute("key", prop_dist.first.c_str());
      for( const auto& prop : prop_dist.second ){
        auto prop_elem = document.NewElement("semantic_property");
	prop_elem->SetAttribute("key", prop.first.c_str());
	prop_elem->SetAttribute("value", to_std_string(prop.second).c_str());
        distribution_elem->InsertEndChild( prop_elem );
      }
      distributions_elem->InsertEndChild( distribution_elem );
    }
    object_elem->InsertEndChild( distributions_elem );
  }

  root->InsertEndChild( object_elem );
  return;
}

//
// Method to write to a json formatted string
//
std::string ObjectProbabilistic::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

//
// Method to write to a json value
//
void ObjectProbabilistic::to_json( Json::Value& root )const{

  root["class"] = "object_probabilistic";
  root["uid"] = uid;

  // Write the state_history
  root["state_history"] = Json::Value( Json::arrayValue );
  for( const auto& object_state : state_history ){
    Json::Value state_history_root;
    object_state.to_json( state_history_root );
    root["state_history"].append( state_history_root );
  }

  // TODO - Handle semantic property distributions

  return;
}

//
// ObjectProbabilistic class ostream operator
//
ostream& operator<<(ostream& out, const ObjectProbabilistic& other) {
  out << "ObjectProbabilistic(uid=\"" << other.uid << "\",";

  out << "state_history{";
  for( auto it_object_state = other.state_history.begin();
       it_object_state != other.state_history.end(); it_object_state++ ){
    out << *it_object_state;
    if( next(it_object_state) != other.state_history.end() ){
      out << ",";
    }
  }
  out << "}";

  if( !other.property_distributions.empty() ){
    out << ",property_distributions{";
    for (auto it_prop_dist = other.property_distributions.begin();
        it_prop_dist != other.property_distributions.end(); it_prop_dist++) {
      out << it_prop_dist->first << "(";
      for (auto it_prop = it_prop_dist->second.begin();
          it_prop != it_prop_dist->second.end(); it_prop++) {
        out << "\"" << it_prop->first << "\"=" << it_prop->second;
        if (next(it_prop) != it_prop_dist->second.end()) {
          out << ",";
        }
      }
      out << ")";
      if (next(it_prop_dist) != other.property_distributions.end()) {
        out << ", ";
      }
    }
    out << "}";
  }
  out << ")";
  return out;
}

} // namespace h2sl
