/*****************************************************************/
/*  network.c
/*	Created by richard at  9:53 on 15/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "network.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int connectToHost(char *IPAddress)
{
	int sockfd, returnValue;
	char s[INET6_ADDRSTRLEN];
	struct addrinfo hints, *servinfo, *p;
	
	if(strlen(IPAddress)<7) error(ERROR_IPDDRESS);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((returnValue = getaddrinfo(IPAddress, PORT, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(returnValue));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
	printf("client: connecting to %s\n", s);
	
	freeaddrinfo(servinfo); // all done with this structure
	//At this point we should be connected to the server and ready to recieve...	
	return sockfd;
}
