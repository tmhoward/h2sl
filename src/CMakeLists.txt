set(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")


include_directories(${PROJECT_SOURCE_DIR}/src/symbol
                    ${PROJECT_SOURCE_DIR}/src/language
                    ${PROJECT_SOURCE_DIR}/src/parser
                    ${PROJECT_SOURCE_DIR}/src/llm
                    ${PROJECT_SOURCE_DIR}/src/dcg
                    ${PROJECT_SOURCE_DIR}/src/util
		    ${PROJECT_BINARY_DIR}/src/parser
		    ${PROJECT_BINARY_DIR}/src/llm
		    ${PROJECT_BINARY_DIR}/src/util)

find_package(LibXml2)
include_directories(${LIBXML2_INCLUDE_DIR})

find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIR})

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
find_package (LCM)
if (LCM_FOUND)
    include_directories (${LCM_INCLUDE_DIRS})
else (LCM_FOUND)    
    message (STATUS "LCM was not found. LCM related bindings GUI will not be built")
endif ()


find_package (LBFGS REQUIRED)
include_directories (${LBFGS_INCLUDE_DIRS})

find_program (LCM_GEN lcm-gen)
if (NOT LCM_GEN)
    message (STATUS "lcm-gen not found\n")
    set (LCM_GEN_FOUND 0)
else ()
    set (LCM_GEN_FOUND 1)
endif()

find_program (GENGETOPT gengetopt)
if( NOT GENGETOPT)
    message (FATAL_ERROR "gengetopt not found\n")
    return()
endif()

macro(generate_lcm input output)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${output}
    COMMAND ${LCM_GEN} --lazy --cpp ${CMAKE_CURRENT_SOURCE_DIR}/${input} --cpp-hpath ${CMAKE_CURRENT_BINARY_DIR}/
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${input}
  )
  include_directories (${CMAKE_CURRENT_BINARY_DIR})
endmacro(generate_lcm)

macro(generate_ggo input output)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${output}.c ${CMAKE_CURRENT_BINARY_DIR}/${output}.h
    COMMAND ${GENGETOPT} -i ${CMAKE_CURRENT_SOURCE_DIR}/${input} --file-name ${output} --output-dir=${CMAKE_CURRENT_BINARY_DIR} --unamed-opts
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${input}
  )
  include_directories (${CMAKE_CURRENT_BINARY_DIR})
endmacro(generate_ggo)

add_definitions(-Wall -g -O3)

add_subdirectory(symbol)
add_subdirectory(language)
add_subdirectory(parser)
add_subdirectory(llm)
add_subdirectory(dcg)
add_subdirectory(util)
