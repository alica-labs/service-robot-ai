cmake_minimum_required(VERSION 2.8.3)
project(srg_worldmodel)
    
## Use c++ 11x std
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
 
## Enable exception handling for segfaults using SigFault.h
set(CMAKE_CXX_FLAGS "-rdynamic -fnon-call-exceptions -ggdb ${CMAKE_CXX_FLAGS}")

##Yaml
find_package(yaml-cpp REQUIRED)

find_package(catkin REQUIRED COMPONENTS system_config event_handling alica_engine world_model asp_solver_wrapper asp_solver telegram-client id_manager)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES srg_worldmodel
  CATKIN_DEPENDS system_config event_handling world_model asp_solver_wrapper asp_solver telegram-client id_manager
)

include_directories(include ${catkin_INCLUDE_DIRS} ${YAML_CPP_INCLUDE_DIR})

add_library(srg_worldmodel
        src/SRGWorldModel.cpp
        src/SRGSimData.cpp
        src/ASPKnowledgeBase.cpp
        src/KnowledgeManager.cpp
        src/ConceptNet.cpp
        src/Edge.cpp
        src/Concept.cpp
        src/EdgeTranslator.cpp
        src/RawSensorData.cpp
)

target_link_libraries(srg_worldmodel
  ${catkin_LIBRARIES}
  ${YAML_CPP_LIBRARIES}
  curl
)