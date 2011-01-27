/*****************************************************************/
//  fitMonitor.c
//	Created by richard at 10:51 on 30/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "fitMonitor.h"


int main( void )
{
	//Camera Variables
	CvCapture* capture = 0;
	IplImage *frame = 0;
	//UI Variables
	int c;
	//Helpers
	int i; char buffer;
	
	//Initialise the robot robots
	for (i = 0; i < NUMBER_ROBOTS; i += 1)
	{
		robots[i].robot=i;
		askFront:
			printf("\nRobot %d has a red (r), blue (blue) or green (g) FRONT light:", i);
			scanf("%s", &buffer);
			if(buffer=='r'||buffer=='b'||buffer=='g'){robots[i].lights.front=buffer; printf("%c",buffer);}
			else { printf("\nYou must enter either r, b or g"); goto askFront; }
		askRear:
			printf("\nRobot %d has a red (r), blue (blue) or green (g) REAR light:", i);
			scanf("%s", &buffer);
			if(buffer=='r'||buffer=='b'||buffer=='g') {robots[i].lights.rear=buffer; printf("%c",buffer);}
			else { printf("\nYou must enter either r, b or g"); goto askRear; }
		robots[i].positionIsKnown=NO;
		robots[i].position.x=UNKNOWN;
		robots[i].position.y=UNKNOWN;
		robots[i].heading=UNKNOWN;
	}
	printf("\nGot initial values...\n");
	
	//Set up camera
	capture = cvCaptureFromCAM(ANY_CAMERA);
	if( !capture )
	{
		fprintf(stderr,"Could not initialize capturing...\n");
		return -1;
	}
	
	//Find initial robots
	for (i = 0; i < NUMBER_ROBOTS; i += 1)
	{
		if(robots[i].positionIsKnown==NO)
		{
			printf("Attempting to find robot %d (%c,%c)... Press SPACE when found.\n",
			i,robots[i].lights.front,robots[i].lights.rear);
			findInitialPosition(capture, i);
		}
	}
		
	//Open window for output
	cvNamedWindow( "fitMonitor", 0 );
  	
  	for(;;)
    {
        frame = cvQueryFrame( capture );
        if( !frame ) break;
		cvShowImage( "fitMonitor", frame );
		

        c = cvWaitKey(5);
        if( (char) c == 27 ) exit(0);
    
  	
		//Find initial robots
		for (i = 0; i < NUMBER_ROBOTS; i += 1)
		{
			if(robots[i].positionIsKnown==NO)
			{
				printf("Attempting to find robot %d (%c,%c)... Press SPACE when found.\n",
				i,robots[i].lights.front,robots[i].lights.rear);
				findInitialPosition(capture, i);
			}
		}
		printPositions();
  	}
	//Accept user input
  
	//Search ROIs for robots
	
}

void findInitialPosition(CvCapture *capture, int robot)
{
	CvPoint frontLightPos, backLightPos;
	double x,y,heading;
	char keyPress;
	CvRect ROI, frameBounds={2,2,WIDTH,HEIGHT};
	IplImage *frame;
	IplImage *subImage;
	
	
    
	//Open window
	cvNamedWindow("FindRobot",0);
	frame = cvQueryFrame(capture);
	for(;;)
	{
		frame = cvQueryFrame( capture );
        if( !frame ) break;
		cvSetImageROI(frame, frameBounds);
		cvDilate(frame, frame, NULL, 4);
		
		frontLightPos=cvPoint(0,0);
		frontLightPos=findLightInROI(frame, NULL_POINT, robots[robot].lights.front);
		
		ROI = getROIFromPoint(frontLightPos, frame);
		printf("{%d,%d,%d,%d} vs {%d,%d,%d,%d}\n",ROI.x,ROI.y,ROI.width,ROI.height, 0, 0, frame->width, frame->height);
		
		cvSetImageROI(frame, ROI);
		subImage = cvCreateImage( cvGetSize(frame), 8, 3 );
    	subImage->origin = frame->origin;
		cvCopy(frame, subImage, 0);
		
		
		backLightPos=findLightInROI(subImage, frontLightPos, robots[robot].lights.rear);
		cvSetImageROI(frame, frameBounds);
		cvReleaseImage(&subImage);
		cvRectangle(frame, cvPoint(ROI.x,ROI.y), cvPoint(ROI.x+ROI.width,ROI.y+ROI.height), RED, 1, CV_AA, 0);
		cvLine(frame, frontLightPos, backLightPos, BLUE, 1, CV_AA, 0);
		
		robots[robot].position.x=(frontLightPos.x+backLightPos.x)/2;
		robots[robot].position.y=(frontLightPos.y+backLightPos.y)/2;
		x=(double)(frontLightPos.x-backLightPos.x);
		y=(double)(frontLightPos.y-backLightPos.y);
		robots[robot].heading=360*atan2(y,x)/(2*PI);
		heading=robots[robot].heading;
		
		drawRobot(frame, robot);
		cvShowImage("FindRobot", frame );
		keyPress = cvWaitKey(5);
        switch(keyPress)
        {
        	case ' ':
        		goto endOfLoop;
        	case 27:
        		exit(0);
        }
	} endOfLoop:
	
	robots[robot].positionIsKnown=YES;
	cvDestroyWindow("FindRobot");
}


void printPositions(void)
{
	int i;
	for (i = 0; i < NUMBER_ROBOTS; i += 1)
	{
		printf("Robot %d (%c,%c):\n\tLocated at (%d,%d), pointing %f\n",
		robots[i].robot, robots[i].lights.front, robots[i].lights.rear, robots[i].position.x,
		robots[i].position.y, robots[i].heading);
	}
}


