# 添加可执行文件
add_executable(my_mavlink_control
  my_mavlink_control.cpp
)
add_executable(common_usage
  common_usage.cpp
)

# 链接库文件
target_link_libraries(my_mavlink_control
  mavlink_control
  pthread
)
target_link_libraries(common_usage
  mavlink_control
  pthread
)

