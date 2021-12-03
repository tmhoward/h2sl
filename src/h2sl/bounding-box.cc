/**
 * @file bounding-box.cc
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

#include "h2sl/common.h"
#include "h2sl/bounding-box.h"

using namespace std;

namespace h2sl{

//
// BoundingBox class default constructor
//
BoundingBox::BoundingBox( const h2sl::Vector3D& dimensionsArg,
                          const h2sl::Transform3D& offsetArg ) : dimensions( dimensionsArg ),
                                                                offset( offsetArg ){}

//
// BoundingBox XML constructor
//
BoundingBox::BoundingBox( const tinyxml2::XMLElement *root ){
  if( not from_xml( root ) ){
    std::cout << "Class XML constructor failed. Line:  " << __LINE__ << " File:  " << __FILE__ << std::endl;
    exit(EXIT_FAILURE);
  }
}

//
// Class constructor for BoundingBox from BoundingBoxMessage message.
//
BoundingBox::BoundingBox( const BoundingBoxMessage& msg ) : dimensions( msg.dimensions ), offset( msg.offset ){

}

//
// Imports the BoundingBox class from an XML file
//
bool BoundingBox::from_xml(const string& filename) {

  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename.c_str() );

  // Check for loading errors
  if( doc.Error() )
    return false;

  // Extract the root element from the document object
  const auto* root = doc.RootElement();
  if( root == nullptr )
    return false;

  // Check for any bbox child element of the root
  const tinyxml2::XMLElement* bbox_element = root->FirstChildElement("bbox");
  if( bbox_element == nullptr )
    return false;

  return from_xml( bbox_element );

}

//
// Imports the BoundingBox class from an tinyxml2::XMLElement pointer
//
bool BoundingBox::from_xml(const tinyxml2::XMLElement *bbox_element) {
  if (bbox_element == nullptr)
    return false;

  dimensions = h2sl::Vector3D(0.0, 0.0, 0.0);
  offset = h2sl::Transform3D();

  // Read the attributes
  const auto* bdim_attr = bbox_element->FindAttribute("bdim");
  const auto* bxyz_attr = bbox_element->FindAttribute("bxyz");
  const auto* bq_attr = bbox_element->FindAttribute("bq");

  // Signal false if dimension attributes is missing
  if(bdim_attr == nullptr){
    std::cout << "failed to find box dimension" << std::endl;
    return false;
  }

  // Signal false if offset position attributes is missing
  if(bxyz_attr == nullptr){
    std::cout << "failed to find box offset position" << std::endl;
    return false;
  }

  // Signal false if offset orientation attributes is missing
  if(bq_attr == nullptr){
    std::cout << "failed to find box offset orientation" << std::endl;
    return false;
  }

  dimensions.from_cs_string( bdim_attr->Value() );
  offset.position.from_cs_string( bxyz_attr->Value() );
  offset.orientation.from_cs_string( bq_attr->Value() );

  return true;
}

//
// Exports the BoundingBox class to an XML file
//
bool BoundingBox::to_xml(const string& filename) const {

  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertEndChild( root );
  to_xml( doc, root );

  // Save the file and store the error status
  tinyxml2::XMLError error_result = doc.SaveFile( filename.c_str() );
  if( error_result != tinyxml2::XML_SUCCESS )
    return false;

  return true;

}

//
// Exports the BoundingBox class to an XML node pointer
//
void BoundingBox::to_xml(tinyxml2::XMLDocument& document,
                         tinyxml2::XMLElement *root) const {

  tinyxml2::XMLElement* bbox_element = document.NewElement("bbox");

  bbox_element->SetAttribute("bdim", dimensions.to_csv_string().c_str() );
  bbox_element->SetAttribute("bxyz", offset.position.to_csv_string().c_str() );
  bbox_element->SetAttribute("bq", offset.orientation.to_csv_string().c_str() );

  // Add the constructed element to the root
  root->InsertEndChild(bbox_element);

  return;
}

//
// This method exports a BoundingBox to a BoundingBoxMessage.
//
BoundingBoxMessage BoundingBox::to_msg( void )const{
  BoundingBoxMessage msg;
  msg.dimensions = dimensions.to_msg();
  msg.offset = offset.to_msg();
  return msg;
}

//
// BoundingBox class ostream operator
//
ostream& operator<<(ostream& out, const BoundingBox& other) {

  // out << "(BoundingBox:";

  out << "dimX=" << other.dimensions.x() << ",";
  out << " dimY=" << other.dimensions.y() << ",";
  out << " dimZ=" << other.dimensions.z() << ",";
  out << " offset=" << other.offset;

  // out << ")";
  return out;
}

} // namespace h2sl
