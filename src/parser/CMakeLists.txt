generate_ggo(grammar_demo.ggo grammar_demo_cmdline)
generate_ggo(parser_demo.ggo parser_demo_cmdline)
generate_ggo(parser_cyk_demo.ggo parser_cyk_demo_cmdline)

set(LIB h2sl-parser)

add_library(${LIB} SHARED
            grammar_terminal.cc
            grammar_non_terminal.cc
            grammar_unit_production.cc
            grammar.cc)
target_link_libraries(${LIB} h2sl-language h2sl-symbol h2sl-common ${LIBXML2_LIBRARIES})

add_executable(h2sl-grammar-demo grammar_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/grammar_demo_cmdline.c)
target_link_libraries(h2sl-grammar-demo ${LIB} h2sl-language h2sl-symbol h2sl-common ${LIBXML2_LIBRARIES})

add_executable(h2sl-parser-demo parser_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/parser_demo_cmdline.c)
target_link_libraries(h2sl-parser-demo ${LIB} h2sl-language h2sl-symbol h2sl-common ${LIBXML2_LIBRARIES})

add_executable(h2sl-parser-cyk-demo parser_cyk_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/parser_cyk_demo_cmdline.c)
target_link_libraries(h2sl-parser-cyk-demo ${LIB} h2sl-language h2sl-symbol h2sl-common ${LIBXML2_LIBRARIES})

install(FILES 
        h2sl/grammar_terminal.h
        h2sl/grammar_non_terminal.h
        h2sl/grammar_unit_production.h
        h2sl/grammar.h
        h2sl/parser.h
        h2sl/parser.ipp
        h2sl/parser_cyk.h
        h2sl/parser_cyk.ipp
        DESTINATION include/${PROJECT_NAME})

install(TARGETS ${LIB}
        DESTINATION lib)

install(TARGETS 
        h2sl-grammar-demo
        h2sl-parser-demo
        h2sl-parser-cyk-demo
        DESTINATION bin)
