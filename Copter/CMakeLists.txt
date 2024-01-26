add_library(mavlink_control_arducopter SHARED
  integrated_interface.cpp
)

# 链接库文件
target_link_libraries(mavlink_control_arducopter
  mavlink_control
  pthread
)

install(TARGETS mavlink_control_arducopter
  LIBRARY DESTINATION /usr/lib
)