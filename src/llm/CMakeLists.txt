# GENGETOPT FILES
set(GGOS
    cv_demo.ggo
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
    h2sl/feature_product.h
    h2sl/feature_set.h
    h2sl/llm.h)

# QT HEADER FILES
set(QT_HDRS )

# SOURCE FILES
set(SRCS
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
    feature_product.cc
    feature_set.cc
    llm.cc)

# BINARY SOURCE FILES
set(BIN_SRCS
    cv_demo.cc
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
