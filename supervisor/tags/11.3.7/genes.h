/*****************************************************************/
//  genes.h
//	Created by richard at 21:20 on 17/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/
#ifndef INC_GENES
#define INC_GENES

#include <stdio.h>
#include <stdlib.h>


//Genotype stored as global:
extern unsigned short int nInputs, nHiddens, nOutputs, nNodes;
extern short int *weights;

short int* readGenotype(const char *filename, short int* weights);
void writeGenotype(const char *filename, short int* weights);
//void printGenotype(void);

#endif //INC_GENES
