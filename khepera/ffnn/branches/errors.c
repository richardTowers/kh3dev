/*****************************************************************/
/*  errors.c
/*	Created by richard at 10:33 on 08/11/2010
/*	Copyright Durham University 2010, all rights reserved
/*
/*****************************************************************/

#include "errors.h"

void startLoggingErrors(void)
{
	errorFile=fopen("ErrorLog.txt","w+");
}

void stopLoggingErrors(void)
{
	fclose(errorFile);
}

const char* errorLookup(int errorCode)
{
	static const char * errors[6]=
	{
		"No Error", "Unspecified Error", "Malloc could not allocate memory", "Unable to initialise motor", "Unable to initialise khepera",
		"Failed to get IR ranges"
	};
	return errors[errorCode];
}

void error(int errorCode)
{
	fprintf(errorFile, "Exited with error code %d, \"%s\"\n", errorCode, errorLookup(errorCode));
	stopLoggingErrors();
	exit(EXIT_FAILURE);
}

void warning(int errorCode)
{
	fprintf(errorFile, "Warning: \"%s\"\n", errorLookup(errorCode));
}

