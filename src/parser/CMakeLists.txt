# GENGETOPT FILES
set(GGOS
    grammar_demo.ggo
    parser_demo.ggo
    parser_cyk_demo.ggo)

# HEADER FILES
set(HDRS
    h2sl/grammar_terminal.h
    h2sl/grammar_non_terminal.h
    h2sl/grammar_unit_production.h
    h2sl/grammar.h
    h2sl/parser.h
    h2sl/parser.ipp
    h2sl/parser_cyk.h
    h2sl/parser_cyk.ipp)

# QT HEADER FILES
set(QT_HDRS )

# SOURCE FILES
set(SRCS
    grammar_terminal.cc
    grammar_non_terminal.cc
    grammar_unit_production.cc
    grammar.cc)

# BINARY SOURCE FILES
set(BIN_SRCS
    grammar_demo.cc
    parser_demo.cc
    parser_cyk_demo.cc)

# LIBRARY DEPENDENCIES
set(DEPS h2sl-language h2sl-symbol h2sl-common ${LIBXML2_LIBRARIES})

# LIBRARY NAME
set(LIB h2sl-parser)

# process the headers
process_hdrs(HDRS ${HDRS} QT_HDRS ${QT_HDRS})

# process the library
process_lib(LIB ${LIB} QT_HDRS ${QT_HDRS} SRCS ${SRCS} DEPS ${DEPS})

# process the binaries
process_bin_srcs(LIB ${LIB} BIN_SRCS ${BIN_SRCS} GGOS ${GGOS} DEPS ${DEPS})
