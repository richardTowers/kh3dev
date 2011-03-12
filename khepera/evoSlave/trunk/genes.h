/*****************************************************************/
//  genes.h
//	Created by richard at 21:20 on 17/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/
#ifndef INC_GENES
#define INC_GENES

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define BIAS_IN 0x3F
#define TCONST_IN 0x3F
#define WEIGHT_IN 0x3F

#define BIAS_MAX 1.0
#define TCONST_MAX 1.0
#define WEIGHT_MAX 1.0

//Genotype stored as global:
extern unsigned short int nInputs, nHiddens, nOutputs, nNeurons;
extern float *weights, *biases, *tConsts;

void readGenotype(const char *filename);
void printGenotype(void);

#endif //INC_GENES
