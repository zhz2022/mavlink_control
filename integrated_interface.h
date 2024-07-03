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
#include <vector>

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
#ifdef PX4 //1.12.3
enum Mode_Number {
        MANUAL   =      1,  // manual airframe angle with automatic throttle
        ALTITUDE =      2,  // ALTITUDE
        POSITION =      3,  // Position
        MISSION  =      4,  // 任务模式允许无人机执行预先定义的自主任务
        ARCO     =      5,  // 飞行器会悬停在当前的高度和位置。
        OFFBOARD =      6,  // automatic return to launching point
        STABILIZED =    7,  // roll和pitch独立控制多旋翼的角速度。  6 / 10
        
        
        //AUTO_TAKEOFF =      17,  // 飞行器会遵循通过MAVLink提供的飞行设定点 7/14
};
#else
enum Mode_Number {
        STABILIZE =     0,  // manual airframe angle with manual throttle
        ACRO =          1,  // manual body-frame angular rate with manual throttle
        ALT_HOLD =      2,  // manual airframe angle with automatic throttle
        AUTO =          3,  // fully automatic waypoint control using mission commands
        GUIDED =        4,  // fully automatic fly to coordinate or fly at velocity/direction using GCS immediate commands
        LOITER =        5,  // automatic horizontal acceleration with automatic throttle
        RTL =           6,  // automatic return to launching point
        CIRCLE =        7,  // automatic circular flight with automatic throttle
        LAND =          9,  // automatic landing with horizontal position control
        DRIFT =        11,  // semi-autonomous position, yaw and throttle control
        SPORT =        13,  // manual earth-frame angular rate control with manual throttle
        FLIP =         14,  // automatically flip the vehicle on the roll axis
        AUTOTUNE =     15,  // automatically tune the vehicle's roll and pitch gains
        POSHOLD =      16,  // automatic position hold with manual override, with automatic throttle
        BRAKE =        17,  // full-brake using inertial/GPS system, no pilot input
        THROW =        18,  // throw to launch mode using inertial/GPS system, no pilot input
        AVOID_ADSB =   19,  // automatic avoidance of obstacles in the macro scale - e.g. full-sized aircraft
        GUIDED_NOGPS = 20,  // guided mode but only accepts attitude and altitude
        SMART_RTL =    21,  // SMART_RTL returns to home by retracing its steps
        FLOWHOLD  =    22,  // FLOWHOLD holds position with optical flow without rangefinder
        FOLLOW    =    23,  // follow attempts to follow another vehicle or ground station
        ZIGZAG    =    24,  // ZIGZAG mode is able to fly in a zigzag manner with predefined point A and point B
        SYSTEMID  =    25,  // System ID mode produces automated system identification signals in the controllers
        AUTOROTATE =   26,  // Autonomous autorotation
        AUTO_RTL =     27,  // Auto RTL, this is not a true mode, AUTO will report as this mode if entered to perform a DO_LAND_START Landing sequence
        TURTLE =       28,  // Flip over after crash

        // Mode number 127 reserved for the "drone show mode" in the Skybrush
        // fork at https://github.com/skybrush-io/ardupilot
};
#endif
int mode_selecter();
void mode_init(Autopilot_Interface &autopilot_interface);
void mode_quit(Autopilot_Interface &autopilot_interface,Generic_Port *port);
void mode_rtl(Autopilot_Interface &autopilot_interface);
void mode_takeoff_local(Autopilot_Interface &autopilot_interface);
void waypoint(Autopilot_Interface &autopilot_interface,double lon,double lat,float alt);
void print_msg_test(Autopilot_Interface &autopilot_interface);
void mode_circle(Autopilot_Interface &autopilot_interface);
void do_set_mode(Autopilot_Interface &autopilot_interface,int mode_number);
void move_ned_duration(Autopilot_Interface &autopilot_interface,float vn,float ve,float vd,float duration);
void move_ned_offset(Autopilot_Interface &autopilot_interface,float offset_n,float offset_e,float offset_d,mavlink_set_position_target_local_ned_t &sp);
void upload_waypoint_list(Autopilot_Interface &autopilot_interface,const std::vector<std::vector<float>>& waypoints);
#endif // MODE_SELECTER_H_
