/**
 * @file test_combine_model_results.cc
 *
 * @brief
 *
 * Program used to combine the results of cross-validation tests for different models that are in separate xml files
 */

/*
  Main algorithm to combine the model result information:
    1. Loop through each test ( "test" xml node ) in the source file. For each,
      2. Grab the "filename" property
      3. Find the corresponding XML node in the target file by comparing this filename prop
      4. Copy the relevant properties from the source node to the target node (e.g. 'test_set_correct_root_{source model name}')
      5. Loop through each partition for the current test ("test_set" and "training_set")
        6. Find the corresponding XML node in the target file by comparing the node name
        7. copy the revelant properties from the source node to the target node (e.g. "correct_root_{ source model name}) 
        8. Loop through each partition's examples ("example")
          9. Grab the "filename" property
         10. Find the corresponding XML node in the target file
         11. Copy the "runtime_object_ration_{source model name}" property to the source node
         12. Loop through the example's children to find the XML node with name {source model name}
         13. Add the node as a child to the target's "example" node
*/

#include "test_combine_model_results_cmdline.h"
#include <boost/filesystem/path.hpp>

//#include <string.h>
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

/*
 * Function to return the corresponding "test" node for a provided filename
 */
xmlNodePtr
get_matching_test_node( const xmlNodePtr results_node, string filename ){
  // Loop through each child of results_node
  for( xmlNodePtr l1 = results_node->children; l1; l1 = l1->next ){
    if( l1->type == XML_ELEMENT_NODE ){
      if( xmlStrcmp( l1->name, ( const xmlChar* )( "test" ) ) == 0 ){
        // grab the "filename" property
        pair< bool, string > filename_prop = has_prop< string >( l1, "filename" );
        if( filename_prop.first ){
          if( filename == filename_prop.second ){
            return l1;
          }
        }
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
 * Function to return the corresponding example node for a given filename
 */
xmlNodePtr
get_matching_example_node( const xmlNodePtr partition_node, string filename ){
  // Loop through each child of partition_node
  for( xmlNodePtr l1 = partition_node->children; l1; l1 = l1->next ){
    if( l1->type == XML_ELEMENT_NODE ){
      if( xmlStrcmp( l1->name, ( const xmlChar* )( "example" ) ) == 0 ){
        // grab the fileame property
        pair< bool, string > filename_prop = has_prop< string >( l1, "filename" );
        if( filename_prop.first ){
          if( filename == filename_prop.second ){
            return l1;
          }
        }
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
 * Combine Test Model Results Program
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of test_combine_model_results program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }

  // print the provided source model name 
  cout << "source model name: " << args.source_model_arg << endl;

  // XML document and "results" node pointers for the source results file
  xmlDocPtr src_doc = NULL;
  xmlNodePtr src_root = NULL;
  xmlNodePtr src_results_node = NULL;
  src_doc = xmlReadFile( args.source_arg, NULL, 0 );
  if( src_doc != NULL ){
    src_root = xmlDocGetRootElement( src_doc );
    if( src_root->type == XML_ELEMENT_NODE ){
      src_results_node = get_results_node( src_root );
    }
  }
  // check the results node - this catches error with any of the parsing steps
  if( src_results_node == NULL ){
    cout << "Error when trying to find the source file's \"results\" node" << endl;
    assert( false );
  }

  // XML document and "results" node pointers for the source results file
  xmlDocPtr trgt_doc = NULL;
  xmlNodePtr trgt_root = NULL; 
  xmlNodePtr trgt_results_node = NULL;
  trgt_doc = xmlReadFile( args.target_arg, NULL, 0 );
  if( trgt_doc != NULL ){
    trgt_root = xmlDocGetRootElement( trgt_doc );
    if( trgt_root->type == XML_ELEMENT_NODE ){
      trgt_results_node = get_results_node( trgt_root );
    }
  }
  // check the results node - this catches error with any of the parsing steps
  if( trgt_results_node == NULL ){
    cout << "Error when trying to find the target file's \"results\" node" << endl;
    assert( false );
  }   

  // Loop through each source "test" node, which are children of the src_results_node
  for( xmlNodePtr src_test_node = src_results_node->children; src_test_node; src_test_node = src_test_node->next ){
    if( src_test_node->type == XML_ELEMENT_NODE ){
      if( xmlStrcmp( src_test_node->name, ( const xmlChar* )( "test" ) ) == 0 ){
        // Grab the "filename" property
        pair< bool, string > src_test_node_filename_prop = has_prop< string >( src_test_node, "filename" );
        // Find the corrsponding node in the target file
        if( src_test_node_filename_prop.first ){
          xmlNodePtr trgt_test_node = get_matching_test_node( trgt_results_node, src_test_node_filename_prop.second );
          if( trgt_test_node != NULL ){
            // At Step 4 - copy the test node properties
            copy_xml_node_properties( src_test_node, trgt_test_node );
            // At step 5 - loop through each partition
            for( xmlNodePtr src_partition_node = src_test_node->children; src_partition_node; src_partition_node = src_partition_node->next ){
              if( src_partition_node->type == XML_ELEMENT_NODE ){
                if( ( xmlStrcmp( src_partition_node->name, ( const xmlChar* )( "training_set" ) ) == 0 ) ||
                ( xmlStrcmp( src_partition_node->name, ( const xmlChar* )( "test_set" ) ) == 0 ) ){
                  // Find the corresponding node in the target file
                  xmlNodePtr trgt_partition_node = get_matching_partition_node( trgt_test_node, src_partition_node->name );
                  if( trgt_partition_node != NULL ){
                    // At step 7 - copy the partition node properties 
                    copy_xml_node_properties( src_partition_node, trgt_partition_node );
                    // At step 8 - loop through each partition's example nodes
                    for( xmlNodePtr src_example_node = src_partition_node->children; src_example_node; src_example_node = src_example_node->next ){
                      if( src_example_node->type == XML_ELEMENT_NODE ){
                        if( xmlStrcmp( src_example_node->name, ( const xmlChar* )( "example" ) ) == 0 ){
                          // Grab the "filename" property
                          pair< bool, string > src_example_node_filename_prop = has_prop< string >( src_example_node, "filename" );
                          // Find the corresponding node in the target file
                          if( src_example_node_filename_prop.first ){
                            xmlNodePtr trgt_example_node = get_matching_example_node( trgt_partition_node, src_example_node_filename_prop.second ); 
                            if( trgt_example_node != NULL ){
                              // At step 11
                              copy_xml_node_properties( src_example_node, trgt_example_node );
                              // At step 12
                              for( xmlNodePtr src_model_node = src_example_node->children; src_model_node; src_model_node = src_model_node->next ){
                                if( src_model_node->type == XML_ELEMENT_NODE ){
                                  if( xmlStrcmp( src_model_node->name, ( const xmlChar* )( args.source_model_arg ) ) == 0 ){
                                    // At step 13
                                    xmlAddChild( trgt_example_node, src_model_node ); 
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
        }
      }
    }
  }
  //save the new combined results file to the provided filepath & filename
  cout << "saving to file " << args.output_arg << endl;
  xmlSaveFormatFileEnc( args.output_arg, trgt_doc, "UTF-8", 1 );
 
  cout << "end of test_combine_model_results program" << endl;
  return 0;
} 
