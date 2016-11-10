# GENGETOPT FILES
set(GGOS
    cv_demo.ggo
    feature_matches_child_demo.ggo
    feature_object_matches_child_demo.ggo
    feature_product_demo.ggo
    feature_set_demo.ggo
    llm_demo.ggo
    example_demo.ggo)

# HEADER FILES
set(HDRS
    h2sl/cv.h
    h2sl/feature.h
    h2sl/feature_cv.h
    h2sl/feature_word.h
    h2sl/feature_num_words.h
    h2sl/feature_type.h
    h2sl/feature_type.ipp
    h2sl/feature_matches_child.h
    h2sl/feature_matches_child.ipp
    h2sl/feature_grounding_property_value.h
    h2sl/feature_region_object_property_value.h
    h2sl/feature_object_matches_child.h
    h2sl/feature_object_matches_child.ipp
    h2sl/feature_region_merge_partially_known_regions.h
    h2sl/feature_constraint_parent_matches_child_region.h
    h2sl/feature_constraint_child_matches_child_region.h
    h2sl/feature_constraint_parent_is_robot.h
    h2sl/feature_constraint_child_is_robot.h
    h2sl/feature_product.h
    h2sl/feature_set.h
    h2sl/feature_abstract_container_matches_child.h
    h2sl/feature_container_matches_child.h
    h2sl/feature_region_container_matches_child.h
    #h2sl/feature_action_object_matches_child.h
    h2sl/feature_spatial_relation_matches_child.h
    h2sl/feature_is_abstract_container.h
    h2sl/feature_is_object.h
    h2sl/feature_is_container.h
    h2sl/feature_is_region.h
    h2sl/feature_is_region_container.h
    h2sl/feature_is_region_abstract_container.h
    h2sl/feature_is_spatial_relation.h
    h2sl/feature_container_number.h
    #h2sl/feature_container_number_equals_world_objects.h
    h2sl/feature_container_is_empty.h
    h2sl/feature_container_type_matches_child_container_type.h
    h2sl/feature_object_property_merge_object_property_container.h
    h2sl/feature_phrase_has_pos_tag.h
    h2sl/feature_phrase_has_single_pos_tag.h
    h2sl/feature_phrase_has_ordered_pos_tag_pair.h
    h2sl/feature_object_property_merge_object_property_spatial_relation.h
    h2sl/feature_container_matches_empty_child_container.h
    h2sl/feature_container_merge_empty_container_container.h
    h2sl/feature_container_merge_object_property_container.h
    h2sl/feature_container_merge_container_spatial_relation.h
    h2sl/feature_region_container_merge_container_spatial_relation.h
    h2sl/feature_region_container_container_matches_child_container.h
    h2sl/feature_object_property_index.h
    h2sl/feature_abstract_container_type.h
    h2sl/feature_abstract_container_color.h
    h2sl/feature_abstract_container_number.h
    h2sl/feature_abstract_container_index.h
    h2sl/feature_region_container_type.h
    h2sl/feature_region_container_container_type.h
    h2sl/feature_object_property_type.h
    h2sl/feature_object_property_relation_type.h
    h2sl/feature_object_property_index.h
    h2sl/feature_region_abstract_container_type.h
    h2sl/feature_region_abstract_container_object_type.h
    h2sl/feature_region_abstract_container_number.h
    h2sl/feature_min_x_object.h
    h2sl/feature_max_x_object.h
    h2sl/feature_max_y_object.h
    h2sl/feature_min_y_object.h
    h2sl/feature_max_x_abstract_container.h
    h2sl/feature_min_distance_object.h
    h2sl/feature_max_distance_object.h
    h2sl/feature_max_distance_object_within.h
    h2sl/feature_min_distance_object_within.h
    h2sl/feature_min_center_distance_object.h
    h2sl/feature_container_dominant_y_shape.h
    h2sl/feature_container_dominant_x_shape.h
    h2sl/feature_container_dominant_z_shape.h
    h2sl/feature_container_non_dominant_xyz_shape.h
    h2sl/feature_max_y_abstract_container.h
    h2sl/feature_min_y_index_abstract_container_region_container.h
    h2sl/feature_min_x_abstract_container.h
    h2sl/feature_min_y_abstract_container.h
    h2sl/feature_min_distance_container.h
    h2sl/feature_min_distance_abstract_container.h
    h2sl/feature_min_abs_y_abstract_container.h
    h2sl/feature_container_dominant_x_shape.h
    h2sl/feature_container_dominant_y_shape.h
    h2sl/feature_container_dominant_z_shape.h
    h2sl/feature_container_non_dominant_xyz_shape.h
    h2sl/feature_min_x_container_abstract_container_spatial_relation.h
    h2sl/feature_objects_shape_matches_container_type.h
    h2sl/feature_min_y_container_abstract_container_spatial_relation.h
    h2sl/feature_max_y_container_abstract_container.h
    h2sl/feature_min_abs_y_container_abstract_container.h
    h2sl/feature_min_x_container_region_abstract_container.h
    h2sl/feature_min_x_region_abstract_container_spatial_relation.h
    h2sl/feature_max_x_region_abstract_container.h
    #h2sl/feature_object_matches_child.h
    h2sl/feature_container_min_distance.h
    h2sl/llm.h)

