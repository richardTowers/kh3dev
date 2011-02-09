/*****************************************************************/
//  supervisor.h
//	Created by Richard Towers at 16:40 on 08/02/2011
//	Copyright Durham University 2011, all rights reserved
//
/*****************************************************************/
#ifndef INC_SUPERVISOR
#define INC_SUPERVISOR

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "cv.h"
#include "highgui.h"
#include "network.h"
#include "genes.h"

//Physical:
#define nROBOTS 1			//This represents the physical number of robots in the environment
//Genetics:
#define POP_SIZE 10			//The number of individuals to be evaluated before reproduction
#define GENERATIONS 10		//The number of generations to evaluate
#define EVAL_TIME 10		//The max time to spend evaluating an individual
//Neurology:
#include "defines/neurology.h"

//Colours:
#include "defines/colours.h"



typedef struct rtRobot		//This refers to a physical robot, not an individual
{
	char ip[15];
	int socket;
	IplImage *mark;
	CvScalar colour;
} rtRobot;

typedef struct rtIndividual	//An individual to be evaluated and bred
{
	char geneFile[20];
	int fitness;
} rtIndividual;

typedef struct rtGeneration	//A generation contains several individuals
{
	rtIndividual inds[POP_SIZE];
	int maxFit;
	int avgFit;
} rtGeneration;

void createInitialGenes(int population, rtGeneration* generation);
extern void setupTracker(void);

//Assign architecure
unsigned short int nInputs=INPUTS;
unsigned short int nHiddens=HIDDENS;
unsigned short int nOutputs=OUTPUTS;
unsigned short int nNodes=INPUTS+HIDDENS+OUTPUTS;

#endif //INC_SUPERVISOR
