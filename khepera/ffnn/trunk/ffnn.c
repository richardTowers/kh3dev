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
	float weightsIH[INPUTS][HIDDENS] = {{0.5,0.5}	,
										{-0.2,0.2},
										{-0.4,0.2},
										{-1,-0.2}	,
										{-0.2,-1}	,
										{0.2,-0.4},
										{0.2,-0.2},
										{0.5,0.5}	,
										{1,1}	};
	
	float weightsHO[HIDDENS][OUTPUTS] = {{1,0}		,
																			 {0,1}		};
	float *outputs;

	int leftMotorSpeed, rightMotorSpeed;

	startLoggingErrors();
	
	//Initialise Robot
	printf("Initialising Robot...\n");
	initialiseRobot();
	printf("Robot Initialised\n\n");
	for(i=0;i<10;i++)
	{
		printf("Program will start in %d seconds\n", 10-i);
		sleep(1);
	}
		
	while(1)
	{
		//Get IR Values from Robot
		//printf("Getting IR Values...\nValues: ");		
		for(i=0;i<INPUTS;i++)
		{
			inputs[i]=getIRRange(i);
			//printf("%d: %f, ", i, inputs[i]);
		}
		//printf("\nGot IR Values\n\n");
		
		//printf("Running Neural Net to get Outpus\n");
		//Send IR Values to NN and get outputs
		outputs = ffnn(inputs, weightsIH, weightsHO);
		//printf("Got Outputs...\n\n");

		//Set	Motor values to outputs
		leftMotorSpeed = (int)(outputs[1]*20000) + FORWARD_BIAS;
		rightMotorSpeed = (int)(outputs[0]*20000) + FORWARD_BIAS;
			//DONE WITH OUTPUTS NOW
			free(outputs);
		printf("\nLeft Motor: %d, Right Motor: %d\n", leftMotorSpeed, rightMotorSpeed);
		//Set Motors
		setMotor(LEFT_MOTOR, leftMotorSpeed);
		setMotor(RIGHT_MOTOR, rightMotorSpeed);
		
		//Just so we don't get ahead of oursleves...
		usleep(20000);
	}
	
	stopLoggingErrors();
	return 0;
}
