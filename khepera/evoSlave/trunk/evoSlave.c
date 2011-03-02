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
	semaphor expectedSignal=genotype;
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
			
				if((strncmp(buffer,"Genotype",5)==0) && (expectedSignal==genotype))
				{
					expectedSignal=stopMotors;
					//If there's a child process, kill it
					if(processID>0){ kill(processID, SIGKILL); wait(&status);}	//FREE
					//Fork process
					processID = fork();
					if(processID==CHILD) childProcess(buffer);
				}
				else if((strcmp(buffer,"Stop Motors")==0) && (expectedSignal==stopMotors))
				{
					expectedSignal=genotype;
					//If there's a child process, kill it
					if(processID>0){ kill(processID, SIGKILL); wait(&status);}	//FREE
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
	short inputs[INPUTS];
	short *neuronStates;
	short outputs[OUTPUTS];
	short timeStep=CLOCKS_PER_SEC/1000;	//This timeStep will be much too small, but we'll calculate it dynamically.
	clock_t start;

	int leftMotorSpeed=0, rightMotorSpeed=0;
	
	readGenotype(filename);
	printf("Read Genotype...\n");
	printGenotype();
	
	//Initially all neurons have 0 states...
	neuronStates = calloc(nNeurons, sizeof(float));

	//Just wait a few seconds for the fitness monitor to catch up...
	sleep(4);
	
	for(;;)
	{
		start=clock();
		for(i=0;i<INPUTS;i++)
		{
			#ifndef TESTING
				inputs[i]=getIRRange(i);
			#else
				inputs[i]=(rand()%4096);
			#endif
		}
		
		//Send IR Values to NN and get outputs
		//outputs = ffnn(inputs, weightsIH, weightsHO);
		//outputs = dtrnn(inputs, weights);
		ctrnn(neuronStates, nNeurons, inputs, biases, tConsts, weights, timeStep);

		//Set Motor values to outputs
		leftMotorSpeed = (int)(outputs[1]*20000);
		rightMotorSpeed = (int)(outputs[0]*20000);
		//DONE WITH OUTPUTS NOW
		free(outputs);
		#ifndef TESTING
			//Set Motors
			setMotor(LEFT_MOTOR, leftMotorSpeed);
			setMotor(RIGHT_MOTOR, rightMotorSpeed);
		#endif
		timeStep=clock()-start;
	}
}
