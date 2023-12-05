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
// enum Mode {INIT=1,TAKEOFF,MOVE_FORWARD,MOVE_BACKWARD,MOVE_LEFT,MOVE_RIGHT,STOP,LAND,QUIT,RTL,\
// TAKEOFF_LOCAL,WAYPOINT,SET_GUIDED,SET_AUTO,PRINT_MSG,MOVE_UP,MOVE_DOWN,CIRCLE};

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
