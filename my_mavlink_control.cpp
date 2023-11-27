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
enum Mode {INIT=1,TAKEOFF,MOVE_FORWARD,MOVE_BACKWARD,MOVE_LEFT,MOVE_RIGHT,STOP,LAND,QUIT,RTL,\
TAKEOFF_LOCAL,WAYPOINT,SET_GUIDED,SET_AUTO,SET_VEL};

// ------------------------------------------------------------------------------
//   Main
// ------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    char *uart_name = (char *)"/dev/ttyUSB0";
    int baudrate = 57600;

    bool use_udp = false;
    char *udp_ip = (char *)"127.0.0.1";
    int udp_port = 14550;
    // do the parse, will throw an int if it fails
    parse_commandline(argc, argv, uart_name, baudrate, use_udp, udp_ip, udp_port);
    Generic_Port *port;
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
    signal(SIGINT, quit_handler);

    port->start();
    // autopilot_interface.start();

    // autopilot_interface.enable_offboard_control();
	// usleep(100); // give some time to let it sink in

	// // now the autopilot is accepting setpoint commands

    // autopilot_interface.arm_disarm(true);
    // usleep(100); // give some time to let it sink in

	// printf("SEND OFFBOARD COMMANDS\n");

	// initialize command data strtuctures
	mavlink_set_position_target_local_ned_t sp;
	mavlink_set_position_target_local_ned_t ip = autopilot_interface.initial_position;


    while(1){
        usleep(100);
        switch (gl_mode_select){
            case INIT:
                mode_init(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case TAKEOFF:
                mode_takeoff(autopilot_interface,ip,sp);
                gl_mode_select = mode_selecter();
                break;
            case MOVE_FORWARD:
                mode_move_forward(autopilot_interface,sp);
                gl_mode_select = mode_selecter();
                break;
            case MOVE_BACKWARD:
                mode_move_backward(autopilot_interface,sp);
                gl_mode_select = mode_selecter();
                break;
            case MOVE_LEFT:
                mode_move_left(autopilot_interface,sp);
                gl_mode_select = mode_selecter();
                break;
            case MOVE_RIGHT:
                mode_move_right(autopilot_interface,sp);
                gl_mode_select = mode_selecter();
                break;
            case STOP:
                mode_stop(autopilot_interface,sp);
                gl_mode_select = mode_selecter();
                break;
            case LAND:
                mode_land(autopilot_interface);
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
            case SET_GUIDED:
                set_guided(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case SET_AUTO:
                set_auto(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case SET_VEL:
                set_velocity_test(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            default :
                std::cout << "无效的模式选择" << std::endl;
                gl_mode_select = mode_selecter();
        }
    }
    return 0;
}

// ------------------------------------------------------------------------------
//   Parse Command Line
// ------------------------------------------------------------------------------
// throws EXIT_FAILURE if could not open the port
void parse_commandline(int argc, char **argv, char *&uart_name, int &baudrate,
                       bool &use_udp, char *&udp_ip, int &udp_port)
{

    // string for command line usage
    const char *commandline_usage = "usage: mavlink_control [-d <devicename> -b <baudrate>] [-u <udp_ip> -p <udp_port>] [-a ]";

    // Read input arguments
    for (int i = 1; i < argc; i++)
    { // argv[0] is "mavlink"

        // Help
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("%s\n", commandline_usage);
            throw EXIT_FAILURE;
        }

        // UART device ID
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--device") == 0)
        {
            if (argc > i + 1)
            {
                i++;
                uart_name = argv[i];
            }
            else
            {
                printf("%s\n", commandline_usage);
                throw EXIT_FAILURE;
            }
        }

        // Baud rate
        if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--baud") == 0)
        {
            if (argc > i + 1)
            {
                i++;
                baudrate = atoi(argv[i]);
            }
            else
            {
                printf("%s\n", commandline_usage);
                throw EXIT_FAILURE;
            }
        }

        // UDP ip
        if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--udp_ip") == 0)
        {
            if (argc > i + 1)
            {
                i++;
                udp_ip = argv[i];
                use_udp = true;
            }
            else
            {
                printf("%s\n", commandline_usage);
                throw EXIT_FAILURE;
            }
        }

        // UDP port
        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0)
        {
            if (argc > i + 1)
            {
                i++;
                udp_port = atoi(argv[i]);
            }
            else
            {
                printf("%s\n", commandline_usage);
                throw EXIT_FAILURE;
            }
        }
    }
    // end: for each input argument

    // Done!
    return;
}

// ------------------------------------------------------------------------------
//   Quit Signal Handler
// ------------------------------------------------------------------------------
// this function is called when you press Ctrl-C
void quit_handler(int sig)
{
    printf("\n");
    printf("TERMINATING AT USER REQUEST\n");
    printf("\n");

    // autopilot interface
    try
    {
        autopilot_interface_quit->handle_quit(sig);
    }
    catch (int error)
    {
    }

    // port
    try
    {
        port_quit->stop();
    }
    catch (int error)
    {
    }

    // end program here
    exit(0);
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
    std::cout << "15:set vel test  16:xxt auto        "  << std::endl;
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
void mode_takeoff(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t ip,mavlink_set_position_target_local_ned_t sp){
    std::cout << "mode_takeoff started" << std::endl;
	// Example 1 - Fly up by to 2m
	set_position( ip.x ,       // [m]
			 	  ip.y ,       // [m]
				  ip.z - 12.0 , // [m]
				  sp         );
    sp.type_mask |= MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_TAKEOFF;
	// SEND THE COMMAND
	autopilot_interface.update_setpoint(sp);
	// NOW pixhawk will try to move

	// Wait for 8 seconds, check position
	for (int i=0; i < 8; i++)
	{
		mavlink_local_position_ned_t pos = autopilot_interface.current_messages.local_position_ned;
		printf("%i CURRENT POSITION XYZ = [ % .4f , % .4f , % .4f ] \n", i, pos.x, pos.y, pos.z);
		sleep(1);
	}
}
void mode_move_forward(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t sp){
	std::cout << "mode_move_forward started" << std::endl;
    set_velocity(  0.0       , // [m/s]
				   0.5       , // [m/s]
				   0.0       , // [m/s]
				   sp        );
	// SEND THE COMMAND
	autopilot_interface.update_setpoint(sp);
    // Wait for 4 seconds, check position
	for (int i=0; i < 20; i++)
	{
		mavlink_local_position_ned_t pos = autopilot_interface.current_messages.local_position_ned;
		printf("%i CURRENT POSITION XYZ = [ % .4f , % .4f , % .4f ] \n", i, pos.x, pos.y, pos.z);
		sleep(1);
	}
}
void mode_move_backward(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t sp){
	std::cout << "mode_move_backward started" << std::endl;
    set_velocity(  0.0       , // [m/s]
				   -0.5       , // [m/s]
				   0.0       , // [m/s]
				   sp        );
	// SEND THE COMMAND
	autopilot_interface.update_setpoint(sp);
    // Wait for 4 seconds, check position
	for (int i=0; i < 2; i++)
	{
		mavlink_local_position_ned_t pos = autopilot_interface.current_messages.local_position_ned;
		printf("%i CURRENT POSITION XYZ = [ % .4f , % .4f , % .4f ] \n", i, pos.x, pos.y, pos.z);
		sleep(1);
	}
}
void mode_move_left(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t sp){
	std::cout << "mode_move_left started" << std::endl;
    set_velocity(  0.5       , // [m/s]
				   0.0       , // [m/s]
				   0.0       , // [m/s]
				   sp        );
	// SEND THE COMMAND
	autopilot_interface.update_setpoint(sp);
	// Wait for 4 seconds, check position
	for (int i=0; i < 2; i++)
	{
		mavlink_local_position_ned_t pos = autopilot_interface.current_messages.local_position_ned;
		printf("%i CURRENT POSITION XYZ = [ % .4f , % .4f , % .4f ] \n", i, pos.x, pos.y, pos.z);
		sleep(1);
	}
}
void mode_move_right(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t sp){
	std::cout << "mode_move_right started" << std::endl;
    set_velocity(  -0.5       , // [m/s]
				   0.0       , // [m/s]
				   0.0       , // [m/s]
				   sp        );
	// SEND THE COMMAND
	autopilot_interface.update_setpoint(sp);
	// Wait for 4 seconds, check position
	for (int i=0; i < 2; i++)
	{
		mavlink_local_position_ned_t pos = autopilot_interface.current_messages.local_position_ned;
		printf("%i CURRENT POSITION XYZ = [ % .4f , % .4f , % .4f ] \n", i, pos.x, pos.y, pos.z);
		sleep(1);
	}
}
void mode_stop(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t sp){
	std::cout << "mode_stop started" << std::endl;
    set_velocity(  0.0       , // [m/s]
				   0.0       , // [m/s]
				   0.0       , // [m/s]
				   sp        );
	// SEND THE COMMAND
	autopilot_interface.update_setpoint(sp);
	// Wait for 4 seconds, check position
	for (int i=0; i < 2; i++)
	{
		mavlink_local_position_ned_t pos = autopilot_interface.current_messages.local_position_ned;
		printf("%i CURRENT POSITION XYZ = [ % .4f , % .4f , % .4f ] \n", i, pos.x, pos.y, pos.z);
		sleep(1);
	}
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
void set_guided(Autopilot_Interface &autopilot_interface){
    std::cout << "setmode_guided started" << std::endl;
    // return to launch
    autopilot_interface.do_setmode_guided();
    usleep(100); // give some time to let it sink in
}
void set_auto(Autopilot_Interface &autopilot_interface){
    std::cout << "set_auto started" << std::endl;
    // return to launch
    autopilot_interface.do_setmode_auto();
    usleep(100); // give some time to let it sink in
}
void set_velocity_test(Autopilot_Interface &autopilot_interface){
    std::cout << "set_velocity_test started" << std::endl;
    // return to launch
    while(1){
        autopilot_interface.set_velocity_test();
        usleep(100); // give some time to let it sink in
    }
    // autopilot_interface.set_velocity_test();
    // usleep(100); // give some time to let it sink in
}