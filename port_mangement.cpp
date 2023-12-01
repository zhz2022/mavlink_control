#include "port_mangement.h"

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
void port_mangement(int argc, char **argv)
{
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

    port_quit = port;
    autopilot_interface_quit = &autopilot_interface;
    signal(SIGINT, quit_handler);

    port->start();
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