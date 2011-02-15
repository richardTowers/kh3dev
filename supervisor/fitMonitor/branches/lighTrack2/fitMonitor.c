/*****************************************************************/
/*  fitMonitor.c
/*	Created by richard at 10:51 on 30/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "fitMonitor.h"

#define CV_NO_BACKWARD_COMPATIBILITY


int main( void )
{
    CvCapture* capture = 0;
    IplImage *frame = 0;
    int keyPress;

    capture = cvCaptureFromCAM(ANY_CAMERA);

    if( !capture )
    {
        fprintf(stderr,"Could not initialize capturing...\n");
        return -1;
    }

    cvNamedWindow( "fitMonitor", 1 );

		
    for(;;)
    {
        frame = cvQueryFrame( capture );
        if( !frame ) break;
		findLights(frame);
		
        cvShowImage( "fitMonitor", frame );

        keyPress = cvWaitKey(5);
        if( (char) keyPress == 27 ) exit(0);
    }

    cvReleaseCapture( &capture );
    cvDestroyWindow("fitMonitor");

    return 0;
}

void findLights(IplImage *frame)
{
	CvPoint maxloc;
		
	//Convert the image to HLS:
	cvCvtColor(frame, frame, CV_BGR2HLS);
	
	cvSetImageCOI(frame, 3);
	
	cvMinMaxLoc(frame, NULL, NULL, NULL, &maxloc, 0);
	
	cvSetImageCOI(frame, 0);
	//Convert back to BGR:
	cvCvtColor(frame, frame, CV_HLS2BGR);
	
	//Put markers on the image
	cvCircle(frame, maxloc, 5, cvScalar(255,0,0,0),-1,8, 0);
}

