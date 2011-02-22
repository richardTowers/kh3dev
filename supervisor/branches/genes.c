/*****************************************************************/
//  genes.c
//	Created by richard at 21:20 on 17/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "genes.h"

short int* readGenotype(const char *filename, short int* weights)
{
	int theWeight;
	FILE *file;
	
	file = fopen(filename, "r");
	//Get number of nodes:
	fscanf(file,"%hd %hd %hd",&nInputs,&nHiddens,&nOutputs);
	
	//Allocate enough memory for the weights, biases and time constants
	weights=(short*)malloc(sizeof(short)*nNodes*nNodes);
		//biases=malloc(sizeof(int)*(nInputs+nHiddens+nOutputs));
		//timeConstants=malloc(sizeof(int)*(nInputs+nHiddens+nOutputs));
	
	//Loop through weights:
	for (theWeight = 0; theWeight < nNodes*nNodes; theWeight ++)
	{
		fscanf(file,"%hd",&weights[theWeight]);
	}
	
	fclose(file);
	return weights;
}

void writeGenotype(const char *filename, short int* weights)
{
	int theRow, theColumn;
	FILE *file;
	
	file=fopen(filename, "w+");
	//Print number of nodes:
	fprintf(file, "%d %d %d\n", nInputs, nHiddens, nOutputs);
	
	//Loop through weights:
	for (theRow = 0; theRow < (nInputs+nHiddens+nOutputs); theRow ++)
	{
		for (theColumn = 0; theColumn < (nInputs+nHiddens+nOutputs); theColumn ++)
			fprintf(file,"%6d ",weights[theRow*(nInputs+nHiddens+nOutputs)+theColumn]);
		fprintf(file,"\n");
	}
	fclose(file);
}

void createInitialGenes(int population, rtGeneration* generation)
{
	short int* weights;
	int theIndividual, theWeight, row, column;
	
	weights=(short*)malloc(sizeof(short)*nNodes*nNodes);
	
	for (theIndividual=0; theIndividual < population; theIndividual++)
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
		sprintf(generation[0].inds[theIndividual].geneFile, "%s/Gen0Ind%d.txt", genotypeFolder ,theIndividual);
		//Store genotype in file...
		writeGenotype(generation[0].inds[theIndividual].geneFile, weights);
		generation[0].inds[theIndividual].generation=0;
		generation[0].inds[theIndividual].number=theIndividual;
	}
	free(weights);
}

void reproduce(int population, int gen, rtGeneration* parentGen, rtGeneration* childGen)
{
	short int *weights=NULL;
	short int child, parent, bestParent;
	int maxFitness=0;
	
	weights=(short*)malloc(sizeof(short)*nNodes*nNodes);
	
	//We're going to use tournament selection
	//We require "population" of children:
	for (child = 0; child < population; child ++)
	{
		//Shuffle array of parents:
		rtShuffle((*parentGen).inds, population);
		//Find fittest parent in tournament:
		maxFitness=0;
		for (parent = 0; parent < TOUR_SIZE && parent < population; parent ++)
		{
			if((*parentGen).inds[parent].fitness > maxFitness)
			{
				maxFitness = (*parentGen).inds[parent].fitness;
				bestParent=parent;
			}
		}
		//Fittest parent is allowed a child!
		(*childGen).inds[child] = (*parentGen).inds[bestParent];
		
		weights=readGenotype((*parentGen).inds[bestParent].geneFile, weights);
		sprintf((*childGen).inds[child].geneFile, "%s/Gen%dInd%d.txt", genotypeFolder, gen+1, child);
		writeGenotype((*childGen).inds[child].geneFile, weights);
		(*childGen).inds[child].number=child;
		(*childGen).inds[child].generation=gen+1;
	}
	//We now have a population of children identical to their fit parents
	//Mutate kiddies
	for (child = 0; child < population; child ++) mutate(&(*childGen).inds[child]);
	//Done!
}

void mutate(rtIndividual* individual)
{
	short int* weights=NULL;	//This will be initialized properly in readGenotype()
	int theWeight, row, column;
	
	weights=readGenotype((*individual).geneFile, weights);
	
	//Change weights
	for (row = 0; row < nNodes; row ++)
	{
		for (column = 0; column < nNodes; column ++)
		{
			theWeight=row*nNodes+column;
			if(rand()%pMUTATE<pSET_ZERO) weights[theWeight]=0;
			if((rand()%pMUTATE<pADD_WEIGHT) && (weights[theWeight]==0)) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
			if(rand()%pMUTATE<pFLIP_NEG) weights[theWeight]=-1*weights[theWeight];
			if((rand()%pMUTATE<pCHANGE_WEIGHT) && (weights[theWeight]!=0)) weights[theWeight]=mutateWeight(weights[theWeight]);
		}
	}
	writeGenotype((*individual).geneFile, weights);
}

short int mutateWeight(short int weight)
{
	//Take care weight is in correct range since we're about to to play with bits...
	if(weight>1023) weight=1023;
	if(weight<-1024) weight=-1024;
	//Weights are between -1024 and 1023, ie. 0x80000400 and 0x000003ff
	//1000001111111111 and 0000001111111111
	short int theBit;
	for (theBit = 0; theBit < 10; theBit += 1)
	{
		if(rand()%pMUTATE<pFLIP_THIS_BIT) weight^=(1<<theBit);
	}
	return weight;
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
