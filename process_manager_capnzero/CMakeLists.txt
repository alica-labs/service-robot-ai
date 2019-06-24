cmake_minimum_required(VERSION 2.8.3)
project(process_manager_capnzero)

add_compile_options(-std=c++11)

find_package(Threads)
find_package(catkin REQUIRED id_manager system_config system_util id_capnzero capnzero roscpp)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES robot_executable_meta_data pm_msgs
  CATKIN_DEPENDS id_manager system_config system_util id_capnzero
)

include_directories(include ${catkin_INCLUDE_DIRS})

#### Process Manager MESSAGES
find_package(CapnProto REQUIRED)
set(CAPNPC_IMPORT_DIRS "${id_capnzero_SOURCE_PREFIX}/include")# include external capnp message schema files
set(capnp_msg_folder "${CMAKE_CURRENT_SOURCE_DIR}/include/process_manager")
file(GLOB_RECURSE capnp_messages ${capnp_msg_folder} *.capnp)
set(CAPNPC_SRC_PREFIX ${capnp_msg_folder})
set(CAPNPC_OUTPUT_DIR ${capnp_msg_folder})
capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${capnp_messages})

add_library(pm_msgs
	src/ContainerUtils.cpp
	${CAPNP_SRCS}
)

add_library(robot_executable_meta_data
  src/RobotMetaData.cpp
  src/ExecutableMetaData.cpp
  src/RobotExecutableRegistry.cpp
)
target_link_libraries(robot_executable_meta_data ${catkin_LIBRARIES})

add_executable(process_manager 
  src/ProcessManager.cpp
  src/ManagedExecutable.cpp
  src/ManagedRobot.cpp
  src/Communication.cpp
)
target_link_libraries(process_manager
	${catkin_LIBRARIES}
	${CAPNP_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	robot_executable_meta_data
	pm_msgs
)
