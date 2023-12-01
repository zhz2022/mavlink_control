#ifndef PORT_MANGEMENT_H_
#define PORT_MANGEMENT_H_
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

class Port_Mangement
{
public:
	Port_Mangement();
	Port_Mangement(const char *target_ip_, int udp_port_);
	virtual ~Port_Mangement();

	void parse_commandline(int argc, char **argv, char *&uart_name, int &baudrate,
		bool &use_udp, char *&udp_ip, int &udp_port);

	// quit handler
	Autopilot_Interface *autopilot_interface_quit;
	Generic_Port *port_quit;
	void quit_handler( int sig );

private:


}
#endif // PORT_MANGEMENT_H_