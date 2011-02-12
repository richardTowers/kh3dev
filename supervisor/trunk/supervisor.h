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
#include "types.h"


//Physical:
#define nROBOTS 1			//This represents the physical number of robots in the environment
//Genetics:
#include "defines/genetics.h"
//Neurology:
#include "defines/neurology.h"
//Colours:
#include "defines/colours.h"


void createInitialGenes(int population, rtGeneration* generation);
void reproduce(int population, int gen, rtGeneration* parentGen, rtGeneration* childGen);
void rtShuffle(rtIndividual* array, int size);
void mutate();
extern void setupTracker(int nRobots, rtRobot* robots);
extern void testIndividualOnRobot(rtIndividual* individual, rtRobot robot);

extern CvCapture* capture;
//Assign architecure
unsigned short int nInputs=INPUTS;
unsigned short int nHiddens=HIDDENS;
unsigned short int nOutputs=OUTPUTS;
unsigned short int nNodes=INPUTS+HIDDENS+OUTPUTS;

#endif //INC_SUPERVISOR
