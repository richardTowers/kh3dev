/*****************************************************************/
//  supervisor.c
//	Created by Richard Towers at 16:40 on 08/02/2011
//	Copyright Durham University 2011, all rights reserved
//
/*****************************************************************/

#include "supervisor.h"


int main(int argc, char *argv[])
{
	char buffer[100], folder[50];
	rtRobot bots[nROBOTS]={{"192.168.1.2",NULL,NULL,RED,{0,0},{0,0}}};
	rtGeneration gens[GENERATIONS];
	short int ind=0, gen=0, bot, nInds=0, nGens=0; char command[300], filename[200];
	time_t progStartTime; struct tm *localTime; char timeStr[30];
	unsigned short int i; 
	
	if(argc>2)
	{
		printf("Too many arguments for program %s\n", argv[0]);
		exit(1);
	}
	else if(argc==2)	//Folder of pre-existing genotypes specified...
	{
		if(access(argv[1],F_OK)!=0)	//No such folder
		{
			printf("%s: No such experiment folder\n", argv[1]);
			exit(1);
		}
		else	//argv[1] is a valid folder, does it contain genes?
		{
			sprintf(genotypeFolder, "%s/Genotypes", argv[1]);
			sprintf(buffer, "%s/Gen0Ind0.txt", genotypeFolder);
			if(access(buffer,F_OK)!=0)	//Not a genotype folder
			{
				printf("%s: Specified file does not contain a non-empty Genotypes folder\n", argv[1]);
				exit(1);
			}
			else	//argv[1] is a genotype folder!
			{
				//Get number of individuals by counting through Gen0
				do
				{
					sprintf(buffer, "%s/Gen0Ind%d.txt", genotypeFolder, ind);
					ind++;
				} while (access(buffer,F_OK)==0); ind--;
				nInds=ind;
				//nInds is now equal to the number of individuals
				printf("Number of individuals = %d\n",nInds);
				//Get most recent full generation by counting GenXIndNinds
				do
				{
					sprintf(buffer, "%s/Gen%dInd%d.txt", genotypeFolder, gen, nInds-1);
					gen++;
				} while (access(buffer,F_OK)==0); gen--;
				nGens=gen;
				//nGens is now equal to the number of generations evaluated so far
				printf("Number of Generations = %d\n",nGens);
				
				//Make sure Log File exists too:
				sprintf(logFolder, "%s/LogFiles", argv[1]);
				if(access(logFolder,F_OK)!=0)
				{
					printf("No LogFiles folder in experiment directory, creating a new folder...\n");
					sprintf(buffer, "mkdir %s", logFolder);
					system(buffer);
				}
			}
		}
	}
	else	//We need to create a new folder...
	{
		nInds=POP_SIZE; nGens=0;
		//Create new experiment directory:
		progStartTime=time(NULL);
		localTime=localtime(&progStartTime);
		strcpy(timeStr,asctime(localTime));
		for (i = 0; i < strlen(timeStr); i += 1)
		{
			if(timeStr[i]==' ') timeStr[i]='-';
			else if(timeStr[i]=='\n') timeStr[i]='\0';
		}
		sprintf(folder, "Experiments/%s", timeStr);
		sprintf(buffer, "mkdir %s", folder);
		
		if(access("./Experiments",F_OK)!=0) system("mkdir Experiments");
		if(access(folder,F_OK)!=0) system(buffer);
		else
		{
			printf("Already an experiment directory with this time stamp!! Exiting...\n");
			exit(1);
		}
		sprintf(logFolder, "%s/LogFiles", folder);
		sprintf(genotypeFolder, "%s/Genotypes", folder);
		sprintf(buffer, "mkdir %s", logFolder);
		system(buffer);
		sprintf(buffer, "mkdir %s", genotypeFolder);
		system(buffer);
		
		//Create initial population of genotype files
		createInitialGenes(nInds, &gens[0]);
	}
	
	#ifndef TESTING
		//Connect to robots
		for (bot = 0; bot < nROBOTS; bot ++) bots[bot].socket=connectToClient(bots[bot].ip);
	#endif
	
	//Set up fitness tracker
	setupTracker(nROBOTS, bots);
	
	#ifndef TESTING
	//For each generation starting at 0
	for (gen = nGens; gen < GENERATIONS; gen ++)
	{
		#endif
		//For each individual in the population
		for (ind = 0; ind < nInds; ind ++)
		{
			#ifndef TESTING
			//Send the file to the individual
			//strcpy(filename,gens[gen].inds[ind].geneFile);
			sprintf(filename, "Genotypes/Gen%dInd%d.txt", gen, ind);
			printf("Filename: %s\n", gens[gen].inds[ind].geneFile);
			sprintf(command, "scp \'%s\' root@%s:Genotypes",gens[gen].inds[ind].geneFile, bots[0].ip);
			printf("Command: %s\n",command);
			system(command);
			//Tell the individual to get going
			send(bots[0].socket, filename, strlen(filename)+1, 0);
			//Monitor and then store its fitness
			testIndividualOnRobot(&(gens[gen].inds[ind]), bots[0]);
			//Stop the motors:
			send(bots[0].socket, "Stop Motors", 11, 0);
			
			#endif
		}
		//When entire population have fitness values
		reproduce(nInds, gen, &gens[gen], &gens[gen+1]);
		
		//Next Generation
		#ifndef TESTING
	}
	#endif
	//When enough generations have elapsed
		//Print useful data
		//END
}

