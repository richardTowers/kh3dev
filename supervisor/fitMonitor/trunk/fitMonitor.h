/*****************************************************************/
//  fitMonitor.h
//	Created by richard at 10:51 on 30/11/2010
//	Copyright Durham University 2010, all rights reserved
//
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
#define GREY cvScalar(150,150,150)
#define YELLOW cvScalar(0,255,255)
#define AREA 16
#define ANY_CAMERA -1
#define YES 1
#define NO 0
#define UNKNOWN 0
#define EXCL_RADIUS 2
#define ROI_SIZE 200
#define PI 3.14159
#define MARK_SIZE 30
#define LINE_WIDTH 2
#define NULL_POINT cvPoint(0xffff,0xffff)
#define WIDTH 635
#define HEIGHT 466
#define ESC 27
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define GOTRECT 0
#define SELECTIONORWINDOW 1
#define MOUSEDOWN 2
#define ESC_ONLY 3


//Types:
typedef struct rtFlags {
	IplImage* front;
	IplImage* rear;
} rtFlags;

typedef struct rtRobotPosition {
	rtFlags marks;
	char positionIsKnown;
	CvPoint position;
	double heading;
} rtRobotPosition;

//Functions:
void mainCapture(void);
void keyHandler(void);
void subCapture(IplImage* fullFrame, CvRect bounds, int robot);
void mouseHandler(int event, int x, int y, int flags, void* param);
IplImage * downsize4(IplImage * frame);

//Globals:
rtRobotPosition robots[NUMBER_ROBOTS];
unsigned short int theSemaphor;
volatile bool mouseDown=NO;
volatile bool gotRect=NO;
volatile bool selectionMade=NO;
volatile bool windowNeedsReset=NO;

CvPoint rectStart={0,0};
CvPoint rectEnd={0,0};
CvRect fullRect={0,0,0,0};

#endif //INC_FITMONITOR
