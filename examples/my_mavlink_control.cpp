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
                waypoint(autopilot_interface,-35.361297*1e7,149.161120*1e7,-20);
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
