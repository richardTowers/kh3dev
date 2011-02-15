/*****************************************************************/
//  fitMonitor.c
//	Created by richard at 10:51 on 30/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "fitMonitor.h"


void setupTracker(int nRobots, rtRobot* robots)
{
	IplImage* frame;
	CvCapture* capture;
	int bot=0, bound=0;
	char timeStr[30], makeDirectory[100];
	time_t progStartTime;
	struct tm *localTime;
	unsigned short int i;
	CvLine *tmp;
	//Create Log Folder:
		progStartTime=time(NULL);
		localTime=localtime(&progStartTime);
		strcpy(timeStr,asctime(localTime));
		for (i = 0; i < strlen(timeStr); i += 1)
		{
			if(timeStr[i]==' ') timeStr[i]='-';
			else if(timeStr[i]=='\n') timeStr[i]='\0';
		}
		sprintf(lFolderName, "LogFiles/%s", timeStr);
		printf("%s\n",lFolderName);
		sprintf(makeDirectory, "mkdir %s", lFolderName);
		printf("%s\n",makeDirectory);
		
		if(access("./LogFiles",F_OK)!=0) system("mkdir LogFiles");
		if(access(lFolderName,F_OK)!=0) system(makeDirectory);
	
	//Start capturing:
	if( !(capture = cvCaptureFromCAM(ANY_CAMERA)))
	{
		fprintf(stderr,"Could not initialize capturing...\n");
		exit(1);
	}
	cvNamedWindow("Select Robots",0);
	cvSetMouseCallback("Select Robots",mouseHandler,NULL);
	
	
	//Now we stream frames and collect the images to track
	
	while(bot<nRobots)	//Get image to track
	{
		keyHandler();
		frame=cvQueryFrame(capture);
		//The mouse is collecting a rectangle, draw it:
		cvRectangle(frame, rectStart, rectEnd, RED, 1, CV_AA, 0);
		//Show the image:
		cvShowImage("Select Robots", frame);
		
		uiAction=getRect;
		if(gotRect==YES)	//This is set in the key handler...
		{
			gotRect=NO;
			cvDestroyAllWindows();
			cvNamedWindow("Is this your robot?",0);
			//Clear the frame of the rectangle we just drew
			frame = cvQueryFrame( capture );
			//Clip to the image we just outlined with the rectangle
			cvSetImageROI(frame, fullRect);
			//Show the cropped image:
			cvShowImage("Is this your robot?", frame);
			
			printf("\nYou should be looking at a small image of robot %d\n\nPress Return if you are happy with your selection\n"
			"Press C to change your selection\n\n",bot+1);
			//Wait for user to make choice:
			uiAction=acceptOrReset;
			for(;;)
			{
				keyHandler();
				if(selectionMade)
				{
					selectionMade=NO;
					robots[bot].mark=cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
					cvCopy(frame, robots[bot].mark, 0);
					bot++;
					break;
				}
				if(windowNeedsReset)
				{
					windowNeedsReset=NO;
					cvNamedWindow("Select Robots");
					cvSetMouseCallback("Select Robots", mouseHandler, NULL);
					break;
				}
			}
			cvResetImageROI(frame);
			rectStart=cvPoint(0,0); rectEnd=cvPoint(0,0);
		}
	}
	cvDestroyWindow("Is this your robot?");

	printf("\nGot targets...\n"
	"Please draw straight lines around the bounds and any obstacles.\n"
	"Press \'+\' to add a line, \'-\' to remove a line\n");
	cvNamedWindow("Select Boundaries",0);
	cvSetMouseCallback("Select Boundaries",mouseHandler,NULL);
	
	bounds=(CvLine*)malloc(sizeof(CvLine)*nBounds);
	for(;;)
	{
		keyHandler();
		frame=cvQueryFrame(capture);
		cvShowImage("Select Boundaries", frame);
		uiAction=getLine;
		//The mouse is collecting a line, draw it:
		cvLine(frame, rectStart, rectEnd, BLUE, 1, CV_AA, 0);
		//Draw the other lines
		for (bound = 0; bound < nBounds; bound ++)
		{
			cvLine(frame, bounds[bound].start, bounds[bound].end, GREY, 1, CV_AA, 0);
		}
		//Show the image:
		cvShowImage("Select Boundaries", frame);
		if(gotLine)
		{
			gotLine=NO;
			//expand bounds
			nBounds++;
			tmp = (CvLine*)realloc(bounds, nBounds*sizeof(CvLine));
			if (tmp != NULL) bounds = tmp;
			else perror("malloc");
			//add line
			bounds[nBounds-1].start=rectStart;
			bounds[nBounds-1].end=rectEnd;
		}
		if(undoLine)
		{
			undoLine=NO;
			//Shrink bounds
			nBounds--;
			if(nBounds<=0) {nBounds=0; if(bounds!=NULL) {bounds=NULL;free(bounds);}}
			else
			{
				tmp = (CvLine*)realloc(bounds, nBounds*sizeof(CvLine));
				if (tmp != NULL) bounds = tmp;
				else perror("malloc");
			}
		}
		if(selectionMade)
		{
			selectionMade=NO;
			break;
		}
	}
	//cvReleaseImage(&frame);
	cvDestroyWindow("Select Boundaries");
	printf("Got Boundaries...\n");
	
	cvReleaseCapture(&capture);
}

