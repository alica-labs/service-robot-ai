cmake_minimum_required(VERSION 2.8.3)
project(srg_robot)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED srg_worldmodel capnzero grid_sim id_capnzero)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES srg_robot
  CATKIN_DEPENDS srg_worldmodel capnzero id_capnzero
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
  src/Robot.cpp
  src/Path.cpp
  src/Movement.cpp
)

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)
