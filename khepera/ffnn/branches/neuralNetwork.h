/*****************************************************************/
/*  neuralNetwork.h
/*	Created by richard at 12:28 on 06/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/
#ifndef INC_NEURALNETWORK
#define INC_NEURALNETWORK

#include "errors.h"
#include "math.h"

#define INPUTS 9
#define HIDDENS 2
#define OUTPUTS 2

#define sigmoid(input) 2.0/(1.0 + exp(-input)) - 1;

float *ffnn(float inputsIN[INPUTS], float weightsIH[INPUTS][HIDDENS], float weightsHO[HIDDENS][OUTPUTS]);

#endif //INC_NEURALNETWORK
