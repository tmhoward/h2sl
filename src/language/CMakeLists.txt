set(LIB h2sl-language)

add_library(${LIB} SHARED
            word.cc
            phrase.cc)
target_link_libraries(${LIB} h2sl-symbol ${LIBXML2_LIBRARIES})

add_executable(h2sl-pos-demo pos_demo.cc)
target_link_libraries(h2sl-pos-demo ${LIB} h2sl-symbol ${LIBXML2_LIBRARIES})

add_executable(h2sl-word-demo word_demo.cc)
target_link_libraries(h2sl-word-demo ${LIB} h2sl-symbol ${LIBXML2_LIBRARIES})

add_executable(h2sl-phrase-demo phrase_demo.cc)
target_link_libraries(h2sl-phrase-demo ${LIB} h2sl-symbol ${LIBXML2_LIBRARIES})

install(FILES 
        h2sl/pos.h
        h2sl/word.h
        h2sl/phrase.h
        DESTINATION include/${PROJECT_NAME})

install(TARGETS ${LIB}
        LIBRARY
        DESTINATION lib)

install(TARGETS 
        h2sl-pos-demo
        h2sl-word-demo
        h2sl-phrase-demo
        DESTINATION bin)
