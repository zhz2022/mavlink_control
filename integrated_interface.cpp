#include "integrated_interface.h"


void mode_init(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_init started" << std::endl;
    autopilot_interface.start();
	usleep(100); // give some time to let it sink in
    // autopilot_interface.arm_disarm(true);
    // usleep(100); // give some time to let it sink in
	printf("SEND OFFBOARD COMMANDS\n");
}

void mode_armed(Autopilot_Interface &autopilot_interface){
    std::cout << "armed " << std::endl;
    autopilot_interface.arm_disarm(true);
    usleep(100); // give some time to let it sink in
	printf("ARMED\n");
}

void mode_disarmed(Autopilot_Interface &autopilot_interface){
    std::cout << "disarmed " << std::endl;
    autopilot_interface.arm_disarm(false);
    usleep(100); // give some time to let it sink in
	printf("DISARMED\n");
}

void mode_takeoff_local(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_takeoff_local started" << std::endl;
    autopilot_interface.takeoff_local(10);
    usleep(100); // give some time to let it sink in
}

void mode_manual(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_manual" << std::endl;
    autopilot_interface.do_set_mode(MANUAL);
    usleep(100); // give some time to let it sink in
}


void mode_altitude(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_altitude started" << std::endl;
    autopilot_interface.do_set_mode(ALTITUDE);
    usleep(100); // give some time to let it sink in
}

void mode_position(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_position started" << std::endl;
    autopilot_interface.do_set_mode(POSITION);
    usleep(100); // give some time to let it sink in
}

void mode_mission(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_mission started" << std::endl;
    autopilot_interface.do_set_mode(MISSION);
    usleep(100); // give some time to let it sink in
}

void mode_arco(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_arco started" << std::endl;
    autopilot_interface.do_set_mode(ARCO);
    usleep(100); // give some time to let it sink in
}

void mode_stabilized(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_stabilized started" << std::endl;
    autopilot_interface.do_set_mode(STABILIZED);
    usleep(100); // give some time to let it sink in
}

void mode_offboard(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_offboard started" << std::endl;
    autopilot_interface.do_set_mode(OFFBOARD);
    usleep(100); // give some time to let it sink in
}

void mode_land(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_land started" << std::endl;
    autopilot_interface.land();
    usleep(100); // give some time to let it sink in
}

void mode_quit(Autopilot_Interface &autopilot_interface, Generic_Port *port){
    std::cout << "mode_quit started" << std::endl;
    autopilot_interface.arm_disarm(false);
    std::cout << "mode_quit started disarm" << std::endl;
    usleep(100); // give some time to let it sink in

    autopilot_interface.stop();
    port->stop();
    delete port;
}

void mode_rtl(Autopilot_Interface &autopilot_interface){
    std::cout << "mode_rtl started" << std::endl;
    autopilot_interface.return_to_launch();
    usleep(100); // give some time to let it sink in
}

void move_forward_north(Autopilot_Interface &autopilot_interface){
    //set_position(x + 1.0, y, z, sp); // set_velocity(1.0, 0.0, 0.0, sp);
    mavlink_set_position_target_local_ned_t sp;
    set_velocity( 1.0,0.0,0.0,sp);
    autopilot_interface.update_setpoint(sp);
    sleep(1);
    set_velocity( 0.0,0.0,0.0,sp);
    autopilot_interface.update_setpoint(sp);
}

void move_back_south(Autopilot_Interface &autopilot_interface){
    mavlink_set_position_target_local_ned_t sp;
    set_velocity( -1.0,0.0,0.0,sp);
    autopilot_interface.update_setpoint(sp);
    sleep(1);
    set_velocity( 0.0,0.0,0.0,sp);
    autopilot_interface.update_setpoint(sp);
}

void move_left_west(Autopilot_Interface &autopilot_interface){
    mavlink_set_position_target_local_ned_t sp;
    set_velocity( 0.0,-1.0,0.0,sp);
    autopilot_interface.update_setpoint(sp);
    sleep(1);
    set_velocity( 0.0,0.0,0.0,sp);
    autopilot_interface.update_setpoint(sp);
}

void move_right_east(Autopilot_Interface &autopilot_interface){
    mavlink_set_position_target_local_ned_t sp;
    set_velocity( 0.0,1.0,0.0,sp);
    autopilot_interface.update_setpoint(sp);
    sleep(1);
    set_velocity( 0.0,0.0,0.0,sp);
    autopilot_interface.update_setpoint(sp);
}

