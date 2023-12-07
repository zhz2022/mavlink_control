cmake_minimum_required(VERSION 3.10)
project(mavlink_control)

set(CMAKE_CXX_STANDARD 11)

include_directories(
  third_party/mavlink/v2.0
  examples
  .
)

set(CMAKE_CXX_FLAGS "-g -Wall -fPIC")

add_library(mavlink_control SHARED
  port_mangement.cpp
  mode_selecter.cpp
  serial_port.cpp
  udp_port.cpp
  autopilot_interface.cpp
)

add_executable(my_mavlink_control
  examples/my_mavlink_control.cpp
)

add_executable(common_usage
  examples/common_usage.cpp
)

target_link_libraries(my_mavlink_control
  mavlink_control
  pthread
)

target_link_libraries(common_usage
  mavlink_control
  pthread
)

install(TARGETS mavlink_control
  LIBRARY DESTINATION /usr/lib
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)