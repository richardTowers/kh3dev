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
#include "genes.h"

#define INPUTS 9
#define OUTPUTS 2

float sigmoidTable[1025];

void initSigmoid(void);
float sigmoid(float x);

//WARNING!
//It is your responsibility to allocate and free the returned array of states!
void ctrnn(float *y, const short n, const float *I, const float *b, const float *t, const float *w, const float dT);
void ffnn(float *y, const short n, const float *I, const float *w);

#endif //INC_NEURALNETWORK

