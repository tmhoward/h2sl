/**
 * @file test_combine_any_results.cc
 *
 * @brief
 *
 * Program used to combine the results of cross-validation tests for any circumstance of separated files
 */

#include "test_combine_any_results_cmdline.h"
#include <boost/filesystem/path.hpp>

#include <string.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <libxml/tree.h>

#include "h2sl/common.h"

using namespace std;
using namespace h2sl;


/*
 * Function to return the "results" node for a provided root node
 * Note that the argument is expected to be a root of a results file
 */
xmlNodePtr
get_results_node( const xmlNodePtr root_node ){
  // loop through each child and look for a node with name "results"
  for( xmlNodePtr l1 = root_node->children; l1; l1 = l1->next ){
    if( l1->type == XML_ELEMENT_NODE ){
      if( xmlStrcmp( l1->name, ( const xmlChar* )( "results" ) ) == 0 ){
        return l1;
      }
    }
  }
  return NULL;
}

/**
 * Function to return the corresponding partition node (test_set vs training_set) for a given source partition node
 */
xmlNodePtr
get_matching_partition_node( const xmlNodePtr test_node, const xmlChar* partition_name ){
  //loop through each child of test_node
  for( xmlNodePtr l1 = test_node->children; l1; l1 = l1->next ){
    if( l1->type == XML_ELEMENT_NODE ){
      if( xmlStrcmp( l1->name, partition_name ) == 0 ){
        return l1;
      }
    }
  }
  return NULL;
}

/**
 * Function to copy over new properties from one xml node to another
 * If a property already exists, do not copy it
 */
void
copy_xml_node_properties( const xmlNodePtr source_node, xmlNodePtr& target_node ){
  for( xmlAttrPtr source_prop = source_node->properties; source_prop != NULL; source_prop = source_prop->next ){
    // look for a matching property in the target
    bool found_prop = false;
    for( xmlAttrPtr target_prop = target_node->properties; target_prop != NULL; target_prop = target_prop->next ){
      if( xmlStrcmp( target_prop->name, source_prop->name ) == 0 ){
        found_prop = true;
      }
    }
    // if no matching propety, add the property
    if( !found_prop ){
      xmlNewProp( target_node, source_prop->name, xmlGetProp( source_node, source_prop->name ) ); 
    }
  }
  return;
}

/**
 * Structure to hold information pertaining to a given example node
 */
struct example_node_data_t{
  xmlNodePtr node_ptr;
  map< string, xmlNodePtr > children;
};

/**
 * Structure to hold information pertaining to a given data partition node
 */
struct partition_node_data_t{
  xmlNodePtr node_ptr;
  map< string, example_node_data_t > examples;
};

/**
 * Structure to hold information pertaining to a given test node
 */
struct test_node_data_t{
  xmlNodePtr node_ptr;
  map< string, partition_node_data_t > partitions;
};

