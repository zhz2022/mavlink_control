// ------------------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------------------

#include "common_usage.h"


// ------------------------------------------------------------------------------
//   Main
// ------------------------------------------------------------------------------
int main(int argc, char **argv)
{
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