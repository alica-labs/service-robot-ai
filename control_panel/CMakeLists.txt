cmake_minimum_required(VERSION 2.8.3)
project(control_panel)

add_compile_options(-std=c++11)

set(CMAKE_BUILD_TYPE Debug)

find_package(Threads)
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
find_package(catkin REQUIRED process_manager_capnzero capnzero alica_engine id_manager id_capnzero alica_capnzero_proxy srg_agent_msgs srg_agent)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES control_msgs
  CATKIN_DEPENDS process_manager_capnzero capnzero alica_engine id_manager id_capnzero alica_capnzero_proxy srg_agent_msgs srg_agent
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

#### CONTROL PANEL
QT5_WRAP_CPP(control_MOCS
  include/control/ControlPanel.h
  include/control/Process.h
  include/control/Agent.h
  include/control/Talker.h
)

QT5_ADD_RESOURCES(control_RESOURCES
  resources/resources.qrc
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
  ${control_RESOURCES}
)

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  ${Qt5Core_LIBRARIES}
  ${Qt5Widgets_LIBRARIES}
  ${Qt5Gui_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)
