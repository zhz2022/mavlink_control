# 添加可执行文件
add_executable(mavlink_controller_copter
  mavlink_controller_copter.cpp
)

# 链接库文件
target_link_libraries(mavlink_controller_copter
  mavlink_control_arducopter
  mavlink_control
  pthread
)


# target_link_libraries(mavlink_control_rover
#   mavlink_control
#   pthread
# )

