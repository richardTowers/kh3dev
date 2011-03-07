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
	//semaphor expectedSignal=genotype;
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
					if(processID>0){ kill(processID, SIGKILL); wait(&status);}
					//Fork process
					processID = fork();
					if(processID==CHILD) childProcess(buffer);
				}
				else if(strcmp(buffer, "Retreat")==0)
				{
					//If there's a child process, kill it
					if(processID>0){ kill(processID, SIGKILL); wait(&status); stopAllMotors();}
					processID=fork();
					if(processID==CHILD) childProcess("Retreat");
				}
				else if(strcmp(buffer,"Stop Motors")==0)
				{
					//If there's a child process, kill it
					if(processID>0){ kill(processID, SIGKILL); wait(&status);}
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
	unsigned short int i, count;
	short *inputs;
	short *neuronStates;
	short timeStep=2000;	//This timeStep will be caculated dynamically
	clock_t start;

	int leftMotorSpeed=0, rightMotorSpeed=0;
	if(strcmp(filename, "Retreat")==0)
	{
		retreat();
	}
	else
	{
		readGenotype(filename);
		printf("Read Genotype %s...\n", filename);
		printGenotype();
	
		//Initially all neurons have 0 states...
		neuronStates = calloc(nNeurons, sizeof(float));
		inputs = calloc(nNeurons, sizeof(short));

		//Just wait a few seconds for the fitness monitor to catch up...
		sleep(4);
	
		initSigmoid();
		for(;;)
		{
			start=clock();
			for (count = 0; count < 100; count += 1)
			{
				for(i=0;i<INPUTS;i++)
				{
					#ifndef TESTING
						inputs[i]=getIRRange(i)*(2*SHRT_MAX/4096);
					#else
						inputs[i]=(rand()%4096);
					#endif
				}
		
				//Send IR Values to NN and get new neuron states:
				//ctrnn(neuronStates, nNeurons, inputs, biases, tConsts, weights, timeStep);
				ffnn(neuronStates, nNeurons, inputs, weights);
	/*			printf("Neuron States: ");*/
	/*			for (i = 0; i < nNeurons; i += 1) printf("%hd, ", neuronStates[i]);*/
	/*			printf("\n");*/
			
				//Set Motor values to outputs
				leftMotorSpeed = (int)(neuronStates[nNeurons-OUTPUTS+1]);
				rightMotorSpeed = (int)(neuronStates[nNeurons-OUTPUTS]);
		
				#ifndef TESTING
					//Set Motors
					setMotor(LEFT_MOTOR, leftMotorSpeed);
					setMotor(RIGHT_MOTOR, rightMotorSpeed);
				#endif
			}
			timeStep=(int)((clock()-start)/100.0+0.5);
		}
	}
}

void retreat(void)
{
	float inputs[INPUTS], *neuronState, *neuronStatePrev, *outputs;
	int i, leftMotorSpeed=0, rightMotorSpeed=0;
	float retreatWeights[169]={
	0,0,0,0,0,0,0,0,0,0.6,0.6,0,0, 
	0,0,0,0,0,0,0,0,0,-0.1,0.3,0,0, 
	0,0,0,0,0,0,0,0,0,-0.3,0.3,0,0, 
	0,0,0,0,0,0,0,0,0,-0.9,-0.1,0,0, 
	0,0,0,0,0,0,0,0,0,-0.1,-0.9,0,0, 
	0,0,0,0,0,0,0,0,0,0.3,-0.3,0,0,
	0,0,0,0,0,0,0,0,0,0.3,-0.1,0,0,
	0,0,0,0,0,0,0,0,0,0.6,0.6,0,0,
	0,0,0,0,0,0,0,0,0,1.1,1.1,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1.1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1.1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0};
	short int pre, post, neuron;
	NNlayer preLayer, postLayer;
	nInputs=INPUTS; nHiddens=2; nOutputs=OUTPUTS;
	short int nNeurons = nInputs+nHiddens+nOutputs;
	
	outputs=malloc(nOutputs*sizeof(float));
	neuronState=calloc(nNeurons,sizeof(float));
	neuronStatePrev=malloc(nNeurons*sizeof(float));
	
	printf("Retreating...");
	for(;;)
	{
		for(i=0;i<INPUTS;i++)
		{
			inputs[i]=getIRRange(i)/4096.0;
			//printf("%d: %f ", i, inputs[i]);
		}
		for (pre = 0; pre < nNeurons; pre ++)
		{
			neuronStatePrev[pre]=neuronState[pre];
			if(pre<nInputs) preLayer=input;
			else if(pre<nInputs+nHiddens) preLayer=hidden;
			else preLayer=output;

			if(preLayer==input) neuronState[pre]=inputs[pre];
			else neuronState[pre]=0;
		
			for (post = 0; post < nNeurons; post ++)
			{
				if(post<nInputs) postLayer=input;
				else if(post<nInputs+nHiddens) postLayer=hidden;
				else postLayer=output;

				if(preLayer<=postLayer) neuronState[pre]+=retreatWeights[post*nNeurons+pre]*neuronStatePrev[post];
				else neuronState[pre]+=retreatWeights[post*nNeurons+pre]*neuronState[post];
			}
			neuronState[pre]=rSigmoid(neuronState[pre]);
			//printf("%d: %f ", pre, neuronState[pre]);
		}
		
		for (neuron = nInputs+nHiddens; neuron < nNeurons; neuron ++)
		{
			outputs[neuron-(nInputs+nHiddens)] = neuronState[neuron];
		}
		//printf("output1:%f, output2:%f\n", outputs[0], outputs[1]);

		leftMotorSpeed = (int)(neuronState[nNeurons-OUTPUTS+1]*40000);
		rightMotorSpeed = (int)(neuronState[nNeurons-OUTPUTS]*40000);
		//printf("leftMotorSpeed:%d, rightMotorSpeed:%d\n", leftMotorSpeed, rightMotorSpeed);
	
		setMotor(LEFT_MOTOR, leftMotorSpeed);
		setMotor(RIGHT_MOTOR, rightMotorSpeed);
	}
}
