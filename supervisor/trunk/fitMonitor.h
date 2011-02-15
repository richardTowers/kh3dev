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
#include <time.h>


#include "cv.h"
#include "highgui.h"
#include "types.h"
#include "limits.h"
#include "network.h"

//Defines:
#define CV_NO_BACKWARD_COMPATIBILITY
#define NUMBER_ROBOTS 1
#define MARKS_PER_BOT 1
#define XSCALE 100/(float)width
#define YSCALE 100/(float)height
#define TARGETS 3
#define HUE data[row*step+column*channels+0]
#define LIGHTNESS data[row*step+column*channels+1]
#define SATURATION data[row*step+column*channels+2]
#define ISRED (HUE > 90 && HUE < 150)
#define ISBLUE (HUE < 30 || HUE > 150)
#define ISGREEN (HUE > 30 && HUE < 90)
#define COLLISION_DISTANCE 50

#include "defines/colours.h"
#include "defines/genetics.h"

#define AREA 16
#define ANY_CAMERA -1
#define YES 1
#define NO 0
#define UNKNOWN 0
#define EXCL_RADIUS 2
#define ROI_SIZE 200
#define PI 3.14159
#define MARK_SIZE 60
#define LINE_WIDTH 2
#define NULL_POINT cvPoint(0xffff,0xffff)
#define WIDTH 635
#define HEIGHT 466
#define ESC 27
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))


//Functions:
void setupTracker(int nRobots, rtRobot* robots);
void testIndividualOnRobot(rtIndividual* individual, rtRobot robot);

int pointToLine(CvPoint point, CvPoint start, CvPoint end);
void keyHandler(void);
void mouseHandler(int event, int x, int y, int flags, void* param);


//Globals:
extern rtRobot* bots;

typedef enum semaphor {getRect, getLine, acceptOrReset, watchMouse, watchEscOnly} semaphor;

semaphor uiAction;
volatile bool mouseDown=NO;
volatile bool gotRect=NO;
volatile bool gotLine=NO;
volatile bool undoLine=NO;
volatile bool selectionMade=NO;
volatile bool windowNeedsReset=NO;

CvPoint rectStart={0,0};
CvPoint rectEnd={0,0};
CvRect fullRect={0,0,0,0};
int nBounds=0;
CvLine* bounds;

#endif //INC_FITMONITOR
