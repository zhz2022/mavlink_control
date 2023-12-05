makefile
CC := g++
CFLAGS := -g -Wall -I third_party/mavlink/v2.0 -I examples -I .
LDFLAGS := -shared

# 编译common_usage的依赖为.so共享库
mavlink_control.so: port_mangement.o mode_selecter.o serial_port.o udp_port.o autopilot_interface.o
	$(CC) $(LDFLAGS) $^ -o $@

# 编译每个源文件为目标文件
common_usage.o: ./examples/common_usage.cpp
	$(CC) $(CFLAGS) -c $< -o $@

port_mangement.o: port_mangement.cpp
	$(CC) $(CFLAGS) -c $< -o $@

mode_selecter.o: mode_selecter.cpp
	$(CC) $(CFLAGS) -c $< -o $@

serial_port.o: serial_port.cpp
	$(CC) $(CFLAGS) -c $< -o $@

udp_port.o: udp_port.cpp
	$(CC) $(CFLAGS) -c $< -o $@

autopilot_interface.o: autopilot_interface.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f *.so *.o

# all: my_mavlink_control common_usage
# #all: mavlink_control my_mavlink_control 

# mavlink_control: mavlink_control.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp
# 	g++ -g -Wall -I mavlink/include/mavlink/v2.0 mavlink_control.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp -o mavlink_control -lpthread

# my_mavlink_control: ./examples/my_mavlink_control.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp
# 	g++ -g -Wall -I third_party/mavlink/v2.0 -I examples -I . ./examples/my_mavlink_control.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp -o my_mavlink_control -lpthread

# common_usage: ./examples/common_usage.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp
# 	g++ -g -Wall -I third_party/mavlink/v2.0 -I examples -I . ./examples/common_usage.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp -o common_usage -lpthread

# git_submodule:
# 	git submodule update --init --recursive

# clean:
# 	 rm -rf *o *mavlink_control *common_usage

