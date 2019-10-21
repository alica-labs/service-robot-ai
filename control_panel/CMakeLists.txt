cmake_minimum_required(VERSION 2.8.3)
project(control_panel)

add_compile_options(-std=c++11)

set(CMAKE_BUILD_TYPE Debug)

find_package(Threads)
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
find_package(catkin REQUIRED process_manager_capnzero capnzero alica_engine id_manager id_capnzero alica_capnzero_proxy)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES control_msgs
  CATKIN_DEPENDS process_manager_capnzero capnzero alica_engine id_manager id_capnzero alica_capnzero_proxy
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Gui_INCLUDE_DIRS}
  "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}"
)
if(NOT EXISTS "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}")
  file(MAKE_DIRECTORY "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}")
endif()

#### CONTROL MESSAGES
find_package(CapnProto REQUIRED)
set(CAPNPC_IMPORT_DIRS "${id_capnzero_SOURCE_PREFIX}/include")# include external capnp message schema files
set(capnp_msg_folder "${CMAKE_CURRENT_SOURCE_DIR}/include/control")
file(GLOB_RECURSE capnp_messages ${capnp_msg_folder} *.capnp)
set(CAPNPC_SRC_PREFIX ${capnp_msg_folder})
set(CAPNPC_OUTPUT_DIR ${capnp_msg_folder})
capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${capnp_messages})

add_library(control_msgs
  src/ContainerUtils.cpp
  ${CAPNP_SRCS}
)
target_link_libraries(control_msgs
  ${catkin_LIBRARIES}
)

#### CONTROL
QT5_WRAP_CPP(control_MOCS
        include/control/ControlPanel.h
        include/control/Process.h
        include/control/Agent.h
        include/control/Talker.h
)
# ensure generated header files are being created in the devel space
set(_cmake_current_binary_dir "${CMAKE_CURRENT_BINARY_DIR}")
set(CMAKE_CURRENT_BINARY_DIR "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}")
QT5_WRAP_UI(control_UIS ui/Agent.ui ui/Process.ui ui/ControlPanel.ui)
set(CMAKE_CURRENT_BINARY_DIR "${_cmake_current_binary_dir}")
add_executable(${PROJECT_NAME}
  src/ControlPanel.cpp
  src/Communication.cpp
  src/Agent.cpp
  src/Process.cpp
  src/Talker.cpp
  src/ExecutableRegistry.cpp
  ${control_MOCS}
  ${control_UIS}
)

message("######################################################## catkin_LIBRARIES: ${catkin_LIBRARIES}")

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  ${Qt5Core_LIBRARIES}
  ${Qt5Widgets_LIBRARIES}
  ${Qt5Gui_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  control_msgs
)
