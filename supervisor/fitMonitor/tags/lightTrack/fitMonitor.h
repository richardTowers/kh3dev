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


#define NUMBER_ROBOTS 2
#define XSCALE 100/(float)width
#define YSCALE 100/(float)height
#define TARGETS 3
#define HUE data[row*step+column*channels+0]
#define LIGHTNESS data[row*step+column*channels+1]
#define SATURATION data[row*step+column*channels+2]
#define RED (HUE > 90 && HUE < 150)
#define BLUE (HUE < 30 || HUE > 150)
#define GREEN (HUE > 30 && HUE < 90)
#define AREA 16
#define ANY_CAMERA -1

struct rtRobotPosition {
	int theRobot;
	CvPoint thePosition;
};

struct rtTarget {
	CvPoint position;
	int hue;
	int lightness;
	int saturation;
	int count;
};

IplImage *findLights(IplImage *frame);
IplImage * downsize4(IplImage * frame);

CvPoint redPosition;
CvPoint greenPosition;
CvPoint bluePosition;

#endif //INC_FITMONITOR
