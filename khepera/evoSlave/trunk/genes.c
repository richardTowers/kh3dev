/*****************************************************************/
//  genes.c
//	Created by richard at 21:20 on 17/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "genes.h"

void readGenotype(const char *filename)
{
	unsigned short int preNeuron, postNeuron, theBias, theTConst;
	FILE *file;
	
	file = fopen(filename, "r");

	//Get number of nodes:
	fscanf(file,"%hd %hd %hd",&nInputs,&nHiddens,&nOutputs);
	//printf("Inputs: %d\nHiddens: %d\nOutputs: %d\n",nInputs,nHiddens,nOutputs);
	
	nNeurons=nInputs+nHiddens+nOutputs;
	//printf("nNeurons: %d\n",nNeurons);
	
	//Allocate enough memory for the weights, biases and time constants
	tConsts=malloc(sizeof(short)*nNeurons);
	biases=malloc(sizeof(short)*nNeurons);
	weights=malloc(sizeof(short)*nNeurons*nNeurons);
	
	//Loop through biases:
	for (theBias = 0; theBias < nNeurons; theBias ++) fscanf(file,"%hd",&biases[theBias]);
	
	//Loop through time constants:
	for (theTConst = 0; theTConst < nNeurons; theTConst ++) fscanf(file,"%hd",&tConsts[theTConst]);

	//Loop through weights:
	for (postNeuron = 0; postNeuron < nNeurons; postNeuron ++)
	{
		for (preNeuron = 0; preNeuron < nNeurons; preNeuron ++)
		{
			fscanf(file,"%hd",&weights[postNeuron*nNeurons+preNeuron]);
		}
	}
	//All of the characteristics are stored as globals on the heap and are now set.
	return;
}

void printGenotype(void)
{
	unsigned short i, j, theBias, theTConst;
	printf("Inputs: %d\nHiddens: %d\nOutputs: %d\nTotal: %d\n",nInputs,nHiddens,nOutputs,nInputs+nHiddens+nOutputs);
	
	//Loop through biases:
	for (theBias = 0; theBias < nNeurons; theBias ++) printf("%hd ", biases[theBias]);
	printf("\n");
	
	//Loop through time constants:
	for (theTConst = 0; theTConst < nNeurons; theTConst ++) printf("%hd ",tConsts[theTConst]);
	printf("\n");
		
	for (i = 0; i < (nInputs+nHiddens+nOutputs); i += 1)
	{
		for (j = 0; j < (nInputs+nHiddens+nOutputs); j += 1)
		{
			if(weights[i*(nInputs+nHiddens+nOutputs)+j]==0) printf(" 0     ");
			else printf("%hd ",weights[i*(nInputs+nHiddens+nOutputs)+j]);
		}
		printf("\n");
	}
}
