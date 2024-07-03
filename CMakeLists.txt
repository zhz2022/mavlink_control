cmake_minimum_required(VERSION 3.10)
project(mavlink_control)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall -fPIC")

include_directories(
  third_party/mavlink/v2.0
  examples
  .
)

if(FAKE_GPS)
    add_definitions(-DFAKE_GPS)
endif()

if(APM_4.1.3)
    add_definitions(-DAPM_4.1.3)
endif()

if(PX4)
    add_definitions(-DPX4)
endif()


add_library(mavlink_control SHARED
  port_mangement.cpp
  integrated_interface.cpp
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