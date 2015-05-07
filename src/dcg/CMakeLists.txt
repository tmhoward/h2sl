# GENGETOPT FILES
set(GGOS
    factor_demo.ggo
    factor_set_demo.ggo
    dcg_demo.ggo
    dcg_test.ggo)

# HEADER FILES
set(HDRS
    h2sl/factor.h
    h2sl/factor_set.h
    h2sl/dcg.h)

# QT HEADER FILES
set(QT_HDRS )

# SOURCE FILES
set(SRCS
    factor.cc
    factor_set.cc
    dcg.cc)

# BINARY SOURCE FILES
set(BIN_SRCS
    factor_demo.cc
    factor_set_demo.cc
    dcg_demo.cc
    dcg_test.cc)

# LIBRARY DEPENDENCIES
set(DEPS h2sl-llm h2sl-language h2sl-symbol h2sl-common ${LIBXML2_LIBRARIES})

# LIBRARY NAME
set(LIB h2sl-dcg)

# process the headers
process_hdrs(HDRS ${HDRS} QT_HDRS ${QT_HDRS})

# process the library
process_lib(LIB ${LIB} QT_HDRS ${QT_HDRS} SRCS ${SRCS} DEPS ${DEPS})

# process the binaries
process_bin_srcs(LIB ${LIB} BIN_SRCS ${BIN_SRCS} GGOS ${GGOS} DEPS ${DEPS})
