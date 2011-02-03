/*****************************************************************/
/*  evoMaster.h
/*	Created by richard at 16:14 on 18/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/
#ifndef INC_EVOMASTER_H
#define INC_EVOMASTER_H

#include <stdlib.h>
#include <dirent.h>

#include "network.h"
#include "genes.h"
#include "errors.h"

#define POP_SIZE 12
#define	GENERATIONS 10

#define ROBOT_IP "192.168.1.2"
#define GENE_FOLDER "/home/root/Genotypes"
#define WEIGHT_RANGE 1024

#define INPUTS 9
#define	HIDDENS 2
#define OUTPUTS 2

/*I  H  O
I 0 14  1
H 0  4 14
O 0  0  0*/
#define pBASE 20
#define pII 0
#define pIH 14
#define pIO 1
#define pHI 0
#define pHH 2
#define pHO 14
#define pOI 0
#define pOH 0
#define pOO 0

unsigned short int nInputs, nHiddens, nOutputs;
short int* weights;

struct individual
{
	int number;
	int ID;
	char geneFile[20];
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
