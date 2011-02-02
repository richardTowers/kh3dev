/*****************************************************************/
/*  evoSlave.h
/*	Created by richard at  9:49 on 15/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/
#ifndef INC_EVOSLAVE_H
#define INC_EVOSLAVE_H

//#define TESTING

#include <string.h>

#include "network.h"
#include "errors.h"

#ifndef TESTING
#include "robot.h"
#endif //TESTING

#include "neuralNetwork.h"

#define MAXDATASIZE 100
#ifndef TESTING
#define HOST_IP "192.168.1.1"
#else
#define HOST_IP "127.0.0.1"
#endif
#define CHILD 0

//Genotype stored as global:
int nInputs, nHiddens, nOutputs;
int *weights; //Add biases and time constants later...

void childProcess(char *genotype);

#endif //INC_EVOSLAVE
