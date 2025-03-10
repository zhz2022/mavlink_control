C-UART Interface Example
========================

This is a simple MAVLink to UART interface example for *nix systems that can allow communication between Pixhawk and an offboard computer.

This example will receive one MAVLink message and send one MAVLink message.


Building
========

```
$ cd c_uart_interface_example/
$ make
```

Hardware Setup
=========

Connect the USB programming cable to your Pixhawk.  

If you want to be able to interact with this example in Pixhawk's NuttX shell, you'll need a Telemetry Radio or an FTDI developer's cable.  See the Exploration section below for more detail.

Also Note: Using a UART (serial) connection should be preferred over using the USB port for flying systems.  The reason being that the driver for the USB port is much more complicated, so the UART is a much more trusted port for flight-critical functions.  To learn how this works though the USB port will be fine and instructive.

Execution
=========

You have to pick a port name, try searching for it with 
```

$ ls /dev/ttyACM* 
$ ls /dev/ttyUSB*
```

Alternatively, plug in Pixhawk USB cable again and issue the command:
```
$ dmesg
```
The device described at the bottom of dmesg's output will be the port on which the Pixhawk is mounted. 

The Pixhawk USB port will show up on a `ttyACM*`, an FTDI cable will show up on a `ttyUSB*`.


Run the example executable on the host shell:

```
$ cd c_uart_interface_example/
$ ./mavlink_control -d /dev/ttyACM0
```

To stop the program, use the key sequence `Ctrl-C`.

Here's an example output:

```
OPEN PORT
Connected to /dev/ttyUSB0 with 57600 baud, 8 data bits, no parity, 1 stop bit (8N1)

START READ THREAD 

CHECK FOR HEARTBEAT
Found

GOT VEHICLE SYSTEM ID: 1
GOT AUTOPILOT COMPONENT ID: 50

INITIAL POSITION XYZ = [ 8.2935 , -1.1447 , -0.7609 ] 
INITIAL POSITION YAW = 2.1539 

START WRITE THREAD 

ENABLE OFFBOARD MODE

SEND OFFBOARD COMMANDS
POSITION SETPOINT XYZ = [ 3.2935 , -6.1447 , -0.7609 ] 
POSITION SETPOINT YAW = 2.1539 
0 CURRENT POSITION XYZ = [  8.2935 , -1.1447 , -0.7609 ] 
1 CURRENT POSITION XYZ = [  8.2935 , -1.1447 , -0.7609 ] 
2 CURRENT POSITION XYZ = [  8.2524 , -1.1444 , -0.7667 ] 
3 CURRENT POSITION XYZ = [  8.2205 , -1.1431 , -0.7747 ] 
4 CURRENT POSITION XYZ = [  8.1920 , -1.1421 , -0.7737 ] 
5 CURRENT POSITION XYZ = [  8.1920 , -1.1421 , -0.7737 ] 
6 CURRENT POSITION XYZ = [  8.1539 , -1.1414 , -0.7847 ] 
7 CURRENT POSITION XYZ = [  8.1522 , -1.1417 , -0.7820 ] 

DISABLE OFFBOARD MODE

READ SOME MESSAGES 
Got message LOCAL_POSITION_NED (spec: https://mavlink.io/en/messages/common.html#LOCAL_POSITION_NED)
    pos  (NED):  8.152975 -1.141093 -0.784075 (m)
Got message HIGHRES_IMU (spec: https://mavlink.io/en/messages/common.html#HIGHRES_IMU)
    ap time:     3611390110 
    acc  (NED):   0.005503  0.044659 -9.740363 (m/s^2)
    gyro (NED):  -0.003064  0.003857  0.000005 (rad/s)
    mag  (NED):  -0.117767 -0.335362 -0.253204 (Ga)
    baro:        1020.519958 (mBar) 
    altitude:    -60.341393 (m) 
    temperature: 46.779999 C 

CLOSE THREADS

CLOSE PORT
```

Exploration
===========

There are a few things to explore past this example.

First you can connect via:
* a [Telemetry Radio](https://docs.px4.io/en/telemetry/) on TELEM1 or 2
* an [FTDI cable](https://www.sparkfun.com/products/9718) on TELEM2 or Serial 4

> **Note** 
> * Serial 5 can't be used to receive data without reconfiguration (its receive pin is occupied by a second NuttX shell).
> * TELEM1 is typically dedicated to Telemetry Radio, but if you're using another port you will need to make sure it is not configured for use by another peripheral.
> * If using FTDI with a TELEM port, connect all the pins to corresponding pins on port.
> * If using FTDI with SERIAL4 connect the TX, RX GND and 5V pins (CTS, RTS need not be connected).


With this you'll be able to start a second port for communication, and leave the USB port available for viewing prints in the NuttX shell.  

For steps 2 and 3 from the above tutorial, you'll use a different port.  On the off-board computer side, the port might now be `/dev/ttyUSB0`.  On the Pixhawk side, here the port mappings are in the table below.

| PX4 UART | NuttX UART |
|----------|------------|
| Telem 1  | /dev/ttyS1 |
| Telem 2  | /dev/ttyS2 |
| Serial 4 | /dev/ttyS6 |

Now add a print statement in the Pixhawk Firmware to see received messages.  Build and upload this to Pixhawk.

```
[Firmware/src/modules/mavlink/mavlink_receiver.cpp]
/* if read failed, this loop won't execute */
for (ssize_t i = 0; i < nread; i++) {
	if (mavlink_parse_char(_mavlink->get_channel(), buf[i], &msg, &status)) {

		/* --- REPORT HANDLING OF MESSAGE --- */
		printf("\n");
		printf("HANDLE MESSAGE\n");
		printf("MSGID:%i\n",msg.msgid);

		/* handle generic messages and commands */
		handle_message(&msg);
```

Open the system terminal as described here: https://dev.px4.io/en/debug/system_console.html 

On the off-board side, in another terminal run the `c_uart_interface_example` executable. You should see output in the NuttX shell similar to this:

```
HANDLE MESSAGE
MSGID:76

HANDLE MESSAGE
MSGID:84

(...)

HANDLE MESSAGE
MSGID:84

HANDLE MESSAGE
MSGID:76
```

Past this, you can:
- Modify the received message data type
- Modify the sent message data type

Simulation
===========

There is also the possibility to connect this example to the simulator using:

```
$ ./mavlink_control -u 127.0.0.1 -a
```
The -a argument enables arming, takeoff and landing of the copter. Use this argument with care on a real copter!

Usage:
===========

```
$ sudo ./my_mavlink_control /dev/ttyUSB0
./my_mavlink_control -u 127.0.0.1 -p 14550


```

simulate:
===========

```
cd ~/ardupilot/ArduCopter
output add 127.0.0.1:14552

sim_vehicle.py --console --map
mode guided
arm throttle
takeoff 40
setspeed 2
velocity 100 0 0

```
outdoor test:
===========
2023/11/24 guided auto RTL 接口测试通过，实现返航，起飞、降落功能，前后左右控制及上传航点功能待实现。