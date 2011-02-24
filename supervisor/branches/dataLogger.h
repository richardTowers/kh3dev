/*****************************************************************/
//  dataLogger.h
//	Created by Richard Towers at 14:21 on 23/02/2011
//	Copyright Durham University 2011, all rights reserved
//
/*****************************************************************/
#ifndef INC_DATALOGGER
#define INC_DATALOGGER

#include <stdio.h>

#include "cv.h"
#include "types.h"

#define BANNER_HEIGHT 110
#define NN_OFFSET 250
#define TEXT_HMARGIN 30
#define TEXT_LMARGIN 25
#define TEXT_LINE_SPACE 20

void createLogImage(FILE* logImage, short arenaWidth, short arenaHeight, CvLine* bounds, short nBounds);
void startLoggingPositions(FILE* logImage);
void logPosition(FILE* logImage, CvPoint position);
void logFinalPosition(FILE* logImage, CvPoint position, short markSize, char crashed, CvPoint* cherries, short nCherries);
void logFitnessData(FILE* logImage, short fitness, rtIndividual ind);
void addNetworkDiagram(FILE* logImage, char* filename);
void endSVG(FILE* logImage);

#endif //INC_DATALOGGER
