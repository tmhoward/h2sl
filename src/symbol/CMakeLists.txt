generate_ggo(grounding_demo.ggo grounding_demo_cmdline)
generate_ggo(grounding_set_demo.ggo grounding_set_demo_cmdline)
generate_ggo(object_demo.ggo object_demo_cmdline)
generate_ggo(region_demo.ggo region_demo_cmdline)
generate_ggo(constraint_demo.ggo constraint_demo_cmdline)
generate_ggo(world_demo.ggo world_demo_cmdline)

set(LIB h2sl-symbol)

add_library(${LIB} SHARED
            grounding.cc
            grounding_set.cc
            object.cc
            region.cc
            constraint.cc
            world.cc)
target_link_libraries(${LIB} h2sl-common ${LIBXML2_LIBRARIES})

add_executable(h2sl-grounding-demo grounding_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/grounding_demo_cmdline.c)
target_link_libraries(h2sl-grounding-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-grounding-set-demo grounding_set_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/grounding_set_demo_cmdline.c)
target_link_libraries(h2sl-grounding-set-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-object-demo object_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/object_demo_cmdline.c)
target_link_libraries(h2sl-object-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-region-demo region_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/region_demo_cmdline.c)
target_link_libraries(h2sl-region-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-constraint-demo constraint_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/constraint_demo_cmdline.c)
target_link_libraries(h2sl-constraint-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-world-demo world_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/world_demo_cmdline.c)
target_link_libraries(h2sl-world-demo ${LIB} ${LIBXML2_LIBRARIES})

install(FILES 
        h2sl/grounding.h
        h2sl/grounding_set.h
        h2sl/object.h
        h2sl/region.h
        h2sl/constraint.h
        h2sl/world.h
        DESTINATION include/${PROJECT_NAME})

install(TARGETS ${LIB}
        LIBRARY
        DESTINATION lib
        ARCHIVE
        DESTINATION lib)

install(TARGETS 
        h2sl-grounding-demo
        h2sl-grounding-set-demo
        h2sl-object-demo
        h2sl-region-demo
        h2sl-constraint-demo
        h2sl-world-demo
        DESTINATION bin)
