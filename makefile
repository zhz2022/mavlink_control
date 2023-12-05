all: my_mavlink_control common_usage
#all: mavlink_control my_mavlink_control 

mavlink_control: mavlink_control.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp
	g++ -g -Wall -I mavlink/include/mavlink/v2.0 mavlink_control.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp -o mavlink_control -lpthread

my_mavlink_control: ./examples/my_mavlink_control.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp
	g++ -g -Wall -I third_party/mavlink/v2.0 -I examples -I . ./examples/my_mavlink_control.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp -o my_mavlink_control -lpthread

common_usage: ./examples/common_usage.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp
	g++ -g -Wall -I third_party/mavlink/v2.0 -I examples -I . ./examples/common_usage.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp -o my_mavlink_control -lpthread

git_submodule:
	git submodule update --init --recursive

clean:
	 rm -rf *o *mavlink_control *common_usage

