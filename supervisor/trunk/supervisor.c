/*****************************************************************/
//  supervisor.c
//	Created by Richard Towers at 16:40 on 08/02/2011
//	Copyright Durham University 2011, all rights reserved
//
/*****************************************************************/

#include "supervisor.h"


int main(void)
{
	rtRobot bots[nROBOTS]={{"192.168.1.2",NULL,NULL,RED}};
	rtGeneration gens[GENERATIONS];
	short int ind, gen, bot; char command[100], filename[30];
	
	//Connect to robots
	for (bot = 0; bot < nROBOTS; bot ++) bots[bot].socket=connectToClient(bots[bot].ip);
	
	//Set up fitness tracker
	
	//Create initial population of genotype files
	createInitialGenes(POP_SIZE, &gens[0]);
	
	//For each generation starting at 0
	for (gen = 0; gen < GENERATIONS; gen ++)
	{
		//For each individual in the population
		for (ind = 0; ind < POP_SIZE; ind ++)
		{
			//Send the file to the individual
			strcpy(filename,gens[gen].inds[ind].geneFile);
			sprintf(command, "scp %s root@%s:Genotypes",filename, bots[0].ip);
			system(command);
			//Tell the individual to get going
			send(bots[0].socket, filename, strlen(filename)+1, 0);
			
			//Monitor and then store its fitness
			sleep(10);
			//Stop the motors (move back to start position later)
			send(bots[0].socket, "Stop Motors", 11, 0);
			
		}
		//When entire population have fitness values
			//Reproduce, Crossover mutate
		//Next Generation
	}
	//When enough generations have elapsed
		//Print useful data
		//END
}

void createInitialGenes(int population, rtGeneration* generation)
{
	short int* weights;
	int theIndividual, theWeight, row, column;
	
	weights=(short*)malloc(sizeof(short)*nNodes*nNodes);
	
	if(access("./Genotypes",F_OK)!=0) system("mkdir Genotypes");
	for (theIndividual=0; theIndividual < POP_SIZE; theIndividual++)
	{
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
			}
		}
		//Create a file for this individual
		sprintf(generation[0].inds[theIndividual].geneFile, "Genotypes/Gen0Ind%d.txt", theIndividual);
		//Store genotype in file...
		writeGenotype(generation[0].inds[theIndividual].geneFile, weights);
	}
	free(weights);
}
