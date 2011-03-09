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

#include "network.h"
#include "genes.h"
#include "types.h"


//Physical:
#define nROBOTS 1			//This represents the physical number of robots in the environment
//Genetics:
#include "defines/genetics.h"


char* commandLine(int argc, char *argv[]);
extern void createInitialGenes(int population, rtGeneration* generation);
extern void reproduce(int population, int gen, rtGeneration* parentGen, rtGeneration* childGen);
extern void rtShuffle(rtIndividual* array, int size);
extern void mutate(rtIndividual* individual);
extern short int mutateWeight(const short int weight);
extern void setupTracker(int nRobots, rtRobot* robots);
extern void testIndividualOnRobot(rtIndividual* individual, rtRobot robot);


//Assign architecure
unsigned short int nInputs=INPUTS;
unsigned short int nHiddens=HIDDENS;
unsigned short int nOutputs=OUTPUTS;
unsigned short int nNodes=INPUTS+HIDDENS+OUTPUTS;

//File Paths
char genotypeFolder[60];
char logFolder[60];

#endif //INC_SUPERVISOR
