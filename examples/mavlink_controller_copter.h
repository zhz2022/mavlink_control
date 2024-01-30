/**
 * @file mavlink_control.h
 *
 * @brief offboard control process via mavlink, definition
 *
 * This process connects an external MAVLink UART device to send an receive data
 *
 * @author zhangpeng, <zhangpengbjut@sina.com>
 *
 */

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
#include "Copter/integrated_interface.h"

// ------------------------------------------------------------------------------
//   Prototypes
// ------------------------------------------------------------------------------

int main(int argc, char **argv);
// void parse_commandline(int argc, char **argv, char *&uart_name, int &baudrate,
// 		bool &use_udp, char *&udp_ip, int &udp_port);

// quit handler
Autopilot_Interface *autopilot_interface_quit;
Generic_Port *port_quit;
void quit_handler( int sig );