void move_up(Autopilot_Interface &autopilot_interface){
    mavlink_set_position_target_local_ned_t sp;
    set_velocity( 0.0,0.0,-1.0,sp);
    autopilot_interface.update_setpoint(sp);
    sleep(1);
    set_velocity( 0.0,0.0,0.0,sp);
    autopilot_interface.update_setpoint(sp);
}

void move_down(Autopilot_Interface &autopilot_interface){
    mavlink_set_position_target_local_ned_t sp;
    set_velocity( 0.0,0.0,1.0,sp);
    autopilot_interface.update_setpoint(sp);
    sleep(1);
    set_velocity( 0.0,0.0,0.0,sp);
    autopilot_interface.update_setpoint(sp);
}

void move_stop(Autopilot_Interface &autopilot_interface){
    mavlink_set_position_target_local_ned_t sp;
    set_velocity( 0.0,0.0,0.0,sp);
    autopilot_interface.update_setpoint(sp);
}

void waypoint(Autopilot_Interface &autopilot_interface,double lon,double lat,float alt){
    autopilot_interface.waypoint_reposition(lon,lat,alt);
    usleep(100); // give some time to let it sink in
}

void print_msg_test(Autopilot_Interface &autopilot_interface){
    Mavlink_Messages msgs = autopilot_interface.current_messages;
    std::cout << "Current position: " << msgs.local_position_ned.x << " " << msgs.local_position_ned.y << " " << msgs.local_position_ned.z << std::endl;
    std::cout << "Current velocity: " << msgs.local_position_ned.vx << " " << msgs.local_position_ned.vy << " " << msgs.local_position_ned.vz << std::endl;
    std::cout << "Current pose: " << msgs.attitude.roll << " " << msgs.attitude.pitch << " " << msgs.attitude.yaw << " " << std::endl;
    std::cout << "Current globally_set_position_ned: " << msgs.global_position_int.lat << " " << msgs.global_position_int.lon << " " << msgs.global_position_int.alt << " " << msgs.global_position_int.relative_alt << std::endl;
    std::cout << "Current globally_set_velocity_ned: " << msgs.global_position_int.vx << " " << msgs.global_position_int.vy << " " << msgs.global_position_int.vz << msgs.global_position_int.hdg << std::endl;
    std::cout << "Current battery_voltage: " << msgs.sys_status.voltage_battery << std::endl;
}
// void do_set_mode(Autopilot_Interface &autopilot_interface,int mode_number){
//     autopilot_interface.do_set_mode(mode_number);
//     usleep(100); // give some time to let it sink in
// }
void move_ned_duration(Autopilot_Interface &autopilot_interface,float vn,float ve,float vd,float duration){
	std::cout << "mode_move_forward started" << std::endl;
    for(int i = 1; i <= 2000*duration; i++){
        autopilot_interface.set_velocity(vn,ve,vd);
        usleep(100); // give some time to let it sink in
    }
}

void move_ned_offset(Autopilot_Interface &autopilot_interface,float offset_n,float offset_e,float offset_d,mavlink_set_position_target_local_ned_t &sp){
    // mavlink_set_position_target_local_ned_t ip = autopilot_interface.current_messages.local_position_ned;
    mavlink_local_position_ned_t current_position = autopilot_interface.current_messages.local_position_ned;
    set_position( current_position.x+offset_n , current_position.y+offset_e , current_position.z+offset_d , sp );
    autopilot_interface.update_setpoint(sp);
}

void upload_waypoint_list(Autopilot_Interface &autopilot_interface,const std::vector<std::vector<float>>& waypoints){
    for (const auto& waypoint : waypoints) {
        int wp_seq = 0;
        int lon = 0,lat =1,alt = 2;
        // std::cout << waypoint[lon] << waypoint[lat]<< waypoint[alt]<< std::endl;
        autopilot_interface.add_waypoint(waypoint[lon],waypoint[lat],waypoint[alt],wp_seq);
        wp_seq ++;
    }
}

// this function is called when you press Ctrl-C
// void quit_handler(int sig )
// {
//     printf("\n");
//     printf("TERMINATING AT USER REQUEST\n");
//     printf("\n");

//     // autopilot interface
//     try
//     {
//         autopilot_interface_quit->handle_quit(sig);
//     }
//     catch (int error)
//     {
//     }

//     // port
//     try
//     {
//         port_quit->stop();
//     }
//     catch (int error)
//     {
//     }
//     // end program here
//     exit(0);
// }