/*****************************************************************/
//  neuralNetwork.h
//	Created by richard at 12:28 on 06/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/
#ifndef INC_NEURALNETWORK
#define INC_NEURALNETWORK

#include "errors.h"
#include "math.h"

#define INPUTS 9
#define HIDDENS 2
#define OUTPUTS 2

#define sigmoid(input) 2.0/(1.0 + exp(-3*input)) - 1;

typedef enum NNlayer
{
	input,
	hidden,
	output
} NNlayer;

extern unsigned short int nInputs, nHiddens, nOutputs;

//We need to keep track of this so it's declared as global and must be initiated and released using NNalloc() and NNdealloc()
float *neuronState;

void NNalloc(const short int nNeurons);
void NNdealloc(void);
//WARNING!
//It is your responsibility to free the returned array of outputs!
float *ffnn(float inputsIN[INPUTS], float weightsIH[INPUTS][HIDDENS], float weightsHO[HIDDENS][OUTPUTS]);
//WARNING!
//It is your responsibility to free the returned array of outputs!
float *dtrnn(const float *inputs, const float *weights);

#endif //INC_NEURALNETWORK
