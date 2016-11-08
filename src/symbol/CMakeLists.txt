# GENGETOPT FILES
set(GGOS
    abstract_container_demo.ggo
    constraint_demo.ggo
    grounding_demo.ggo
    grounding_set_demo.ggo
    container_demo.ggo
    object_demo.ggo
    object_color_demo.ggo
    object_property_demo.ggo
    object_type_demo.ggo
    region_demo.ggo
    world_demo.ggo)

# HEADER FILES
set(HDRS
    h2sl/container.h
    h2sl/grounding.h
    h2sl/grounding_set.h
    h2sl/index.h
    h2sl/number.h    
    h2sl/object.h
    h2sl/object_color.h
    h2sl/object_type.h
    h2sl/object_property.h
    h2sl/region.h
    h2sl/spatial_relation.h
    h2sl/constraint.h
    h2sl/abstract_container.h
    h2sl/region_container.h
    h2sl/region_abstract_container.h
    h2sl/world.h)

# QT HEADER FILES
set(QT_HDRS )

# SOURCE FILES
set(SRCS
    container.cc
    grounding.cc
    grounding_set.cc
    index.cc
    number.cc
    object.cc
    object_color.cc
    object_type.cc
    object_property.cc
    region.cc
    spatial_relation.cc
    constraint.cc
    abstract_container.cc
    region_container.cc
    region_abstract_container.cc
    world.cc)

# BINARY SOURCE FILES
set(BIN_SRCS
    abstract_container_demo.cc
    constraint_demo.cc
    grounding_demo.cc
    grounding_set_demo.cc
    container_demo.cc
    object_demo.cc
    object_color_demo.cc
    object_property_demo.cc
    object_type_demo.cc
    region_demo.cc
    world_demo.cc)

# LIBRARY DEPENDENCIES
set(DEPS h2sl-common ${LIBXML2_LIBRARIES})

# LIBRARY NAME
set(LIB h2sl-symbol)

# process the headers
process_hdrs(HDRS ${HDRS} QT_HDRS ${QT_HDRS})

# process the library
process_lib(LIB ${LIB} QT_HDRS ${QT_HDRS} SRCS ${SRCS} DEPS ${DEPS})

# process the binaries
process_bin_srcs(LIB ${LIB} BIN_SRCS ${BIN_SRCS} GGOS ${GGOS} DEPS ${DEPS})
