cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(h2sl
  LANGUAGES CXX
  VERSION 2.0
)

## Compile as C++17, supported in ROS Kinetic and newer
set(CMAKE_CXX_STANDARD 17)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS cmake_modules roscpp message_generation)

## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS system program_options filesystem)
find_package(TinyXML2 REQUIRED)
find_package(PkgConfig REQUIRED)

PKG_CHECK_MODULES(PC_LIBLBFGS QUIET liblbfgs)
find_path(LBFGS_INCLUDE_DIRS NAMES lbfgs.h HINTS ${PC_LIBBFGS_INCLUDEDIR} ${PC_LIBLBFGS_INCLUDE_DIRS})
find_library(LBFGS_LIBRARIES NAMES lbfgs liblbfgs HINTS ${PC_LIBLBFGS_LIBDIR} ${PC_LIBLBFGS_LIBRARY_DIRS})


pkg_check_modules(JSONCPP jsoncpp)
if( NOT JSONCPP_FOUND )
  message( FATAL_ERROR "jsoncpp not found\n" )
  return()
endif()

################################################
## Declare ROS messages, services and actions ##
################################################

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  Vector3DMessage.msg
  UnitQuaternionMessage.msg
  Transform3DMessage.msg
  BoundingBoxMessage.msg
  PropertyMessage.msg
  ObjectStateMessage.msg
  ObjectMessage.msg
  WorldMessage.msg
  WordMessage.msg
  SymbolMessage.msg
  SymbolsMessage.msg
  LanguageVariableConnectionMessage.msg
  LanguageVariableNodeMessage.msg
  LanguageVariableMessage.msg
)

## Generate services in the 'srv' folder
add_service_files(
  FILES
  GroundLanguage.srv
  Parse.srv
)

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES h2sl
  CATKIN_DEPENDS message_runtime
  DEPENDS Boost TinyXML2
)

###########
## Build ##
###########

