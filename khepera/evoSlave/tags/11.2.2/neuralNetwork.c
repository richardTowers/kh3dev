/*****************************************************************/
/*  neuralNetwork.c
/*	Created by richard at 12:28 on 06/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "neuralNetwork.h"


/* Inputs and Weights should be from -1 to 1, uses a sigmoid function which maps ±∞ to ±1
 * Outputs in the range -1 to 1, these should be scaled appropriately.*/
//WARNING!
//It is your responsibility to free the returned array of outputs!
float *ffnn(float inputsIN[INPUTS], float weightsIH[INPUTS][HIDDENS], float weightsHO[HIDDENS][OUTPUTS])
{
	float inputsOUT[INPUTS];
	float hiddensIN[HIDDENS];
	float hiddensOUT[HIDDENS];
	float outputsIN[OUTPUTS];
	float *outputsOUT = malloc(OUTPUTS*sizeof(float));
	
	if (outputsOUT == NULL) error(ERROR_MALLOC);
	
	int theInput, theHidden, theOutput;
	float inputSum=0;
	
	//Get outputs of input neurons
	//printf("\nOutputs of Input Neurons:\n");
	for (theInput=0; theInput<INPUTS; theInput++)
	{
		inputsOUT[theInput]=sigmoid(inputsIN[theInput]);
		//printf("%d: %f, ", theInput, inputsOUT[theInput]);
	}
	
	//Get the summed inputs to the hidden neurons
	//printf("\nInputs to Hidden Neurons:\n");
	for (theHidden=0; theHidden<HIDDENS; theHidden++) {
		//Sum the inputs to this neuron
		for (theInput=0; theInput<INPUTS; theInput++) inputSum += inputsOUT[theInput]*weightsIH[theInput][theHidden];
		hiddensIN[theHidden]=inputSum;
		inputSum=0; //Clear up our mess
		//printf("%d: %f, ", theHidden, hiddensIN[theHidden]);
	}
	//Get outputs of hidden neurons
	//printf("\nOutputs from Hidden Neurons:\n");
	for (theHidden=0; theHidden<HIDDENS; theHidden++)
	{
		hiddensOUT[theHidden]=sigmoid(hiddensIN[theHidden]);
		//printf("%d: %f, ", theHidden, hiddensOUT[theHidden]);
	}
	
	//Get the summed inputs to the output neurons
	for (theOutput=0; theOutput<OUTPUTS; theOutput++) {
		//Sum the inputs to this neuron
		for (theHidden=0; theHidden<HIDDENS; theHidden++) inputSum += hiddensOUT[theHidden]*weightsHO[theHidden][theOutput];
		outputsIN[theOutput]=inputSum;
		inputSum=0;	//Clear up our mess
	}
	//Get outputs of output neurons
	for (theOutput=0; theOutput<OUTPUTS; theOutput++) {
		outputsOUT[theOutput]=sigmoid(outputsIN[theOutput]);
		//Set whatever output to whatever output
	}
	return outputsOUT;
}
