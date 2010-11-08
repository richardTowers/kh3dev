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
	unsigned int inputs[INPUTS];
	double weightsIH[INPUTS][HIDDENS], weightsHO[HIDDENS][OUTPUTS];
	double *outputs;

	startLoggingErrors();
	
	//Initialise Robot
	printf("Initialising Robot...\n");
	initialiseRobot();
	printf("Robot Initialised\n\n");
		

		//Get IR Values from Robot
		printf("Getting IR Values...\n");		
		for(i=0;i<INPUTS;i++)
		{
			inputs[i]=getIRRange(i);
		}
		printf("Got IR Values\n\n");
		
		//Send IR Values to NN and get outputs
		outputs = ffnn((double*)inputs, weightsIH, weightsHO);
		
		//Set	Motor values to outputs
		for(i=0;i<OUTPUTS;i++)
		{
			printf("%4.2f, ",outputs[i]);
		}
		printf("\n");

	stopLoggingErrors();
	return 0;
}