# QT HEADER FILES
set(QT_HDRS )

# SOURCE FILES
set(SRCS
    feature.cc
    feature_cv.cc
    feature_word.cc
    feature_num_words.cc
    feature_grounding_property_value.cc
    feature_region_object_property_value.cc
    feature_region_merge_partially_known_regions.cc
    feature_constraint_parent_matches_child_region.cc
    feature_constraint_child_matches_child_region.cc
    feature_constraint_parent_is_robot.cc
    feature_constraint_child_is_robot.cc
    feature_product.cc
    feature_set.cc
    feature_abstract_container_matches_child.cc
    feature_container_matches_child.cc
    feature_region_container_matches_child.cc
    #feature_action_object_matches_child.cc
    feature_spatial_relation_matches_child.cc
    feature_is_abstract_container.cc
    feature_is_object.cc
    feature_is_container.cc
    feature_is_region.cc
    feature_is_region_container.cc
    feature_is_region_abstract_container.cc
    feature_is_spatial_relation.cc
    feature_container_number.cc
    #feature_container_number_equals_world_objects.cc
    feature_container_is_empty.cc
    feature_container_type_matches_child_container_type.cc
    feature_object_property_merge_object_property_container.cc
    feature_phrase_has_pos_tag.cc
    feature_phrase_has_single_pos_tag.cc
    feature_phrase_has_ordered_pos_tag_pair.cc
    feature_object_property_merge_object_property_spatial_relation.cc
    feature_container_matches_empty_child_container.cc
    feature_container_merge_empty_container_container.cc
    feature_container_merge_object_property_container.cc
    feature_container_merge_container_spatial_relation.cc
    feature_region_container_merge_container_spatial_relation.cc
    feature_region_container_container_matches_child_container.cc
    feature_object_property_index.cc
    feature_abstract_container_type.cc
    feature_abstract_container_color.cc
    feature_abstract_container_number.cc
    feature_abstract_container_index.cc
    feature_region_container_type.cc
    feature_region_container_container_type.cc
    feature_object_property_type.cc
    feature_object_property_relation_type.cc
    feature_object_property_index.cc
    feature_region_abstract_container_type.cc
    feature_region_abstract_container_object_type.cc
    feature_region_abstract_container_number.cc
    feature_min_x_object.cc
    feature_max_x_object.cc
    feature_max_y_object.cc
    feature_min_y_object.cc
    feature_max_x_abstract_container.cc
    feature_min_distance_object.cc
    feature_max_distance_object.cc
    feature_max_distance_object_within.cc
    feature_min_distance_object_within.cc
    feature_min_center_distance_object.cc
    feature_container_dominant_y_shape.cc
    feature_container_dominant_x_shape.cc
    feature_container_dominant_z_shape.cc
    feature_container_non_dominant_xyz_shape.cc
    feature_max_y_abstract_container.cc
    feature_min_y_index_abstract_container_region_container.cc
    feature_min_x_abstract_container.cc
    feature_min_y_abstract_container.cc
    feature_min_distance_container.cc
    feature_min_distance_abstract_container.cc
    feature_min_abs_y_abstract_container.cc
    feature_container_dominant_x_shape.cc
    feature_container_dominant_y_shape.cc
    feature_container_dominant_z_shape.cc
    feature_container_non_dominant_xyz_shape.cc
    feature_min_x_container_abstract_container_spatial_relation.cc
    feature_objects_shape_matches_container_type.cc
    feature_min_y_container_abstract_container_spatial_relation.cc
    feature_max_y_container_abstract_container.cc
    feature_min_abs_y_container_abstract_container.cc
    feature_min_x_container_region_abstract_container.cc
    feature_min_x_region_abstract_container_spatial_relation.cc
    feature_max_x_region_abstract_container.cc
    #feature_object_matches_child.cc
    feature_container_min_distance.cc
    llm.cc)

# BINARY SOURCE FILES
set(BIN_SRCS
    cv_demo.cc
    feature_matches_child_demo.cc
    feature_object_matches_child_demo.cc
    feature_product_demo.cc
    feature_set_demo.cc
    llm_demo.cc
    example_demo.cc )

# LIBRARY DEPENDENCIES
set(DEPS h2sl-parser h2sl-language h2sl-symbol h2sl-common ${LBFGS_LIBRARY} ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES})

# LIBRARY NAME
set(LIB h2sl-llm)

# process the headers
process_hdrs(HDRS ${HDRS} QT_HDRS ${QT_HDRS})

# process the library
process_lib(LIB ${LIB} QT_HDRS ${QT_HDRS} SRCS ${SRCS} DEPS ${DEPS})

# process the binaries
process_bin_srcs(LIB ${LIB} BIN_SRCS ${BIN_SRCS} GGOS ${GGOS} DEPS ${DEPS})
