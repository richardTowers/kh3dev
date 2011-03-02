/*****************************************************************/
//  neuralNetwork.h
//	Created by richard at 12:28 on 06/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/
#ifndef INC_NEURALNETWORK
#define INC_NEURALNETWORK

#include <time.h>


#include "errors.h"
#include "math.h"

#define INPUTS 9
#define OUTPUTS 2

#define sigmoid(x) (1.0/(1.0 + exp(-x)))

//WARNING!
//It is your responsibility to allocate and free the returned array of states!
void ctrnn(short *y, const short n, const short *I, const short *b, const short *t, const short *w, const short dT);

#endif //INC_NEURALNETWORK

