/*****************************************************************/
/*  evoSlave.h
/*	Created by richard at  9:49 on 15/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/
#ifndef INC_EVOSLAVE_H
#define INC_EVOSLAVE_H

#include "network.h"
#include "errors.h"
#include "robot.h"
#include "neuralNetwork.h"

#define MAXDATASIZE 100
#define HOST_IP "192.168.1.1"
#define CHILD 0

void childProcess(char *genotype);

#endif //INC_EVOSLAVE
