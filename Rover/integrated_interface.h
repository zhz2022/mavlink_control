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

#include "ardurover_interface.h"
#include "serial_port.h"
#include "udp_port.h"
#include "port_mangement.h"

// ------------------------------------------------------------------------------
//   Prototypes
// ------------------------------------------------------------------------------

enum Mode_Number {
    // Auto Pilot modes
    // ----------------
        MANUAL       = 0,
        ACRO         = 1,
        STEERING     = 3,
        HOLD         = 4,
        LOITER       = 5,
        FOLLOW       = 6,
        SIMPLE       = 7,
#if MODE_DOCK_ENABLED == ENABLED
        DOCK         = 8,
#endif
        CIRCLE       = 9,
        AUTO         = 10,
        RTL          = 11,
        SMART_RTL    = 12,
        GUIDED       = 15,
        INITIALISING = 16,
};

int mode_selecter();
void mode_init(Ardurover_Interface &ardurover_interface);
void mode_quit(Ardurover_Interface &ardurover_interface,Generic_Port *port);

void print_msg_test(Ardurover_Interface &ardurover_interface);

void do_set_mode(Ardurover_Interface &ardurover_interface,int mode_number);


#endif // MODE_SELECTER_H_
