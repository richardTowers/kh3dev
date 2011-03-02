/*****************************************************************/
//  neuralNetwork.c
//	Created by richard at 12:28 on 06/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "neuralNetwork.h"

void NNalloc(const short int nNeurons)
{
	neuronState = calloc(nNeurons, sizeof(float));
}

void NNdealloc(void)
{
	if(neuronState!=NULL) free(neuronState);
	if(y!=NULL) free(y);
}

c_time ctrnn(float *y, const short n, const float *I, const float *b, const float *t, const float *w, const float dT)
{
	float *yOld;
	c_time clockCount;
	//i represents the postsynaptic neuron, j the presynaptic neuron
	short i, j;
	//k1-4 are the runge-kutta coefficients
	float k1, k2, k3, k4;
	//sum holds the value of the sum[w(ij)*sigmoid(y(i)-Theta(j)] term
	float sum=0;
	
	//Copy neuron states into new array:
	yOld=malloc(n*sizeof(float));
	for (i = 0; i < n; i += 1) yOld[n] = *(y+n);
	
	//Calculate neuron states at t+dT:
	for (i = 0; i < n; i += 1)
	{
		for (j = 0, sum = 0; j < n; j += 1) sum+=(*(w+i*nNeurons+j))*sigmoid(yOld[j]-b[j]);
		
		k1=(-(yOld[i]          )+sum+I[i])/t[i];
		k2=(-(yOld[i]+0.5*k1*dT)+sum+I[i])/t[i];
		k3=(-(yOld[i]+0.5*k2*dT)+sum+I[i])/t[i];
		k4=(-(yOld[i]  +  k3*dT)+sum+I[i])/t[i];
		y[i]+=(k1+2*k2+2*k3+k4)/6;
	}
	free(yOld);
	clockCount=clock();
	return clockCount;
}

/* 	inputs should be allocated: malloc(INPUTS*sizeof(float))
	weights should be allocated: malloc((INPUTS+HIDDENS+OUTPUTS)*sizeof(float))*/
//WARNING!
//It is your responsibility to free the returned array of outputs!
float *dtrnn(const float *inputs, const float *weights)
{
	float *neuronStatePrev;
	float *outputs;
	//Throughout this function I'm using 'pre' to indicate the PREsynaptic neuron, and 'post' to indicate the POSTsynaptic neuron.
	short int pre, post, neuron;
	NNlayer preLayer, postLayer;
	short int nNeurons = nInputs+nHiddens+nOutputs;
	
	//Allocate memory:
	neuronStatePrev = malloc(nNeurons*sizeof(float));
	outputs = malloc(nOutputs*sizeof(float));
	
	for (pre = 0; pre < nNeurons; pre ++)
	{
		//Store previous (t -1) state
		neuronStatePrev[pre]=neuronState[pre];
		//Determine the layer of the postsynaptic neuron
		if(pre<nInputs) preLayer=input;
		else if(pre<nInputs+nHiddens) preLayer=hidden;
		else preLayer=output;
		
		//If it's an input, then it'll have some input, otherwise we start counting from 0
		if(preLayer==input) neuronState[pre]=inputs[pre];
		else neuronState[pre]=0;
		
		for (post = 0; post < nNeurons; post ++)
		{
			//Determine the layer of the presynaptic neuron
			if(post<nInputs) postLayer=input;
			else if(post<nInputs+nHiddens) postLayer=hidden;
			else postLayer=output;
			
			/*If the postsynaptic neuron is in a higher layer than the presynaptic
				it can take the value from this time step, since all of the lower layers
				have been calculated. Otherwise it should take the value from t-1*/
			if(preLayer<=postLayer) neuronState[pre]+=weights[post*nNeurons+pre]*neuronStatePrev[post];
			else neuronState[pre]+=weights[post*nNeurons+pre]*neuronState[post];
		}
		neuronState[pre]=sigmoid(neuronState[pre]);
	}
	//At this point all of the neuron states for this timestep have been calculated, we're only interested in the outputs:
	for (neuron = nInputs+nHiddens; neuron < nNeurons; neuron ++)
	{
		outputs[neuron-(nInputs+nHiddens)] = neuronState[neuron];
	}
	//Free the memory:
	free(neuronStatePrev);
	#ifdef TESTING
		printf("Neuron States: ");
		for (neuron = 0; neuron < nNeurons; neuron += 1)
		{
			printf("%f ",neuronState[neuron]);
		}
		printf("\n");
		printf("Outputs: ");
		for (neuron = 0; neuron < nOutputs; neuron += 1)
		{
			printf("%f ",outputs[neuron]);
		}
		printf("\n");
	#endif
	//Return the outputs which MUST BE RELEASED by the caller
	return outputs;
}

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