## Specify additional locations of header files
include_directories(
  include
  ${LBFGS_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${TinyXML2_INCLUDE_DIRS}
)

## Declare a C++ library
add_library(h2sl
  src/h2sl/dcg.cc
  src/h2sl/dialogue.cc
  src/h2sl/environment-constraint.cc
  src/h2sl/environment-constraint-map.cc
  src/h2sl/example.cc
  src/h2sl/expressed-feature.cc
  src/h2sl/factor.cc
  src/h2sl/factorset.cc
  src/h2sl/feature.cc
  src/h2sl/feature-cv.cc
  src/h2sl/feature-word.cc
  src/h2sl/feature-num-words.cc
  src/h2sl/feature-lv-type.cc
  src/h2sl/feature-lv-edge-label.cc 
  src/h2sl/feature-lv-role-value.cc
  src/h2sl/feature-object-is-unique.cc
  src/h2sl/feature-object-position-order.cc
  src/h2sl/feature-object-merge-object-type-region.cc
  src/h2sl/feature-object-merge-object-type-spatial-relation.cc
  src/h2sl/feature-object-position-order.cc
  src/h2sl/feature-object-less-position-order.cc
  src/h2sl/feature-set.cc
  src/h2sl/feature-symbol-attribute-value.cc
  src/h2sl/feature-symbol-type.cc
  src/h2sl/feature-symbol-has-attribute.cc
  src/h2sl/feature-symbol-has-child.cc
  src/h2sl/feature-symbol-matches-child.cc
  src/h2sl/feature-symbol-child-attribute-value.cc
  src/h2sl/feature-symbol-attribute-value-matches-child-attribute-value.cc
  src/h2sl/feature-symbol-attribute-value-matches-child-attribute-value-label.cc
  src/h2sl/feature-symbol-attribute-value-matches-lv-role.cc
  src/h2sl/feature-symbol-property-csv-contains-child-property-value.cc
  src/h2sl/feature-spatial-relation-position.cc
  src/h2sl/feature-spatial-relation-position-object-type.cc
  src/h2sl/feature-pool.cc
  src/h2sl/feature-spatial-relation-children-satisfied.cc
  src/h2sl/feature-spatial-relation-children-satisfied-object-type.cc
  src/h2sl/feature-spatial-relation-children-contradicted.cc
  src/h2sl/feature-spatial-relation-merge-object-spatial-relation.cc
  src/h2sl/pcfg.cc
  src/h2sl/binarized-pcfg.cc
  src/h2sl/object.cc
  src/h2sl/object-probabilistic.cc
  src/h2sl/objectstate.cc
  src/h2sl/llm.cc
  src/h2sl/language-variable.cc
  src/h2sl/language-constraint.cc
  src/h2sl/language-constraint-map.cc
  src/h2sl/parser.cc
  src/h2sl/sentence.cc
  src/h2sl/spatial-relation.cc
  src/h2sl/symbol.cc
  src/h2sl/symbolcompositionalinfo.cc
  src/h2sl/symboldictionary.cc
  src/h2sl/symboldictionary-methods.cc
  src/h2sl/symbolspace.cc
  src/h2sl/word.cc
  src/h2sl/world.cc
  src/h2sl/world-probabilistic.cc
  src/h2sl/lexicon.cc
  src/h2sl/vector3d.cc
  src/h2sl/unit-quaternion.cc
  src/h2sl/transform3d.cc
  src/h2sl/bounding-box.cc
)

## Add cmake target dependencies of the library
add_dependencies(h2sl ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${catkin_LIBRARIES})

## C++ executables
add_executable(h2sl-word-demo src/word-demo.cc)
add_dependencies(h2sl-word-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-word-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-environment-constraint-demo src/environment-constraint-demo.cc)
add_dependencies(h2sl-environment-constraint-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-environment-constraint-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-feature-demo src/feature-demo.cc)
add_dependencies(h2sl-feature-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-feature-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-feature-set-demo src/feature-set-demo.cc)
add_dependencies(h2sl-feature-set-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-feature-set-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-feature-pool-demo src/feature-pool-demo.cc)
add_dependencies(h2sl-feature-pool-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-feature-pool-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-symbol-demo src/symbol-demo.cc)
add_dependencies(h2sl-symbol-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-symbol-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-symbolspace-demo src/symbolspace-demo.cc)
add_dependencies(h2sl-symbolspace-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-symbolspace-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-symbolcompositionalinfo-demo src/symbolcompositionalinfo-demo.cc)
add_dependencies(h2sl-symbolcompositionalinfo-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-symbolcompositionalinfo-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-symboldictionary-demo src/symboldictionary-demo.cc)
add_dependencies(h2sl-symboldictionary-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-symboldictionary-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-lexicon-generator src/lexicon-generator.cc)
add_dependencies(h2sl-lexicon-generator ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-lexicon-generator h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-pcfg-generator src/pcfg-generator.cc)
add_dependencies(h2sl-pcfg-generator ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-pcfg-generator h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-symbol-dictionary-generator src/symbol-dictionary-generator.cc)
add_dependencies(h2sl-symbol-dictionary-generator ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-symbol-dictionary-generator h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-feature-pool-generator src/feature-pool-generator.cc)
add_dependencies(h2sl-feature-pool-generator ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-feature-pool-generator h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-feature-pool-generator-cky src/feature-pool-generator-cky.cc)
add_dependencies(h2sl-feature-pool-generator-cky ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-feature-pool-generator-cky h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-feature-pool-generator-cky-sr src/feature-pool-generator-cky-sr.cc)
add_dependencies(h2sl-feature-pool-generator-cky-sr ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-feature-pool-generator-cky-sr h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-language-variable-demo src/language-variable-demo.cc)
add_dependencies(h2sl-language-variable-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-language-variable-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-pcfg-demo src/pcfg-demo.cc)
add_dependencies(h2sl-pcfg-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-pcfg-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-parser-demo src/parser-demo.cc)
add_dependencies(h2sl-parser-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-parser-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-sentence-demo src/sentence-demo.cc)
add_dependencies(h2sl-sentence-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-sentence-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-dialogue-demo src/dialogue-demo.cc)
add_dependencies(h2sl-dialogue-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-dialogue-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-unit-quaternion-demo src/unit-quaternion-demo.cc)
add_dependencies(h2sl-unit-quaternion-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-unit-quaternion-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-object-demo src/object-demo.cc)
add_dependencies(h2sl-object-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-object-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-object-probabilistic-demo src/object-probabilistic-demo.cc)
add_dependencies(h2sl-object-probabilistic-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-object-probabilistic-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-world-demo src/world-demo.cc)
add_dependencies(h2sl-world-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-world-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-world-probabilistic-demo src/world-probabilistic-demo.cc)
add_dependencies(h2sl-world-probabilistic-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-world-probabilistic-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-example-demo src/example-demo.cc)
add_dependencies(h2sl-example-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-example-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-dcg-demo src/dcg-demo.cc)
add_dependencies(h2sl-dcg-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-dcg-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-trained-dcg-demo src/trained-dcg-demo.cc)
add_dependencies(h2sl-trained-dcg-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-trained-dcg-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-llm-train src/llm-train.cc)
add_dependencies(h2sl-llm-train ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-llm-train h2sl ${LBFGS_LIBRARIES} ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-spatial-relations-demo src/spatial-relations-demo.cc)
add_dependencies(h2sl-spatial-relations-demo ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-spatial-relations-demo h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-dcg-server src/dcg-server.cc)
add_dependencies(h2sl-dcg-server ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-dcg-server h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-language-client src/language-client.cc)
add_dependencies(h2sl-language-client ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-language-client h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

add_executable(h2sl-parser-server src/parser-server.cc)
add_dependencies(h2sl-parser-server ${h2sl_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(h2sl-parser-server h2sl ${Boost_LIBRARIES} ${TinyXML2_LIBRARIES} ${JSONCPP_LIBRARIES} ${catkin_LIBRARIES})

#############
## Install ##
#############

## Mark executables for installation
install(TARGETS
  h2sl-word-demo
  h2sl-feature-demo
  h2sl-feature-set-demo
  h2sl-feature-pool-demo
  h2sl-symbol-demo
  h2sl-symbolspace-demo
  h2sl-symbolcompositionalinfo-demo
  h2sl-symboldictionary-demo
  h2sl-lexicon-generator
  h2sl-pcfg-generator
  h2sl-symbol-dictionary-generator
  h2sl-language-variable-demo
  h2sl-feature-pool-generator
  h2sl-feature-pool-generator-cky
  h2sl-feature-pool-generator-cky-sr
  h2sl-pcfg-demo
  h2sl-parser-demo
  h2sl-sentence-demo
  h2sl-dialogue-demo
  h2sl-unit-quaternion-demo
  h2sl-object-demo
  h2sl-object-probabilistic-demo
  h2sl-world-demo
  h2sl-world-probabilistic-demo
  h2sl-example-demo
  h2sl-dcg-demo
  h2sl-dcg-server
  h2sl-language-client
  h2sl-trained-dcg-demo
  h2sl-llm-train
  h2sl-spatial-relations-demo
  h2sl-parser-server
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark libraries for installation
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE
)

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )
