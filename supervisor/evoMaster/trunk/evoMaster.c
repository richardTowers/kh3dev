/*****************************************************************/
/*  evoMaster.c
/*	Created by richard at 16:14 on 18/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "evoMaster.h"

int main(int argc, char * argv[])
{
	struct individual individual[POP_SIZE*GENERATIONS];
	struct generation generation[GENERATIONS];
	
	int theIndividual=0, theGeneration=0;
	
	//Connect to Robot
	
	//Create initial population of random genotype files
	for (theIndividual=0; theIndividual < POP_SIZE; theIndividual++)
	{
			//Random Weights etc.
			
			//Store in file...
	}
	
	for (theGeneration = 0; theGeneration < generations; theGeneration ++)
	{
		for (theIndividual = 0; theIndividual < populationSize; theIndividual ++)
		{
			//Send genotype to robot
			
			//Send 'File Sent!' to robot
			
			//Monitor robots fitness
				//This will take a long time to return
				//Store robots fitness
				
			//Send 'Stop Motors' to robot
			
		}
	}
	
}
