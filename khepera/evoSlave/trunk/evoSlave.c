/*****************************************************************/
//  evoSlave.c
//	Created by richard at  9:49 on 15/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "evoSlave.h"

int main(int argc, char * argv[])
{
	int host, bytesRecieved;
	char buffer[MAXDATASIZE];
	pid_t processID = 0;
	int status;
	
	#ifndef TESTING
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
					if(processID>0){ kill(processID, SIGKILL); wait(&status); NNdealloc();}
					//Fork process
					processID = fork();
					if(processID==CHILD) childProcess(buffer);
				}
				else if(strcmp(buffer,"Stop Motors")==0)
				{
					//If there's a child process, kill it
					if(processID>0){ kill(processID, SIGKILL); wait(&status); NNdealloc();}
					//Stop Motors
					printf("Stopping Motors\n");
					#ifndef TESTING
						stopAllMotors();
					#endif
				}
			}
		}
	#else
		childProcess("testGene.txt");
	#endif
	return 0;
}

void childProcess(char *filename)
{
	unsigned short int i;
	float inputs[INPUTS];
	/*float weightsIH[INPUTS][HIDDENS] = {{0.2,0.2},
										{-0.2,0.2},
										{-0.4,0.2},
										{-1,0.5},
										{0.5,-1},
										{0.2,-0.4},
										{0.2,-0.2},
										{0.2,0.2},
										{0.4,0.4}};

	float weightsHO[HIDDENS][OUTPUTS] = {{1,0},
										{0,1}};*/
	float *outputs;

	int leftMotorSpeed=0, rightMotorSpeed=0;
	
	readGenotype(filename);
	printf("Read Genotype...\n");
	printGenotype();
	//Allocate memory for NN:
	NNalloc(nInputs+nOutputs+nHiddens);
	for(;;)
	{
		for(i=0;i<INPUTS;i++)
		{
			#ifndef TESTING
				inputs[i]=getIRRange(i);
			#else
				inputs[i]=(rand()%2048-1024)/1024.0;
			#endif
		}
		
		//Send IR Values to NN and get outputs
		//outputs = ffnn(inputs, weightsIH, weightsHO);
		outputs = dtrnn(inputs, weights);

		//Set	Motor values to outputs
		leftMotorSpeed = (int)(outputs[1]*20000);
		rightMotorSpeed = (int)(outputs[0]*20000);
		//DONE WITH OUTPUTS NOW
		free(outputs);
		#ifndef TESTING
			//Set Motors
			setMotor(LEFT_MOTOR, leftMotorSpeed);
			setMotor(RIGHT_MOTOR, rightMotorSpeed);
		#endif
		
		//Just so we don't get ahead of oursleves...
		usleep(20000);
	}
}
