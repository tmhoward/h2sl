# GENGETOPT FILES
set(GGOS
    example_demo.ggo
    rewrite_examples.ggo
    generate_rule_examples.ggo
    generate_symbol_dictionary.ggo
    llm_train_groundings.ggo
    llm_train_rules.ggo
    llm_train_groundings_and_rules.ggo
#    llm_train_adcg.ggo
    grammar_generator.ggo
    gui_demo.ggo
    #scrape_results_runtime_vs_world_size.ggo 
    #scrape_results_runtime_normalized_vs_world_size.ggo 
    test_process.ggo
    test_process_training.ggo
    test_process_inference.ggo
    test_generator.ggo
    test_combine_results.ggo
    search_space_corpus_analysis.ggo
    search_space_corpus_analysis_dcg_adcg.ggo
    search_space_corpus_analysis_hdcg_hadcg.ggo
    test_combine_any_results.ggo
    test_combine_model_results.ggo
    test_convert_xml_to_mat.ggo)

# HEADER FILES
set(HDRS
    h2sl/gui.h)

# QT HEADER FILES
set(QT_HDRS 
    h2sl/gui.h)

# SOURCE FILES
set(SRCS
    gui.cc)

# BINARY SOURCE FILES
set(BIN_SRCS
    example_demo.cc
    rewrite_examples.cc
    generate_rule_examples.cc
    generate_symbol_dictionary.cc
    llm_train_groundings.cc
    llm_train_rules.cc
    llm_train_groundings_and_rules.cc
#    llm_train_adcg.cc
    grammar_generator.cc
    gui_demo.cc
    #scrape_results_runtime_vs_world_size.cc 
    #scrape_results_runtime_normalized_vs_world_size.cc 
    test_process.cc
    test_process_training.cc
    test_process_inference.cc
    test_generator.cc
    test_combine_results.cc
    search_space_corpus_analysis.cc
    search_space_corpus_analysis_dcg_adcg.cc
    search_space_corpus_analysis_hdcg_hadcg.cc
    test_combine_any_results.cc
    test_combine_model_results.cc
    test_convert_xml_to_mat.cc)

# LIBRARY DEPENDENCIES
set(DEPS h2sl-dcg h2sl-llm h2sl-parser h2sl-language h2sl-symbol h2sl-common ${LBFGS_LIBRARY} ${QT_LIBRARIES} ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} 
${GLIB_LIBRARIES})

# LIBRARY NAME
set(LIB h2sl-util)

# process the headers
process_hdrs(HDRS ${HDRS} QT_HDRS ${QT_HDRS})

# process the library
process_lib(LIB ${LIB} QT_HDRS ${QT_HDRS} SRCS ${SRCS} DEPS ${DEPS})

# process the binaries
process_bin_srcs(LIB ${LIB} BIN_SRCS ${BIN_SRCS} GGOS ${GGOS} DEPS ${DEPS})
