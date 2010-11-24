/*****************************************************************/
/*  evoMaster.h
/*	Created by richard at 16:14 on 18/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/
#ifndef INC_EVOMASTER_H
#define INC_EVOMASTER_H

#include <stdlib.h>
#include "network.h"
#include "genes.h"
#include "errors.h"


#define POP_SIZE 12
#define	GENERATIONS 10

#define INPUTS 9
#define	HIDDENS 9
#define OUTPUTS 2

extern int nInputs;
extern int nHiddens;
extern int nOutputs;
extern int* weights;

struct individual
{
	int number;
	int ID;
	char geneFile[8];
	int fitness;
};

struct generation
{
	int number;
	int averageFitness;
	int bestFitness;
	struct individual individual[POP_SIZE];
};
#endif //INC_EVOMASTER
