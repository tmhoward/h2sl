# Install script for directory: /Users/derya/Desktop/codes/h2sl/src/llm

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/h2sl" TYPE FILE FILES
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/cv.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_cv.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_word.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_num_words.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_type.ipp"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_matches_child.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_matches_child.ipp"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_grounding_property_value.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_region_object_property_value.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_region_merge_partially_known_regions.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_constraint_parent_matches_child_region.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_constraint_child_matches_child_region.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_constraint_payload_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_constraint_reference_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_product.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_set.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_abstract_container_matches_child.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_matches_child.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_region_container_matches_child.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_spatial_relation_matches_child.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_is_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_is_object.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_is_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_is_region.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_is_region_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_is_region_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_is_spatial_relation.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_number.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_number_equals_world_objects.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_is_empty.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_type_matches_child_container_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_property_merge_object_property_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_phrase_has_pos_tag.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_phrase_has_single_pos_tag.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_phrase_has_ordered_pos_tag_pair.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_property_merge_object_property_spatial_relation.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_matches_empty_child_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_merge_empty_container_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_merge_object_property_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_merge_container_spatial_relation.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_region_container_merge_container_spatial_relation.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_region_container_container_matches_child_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_property_index.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_abstract_container_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_abstract_container_color.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_abstract_container_number.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_abstract_container_index.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_region_container_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_region_container_container_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_property_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_property_relation_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_property_index.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_region_abstract_container_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_region_abstract_container_object_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_region_abstract_container_number.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_x_object.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_max_x_object.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_max_y_object.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_y_object.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_max_x_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_distance_object.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_max_distance_object.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_max_distance_object_within.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_distance_object_within.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_center_distance_object.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_dominant_y_shape.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_dominant_x_shape.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_dominant_z_shape.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_non_dominant_xyz_shape.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_max_y_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_y_index_abstract_container_region_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_x_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_y_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_distance_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_distance_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_abs_y_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_dominant_x_shape.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_dominant_y_shape.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_dominant_z_shape.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_non_dominant_xyz_shape.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_x_container_abstract_container_spatial_relation.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_objects_shape_matches_container_type.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_y_container_abstract_container_spatial_relation.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_max_y_container_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_abs_y_container_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_x_container_region_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_min_x_region_abstract_container_spatial_relation.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_max_x_region_abstract_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_min_distance.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_max_distance.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_matches_child_container_object.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_number_matches_world_object_number.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_merge_abstract_container_index_region_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_merge_abstract_container_region_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_container_merge_container_pair.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_merge_container_pair.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_merge_abstract_container_region_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_merge_object_property_spatial_relation.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_merge_abstract_container_spatial_relation.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_merge_object_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_merge_single_object_different_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_merge_object_different_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_merge_object_property_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_merge_object_property_region_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/feature_object_merge_object_region_container.h"
    "/Users/derya/Desktop/codes/h2sl/src/llm/h2sl/llm.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/derya/Desktop/codes/h2sl/build/lib/libh2sl-llm.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libh2sl-llm.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libh2sl-llm.a")
    execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libh2sl-llm.a")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/derya/Desktop/codes/h2sl/build/bin/h2sl-cv-demo")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-cv-demo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-cv-demo")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/usr/local/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-cv-demo")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-cv-demo")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/derya/Desktop/codes/h2sl/build/bin/h2sl-feature-matches-child-demo")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-matches-child-demo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-matches-child-demo")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/usr/local/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-matches-child-demo")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-matches-child-demo")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/derya/Desktop/codes/h2sl/build/bin/h2sl-feature-object-matches-child-demo")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-object-matches-child-demo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-object-matches-child-demo")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/usr/local/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-object-matches-child-demo")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-object-matches-child-demo")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/derya/Desktop/codes/h2sl/build/bin/h2sl-feature-product-demo")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-product-demo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-product-demo")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/usr/local/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-product-demo")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-product-demo")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/derya/Desktop/codes/h2sl/build/bin/h2sl-feature-set-demo")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-set-demo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-set-demo")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/usr/local/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-set-demo")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-feature-set-demo")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/derya/Desktop/codes/h2sl/build/bin/h2sl-llm-demo")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-llm-demo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-llm-demo")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/usr/local/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-llm-demo")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-llm-demo")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/derya/Desktop/codes/h2sl/build/bin/h2sl-example-demo")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-example-demo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-example-demo")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/usr/local/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-example-demo")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/h2sl-example-demo")
    endif()
  endif()
endif()

