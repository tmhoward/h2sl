if (LCM_GEN)
    generate_lcm(h2sl_transform_t.lcm h2sl/transform_t.hpp)
    generate_lcm(h2sl_object_t.lcm h2sl/object_t.hpp)
    generate_lcm(h2sl_world_t.lcm h2sl/world_t.hpp)
    generate_lcm(h2sl_region_t.lcm h2sl/region_t.hpp)
    generate_lcm(h2sl_constraint_t.lcm h2sl/constraint_t.hpp)
    generate_lcm(h2sl_constraint_set_t.lcm h2sl/constraint_set_t.hpp)
endif (LCM_GEN)

generate_ggo(llm_train.ggo llm_train_cmdline)
generate_ggo(gui_demo.ggo gui_demo_cmdline)
generate_ggo(gui_lcm_demo.ggo gui_lcm_demo_cmdline)
generate_ggo(lcm_message_spy.ggo lcm_message_spy_cmdline)

set(LIB h2sl-util)

if (LCM_FOUND)
    qt_wrap_cpp(h2sl-util
        MOC_SRCS
        h2sl/gui.h
        h2sl/gui_lcm.h)
else (LCM_FOUND)
    qt_wrap_cpp(h2sl-util
        MOC_SRCS
        h2sl/gui.h)
endif (LCM_FOUND)

if (LCM_FOUND)
    set (LCM_MESSAGE_HEADERS
        ${CMAKE_CURRENT_BINARY_DIR}/h2sl/transform_t.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/h2sl/object_t.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/h2sl/world_t.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/h2sl/region_t.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/h2sl/constraint_t.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/h2sl/constraint_set_t.hpp)
    set (LCM_GUI_SRC gui_lcm.cc)
else (LCM_FOUND)
    set (LCM_MESSAGE_HEADERS)
    set (LCM_GUI_SRC)
endif (LCM_FOUND)

add_library(${LIB} SHARED
            gui.cc
            ${LCM_GUI_SRC}
            ${LCM_MESSAGE_HEADERS}
            ${MOC_SRCS})
if (LCM_FOUND)
  target_link_libraries(${LIB} h2sl-dcg h2sl-llm h2sl-parser h2sl-language h2sl-symbol h2sl-common ${LBFGS_LIBRARIES} ${QT_LIBRARIES} ${LIBXML2_LIBRARIES} ${LCM_LIBRARIES})
else (LCM_FOUND)
  target_link_libraries(${LIB} h2sl-dcg h2sl-llm h2sl-parser h2sl-language h2sl-symbol h2sl-common ${LBFGS_LIBRARIES} ${QT_LIBRARIES} ${LIBXML2_LIBRARIES})
endif (LCM_FOUND)

add_executable(h2sl-llm-train llm_train.cc ${CMAKE_CURRENT_BINARY_DIR}/llm_train_cmdline.c)
target_link_libraries(h2sl-llm-train ${LIB} h2sl-dcg h2sl-llm h2sl-parser h2sl-language h2sl-symbol h2sl-common ${LBFGS_LIBRARIES} ${QT_LIBRARIES} ${LIBXML2_LIBRARIES})

add_executable(h2sl-gui-demo gui_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/gui_demo_cmdline.c)
target_link_libraries(h2sl-gui-demo ${LIB} h2sl-dcg h2sl-llm h2sl-parser h2sl-language h2sl-symbol h2sl-common ${LBFGS_LIBRARIES} ${QT_LIBRARIES} ${LIBXML2_LIBRARIES})

if (LCM_FOUND)
    add_executable(h2sl-gui-lcm-demo gui_lcm_demo.cc ${CMAKE_CURRENT_BINARY_DIR}/gui_lcm_demo_cmdline.c)
    target_link_libraries(h2sl-gui-lcm-demo ${LIB} h2sl-dcg h2sl-llm h2sl-parser h2sl-language h2sl-symbol h2sl-common ${LBFGS_LIBRARIES} ${QT_LIBRARIES} ${LIBXML2_LIBRARIES} ${LCM_LIBRARIES})

    add_executable(h2sl-lcm-message-spy lcm_message_spy.cc ${CMAKE_CURRENT_BINARY_DIR}/lcm_message_spy_cmdline.c)
    target_link_libraries(h2sl-lcm-message-spy ${LIB} h2sl-dcg h2sl-llm h2sl-parser h2sl-language h2sl-symbol h2sl-common ${LBFGS_LIBRARIES} ${QT_LIBRARIES} ${LIBXML2_LIBRARIES} ${LCM_LIBRARIES})
endif (LCM_FOUND)

install(FILES 
        h2sl/gui.h
        h2sl/gui_lcm.h
        ${LCM_MESSAGE_HEADERS}
        DESTINATION include/${PROJECT_NAME})

install(TARGETS ${LIB}
        DESTINATION lib)

if (LCM_FOUND)
    install(TARGETS 
        h2sl-gui-demo
        h2sl-llm-train
        h2sl-gui-lcm-demo
        h2sl-lcm-message-spy
        DESTINATION bin)
else (LCM_FOUND)
    install(TARGETS 
        h2sl-gui-demo
        h2sl-llm-train
        DESTINATION bin)
endif (LCM_FOUND)
