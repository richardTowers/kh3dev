/*****************************************************************/
/*  robot.h
/*	Created by richard at 12:28 on 06/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/
#ifndef INC_ROBOT
#define INC_ROBOT

#include <korebot/korebot.h>
#include "errors.h"

#define LEFT_MOTOR 'l'
#define RIGHT_MOTOR 'r'

#define MAXIMUM_SPEED 30000

//Globals
static knet_dev_t * khepera;
static knet_dev_t * leftMotor;
static knet_dev_t * rightMotor;

//Initialises the robot. A bit of a black box I'm afraid, so far no bugs suspected.
void initialiseRobot(void);

//Returns an IR range scaled to be from 0(∞) to 1(touching)
float getIRRange(int sensorNumber);

//Sets the motor speed, a reasonable speed is found with theSpeed=20,000.
void setMotor(char theMotor, int theSpeed);

//Stop the motors
void stopAllMotors(void);

#endif //INC_ROBOT
