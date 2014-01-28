generate_ggo(cv_demo.ggo cv_demo_cmdline)
generate_ggo(feature_set_demo.ggo feature_set_demo_cmdline)
generate_ggo(llm_demo.ggo llm_demo_cmdline)
generate_ggo(example_demo.ggo example_demo_cmdline)

set(LIB h2sl-llm)

add_library(${LIB} SHARED
            feature.cc
            feature_cv.cc
            feature_word.cc
            feature_num_words.cc
            feature_object.cc
            feature_region_object.cc
            feature_region.cc
            feature_constraint.cc
            feature_region_object_matches_child.cc
            feature_region_matches_child.cc
            feature_region_merge_partially_known_regions.cc
            feature_constraint_parent_matches_child_region.cc
            feature_constraint_child_matches_child_region.cc
            feature_constraint_parent_is_robot.cc
            feature_constraint_child_is_robot.cc
            feature_set.cc
            llm.cc)
target_link_libraries(${LIB} h2sl-language h2sl-symbol ${LBFGS_LIBRARIES} ${LIBXML2_LIBRARIES})

add_executable(h2sl-cv-demo cv_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/cv_demo_cmdline.c)
target_link_libraries(h2sl-cv-demo ${LIB} h2sl-language h2sl-symbol ${LIBXML2_LIBRARIES})

add_executable(h2sl-feature-set-demo feature_set_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/feature_set_demo_cmdline.c)
target_link_libraries(h2sl-feature-set-demo ${LIB} h2sl-parser h2sl-language h2sl-symbol ${LIBXML2_LIBRARIES})

add_executable(h2sl-llm-demo llm_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/llm_demo_cmdline.c)
target_link_libraries(h2sl-llm-demo ${LIB} h2sl-language h2sl-symbol ${LBFGS_LIBRARIES} ${LIBXML2_LIBRARIES})

add_executable(h2sl-example-demo example_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/example_demo_cmdline.c)
target_link_libraries(h2sl-example-demo ${LIB} h2sl-parser h2sl-language h2sl-symbol ${LBFGS_LIBRARIES} ${LIBXML2_LIBRARIES})

install(FILES 
        h2sl/cv.h
        h2sl/feature.h
        h2sl/feature_cv.h
        h2sl/feature_word.h
        h2sl/feature_num_words.h
        h2sl/feature_object.h
        h2sl/feature_region_object.h
        h2sl/feature_region.h
        h2sl/feature_constraint.h
        h2sl/feature_region_object_matches_child.h
        h2sl/feature_region_matches_child.h
        h2sl/feature_region_merge_partially_known_regions.h
        h2sl/feature_constraint_parent_matches_child_region.h
        h2sl/feature_constraint_child_matches_child_region.h
        h2sl/feature_constraint_parent_is_robot.h
        h2sl/feature_constraint_child_is_robot.h
        h2sl/feature_set.h
        h2sl/llm.h
        DESTINATION include/${PROJECT_NAME})

install(TARGETS ${LIB}
        DESTINATION lib)

install(TARGETS 
        h2sl-cv-demo
        h2sl-feature-set-demo
        h2sl-llm-demo
        h2sl-example-demo
        DESTINATION bin)
