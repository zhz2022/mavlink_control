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
#ifdef PX4
enum Mode{MY_INIT=1,MY_ARM,MY_TAKEOFF_LOCAL,MY_MANUAL,MY_ALTITUDE,MY_POSITION,MY_MISSION,MY_ARCO,MY_OFFBOARD,MY_STABILIZED\
,MY_MOVE_FORWARD,MY_MOVE_BACKWARD,MY_MOVE_LEFT,MY_MOVE_RIGHT,MY_MOVE_UP,MY_MOVE_DOWN,MY_STOP,MY_LAND,mode_RTL,MY_RTL,MY_WAYPOINT,MY_PRINT_MSG,\
MY_OFFSET,MY_QUIT};


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
    signal(SIGINT, quit_handler);

    port->start();

    mavlink_set_position_target_local_ned_t sp;
    // Mavlink_Messages cur_msgs = autopilot_interface.current_messages;

	// mavlink_set_position_target_local_ned_t ip = autopilot_interface.initial_position;
    std::vector<std::vector<float>> waypoint_list = { {36.7255, 117.1751, 30}, {36.7265, 117.1751, 40}, {36.7255, 117.1751, 50} };
    while(1){
        Mavlink_Messages cur_msgs = autopilot_interface.current_messages;
        float x = cur_msgs.local_position_ned.x;
        float y = cur_msgs.local_position_ned.y;
        float z = cur_msgs.local_position_ned.z;

        usleep(1000);
        switch (gl_mode_select){
            case MY_INIT:
                mode_init(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case MY_ARM:
                autopilot_interface.arm_disarm(true);
                usleep(100); // give some time to let it sink in
                gl_mode_select = mode_selecter();
                break;
            case MY_TAKEOFF_LOCAL:
                mode_takeoff_local(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case MY_MANUAL:
                do_set_mode(autopilot_interface,MANUAL);
                gl_mode_select = mode_selecter();
                break;
            case MY_ALTITUDE:
                do_set_mode(autopilot_interface,ALTITUDE);
                gl_mode_select = mode_selecter();
                break;
            case MY_POSITION:
                do_set_mode(autopilot_interface,POSITION);
                gl_mode_select = mode_selecter();
                break;
            case MY_MISSION:
                do_set_mode(autopilot_interface,MISSION);
                gl_mode_select = mode_selecter();
                break;
            case MY_ARCO:
                do_set_mode(autopilot_interface,ARCO);
                gl_mode_select = mode_selecter();
                break;
            case MY_OFFBOARD:
                do_set_mode(autopilot_interface,OFFBOARD);
                gl_mode_select = mode_selecter();
                break;
            case MY_STABILIZED:
                do_set_mode(autopilot_interface,STABILIZED);
                gl_mode_select = mode_selecter();
                break;
        
            case MY_MOVE_FORWARD:
                //set_position(x + 1.0, y, z, sp); // set_velocity(1.0, 0.0, 0.0, sp);
                set_velocity( 1.0,0.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                sleep(1);
                set_velocity( 0.0,0.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;

            case MY_MOVE_BACKWARD:
                //set_position(x - 1.0, y, z ,sp);//set_velocity( -1.0,0.0,0.0,sp);
                set_velocity( -1.0,0.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                sleep(1);
                set_velocity( 0.0,0.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_LEFT:
                //set_position( x,y -1.0,z,sp);
                set_velocity( 0.0,-1.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                sleep(1);
                set_velocity( 0.0,0.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_RIGHT:
                //set_position( x, y+1.0,z,sp);
                set_velocity( 0.0,1.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                sleep(1);
                set_velocity( 0.0,0.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_UP:
                set_velocity( 0.0,0.0,-1.0,sp);
                autopilot_interface.update_setpoint(sp);
                sleep(1);
                set_velocity( 0.0,0.0,0.0,sp);
                //set_position( x,y,z-1.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_DOWN:
                //set_position( x,y,z+1.0,sp);
                set_velocity( 0.0,0.0,+1.0,sp);
                autopilot_interface.update_setpoint(sp);
                sleep(1);
                set_velocity( 0.0,0.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_STOP:
                set_velocity( x,y,z,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;

            case MY_LAND:
                autopilot_interface.land();
                usleep(100); // give some time to let it sink in
                gl_mode_select = mode_selecter();
                break;

            case mode_RTL:
                autopilot_interface.return_to_launch();
                gl_mode_select = mode_selecter();
                break;
            case MY_RTL:
                autopilot_interface.return_to_launch();
                gl_mode_select = mode_selecter();
                break;

            case MY_WAYPOINT:
                // waypoint(autopilot_interface,-35.361297*1e7,149.161120*1e7,-20);
                upload_waypoint_list(autopilot_interface,waypoint_list);
                gl_mode_select = mode_selecter();
                break;
            case MY_PRINT_MSG:
                print_msg_test(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case MY_OFFSET:
                move_ned_offset(autopilot_interface,100,100,-1,sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_QUIT:
                mode_quit(autopilot_interface,port);
                gl_mode_select = mode_selecter();
                break;
            default :
                std::cout << "无效的模式选择" << std::endl;
                gl_mode_select = mode_selecter();
                
        }
    }
    return 0;

}

#else
enum Mode {MY_INIT=1,MY_ARM,MY_MOVE_FORWARD,MY_MOVE_BACKWARD,MY_MOVE_LEFT,MY_MOVE_RIGHT,MY_STOP,MY_LAND,MY_QUIT,MY_RTL,\
MY_TAKEOFF_LOCAL,MY_WAYPOINT,MY_GUIDED,MY_AUTO,MY_PRINT_MSG,MY_MOVE_UP,MY_MOVE_DOWN,MY_CIRCLE,MY_ZIGZAG,MY_OFFSET};
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
    signal(SIGINT, quit_handler);

    port->start();

    mavlink_set_position_target_local_ned_t sp;
	// mavlink_set_position_target_local_ned_t ip = autopilot_interface.initial_position;
    std::vector<std::vector<float>> waypoint_list = { {36.7255, 117.1751, 30}, {36.7265, 117.1751, 40}, {36.7255, 117.1751, 50} };
    while(1){
        usleep(100);
        switch (gl_mode_select){
            case MY_INIT:
                mode_init(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case MY_ARM:
                autopilot_interface.arm_disarm(true);
                usleep(100); // give some time to let it sink in
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_FORWARD:
                set_velocity( 1.0,0.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_BACKWARD:
                set_velocity( -1.0,0.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_LEFT:
                set_velocity( 0.0,-1.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_RIGHT:
                set_velocity( 0.0,1.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_UP:
                set_velocity( 0.0,0.0,-1.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_DOWN:
                set_velocity( 0.0,0.0,1.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_STOP:
                set_velocity( 0.0,0.0,0.0,sp);
                autopilot_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_LAND:
                autopilot_interface.land();
                usleep(100); // give some time to let it sink in
                gl_mode_select = mode_selecter();
                break;
            case MY_QUIT:
                mode_quit(autopilot_interface,port);
                gl_mode_select = mode_selecter();
                break;
            case MY_RTL:
                do_set_mode(autopilot_interface,RTL);
                gl_mode_select = mode_selecter();
                break;
            case MY_GUIDED:
                do_set_mode(autopilot_interface,GUIDED);
                gl_mode_select = mode_selecter();
                break;
            case MY_AUTO:
                do_set_mode(autopilot_interface,AUTO);
                gl_mode_select = mode_selecter();
                break;
            case MY_CIRCLE:
                do_set_mode(autopilot_interface,CIRCLE);
                gl_mode_select = mode_selecter();
                break;
            case MY_ZIGZAG:
                do_set_mode(autopilot_interface,ZIGZAG);
                gl_mode_select = mode_selecter();
                break;
            case MY_TAKEOFF_LOCAL:
                mode_takeoff_local(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case MY_WAYPOINT:
                // waypoint(autopilot_interface,-35.361297*1e7,149.161120*1e7,-20);
                upload_waypoint_list(autopilot_interface,waypoint_list);
                gl_mode_select = mode_selecter();
                break;
            case MY_PRINT_MSG:
                print_msg_test(autopilot_interface);
                gl_mode_select = mode_selecter();
                break;
            case MY_OFFSET:
                move_ned_offset(autopilot_interface,100,100,-1,sp);
                gl_mode_select = mode_selecter();
                break;
            default :
                std::cout << "无效的模式选择" << std::endl;
                gl_mode_select = mode_selecter();
        }
    }
    return 0;
}
#endif
// ------------------------------------------------------------------------------
//   Quit Signal Handler
// ------------------------------------------------------------------------------
// this function is called when you press Ctrl-C
void quit_handler(int sig )
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
