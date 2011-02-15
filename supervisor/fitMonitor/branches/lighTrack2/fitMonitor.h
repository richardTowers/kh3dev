/*****************************************************************/
/*  fitMonitor.h
/*	Created by richard at 10:51 on 30/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/
#ifndef INC_FITMONITOR_H
#define INC_FITMONITOR_H

#include <stdio.h>

#include "cv.h"
#include "highgui.h"

#define ANY_CAMERA -1


void findLights(IplImage *frame);

#endif //INC_FITMONITOR
