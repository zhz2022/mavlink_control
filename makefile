CC := g++
CFLAGS := -g -Wall -fPIC -I third_party/mavlink/v2.0 -I examples -I .
LDFLAGS := -shared -lpthread 
# 库文件
DLIBS = -lmavlink_control -lpthread

all: libmavlink_control.so my_mavlink_control common_usage

libmavlink_control.so: port_mangement.o mode_selecter.o serial_port.o udp_port.o autopilot_interface.o
	$(CC) $(LDFLAGS) $^ -o $@

my_mavlink_control: ./examples/my_mavlink_control.cpp 
	$(CC) $(CFLAGS) -L. $< -o $@ $(DLIBS)

common_usage: ./examples/common_usage.cpp 
	$(CC) $(CFLAGS) -L. $< -o $@ $(DLIBS)

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

install: libmavlink_control.so
	install -d /usr/lib
	install libmavlink_control.so /usr/lib

clean:
	rm -f *.so *.o my_mavlink_control common_usage

# all: my_mavlink_control common_usage

# my_mavlink_control: ./examples/my_mavlink_control.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp
# 	g++ -g -Wall -I third_party/mavlink/v2.0 -I examples -I . ./examples/my_mavlink_control.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp -o my_mavlink_control -lpthread

# common_usage: ./examples/common_usage.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp
# 	g++ -g -Wall -I third_party/mavlink/v2.0 -I examples -I . ./examples/common_usage.cpp port_mangement.cpp mode_selecter.cpp serial_port.cpp udp_port.cpp autopilot_interface.cpp -o common_usage -lpthread

# git_submodule:
# 	git submodule update --init --recursive

# clean:
# 	 rm -rf *o *mavlink_control *common_usage

