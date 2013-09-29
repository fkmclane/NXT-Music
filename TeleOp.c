#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     FrontLeft,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     FrontRight,    tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     BackLeft,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     BackRight,     tmotorTetrix, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "common.c"

//Constants
#include "constants.h"

//Initialize our globals
byte drive_scale = DRIVE_HIGH; //Used to scale down robot movements

bool forwards = true; //Used for direction locking
bool sideways = true;

task joystickControl(){ //Asynchronous task for joystick control
	while(true){
		//Joystick 1 - Driver

		//Deadband between -15 to 15
		//Scale linearly for all other values
		float x1, y1, x2, y2;

		//Check each axis for deadband
		if(joystick.joy1_x1 > 15 || joystick.joy1_x1 < -15)
			x1 = joystick.joy1_x1 / 128.0 * 100;
		else
			x1 = 0;

		if(joystick.joy1_y1 > 15 || joystick.joy1_y1 < -15)
			y1 = joystick.joy1_y1 / 128.0 * 100;
		else
			y1 = 0;

		if(joystick.joy1_x2 > 15 || joystick.joy1_x2 < -15)
			x2 = joystick.joy1_x2 / 128.0 * 100;
		else
			x2 = 0;

		if(joystick.joy1_y2 > 15 || joystick.joy1_y2 < -15)
			y2 = joystick.joy1_y2 / 128.0 * 100;
		else
			y2 = 0;

		//Set the motors
		motor[FrontLeft] = drive_scale * (x2 + (y1 + x1));
		motor[FrontRight] = drive_scale * (x2 + (-y1 + x1));
		motor[BackLeft] = drive_scale * (x2 + (y1 + -x1));
		motor[BackRight]= drive_scale * (x2 + (-y1 + -x1));
	}
}

task main() {
	//Initialize
	motor[FrontLeft] = motor[FrontRight] = motor[BackLeft] = motor[BackRight] = 0; //Turn off the motors
	nMotorEncoder[FrontLeft] = nMotorEncoder[FrontRight] = nMotorEncoder[BackLeft] = nMotorEncoder[BackRight] = 0; // Might as well reset the encoders too

	waitForStart();
	StartTask(joystickControl); //Go ahead and start critical joystick functions in their own task

	while(true) {
		//Joystick 1 - Driver

		if(joy1Btn(6)) //If the driver is pressing button 6, scale down robot movements
			drive_scale = DRIVE_LOW;
		else //Else leave at full speed
			drive_scale = DRIVE_HIGH;

		if(joy1Btn(5)) //If the driver is pressing button 5, lock to y-axis movement
			sideways = false;
		else
			sideways = true;

		if(joy1Btn(7)) //If the driver is pressing button 7, lock to x-axis movement
			forwards = false;
		else
			forwards = true;
	}
}
