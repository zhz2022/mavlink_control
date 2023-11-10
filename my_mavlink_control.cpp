/**
 * @file mavlink_control.cpp
 *
 * @brief An example offboard control process via mavlink
 *
 * This process connects an external MAVLink UART device to send an receive data
 *
 *
 */

// ------------------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------------------

#include "my_mavlink_control.h"
int mode_gl;
enum Mode {INIT=1,TAKEOFF,MOVE_FORWARD,MOVE_BACKWARD,MOVE_LEFT,MOVE_RIGHT,LAND,QUIT};
// ------------------------------------------------------------------------------
//   TOP
// ------------------------------------------------------------------------------
// int top(int argc, char **argv)
// {
//     // --------------------------------------------------------------------------
//     //   RUN COMMANDS
//     // --------------------------------------------------------------------------

//     /*
//      * Now we can implement the algorithm we want on top of the autopilot interface
//      */
//     // commands(autopilot_interface, autotakeoff);

//     // --------------------------------------------------------------------------
//     //   DONE
//     // --------------------------------------------------------------------------

//     // woot!
//     return 0;
// }



// ------------------------------------------------------------------------------
//   Main
// ------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    mode_select();

    char *uart_name = (char *)"/dev/ttyUSB0";
    int baudrate = 57600;

    bool use_udp = false;
    char *udp_ip = (char *)"127.0.0.1";
    int udp_port = 14550;
    // do the parse, will throw an int if it fails
    parse_commandline(argc, argv, uart_name, baudrate, use_udp, udp_ip, udp_port);
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

    while(1){
        switch (mode_gl){
        case INIT:
            mode_init();
            mode_select();
        case TAKEOFF:
            mode_takeoff();
            mode_select();
        case MOVE_FORWARD:
            mode_move_forward();
            mode_select();
        case MOVE_BACKWARD:
            mode_move_backward();
            mode_select();
        case MOVE_LEFT:
            mode_move_left();
            mode_select();
        case MOVE_RIGHT:
            mode_move_right();
            mode_select();
        case LAND:
            mode_land();
            mode_select();
        case QUIT:
            mode_quit();
            mode_select();
        default :
            std::cout << "无效的模式选择" << std::endl;
            mode_select();
        }
    }
    return 0;
}

// ------------------------------------------------------------------------------
//   Parse Command Line
// ------------------------------------------------------------------------------
// throws EXIT_FAILURE if could not open the port
void parse_commandline(int argc, char **argv, char *&uart_name, int &baudrate,
                       bool &use_udp, char *&udp_ip, int &udp_port)
{

    // string for command line usage
    const char *commandline_usage = "usage: mavlink_control [-d <devicename> -b <baudrate>] [-u <udp_ip> -p <udp_port>] [-a ]";

    // Read input arguments
    for (int i = 1; i < argc; i++)
    { // argv[0] is "mavlink"

        // Help
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("%s\n", commandline_usage);
            throw EXIT_FAILURE;
        }

        // UART device ID
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--device") == 0)
        {
            if (argc > i + 1)
            {
                i++;
                uart_name = argv[i];
            }
            else
            {
                printf("%s\n", commandline_usage);
                throw EXIT_FAILURE;
            }
        }

        // Baud rate
        if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--baud") == 0)
        {
            if (argc > i + 1)
            {
                i++;
                baudrate = atoi(argv[i]);
            }
            else
            {
                printf("%s\n", commandline_usage);
                throw EXIT_FAILURE;
            }
        }

        // UDP ip
        if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--udp_ip") == 0)
        {
            if (argc > i + 1)
            {
                i++;
                udp_ip = argv[i];
                use_udp = true;
            }
            else
            {
                printf("%s\n", commandline_usage);
                throw EXIT_FAILURE;
            }
        }

        // UDP port
        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0)
        {
            if (argc > i + 1)
            {
                i++;
                udp_port = atoi(argv[i]);
            }
            else
            {
                printf("%s\n", commandline_usage);
                throw EXIT_FAILURE;
            }
        }
    }
    // end: for each input argument

    // Done!
    return;
}

// ------------------------------------------------------------------------------
//   COMMANDS
// ------------------------------------------------------------------------------

