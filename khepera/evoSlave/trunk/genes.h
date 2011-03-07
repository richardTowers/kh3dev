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

#define BIAS_OUT 500
#define BIAS_IN 0x3F
#define TCONST_OUT 2000
#define TCONST_IN 0x3F
#define WEIGHT_RANGE 3.0
#define WEIGHT_IN 0x3F


//Genotype stored as global:
extern unsigned short int nInputs, nHiddens, nOutputs, nNeurons;
extern short *weights, *biases, *tConsts;

void readGenotype(const char *filename);
void printGenotype(void);

#endif //INC_GENES