void testIndividualOnRobot(rtIndividual* individual, rtRobot robot)
{
	IplImage* frame, *histogram;
	CvCapture* capture; CvPoint maxloc, origin; CvSize hSize; CvFont font;
	FILE* logImage; char lFileName[30], openFile[200];
	int dFromLastPoint=0, dFromOrigin=0, dTravelled=0, dToNearestBound=INT_MAX, dToBound, bound; char line1[100], line2[100], line3[100];
	//int health=0xffff, fitness=0;
	time_t startTime, retreatTime;
	
	startTime=time(NULL);
	//Create Log File:
		strcpy(lFileName, replace_str(replace_str((*individual).geneFile, ".txt", ".svg"), "Genotypes/", ""));
		printf("%s\n",lFileName);
		sprintf(openFile, "%s/%s", lFolderName, lFileName);
		printf("%s\n\n",openFile);
	
		logImage=fopen(openFile, "w+");
		
	
	//Start Camera
	capture=cvCaptureFromCAM(ANY_CAMERA);
	cvNamedWindow("Fitness Monitor",0);
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 0.5,0.5,0,LINE_WIDTH/3);
	
	printf("Testing individual with genome: %s on robot with IP: %s\n", (*individual).geneFile, robot.ip);
	
	//Grab a frame for the initial position:
	frame=cvQueryFrame(capture);
	
	//Find initial position
	hSize=cvSize(frame->width-robot.mark->width+1,frame->height-robot.mark->height+1);
	histogram = cvCreateImage(hSize, IPL_DEPTH_32F,1);
	
	cvMatchTemplate(frame, robot.mark, histogram, CV_TM_CCOEFF);
	cvNormalize(histogram, histogram, 1, 0, CV_MINMAX);
	cvMinMaxLoc(histogram, NULL, NULL, NULL, &maxloc, 0);
	
	maxloc.x+=robot.mark->width/2; maxloc.y+=robot.mark->height/2;

	robot.currPos=maxloc;
	robot.prevPos=maxloc;
	origin=maxloc;
	
	//Log the status at the beginning of the run:
		//Print header
		fprintf(logImage, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"no\"?>\n"
		"<svg xmlns=\"http://www.w3.org/2000/svg\" height=\"%d\" width=\"%d\" version=\"1.1\" "
		"y=\"0\" x=\"0\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"  >\n\n", frame->height, frame->width);
	
		//Print background rectangle
		fprintf(logImage, "<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" style=\"fill:white; fill-opacity:1;\"/>\n\n", frame->width, frame->height);
	
		//Print boundaries:
		for (bound = 0; bound < nBounds; bound ++)
		{
			fprintf(logImage, "<line style=\"fill:none; stroke:red; stroke-width:2\" "
			"x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n\n", bounds[bound].start.x, bounds[bound].start.y, bounds[bound].end.x, bounds[bound].end.y);
		}
		
		fprintf(logImage, "<polyline style=\"fill:none; stroke:green; stroke-width:1\" "
		"points=\"\n");
	
	while(time(NULL)-startTime < EVAL_TIME)
	{	
		keyHandler();
		frame=cvQueryFrame(capture);
		dToNearestBound=INT_MAX;
		//Get position
		cvMatchTemplate(frame, robot.mark, histogram, CV_TM_CCOEFF);
		cvNormalize(histogram, histogram, 1, 0, CV_MINMAX);
		cvMinMaxLoc(histogram, NULL, NULL, NULL, &maxloc, 0);
		maxloc.x+=robot.mark->width/2; maxloc.y+=robot.mark->height/2;
		robot.prevPos=robot.currPos;
		robot.currPos=maxloc;
		//Plot position and bounds
		cvCircle(frame, robot.currPos, MARK_SIZE, YELLOW, LINE_WIDTH, CV_AA);
		fprintf(logImage, "%d,%d\n",robot.currPos.x, robot.currPos.y);
		
		for (bound = 0; bound < nBounds; bound ++)
		{
			cvLine(frame, bounds[bound].start, bounds[bound].end, RED, 1, CV_AA, 0);
		}
		
		//Calculate distance moved from last position, orgin and total:
		dFromLastPoint=(int)hypot(robot.currPos.x-robot.prevPos.x,robot.currPos.y-robot.prevPos.y);
		dTravelled+=dFromLastPoint;
		dFromOrigin=(int)hypot(robot.currPos.x-origin.x,robot.currPos.y-origin.y);
		
		//Calculate minimum distance from obstacles
		for (bound = 0; bound < nBounds; bound ++)
		{
			dToBound=pointToLine(robot.currPos, bounds[bound].start, bounds[bound].end);
			if(dToBound<dToNearestBound) dToNearestBound=dToBound;
		}
		
		//If we're closer than 60 px to an object we're basically about to crash...
		//Use the "retreat" neural network to get out of trouble.
		if (dToNearestBound<60)
		{
			//Stop motors
			send(robot.socket, "Stop Motors", 11, 0);
			//Finish up log file:
			fprintf(logImage, "\" />\n\n</svg>");
			fclose(logImage);
			//Wait for the message to sink in:
			sleep(1);
			//Use retreat genotype:
			send(robot.socket, "Genotypes/Retreat.txt", 22, 0);
			retreatTime=time(NULL);
			while((time(NULL)-retreatTime) < 12)
			{
				keyHandler();
				frame = cvQueryFrame(capture);
				cvPutText(frame, "Retreating...", cvPoint(10,10), &font, BLUE);
				cvPutText(frame, "CRASH!", robot.currPos, &font, RED);
				cvShowImage("Fitness Monitor", frame);
			}
			//Give fitness equal to distance it travelled before the crash
			(*individual).fitness=dTravelled;
			cvReleaseCapture(&capture);
			return;
		}
		//Write data:
		sprintf(line1, "Distance from origin: %d px",dFromOrigin);
		sprintf(line2, "Total Distance: %d px", dTravelled);
		sprintf(line3, "Distance to nearest bound: %d px", dToNearestBound);
		cvPutText(frame,line1,cvPoint(10,15), &font, WHITE);
		cvPutText(frame,line2,cvPoint(10,30), &font, WHITE);
		cvPutText(frame,line3,cvPoint(10,45), &font, WHITE);
		
		//Store stuff we can later use for fitness
		
		
		cvShowImage("Fitness Monitor",frame);
	}
	//Timer ran down, we should have all the stuff needed to calculate a fitness
	//Finish up log file:
	fprintf(logImage, "\" />\n\n</svg>");
	fclose(logImage);
	//Store fitness:
	(*individual).fitness=dTravelled+SURVIVAL_BONUS;
	//Clear up:
	cvReleaseCapture(&capture);
}