void commands(Autopilot_Interface &api, bool autotakeoff)
{

    // --------------------------------------------------------------------------
    //   START OFFBOARD MODE
    // --------------------------------------------------------------------------

    api.enable_offboard_control();
    usleep(100); // give some time to let it sink in

    // now the autopilot is accepting setpoint commands

    if (autotakeoff)
    {
        // arm autopilot
        api.arm_disarm(true);
        usleep(100); // give some time to let it sink in
    }

    // --------------------------------------------------------------------------
    //   SEND OFFBOARD COMMANDS
    // --------------------------------------------------------------------------
    printf("SEND OFFBOARD COMMANDS\n");

    // initialize command data strtuctures
    mavlink_set_position_target_local_ned_t sp;
    mavlink_set_position_target_local_ned_t ip = api.initial_position;

    // autopilot_interface.h provides some helper functions to build the command

    // Example 1 - Fly up by to 2m
    set_position(ip.x,		 // [m]
                 ip.y,		 // [m]
                 ip.z - 2.0, // [m]
                 sp);

    if (autotakeoff)
    {
        sp.type_mask |= MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_TAKEOFF;
    }

    // SEND THE COMMAND
    api.update_setpoint(sp);
    // NOW pixhawk will try to move

    // Example 2 - Set Velocity
    set_velocity(-100.0, // [m/s]
                 -100.0, // [m/s]
                 0.0,  // [m/s]
                 sp);

    // Example 2.1 - Append Yaw Command
    // set_yaw(ip.yaw + 90.0 / 180.0 * M_PI, // [rad]
    //         sp);

    // SEND THE COMMAND
    api.update_setpoint(sp);
    // NOW pixhawk will try to move

    // Wait for 4 seconds, check position
    for (int i = 0; i < 10000; i++)
    {
        mavlink_local_position_ned_t pos = api.current_messages.local_position_ned;
        printf("%i CURRENT POSITION XYZ = [ % .4f , % .4f , % .4f ] \n", i, pos.x, pos.y, pos.z);
        sleep(1);
    }

    if (autotakeoff)
    {
        // Example 3 - Land using fixed velocity
        set_velocity(0.0, // [m/s]
                     0.0, // [m/s]
                     1.0, // [m/s]
                     sp);
        printf("velocity seted====================================================\n");

        sp.type_mask |= MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_LAND;

        // SEND THE COMMAND
        api.update_setpoint(sp);
        // NOW pixhawk will try to move

        // Wait for 8 seconds, check position
        for (int i = 0; i < 8; i++)
        {
            mavlink_local_position_ned_t pos = api.current_messages.local_position_ned;
            printf("%i CURRENT POSITION XYZ = [ % .4f , % .4f , % .4f ] \n", i, pos.x, pos.y, pos.z);
            sleep(1);
        }

        printf("\n");

        // disarm autopilot
        api.arm_disarm(false);
        usleep(100); // give some time to let it sink in
    }

    // --------------------------------------------------------------------------
    //   STOP OFFBOARD MODE
    // --------------------------------------------------------------------------

    api.disable_offboard_control();

    // now pixhawk isn't listening to setpoint commands

    // --------------------------------------------------------------------------
    //   GET A MESSAGE
    // --------------------------------------------------------------------------
    printf("READ SOME MESSAGES \n");

    // copy current messages
    Mavlink_Messages messages = api.current_messages;

    // local position in ned frame
    mavlink_local_position_ned_t pos = messages.local_position_ned;
    printf("Got message LOCAL_POSITION_NED (spec: https://mavlink.io/en/messages/common.html#LOCAL_POSITION_NED)\n");
    printf("    pos  (NED):  %f %f %f (m)\n", pos.x, pos.y, pos.z);

    // hires imu
    mavlink_highres_imu_t imu = messages.highres_imu;
    printf("Got message HIGHRES_IMU (spec: https://mavlink.io/en/messages/common.html#HIGHRES_IMU)\n");
    printf("    ap time:     %lu \n", imu.time_usec);
    printf("    acc  (NED):  % f % f % f (m/s^2)\n", imu.xacc, imu.yacc, imu.zacc);
    printf("    gyro (NED):  % f % f % f (rad/s)\n", imu.xgyro, imu.ygyro, imu.zgyro);
    printf("    mag  (NED):  % f % f % f (Ga)\n", imu.xmag, imu.ymag, imu.zmag);
    printf("    baro:        %f (mBar) \n", imu.abs_pressure);
    printf("    altitude:    %f (m) \n", imu.pressure_alt);
    printf("    temperature: %f C \n", imu.temperature);

    printf("\n");

    // --------------------------------------------------------------------------
    //   END OF COMMANDS
    // --------------------------------------------------------------------------

    return;
}


// ------------------------------------------------------------------------------
//   Quit Signal Handler
// ------------------------------------------------------------------------------
// this function is called when you press Ctrl-C
void quit_handler(int sig)
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
void mode_select()
{
    // int mode;
    std::cout << "请选择模式(输入number)"         << std::endl;
    std::cout << "1:init      2:takeoff"        << std::endl;
    std::cout << "3:forward   4:move_backward"  << std::endl;
    std::cout << "5:move_left 6:move_right"     << std::endl;
    std::cout << "7:land      8:quit"           << std::endl;
    std::cout << "============================" << std::endl;
    std::cin  >> mode_gl;
    std::cout << ":mode selected to: "<< mode_gl<< std::endl;
    std::cout << "============================" << std::endl;
    // return mode;
}
void mode_init(){
    // --------------------------------------------------------------------------
    //   PARSE THE COMMANDS
    // --------------------------------------------------------------------------
    // char *uart_name = (char *)"/dev/ttyUSB0";
    // int baudrate = 57600;

    // bool use_udp = false;
    // char *udp_ip = (char *)"127.0.0.1";
    // int udp_port = 14550;
    // bool autotakeoff = false;
    // printf("autotakeoff = false====================================================\n");

    // // do the parse, will throw an int if it fails
    // parse_commandline(argc, argv, uart_name, baudrate, use_udp, udp_ip, udp_port);

    // --------------------------------------------------------------------------
    //   PORT and THREAD STARTUP
    // --------------------------------------------------------------------------
    // Generic_Port *port;
    // if (use_udp)
    // {
    //     port = new UDP_Port(udp_ip, udp_port);
    // }
    // else
    // {
    //     port = new Serial_Port(uart_name, baudrate);
    // }

    // Autopilot_Interface autopilot_interface(port);

    // port_quit = port;
    // autopilot_interface_quit = &autopilot_interface;
    // signal(SIGINT, quit_handler);

    // port->start();
    // autopilot_interface.start();
}
void mode_takeoff(){
    // commands(autopilot_interface, autotakeoff);
}
void mode_move_forward(){
    ;
}
void mode_move_backward(){
    ;
}
void mode_move_left(){
    ;
}
void mode_move_right(){
    ;
}
void mode_land(){
    ;
}
void mode_quit(){
    // --------------------------------------------------------------------------
    //   THREAD and PORT SHUTDOWN
    // --------------------------------------------------------------------------TODO
//     autopilot_interface.stop();
//     port->stop();

//     delete port;
}