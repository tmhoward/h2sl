# GENGETOPT FILES
set(GGOS
    common_demo.ggo
    vector3_demo.ggo
    unit_quaternion_demo.ggo
    transform_demo.ggo)

# HEADER FILES
set(HDRS
    h2sl/common.h
    h2sl/vector3.h
    h2sl/unit_quaternion.h
    h2sl/transform.h)

# QT HEADER FILES
set(QT_HDRS )

# SOURCE FILES
set(SRCS
    vector3.cc
    unit_quaternion.cc
    transform.cc
    common.cc)

# BINARY SOURCE FILES
set(BIN_SRCS
    common_demo.cc 
    vector3_demo.cc
    unit_quaternion_demo.cc
    transform_demo.cc)

# LIBRARY DEPENDENCIES
set(DEPS ${LIBXML2_LIBRARIES} ${GLIB_LIBRARIES})

# LIBRARY NAME
set(LIB h2sl-common)

# process the headers
process_hdrs(HDRS ${HDRS} QT_HDRS ${QT_HDRS})

# process the library
process_lib(LIB ${LIB} QT_HDRS ${QT_HDRS} SRCS ${SRCS} DEPS ${DEPS})

# process the binaries
process_bin_srcs(LIB ${LIB} BIN_SRCS ${BIN_SRCS} GGOS ${GGOS} DEPS ${DEPS})
