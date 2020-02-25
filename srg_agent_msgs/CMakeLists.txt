cmake_minimum_required(VERSION 2.8.3)
project(srg_agent_msgs)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED capnzero id_capnzero id_manager grid_sim)

catkin_package(
        INCLUDE_DIRS include
        LIBRARIES srg_agent_msgs
        CATKIN_DEPENDS capnzero id_capnzero grid_sim
        #  DEPENDS system_lib
)

include_directories(
        include
        ${catkin_INCLUDE_DIRS}
)

#### AGENT MESSAGES
find_package(CapnProto REQUIRED)
# include external capnp message schema files
set(CAPNPC_IMPORT_DIRS
        "${id_capnzero_SOURCE_PREFIX}/include"
        "${grid_sim_SOURCE_PREFIX}/include"
)
set(capnp_msg_folder "${CMAKE_CURRENT_SOURCE_DIR}/include/srg/agent")
file(GLOB_RECURSE capnp_messages ${capnp_msg_folder} *.capnp)
set(CAPNPC_SRC_PREFIX ${capnp_msg_folder})
set(CAPNPC_OUTPUT_DIR ${capnp_msg_folder})
capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${capnp_messages})

add_library(${PROJECT_NAME}
    src/ContainerUtils.cpp
    ${CAPNP_SRCS}
        src/SpeechAct.cpp)
target_link_libraries(${PROJECT_NAME}
    ${catkin_LIBRARIES}
)
