cmake_minimum_required(VERSION 3.10)
project(mavlink_control)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall -fPIC")

# 设置头文件目录
include_directories(
  third_party/mavlink/v2.0
  .
)

# if(FAKE_GPS)
#     add_definitions(-DFAKE_GPS)
# endif()

# if(APM_4.1.3)
#     add_definitions(-DAPM_4.1.3)
# endif()

# if(PX4)
#     add_definitions(-DPX4)
# endif()

# 设置源文件
set(SOURCES   
  port_mangement.cpp
  # integrated_interface.cpp
  serial_port.cpp
  udp_port.cpp
  autopilot_interface.cpp)


add_library(mavlink_control SHARED ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# 安装共享库到 /usr/local/lib
install(TARGETS mavlink_control
  LIBRARY DESTINATION /usr/local/lib
)

# 安装头文件到 /usr/include
#install(DIRECTORY include/ DESTINATION /usr/include)
# 安装所有头文件到 /usr/include
# install(DIRECTORY /home/lianran/c_uart_interface_mavlink/
#     DESTINATION /usr/include
#     FILES_MATCHING PATTERN "*.h")
# 安装指定头文件到 /usr/include
install(FILES 
    # /home/lianran/c_uart_interface_mavlink/autopilot_interface.h
    # /home/lianran/c_uart_interface_mavlink/generic_port.h
    # /home/lianran/c_uart_interface_mavlink/integrated_interface.h
    # DESTINATION /usr/include/c_uart_interface_mavlink

    ${CMAKE_SOURCE_DIR}/autopilot_interface.h
    ${CMAKE_SOURCE_DIR}/generic_port.h
    ${CMAKE_SOURCE_DIR}/port_mangement.h
    ${CMAKE_SOURCE_DIR}/serial_port.h
    ${CMAKE_SOURCE_DIR}/udp_port.h
    DESTINATION /usr/local/include/c_uart_interface_mavlink
    )

# 安装整个 third_party/mavlink 文件夹
install(DIRECTORY ${CMAKE_SOURCE_DIR}/third_party/mavlink/
DESTINATION /usr/local/include/mavlink
FILES_MATCHING PATTERN "*.h"
)

