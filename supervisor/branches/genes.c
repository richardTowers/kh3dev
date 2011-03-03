/*****************************************************************/
//  genes.c
//	Created by richard at 21:20 on 17/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "genes.h"

void readGenotype(const char *filename, short int* weights, short* biases, short* tConsts)
{
	short theWeight, theBias, theTConst;
	FILE *file;
	
	file = fopen(filename, "r");
	//Get number of nodes:
	fscanf(file,"%hd %hd %hd",&nInputs,&nHiddens,&nOutputs);
	
	//Allocate enough memory for the weights, biases and time constants
	tConsts=(short*)malloc(sizeof(short)*nNodes);
	biases=(short*)malloc(sizeof(short)*nNodes);
	weights=(short*)malloc(sizeof(short)*nNodes*nNodes);
	
	//Loop through biases:
	for (theBias = 0; theBias < nNodes; theBias ++) fscanf(file,"%hd",&biases[theBias]);
	
	//Loop through time constants:
	for (theTConst = 0; theTConst < nNodes; theTConst ++) fscanf(file,"%hd",&tConsts[theTConst]);
	
	//Loop through weights:
	for (theWeight = 0; theWeight < nNodes*nNodes; theWeight ++)
	{
		fscanf(file,"%hd",&weights[theWeight]);
	}
	
	fclose(file);
	return;
}

void writeGenotype(const char *geneFile, const char *networkDiagram, short* weights, short* biases, short*tConsts)
{
	short row, column, theBias, theTConst;
	short theWeight;
	FILE *file_gene, *file_diagram;
	
	file_gene=fopen(geneFile, "w+");
	//Print number of nodes:
	fprintf(file_gene, "%d %d %d\n", nInputs, nHiddens, nOutputs);
	
	//Loop through biases:
	for (theBias = 0; theBias < nNodes; theBias ++) fprintf(file_gene,"%hd ",biases[theBias]);
	fprintf(file_gene, "\n");
	
	//Loop through time constants:
	for (theTConst = 0; theTConst < nNodes; theTConst ++) fprintf(file_gene,"%hd ",tConsts[theTConst]);
	fprintf(file_gene, "\n");
	
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
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/64000.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/64000.0);
				}
				else if(column<nInputs+nHiddens)
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/64000.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/64000.0);
				}
				else
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/64000.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/64000.0);
				}
			}
			else if(row<nInputs+nHiddens)
			{
				if(column<nInputs)
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/64000.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/64000.0);
				}
				else if(column<nInputs+nHiddens)
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/64000.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/64000.0);
				}
				else
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/64000.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/64000.0);
				}
			}
			else
			{
				if(column<nInputs)
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/64000.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/64000.0);
				}
				else if(column<nInputs+nHiddens)
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/64000.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/64000.0);
				}
				else
				{
					if(weights[theWeight]<0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\", color=Red];\n",row, column, -1*weights[theWeight]/64000.0);
					else if(weights[theWeight]>0) fprintf(file_diagram, "n%d->n%d [style=\"setlinewidth(%f)\"];\n", row, column, weights[theWeight]/64000.0);
				}
			}
		}
	}
	fprintf(file_diagram, "\n}\n");
	fclose(file_diagram);
}

void createInitialGenes(int population, rtGeneration* generation)
{
	short int* weights, *biases, *tConsts;
	int theIndividual, theWeight, row, column;
	
	weights=(short*)malloc(sizeof(short)*nNodes*nNodes);
	biases=(short*)malloc(sizeof(short)*nNodes);
	tConsts=(short*)malloc(sizeof(short)*nNodes);
	
	for (theIndividual=0; theIndividual < population; theIndividual++)
	{
		//Assign Biases
		for (row = 0; row < nNodes; row += 1)
		{
			biases[row]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
		}
		//Assign Time Constants
		for (row = 0; row < nNodes; row += 1)
		{
			tConsts[row]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
		}
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
		writeGenotype(generation[0].inds[theIndividual].geneFile, generation[0].inds[theIndividual].networkDiagram, weights, biases, tConsts);
		generation[0].inds[theIndividual].fitness=0; generation[0].inds[theIndividual].parent=NULL;
		generation[0].inds[theIndividual].generation=0;
		generation[0].inds[theIndividual].number=theIndividual;
	}
	free(weights);
	free(biases);
	free(tConsts);
}

