
add_library(mavlink_control SHARED
  port_mangement.cpp
  integrated_interface.cpp
  serial_port.cpp
  udp_port.cpp
  autopilot_interface.cpp
)

