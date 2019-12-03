cmake_minimum_required(VERSION 2.8.3)
project(srg_agent)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED srg_agent_msgs srg_worldmodel capnzero grid_sim id_manager)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES srg_agent
  CATKIN_DEPENDS srg_agent_msgs srg_worldmodel capnzero id_manager
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

#### SRG AGENT LIBRARY
add_library(${PROJECT_NAME}
  src/Agent.cpp
  src/Path.cpp
  src/Movement.cpp
  src/ObjectSearch.cpp
  src/SearchCellSorter.cpp
  src/SearchCell.cpp
  src/Voice.cpp
)

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)