/**
 * Combine Test Any Results Program
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of test_combine_any_results program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }

  // new output file
  xmlDocPtr output_doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr output_root = xmlNewDocNode( output_doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( output_doc, output_root );

  // Initialize the data structure to hold the result information
  map< string, test_node_data_t > results;

  for( unsigned int i = 0; i < args.inputs_num; i++ ){
    // xml objects
    xmlDocPtr input_doc = NULL;
    xmlNodePtr input_root = NULL;
    xmlNodePtr results_node = NULL;
    // read the current input target
    input_doc = xmlReadFile( args.inputs[ i ], NULL, 0 );
    if( input_doc != NULL ){
      // get the root node element
      input_root = xmlDocGetRootElement( input_doc );
      if( input_root->type == XML_ELEMENT_NODE ){
        // get the "results" node
        results_node = get_results_node( input_root );
        // check the results node
        if( results_node == NULL ){
          cout << "Error when trying to find the input file's \"results\" node" << endl;
          assert( false );
        }
        // loop through each "test" node, which are children of the "results" node
        // determine the 'test_id' from the training_set size and the filenames of the training examples
        for( xmlNodePtr test_node = results_node->children; test_node; test_node = test_node->next ){
          if( test_node->type == XML_ELEMENT_NODE ){
            if( xmlStrcmp( test_node->name, ( const xmlChar* )( "test" ) ) == 0 ){
              string test_id;
              // grab the 'training_set_size' property and use it to initialize the test_id string 
              pair< bool, string > training_set_size_prop = has_prop< string >( test_node, "training_set_size" );
              if( training_set_size_prop.first ){
                test_id = training_set_size_prop.second;
              }
              // loop through each example in the training_set, scrape the index from the filename, and append to test_id
              // get the training_set partition node
              xmlNodePtr training_set_node = get_matching_partition_node( test_node, ( const xmlChar* )( "training_set" ) );
              // training_set_node will be NULL if it is not the training_set node
              if( training_set_node != NULL ){
                // loop through each "example" child node
                for( xmlNodePtr example_node = training_set_node->children; example_node; example_node = example_node->next ){
                  if( example_node->type == XML_ELEMENT_NODE ){
                    if( xmlStrcmp( example_node->name, ( const xmlChar* )( "example" ) ) == 0 ){
                      // get the filename property
                      pair< bool, string > filename_prop = has_prop< string >( example_node, "filename" );
                      // scrape the example index
                      if( filename_prop.first ){
                        string example_index = filename_prop.second.substr( ( filename_prop.second.size() - 8 ), 4 );
                        test_id.append( "," );
                        test_id.append( example_index );
                      }
                    }
                  }
                }
                // check if this test already exists in the "results" structure
                map< string, test_node_data_t >::iterator test_node_it = results.find( test_id );
                // if not, add it
                if( test_node_it == results.end() ){
                  results.insert( pair< string, test_node_data_t >( test_id, test_node_data_t() ) );
                  test_node_it = results.find( test_id );
                }
                // copy any new node information to the node_ptr member's attributes
                if( test_node_it->second.node_ptr == NULL ){ 
                  test_node_it->second.node_ptr = xmlNewDocNode( output_doc, NULL, ( xmlChar* )( "test" ), NULL );
                }
                copy_xml_node_properties( test_node, test_node_it->second.node_ptr );
                
                // get the data from the training_set partition && corresponding children
                map< string, partition_node_data_t >::iterator partition_it = test_node_it->second.partitions.find( "training_set" );
                if( partition_it == test_node_it->second.partitions.end() ){
                  test_node_it->second.partitions.insert( pair< string, partition_node_data_t >( "training_set", partition_node_data_t() ) );
                  partition_it = test_node_it->second.partitions.find( "training_set" );
                }
                // copy any new node information to the node_ptr member's attributes
                if( partition_it->second.node_ptr == NULL ){
                  partition_it->second.node_ptr = xmlNewDocNode( output_doc, NULL, ( xmlChar* )( "training_set" ), NULL );
                }
                copy_xml_node_properties( training_set_node, partition_it->second.node_ptr );

                // loop through each "example" child node
                for( xmlNodePtr example_node = training_set_node->children; example_node; example_node = example_node->next ){
                  if( example_node->type == XML_ELEMENT_NODE ){
                    if( xmlStrcmp( example_node->name, ( const xmlChar* )( "example" ) ) == 0 ){
                      // get the filename property
                      pair< bool, string > filename_prop = has_prop< string >( example_node, "filename" );
                      if( filename_prop.first ){
                        // look for the filename in the map of examples
                        map< string, example_node_data_t >::iterator example_it = partition_it->second.examples.find( filename_prop.second );
                        if( example_it == partition_it->second.examples.end() ){
                          partition_it->second.examples.insert( pair< string, example_node_data_t >( filename_prop.second, example_node_data_t() ) );
                          example_it = partition_it->second.examples.find( filename_prop.second );
                        }
                        // copy any new node information 
                        if( example_it->second.node_ptr == NULL ){
                          example_it->second.node_ptr = xmlNewDocNode( output_doc, NULL, ( xmlChar* )( "example" ), NULL );
                        }
                        copy_xml_node_properties( example_node, example_it->second.node_ptr );

                        // add any new child nodes
                        for( xmlNodePtr child_node = example_node->children; child_node; child_node = child_node->next ){
                          if( child_node->type == XML_ELEMENT_NODE ){
                            // look for an element with a key_value matching the xml node name
                            stringstream child_name;
                            child_name << child_node->name;
                            if( !example_it->second.children.count( child_name.str() ) ){
                              // handle the other children (the models)
                              xmlNodePtr tmp = xmlNewDocNode( output_doc, NULL, child_node->name, NULL );
                              copy_xml_node_properties( child_node, tmp );
                              // if world, get all of the children (object information)
                              if( child_name.str() == "world" ){
                                for( xmlNodePtr world_child_node = child_node->children; world_child_node; world_child_node = world_child_node->next ){
                                  if( world_child_node->type == XML_ELEMENT_NODE ){
                                    if( xmlStrcmp( world_child_node->name, ( const xmlChar* )( "object" ) ) == 0 ){
                                      xmlNodePtr tmp_world_child = xmlNewDocNode( output_doc, NULL, world_child_node->name, NULL );
                                      copy_xml_node_properties( world_child_node, tmp_world_child );
                                      xmlAddChild( tmp, tmp_world_child );
                                    }
                                  }
                                }
                              }
                              example_it->second.children.insert( pair< string, xmlNodePtr >( child_name.str(), tmp ) );
                            }
                          }
                        }
                      }
                    }
                  }
                } 
                // get the test_set partition node
                xmlNodePtr test_set_node = get_matching_partition_node( test_node, ( const xmlChar* )( "test_set" ) );
                if( test_set_node != NULL ){
                  // get the data from the training_set partition && corresponding children
                  map< string, partition_node_data_t >::iterator partition_it = test_node_it->second.partitions.find( "test_set" );
                  if( partition_it == test_node_it->second.partitions.end() ){
                    test_node_it->second.partitions.insert( pair< string, partition_node_data_t >( "test_set", partition_node_data_t() ) );
                    partition_it = test_node_it->second.partitions.find( "test_set" );
                  }
                  // copy any new node information to the node_ptr member's attributes
                  if( partition_it->second.node_ptr == NULL ){
                    partition_it->second.node_ptr = xmlNewDocNode( output_doc, NULL, ( xmlChar* )( "test_set" ), NULL );
                  }
                  copy_xml_node_properties( test_set_node, partition_it->second.node_ptr );
                  // loop through each "example" child node
                  for( xmlNodePtr example_node = test_set_node->children; example_node; example_node = example_node->next ){
                    if( example_node->type == XML_ELEMENT_NODE ){
                      if( xmlStrcmp( example_node->name, ( const xmlChar* )( "example" ) ) == 0 ){
                        // get the filename property
                        pair< bool, string > filename_prop = has_prop< string >( example_node, "filename" );
                        if( filename_prop.first ){
                          // look for the filename in the map of examples
                          map< string, example_node_data_t >::iterator example_it = partition_it->second.examples.find( filename_prop.second );
                          if( example_it == partition_it->second.examples.end() ){
                            partition_it->second.examples.insert( pair< string, example_node_data_t >( filename_prop.second, example_node_data_t() ) );
                            example_it = partition_it->second.examples.find( filename_prop.second );
                          }
                          // copy any new node information 
                          if( example_it->second.node_ptr == NULL ){
                            example_it->second.node_ptr = xmlNewDocNode( output_doc, NULL, ( xmlChar* )( "example" ), NULL );
                          }
                          copy_xml_node_properties( example_node, example_it->second.node_ptr );

                          // add any new child nodes
                          for( xmlNodePtr child_node = example_node->children; child_node; child_node = child_node->next ){
                            if( child_node->type == XML_ELEMENT_NODE ){
                              // look for an element with a key_value matching the xml node name
                              stringstream child_name;
                              child_name << child_node->name;
                              if( !example_it->second.children.count( child_name.str() ) ){
                                xmlNodePtr tmp = xmlNewDocNode( output_doc, NULL, child_node->name, NULL );
                                copy_xml_node_properties( child_node, tmp );
                                // if world, get all of the children (object information)
                                if( child_name.str() == "world" ){
                                  for( xmlNodePtr world_child_node = child_node->children; world_child_node; world_child_node = world_child_node->next ){
                                    if( world_child_node->type == XML_ELEMENT_NODE ){
                                      if( xmlStrcmp( world_child_node->name, ( const xmlChar* )( "object" ) ) == 0 ){
                                        xmlNodePtr tmp_world_child = xmlNewDocNode( output_doc, NULL, world_child_node->name, NULL );
                                        copy_xml_node_properties( world_child_node, tmp_world_child );
                                        xmlAddChild( tmp, tmp_world_child );
                                      }
                                    }
                                  }
                                }
                                example_it->second.children.insert( pair< string, xmlNodePtr >( child_name.str(), tmp ) );
                              }
                            }
                          }
                        }
                      }
                    }
                  } 
                }
              }
            }
          }
        } 
      }
      // memory management
      xmlFreeDoc( input_doc );
    }
  }


  xmlNodePtr output_results_node = xmlNewDocNode( output_doc, NULL, ( xmlChar* )( "results" ), NULL );
  xmlNewProp( output_results_node, ( const xmlChar* )( "num" ), ( const xmlChar* )( to_string( results.size() ).c_str() ) );
  xmlAddChild( output_root, output_results_node );

 
  for( map< string, test_node_data_t >::const_iterator test_node_it = results.begin(); test_node_it != results.end(); ++test_node_it ){
    xmlAddChild( output_results_node, test_node_it->second.node_ptr );
    for( map< string, partition_node_data_t >::const_iterator partition_node_it = test_node_it->second.partitions.begin(); partition_node_it != test_node_it->second.partitions.end(); ++partition_node_it ){
      xmlAddChild( test_node_it->second.node_ptr, partition_node_it->second.node_ptr );
      for( map< string, example_node_data_t >::const_iterator example_node_it = partition_node_it->second.examples.begin(); example_node_it != partition_node_it->second.examples.end(); ++example_node_it ){
        xmlAddChild( partition_node_it->second.node_ptr, example_node_it->second.node_ptr );
        for( map< string, xmlNodePtr >::const_iterator child_node_it = example_node_it->second.children.begin(); child_node_it != example_node_it->second.children.end(); ++child_node_it ){
          xmlAddChild( example_node_it->second.node_ptr, child_node_it->second );
        }
      }
    }
  } 
  
  //save the new combined results file to the provided filepath & filename
  cout << "saving to file " << args.output_arg << endl;
  xmlSaveFormatFileEnc( args.output_arg, output_doc, "UTF-8", 1 );
  xmlFreeDoc( output_doc );
  cout << "end of test_combine_any_results program" << endl;
  return 0;
} 
