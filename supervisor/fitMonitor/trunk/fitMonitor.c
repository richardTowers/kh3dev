/*****************************************************************/
/*  fitMonitor.c
/*	Created by richard at 10:51 on 30/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include <highgui.h>

int main( int argc, char** argv )
{
	//Need to store all past positions, as well as current positions and the frame number
	int time;
	struct RSTposition currentPosition[NUMBER_ROBOTS];
	struct RSTposition *pastPosition;


/*EXAMPLE CODE 1
// cvLoadImage determines an image type and creates datastructure with appropriate size
    IplImage* img = cvLoadImage( argv[1],0);

// create a window. Window name is determined by a supplied argument
    cvNamedWindow( argv[1], CV_WINDOW_AUTOSIZE );
// Display an image inside and window. Window name is determined by a supplied argument
    cvShowImage( argv[1], img );
// wait indefinitely for keystroke
    cvWaitKey(0);

// release pointer to an object
    cvReleaseImage( &img );
// Destroy a window
    cvDestroyWindow( argv[1] );*/
}

cvPoint * getRobotPositions(int numberOfRobots)
{
	//Find lights, return malloc'd array of positions
}
