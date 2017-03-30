/* 
 * Run tests for the HADCG model. 
 */
void
run_tests_hadcg( const std::vector< std::string >& filenames,
            LLM*& llm,
            xmlDocPtr& docPtr,
            xmlNodePtr& parentNode,
            const string& testGroup,
            const unsigned int cur_test_num,
            const string& solution_directory, 
            const string& symbol_dictionary_groundings_path, 
            const unsigned int beam_width,
            const bool debug ){

  /******* Variables to record statistics **************************/
  // Num objects. Phrases.
  double num_objects = 0.0;
  double num_phrases = 0.0;

  // HADCG
  unsigned int num_correct_root_hadcg = 0;
  unsigned int num_incorrect_root_hadcg = 0;
  unsigned int num_correct_complete_hadcg = 0;
  unsigned int num_incorrect_complete_hadcg = 0;
  double num_concrete_hadcg = 0.0;
  double num_abstract_avg_hadcg = 0.0;
  double num_abstract_max_hadcg = 0.0;

  // Runtime
  double runtime_hadcg = 0.0;
 
  // Search Space Sizes
  double dataset_concrete_hadcg = 0.0;
  double dataset_abstract_avg_hadcg = 0.0;
  double dataset_abstract_max_hadcg = 0.0;

  // Averages.
  double average_runtime_ratio = 0.0;

  /******* Iterate over the test group **************************/
  xmlNodePtr test_group_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( testGroup.c_str() ), NULL );

  for( unsigned int j = 0; j < filenames.size(); j++ ){
    cout << "running " << testGroup << " " << j << " (" << filenames[ j ] << ")" << endl;

    string filename = boost::filesystem::path( filenames[ j ] ).filename().generic_string();
    xmlNodePtr example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "example" ), NULL );
    xmlNewProp( example_node, ( const xmlChar* )( "filename" ), ( const xmlChar* )( filenames[ j ].c_str() ) );

    // World
    World * world = new World();
    world->from_xml( filenames[ j ] );

    // Phrase truth. Note no parsing.
    Phrase* truth_phrase = new Phrase();
    truth_phrase->from_xml( filenames[ j ], world );
    // Phrase input.
    Phrase * input_phrase = new Phrase();
    input_phrase->from_xml( filenames[ j ], world );
    clear( input_phrase );

    // Symbol Dictionary 
    Symbol_Dictionary * symbol_dictionary = new Symbol_Dictionary( symbol_dictionary_groundings_path );
 
    // Search Space and fill the space of groundings.
    Search_Space * search_space = new Search_Space();
    struct timeval start_time;
    gettimeofday( &start_time, NULL );
    search_space->fill_groundings( *symbol_dictionary, world );
    struct timeval end_time;
    gettimeofday( &end_time, NULL );
    cout << "finished fill_seach_space in " << diff_time( start_time, end_time ) << " seconds" << endl;

    // Grounding context
    Grounding * context = NULL;

    /******************* Running HADCG ********************************/
    //HADCG: leaf search
    HADCG * hadcg = new HADCG();
    cout << "  running hadcg" << endl;
    uint64_t hadcg_start_time = current_time();
    hadcg->leaf_search( input_phrase, *symbol_dictionary, search_space, world, 
                      context, llm, beam_width, ( bool )( debug ) );
    uint64_t hadcg_end_time = current_time();
    runtime_hadcg += microseconds_to_seconds( hadcg_end_time - hadcg_start_time );

    assert( dynamic_cast< const Phrase* >( hadcg->solutions().front().second ) != NULL );

    stringstream hadcg_solution_filename;
    hadcg_solution_filename << solution_directory << "/test_" << setw( 4 ) << setfill( '0' ) << cur_test_num << "_hadcg_solution_" << filename;
    hadcg->solutions().front().second->to_xml( hadcg_solution_filename.str() );

    xmlNodePtr hadcg_example_node = xmlNewDocNode( docPtr, NULL, ( const xmlChar* )( "hadcg" ), NULL );

    // Runtime.
    stringstream hadcg_example_runtime_string;
    hadcg_example_runtime_string << microseconds_to_seconds( hadcg_end_time - hadcg_start_time );
    xmlNewProp( hadcg_example_node, ( const xmlChar* )( "runtime" ), ( const xmlChar* )( hadcg_example_runtime_string.str().c_str() ) );
    cout << "    runtime:" << hadcg_example_runtime_string.str() << endl;

    // Search space size: concrete
    double tmp_hadcg_stats = 0.0;
    
    tmp_hadcg_stats = hadcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" );
    num_concrete_hadcg += tmp_hadcg_stats;
    xmlNewProp( hadcg_example_node, ( const xmlChar* )( "search_space_concrete_size" ), (const xmlChar* )( to_std_string( tmp_hadcg_stats ).c_str() ) );

    // Search space size: abstract average
    tmp_hadcg_stats = hadcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" );
    num_abstract_avg_hadcg += tmp_hadcg_stats;
    xmlNewProp( hadcg_example_node, ( const xmlChar* )( "search_space_abstract_avg_size" ), (const xmlChar* )( to_std_string( tmp_hadcg_stats ).c_str() ) );

    // Search space size: abstract max 
    tmp_hadcg_stats = hadcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" );
    num_abstract_max_hadcg += tmp_hadcg_stats;
    xmlNewProp( hadcg_example_node, ( const xmlChar* )( "search_space_abstract_max_size" ), (const xmlChar* )( to_std_string( tmp_hadcg_stats ).c_str() ) );

    // Accuracy statistics.
    if( root_compare_phrases( *truth_phrase, *static_cast< Phrase* >( hadcg->solutions().front().second ) ) ){
      cout << "    correct (root)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_root_hadcg++;
    } else {
      cout << "    incorrect (root)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "root_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_root_hadcg++;
    }

    if( complete_compare_phrases( *truth_phrase, *static_cast< Phrase* >( hadcg->solutions().front().second ) ) ){
      cout << "    correct (complete)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "true" ) );
      num_correct_complete_hadcg++;
    } else {
      cout << "    incorrect (complete)" << endl;
      xmlNewProp( hadcg_example_node, ( const xmlChar* )( "complete_correct" ), ( const xmlChar* )( "false" ) );
      num_incorrect_complete_hadcg++;
    }

    // Add the hadcg_example_node to the example_node in the xml file.
    xmlAddChild( example_node, hadcg_example_node );
    // clear the input_phrase for groundings to run the test for the next model.
    clear( input_phrase );
 
    if( debug ) {
      cout << "hadcg_concrete_size" << to_std_string( hadcg->solutions().front().second->statistic_aggregate_property_phrases( "concrete_size", "per-phrase-avg" ) ) << endl;
      cout << "hadcg_abstract_avg_size" << to_std_string( hadcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_avg_size", "per-phrase-avg" ) ) << endl;
      cout << "hadcg_abstract_avg_size" << to_std_string( hadcg->solutions().front().second->statistic_aggregate_property_phrases( "abstract_max_size", "per-phrase-avg" ) ) << endl;
    }
  
    //find example_runtime_object_ratio_hadcg && example_runtime_object_ratio_ahadcg
    unsigned int example_num_objects = world->objects().size(); 
    
    num_objects = num_objects + ( double ) example_num_objects;
    num_phrases = num_phrases + ( double ) truth_phrase->num_phrases();

    double example_runtime_object_ratio_hadcg = ( microseconds_to_seconds( hadcg_end_time - hadcg_start_time ) )/ ( example_num_objects ) ;

    //add # of phrases to example node
    stringstream example_num_phrases_string;
    example_num_phrases_string << truth_phrase->num_phrases();
    xmlNewProp( example_node, ( const xmlChar* )( "num_phrases" ), ( const xmlChar* )( example_num_phrases_string.str().c_str() ) );

    //add # of world objects to example node
    stringstream example_num_objects_string;
    example_num_objects_string << example_num_objects;
    xmlNewProp( example_node, ( const xmlChar* )( "world_objects" ), ( const xmlChar* )( example_num_objects_string.str().c_str() ) );

    stringstream example_runtime_object_ratio_hadcg_string;
    example_runtime_object_ratio_hadcg_string << example_runtime_object_ratio_hadcg;
    xmlNewProp( example_node, ( const xmlChar* )( "runtime_object_ratio_hadcg" ), ( const xmlChar* )( example_runtime_object_ratio_hadcg_string.str().c_str() ) );

    // Export the world to xml. 
    world->to_xml( docPtr, example_node );

    // Clean up.
    delete_ptr< Grounding >( context );
    delete_ptr< HADCG >( hadcg );
    delete_ptr< World >( world );
    delete_ptr< Phrase >( input_phrase );
    delete_ptr< Phrase >( truth_phrase );

    // Add the node in XML.
    xmlAddChild( test_group_node, example_node );
  }

