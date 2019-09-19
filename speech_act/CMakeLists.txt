cmake_minimum_required(VERSION 2.8.3)
project(speech_act)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED capnzero id_manager event_handling system_config id_capnzero)

find_package(Threads)
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}_msgs
  CATKIN_DEPENDS capnzero id_manager event_handling system_config id_capnzero
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
  src/Talker.cpp
  ${CAPNP_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  ${PROJECT_NAME}_msgs
  ${CMAKE_THREAD_LIBS_INIT}
)

#### SpeechActMsg MESSAGES
find_package(CapnProto REQUIRED)
set(CAPNPC_IMPORT_DIRS "${id_capnzero_SOURCE_PREFIX}/include")# include external capnp message schema files
set(${PROJECT_NAME}_msgs "${CMAKE_CURRENT_SOURCE_DIR}/include/srg")
file(GLOB_RECURSE capnp_messages ${${PROJECT_NAME}_msgs} *.capnp)
set(CAPNPC_SRC_PREFIX ${${PROJECT_NAME}_msgs})
set(CAPNPC_OUTPUT_DIR ${${PROJECT_NAME}_msgs})
capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${capnp_messages})

add_library(${PROJECT_NAME}_msgs
  src/containers/ContainerUtils.cpp
  ${CAPNP_SRCS}
  ${CAPNP_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME}_msgs
  ${catkin_LIBRARIES}
)