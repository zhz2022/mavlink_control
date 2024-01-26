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

#include "mavlink_controller_rover.h"

int gl_mode_select = 0;
enum Mode {MY_INIT=1,MY_TAKEOFF,MY_MOVE_FORWARD,MY_MOVE_BACKWARD,MY_MOVE_LEFT,MY_MOVE_RIGHT,MY_STOP,MY_LAND,MY_QUIT,MY_RTL,\
MY_TAKEOFF_LOCAL,MY_WAYPOINT,MY_GUIDED,MY_AUTO,MY_PRINT_MSG,MY_MOVE_UP,MY_MOVE_DOWN,MY_CIRCLE,MY_ZIGZAG};

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

    // while(1){
    //     usleep(100);
    //     switch (gl_mode_select){

    //         default :
    //             std::cout << "无效的模式选择" << std::endl;
    //             gl_mode_select = mode_selecter();
    //     }
    // }
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