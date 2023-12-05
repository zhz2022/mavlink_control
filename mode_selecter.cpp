#include "mode_selecter.h"

int mode_selecter()
{
    int mode;
    std::cout << "请选择模式(输入number)"                 << std::endl;
    std::cout << "1:init           2:takeoff"            << std::endl;
    std::cout << "3:forward        4:move_backward"      << std::endl;
    std::cout << "5:move_left      6:move_right"         << std::endl;
    std::cout << "7:stop           8:land"               << std::endl;
    std::cout << "9:quit           10:return to launch"  << std::endl;
    std::cout << "11:takeoff_local 12:waypoint        "  << std::endl;
    std::cout << "13:set guided    14:set auto        "  << std::endl;
    std::cout << "15:print_msgs    16:move_up         "  << std::endl;
    std::cout << "17:move_down     18:set circle         "  << std::endl;
    std::cout << "===================================="  << std::endl;
    std::cin  >> mode;
    std::cout << ":mode selected to: "<< mode            << std::endl;
    std::cout << "===================================="  << std::endl;
    return mode;
}
void mode_init(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_init started" << std::endl;
    autopilot_interface.start();
	usleep(100); // give some time to let it sink in
    autopilot_interface.arm_disarm(true);
    usleep(100); // give some time to let it sink in

	printf("SEND OFFBOARD COMMANDS\n");
}
void mode_land(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_land started" << std::endl;
    // land
    autopilot_interface.land();
    usleep(100); // give some time to let it sink in
}
void mode_quit(Autopilot_Interface &autopilot_interface, Generic_Port *port){
    std::cout << "mode_quit started" << std::endl;
    autopilot_interface.arm_disarm(false);
    std::cout << "mode_quit started disarm" << std::endl;
    usleep(100); // give some time to let it sink in

    autopilot_interface.disable_offboard_control();

    autopilot_interface.stop();
    port->stop();
    delete port;
}
void mode_rtl(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_rtl started" << std::endl;
    autopilot_interface.return_to_launch();
    usleep(100); // give some time to let it sink in
}
void mode_takeoff_local(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_takeoff_local started" << std::endl;
    autopilot_interface.takeoff_local();
    usleep(100); // give some time to let it sink in
}
void waypoint(Autopilot_Interface &autopilot_interface){
    std::cout << "enable_offboard_control started" << std::endl;
    autopilot_interface.waypoint();
    usleep(100); // give some time to let it sink in
}
void mode_circle(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_circle started" << std::endl;
    autopilot_interface.circle();
    usleep(100); // give some time to let it sink in
}
void print_msg_test(Autopilot_Interface &autopilot_interface){
    std::cout << "set_velocity_test started" << std::endl;
	// Wait for 4 seconds, check position
	for (int i=0; i < 4; i++)
	{
        Mavlink_Messages msgs = autopilot_interface.current_messages;
        std::cout << "Current position: " << msgs.local_position_ned.x << " " << msgs.local_position_ned.y << " " << msgs.local_position_ned.z << std::endl;
        std::cout << "Current velocity: " << msgs.local_position_ned.vx << " " << msgs.local_position_ned.vy << " " << msgs.local_position_ned.vz << std::endl;
        std::cout << "Current pose: " << msgs.attitude.roll << " " << msgs.attitude.pitch << " " << msgs.attitude.yaw << " " << std::endl;
        std::cout << "Current globally_set_position_ned: " << msgs.global_position_int.lat << " " << msgs.global_position_int.lon << " " << msgs.global_position_int.alt << msgs.global_position_int.relative_alt << std::endl;
        std::cout << "Current globally_set_velocity_ned: " << msgs.global_position_int.vx << " " << msgs.global_position_int.vy << " " << msgs.global_position_int.vz << msgs.global_position_int.hdg << std::endl;
        std::cout << "Current battery_voltage: " << msgs.sys_status.voltage_battery << std::endl;
		sleep(1);
	}
}
