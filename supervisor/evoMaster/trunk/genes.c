/*****************************************************************/
/*  genes.c
/*	Created by richard at 21:20 on 17/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "genes.h"

void readGenotype(const char *filename)
{
	int theWeight;
	FILE *file;
	
	file = fopen(filename, "r");
	//Get number of nodes:
	fscanf(file,"%hd %hd %hd",&nInputs,&nHiddens,&nOutputs);
	
	//Allocate enough memory for the weights, biases and time constants
	weights=malloc(sizeof(int)*(nInputs+nHiddens+nOutputs)*(nInputs+nHiddens+nOutputs));
		//biases=malloc(sizeof(int)*(nInputs+nHiddens+nOutputs));
		//timeConstants=malloc(sizeof(int)*(nInputs+nHiddens+nOutputs));
	
	//Loop through weights:
	for (theWeight = 0; theWeight < (nInputs+nHiddens+nOutputs)*(nInputs+nHiddens+nOutputs); theWeight ++)
	{
		fscanf(file,"%hd",&weights[theWeight]);
	}
		//Same for the others...
	
	fclose(file);
	
}

void writeGenotype(const char *filename)
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

void printGenotype(void)
{
	int i;
	printf("Inputs: %d\nHiddens: %d\nOutputs: %d\n",nInputs,nHiddens,nOutputs);
	for (i = 0; i < (nInputs+nHiddens+nOutputs)*(nInputs+nHiddens+nOutputs); i += 1)
	{
		printf("%d ",weights[i]);
	}
}
