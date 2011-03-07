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

#include "cv.h"
#include "types.h"

//Genetics:
#include "defines/genetics.h"
//Neurology:
#include "defines/neurology.h"
//Colours:
#include "defines/colours.h"


//Genotype stored as global:
extern unsigned short int nInputs, nHiddens, nOutputs, nNodes;

short int* readGenotype(const char *filename, short int* weights);
void writeGenotype(const char *geneFile, const char *networkDiagram, short* weights, short* biases, short*tConsts);
void createInitialGenes(int population, rtGeneration* generation);
void reproduce(int population, int gen, rtGeneration* parentGen, rtGeneration* childGen);
void rtShuffle(rtIndividual* array, int size);
void mutate(rtIndividual* individual);
short int mutateWeight(const short int weight);
short int mutateBias(short int bias);
short int mutateTConst(short int tConst);

extern char genotypeFolder[60];
extern char logFolder[60];

#endif //INC_GENES
