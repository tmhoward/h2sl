set(LIB h2sl-symbol)

add_library(${LIB} SHARED
            grounding.cc
            object.cc
            object_set.cc
            region.cc
            region_set.cc
            constraint.cc
            constraint_set.cc
            world.cc)
target_link_libraries(${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-grounding-demo grounding_demo.cc)
target_link_libraries(h2sl-grounding-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-object-demo object_demo.cc)
target_link_libraries(h2sl-object-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-object-set-demo object_set_demo.cc)
target_link_libraries(h2sl-object-set-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-region-demo region_demo.cc)
target_link_libraries(h2sl-region-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-region-set-demo region_set_demo.cc)
target_link_libraries(h2sl-region-set-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-constraint-demo constraint_demo.cc)
target_link_libraries(h2sl-constraint-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-constraint-set-demo constraint_set_demo.cc)
target_link_libraries(h2sl-constraint-set-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-world-demo world_demo.cc)
target_link_libraries(h2sl-world-demo ${LIB} ${LIBXML2_LIBRARIES})

install(FILES 
        h2sl/grounding.h
        h2sl/object.h
        h2sl/object_set.h
        h2sl/region.h
        h2sl/region_set.h
        h2sl/constraint.h
        h2sl/constraint_set.h
        h2sl/world.h
        DESTINATION include/${PROJECT_NAME})

install(TARGETS ${LIB}
        LIBRARY
        DESTINATION lib
        ARCHIVE
        DESTINATION lib)

install(TARGETS 
        h2sl-grounding-demo
        h2sl-object-demo
        h2sl-object-set-demo
        h2sl-region-demo
        h2sl-region-set-demo
        h2sl-constraint-demo
        h2sl-constraint-set-demo
        h2sl-world-demo
        DESTINATION bin)
