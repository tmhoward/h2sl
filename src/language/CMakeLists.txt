# GENGETOPT FILES
set(GGOS
    pos_demo.ggo
    word_demo.ggo
    phrase_demo.ggo)

# HEADER FILES
set(HDRS
    h2sl/pos.h
    h2sl/word.h
    h2sl/phrase.h)

# QT HEADER FILES
set(QT_HDRS )

# SOURCE FILES
set(SRCS
    word.cc
    phrase.cc)

# BINARY SOURCE FILES
set(BIN_SRCS
    pos_demo.cc 
    word_demo.cc
    phrase_demo.cc)

# LIBRARY DEPENDENCIES
set(DEPS h2sl-symbol h2sl-common ${LIBXML2_LIBRARIES})

# LIBRARY NAME
set(LIB h2sl-language)

# process the headers
process_hdrs(HDRS ${HDRS} QT_HDRS ${QT_HDRS})

# process the library
process_lib(LIB ${LIB} QT_HDRS ${QT_HDRS} SRCS ${SRCS} DEPS ${DEPS})

# process the binaries
process_bin_srcs(LIB ${LIB} BIN_SRCS ${BIN_SRCS} GGOS ${GGOS} DEPS ${DEPS})
