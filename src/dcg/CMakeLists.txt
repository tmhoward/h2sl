# GENGETOPT FILES
set(GGOS
#    factor_set_demo.ggo
    search_space_demo.ggo
    dcg_demo.ggo
    dcg_groundings_demo.ggo
    dcg_rules_demo.ggo
    hdcg_demo.ggo
    adcg_demo.ggo
    dcg_test.ggo
    dcg_test.ggo
    hdcg_test.ggo
    hadcg_test.ggo
    adcg_test.ggo
    dcg_tikz_generator.ggo)

# HEADER FILES
set(HDRS
    h2sl/factor_set.h
    h2sl/factor_set_adcg.h
    h2sl/search_space.h
    h2sl/hadcg.h
    h2sl/hdcg.h
    h2sl/dcg.h
    h2sl/adcg.h)
#    h2sl/adcg_base.h)

# QT HEADER FILES
set(QT_HDRS )

# SOURCE FILES
set(SRCS
    factor_set.cc
    factor_set_adcg.cc
    search_space.cc
    hadcg.cc
    hdcg.cc
    dcg.cc
    adcg.cc)
#    adcg_base.cc)

# BINARY SOURCE FILES
set(BIN_SRCS
#    factor_set_demo.cc
    search_space_demo.cc
    dcg_demo.cc
    dcg_groundings_demo.cc
    dcg_rules_demo.cc
    hdcg_demo.cc
    adcg_demo.cc
    dcg_test.cc
    hdcg_test.cc
    hadcg_test.cc
    adcg_test.cc
    dcg_tikz_generator.cc)

# LIBRARY DEPENDENCIES
set(DEPS h2sl-llm h2sl-parser h2sl-language h2sl-symbol h2sl-common ${LBFGS_LIBRARY} ${Boost_LIBRARIES} ${LIBXML2_LIBRARIES})

# LIBRARY NAME
set(LIB h2sl-dcg)

# process the headers
process_hdrs(HDRS ${HDRS} QT_HDRS ${QT_HDRS})

# process the library
process_lib(LIB ${LIB} QT_HDRS ${QT_HDRS} SRCS ${SRCS} DEPS ${DEPS})

# process the binaries
process_bin_srcs(LIB ${LIB} BIN_SRCS ${BIN_SRCS} GGOS ${GGOS} DEPS ${DEPS})
