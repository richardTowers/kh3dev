/*****************************************************************/
/*  evoMaster.h
/*	Created by richard at 16:14 on 18/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/
#ifndef INC_EVOMASTER_H
#define INC_EVOMASTER_H

#define POP_SIZE
#define	GENERATIONS

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
};
#endif //INC_EVOMASTER
