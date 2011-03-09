/*****************************************************************/
//  dataLogger.c
//	Created by Richard Towers at 14:21 on 23/02/2011
//	Copyright Durham University 2011, all rights reserved
//
/*****************************************************************/

#include "dataLogger.h"

void createLogImage(FILE* logImage, short arenaWidth, short arenaHeight, CvLine* bounds, short nBounds)
{
	short bound;
	if(logImage==NULL) {fprintf(stderr, "createLogImage: Passed NULL file pointer"); exit(1);}
	
	//Print header
	fprintf(logImage, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"no\"?>\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" height=\"%d\" width=\"%d\" version=\"1.1\" "
	"y=\"0\" x=\"0\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"  >\n\n", arenaHeight+BANNER_HEIGHT, arenaWidth);
	
	//Print defines:
	fprintf(logImage, "<defs>\n"
	"\t<filter id=\"monitorOffset\" filterUnits=\"userSpaceOnUse\">\n"
	"\t\t<feOffset in=\"SourceGraphic\" dx=\"0\" dy=\"%d\" result=\"output\"/>\n"
	"\t</filter>\n"
	"\t<filter id=\"networkOffset\" filterUnits=\"userSpaceOnUse\">\n"
	"\t\t<feOffset in=\"SourceGraphic\" dx=\"%d\" dy=\"0\" result=\"output\"/>\n"
	"\t</filter>\n"
	"</defs>\n", BANNER_HEIGHT, NN_OFFSET);

	//Print banner background:
	fprintf(logImage, "<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" style=\"fill:white; fill-opacity:1;\"/>\n\n",
	arenaWidth, BANNER_HEIGHT);
	fprintf(logImage, "<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" style=\"fill:black; fill-opacity:1;\"/>\n\n",
	NN_OFFSET, BANNER_HEIGHT);
	//Start monitor group:
	fprintf(logImage, "<g filter=\"url(#monitorOffset)\">\n");
	//Print monitor background:
	fprintf(logImage, "<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" style=\"fill:white; fill-opacity:1;\"/>\n\n",
	arenaWidth, arenaHeight);

	//Print boundaries:
	for (bound = 0; bound < nBounds; bound ++)
	{
		fprintf(logImage, "<line style=\"fill:none; stroke:red; stroke-width:2\" "
		"x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n\n", bounds[bound].start.x, bounds[bound].start.y, bounds[bound].end.x, bounds[bound].end.y);
	}
	
	return;
}

void startLoggingPositions(FILE* logImage)
{
	if(logImage==NULL) {fprintf(stderr, "startLoggingPositions: Passed NULL file pointer"); exit(1);}
	fprintf(logImage, "<polyline style=\"fill:none; stroke:green; stroke-width:1\" points=\"\n");
	return;
}

void logPosition(FILE* logImage, CvPoint position)
{
	if(logImage==NULL) {fprintf(stderr, "logPosition: Passed NULL file pointer"); exit(1);}
	fprintf(logImage, "%d,%d\n",position.x, position.y);
	return;
}

void logFinalPosition(FILE* logImage, CvPoint position, short markSize, char crashed, CvPoint* cherries, short nCherries)
{
	short cherry;
	if(logImage==NULL) {fprintf(stderr, "logFinalPosition: Passed NULL file pointer"); exit(1);}
	//Log final position and close polyline:
	fprintf(logImage, "%d,%d\n",position.x, position.y);
	fprintf(logImage, "\" />\n\n");
	
	//Show concentric circles where the robot stopped:
	fprintf(logImage, "<circle cx=\"%d\" cy=\"%d\" r=\"5\" fill=\"blue\" />\n", position.x, position.y);
	if(crashed)
		fprintf(logImage, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" stroke=\"red\" stroke-width=\"2\" fill=\"none\" />\n",
		position.x, position.y, markSize);
	else
		fprintf(logImage, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" stroke=\"black\" stroke-width=\"2\" fill=\"none\" />\n",
		position.x, position.y, markSize);
	
	//Show the remaining cherries:
	for (cherry = 0; cherry < nCherries; cherry ++)
		fprintf(logImage, "<circle cx=\"%d\" cy=\"%d\" r=\"1\" stroke=\"none\" fill=\"red\" />\n", cherries[cherry].x, cherries[cherry].y);
	//End monitor group:
	fprintf(logImage, "</g>\n");
	return;
}

void logFitnessData(FILE* logImage, short fitness, rtIndividual ind)
{
	if(logImage==NULL) {fprintf(stderr, "logFitnessData: Passed NULL file pointer"); exit(1);}
	
	fprintf(logImage, "<text x=\"%d\" y=\"%d\" font-family=\"sawasdee\" font-size=\"22\" fill=\"white\" font-weight=\"bold\" > Gen%dInd%d </text>\n",
	TEXT_HMARGIN, TEXT_LINE_SPACE, ind.generation, ind.number);
	if(ind.parent!=NULL)
		fprintf(logImage, "<text x=\"%d\" y=\"%d\" font-family=\"sawasdee\" font-size=\"14\" fill=\"white\""
		" font-weight=\"bold\" > Parent: Gen%dInd%d </text>\n",
		TEXT_LMARGIN, 2*TEXT_LINE_SPACE, (*(ind.parent)).generation, (*(ind.parent)).number);
	else
		fprintf(logImage, "<text x=\"%d\" y=\"%d\" font-family=\"sawasdee\" font-size=\"14\" fill=\"white\""
		" font-weight=\"bold\" > Parent: None </text>\n", TEXT_LMARGIN, 2*TEXT_LINE_SPACE);
	fprintf(logImage, "<text x=\"%d\" y=\"%d\" font-family=\"sawasdee\" font-size=\"14\" fill=\"red\" > Fitness: %d </text>\n",
	TEXT_LMARGIN, 3*TEXT_LINE_SPACE, fitness);
/*	fprintf(logImage, "<text x=\"%d\" y=\"%d\" font-family=\"sawasdee\" font-size=\"14\" fill=\"white\" > Distance Travelled: %d px</text>\n");*/
/*	fprintf(logImage, "<text x=\"%d\" y=\"%d\" font-family=\"sawasdee\" font-size=\"14\" fill=\"white\" > Cherries Eaten: %d </text>\n");*/
	return;
}

void addNetworkDiagram(FILE* logImage, char* filename)
{
}

void endSVG(FILE* logImage)
{
	fprintf(logImage, "\n</svg>");
}
