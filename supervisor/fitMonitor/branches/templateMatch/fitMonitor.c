/*****************************************************************/
//  fitMonitor.c
//	Created by richard at 10:51 on 30/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "fitMonitor.h"


int main( void )
{
	printf("Main window accepts keyboard and mouse input:\n"
	"  ESC - Quit\n"
	"  Mouse - Drag to Select Area\n"
	"  Return - Confirm Area\n");
	
	mainCapture();
}

void mainCapture(void)
{
	CvCapture *capture = 0;
	IplImage *frame = 0, *histogram = 0;
	rtRobotPosition robots[NUMBER_ROBOTS];
	int robot=0, roundel=0;
	
	//Start capturing:
	if( !(capture = cvCaptureFromCAM(ANY_CAMERA)))
	{
		fprintf(stderr,"Could not initialize capturing...\n");
		exit(1);
	}
	cvNamedWindow("Main");
	cvSetMouseCallback("Main",mouseHandler,NULL);
	
	
	while(robot<NUMBER_ROBOTS)
	{
		while(roundel<2)
		{
			keyHandler();
			frame = cvQueryFrame( capture );
			if( !frame ) {fprintf(stderr,"\nError: Couldn\'t grab frame.\n"); exit(1);}

			cvRectangle(frame, rectStart, rectEnd, RED, 1, CV_AA, 0);
			theSemaphor=GOTRECT;
			if(gotRect==YES)
			{
				gotRect=NO;
				//Clear the frame
				frame = cvQueryFrame( capture );
				//Shrink the image
				cvSetImageROI(frame, fullRect);
				cvShowImage("Main", frame);
				
				printf("\a/************************************/"
				"\nYou should be looking at a small image of:"
				"\nROBOT: %d\nROUNDLE: %d (1 front, 2 back)\n\nPress Return if you are happy with your selection\n"
				"Press C to change your selection\n\n",robot+1, roundel+1);
				
				for(;;)
				{
					keyHandler();
					theSemaphor=SELECTIONORWINDOW;
					if(selectionMade)
					{
						selectionMade=NO;
						if(roundel==0)
						{
							robots[robot].marks.front=cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
							cvCopy(frame, robots[robot].marks.front, 0);
							roundel++;
							break;
						}
						else if(roundel==1)
						{
							robots[robot].marks.rear=cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
							cvCopy(frame, robots[robot].marks.rear, 0);
							roundel++;
							robot++;
							break;
						}
						else roundel=0;
					}
					if(windowNeedsReset)
					{
						windowNeedsReset=NO;
						break;
					}
				}
				cvResetImageROI(frame);
			}
			cvShowImage("Main", frame);
		}
		roundel=0;
	}
	
	//At this point all of the targets have been acquired and we can get on with following them.
	printf("\n/************************************/"
	"\nTarget Images obtained, beginning tracking...\n");
	CvPoint maxloc;
	theSemaphor=ESC_ONLY;
	
	cvNamedWindow("Histogram");
	for(;;)
	{
		keyHandler();
		frame = cvQueryFrame( capture );
		if( !frame ) {fprintf(stderr,"\nError: Couldn\'t grab frame.\n"); exit(1);}
		for (robot = 0; robot < NUMBER_ROBOTS; robot += 1)
		{
			for (roundel = 0; roundel < 2; roundel += 1)
			{
				if(roundel==0)
				{
					histogram = cvCreateImage(
					cvSize(frame->width-robots[robot].marks.front->width+1,frame->height-robots[robot].marks.front->height+1),
					IPL_DEPTH_32F,
					1);
					cvMatchTemplate(frame, robots[robot].marks.front, histogram, CV_TM_CCORR);
					cvNormalize( histogram, histogram, 1, 0, CV_MINMAX );

					cvShowImage("Histogram", histogram);
					cvMinMaxLoc(histogram, NULL, NULL, NULL, &maxloc, 0);
					
				}
				//else cvMatchTemplate(frame, robots[robot].marks.rear, histogram, CV_TM_SQDIFF);
				
			}
		}
		
		cvShowImage("Main", frame);
	}
	
}


/*****************************************EVENT HANDLERS*********************************/
void keyHandler(void)
{
	char keyPress;
	keyPress=cvWaitKey(5);
	switch(keyPress)
        {
        	case ESC:
        		exit(0);
        	case '\n':
        		if(theSemaphor==GOTRECT)
        		{
        			if(rectStart.x!=rectEnd.x && rectStart.y!=rectEnd.y) gotRect=YES;
        			else printf("Please select a robot\n");
        		}
        		else if(theSemaphor==SELECTIONORWINDOW)	selectionMade=YES;
        		break;
        	case 'c':
        		if(theSemaphor==SELECTIONORWINDOW) windowNeedsReset=YES;
        		break;
        	default:
        		break;
        }
}
void mouseHandler(int event, int x, int y, int flags, void* param)
{
	switch(event){
		case CV_EVENT_LBUTTONDOWN:
			mouseDown=YES;
			rectStart=cvPoint(x,y);
			break;

		case CV_EVENT_LBUTTONUP:
			mouseDown=NO;
			rectEnd=cvPoint(x,y);
			fullRect=cvRect(min(rectStart.x, rectEnd.x),
			min(rectStart.y,rectEnd.y),abs(rectEnd.x - rectStart.x),abs(rectEnd.y - rectStart.y) );
			break;

		case CV_EVENT_MOUSEMOVE:
			if(mouseDown) rectEnd=cvPoint(x,y);
			break;

	}
}
