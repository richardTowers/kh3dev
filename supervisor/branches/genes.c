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

void writeGenotype(const char *geneFile, const char *networkDiagram, short int* weights)
{
	short row, column;
	short theWeight;
	FILE *file_gene, *file_diagram;
	
	file_gene=fopen(geneFile, "w+");
	//Print number of nodes:
	fprintf(file_gene, "%d %d %d\n", nInputs, nHiddens, nOutputs);
	
	//Loop through weights:
	for (row = 0; row < (nInputs+nHiddens+nOutputs); row ++)
	{
		for (column = 0; column < (nInputs+nHiddens+nOutputs); column ++)
			fprintf(file_gene,"%6d ",weights[row*(nInputs+nHiddens+nOutputs)+column]);
		fprintf(file_gene,"\n");
	}
	fclose(file_gene);
	
	file_diagram=fopen(networkDiagram, "w+");
	
	fprintf(file_diagram, "digraph hierarchy {\n\nedge [color=\"#009900\", style=solid]\n\n");
	for(row=0; row < nInputs; row++) fprintf(file_diagram, "n%d [label=\"I %d\"]\n",
	row, row+1);
	
	for(row=nInputs; row < nInputs+nHiddens; row++) fprintf(file_diagram, "n%d [label=\"H %d\"]\n",
	row, row-nInputs+1);
	
	for(row=nInputs+nHiddens; row < nNodes; row++) fprintf(file_diagram, "n%d [label=\"O %d\"]\n",
	row, row-nInputs-nHiddens+1);
	
	fprintf(file_diagram, "\n{rank=same; ");
	for(row=0; row < nInputs; row++) fprintf(file_diagram, "n%d ", row);
	fprintf(file_diagram, "}\n{rank=same; ");
	for(row=nInputs; row < nInputs+nHiddens; row++) fprintf(file_diagram, "n%d ", row);
	fprintf(file_diagram, "}\n{rank=same; ");
	for(row=nInputs+nHiddens; row < nNodes; row++) fprintf(file_diagram, "n%d ", row);
	fprintf(file_diagram, "}\n\n");
	
	for (row = 0; row < nNodes; row ++)
	{
		for (column = 0; column < nNodes; column ++)
		{
			theWeight=row*nNodes+column;
			if(row<nInputs)
			{
				if(column<nInputs)
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/200.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/200.0);
				}
				else if(column<nInputs+nHiddens)
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/200.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/200.0);
				}
				else
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/200.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/200.0);
				}
			}
			else if(row<nInputs+nHiddens)
			{
				if(column<nInputs)
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/200.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/200.0);
				}
				else if(column<nInputs+nHiddens)
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/200.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/200.0);
				}
				else
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/200.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/200.0);
				}
			}
			else
			{
				if(column<nInputs)
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/200.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/200.0);
				}
				else if(column<nInputs+nHiddens)
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/200.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/200.0);
				}
				else
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/200.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/200.0);
				}
			}
		}
	}
	fprintf(file_diagram, "\n}\n");
	fclose(file_diagram);
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
		//Create files for this individual
		sprintf(generation[0].inds[theIndividual].geneFile, "%s/Gen0Ind%d.txt", genotypeFolder ,theIndividual);
		sprintf(generation[0].inds[theIndividual].logFile, "%s/Gen0Ind%d.svg", logFolder, theIndividual);
		sprintf(generation[0].inds[theIndividual].networkDiagram, "%s/Gen0Ind%d.dot", genotypeFolder, theIndividual);
		//Store genotype in file...
		writeGenotype(generation[0].inds[theIndividual].geneFile, generation[0].inds[theIndividual].networkDiagram, weights);
		generation[0].inds[theIndividual].fitness=0; generation[0].inds[theIndividual].parent=NULL;
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
		//Create files for this individual
		sprintf((*childGen).inds[child].geneFile, "%s/Gen%dInd%d.txt", genotypeFolder, gen+1, child);
		sprintf((*childGen).inds[child].logFile, "%s/Gen%dInd%d.svg", logFolder, gen+1, child);
		sprintf((*childGen).inds[child].networkDiagram, "%s/Gen%dInd%d.dot", genotypeFolder, gen+1, child);
		//Store genotype in file...
		writeGenotype((*childGen).inds[child].geneFile, (*childGen).inds[child].networkDiagram, weights);
		//Set data
		(*childGen).inds[child].number=child;
		(*childGen).inds[child].generation=gen+1;
		(*childGen).inds[child].fitness=0;
		(*childGen).inds[child].parent = &((*parentGen).inds[bestParent]);
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
	writeGenotype((*individual).geneFile,(*individual).networkDiagram,  weights);
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
