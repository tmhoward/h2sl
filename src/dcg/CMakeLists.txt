generate_ggo(factor_demo.ggo factor_demo_cmdline)
generate_ggo(dcg_demo.ggo dcg_demo_cmdline)

set(LIB h2sl-dcg)

add_library(${LIB} SHARED
            factor.cc
            dcg.cc)
target_link_libraries(${LIB} h2sl-llm h2sl-language h2sl-symbol ${LIBXML2_LIBRARIES})

add_executable(h2sl-factor-demo factor_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/factor_demo_cmdline.c)
target_link_libraries(h2sl-factor-demo ${LIB} h2sl-llm h2sl-language h2sl-symbol ${LIBXML2_LIBRARIES})

add_executable(h2sl-dcg-demo dcg_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/dcg_demo_cmdline.c)
target_link_libraries(h2sl-dcg-demo ${LIB} h2sl-llm h2sl-parser h2sl-language h2sl-symbol ${LBFGS_LIBRARIES} ${LIBXML2_LIBRARIES})

install(FILES 
        h2sl/factor.h
        h2sl/dcg.h
        DESTINATION include/${PROJECT_NAME})

install(TARGETS ${LIB}
        DESTINATION lib)

install(TARGETS 
        h2sl-factor-demo
        h2sl-dcg-demo
        DESTINATION bin)
