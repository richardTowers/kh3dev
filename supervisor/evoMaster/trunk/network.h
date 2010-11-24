/*****************************************************************/
/*  network.h
/*	Created by richard at  9:53 on 15/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/
#ifndef INC_NETWORK
#define INC_NETWORK
///////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "5555"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold

void *get_in_addr(struct sockaddr *sa);
void sigchld_handler(int s);
int connectToClient(char* IPAddress);

///////////////////////////////////////////////////////////////////
#endif //INC_NETWORK
