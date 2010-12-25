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
    IplImage *frame = 0, *image = 0;
    int c;

    capture = cvCaptureFromCAM(ANY_CAMERA);

    if( !capture )
    {
        fprintf(stderr,"Could not initialize capturing...\n");
        return -1;
    }

    printf( "Hot keys: \n"
        "\tESC - quit the program\n");

    cvNamedWindow( "fitMonitor", 1 );

		
    for(;;)
    {
        frame = cvQueryFrame( capture );
        if( !frame ) break;

        if( !image )
        {
            /* allocate all the buffers */
            image = cvCreateImage( cvGetSize(frame), 8, 3 );
            image->origin = frame->origin;
        }

        cvCopy( findLights(frame), image, 0 );
        //cvCvtColor( image, hsv, CV_BGR2HSV );

        cvShowImage( "fitMonitor", image );

        c = cvWaitKey(5);
        if( (char) c == 27 ) exit(0);
    }

    cvReleaseCapture( &capture );
    cvDestroyWindow("fitMonitor");

    return 0;
}

IplImage *findLights(IplImage *frame)
{
	struct rtTarget redTarget = {{0,0},0,0,0,0}, greenTarget={{0,0},0,0,0,0}, blueTarget={{0,0},0,0,0,0};
	//IplImage *theImage = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
	
	//cvCopy(frame, theImage, 0);
	
	//Dilate to make the target bigger
	cvDilate(frame, frame, NULL, 2);
	
	//Reduce resolution to make our job easier
	frame = downsize4(frame);
	
	//Find the brightest pixels who's hue is red, green and blue
	{
		int row, column;
		
		//Convert the image to HLS:
		cvCvtColor(frame, frame, CV_BGR2HLS);

		//Get the image data
		int height     = frame->height;
		int width      = frame->width;
		int step       = frame->widthStep/sizeof(uchar);
		int channels   = frame->nChannels;
		uchar* data    = (uchar *)frame->imageData;
		
		//For each pixel
		for(row=0; row<height; row++) //For each row
		{
			for(column=0; column<width; column++)	//For each column
			{
				if(RED && LIGHTNESS > redTarget.lightness)
				{
					redTarget.position.x=column;
					redTarget.position.y=row;
					redTarget.hue=HUE;
					redTarget.lightness=LIGHTNESS;
					redTarget.saturation=SATURATION;
				}
				else if(BLUE && LIGHTNESS > blueTarget.lightness)
				{
					blueTarget.position.x=column;
					blueTarget.position.y=row;
					blueTarget.hue=HUE;
					blueTarget.lightness=LIGHTNESS;
					blueTarget.saturation=SATURATION;
				}
				else if(GREEN && LIGHTNESS > greenTarget.lightness)
				{
					greenTarget.position.x=column;
					greenTarget.position.y=row;
					greenTarget.hue=HUE;
					greenTarget.lightness=LIGHTNESS;
					greenTarget.saturation=SATURATION;
				}
			}
		}
		//Convert back to BGR:
		cvCvtColor(frame, frame, CV_HLS2BGR);
		
		//Put markers on the image
		cvCircle(frame, redTarget.position, 5, cvScalar(255,0,0,0),-1,8, 0);
		cvCircle(frame, blueTarget.position, 5, cvScalar(0,0,255,0),-1,8, 0);
		cvCircle(frame, greenTarget.position, 5, cvScalar(0,255,0,0),-1,8, 0);
		
		//Relay the coordinates of the targets
		redPosition.x = (int)redTarget.position.x*XSCALE; redPosition.y = (int)redTarget.position.y*YSCALE;
		greenPosition.x = (int)greenTarget.position.x*XSCALE; greenPosition.y = (int)greenTarget.position.y*YSCALE;
		bluePosition.x = (int)blueTarget.position.x*XSCALE; bluePosition.y = (int)blueTarget.position.y*YSCALE;
	}
	
	return frame;
}

IplImage * downsize4(IplImage * frame)
{
		int frameHeight, frameWidth;
		frameHeight=frame->height;
		frameWidth=frame->width;
    //IplImage *texImage = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
    IplImage *pyr = cvCreateImage(cvSize(frameWidth/2, frameHeight/2), IPL_DEPTH_8U, 3);
    IplImage *pyr2 = cvCreateImage(cvSize(frameWidth/4, frameHeight/4), IPL_DEPTH_8U, 3);
    
    cvPyrDown(frame, pyr, IPL_GAUSSIAN_5x5);
    cvPyrDown(pyr, pyr2, IPL_GAUSSIAN_5x5);
    
    cvResize(pyr2, frame, CV_INTER_NN);
    
    cvReleaseImage(&pyr);
    cvReleaseImage(&pyr2);
    
    return frame;
}
