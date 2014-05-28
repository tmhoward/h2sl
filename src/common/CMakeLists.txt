generate_ggo(common_demo.ggo common_demo_cmdline)
generate_ggo(vector3_demo.ggo vector3_demo_cmdline)
generate_ggo(unit_quaternion_demo.ggo unit_quaternion_demo_cmdline)
generate_ggo(transform_demo.ggo transform_demo_cmdline)

set(LIB h2sl-common)

add_library(${LIB} SHARED
            vector3.cc
            unit_quaternion.cc
            transform.cc)
target_link_libraries(${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-vector3-demo vector3_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/vector3_demo_cmdline.c)
target_link_libraries(h2sl-vector3-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-common-demo common_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/common_demo_cmdline.c)
target_link_libraries(h2sl-common-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-unit-quaternion-demo unit_quaternion_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/unit_quaternion_demo_cmdline.c)
target_link_libraries(h2sl-unit-quaternion-demo ${LIB} ${LIBXML2_LIBRARIES})

add_executable(h2sl-transform-demo transform_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/transform_demo_cmdline.c)
target_link_libraries(h2sl-transform-demo ${LIB} ${LIBXML2_LIBRARIES})

install(FILES 
        h2sl/common.h
        h2sl/vector3.h
        h2sl/unit_quaternion.h
        h2sl/transform.h
        DESTINATION include/${PROJECT_NAME})

install(TARGETS ${LIB}
        LIBRARY
        DESTINATION lib
        ARCHIVE
        DESTINATION lib)

install(TARGETS 
        h2sl-common-demo
        h2sl-vector3-demo
        h2sl-unit-quaternion-demo
        h2sl-transform-demo
        DESTINATION bin)
