cmake_minimum_required(VERSION 2.8.3)
project(pm_control_capnzero)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC ${CMAKE_CXX_FLAGS}")
add_compile_options(-std=c++11)

# Load catkin and all dependencies required for this package
find_package(catkin REQUIRED COMPONENTS process_manager_capnzero id_capnzero capnzero)

find_package(Qt5Core REQUIRED)
get_target_property(Qt5Core_location Qt5::Core LOCATION)
find_package(Qt5Gui REQUIRED)
get_target_property(Qt5Gui_location Qt5::Gui LOCATION)
find_package(Qt5OpenGL REQUIRED)
#get_target_property(Qt5OpenGL_location Qt5::OpenGL LOCATION)
#find_package(Qt5Network REQUIRED)
#get_target_property(Qt5Network_location Qt5::Network LOCATION)
#find_package(Qt5Widgets REQUIRED)
get_target_property(Qt5Widgets_location Qt5::Widgets LOCATION)
#SET( QT_USE_QTXML TRUE )
SET( QT_WRAP_CPP TRUE )

set(pm_control_INCLUDE_DIRECTORIES
  include
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Core_INCLUDE_DIRS}
  #${Qt5OpenGL_INCLUDE_DIRS}
  #${Qt5Network_INCLUDE_DIRS}
  ${Qt5Gui_INCLUDE_DIRS}
  "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}"
)
if(NOT EXISTS "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}")
  file(MAKE_DIRECTORY "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}")
endif()

include_directories(
  ${pm_control_INCLUDE_DIRECTORIES}
  ${catkin_INCLUDE_DIRS}
)

catkin_package(
  INCLUDE_DIRS ${pm_control_INCLUDE_DIRECTORIES}
  #LIBRARIES
  CATKIN_DEPENDS process_manager_capnzero id_capnzero capnzero
)

# PROCESS MANAGER CONTROL
set(pm_control_SRCS
  src/pm_control/PMControl.cpp
  src/pm_control/Communication.cpp
  src/pm_control/ControlledExecutable.cpp
  src/pm_control/ControlledProcessManager.cpp
  src/pm_control/ControlledRobot.cpp
)

set(pm_control_HDRS
  include/pm_control/PMControl.h
  include/pm_control/ControlledExecutable.h
  include/pm_control/ControlledProcessManager.h
  include/pm_control/ControlledRobot.h
)

set(pm_control_UIS
  src/pm_control/PMControl.ui
  src/pm_control/ProcessWidget.ui
  src/pm_control/RobotProcessesWidget.ui
)

QT5_WRAP_CPP(pm_control_MOCS ${pm_control_HDRS})
# ensure generated header files are being created in the devel space
set(_cmake_current_binary_dir "${CMAKE_CURRENT_BINARY_DIR}")
set(CMAKE_CURRENT_BINARY_DIR "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}")
QT5_WRAP_UI(pm_control_UIS_H ${pm_control_UIS})
set(CMAKE_CURRENT_BINARY_DIR "${_cmake_current_binary_dir}")

add_executable(${PROJECT_NAME} ${pm_control_SRCS} ${pm_control_MOCS} ${pm_control_UIS_H})
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${Qt5Core_location} ${Qt5Gui_location} ${Qt5Network_location} ${Qt5OpenGL_location} ${Qt5Widgets_location} PMWidget)
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)