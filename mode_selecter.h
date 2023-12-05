#ifndef MODE_SELECTER_H_
#define MODE_SELECTER_H_
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

// ------------------------------------------------------------------------------
//   Prototypes
// ------------------------------------------------------------------------------
enum Mode {INIT=1,TAKEOFF,MOVE_FORWARD,MOVE_BACKWARD,MOVE_LEFT,MOVE_RIGHT,STOP,LAND,QUIT,RTL,\
TAKEOFF_LOCAL,WAYPOINT,SET_GUIDED,SET_AUTO,PRINT_MSG,MOVE_UP,MOVE_DOWN,CIRCLE};


int mode_selecter();
void mode_init(Autopilot_Interface &autopilot_interface);
void mode_quit(Autopilot_Interface &autopilot_interface,Generic_Port *port);
void mode_rtl(Autopilot_Interface &autopilot_interface);
void mode_takeoff_local(Autopilot_Interface &autopilot_interface);
void waypoint(Autopilot_Interface &autopilot_interface);
void print_msg_test(Autopilot_Interface &autopilot_interface);
void mode_circle(Autopilot_Interface &autopilot_interface);
void do_set_mode(Autopilot_Interface &autopilot_interface,int mode_number);
void move_duration(Autopilot_Interface &autopilot_interface,float vn,float ve,float vd,int duration);

#endif // MODE_SELECTER_H_
