/**
 * @file mavlink_control.cpp
 *
 * @brief offboard control process via mavlink
 *
 * This process connects an external MAVLink UART device to send an receive data
 * 
 * @author zhangpeng, <zhangpengbjut@sina.com>
 *
 */

// ------------------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------------------

#include "my_mavlink_control.h"

int gl_mode_select = 0;
enum Mode {INIT=1,TAKEOFF,MOVE,LAND,QUIT,RTL,\
TAKEOFF_LOCAL,WAYPOINT,SET_GUIDED,SET_AUTO,PRINT_MSG,MOVE_UP,MOVE_DOWN,CIRCLE};

// ------------------------------------------------------------------------------
//   Main
// ------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    Generic_Port *port;

    char *uart_name = (char *)"/dev/ttyUSB0";
    int baudrate = 57600;
    bool use_udp = false;
    char *udp_ip = (char *)"127.0.0.1";
    int udp_port = 14550;
    // do the parse, will throw an int if it fails
    parse_commandline(argc, argv, uart_name, baudrate, use_udp, udp_ip, udp_port);
    
    if (use_udp)
    {
        port = new UDP_Port(udp_ip, udp_port);
    }
    else
    {
        port = new Serial_Port(uart_name, baudrate);
    }

    Autopilot_Interface autopilot_interface(port);

    port_quit = port;
    autopilot_interface_quit = &autopilot_interface;
    signal(SIGINT, quit_handler );
    port->start();

    while(1){
        usleep(100);
        switch (gl_mode_select){
            case INIT:
                mode_init(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case MOVE:
                autopilot_interface.set_velocity(1,0,0);//plus down minus up
                usleep(100); // give some time to let it sink in
                gl_mode_select = mode_selecter();
                break;
            case LAND:
                autopilot_interface.land();
                usleep(100); // give some time to let it sink in
                gl_mode_select = mode_selecter();
                break;
            case QUIT:
                mode_quit(autopilot_interface,port);
                gl_mode_select = mode_selecter();
                break;
            case RTL:
                mode_rtl(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case TAKEOFF_LOCAL:
                mode_takeoff_local(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case WAYPOINT:
                waypoint(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case PRINT_MSG:
                print_msg_test(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case CIRCLE:
                mode_circle(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            default :
                std::cout << "无效的模式选择" << std::endl;
                gl_mode_select = mode_selecter();
        }
    }
    return 0;
}

int mode_selecter()
{
    int mode;
    std::cout << "请选择模式(输入number)"                 << std::endl;
    std::cout << "1:init           2:takeoff"            << std::endl;
    std::cout << "3:forward        4:move_backward"      << std::endl;
    std::cout << "5:move_left      6:move_right"         << std::endl;
    std::cout << "7:stop           8:land"               << std::endl;
    std::cout << "9:quit           10:return to launch"  << std::endl;
    std::cout << "11:takeoff_local 12:waypoint        "  << std::endl;
    std::cout << "13:set guided    14:set auto        "  << std::endl;
    std::cout << "15:print_msgs    16:move_up         "  << std::endl;
    std::cout << "17:move_down     18:set circle         "  << std::endl;
    std::cout << "===================================="  << std::endl;
    std::cin  >> mode;
    std::cout << ":mode selected to: "<< mode            << std::endl;
    std::cout << "===================================="  << std::endl;
    return mode;
}
void mode_init(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_init started" << std::endl;
    autopilot_interface.start();

    // autopilot_interface.enable_offboard_control();
	usleep(100); // give some time to let it sink in

	// now the autopilot is accepting setpoint commands

    autopilot_interface.arm_disarm(true);
    usleep(100); // give some time to let it sink in

	printf("SEND OFFBOARD COMMANDS\n");
}
void mode_land(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_land started" << std::endl;
    // land
    autopilot_interface.land();
    usleep(100); // give some time to let it sink in
}
void mode_quit(Autopilot_Interface &autopilot_interface, Generic_Port *port){
    std::cout << "mode_quit started" << std::endl;
    // disarm autopilot
    autopilot_interface.arm_disarm(false);
    std::cout << "mode_quit started disarm" << std::endl;
    usleep(100); // give some time to let it sink in

    autopilot_interface.disable_offboard_control();

    autopilot_interface.stop();
    port->stop();
    delete port;
}
void mode_rtl(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_rtl started" << std::endl;
    // return to launch
    autopilot_interface.return_to_launch();
    usleep(100); // give some time to let it sink in
}
void mode_takeoff_local(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_takeoff_local started" << std::endl;
    // takeoff local(0,0,40)
    autopilot_interface.takeoff_local();
    usleep(100); // give some time to let it sink in
}
void waypoint(Autopilot_Interface &autopilot_interface){
    std::cout << "enable_offboard_control started" << std::endl;
    // return to launch
    autopilot_interface.waypoint();
    usleep(100); // give some time to let it sink in
}
void mode_circle(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_circle started" << std::endl;
    // circle
    autopilot_interface.circle();
    usleep(100); // give some time to let it sink in
}
void print_msg_test(Autopilot_Interface &autopilot_interface){
    std::cout << "set_velocity_test started" << std::endl;
	// Wait for 4 seconds, check position
	for (int i=0; i < 4; i++)
	{
		// mavlink_local_position_ned_t pos = autopilot_interface.current_messages.local_position_ned;
        Mavlink_Messages msgs = autopilot_interface.current_messages;
        std::cout << "Current position: " << msgs.local_position_ned.x << " " << msgs.local_position_ned.y << " " << msgs.local_position_ned.z << std::endl;
        std::cout << "Current velocity: " << msgs.local_position_ned.vx << " " << msgs.local_position_ned.vy << " " << msgs.local_position_ned.vz << std::endl;
        std::cout << "Current pose: " << msgs.attitude.roll << " " << msgs.attitude.pitch << " " << msgs.attitude.yaw << " " << std::endl;
        std::cout << "Current globally_set_position_ned: " << msgs.global_position_int.lat << " " << msgs.global_position_int.lon << " " << msgs.global_position_int.alt << msgs.global_position_int.relative_alt << std::endl;
        std::cout << "Current globally_set_velocity_ned: " << msgs.global_position_int.vx << " " << msgs.global_position_int.vy << " " << msgs.global_position_int.vz << msgs.global_position_int.hdg << std::endl;
        std::cout << "Current battery_voltage: " << msgs.sys_status.voltage_battery << std::endl;
		sleep(1);
	}
}
