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
TAKEOFF_LOCAL,WAYPOINT,SET_GUIDED,SET_AUTO,PRINT_MSG,MOVE_UP,MOVE_DOWN,CIRCLE};

// ------------------------------------------------------------------------------
//   Main
// ------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    Generic_Port *port;
    Port_Mangement port_mangement;
    Autopilot_Interface autopilot_interface(port);
    while(1){
        usleep(100);
        switch (gl_mode_select){
            case INIT:
                mode_init(autopilot_interface);
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
		// printf("%i CURRENT POSITION XYZ = [ % .4f , % .4f , % .4f ] \n", i, pos.x, pos.y, pos.z);
        std::cout << "Current heartbeat: custom_mode,type,autopilot,base_mode,system_status,mavlink_version :" << msgs.heartbeat.custom_mode << "," << msgs.heartbeat.type << "," << msgs.heartbeat.autopilot << "," << msgs.heartbeat.base_mode \
        << "," << msgs.heartbeat.system_status << "," << msgs.heartbeat.mavlink_version << std::endl;
        std::cout << "Current position: " << msgs.local_position_ned.x << " " << msgs.local_position_ned.y << " " << msgs.local_position_ned.z << std::endl;
		std::cout << "Current sys_status" << "onboard_control_sensors_present: " << msgs.sys_status.onboard_control_sensors_present << "," << "onboard_control_sensors_enabled: " << msgs.sys_status.onboard_control_sensors_enabled << "," << "onboard_control_sensors_health: " \
        << msgs.sys_status.onboard_control_sensors_health << "," << "load: " << msgs.sys_status.load << "," << "voltage_battery: " << msgs.sys_status.voltage_battery << "," << "current_battery: " << msgs.sys_status.current_battery << "," \
        << "drop_rate_comm: " << msgs.sys_status.drop_rate_comm << "," << "errors_comm: " << msgs.sys_status.errors_comm << "," << "errors_count1: " << msgs.sys_status.errors_count1 << "," << "errors_count2: " << msgs.sys_status.errors_count2 << "," \
        << "errors_count3: " << msgs.sys_status.errors_count3 << "," << "errors_count4: " << msgs.sys_status.errors_count4 << "," << "battery_remaining: " << msgs.sys_status.battery_remaining << "," << "onboard_control_sensors_present_extended: " \
        << msgs.sys_status.onboard_control_sensors_present_extended << "," << "onboard_control_sensors_enabled_extended: " << msgs.sys_status.onboard_control_sensors_enabled_extended << "," << "onboard_control_sensors_health_extended: " << msgs.sys_status.onboard_control_sensors_health_extended << std::endl;
        std::cout << "Current velocity: " << msgs.local_position_ned.vx << " " << msgs.local_position_ned.vy << " " << msgs.local_position_ned.vz << std::endl;

        std::cout << "Current pose: " << msgs.attitude.roll << " " << msgs.attitude.pitch << " " << msgs.attitude.yaw << " " << std::endl;
        std::cout << "Current globally_set_position_ned: " << msgs.global_position_int.lat << " " << msgs.global_position_int.lon << " " << msgs.global_position_int.alt << msgs.global_position_int.relative_alt << std::endl;
        std::cout << "Current globally_set_velocity_ned: " << msgs.global_position_int.vx << " " << msgs.global_position_int.vy << " " << msgs.global_position_int.vz << msgs.global_position_int.hdg << std::endl;
        // std::cout << "Current globally_set_acceleration_ned: " << msgs.global_acceleration_ned.x << " " << msgs.global_acceleration_ned.y << " " << ms
        // std::cout << "Current battery_state: " << msgs.battery_state << std::endl;
        std::cout << "Current battery_voltage: " << msgs.sys_status.voltage_battery << std::endl;
		sleep(1);
	}
}

//Need to loop
void mode_move(Autopilot_Interface &autopilot_interface,float vn,float ve,float vd)
{
    autopilot_interface.set_velocity(vn,ve,vd);//plus down minus up
    usleep(100); // give some time to let it sink in
}
// void mode_rotate(Autopilot_Interface &autopilot_interface,float vn,float ve,float vd);
