cmake_minimum_required(VERSION 2.8.3)
project(speech_act)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED capnzero srg_msgs id_manager event_handling system_config srg_worldmodel)

find_package(Threads)
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS capnzero srg_msgs id_manager event_handling system_config srg_worldmodel
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
  ${CMAKE_THREAD_LIBS_INIT}
)