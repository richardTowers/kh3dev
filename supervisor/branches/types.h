/*****************************************************************/
//  types.h
//	Created by Richard Towers at 10:56 on 09/02/2011
//	Copyright Durham University 2011, all rights reserved
//
/*****************************************************************/
#ifndef INC_TYPES
#define INC_TYPES

#include "defines/genetics.h"
#include "cv.h"

typedef struct rtRobot		//This refers to a physical robot, not an individual
{
	char ip[15];
	int socket;
	IplImage *mark;
	CvScalar colour;
	CvPoint currPos;
	CvPoint prevPos;
} rtRobot;

typedef struct rtIndividual	//An individual to be evaluated and bred
{
	char geneFile[100];
	char logFile[100];
	char networkDiagram[100];
	int fitness;
	int generation;
	int number;
	rtIndividual* parent;
} rtIndividual;

typedef struct rtGeneration	//A generation contains several individuals
{
	rtIndividual inds[POP_SIZE];
	int maxFit;
	int avgFit;
} rtGeneration;

typedef struct CvLine
{
	CvPoint start;
	CvPoint end;
} CvLine;

#endif //INC_TYPES
