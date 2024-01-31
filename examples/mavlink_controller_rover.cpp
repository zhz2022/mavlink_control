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
enum Mode {MY_INIT=1,MY_ARM,MY_MOVE_FORWARD,MY_MOVE_BACKWARD,MY_MOVE_LEFT,MY_MOVE_RIGHT,MY_STOP,MY_LAND,MY_QUIT,MY_RTL,\
TMP,MY_WAYPOINT,MY_GUIDED,MY_AUTO,MY_PRINT_MSG};

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

    Ardurover_Interface ardurover_interface(port);

    port_quit = port;
    ardurover_interface_quit = &ardurover_interface;
    signal(SIGINT, quit_handler);

    port->start();

    mavlink_set_position_target_local_ned_t sp;

    while(1){
        usleep(100);
        switch (gl_mode_select){
            case MY_INIT:
                mode_init(ardurover_interface);
                std::cout << "case 1" << std::endl;
                gl_mode_select = mode_selecter();
                break;
            case MY_ARM:
                ardurover_interface.arm_disarm(true);
                usleep(100); // give some time to let it sink in
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_FORWARD:
                set_velocity( 1.0,0.0,0.0,sp);
                ardurover_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_BACKWARD:
                set_velocity( -5.0,0.0,0.0,sp);
                ardurover_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_LEFT:
                set_velocity( 0.0,-5.0,0.0,sp);
                ardurover_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_MOVE_RIGHT:
                set_velocity( 0.0,5.0,0.0,sp);
                ardurover_interface.update_setpoint(sp);
                gl_mode_select = mode_selecter();
                break;
            case MY_QUIT:
                mode_quit(ardurover_interface,port);
                gl_mode_select = mode_selecter();
                break;
            case MY_RTL:
                do_set_mode(ardurover_interface,RTL);
                gl_mode_select = mode_selecter();
                break;
            case MY_GUIDED:
                do_set_mode(ardurover_interface,GUIDED);
                gl_mode_select = mode_selecter();
                break;
            case MY_AUTO:
                do_set_mode(ardurover_interface,AUTO);
                gl_mode_select = mode_selecter();
                break;
            case MY_WAYPOINT:
                goto_location(ardurover_interface,-35.36130812,149.16114736,30);
                std::cout << "goto location" << std::endl;
                gl_mode_select = mode_selecter();
                break;
            case MY_PRINT_MSG:
                print_msg_test(ardurover_interface);
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

    // ardurover interface
    try
    {
        ardurover_interface_quit->handle_quit(sig);
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