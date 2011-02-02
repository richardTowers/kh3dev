/*****************************************************************/
/*  genes.c
/*	Created by richard at 21:20 on 17/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "genes.h"

void readGenotype(const char *filename)
{
	int i,j=0,startNode,endNode,theWeight;
	char theChar, lineBuffer[100], intBuffer[10];
	FILE *file;
	
	file = fopen(filename, "r");
	//Get number of nodes:
	fscanf(file,"%d %d %d",&nInputs,&nHiddens,&nOutputs);
	
	//Allocate enough memory for the weights, biases and time constants
	weights=malloc(sizeof(int)*(nInputs+nHiddens+nOutputs)*(nInputs+nHiddens+nOutputs));
		//biases=malloc(sizeof(int)*(nInputs+nHiddens+nOutputs));
		//timeConstants=malloc(sizeof(int)*(nInputs+nHiddens+nOutputs));
	
	//Loop through weights:
	for (i = 0; i < (nInputs+nHiddens+nOutputs)*(nInputs+nHiddens+nOutputs); i ++)
	{
		fscanf(file,"%d",&weights[i]);
	}
		//Same for the others...
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