/****************************** Write the statistics to XML: HADCG and AHADCG*************************/

  // Average runtime.
  runtime_hadcg /= ( double )( filenames.size() );

  // Avg. runtime ratio
  average_runtime_ratio /= ( double )( filenames.size() );
 
  // Average number of phrases and objects 
  num_phrases /= ( double )( filenames.size() );
  num_objects /= ( double )( filenames.size() );

  // Correct root for all models.
  double correct_root_hadcg = ( double )( num_correct_root_hadcg ) / ( double )( num_correct_root_hadcg + num_incorrect_root_hadcg );
  // Correct complete for all models.
  double correct_complete_hadcg = ( double )( num_correct_complete_hadcg ) / ( double )( num_correct_complete_hadcg + num_incorrect_complete_hadcg );
  // Search Space
  // Search Space Sizes
  dataset_concrete_hadcg = num_concrete_hadcg / ( double )( filenames.size() ) ;
  dataset_abstract_avg_hadcg = num_abstract_avg_hadcg / ( double )( filenames.size() ) ;
  dataset_abstract_max_hadcg = num_abstract_avg_hadcg / ( double )( filenames.size() );

  /******  XML: correct root **************************/ 
  stringstream correct_root_hadcg_string;
  correct_root_hadcg_string << correct_root_hadcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_root_hadcg" ), ( const xmlChar* )( correct_root_hadcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_root_hadcg" ).c_str() ), ( const xmlChar* )( correct_root_hadcg_string.str().c_str() ) );

  /******  XML: correct complete  **************************/ 
  stringstream correct_complete_hadcg_string;
  correct_complete_hadcg_string << correct_complete_hadcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "correct_complete_hadcg" ), ( const xmlChar* )( correct_complete_hadcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_correct_complete_hadcg" ).c_str() ), ( const xmlChar* )( correct_complete_hadcg_string.str().c_str() ) );

  /******  XML: runtime  **************************/ 
  stringstream runtime_hadcg_string;
  runtime_hadcg_string << runtime_hadcg;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_hadcg" ), ( const xmlChar* )( runtime_hadcg_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_hadcg" ).c_str() ), ( const xmlChar* )( runtime_hadcg_string.str().c_str() ) );

  /******  XML: runtime  **************************/ 
  stringstream runtime_ratio_string;
  runtime_ratio_string << average_runtime_ratio;
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_ratio" ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_runtime_ratio" ).c_str() ), ( const xmlChar* )( runtime_ratio_string.str().c_str() ) );

  /******  XML: num phrases  **************************/ 
  stringstream num_phrases_string;
  num_phrases_string << num_phrases;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_phrases" ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_phrases" ).c_str() ), ( const xmlChar* )( num_phrases_string.str().c_str() ) );

  /******  XML: num objects  **************************/ 
  stringstream num_objects_string;
  num_objects_string << num_objects;
  xmlNewProp( test_group_node, ( const xmlChar* )( "avg_num_objects" ), ( const xmlChar* )( num_objects_string.str().c_str() ) );
  xmlNewProp( parentNode, ( const xmlChar* )( ( testGroup + "_num_objects" ).c_str() ), ( const xmlChar* )( num_objects_string.str().c_str() ) );

  /******  XML: num objects  **************************/ 
  stringstream runtime_object_ratio_hadcg_string;
  runtime_object_ratio_hadcg_string << ( runtime_hadcg / num_objects );
  xmlNewProp( test_group_node, ( const xmlChar* )( "runtime_object_ratio_hadcg" ), ( const xmlChar* )( runtime_object_ratio_hadcg_string.str().c_str() ) );

  /******  console output **************************/ 
  cout << endl;
  cout << "correct_root_hadcg: " << correct_root_hadcg << endl;

  cout << "correct_complete_hadcg: " << correct_complete_hadcg << endl;
  cout << "runtime_hadcg: " << runtime_hadcg << endl;

  cout << "hadcg_search_space_concrete: " << dataset_concrete_hadcg << endl;
  cout << "hadcg_search_space_abstract_avg: " << dataset_abstract_avg_hadcg << endl;
  cout << "hadcg_search_space_abstract_max: " << dataset_abstract_max_hadcg << endl;
  xmlAddChild( parentNode, test_group_node );

  return;

}





