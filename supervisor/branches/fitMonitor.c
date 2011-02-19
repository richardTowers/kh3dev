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
	CvLine *tmp;
	
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
			"Press C to change your selection\n",bot+1);
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

	printf("Got targets...\n\n"
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
	printf("Got Boundaries...\n\n");
	
	cvReleaseCapture(&capture);
}

void testIndividualOnRobot(rtIndividual* individual, rtRobot robot)
{
	IplImage* frame, *histogram;
	CvCapture* capture; CvPoint maxloc, origin; CvSize hSize; CvFont font;
	FILE* logImage; char lFileName[200];
	int dFromLastPoint=0, dFromOrigin=0, dTravelled=0, dFit=0, dToNearestBound=INT_MAX, dToBound, bound, fitness=0;
	char line1[100], line2[100], line3[100], line4[100];
	time_t startTime, retreatTime;
	CvPoint *cherries, point, topLeft={9999,9999}, bottomRight={0,0};
	short nCherries=0, cherry=0;
	
	//The objective will be to eat as many cherries as possible
	//Get corners of bounds
	for (bound = 0; bound < nBounds; bound ++)
	{
		if(bounds[bound].start.x<topLeft.x) topLeft.x=bounds[bound].start.x;
		if(bounds[bound].start.y<topLeft.y) topLeft.y=bounds[bound].start.y;
		if(bounds[bound].start.x>bottomRight.x) bottomRight.x=bounds[bound].start.x;
		if(bounds[bound].start.y>bottomRight.y) bottomRight.y=bounds[bound].start.y;
		if(bounds[bound].end.x<topLeft.x) topLeft.x=bounds[bound].end.x;
		if(bounds[bound].end.y<topLeft.y) topLeft.y=bounds[bound].end.y;
		if(bounds[bound].end.x>bottomRight.x) bottomRight.x=bounds[bound].end.x;
		if(bounds[bound].end.y>bottomRight.y) bottomRight.y=bounds[bound].end.y;
	}
	//Layout a 30*30 (ish) grid of cherries:
	for (point.x = topLeft.x; point.x < bottomRight.x; point.x += CHERRY_SPACE)
		for (point.y = topLeft.y; point.y < bottomRight.y; point.y += CHERRY_SPACE) nCherries++;
	cherries=(CvPoint*)malloc(sizeof(CvPoint)*nCherries);
	for (point.x = topLeft.x; point.x < bottomRight.x; point.x += CHERRY_SPACE)
	{
		for (point.y = topLeft.y; point.y < bottomRight.y; point.y += CHERRY_SPACE, cherry++)
		{
			cherries[cherry]=point;
		}
	}
	
	startTime=time(NULL);
	//Create Log File:
		sprintf(lFileName, "%s/Gen%dInd%d.svg",logFolder, (*individual).generation, (*individual).number);
	
		logImage=fopen(lFileName, "w+");
		if(!logImage)
		{
			fprintf(stderr, "Failed to open file...");
			exit(1);
		}
	
	//Start Camera
	capture=cvCaptureFromCAM(ANY_CAMERA);
	cvNamedWindow("Fitness Monitor",0);
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 0.5,0.5,0,LINE_WIDTH/3);
	
	printf("Testing individual with genome: %s on robot with IP: %s\n", (*individual).geneFile, robot.ip);
	
	//Find initial position
	frame=cvQueryFrame(capture);
	hSize=cvSize(frame->width-robot.mark->width+1,frame->height-robot.mark->height+1);
	histogram = cvCreateImage(hSize, IPL_DEPTH_32F,1);
	startTime=time(NULL);
	while(time(NULL)-startTime < 4)
	{
		keyHandler();
		frame=cvQueryFrame(capture);
		
		//Get position
		cvMatchTemplate(frame, robot.mark, histogram, CV_TM_CCOEFF);
		cvNormalize(histogram, histogram, 1, 0, CV_MINMAX);
		cvMinMaxLoc(histogram, NULL, NULL, NULL, &maxloc, 0);
		maxloc.x+=robot.mark->width/2; maxloc.y+=robot.mark->height/2;
		//Plot position
		cvCircle(frame, maxloc, MARK_SIZE, YELLOW, LINE_WIDTH, CV_AA);
		cvPutText(frame,"Finding Initial Position...",cvPoint(10,20), &font, WHITE);
		cvShowImage("Fitness Monitor",frame);
	}

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
	
	startTime=time(NULL);
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
		dFromLastPoint=pointToPoint(robot.currPos,robot.prevPos);
		dTravelled+=dFromLastPoint;
		dFromOrigin=pointToPoint(robot.currPos,origin);
		
		//Calculate minimum distance from obstacles
		for (bound = 0; bound < nBounds; bound ++)
		{
			dToBound=pointToLine(robot.currPos, bounds[bound].start, bounds[bound].end);
			if(dToBound<dToNearestBound) dToNearestBound=dToBound;
		}
		if(dToNearestBound<120) dFit+=dFromLastPoint;
		
		//If we're closer than 60 px to an object we're basically about to crash...
		//Use the "retreat" neural network to get out of trouble.
		if (dToNearestBound<60)
		{
			//Stop motors
			send(robot.socket, "Stop Motors", 11, 0);
			//Finish up log file:
			fprintf(logImage, "\" />\n\n");
			for (cherry = 0; cherry < nCherries; cherry ++)
			{
				fprintf(logImage, "<circle cx=\"%d\" cy=\"%d\" r=\"1\" stroke=\"none\" fill=\"red\" />\n", cherries[cherry].x, cherries[cherry].y);
			}
			fprintf(logImage, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" />\n", robot.currPos.x, robot.currPos.y, MARK_SIZE);
			fprintf(logImage, "\n</svg>");
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
				cvPutText(frame, "Crashed, Retreating...", cvPoint(10,15), &font, BLUE);
				cvPutText(frame, "CRASH!", robot.currPos, &font, RED);
				cvShowImage("Fitness Monitor", frame);
			}
			//Give fitness equal to whatever fitness it had before the crash
			(*individual).fitness=fitness;//3*dFit+dFromOrigin;
			cvReleaseCapture(&capture);
			return;
		}
		//Write data:
		sprintf(line1, "Distance from origin: %d px",dFromOrigin);
		sprintf(line2, "Total Distance: %d px", dTravelled);
		sprintf(line3, "Distance to nearest bound: %d px", dToNearestBound);
		sprintf(line4, "Fitness: %d", fitness);//3*dFit+dFromOrigin);
		cvPutText(frame,line1,cvPoint(10,15), &font, WHITE);
		cvPutText(frame,line2,cvPoint(10,30), &font, WHITE);
		cvPutText(frame,line3,cvPoint(10,45), &font, WHITE);
		cvPutText(frame,line4,cvPoint(10,60), &font, RED);
		//Remove cherries:
		for (cherry = 0; cherry < nCherries; cherry ++)
		{
			if(pointToPoint(robot.currPos,cherries[cherry]) < MARK_SIZE)
			{
				removeFromArray(cherries, &nCherries, cherry);
				fitness++;
			}
		}
		//show cherries:
		for (cherry = 0; cherry < nCherries; cherry ++) cvCircle(frame, cherries[cherry], 1, RED, -1, CV_AA);
		cvShowImage("Fitness Monitor",frame);
	}
	//Timer ran down, we should have all the stuff needed to calculate a fitness
	//Finish up log file:
	fprintf(logImage, "\" />\n\n");
	for (cherry = 0; cherry < nCherries; cherry ++)
	{
		fprintf(logImage, "<circle cx=\"%d\" cy=\"%d\" r=\"1\" stroke=\"none\" fill=\"red\" />\n", cherries[cherry].x, cherries[cherry].y);
	}
	fprintf(logImage, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" />\n", robot.currPos.x, robot.currPos.y, MARK_SIZE);
	fprintf(logImage, "\n</svg>");
	
	fclose(logImage);
	//Store fitness:
	(*individual).fitness=fitness+SURVIVAL_BONUS;//3*dFit+dFromOrigin+SURVIVAL_BONUS;
	//Retreat to sensible position:
	//Stop motors
	send(robot.socket, "Stop Motors", 11, 0);
	//Wait for the message to sink in:
	sleep(1);
	//Use retreat genotype:
	send(robot.socket, "Genotypes/Retreat.txt", 22, 0);
	retreatTime=time(NULL);
	while((time(NULL)-retreatTime) < 12)
	{
		keyHandler();
		frame = cvQueryFrame(capture);
		cvPutText(frame, "Finished Evaluation, Retreating...", cvPoint(10,15), &font, BLUE);
		cvShowImage("Fitness Monitor", frame);
	}
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

int pointToPoint(CvPoint p1, CvPoint p2) {return (int)hypot(p1.x-p2.x,p1.y-p2.y);}

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
	static char buffer[300];
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

CvPoint* removeFromArray(CvPoint* array, short *length, const short index)
{
	short i;
	if(index>*length || index < 0) {fprintf(stderr, "removeFromArray(): Index out of bounds\n"); return array;}
	else
	{
		*length=*length-1;
		for(i = index; i < *length; i++) *(array+i) = *(array+(i+1));
	}
	return (CvPoint*)realloc(array, sizeof(CvPoint)*(*length));
}
