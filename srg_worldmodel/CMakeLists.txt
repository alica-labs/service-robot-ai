cmake_minimum_required(VERSION 2.8.3)
project(srg_worldmodel)
    
## Use c++ 11x std
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
 
## Enable exception handling for segfaults using SigFault.h
set(CMAKE_CXX_FLAGS "-rdynamic -fnon-call-exceptions -ggdb ${CMAKE_CXX_FLAGS}")

##Yaml
find_package(yaml-cpp REQUIRED)
find_package(catkin REQUIRED COMPONENTS system_config event_handling alica_engine world_model asp_solver_wrapper asp_solver telegram-client id_manager knowledge_manager control_panel grid_sim speech_act)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(gvc REQUIRED libgvc libcgraph libcdt)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES srg_worldmodel
  CATKIN_DEPENDS system_config event_handling world_model asp_solver_wrapper asp_solver telegram-client id_manager knowledge_manager control_panel grid_sim speech_act
)

include_directories(include
  ${catkin_INCLUDE_DIRS}
  ${YAML_CPP_INCLUDE_DIR}
  ${gvc_INCLUDE_DIRS}
)

add_library(srg_worldmodel
  src/SRGWorldModel.cpp
  src/wm/SRGSimData.cpp
  src/conceptnet/ConceptNet.cpp
  src/conceptnet/Edge.cpp
  src/conceptnet/Concept.cpp
  src/conceptnet/EdgeTranslator.cpp
  src/conceptnet/ConceptPath.cpp
  src/dialogue/AnswerGraph.cpp
        src/wm/RawSensorData.cpp
  src/wm/Communication.cpp
  src/dialogue/BasicHumanNeeds.cpp
  src/dialogue/DialogueManager.cpp
  src/dialogue/InformHandler.cpp
  src/dialogue/CommandHandler.cpp
  src/asp/ASPTranslator.cpp
)

target_link_libraries(srg_worldmodel
  ${catkin_LIBRARIES}
  ${YAML_CPP_LIBRARIES}
  curl
  ${gvc_LIBRARIES}
)