void createInitialGenes(int population, rtGeneration* generation)
{
	short int* weights;
	int theIndividual, theWeight, row, column;
	
	weights=(short*)malloc(sizeof(short)*nNodes*nNodes);
	
	for (theIndividual=0; theIndividual < population; theIndividual++)
	{
		//Assign Weights
		for (row = 0; row < nNodes; row ++)
		{
			for (column = 0; column < nNodes; column ++)
			{
				theWeight=row*nNodes+column;
				if(row<nInputs)
				{
					if(column<nInputs)
					{
						if(rand()%pBASE<pII) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else if(column<nInputs+nHiddens)
					{
						if(rand()%pBASE<pIH) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else
					{
						if(rand()%pBASE<pIO) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
				}
				else if(row<nInputs+nHiddens)
				{
					if(column<nInputs)
					{
						if(rand()%pBASE<pHI) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else if(column<nInputs+nHiddens)
					{
						if(rand()%pBASE<pHH) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else
					{
						if(rand()%pBASE<pHO) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
				}
				else
				{
					if(column<nInputs)
					{
						if(rand()%pBASE<pOI) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else if(column<nInputs+nHiddens)
					{
						if(rand()%pBASE<pOH) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
					else
					{
						if(rand()%pBASE<pOO) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
						else weights[theWeight]=0;
					}
				}
			}
		}
		//Create a file for this individual
		sprintf(generation[0].inds[theIndividual].geneFile, "%s/Gen0Ind%d.txt", genotypeFolder ,theIndividual);
		printf("%s\n",generation[0].inds[theIndividual].geneFile);
		//Store genotype in file...
		writeGenotype(generation[0].inds[theIndividual].geneFile, weights);
		generation[0].inds[theIndividual].generation=0;
		generation[0].inds[theIndividual].number=theIndividual;
	}
	free(weights);
}

void reproduce(int population, int gen, rtGeneration* parentGen, rtGeneration* childGen)
{
	short int *weights=NULL;
	short int child, parent, bestParent;
	int maxFitness=0;
	
	weights=(short*)malloc(sizeof(short)*nNodes*nNodes);
	
	//We're going to use tournament selection
	//We require "population" of children:
	for (child = 0; child < population; child ++)
	{
		//Shuffle array of parents:
		rtShuffle((*parentGen).inds, population);
		//Find fittest parent in tournament:
		maxFitness=0;
		for (parent = 0; parent < TOUR_SIZE && parent < population; parent ++)
		{
			if((*parentGen).inds[parent].fitness > maxFitness)
			{
				maxFitness = (*parentGen).inds[parent].fitness;
				bestParent=parent;
			}
		}
		//Fittest parent is allowed a child!
		(*childGen).inds[child] = (*parentGen).inds[bestParent];
		
		weights=readGenotype((*parentGen).inds[bestParent].geneFile, weights);
		sprintf((*childGen).inds[child].geneFile, "Genotypes/Gen%dInd%d.txt", gen+1, child);
		writeGenotype((*childGen).inds[child].geneFile, weights);
		(*childGen).inds[child].number=child;
		(*childGen).inds[child].generation=gen;
	}
	//We now have a population of children identical to their fit parents
	//Mutate kiddies
	for (child = 0; child < population; child ++) mutate(&(*childGen).inds[child]);
	//Done!
}

void mutate(rtIndividual* individual)
{
	short int* weights=NULL;	//This will be initialized properly in readGenotype()
	int theWeight, row, column;
	
	weights=readGenotype((*individual).geneFile, weights);
	
	//Change weights
	for (row = 0; row < nNodes; row ++)
	{
		for (column = 0; column < nNodes; column ++)
		{
			theWeight=row*nNodes+column;
			if(rand()%pMUTATE<pSET_ZERO) weights[theWeight]=0;
			if((rand()%pMUTATE<pADD_WEIGHT) && (weights[theWeight]==0)) weights[theWeight]=rand()%(2*WEIGHT_RANGE)-WEIGHT_RANGE;
			if(rand()%pMUTATE<pFLIP_NEG) weights[theWeight]=-1*weights[theWeight];
			if((rand()%pMUTATE<pCHANGE_WEIGHT) && (weights[theWeight]!=0)) weights[theWeight]=mutateWeight(weights[theWeight]);
		}
	}
	writeGenotype((*individual).geneFile, weights);
}

short int mutateWeight(short int weight)
{
	//Take care weight is in correct range since we're about to to play with bits...
	if(weight>1023) weight=1023;
	if(weight<-1024) weight=-1024;
	//Weights are between -1024 and 1023, ie. 0x80000400 and 0x000003ff
	//1000001111111111 and 0000001111111111
	short int theBit;
	for (theBit = 0; theBit < 10; theBit += 1)
	{
		if(rand()%pMUTATE<pFLIP_THIS_BIT) weight^=(1<<theBit);
	}
	return weight;
}

void rtShuffle(rtIndividual* array, int size)
{
	int i, random;
	rtIndividual temp;
	for (i = size-1; i > 0; i --)
	{
		random=rand() % i;
		temp=array[random];
		array[random]=array[i];
		array[i]=temp;
	}
}
