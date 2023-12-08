cmake_minimum_required(VERSION 3.10)
project(mavlink_control)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall -fPIC")

include_directories(
  third_party/mavlink/v2.0
  examples
  .
)

add_library(mavlink_control SHARED
  port_mangement.cpp
  mode_selecter.cpp
  serial_port.cpp
  udp_port.cpp
  autopilot_interface.cpp
)

add_subdirectory(examples)

install(TARGETS mavlink_control
  LIBRARY DESTINATION /usr/lib
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)