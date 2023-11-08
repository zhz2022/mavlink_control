#all: git_submodule mavlink_control
all: my_mavlink_control udp_example

mavlink_control: mavlink_control.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp
	g++ -g -Wall -I mavlink/include/mavlink/v2.0 mavlink_control.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp -o mavlink_control -lpthread

my_mavlink_control: my_mavlink_control.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp
	g++ -g -Wall -I mavlink/include/mavlink/v2.0 my_mavlink_control.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp -o my_mavlink_control -lpthread
udp_example: udp_example.c
	g++ -g -Wall -I mavlink/include/mavlink/v2.0 udp_example.c -o udp_example -lpthread

git_submodule:
	git submodule update --init --recursive

clean:
	 rm -rf *o *mavlink_control
