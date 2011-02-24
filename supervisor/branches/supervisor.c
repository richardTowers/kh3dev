/*****************************************************************/
//  supervisor.c
//	Created by Richard Towers at 16:40 on 08/02/2011
//	Copyright Durham University 2011, all rights reserved
//
/*****************************************************************/

#include "supervisor.h"


int main(int argc, char *argv[])
{
	rtRobot bots[nROBOTS]={{"192.168.1.2",NULL,NULL,RED,{0,0},{0,0}}};
	rtGeneration gens[GENERATIONS];
	
	char buffer[100], folder[40], filename[30];
	
	short int ind=0, gen=0, bot, nInds=0, nGens=0;
	
	time_t progStartTime; char timeStr[30];
	
	unsigned short int i;
	
	if(commandLine(argc, argv)!=NULL)
	{
		strcpy(buffer,commandLine(argc, argv));
		
		sprintf(genotypeFolder, "%sGenotypes/", buffer);
		sprintf(logFolder, "%sLogFiles/", buffer);
		
		//Get number of individuals by counting through Gen0
		do{sprintf(buffer, "%sGen0Ind%d.txt", genotypeFolder, ind); ind++;} while(access(buffer,F_OK)==0); ind-=2;
		nInds=ind;
		//nInds is now equal to the number of individuals
		
		//Get most recent full generation by counting GenXIndNinds
		do{sprintf(buffer, "%sGen%dInd%d.txt", genotypeFolder, gen, nInds);gen++;} while(access(buffer,F_OK)==0); gen-=2;
		nGens=gen;
		//nGens is now equal to the number of generations evaluated so far
		
		//Make sure Log File exists too:
		if(access(logFolder,F_OK)!=0)
		{
			printf("No LogFiles folder in experiment directory, creating a new folder...\n");
			sprintf(buffer, "mkdir %s", logFolder);
			system(buffer);
		}
		
		//Now need to set data of individuals in the first generation
		for (ind = 0; ind < nInds; ind += 1)
		{
			sprintf(gens[nGens].inds[ind].geneFile, "%s/Gen%dInd%d.txt", genotypeFolder, nGens, ind);
			sprintf(gens[nGens].inds[ind].logFile, "%s/Gen%dInd%d.svg", logFolder, nGens, ind);
			sprintf(gens[nGens].inds[ind].networkDiagram, "%s/Gen%dInd%d.dot", genotypeFolder, nGens, ind);
			gens[nGens].inds[ind].fitness=0; gens[nGens].inds[ind].parent=NULL;
			gens[nGens].inds[ind].generation=nGens; gens[nGens].inds[ind].number=ind;
		}
	}
	else	//We need to create a new folder...
	{
		nInds=POP_SIZE; nGens=0;
		//Create new experiment directory:
		progStartTime=time(NULL);
		strcpy(timeStr,asctime(localtime(&progStartTime)));
		//Replace Illegal characters...
		for (i = 0; i < strlen(timeStr); i += 1){if(timeStr[i]==' ') timeStr[i]='-';else if(timeStr[i]=='\n') timeStr[i]='\0';}
		
		sprintf(folder, "Experiments/%s", timeStr);
		sprintf(buffer, "mkdir %s", folder);
		
		if(access("./Experiments",F_OK)!=0) system("mkdir Experiments");
		if(access(folder,F_OK)!=0) system(buffer);
		else
		{
			fprintf(stderr, "Already an experiment directory with this time stamp!?!? Exiting...\n");
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
	
	//Connect to robots
	for (bot = 0; bot < nROBOTS; bot ++) bots[bot].socket=connectToClient(bots[bot].ip);
	
	//Set up fitness tracker
	setupTracker(nROBOTS, bots);
	
	//For each generation starting at 0
	for (gen = nGens; gen < GENERATIONS; gen ++)
	{
		//For each individual in the population
		for (ind = 0; ind < nInds; ind ++)
		{
			//Send the file to the individual
			//strcpy(filename,gens[gen].inds[ind].geneFile);
			sprintf(filename, "Genotypes/Gen%dInd%d.txt", gen, ind);
			sprintf(buffer, "scp \'%s\' root@%s:Genotypes",gens[gen].inds[ind].geneFile, bots[0].ip);
			system(buffer);
			
			//Tell the individual to get going
			send(bots[0].socket, filename, strlen(filename)+1, 0);
			//Monitor and then store its fitness
			testIndividualOnRobot(&(gens[gen].inds[ind]), bots[0]);
			//Stop the motors:
			send(bots[0].socket, "Stop Motors", 11, 0);
		}
		//When entire population have fitness values
		reproduce(nInds+1, gen, &gens[gen], &gens[gen+1]);
		//Next Generation
	}
	
	//When enough generations have elapsed
		//Print useful data
		//END
}

char* commandLine(int argc, char *argv[])
{
	char buffer[100];
	
	if(argc>2) { fprintf(stderr, "Too many arguments for program %s\n", argv[0]); return NULL; }
	
	else if(argc==2)	//Folder of pre-existing genotypes specified...
	{
		if(access(argv[1],F_OK)!=0)	{ fprintf(stderr, "%s: No such experiment folder\n", argv[1]); exit(1); }
		
		else	//argv[1] is a valid folder, does it contain genes?
		{
			sprintf(buffer, "%sGenotypes/Gen0Ind0.txt", argv[1]);
			if(access(buffer,F_OK)!=0)	//Not a genotype folder
			{
				fprintf(stderr, "%s: Specified file does not contain a non-empty Genotypes folder\n"
				"You may have omitted the trailing / after your folder.\n", argv[1]);
				exit(1);
			}
			else return argv[1];
		}
	}
	
	else return NULL;
}
