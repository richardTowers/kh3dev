/*****************************************************************/
//  supervisor.c
//	Created by Richard Towers at 16:40 on 08/02/2011
//	Copyright Durham University 2011, all rights reserved
//
/*****************************************************************/

#include "supervisor.h"


int main(void)
{
	rtRobot bots[nROBOTS]={{"192.168.1.2",NULL,NULL,RED,{0,0},{0,0}}};
	rtGeneration gens[GENERATIONS];
	short int ind=0, gen=0, bot; char command[100], filename[30];
	
	#ifndef TESTING
		//Connect to robots
		for (bot = 0; bot < nROBOTS; bot ++) bots[bot].socket=connectToClient(bots[bot].ip);
	#endif
	
	//Set up fitness tracker
	setupTracker(nROBOTS, bots);

	//Create initial population of genotype files
	createInitialGenes(POP_SIZE, &gens[0]);
	
	#ifndef TESTING
	//For each generation starting at 0
	for (gen = 0; gen < GENERATIONS; gen ++)
	{
		#endif
		//For each individual in the population
		for (ind = 0; ind < POP_SIZE; ind ++)
		{
			#ifndef TESTING
			//Send the file to the individual
			strcpy(filename,gens[gen].inds[ind].geneFile);
			sprintf(command, "scp %s root@%s:Genotypes",filename, bots[0].ip);
			system(command);
			//Tell the individual to get going
			send(bots[0].socket, filename, strlen(filename)+1, 0);
			#endif
			//Monitor and then store its fitness
			testIndividualOnRobot(&(gens[gen].inds[ind]), bots[0]);
			#ifndef TESTING
			//Stop the motors:
			send(bots[0].socket, "Stop Motors", 11, 0);
			
			#endif
		}
		//When entire population have fitness values
			//Reproduce, Crossover mutate
		//Next Generation
		#ifndef TESTING
	}
	#endif
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

void reproduce(int population, int gen, rtGeneration* parentGen, rtGeneration* childGen)
{
	short int *weights;
	short int child, parent, bestParent, weight;
	int maxFitness=0;
	
	//We're going to use tournament selection
	//We require "population" of children:
	for (child = 0; child < population; child ++)
	{
		//Shuffle array of parents:
		rtShuffle((*parentGen).inds, population);
		//Find fittest parent in tournament:
		maxFitness=0;
		for (parent = 0; parent < TOUR_SIZE; parent ++)
		{
			if((*parentGen).inds[parent].fitness > maxFitness)
			{
				maxFitness = (*parentGen).inds[parent].fitness;
				bestParent=parent;
			}
		}
		//Fittest parent is allowed a child!
		(*childGen).inds[child] = (*parentGen).inds[bestParent];
		readGenotype((*parentGen).inds[bestParent].geneFile, weights);
		sprintf((*childGen).inds[child].geneFile, "Genotypes/Gen%dInd%d.txt", gen+1, child);
		writeGenotype((*childGen).inds[child].geneFile, weights);
	}
	//We now have a population of children identical to their fit parents
}

void mutate(rtIndividual* individual)
{
	short int* weights;
	int theWeight, row, column;
	
	readGenotype((*individual).geneFile, weights);
	
	//Change weights
	for (row = 0; row < nNodes; row ++)
	{
		for (column = 0; column < nNodes; column ++)
		{
			theWeight=row*nNodes+column;
			if(row<nInputs)
			{
				if(column<nInputs)
				{
					if(rand()%pMUTATE<pII) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
					else weights[theWeight]=0;
				}
				else if(column<nInputs+nHiddens)
				{
					if(rand()%pMUTATE<pIH) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
					else weights[theWeight]=0;
				}
				else
				{
					if(rand()%pMUTATE<pIO) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
					else weights[theWeight]=0;
				}
			}
			else if(row<nInputs+nHiddens)
			{
				if(column<nInputs)
				{
					if(rand()%pMUTATE<pHI) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
					else weights[theWeight]=0;
				}
				else if(column<nInputs+nHiddens)
				{
					if(rand()%pMUTATE<pHH) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
					else weights[theWeight]=0;
				}
				else
				{
					if(rand()%pMUTATE<pHO) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
					else weights[theWeight]=0;
				}
			}
			else
			{
				if(column<nInputs)
				{
					if(rand()%pMUTATE<pOI) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
					else weights[theWeight]=0;
				}
				else if(column<nInputs+nHiddens)
				{
					if(rand()%pMUTATE<pOH) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
					else weights[theWeight]=0;
				}
				else
				{
					if(rand()%pMUTATE<pOO) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
					else weights[theWeight]=0;
				}
			}
		}
	}
	writeGenotype((*individual).geneFile, weights);
}

void rtShuffle(rtIndividual* array, int size)
{
	int i, random;
	rtIndividual temp;
	for (i = size-1; i > 0; i --)
	{
		random=rand() % i;
		temp=array[random];
		array[random]=array[i];
		array[i]=temp;
	}
}
