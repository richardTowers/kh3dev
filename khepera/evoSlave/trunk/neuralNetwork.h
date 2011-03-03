/*****************************************************************/
//  neuralNetwork.h
//	Created by richard at 12:28 on 06/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/
#ifndef INC_NEURALNETWORK
#define INC_NEURALNETWORK

#include <time.h>
#include <limits.h>

#include "errors.h"
#include "math.h"

#define INPUTS 9
#define OUTPUTS 2

/****************************************************************/
//This is just for the retreat function and needs redoing...
#define rSigmoid(input) 2.0/(1.0 + exp(-3*input)) - 1;

typedef enum NNlayer
{
	input,
	hidden,
	output
} NNlayer;
/****************************************************************/

short sigmoidTable[1025];

void initSigmoid(void);
short sigmoid(short x);

//WARNING!
//It is your responsibility to allocate and free the returned array of states!
void ctrnn(short *y, const short n, const short *I, const short *b, const short *t, const short *w, const short dT);
float *dtrnn(const float *inputs, const float *weights);

#endif //INC_NEURALNETWORK

