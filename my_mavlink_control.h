/**
 * @file mavlink_control.h
 *
 * @brief An example offboard control process via mavlink, definition
 *
 * This process connects an external MAVLink UART device to send an receive data
 *
 * @author Trent Lukaczyk, <aerialhedgehog@gmail.com>
 * @author Jaycee Lock,    <jaycee.lock@gmail.com>
 * @author Lorenz Meier,   <lm@inf.ethz.ch>
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

// ------------------------------------------------------------------------------
//   Prototypes
// ------------------------------------------------------------------------------

int main(int argc, char **argv);
int top(int argc, char **argv);

void commands(Autopilot_Interface &autopilot_interface, bool autotakeoff);
void parse_commandline(int argc, char **argv, char *&uart_name, int &baudrate,
		bool &use_udp, char *&udp_ip, int &udp_port);

// quit handler
Autopilot_Interface *autopilot_interface_quit;
Generic_Port *port_quit;
void quit_handler( int sig );

int mode_selecter();
void mode_init(Autopilot_Interface &autopilot_interface);
void mode_takeoff(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t ip,mavlink_set_position_target_local_ned_t sp);
void mode_move_forward(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t sp);
void mode_move_backward(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t sp);
void mode_move_left(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t sp);
void mode_move_right(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t sp);
void mode_land(Autopilot_Interface &autopilot_interface,mavlink_set_position_target_local_ned_t sp);
void mode_quit(Autopilot_Interface &autopilot_interface,Generic_Port *port);



