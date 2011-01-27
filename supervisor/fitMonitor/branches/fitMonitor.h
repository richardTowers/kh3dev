/*****************************************************************/
/*  fitMonitor.h
/*	Created by richard at 10:51 on 30/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/
#ifndef INC_FITMONITOR_H
#define INC_FITMONITOR_H

//Includes:
#include <stdio.h>
#include <math.h>

#include "cv.h"
#include "highgui.h"

//Defines:
#define CV_NO_BACKWARD_COMPATIBILITY
#define NUMBER_ROBOTS 2
#define XSCALE 100/(float)width
#define YSCALE 100/(float)height
#define TARGETS 3
#define HUE data[row*step+column*channels+0]
#define LIGHTNESS data[row*step+column*channels+1]
#define SATURATION data[row*step+column*channels+2]
#define ISRED (HUE > 90 && HUE < 150)
#define ISBLUE (HUE < 30 || HUE > 150)
#define ISGREEN (HUE > 30 && HUE < 90)
#define BLUE cvScalar(255,0,0)
#define RED cvScalar(0,0,255)
#define GREEN cvScalar(0,255,0)
#define GREY cvScalar(50,50,50)
#define AREA 16
#define ANY_CAMERA -1
#define YES 1
#define NO 0
#define UNKNOWN 0
#define EXCL_RADIUS 2
#define ROI_SIZE 200
#define PI 3.14159
#define MARK_SIZE 20
#define LINE_WIDTH 2
#define NULL_POINT cvPoint(0xffff,0xffff)
#define WIDTH 635
#define HEIGHT 466

//Types:
typedef struct rtFlags {
	char front;
	char rear;
} rtFlags;

typedef struct rtRobotPosition {
	short unsigned int robot;
	struct rtFlags lights;
	char positionIsKnown;
	CvPoint position;
	double heading;
} rtRobotPosition;

typedef struct rtLight {
	CvPoint position;
	int hue;
	int lightness;
	int saturation;
	int count;
} rtLight;

//Functions:
rtRobotPosition findRobot(IplImage *frame, rtRobotPosition lastPos);
void findInitialPosition(CvCapture *capture, int robot);
void printPositions(void);

CvRect getROIFromPoint(CvPoint point, IplImage* bounds);

CvPoint findLightInROI(IplImage *frame, CvPoint exclude, char colour);

IplImage* downsize4(IplImage * frame);

void drawRobot(IplImage *frame, int robot);

//Globals:
rtRobotPosition robots[NUMBER_ROBOTS];

#endif //INC_FITMONITOR
