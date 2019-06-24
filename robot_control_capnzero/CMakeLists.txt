cmake_minimum_required(VERSION 2.8.3)
project(robot_control_capnzero)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS 
	rqt_gui
	rqt_gui_cpp
	alica_engine
	process_manager_capnzero
	system_util
	id_capnzero
	alica_client
	pm_control
	capnzero
)

find_package(Qt5Core REQUIRED)
get_target_property(Qt5Core_location Qt5::Core LOCATION)
find_package(Qt5Gui REQUIRED)
get_target_property(Qt5Gui_location Qt5::Gui LOCATION)
find_package(Qt5Network REQUIRED)
get_target_property(Qt5Network_location Qt5::Network LOCATION)
find_package(Qt5Widgets REQUIRED)
get_target_property(Qt5Widgets_location Qt5::Widgets LOCATION)
SET( QT_USE_QTXML TRUE )
SET( QT_WRAP_CPP TRUE )

set(robot_control_SRCS
  src/robot_control/flowlayout.cpp
  src/robot_control/RobotsControl.cpp
  src/robot_control/Robot.cpp
  src/robot_control/Communication.cpp
  src/robot_control/ContainerUtils.cpp
)

set(robot_control_HDRS
  include/robot_control/flowlayout.h
  include/robot_control/RobotsControl.h
  include/robot_control/Robot.h
  include/robot_control/Communication.h
)

set(robot_control_UIS
  ui/RobotsControl.ui
  ui/ControlledRobot.ui  
)

set(robot_control_INCLUDE_DIRECTORIES
  include
  ${Qt5Core_INCLUDE_DIRS} 
  ${Qt5Gui_INCLUDE_DIRS} 
  ${Qt5Network_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
  "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}"
)
if(NOT EXISTS "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}")
  file(MAKE_DIRECTORY "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}")
endif()

include_directories(
  ${robot_control_INCLUDE_DIRECTORIES}
  ${catkin_INCLUDE_DIRS}
)

set(robot_control_RCS
  resources/resources.qrc
)

catkin_package(
  INCLUDE_DIRS ${robot_control_INCLUDE_DIRECTORIES}
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS rqt_gui rqt_gui_cpp id_capnzero alica_msgs
)

#### Robot Control MESSAGES
find_package(CapnProto REQUIRED)
set(CAPNPC_IMPORT_DIRS "${id_capnzero_SOURCE_PREFIX}/include")# include external capnp message schema files
set(capnp_msg_folder "${CMAKE_CURRENT_SOURCE_DIR}/include/robot_control")
file(GLOB_RECURSE capnp_messages ${capnp_msg_folder} *.capnp)
set(CAPNPC_SRC_PREFIX ${capnp_msg_folder})
set(CAPNPC_OUTPUT_DIR ${capnp_msg_folder})
capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${capnp_messages})

add_library(rc_msgs
	src/robot_control/ContainerUtils.cpp
	${CAPNP_SRCS}
)

#### RQT GUI LIBRARY
QT5_WRAP_CPP(robot_control_MOCS ${robot_control_HDRS})
QT5_ADD_RESOURCES(robot_control_RESOURCES ${robot_control_RCS}) 
# ensure generated header files are being created in the devel space
set(_cmake_current_binary_dir "${CMAKE_CURRENT_BINARY_DIR}")
set(CMAKE_CURRENT_BINARY_DIR "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}")
QT5_WRAP_UI(robot_control_UIS_H ${robot_control_UIS})
set(CMAKE_CURRENT_BINARY_DIR "${_cmake_current_binary_dir}")

add_library(${PROJECT_NAME}
	${robot_control_SRCS}
	${robot_control_MOCS}
	${robot_control_UIS_H}
	${robot_control_RESOURCES}
)
target_link_libraries(${PROJECT_NAME}
	${catkin_LIBRARIES}
	${Qt5Core_location}
	${Qt5Gui_location}
	${Qt5Network_location}
	rc_msgs
)

find_package(class_loader)
class_loader_hide_library_symbols(${PROJECT_NAME})

install(FILES plugin.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

install(PROGRAMS scripts/robot_control
  DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)
install(PROGRAMS scripts/robot_control
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
