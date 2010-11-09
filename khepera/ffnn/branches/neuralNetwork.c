/*****************************************************************/
/*  neuralNetwork.c
/*	Created by richard at 12:28 on 06/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "neuralNetwork.h"


/* Inputs and Weights should be from -1 to 1, uses a sigmoid function which maps ±∞ to ±1
 * Outputs in the range -1 to 1, these should be scaled appropriately.*/
float *ffnn(float inputsIN[INPUTS], float weightsIH[INPUTS][HIDDENS], float weightsHO[HIDDENS][OUTPUTS])
{
	float inputsOUT[INPUTS];
	float hiddensIN[HIDDENS];
	float hiddensOUT[HIDDENS];
	float outputsIN[OUTPUTS];
	float *outputsOUT = malloc(OUTPUTS*sizeof(float));
	
	if (outputsOUT == NULL) error(ERROR_MALLOC);
	
	int theInput, theHidden, theOutput, inputSum;
	
	//Get outputs of input neurons
	for (theInput=0; theInput<INPUTS; theInput++) inputsOUT[theInput]=sigmoid(inputsIN[theInput]);
	
	//Get the summed inputs to the hidden neurons
	for (theHidden=0; theHidden<HIDDENS; theHidden++) {
		//Sum the inputs to this neuron
		for (theInput=0; theInput<INPUTS; theInput++) inputSum += inputsOUT[theInput]*weightsIH[theInput][theHidden];
		hiddensIN[theHidden]=inputSum;
		inputSum=0; //Clear up our mess
	}
	//Get outputs of hidden neurons
	for (theHidden=0; theHidden<HIDDENS; theHidden++) hiddensOUT[theHidden]=sigmoid(hiddensIN[theHidden]);
	
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
