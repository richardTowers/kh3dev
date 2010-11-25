/*****************************************************************/
/*  evoSlave.c
/*	Created by richard at  9:49 on 15/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "evoSlave.h"

int main(int argc, char * argv[])
{
	int host, bytesRecieved;
	char buffer[MAXDATASIZE];
	pid_t processID = 0;
	int status;
	
	
	//Connect to host
	host = connectToHost(HOST_IP);
	initialiseRobot();

	for(;;)
	{
		//Wait for Signal:		
		if ((bytesRecieved = recv(host, buffer, MAXDATASIZE-1, 0)) == -1)
		{
	  	  perror("recv");
	  	  exit(1);
		}
		else if (bytesRecieved == 0);
		else	//Got Signal:
		{
			buffer[bytesRecieved] = '\0';
			printf("Recieved Signal: '%s'\n",buffer);
			
			if(strncmp(buffer,"Genotype",5)==0)
			{
				//If there's a child process, kill it
				if(processID>0){ kill(processID, SIGKILL); wait(&status);}
				//Fork process
				processID = fork();
				if(processID==CHILD) childProcess(buffer);
			}
			else if(strcmp(buffer,"Stop Motors")==0)
			{
				//If there's a child process, kill it
				if(processID>0){ kill(processID, SIGKILL); wait(&status);}
				//Stop Motors
				printf("Stopping Motors\n");
				stopAllMotors();
			}
		}
	}
	return 0;
}

void childProcess(char *filename)
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
	
	printf("Running...\n");
	//readGenotype(filename);
	for(;;)
	{
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
		leftMotorSpeed = (int)(outputs[1]*20000);
		rightMotorSpeed = (int)(outputs[0]*20000);
			//DONE WITH OUTPUTS NOW
			free(outputs);
		//printf("\nLeft Motor: %d, Right Motor: %d\n", leftMotorSpeed, rightMotorSpeed);
		//Set Motors
		setMotor(LEFT_MOTOR, leftMotorSpeed);
		setMotor(RIGHT_MOTOR, rightMotorSpeed);
		
		//Just so we don't get ahead of oursleves...
		usleep(20000);
	}
}
