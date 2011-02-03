/*****************************************************************/
//  network.h
//	Created by richard at  9:53 on 15/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/
#ifndef INC_NETWORK
#define INC_NETWORK
///////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#include "errors.h"

#define PORT "5555" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once

void *get_in_addr(struct sockaddr *sa);

int connectToHost(char *IPAddress);

///////////////////////////////////////////////////////////////////
#endif //INC_NETWORK