/*****************************************GEOMETRY*********************************/
int pointToLine(CvPoint point, CvPoint start, CvPoint end)
{
	int distanceLine;
    double r_numerator = (double)((point.x-start.x)*(end.x-start.x) + (point.y-start.y)*(end.y-start.y));
    double r_denomenator = (double)((end.x-start.x)*(end.x-start.x) + (end.y-start.y)*(end.y-start.y));
    double r = r_numerator / r_denomenator;
   
    double s =  ((start.y-point.y)*(end.x-start.x)-(start.x-point.x)*(end.y-start.y) ) / r_denomenator;

    distanceLine = fabs(s)*sqrt(r_denomenator);

    if ( (r >= 0) && (r <= 1) ) return distanceLine;
    else
    {
        double dist1 = (point.x-start.x)*(point.x-start.x) + (point.y-start.y)*(point.y-start.y);
        double dist2 = (point.x-end.x)*(point.x-end.x) + (point.y-end.y)*(point.y-end.y);
        if (dist1 < dist2) return sqrt(dist1);
	    else return sqrt(dist2);
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
        		if(uiAction==getRect)
        		{
        			if(rectStart.x!=rectEnd.x && rectStart.y!=rectEnd.y) gotRect=YES;
        			else printf("Please select a robot\n");
        		}
        		else if(uiAction==acceptOrReset)	selectionMade=YES;
        		else if(uiAction==getLine) selectionMade=YES;
        		break;
        	case '+':
        		if(uiAction==getLine) gotLine=YES;
        		break;
        	case '-':
        		if(uiAction==getLine) undoLine=YES;
        		break;
        	case 'c':
        		if(uiAction==acceptOrReset) windowNeedsReset=YES;
        		break;
        	default:
        		break;
        }
}
void mouseHandler(int event, int x, int y, int flags, void* param)
{
	switch(event)
	{
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

/*****************************************HELPERS*********************************/
char *replace_str(const char *str, const char *orig, const char *rep)
{
	static char buffer[100];
	const char *p;

	if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
	{
		strcpy(buffer,str);
		return buffer;
	}
	strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
	buffer[p-str] = '\0';

	sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));
	return buffer;
}
