cmake_minimum_required(VERSION 2.8.3)
project(srg_worldmodel)
    
## Use c++ 11x std
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
 
## Enable exception handling for segfaults using SigFault.h
set(CMAKE_CXX_FLAGS "-rdynamic -fnon-call-exceptions -ggdb ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS system_config event_handling alica_engine world_model asp_solver_wrapper asp_solver capnzero)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES srg_worldmodel
  CATKIN_DEPENDS system_config event_handling world_model asp_solver_wrapper asp_solver capnzero
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_library(srg_worldmodel
    src/SRGWorldModel.cpp
    src/SRGSimData.cpp
    src/ASPKnowledgeBase.cpp
    src/KnowledgeManager.cpp
    src/ConceptNet.cpp include/srg/wm/ConceptNet.h
)

target_link_libraries(srg_worldmodel
    ${catkin_LIBRARIES}
    srg_communication
)

add_library(srg_communication
    src/Communication.cpp
)

target_link_libraries(srg_communication
    system_config
#    srg_worldmodel
)