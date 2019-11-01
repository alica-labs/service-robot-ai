cmake_minimum_required(VERSION 3.10.2) 
project(srg_base)

add_compile_options(-std=c++11)
set(CMAKE_BUILD_TYPE Debug)

find_package(catkin REQUIRED COMPONENTS srg_worldmodel alica_engine srg_expressions alica_capnzero_proxy asp_solver_wrapper asp_solver)
find_package(CapnProto REQUIRED)

catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES srg-base
  CATKIN_DEPENDS srg_worldmodel alica_engine srg_expressions alica_capnzero_proxy  asp_solver_wrapper asp_solver
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
  src/Base.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${catkin_LIBRARIES}
    ${CAPNP_LIBRARIES}
    clingo
)

target_compile_definitions(${PROJECT_NAME} PUBLIC ZMQ_BUILD_DRAFT_API=1)
