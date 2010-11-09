/*****************************************************************/
/*  ffnn.c
/*	Created by richard at 12:06 on 06/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "ffnn.h"

int main(void)
{
	unsigned short int i;
	float inputs[INPUTS];
	float weightsIH[INPUTS][HIDDENS] = {{0.2,0.2}	,
																			{-0.2,0.2},
																			{-0.4,0.2},
																			{-1,0.5}	,
																			{0.5,-1}	,
																			{0.2,-0.4},
																			{0.2,-0.2},
																			{0.2,0.2}	,
																			{0.4,0.4}	};
	
	float weightsHO[HIDDENS][OUTPUTS] = {{1,0}		,
																			 {0,1}		};
	float *outputs;

	int leftMotorSpeed, rightMotorSpeed;

	startLoggingErrors();
	
	//Initialise Robot
	printf("Initialising Robot...\n");
	initialiseRobot();
	printf("Robot Initialised\n\n");
		
	while(true)
	{
		//Get IR Values from Robot
		printf("Getting IR Values...\n");		
		for(i=0;i<INPUTS;i++)
		{
			inputs[i]=getIRRange(i);
		}
		printf("Got IR Values\n\n");
		
		printf("Running Neural Net to get Outpus\n");
		//Send IR Values to NN and get outputs
		outputs = ffnn(inputs, weightsIH, weightsHO);
		printf("Got Outputs...\n\n");

		//Set	Motor values to outputs
		leftMotorSpeed = (int*)(outputs[0]*200);
		rightMotorSpeed = (int*)(outputs[1]*200);
		printf("Left Motor: %d, Right Motor: %d\n", leftMotorSpeed, rightMotorSpeed);
		//Don't want to set motors just yet...
		
		//Just so we don't get ahead of oursleves...
		usleep(20000);
	}
	
	stopLoggingErrors();
	return 0;
}
