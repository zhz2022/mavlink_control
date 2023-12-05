// ------------------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <string.h>
#include <inttypes.h>
#include <fstream>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

using std::string;
using namespace std;

#include <common/mavlink.h>

#include "autopilot_interface.h"
#include "serial_port.h"
#include "udp_port.h"
#include "port_mangement.h"
#include "mode_selecter.h"



// ------------------------------------------------------------------------------
//   Main
// ------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    Generic_Port *port;

    // char *uart_name = (char *)"/dev/ttyUSB0";
    // int baudrate = 57600;
    // port = new Serial_Port(uart_name, baudrate);

    char *udp_ip = (char *)"127.0.0.1";
    int udp_port = 14550;
    port = new UDP_Port(udp_ip, udp_port);    

    Autopilot_Interface autopilot_interface(port);
    port->start();

    mode_init(autopilot_interface);
    do_set_mode(autopilot_interface,GUIDED);
    mode_takeoff_local(autopilot_interface);
    move_ned_duration(autopilot_interface,1,0,0,1);
    print_msg_test(autopilot_interface);
    mode_rtl(autopilot_interface);
    std::cout<<"test over"<<std::endl;
    return 0;

}