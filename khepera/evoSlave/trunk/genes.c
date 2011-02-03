/*****************************************************************/
//  genes.c
//	Created by richard at 21:20 on 17/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "genes.h"

void readGenotype(const char *filename)
{
	unsigned int preNeuron, postNeuron, nNeurons;
	int integerWeight;
	FILE *file;
	
	file = fopen(filename, "r");

	//Get number of nodes:
	fscanf(file,"%hd %hd %hd",&nInputs,&nHiddens,&nOutputs);
	//printf("Inputs: %d\nHiddens: %d\nOutputs: %d\n",nInputs,nHiddens,nOutputs);
	
	nNeurons=nInputs+nHiddens+nOutputs;
	//printf("nNeurons: %d\n",nNeurons);
	
	//Allocate enough memory for the weights, biases and time constants
	weights=malloc(sizeof(float)*nNeurons*nNeurons);
	
	//Loop through weights:
	for (postNeuron = 0; postNeuron < nNeurons; postNeuron ++)
	{
		for (preNeuron = 0; preNeuron < nNeurons; preNeuron ++)
		{
			fscanf(file,"%d",&integerWeight);
			weights[postNeuron*nNeurons+preNeuron]=0.1+integerWeight/1024.0;
		}
	}
		//Same for the others...
}

void printGenotype(void)
{
	int i,j;
	printf("Inputs: %d\nHiddens: %d\nOutputs: %d\nTotal: %d\n",nInputs,nHiddens,nOutputs,nInputs+nHiddens+nOutputs);
	for (i = 0; i < (nInputs+nHiddens+nOutputs); i += 1)
	{
		for (j = 0; j < (nInputs+nHiddens+nOutputs); j += 1)
		{
			if(weights[i*(nInputs+nHiddens+nOutputs)+j]==0) printf(" 0     ");
			else printf("%6.3f ",weights[i*(nInputs+nHiddens+nOutputs)+j]);
		}
		printf("\n");
	}
}
