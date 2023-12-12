# 添加可执行文件
add_executable(mavlink_control_copter
  mavlink_control_copter.cpp
)
add_executable(common_usage
  common_usage.cpp
)

# 链接库文件
target_link_libraries(mavlink_control_copter
  mavlink_control
  pthread
)
target_link_libraries(common_usage
  mavlink_control
  pthread
)

