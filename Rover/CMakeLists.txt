add_library(mavlink_control_ardurover SHARED
  integrated_interface.cpp
)

# 链接库文件
target_link_libraries(mavlink_control_ardurover
  mavlink_control
  pthread
)

install(TARGETS mavlink_control_ardurover
  LIBRARY DESTINATION /usr/lib
)