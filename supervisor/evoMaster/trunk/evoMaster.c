/*****************************************************************/
/*  evoMaster.c
/*	Created by richard at 16:14 on 18/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "evoMaster.h"

int main(int argc, char * argv[])
{
	struct generation generation[GENERATIONS];
	
	int theIndividual=0, theGeneration=0, theWeight, robotSocket;
	char command[50];
	nInputs=INPUTS;
	nHiddens=HIDDENS;
	nOutputs=OUTPUTS;
	
	FILE *file;
	
	weights=malloc( sizeof(int)*(nInputs+nHiddens+nOutputs)*(nInputs+nHiddens+nOutputs) );
	//Connect to Robot
	robotSocket=connectToClient(ROBOT_IP);
	
	//Create initial population of random genotype files
	for (theIndividual=0; theIndividual < POP_SIZE; theIndividual++)
	{
		//Assign architecure
		nInputs=INPUTS;
		nHiddens=HIDDENS;
		nOutputs=OUTPUTS;
		printf("!\n");
		//Assign Weights
		for (theWeight = 0; theWeight < (nInputs+nHiddens+nOutputs)*(nInputs+nHiddens+nOutputs); theWeight ++)
		{
			weights[theWeight]=rand()%1024;
			printf("%d\n",theWeight);
		}
		//Create a file for this individual
		sprintf(generation[0].individual[theIndividual].geneFile, "Gen0Ind%d.txt", theIndividual);
		//Store genotype in file...
		writeGenotype(generation[0].individual[theIndividual].geneFile);
	}

	for (theGeneration = 0; theGeneration < GENERATIONS; theGeneration ++)
	{
		for (theIndividual = 0; theIndividual < POP_SIZE; theIndividual ++)
		{
			//Send genotype to robot
			sprintf(command, "scp %s root@%s:%s",
			generation[theGeneration].individual[theIndividual].geneFile,
			ROBOT_IP,
			GENE_FOLDER);
			system(command);
			
			//Send 'File Sent!' to robot
			send(robotSocket, "File Sent!", 10, 0);
			
			//Monitor robots fitness
				//This will take a long time to return
				sleep(10);
				//Store robots fitness
				
			//Send 'Stop Motors' to robot
			send(robotSocket, "Stop Motors", 11, 0);
			
		}
		//Reproduce, crossover mutate
	}
	
}