CvPoint findLightInROI(IplImage *frame, CvPoint exclude, char colour)
{
	int row, column, maxLightness=0;
	CvPoint point;
	bool pointIsFarEnough;
	
	point=cvPoint(0,0);
	
	//Convert the image to HLS:
	cvCvtColor(frame, frame, CV_BGR2HLS);

	//Get the image data
	int height     = frame->height;
	int width      = frame->width;
	int step       = frame->widthStep/sizeof(uchar);
	int channels   = frame->nChannels;
	uchar* data    = (uchar *)frame->imageData;
	
	switch(colour)
	{
		case 'r':
			for(row=0; row<height; row++) //For each row
			{
				for(column=0; column<width; column++)	//For each column
				{
					pointIsFarEnough=( EXCL_RADIUS < hypot((column-exclude.x),(row-exclude.y)));

					if( ISRED && (LIGHTNESS > maxLightness) && pointIsFarEnough)
					{
						point=cvPoint(row, column);
						maxLightness=LIGHTNESS;
					}
				}
			}
			break;
		case 'b':
			for(row=0; row<height; row++) //For each row
			{
				for(column=0; column<width; column++)	//For each column
				{
					pointIsFarEnough=( EXCL_RADIUS < hypot((column-exclude.x),(row-exclude.y)));
					if( ISBLUE && (LIGHTNESS > maxLightness) && pointIsFarEnough)
					{
						point=cvPoint(row, column);
						maxLightness=LIGHTNESS;
					}
				}
			}
			break;
		case 'g':
			for(row=0; row<height; row++) //For each row
			{
				for(column=0; column<width; column++)	//For each column
				{
					pointIsFarEnough=( EXCL_RADIUS < hypot((column-exclude.x),(row-exclude.y)));
					if( ISGREEN && (LIGHTNESS > maxLightness) && pointIsFarEnough)
					{
						point=cvPoint(row, column);
						maxLightness=LIGHTNESS;
					}
				}
			}
			break;
		default:
			printf("\nWARNING: findLight given non-RGB colour.\n");
			return cvPoint(UNKNOWN,UNKNOWN);
			break;
	}
	//Convert back...
	cvCvtColor(frame, frame, CV_HLS2BGR);
	
	return point;
}

void drawRobot(IplImage *frame, int robot)
{
	CvPoint frontLine, rearLine;

	cvCircle(frame, robots[robot].position, MARK_SIZE, GREY, LINE_WIDTH, CV_AA);
	//Draw lines to represent lights
	frontLine.x=robots[robot].position.x+(int)(MARK_SIZE*sin(robots[robot].heading)+0.5);
	frontLine.y=robots[robot].position.y+(int)(MARK_SIZE*cos(robots[robot].heading)+0.5);
	rearLine.x=robots[robot].position.x-(int)(MARK_SIZE*sin(robots[robot].heading)+0.5);
	rearLine.y=robots[robot].position.y-(int)(MARK_SIZE*cos(robots[robot].heading)+0.5);
	switch(robots[robot].lights.front)
	{
		case 'r':
			cvLine(frame, robots[robot].position, frontLine, RED, LINE_WIDTH);
			break;
		case 'b':
			cvLine(frame, robots[robot].position, frontLine, BLUE, LINE_WIDTH);
			break;
		case 'g':
			cvLine(frame, robots[robot].position, frontLine, GREEN, LINE_WIDTH);
			break;
	}
	switch(robots[robot].lights.rear)
	{
		case 'r':
			cvLine(frame, robots[robot].position, rearLine, RED, LINE_WIDTH/2);
			break;
		case 'b':
			cvLine(frame, robots[robot].position, rearLine, BLUE, LINE_WIDTH/2);
			break;
		case 'g':
			cvLine(frame, robots[robot].position, rearLine, GREEN, LINE_WIDTH/2);
			break;
	}
}

IplImage * downsize4(IplImage * frame)
{
	int frameHeight, frameWidth;
	frameHeight=frame->height;
	frameWidth=frame->width;

	IplImage *pyr = cvCreateImage(cvSize(frameWidth/2, frameHeight/2), IPL_DEPTH_8U, 3);
	IplImage *pyr2 = cvCreateImage(cvSize(frameWidth/4, frameHeight/4), IPL_DEPTH_8U, 3);

	cvPyrDown(frame, pyr, IPL_GAUSSIAN_5x5);
	cvPyrDown(pyr, pyr2, IPL_GAUSSIAN_5x5);

	cvResize(pyr2, frame, CV_INTER_NN);

	cvReleaseImage(&pyr);
	cvReleaseImage(&pyr2);

	return frame;
}

CvRect getROIFromPoint(CvPoint point, IplImage* bounds)
{
	CvRect rect;
	rect = cvRect(point.x-ROI_SIZE/2, point.y-ROI_SIZE/2, ROI_SIZE, ROI_SIZE);
	if(rect.x<2) rect=cvRect(2, rect.y, rect.width, rect.height);
	if(rect.y<2) rect=cvRect(rect.x, 2, rect.width, rect.height);
	if(rect.x > WIDTH || rect.y > HEIGHT) rect=cvRect(2,2,WIDTH,HEIGHT); 
	if(rect.x+rect.width>WIDTH) rect=cvRect(rect.x, rect.y, WIDTH-rect.x, rect.height);
	if(rect.y+rect.height>HEIGHT) rect=cvRect(rect.x, rect.y, rect.width, HEIGHT-rect.y);
	return rect;
}



