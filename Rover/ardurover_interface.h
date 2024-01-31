/**
 * @file ardurover_interface.h
 *
 * @brief Ardurover interface definition
 *
 * Functions for sending and recieving commands to an ardurover via MAVlink
 *
 * @author Trent Lukaczyk, <aerialhedgehog@gmail.com>
 * @author Jaycee Lock,    <jaycee.lock@gmail.com>
 * @author Lorenz Meier,   <lm@inf.ethz.ch>
 * @author ZhangPeng,      <zhangpengbjut@sina.com>
 *
 */


#ifndef ARDUROVER_INTERFACE_H_
#define ARDUROVER_INTERFACE_H_

// ------------------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------------------

#include "generic_port.h"
#include "common.h"

#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h> // This uses POSIX Threads
#include <unistd.h>  // UNIX standard function definitions
#include <mutex>

#include <common/mavlink.h>

void* start_ardurover_interface_read_thread(void *args);
void* start_ardurover_interface_write_thread(void *args);

class Ardurover_Interface
{

public:

	Ardurover_Interface();
	Ardurover_Interface(Generic_Port *port_);
	~Ardurover_Interface();

	char reading_status;
	char writing_status;
	char control_status;
    uint64_t write_count;

    int system_id;
	int ardurover_id;
	int companion_id;

	Mavlink_Messages current_messages;
	mavlink_set_position_target_local_ned_t initial_position;

	void update_setpoint(mavlink_set_position_target_local_ned_t setpoint);
	void read_messages();
	int  write_message(mavlink_message_t message);

	int	 arm_disarm( bool flag );
	void enable_offboard_control();
	void disable_offboard_control();

	void start();
	void stop();

	void start_read_thread();
	void start_write_thread(void);

	void handle_quit( int sig );

	int  do_set_mode(int mode_number);
	int  goto_location(double lon,double lat,float alt);


private:

	Generic_Port *port;

	bool time_to_exit;

	pthread_t read_tid;
	pthread_t write_tid;

	struct {
		std::mutex mutex;
		mavlink_set_position_target_local_ned_t data;
	} current_setpoint;

	void read_thread();
	void write_thread(void);

	int toggle_offboard_control( bool flag );
	void write_setpoint();

};



#endif // ARDUROVER_INTERFACE_H_


