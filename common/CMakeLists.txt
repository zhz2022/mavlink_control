
add_library(mavlink_control SHARED
  port_mangement.cpp
  serial_port.cpp
  udp_port.cpp
)

install(TARGETS mavlink_control
  LIBRARY DESTINATION /usr/lib
)