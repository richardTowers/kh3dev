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
	
	int theIndividual=0, theGeneration=0, theWeight, robotSocket,i, row, column, nNodes;
	char command[100], filename[30];
	nInputs=INPUTS;
	nHiddens=HIDDENS;
	nOutputs=OUTPUTS;
	nNodes=INPUTS+HIDDENS+OUTPUTS;
	
	FILE *file;
	
	startLoggingErrors();
	
	weights=malloc( sizeof(int)*nNodes*nNodes );
	//Connect to Robot
	robotSocket=connectToClient(ROBOT_IP);
	
	//Set up fitMonitor
	
	//Create initial population of random genotype files
	if(access("./Genotypes",F_OK)!=0) system("mkdir Genotypes");
	for (theIndividual=0; theIndividual < POP_SIZE; theIndividual++)
	{
		//Assign architecure
		nInputs=INPUTS;
		nHiddens=HIDDENS;
		nOutputs=OUTPUTS;
		nNodes=INPUTS+HIDDENS+OUTPUTS;

		//Assign Weights
		for (row = 0; row < nNodes; row ++)
		{
			for (column = 0; column < nNodes; column ++)
			{
				theWeight=row*nNodes+column;
				if(row<nInputs)
				{
					if(column<nInputs)
					{
						if(rand()%pBASE<pII) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else if(column<nInputs+nHiddens)
					{
						if(rand()%pBASE<pIH) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else
					{
						if(rand()%pBASE<pIO) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
				}
				else if(row<nInputs+nHiddens)
				{
					if(column<nInputs)
					{
						if(rand()%pBASE<pHI) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else if(column<nInputs+nHiddens)
					{
						if(rand()%pBASE<pHH) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else
					{
						if(rand()%pBASE<pHO) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
				}
				else
				{
					if(column<nInputs)
					{
						if(rand()%pBASE<pOI) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else if(column<nInputs+nHiddens)
					{
						if(rand()%pBASE<pOH) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else
					{
						if(rand()%pBASE<pOO) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
				}
				//else weights[theWeight]=0;
			}
		}
		/*for (theWeight = 0; theWeight < nNodes*nNodes; theWeight ++)
		{
			if(rand()%169<22) weights[theWeight]=rand()%2048-1024;
			else weights[theWeight]=0;
		}*/
		//Create a file for this individual
		sprintf(generation[0].individual[theIndividual].geneFile, "Genotypes/Gen0Ind%d.txt", theIndividual);
		//Store genotype in file...
		writeGenotype(generation[0].individual[theIndividual].geneFile);
	}

	for (theGeneration = 0; theGeneration < GENERATIONS; theGeneration ++)
	{
		for (theIndividual = 0; theIndividual < POP_SIZE; theIndividual ++)
		{
			strcpy(filename,generation[theGeneration].individual[theIndividual].geneFile);
			//Send genotype to robot
			sprintf(command, "scp %s root@%s:%s",filename, ROBOT_IP, GENE_FOLDER);
			if(system(command)!=0)warning(WARNING_SCP_FAIL);

			//Send Filename to robot
			send(robotSocket, filename, strlen(filename)+1, 0);
		
			//Monitor robots fitness
				//This will take a long time to return
				sleep(10);
				//Store robots fitness
					//generation[theGeneration].individual[theIndividual].fitness=someFitness;
			
			//Send 'Stop Motors' to robot
			send(robotSocket, "Stop Motors", 11, 0);
			sleep(1);
		}
		//Reproduce, crossover mutate
	}
	
	stopLoggingErrors();
}
