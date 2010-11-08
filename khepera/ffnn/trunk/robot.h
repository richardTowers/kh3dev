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

//Globals
static knet_dev_t * khepera;
static knet_dev_t * leftMotor;
static knet_dev_t * rightMotor;

void initialiseRobot(void);
unsigned int getIRRange(int sensorNumber);
void setMotor(char theMotor);

#endif //INC_ROBOT
