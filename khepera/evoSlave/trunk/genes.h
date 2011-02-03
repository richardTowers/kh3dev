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



//Genotype stored as global:
extern unsigned short int nInputs, nHiddens, nOutputs;
extern float *weights;

void readGenotype(const char *filename);
void printGenotype(void);

#endif //INC_GENES