void reproduce(int population, int gen, rtGeneration* parentGen, rtGeneration* childGen)
{
	short int *weights=NULL, *biases=NULL, *tConsts=NULL;
	short int child, parent, bestParent;
	int maxFitness=-100;
	
	biases=(short*)malloc(sizeof(short)*nNodes);
	tConsts=(short*)malloc(sizeof(short)*nNodes);
	weights=(short*)malloc(sizeof(short)*nNodes*nNodes);
	
	//We're going to use tournament selection
	//We require "population" of children:
	for (child = 0; child < population; child ++)
	{
		//Shuffle array of parents:
		rtShuffle((*parentGen).inds, population);
		//Find fittest parent in tournament:
		maxFitness=-100;
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
		
		readGenotype((*parentGen).inds[bestParent].geneFile, weights, biases, tConsts);
		//Create files for this individual
		sprintf((*childGen).inds[child].geneFile, "%s/Gen%dInd%d.txt", genotypeFolder, gen+1, child);
		sprintf((*childGen).inds[child].logFile, "%s/Gen%dInd%d.svg", logFolder, gen+1, child);
		sprintf((*childGen).inds[child].networkDiagram, "%s/Gen%dInd%d.dot", genotypeFolder, gen+1, child);
		//Store genotype in file...
		writeGenotype((*childGen).inds[child].geneFile, (*childGen).inds[child].networkDiagram, weights, biases, tConsts);
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
	free(weights); free(biases); free(tConsts);
}

void mutate(rtIndividual* individual)
{
	short int *weights=NULL, *biases=NULL, *tConsts=NULL;
	int theWeight, row, column;
	
	biases=(short*)malloc(sizeof(short)*nNodes);
	tConsts=(short*)malloc(sizeof(short)*nNodes);
	weights=(short*)malloc(sizeof(short)*nNodes*nNodes);
	
	readGenotype((*individual).geneFile, weights, biases, tConsts);
	//Change biases
	for (row = 0; row < nNodes; row += 1)
	{
		if(rand()%pMUTATE<pSET_ZERO) biases[row]=0;
		if((rand()%pMUTATE<pADD_WEIGHT) && (biases[row]==0)) biases[row]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
		if(rand()%pMUTATE<pFLIP_NEG) biases[row]=-biases[row];
		if((rand()%pMUTATE<pCHANGE_WEIGHT) && (biases[row]!=0)) biases[row]=mutateWeight(biases[row]);
	}
	//Change time constants
	for (row = 0; row < nNodes; row += 1)
	{
		if(rand()%pMUTATE<pSET_ZERO) tConsts[row]=0;
		if((rand()%pMUTATE<pADD_WEIGHT) && (tConsts[row]==0)) tConsts[row]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
		if(rand()%pMUTATE<pFLIP_NEG) tConsts[row]=-tConsts[row];
		if((rand()%pMUTATE<pCHANGE_WEIGHT) && (tConsts[row]!=0)) tConsts[row]=mutateWeight(tConsts[row]);
	}
	//Change weights
	for (row = 0; row < nNodes; row ++)
	{
		for (column = 0; column < nNodes; column ++)
		{
			theWeight=row*nNodes+column;
			if(rand()%pMUTATE<pSET_ZERO) weights[theWeight]=0;
			if((rand()%pMUTATE<pADD_WEIGHT) && (weights[theWeight]==0)) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
			if(rand()%pMUTATE<pFLIP_NEG) weights[theWeight]=-weights[theWeight];
			if((rand()%pMUTATE<pCHANGE_WEIGHT) && (weights[theWeight]!=0)) weights[theWeight]=mutateWeight(weights[theWeight]);
		}
	}
	writeGenotype((*individual).geneFile,(*individual).networkDiagram,  weights, biases, tConsts);
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
