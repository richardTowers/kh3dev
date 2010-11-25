/*****************************************************************/
/*  evoSlave.c
/*	Created by richard at  9:49 on 15/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "evoSlave.h"

int main(int argc, char * argv[])
{
	int host, bytesRecieved;
	char buffer[MAXDATASIZE];
	pid_t processID = 0;
	int status;
	
	//Connect to host
	host = connectToHost(HOST_IP);

	for(;;)
	{
		//Control will wait here until recv() returns:		
		if ((bytesRecieved = recv(host, buffer, MAXDATASIZE-1, 0)) == -1)
		{
	  	  perror("recv");
	  	  exit(1);
		}
		else if (bytesRecieved == 0);
		
		else
		{
			//If there's a child process, kill it
			if(processID>0){ kill(processID, SIGKILL); wait(&status);}

			buffer[bytesRecieved] = '\0';
			printf("Recieved Signal: '%s'\n",buffer);
			
			//Fork process
			processID = fork();
			if(processID==CHILD) childProcess(buffer);
		}
	}
	return 0;
}

void childProcess(char *filename)
{
	printf("Running...\n");
	for(;;)
	{
		printf("%s\n", genotype);
		sleep(1);
	}
}
